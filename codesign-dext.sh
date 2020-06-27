#!/bin/bash

if [ $# -ne 1 ]; then 
	echo Usage ./config.sh customerprfix 
	exit 1
fi

PREFIX=${1:=}
PROJECT_BUNDLE_IDENTIFIER=$(make PREFIX=${PREFIX} PROJECT_BUNDLE_IDENTIFIER)
ENTITLEMENTS=$(make PREFIX=${PREFIX} ENTITLEMENTS)

echo PROJECT_BUNDLE_IDENTIFIER: $PROJECT_BUNDLE_IDENTIFIER:
echo ENTITLEMENTS: $ENTITLEMENTS:



# Use this to find your certificate identity:
#	security find-identity
#
readonly CODE_SIGN_IDENTITY=AF81FE1962517ECAABF73719286C4829C4C06EA5

#eval $(sed -e '/\/\//d' < USBApp.xcconfig)

set -e

# N.B. for this to work via ssh, you need to run script from terminal on console, 
# enter password in security dialog, and then click "Allow Always"
#
security  unlock-keychain -p '{}' /Users/${USER}/Library/Keychains/login.keychain 

set -x

codesign --sign $CODE_SIGN_IDENTITY \
    --entitlements ${ENTITLEMENTS} \
    --options runtime --verbose --force \
    build/Release-driverkit/${PROJECT_BUNDLE_IDENTIFIER}.dext

codesign --verify --verbose \
    build/Release-driverkit/${PROJECT_BUNDLE_IDENTIFIER}.dext

