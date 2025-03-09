# AllYourBase - A baseXX encoder/decoder/recoder app

AllYourBase is a command-line utility that can encode, decode or recode data using various 'base' encodings, like hexadecmial, base64 etc. It also offers a couple of other encoding schemes like yenc or quoted-printable.


# CAVEATS

AllYourBase primarily exists to test encodings built into libUseful (https://github.com/ColumPaget/libUseful). Some of the encoders and decoders are rather slow (notably binary). The encoders/decoders have been written to be readable and supportable, rather than fast, and in normal libUseful usage are expected to be used on short strings of data, rather than large files.

AllYourBase can currently only handle one encoded block per file, so it is not possible to encode multiple files into one encoded output file.

libUseful has encoders for ascii85 and z85, but these appear to be incorrect and are not supported in AllYourBase at current.


# INSTALL

```
  ./configure
  make
```

should produce an executable called 'ayb'. Run 'configure --help' to see options that can be passed to configure.



# USAGE

``` 
   ayb [-w <wide>] [-h] [-i <format>] [-o <format>] [infile] [outfile]
``` 


# OPTIONS

``` 
   -w <wide>     break output up into lines that are <wide> bytes wide
   -h            add uuencode/yenc style headers to output
   -i <format>   format of input file (see 'supported encodings' below)
   -o <format>   format of output file (see 'supported encodings' below)
	 -?            print help
	 -help         print help
	 --help        print help
	 --version     print version
``` 


# Supported Encodings

AllYouBase currently supports:

```
  binary           - binary (base 2) (beware: SLOW)
  octal            - octal (base 8)
  decimal          - decimal (base 10)
  hex              - hexadecimal (base 16)
  hexupper         - hexadecimal (base 16) with uppercase characters
  base32           - rfc4648 base32, as per section 6 of rfc4648
  cbase32          - Douglas Crockford's base32
  hbase32          - 'extended hex' base32, as per rfc4648, section 7
  zbase32          - Zooko Wilcox-O'Hearn's base32, as used in ZRTP and Tahoe-LAFS
  wbase32          - Word-safe base32
  bech32           - Bitcoin/Nostr style bech32
  base64           - standard base64, as per rfc4648 section 4
  rbase64          - url/filename safe base64 as per rfc4648 section 5
  crypt            - base64 as used by 'crypt'
  ibase64          - alternative base64, characters differently ordered
  pbase64          - alternative base64, characters differently ordered, url-safe
  uuenc            - uuencode base64
  xxenc            - xxencode base64
  mime             - quoted-printable MIME
  quoted-printable - quoted-printable MIME
``` 

For great justice!
