
#include "Data.h"

#include <stdio.h>
#include <string.h>

#include <DriverKit/IOLib.h>
#include <DriverKit/IOUserServer.h>
#include <DriverKit/OSDictionary.h>
#include <DriverKit/OSCollections.h>
#include <DriverKit/IOService.h>


#include <NetworkingDriverkit/IOUserNetworkPacketBufferPool.iig>
#include <NetworkingDriverkit/IOUserNetworkRxSubmissionQueue.h>
#include <NetworkingDriverkit/IOUserNetworkRxCompletionQueue.h>
#include <NetworkingDriverkit/IOUserNetworkTxSubmissionQueue.h>
#include <NetworkingDriverkit/IOUserNetworkTxCompletionQueue.h>

#include <os/log.h>

#define LOG_PREFIX "com.belcarra.testusbnet"

static char                    dID[4] = "--";

enum {
	RXSubmissionQ = 0,
	RXCompletionQ = 1,
	TXSubmissionQ = 2,
	TXCompletionQ = 3
};

struct Data_IVars {

    IOService               *provider;
    OSDictionaryPtr         properties;

    IOUserNetworkPacketBufferPool *pool;
    IOUserNetworkPacketQueue *queues[4];

    IOUSBHostInterface      *hostInterface;

    const IOUSBDeviceDescriptor *deviceDescriptor;
    const IOUSBConfigurationDescriptor *configurationDescriptor;
    const IOUSBInterfaceDescriptor *firstInterfaceDescriptor;
    const IOUSBInterfaceDescriptor *interfaceDescriptorComm;
    const IOUSBInterfaceDescriptor *interfaceDescriptorData;
    const IOUSBInterfaceDescriptor *interfaceDescriptorNoData;

};

#define VDBG(lvl,flg,fmt,args...)  {if (flg >= lvl) {IOLog(VDBG_ID "::%s " fmt "\n", __FUNCTION__, ##args); /*IOSleep(Sleep_Time);*/}}
#define VDBG_fe(lvl,fmt,args...) VDBG(lvl,dbg_fn_entry,fmt,##args)


#define DLOG(fmt,args...) os_log(OS_LOG_DEFAULT, "[%s]" LOG_PREFIX "::%s " fmt, dID, __FUNCTION__, ##args)

#define LOG(lvl,fmt,args...) os_log(OS_LOG_DEFAULT, "[%s]" LOG_PREFIX "::%s " fmt, dID, __FUNCTION__, ##args)

#define DEFAULT(fmt,args...) LOG(OS_LOG_DEFAULT, fmt, ##args)
#define INFO(fmt,args...) LOG(OS_LOG_INFO, fmt, ##args)
#define DEBUG(fmt,args...) LOG(OS_LOG_DEBUG, fmt, ##args)
#define ERROR(fmt,args...) LOG(OS_LOG_ERROR, fmt, ##args)
#define FAULT(fmt,args...) LOG(OS_LOG_ERROR, fmt, ##args)

/* LOG Macros
 * This provides a compact test for the required calls to initialize the driver.
 * Three paradigms:
 *
 *      Test for false: if (_ISFALSE(b, SomeSystemCallReturningBool())) { failure }
 *      Test for error: if (_ISERROR(b, result, SomeSystemCallReturningkern_return_t())) { failure }
 *      Test for null:  if (_ISNULL(b, pointer, SomeSystemCallReturningPtr())) { failure }
 *
 * In the above result and pointer variables will have the result of the function call.
 *
 * The boolean b variable will have the result of the test for non-zero or non-null. 
 *
 * As a side-effect, the result of the expression will be logged:
 *
 *          .... com.belcarra.data::Start_Impl: Start(provider, SUPERDISPATCH) OK: 0x0
 */

#define _ISFALSE(b, e)  (b = e, DLOG("%{public}s %{public}s", #e, b ? "True":"False"), !b)
#define _ISNULL(b, r, e)  (b = (r = e), DLOG("%{public}s = %{public}s %{public}s %{public}p", #r, #e, b ? "OK":"NOT OK", r), !b)
#define _ISERROR(b, r, e) (b = (r = e), DLOG("%{public}s = %{public}s %{public}s %x %{errno}x", #r, #e, !b ? "OK":"NOT ZERO", r, r), b) 


struct IOUserNetworkMACAddress DataMacAddress = {
    2, 0, 0, 0, 0, 1
};

bool Data::init() {

    /* This puts the compile date and time into the log so we can verify extension being loaded is correct.
     */
    DEFAULT(__DATE__ " " __TIME__);

    /* call our super::init() and then zero the ivars struct
     */
    bool b;
    if (_ISFALSE(b, super::init())) { return false; }
    if (_ISNULL(b, ivars, IONewZero(Data_IVars, 1))) { return false; }

    return true;
}


/* ReleaseResources - free anything Start may have initialized in ivars
 */
void IMPL(Data, ReleaseResources) 
{
	DEFAULT("");

    if (ivars->hostInterface) ivars->hostInterface->Close(this, 0);

    /* Free in reverse order
     */
    if (ivars->pool) ivars->pool->free();
	for (int i = 0; i <= TXCompletionQ; i++)
		if (ivars->queues[i]) 
            ivars->queues[i]->free();

}

/* StartFailed - called from Start for failure, will free allocated resources and then return result
 */
kern_return_t IMPL(Data, StartFailed) 
{
    DLOG("Failing result: %x %{errno}x", result, result);
    ReleaseResources();
    return result;
}

kern_return_t IMPL(Data, Start) 
{
    DLOG("Start %s %s", __DATE__, __TIME__);

    ivars->provider = provider;
    kern_return_t result;

    bool b;

    if (_ISERROR(b, result, Start(provider, SUPERDISPATCH))) { Stop(provider, SUPERDISPATCH); return result; }

    if (_ISNULL(b, ivars->hostInterface, OSDynamicCast(IOUSBHostInterface, provider))) return StartFailed(kIOReturnNotFound);

#if 0
	if (_ISERROR(b, result, IOUserNetworkPacketBufferPool::Create(this, "test pool", 16, 16, 1600, &ivars->pool))) 
        return StartFailed(result); 

	if (_ISERROR(b, result, IOUserNetworkRxSubmissionQueue::Create(ivars->pool, this, 8, 0, NULL, 
                    (IOUserNetworkRxSubmissionQueue **)&ivars->queues[RXSubmissionQ]))) 
		return StartFailed(result); 
	if (_ISERROR(b, result, IOUserNetworkRxCompletionQueue::Create(ivars->pool, this, 8, 0, NULL, 
                    (IOUserNetworkRxCompletionQueue **)&ivars->queues[RXSubmissionQ]))) 
		return StartFailed(result); 
    
	if (_ISERROR(b, result, IOUserNetworkTxSubmissionQueue::Create(ivars->pool, this, 8, 0, NULL, 
                    (IOUserNetworkTxSubmissionQueue **)&ivars->queues[RXSubmissionQ]))) 
		return StartFailed(result); 
	if (_ISERROR(b, result, IOUserNetworkTxCompletionQueue::Create(ivars->pool, this, 8, 0, NULL, 
                    (IOUserNetworkTxCompletionQueue **)&ivars->queues[RXSubmissionQ]))) 
		return StartFailed(result); 
    

	if (_ISERROR(b, result, RegisterEthernetInterface(DataMacAddress, ivars->pool, ivars->queues, 4))) 
		return StartFailed(result); 

#endif

    /* XXX causes panic, but possibly only when IOClass is IOUserNetworkEthernet, not if IOUserService
     */
    DLOG("Calling RegisterService");
    RegisterService();
    return kIOReturnSuccess;
}

/* Stop - release resources and call provide Stop()
 */
kern_return_t IMPL(Data, Stop) {
    DEFAULT("");
    ReleaseResources();
    return Stop(provider, SUPERDISPATCH);
}

void Data::free() {
    DEFAULT("");

    OSSafeReleaseNULL(ivars->properties);
    IOSafeDeleteNULL(ivars, Data_IVars, 1);

}

