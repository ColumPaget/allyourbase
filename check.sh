#!/bin/bash

# This script is run by 'make check' and checks results of multiple encoding types
# encoding a test file (services) and checking the resulting outputs using MD5 hashes
# against the known hashes of the expected outputs.

# This script depends on bash and the gnu 'md5sum' command being available

function Test
{
local CMD

echo -n "  doing: $5"
CMD="./ayb $6"

if [ "$1" != "" ]
then
CMD="$CMD -i $1"
fi

if [ "$2" != "" ]
then
CMD="$CMD -o $2"
fi

CMD="$CMD $3 > $3.$2"
eval "$CMD"

MD5=`md5sum $3.$2 | cut -d ' ' -f 1`

if [ "$MD5" = "$4" ]
then
echo -e "\r  \033[01;32mOKAY\033[00;39m    $5     "
else
echo -e "\r  \033[01;31mFAIL\033[00;39m    $5     "
fi

}



echo "Testing encoders"
Test "" octal tests/services a047393e95170474ca41b50f13282649 "Encode Base8 (octal)"
Test "" decimal tests/services b950eae64992285168a413d738028b23 "Encode Base10 (decimal)"
Test "" hexupper tests/services 70cbdbcd621522b177e0e0e608695808 "Encode Base16 (hex)"
Test "" base32 tests/services dedc9d9e0677f8d2c7f4a17dfe4af85a "Encode Base32"
Test "" cbase32 tests/services 1cdf35ad86f4245b2ad30a8a7c9a4c37 "Encode Crockford's Base32"
Test "" zbase32 tests/services d85629b4506375cbe491ea8c88726c69 "Encode Zooko Wilcox-O'Hearn's Base32"
Test "" hbase32 tests/services 20818355c23841f4b0fbe4c9adf6e511 "Encode Extended-hex Base32"
Test "" bech32 tests/services 4b7473445e7365252e74bfac6461703b "Encode Bitcoin-style Bech32"
Test "" base64 tests/services 4aa22c5cdb47e8fc7194e7626f93301b "Encode Base64"
Test "" rbase64 tests/services 63080bb1e8cba9a67f71843fcee05387 "Encode URL-safe Base64"
Test "" crypt tests/services 5a6f91a35ddf0bafa1fc2a80a9b39770 "Encode crypt-style Base64"
Test "" uuenc tests/services 037ff28d806d2405daaf05b1a76a1080 "Encode UUencode" "-h"
Test "" xxenc tests/services ed760f9032ce781892a0306daf70e786 "Encode XXencode" "-h"
Test "" yenc tests/services a4e74a3c1b0dc27456e48551fc5dbfc1 "Encode Yencode" "-h"
Test "" quoted tests/services 04511e1685907735f849d4fac919f82b "Encode MIME quoted-printable" 
#Test "" binary tests/services 171b64c1ea48ded342f7039f1860d92a "Encode base2 (binary) (SLOW)" 

echo ""
echo "Testing decoders"
Test octal raw tests/services.octal 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Base8 (octal)"
Test decimal raw tests/services.decimal 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Base10 (decimal)"
Test hexupper raw tests/services.hexupper 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Base16 (hex)"
Test base32 raw tests/services.base32 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Base32"
Test cbase32 raw tests/services.cbase32 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Crockford's Base32"
Test zbase32 raw tests/services.zbase32 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Zooko Wilcox-O'Hearn's Base32"
Test hbase32 raw tests/services.hbase32 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Extended-hex Base32"
Test base32 raw tests/services-base32enc 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Base32 encoded by basenc with non-standard line length"
Test bech32 raw tests/services.bech32 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Bitcoin-style Bech32"
Test base64 raw tests/services.base64 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Base64"
Test rbase64 raw tests/services.rbase64 9d39e51e6bd5a30168c26c57b66d2e37 "Decode URL-safe Base64"
Test crypt raw tests/services.crypt 9d39e51e6bd5a30168c26c57b66d2e37 "Decode crypt-style Base64"
Test uuenc raw tests/services.uuenc 9d39e51e6bd5a30168c26c57b66d2e37 "Decode UUencode"
Test xxenc raw tests/services.xxenc 9d39e51e6bd5a30168c26c57b66d2e37 "Decode XXencode"
Test yenc raw tests/services.yenc 9d39e51e6bd5a30168c26c57b66d2e37 "Decode Yencode"
Test quoted raw tests/services.quoted 9d39e51e6bd5a30168c26c57b66d2e37 "Decode MIME quoted-printable"
#Test binary raw tests/services.binary 9d39e51e6bd5a30168c26c57b66d2e37 "Decode base2 (binary) (SLOW)"


