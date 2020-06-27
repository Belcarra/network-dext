# vim: noexpandtab tabstop=8

#
# Configuration
#
# These are used to configure this DEXT build
# 
#	PREFIX			used to customize the PROJECT_BUNDLE_IDENTIFIER
#
#	IDVENDOR
#	IDPRODUCT
#	BCONFIGURATIONVALUE
#	BINTERFACENUMBER
#

#
# Targets
#
#	PROJECT_BUNDLE_IDENTIFIER	return the customized value
#
#	build				build and codesign the DEXT
#
#





.PHONY: build

ifndef PREFIX 
PREFIX = test
endif
ifndef IDVENDOR 
IDVENDOR = 0x15ec
endif
ifndef IDPRODUCT 
IDPRODUCT = 0xf101
endif
ifndef BCONFIGURATIONVALUE 
BCONFIGURATIONVALUE = 0x1
endif
ifndef BINTERFACENUMBER 
BINTERFACENUMBER = 0x0
endif
ifndef IOCLASS
IOCLASS = IOUserNetworkEthernet
endif
#ifndef IOCLASS
#IOCLASS = IOUserService
#endif

include Makefile.cfg

.PHONY: build Info.plist

all:

# definition of the PRODUCT_BUNDLE_IDENTIFIER
#PROJECT_BUNDLE_IDENTIFIER:
#	@echo com.belcarra.${PREFIX}-MyUserUSBInterfaceDriver2 

test:
	@echo PREFIX: ${PREFIX}
	@echo PROJECT_BUNDLE_IDENTIFIER: `make PROJECT_BUNDLE_IDENTIFIER`
	@echo DEXTPATH: `make DEXTPATH`
	@echo ENTITLEMENTS: `make ENTITLEMENTS`

build: clean Info.plist xcodebuild codesign

# Setup Info.plist, N.B. Additional values are set as part of xcodebuild,
# but these values are not supported from the project file, xcconfig or command line.
#
Info.plist: ./Info.plist.in
	sed \
		-e 's/_IDVENDOR_/${IDVENDOR}/' \
		-e 's/_IDPRODUCT_/${IDPRODUCT}/' \
		-e 's/_BCONFIGURATIONVALUE_/${BCONFIGURATIONVALUE}/' \
		-e 's/_BINTERFACENUMBER_/${BINTERFACENUMBER}/' \
		-e 's/_IOCLASS_/${IOCLASS}/' \
		< $< > $@ 

# Build the project 
xcodebuild:
	xcodebuild \
		-quiet \
		INFOPLIST_FILE=Info.plist \
		PRODUCT_BUNDLE_IDENTIFIER=`make PREFIX=${PREFIX} PROJECT_BUNDLE_IDENTIFIER` \
		PROVISIONING_PROFILE_SPECIFIER=`make PREFIX=${PREFIX} PROVISIONING_PROFILE_SPECIFIER` \
		-configuration Release SYMROOT="$(CURDIR)/build"

		#TARGET_TEMP_DIR=tmp \

codesign:
	../codesign-dext.sh ${PREFIX}

clean:
	-rm -rf build tmp Info.plist

really-clean: clean
	-rm -rf *.xcodeproj/*xcworkspace/
	-rm -rf *.xcodeproj/*xcuserdata/


xcode:
	open *.xcodeproj

open: 
	open build/Debug/USBApp.app

install:
	rsync -a --delete build/Debug/USBApp.app /Applications

run: install
	open /Applications/USBApp.app

list:
	systemextensionsctl list

reset:
	systemextensionsctl reset

log-show-extension-manager:
	log show --predicate 'sender == "sysextd" or sender CONTAINS "sc.knight"' --info --debug --last 1h
