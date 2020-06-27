# Implementation Notes.

## Sun Jun 21 00:21:52 PDT 2020 - AppleUserECMData driver anomoaly

When ECM device plugged in and matched against Apple ECM drivers:
```
    +-o IOUserServer(com.apple.DriverKit.AppleUserECM-0x100000603)  <class IOUserServer, id 0x100000609, registered, matched, active, busy 0 (0 ms), retain 11>
    +-o IOUserServer(com.apple.DriverKit.AppleUserECMData-0x100000605)  <class IOUserServer, id 0x10000060a, registered, matched, active, busy 0 (0 ms), retain 11>
```

Same device with our driver plugged in and matched with vid/pid/interface/configuration:
```
    +-o IOUserServer(com.belcarra.driverkit.VIDPID0-0x100000682)  <class IOUserServer, id 0x10000068c, registered, matched, active, busy 0 (0 ms), retain 11>
    +-o IOUserServer(com.belcarra.driverkit.VIDPID1-0x100000687)  <class IOUserServer, id 0x10000068d, registered, matched, active, busy 0 (0 ms), retain 8>
    +-o IOUserServer(com.apple.DriverKit.AppleUserECMData-0x100000688)  <class IOUserServer, id 0x100000692, registered, matched, active, busy 0 (0 ms), retain 11>
```

N.B. the extra match and registered AppleUserECMData driver. 


## Sat Jun 20 20:13:04 PDT 2020 - Match strategy

While the driver will get loaded multiple times, once per interface, we only need one active.

Apple's strategy with AppleECM/AppleECMData is to use a dummy driver for Data interface instance and do everything from the COMM interface instance.

It looks like doing the opposite, everything from the Data instance and use a dummy for the Comm.

That will allow non CDC two interface drivers (e.g. EEM and ECM-subset) to work with the same Data driver.

The same driver can be loaded for both instances. The one loaded for the Comm interface instance just needs to do nothing.

The Data driver instance needs to review the configuration descriptor and as necessary get the COMM interface descriptor information to determine the protocol
to use if present, otherwise rely on the Data interface descriptor information.


## Sat Jun 20 20:13:13 PDT 2020  - Device vs Inteface Match

This can be used to *force* a device match only.

```
		<key>VIDPID</key>
		<dict>
			<key>CFBundleIdentifier</key>       <string>$(PRODUCT_BUNDLE_IDENTIFIER)</string>
            <key>IOProbeScore</key>             <integer>210000</integer>
			<key>IOClass</key>                  <string>IOUserNetworkEthernet</string>
			<key>IOProviderClass</key>          <string>IOUSBHostDevice</string>
			<key>IOUserClass</key>              <string>Data</string>
			<key>IOUserServerName</key>         <string>com.belcarra.driverkit.VIDPID</string>
            <key>idVendor</key>                 <integer>0x15ec</integer>
            <key>idProduct</key>                <integer>0xf101</integer>
		</dict>
```

Then in ::Start() this cast will succeed:
```
    if (_ISNULL(b, ivars->hostDevice, OSDynamicCast(IOUSBHostDevice, provider))) return StartFailed(kIOReturnNotFound);
```

Unfortunately this would require the driver support all interfaces. Which for composite devices
would mean matching the class (or other) drivers for them, e.g. acm and mass storage.



