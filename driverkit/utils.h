#ifndef utils_h
#define utils_h





#define until(x)                while(!(x)) 
#define unless(x)               if (!(x))  

#define mycatch(x)                while(0) x:
#define mythrow(x)                goto x
#define mythrow_if(e, x)          if ((e)) { goto x; } else { /* DO NOTHING */ }
#define mythrow_unless(e, x)      unless ((e)) { goto x; } else { /* DO NOTHING */ }


#define continue_if(x)          if (x) { continue; } else { /* do nothing */ }
#define continue_unless(x)      unless (x) { continue; } else { /* do nothing */ }

#define break_if(x)             if (x) { break; } else { /* do nothing */ }
#define break_unless(x)         unless (x) { break; } else { /* do nothing */ }

#define return_if(x)            if (x) { return; } else { /* do nothing */ }
#define return_unless(x)        unless (x) { return; }  else { /* do nothing */ }

#define return_v_if(x,v)        if (x) { return (v); } else { /* do nothing */ }
#define return_v_unless(x,v)    unless (x) { return (v); } else { /* do nothing */ }

#define return_null_if(x)               return_v_if((x), NULL)
#define return_null_unless(x)           return_v_unless((x), NULL)

#define return_status_insufficient_resources_if(x)       return_v_if((x),     status_insufficient_resources)
#define return_status_insufficient_resources_unless(x)   return_v_unless((x), status_insufficient_resources)

#define return_status_insufficient_resources_if(x)       return_v_if((x),     status_insufficient_resources)
#define return_status_insufficient_resources_unless(x)   return_v_unless((x), status_insufficient_resources)

#define return_status_success_if(x)                     return_v_if((x),     status_success)
#define return_status_success_unless(x)                 return_v_unless((x), status_success)

#define return_status_unsuccessful_if(x)                return_v_if((x),     status_unsuccessful)
#define return_status_unsuccessful_unless(x)            return_v_unless((x), status_unsuccessful)

#define return_true_if(x)               return_v_if((x),     true)
#define return_true_unless(x)           return_v_unless((x), true)

#define return_false_if(x)              return_v_if((x),     false)
#define return_false_unless(x)          return_v_unless((x), false)

#define return_zero_if(x)              return_v_if((x),     0)
#define return_zero_unless(x)          return_v_unless((x), 0)

#define return_kIOReturnSuccess_if(x)              return_v_if((x),     kIOReturnSuccess)
#define return_kIOReturnSuccess_unless(x)          return_v_unless((x), kIOReturnSuccess)

#define return_kIOReturnOutputDropped_if(x)              return_v_if((x),     kIOReturnOutputDropped)
#define return_kIOReturnOutputDropped_unless(x)          return_v_unless((x), kIOReturnOutputDropped)

#define return_kIOReturnError_if(x)              return_v_if((x),     kIOReturnError)
#define return_kIOReturnError_unless(x)          return_v_unless((x), kIOReturnError)

#define return_kIOReturnOutputDropped_if(x)              return_v_if((x),     kIOReturnOutputDropped)
#define return_kIOReturnOutputDropped_unless(x)          return_v_unless((x), kIOReturnOutputDropped)

#define return_kIOReturnIOError_if(x)              return_v_if((x),     kIOReturnIOError)
#define return_kIOReturnIOError_unless(x)          return_v_unless((x), kIOReturnIOError)

#define BOOLEAN(b) ((b)?true:false)



#if 0
bool get_bool_property(IORegistryEntry *provider, const char *aKey, bool *property, int index, const char *part, const char *function);
bool get_UInt32_property(IORegistryEntry *provider, const char *aKey, UInt32 *property, int index, const char *part, const char *function);
bool get_string_property(IORegistryEntry *provider, const char *aKey, char *property, UInt32 maxlength, int index, const char *part, const char *function);
bool get_string_property(IORegistryEntry *provider, const char *aKey, char *property, UInt32 maxlength, int index, const char *part, const char *function);

void get_bool_property_default(IORegistryEntry *provider, const char *aKey, bool *property, bool def, int index, const char *part, const char *function);
void get_UInt32_property_default(IORegistryEntry *provider, const char *aKey, UInt32 *property, UInt32 def, int index, const char *part, const char *function);
void get_string_property_default(IORegistryEntry *provider, const char *aKey, int index, char *property, UInt32 maxLength, char *def);
#endif

#endif

