/*
 * utils.cpp
 *
 * Copyright (c) 2012-2020 Belcarra Technologies 2005 
 *
 * By stuart.lynne@belcarra.com 
 *
 */

#if 0
/* get_bool_property
 */
bool get_bool_property(IORegistryEntry *provider, const char *aKey, bool *property, int index, const char *part, const char *function)
{
        OSBoolean       *boolObj = (OSBoolean *) provider->getProperty(aKey);
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} boolObj: %p\n", index, part, function, aKey, boolObj);
        return_false_unless(boolObj);

        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} boolObj: true: %d false: %d value: %d\n", index, part, function,
                aKey, boolObj->isTrue(), boolObj->isFalse(), boolObj->getValue() );
        *property = boolObj->isTrue();
        return true;
}

/* get_bool_property_default
 */
void get_bool_property_default(IORegistryEntry *provider, const char *aKey, bool *property, bool def, int index, const char *part, const char *function)
{
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} default: %d\n", index, part, function, aKey, def);
        return_if(get_bool_property(provider, aKey, property, index, part, function));
        *property = def;
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} default: %d\n", index, part, function, aKey, *property);
}

/* get_integer_property
 */
bool get_UInt32_property(IORegistryEntry *provider, const char *aKey, UInt32 *property, int index, const char *part, const char *function)
{
        OSNumber       *numberObj = (OSNumber *) provider->getProperty(aKey);
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} boolObj: %p\n", index, part, function, aKey, numberObj);
        return_false_unless(numberObj);
        *property = numberObj->unsigned32BitValue();
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} using: %d\n", index, part, function, aKey, *property);
        return true;
}

/* get_integer_property_default
 */
void get_UInt32_property_default(IORegistryEntry *provider, const char *aKey, UInt32 *property, UInt32 def, int index, const char *part, const char *function)
{
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} default: %d\n", index, part, function, aKey, def);
        return_if(get_UInt32_property(provider, aKey, property, index, part, function));
        *property = def;
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} default: %d\n", index, part, function, aKey, *property);
}

/* get_string_property
 */
bool get_string_property(IORegistryEntry *provider, const char *aKey, char *property, UInt32 maxLength, int index, const char *part, const char *function)
{
        OSString       *stringObj = OSDynamicCast(OSString, provider->getProperty(aKey));
        //OSBoolean *boolObj1 = OSDynamicCast(OSBoolean, provider->getProperty("kSingleDataInterfaceModel"));


        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} stringObj: %p\n", index, part, function, aKey, stringObj);

        return_false_unless(stringObj);

        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} stringObj: %p length: %p max: %d *********************\n", index, part, function,
                        aKey, stringObj->getLength(), maxLength);
        return_false_unless(stringObj->getLength() < maxLength);

        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " template: %{public}\n", index, part, function, aKey, stringObj->getCStringNoCopy());

        strncpy(property, stringObj->getCStringNoCopy(), maxLength);


        unsigned int length = stringObj->getLength();
        // null terminate this string
        if(length > maxLength)
            length = maxLength;

        property[length] = 0;
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} using: %d\n", index, part, function, aKey, *property);

        return true;

}

/* get_string_property_default
 */
void get_string_property_default(IORegistryEntry *provider, const char *aKey, char *property, UInt32 maxLength, int index, char *def, const char *part, const char *function)
{
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} default: %d\n", index, part, function, aKey, def);
        return_if(get_string_property(provider, aKey, property, maxLength, index, part, function));
        strncpy(property, def, maxLength);
        os_log( "[%2x] %{public}::%{public}:" __FUNCTION__ " %{public} default: %d\n", index, part, function, aKey, *property);
}

#endif
