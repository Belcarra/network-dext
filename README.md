# network-dext

This is a minimal DriverKit project that builds a NetworkingDriverKit extension that panics MacOS.

See the *problemreports* directory for sample problemreports generated after system reboot.

This is the Start() function.

```
kern_return_t IMPL(Data, Start) 
{
    DLOG("Start %s %s", __DATE__, __TIME__);

    ivars->provider = provider;
    kern_return_t result;

    bool b;
    if (_ISERROR(b, result, Start(provider, SUPERDISPATCH))) { Stop(provider, SUPERDISPATCH); return result; }
    if (_ISNULL(b, ivars->hostInterface, OSDynamicCast(IOUSBHostInterface, provider))) return StartFailed(kIOReturnNotFound);

    /* XXX causes panic, possibly only when IOClass is IOUserNetworkEthernet 
     */
    RegisterService();
    return kIOReturnSuccess;
}
```


