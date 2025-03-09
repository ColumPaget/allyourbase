#ifdef USE_LIBUSEFUL_BUNDLED
#include "libUseful-bundled/libUseful.h"
#else
#include "libUseful-5/libUseful.h"
#endif

#define FLAG_HEADERS 1

#define VERSION "v1.0"

typedef struct
{
    int Flags;
    int InputEncode;
    int OutputEncode;
    int InputBlockSize;
    int OutputBlockSize;
    int OutputLineLen;
    char *InPath;
    char *OutPath;
} TSettings;

#define ENCODE_RAW 0


void PrintVersion()
{
printf("ayb (all your base) %s\n", VERSION);
exit(0);
}

void PrintHelp()
{
    printf("all your base %s\n", VERSION);
    printf("usage:\n");
    printf("   ayb [-h] [-w <wide>] [-i <format>] [-o <format>] [infile] [outfile]\n\n");
    printf("options:\n");
    printf("   -w <wide>     break output up into lines that are <wide> bytes wide\n");
    printf("   -h            add uuencode/yenc style headers to output\n");
    printf("   -i <format>   format of input file (see 'supported formats' below)\n");
    printf("   -o <format>   format of output file (see 'supported formats' below)\n");
		printf("   -?            print this help\n");
		printf("   -help         print this help\n");
		printf("   --help        print this help\n");
		printf("   -version      print version\n");
		printf("   --version     print version\n");
    printf("\nsupported formats:\n");
    printf("  2                - binary (base 2) (beware: SLOW)\n");
    printf("  base2            - binary (base 2) (beware: SLOW)\n");
    printf("  bin              - binary (base 2) (beware: SLOW)\n");
    printf("  binary           - binary (base 2) (beware: SLOW)\n");
    printf("  8                - octal (base 8)\n");
    printf("  base8            - octal (base 8)\n");
    printf("  oct              - octal (base 8)\n");
    printf("  octal            - octal (base 8)\n");
    printf("  10               - decimal (base 10)\n");
    printf("  base10           - decimal (base 10)\n");
    printf("  dec              - decimal (base 10)\n");
    printf("  decimal          - decimal (base 10)\n");
    printf("  16               - hexadecimal (base 16)\n");
    printf("  base16           - hexadecimal (base 16)\n");
    printf("  hex              - hexadecimal (base 16)\n");
    printf("  hexupper         - hexadecimal (base 16) with uppercase characters\n");
    printf("  32               - rfc4648 base32, as per section 6 of rfc4648\n");
    printf("  base32           - rfc4648 base32, as per section 6 of rfc4648\n");
    printf("  c32              - Douglas Crockford's base32\n");
    printf("  cbase32          - Douglas Crockford's base32\n");
    printf("  h32              - 'extended hex' base32, as per rfc4648, section 7\n");
    printf("  hbase32          - 'extended hex' base32, as per rfc4648, section 7\n");
    printf("  z32              - Zooko Wilcox-O'Hearn's base32, as used in ZRTP and Tahoe-LAFS\n");
    printf("  zbase32          - Zooko Wilcox-O'Hearn's base32, as used in ZRTP and Tahoe-LAFS\n");
    printf("  w32              - Word-safe base32\n");
    printf("  wbase32          - Word-safe base32\n");
    printf("  bech32           - Bitcoin/Nostr style bech32\n");
    printf("  64               - standard base64, as per rfc4648 section 4\n");
    printf("  base64           - standard base64, as per rfc4648 section 4\n");
    printf("  r64              - url/filename safe base64 as per rfc4648 section 5\n");
    printf("  rbase64          - url/filename safe base64 as per rfc4648 section 5\n");
    printf("  crypt            - base64 as used by 'crypt'\n");
    printf("  i64              - alternative base64, characters differently ordered\n");
    printf("  ibase64          - alternative base64, characters differently ordered\n");
    printf("  p64              - alternative base64, characters differently ordered, url-safe\n");
    printf("  pbase64          - alternative base64, characters differently ordered, url-safe\n");
    printf("  uu               - uuencode base64\n");
    printf("  uuenc            - uuencode base64\n");
    printf("  xx               - xxencode base64\n");
    printf("  xxenc            - xxencode base64\n");
    printf("  mime             - quoted-printable MIME\n");
    printf("  quoted-printable - quoted-printable MIME\n");
    printf("  raw              - raw decoded data\n");

    exit(0);
}


int GetDecodedBlockSize(int Encoding)
{
    switch (Encoding)
    {
    case ENCODE_BINARY:
        return(1);
        break;

    case ENCODE_BASE32:  //rfc4648 base 32
    case ENCODE_CBASE32:  //crockford base32
    case ENCODE_HBASE32:  //'extended hex' base32
    case ENCODE_WBASE32:  //'word safe' base32
    case ENCODE_ZBASE32:
    case ENCODE_BECH32:
        return(5);
        break;

    case ENCODE_ASCII85:
    case ENCODE_Z85:
        return(4);
        break;


    case ENCODE_BASE64:
    case ENCODE_IBASE64:
    case ENCODE_PBASE64:
    case ENCODE_RBASE64:
    case ENCODE_UUENC:
    case ENCODE_XXENC:
    case ENCODE_CRYPT:
        return(3);
        break;
    }

    return(1);
}


int GetEncodedBlockSize(int Encoding)
{
    switch (Encoding)
    {

    case ENCODE_BINARY:
    case ENCODE_BASE32:  //rfc4648 base 32
    case ENCODE_CBASE32:  //crockford base32
    case ENCODE_HBASE32:  //'extended hex' base32
    case ENCODE_WBASE32:  //'word safe' base32
    case ENCODE_ZBASE32:
    case ENCODE_BECH32:
        return(8);
        break;

    case ENCODE_ASCII85:
    case ENCODE_Z85:
        return(5);
        break;


    case ENCODE_BASE64:
    case ENCODE_IBASE64:
    case ENCODE_PBASE64:
    case ENCODE_RBASE64:
    case ENCODE_UUENC:
    case ENCODE_XXENC:
    case ENCODE_CRYPT:
        return(4);
        break;

    case ENCODE_OCTAL:
    case ENCODE_DECIMAL:
        return(3);
        break;

    case ENCODE_HEX:
    case ENCODE_HEXUPPER:
        return(2);
        break;
    }

    return(1);
}


void PostProcessSettings(TSettings *Settings)
{
    int bs, len;

    if (Settings->OutputLineLen==-1)
    {
        switch (Settings->OutputEncode)
        {
        case ENCODE_RAW:
            Settings->OutputLineLen=0;
            break;

        case ENCODE_UUENC:
        case ENCODE_XXENC:
            Settings->OutputLineLen=60;
            break;

        case ENCODE_OCTAL:
        case ENCODE_DECIMAL:
            Settings->OutputLineLen=75;
            break;

        case ENCODE_BINARY:
        case ENCODE_BASE32:  //rfc4648 base 32
        case ENCODE_CBASE32:  //crockford base32
        case ENCODE_HBASE32:  //'extended hex' base32
        case ENCODE_WBASE32:  //'word safe' base32
        case ENCODE_ZBASE32:
        case ENCODE_BECH32:
            Settings->OutputLineLen=72;
            break;

        default:
            Settings->OutputLineLen=76;
            break;
        }
    }

    //adjust even if we set the length above, because we might need to adjust the default
    if (Settings->OutputLineLen > 0)
    {
        bs=GetEncodedBlockSize(Settings->OutputEncode);
        len=(Settings->OutputLineLen / bs) * bs;
        if (len != Settings->OutputLineLen)
        {
            fprintf(stderr, "Selected encoding has blocksize of %d. Setting Output Line Len to %d\n", bs, len);
            Settings->OutputLineLen=len;
        }
    }

}


//handle some encoding names that aren't in libUseful
int AybEncodingParse(const char *Name)
{
    if (! StrValid(Name)) return(ENCODE_RAW);
    if (strcasecmp(Name, "raw")==0) return(ENCODE_RAW);
    if (strcasecmp(Name, "base8")==0) return(ENCODE_OCTAL);
    if (strcasecmp(Name, "base16")==0) return(ENCODE_HEX);
    if (strcasecmp(Name, "base32")==0) return(ENCODE_BASE32);
    return(EncodingParse(Name));
}


TSettings *ParseCommandLine(int argc, char *argv[])
{
    CMDLINE *CL;
    const char *item;
    TSettings *Settings;

    Settings=(TSettings *) calloc(1, sizeof(TSettings));
    Settings->OutputLineLen=-1;

    CL=CommandLineParserCreate(argc, argv);
    item=CommandLineNext(CL);
    while (item)
    {
        if (strcmp(item, "-i")==0) Settings->InputEncode=AybEncodingParse(CommandLineNext(CL));
        else if (strcmp(item, "-o")==0) Settings->OutputEncode=AybEncodingParse(CommandLineNext(CL));
        else if (strcmp(item, "-w")==0) Settings->OutputLineLen=atoi(CommandLineNext(CL));
        else if (strcmp(item, "-h")==0) Settings->Flags |= FLAG_HEADERS;
        else if (strcmp(item, "--help")==0) PrintHelp();
        else if (strcmp(item, "-help")==0) PrintHelp();
        else if (strcmp(item, "-?")==0) PrintHelp();
        else if (strcmp(item, "-version")==0) PrintVersion();
        else if (strcmp(item, "--version")==0) PrintVersion();
        else if (! StrValid(Settings->InPath))  Settings->InPath=CopyStr(Settings->InPath, item);
        else Settings->OutPath=CopyStr(Settings->OutPath, item);
        item=CommandLineNext(CL);
    }


    if (! StrValid(Settings->InPath)) Settings->InPath=CopyStr(Settings->InPath, "stdin:");

    //this is decided in 'GetOutputPath'
    //if (! StrValid(Settings->OutPath)) Settings->OutPath=CopyStr(Settings->OutPath, "stdout:");

    Settings->InputBlockSize=GetEncodedBlockSize(Settings->InputEncode);
    Settings->OutputBlockSize=GetDecodedBlockSize(Settings->OutputEncode);

    PostProcessSettings(Settings);
    return(Settings);
}



int IsEndLineCheckNextLine(STREAM *S, const char *Expected)
{
    char *Tempstr=NULL;
    int len, elen;
    int RetVal=FALSE;

    elen=StrLen(Expected);
    Tempstr=SetStrLen(Tempstr, 40);
    len=STREAMPeekBytes(S, Tempstr, 20);

    if ((len >= elen) && (strncmp(Tempstr, Expected, elen)==0))
    {
        Tempstr=STREAMReadLine(Tempstr, S);
        RetVal=TRUE;
    }

    return(RetVal);
}

int IsEndLineMulti(STREAM *S, const char *Buffer, const char *Expect1, const char *Expect2)
{
    if (strcmp(Buffer, Expect1)==0)
    {
        if (IsEndLineCheckNextLine(S, Expect2)) return(TRUE);
    }

    return(FALSE);
}


int IsEndLine(STREAM *S, const char *Buffer, int len, int Encoding)
{
    if (len < 2) return(FALSE);
    switch (Encoding)
    {
    case ENCODE_XXENC:
        if (strncmp(Buffer, "end\n", 4)==0) return(TRUE);
        break;

    case ENCODE_UUENC:
        if (IsEndLineMulti(S, Buffer, "`\n", "end\n")) return(TRUE);
        if (IsEndLineMulti(S, Buffer, "`\r\n", "end\r\n")) return(TRUE);
        break;

    case ENCODE_YENCODE:
        if (strncmp(Buffer, "=y", 2)==0) return(TRUE);
        break;
    }

    return(FALSE);
}


int ReadInputLine(STREAM *S, char *Buffer, int MaxLen, int Encoding)
{
    int len;

    len=STREAMReadBytesToTerm(S, Buffer, MaxLen, '\n');
    if (len < 1) return(len);

    if (IsEndLine(S, Buffer, len, Encoding)) return(0);

    StrUnsafeTrunc(Buffer, len);
    StripTrailingWhitespace(Buffer);

    return(len);
}


// Some encoders break up blocks of encoded bytes, creating line lengths that are
// not multiples of the encoder blocksize. Thus we have to recreate lines of the
// correct size
char *AdjustInputForDecoding(char *RetStr, char *Input, int InputLen, int Encoding)
{
    static char *Prev=NULL;
    static int PrevLen=0;
    int bs, len, remain, max;

    bs=GetEncodedBlockSize(Encoding);

    if (bs == 0) return(CopyStrLen(RetStr, Input, InputLen));

    RetStr=CopyStrLen(RetStr, Prev, PrevLen);
    if (InputLen > 0) RetStr=CatStrLen(RetStr, Input, InputLen);
    len=StrLen(RetStr);
    Prev=CopyStr(Prev, "");

    if (len==0) return(RetStr);

    if (InputLen > 0)
    {
        remain=len % bs;
        if ( (remain > 0) && (remain != len) )
        {
            max=len-remain;
            Prev=CopyStr(Prev, RetStr + max);
            PrevLen=remain;
            StrUnsafeTrunc(RetStr, max);
        }
    }

    return(RetStr);
}


int ReadInput(STREAM *S, char **Bytes, int MaxLen, int Encoding)
{
    char *Buffer=NULL, *Encoded=NULL;
    const char *ptr;
    int len, max;

    if (Encoding > 0)
    {
        Buffer=SetStrLen(Buffer, MaxLen);
        len=ReadInputLine(S, Buffer, MaxLen, Encoding);

            switch (Encoding)
            {
            //uuencode has a leading byte that specifies the number
            //of bytes encoded in a line as 32 + <no of bytes>
            case ENCODE_UUENC:
                if (len > 0)
                {
                    max=(*Buffer) - 32;
                    len=DecodeBytes(Bytes, Buffer+1, Encoding);
                    if (len > max) len=max;
                }
                break;

            //xxencode has a leading byte that specifies the number
            //of bytes encoded in a line using the xxencoding alphabet
            case ENCODE_XXENC:
                if (len > 0)
                {
                    len=DecodeBytes(Bytes, Buffer+1, Encoding);
                    ptr=strchr(XXENC_CHARS, *Buffer);
                    if (ptr)
                    {
                        max=ptr - XXENC_CHARS;
                        if (len > max) len=max;
                    }
                }
                break;

            case ENCODE_YENCODE:
                if (len > 0) len=DecodeBytes(Bytes, Buffer, Encoding);
                break;

            default:
								if (len <= 0) Encoded=AdjustInputForDecoding(Encoded, Buffer, 0, Encoding);
                else Encoded=AdjustInputForDecoding(Encoded, Buffer, len, Encoding);
                if (StrValid(Encoded)) len=DecodeBytes(Bytes, Encoded, Encoding);
                if (len == 0) len=STREAM_CLOSED;
                break;
            }
    }
//otherwise we are reading raw binary bytes to encode
    else len=STREAMReadBytes(S, *Bytes, MaxLen);


    Destroy(Encoded);
    Destroy(Buffer);
    return(len);
}

int AdjustLineLenForEncoding(char *Buffer, int LineLen, int Encoding)
{
    const char *ptr;

    switch (Encoding)
    {
    case ENCODE_QUOTED_MIME:
        for (ptr=Buffer + LineLen - 2; ptr < (Buffer + LineLen); ptr++)
        {
            if (*ptr=='=') LineLen = (ptr-Buffer);
        }
        break;
    }

    return(LineLen);
}




char *EncodeOutputLine(char *Output, const char *Input, int InputLen, int LineLen, int Encoding)
{
    static char *Buffer=NULL;
    static int BuffLen=0;
    //encoded bytes is the number of bytes the characters in the buffer
    //represent when they are decoded
    static int EncodedBytes=0;
    char *Tempstr=NULL;
    char TmpArray[4];
    int len;


    if ( (InputLen==-1) && (! StrValid(Buffer)) ) return(NULL);


    if (Encoding==ENCODE_RAW) return(Output);

    if (InputLen > 0) Output=EncodeBytes(Output, Input, InputLen, Encoding);
    else Output=CopyStr(Output, "");

    if (LineLen > 0)
    {
        len=strlen(Output);
        Buffer=AddBytesToBuffer(Buffer, BuffLen, Output, len);
        BuffLen += len;

        LineLen=AdjustLineLenForEncoding(Buffer, LineLen, Encoding);

        switch (Encoding)
        {
        case ENCODE_QUOTED_MIME:
            Output=CopyStrLen(Output, Buffer, LineLen);
            Output=CatStr(Output, "=\n");
            break;

        case ENCODE_UUENC:
            //uuencode does not have any properly agreed padding, instead the
            //leading byte in uuencode represents number of bytes encoded,
            //as ascii 'bytes encoded + 32' in order to shift the value of
            //bytes encoded into the range of printable chars (with space being zero)
            //except nobody uses space, instead using '`'
            //this is a much inferior system to padding.

            //InputLen can be 0 or less for 'flush'
            if (InputLen > 0) EncodedBytes+=InputLen;
            Tempstr=CopyStrLen(Tempstr, Buffer, LineLen);
            len=StrLen(Tempstr);

            //either the line is LineLen long, or it's less
            //if LineLen long then it fits the 'blocksize' or uuencode
            //and we can calcluate the bytes encoded, otherwise we have a
            //final partial block and must get bytes encoded from what's
            //left in 'EncodedBytes'
            if (len == LineLen)
            {
                len=StrLen(Tempstr) / 4 * 3;
                EncodedBytes-=len;
            }
            else
            {
                len=EncodedBytes;
                EncodedBytes=0;
            }

            len+=32; //add 32 to shift the value up into the printable range
            Output=FormatStr(Output, "%c%s\n", len & 0xFF, Tempstr);
            break;

        case ENCODE_XXENC:
            //xxencode does not have any properly agreed padding, instead the
            //leading byte in xxencode represents number of bytes encoded,
            //and is itself encoded using the xxencode encoding alphabet
            //this is a much inferior system to padding.

            //InputLen can be 0 or less for 'flush'
            if (InputLen > 0) EncodedBytes+=InputLen;
            Tempstr=CopyStrLen(Tempstr, Buffer, LineLen);
            len=StrLen(Tempstr);

            //either the line is LineLen long, or it's less
            //if LineLen long then it fits the 'blocksize' or uuencode
            //and we can calcluate the bytes encoded, otherwise we have a
            //final partial block and must get bytes encoded from what's
            //left in 'EncodedBytes'
            if (len == LineLen)
            {
                len=StrLen(Tempstr) / 4 * 3;
                EncodedBytes-=len;
            }
            else
            {
                len=EncodedBytes;
                EncodedBytes=0;
            }
            Output=FormatStr(Output, "%c%s\n", XXENC_CHARS[len & 0xFF], Tempstr);
            break;

        default:
            len=LineLen;
            if (len > BuffLen) len=BuffLen;
            Output=SetStrLen(Output, len+2);
            strncpy(Output, Buffer, len);
            Output[len]='\n';
            Output[len+1]='\0';
            break;
        }

        if (BuffLen > LineLen)
        {
            len=BuffLen-LineLen;
            memmove(Buffer, Buffer+LineLen, len);
            StrTrunc(Buffer, len);
            BuffLen=len;
        }
        else
        {
            Buffer=CopyStr(Buffer, "");
            BuffLen=0;
        }
    }

    Destroy(Tempstr);
    return(Output);
}



char *ExamineFileHeader(char *FName, STREAM *S, TSettings *Settings)
{
    char *Tempstr=NULL, *Token=NULL;
    const char *ptr;
    int len;

    Tempstr=CopyStr(Tempstr, "");
    Tempstr=SetStrLen(Tempstr, 40);
    len=STREAMPeekBytes(S, Tempstr, 20);
    if (len > 6)
    {
        if ((strncmp(Tempstr, "begin ", 6)==0) || (strncmp(Tempstr, "begin-", 6)==0))
        {
            Tempstr=STREAMReadLine(Tempstr, S);
            StripTrailingWhitespace(Tempstr);
            ptr=GetToken(Tempstr, "\\S", &Token, 0);
            if (strcasecmp(Token, "begin")==0)
            {
                Tempstr=SetStrLen(Tempstr, 40);
                len=STREAMPeekBytes(S, Tempstr, 20);
                StrTrunc(Tempstr, len);
                if (*Tempstr=='h') Settings->InputEncode=ENCODE_XXENC;
                else Settings->InputEncode=ENCODE_UUENC;
            }
            else if (strcasecmp(Token, "begin-base64")==0) Settings->InputEncode=ENCODE_BASE64;
            else if (strcasecmp(Token, "begin-ibase64")==0) Settings->InputEncode=ENCODE_IBASE64;
            else if (strcasecmp(Token, "begin-rbase64")==0) Settings->InputEncode=ENCODE_RBASE64;
            else if (strcasecmp(Token, "begin-bech32")==0) Settings->InputEncode=ENCODE_BECH32;
            else if (strcasecmp(Token, "=ybegin")==0) Settings->InputEncode=ENCODE_YENCODE;

            //ptr=GetToken(ptr, "\\S", &Token, 0);
            //if (! StrValid(FName)) FName=CopyStr(FName, GetBasename(ptr));
        }
    }

    Destroy(Tempstr);
    Destroy(Token);

    return(FName);
}


char *GetOutputPath(char *RetStr, TSettings *Settings, STREAM *In)
{
    char *Tempstr=NULL;

    Tempstr=ExamineFileHeader(Tempstr, In, Settings);


    if (StrValid(Settings->OutPath)) RetStr=CopyStr(RetStr, Settings->OutPath);
    else RetStr=CopyStr(RetStr, Settings->OutPath);

    if (! StrValid(RetStr)) RetStr=CopyStr(RetStr, "stdout:");

    Destroy(Tempstr);
    return(RetStr);
}


void AddEncodingHeaders(STREAM *Out, TSettings *Settings)
{
    char *Tempstr=NULL;
    struct stat FStat;

    if (Settings->Flags & FLAG_HEADERS)
    {
        stat(Settings->InPath, &FStat);

        switch (Settings->OutputEncode)
        {
        case ENCODE_UUENC:
        case ENCODE_XXENC:
            Tempstr=FormatStr(Tempstr, "begin %o %s\n",FStat.st_mode & 511, Settings->InPath);
            break;

        case ENCODE_BASE64:
            Tempstr=FormatStr(Tempstr, "begin-base64 %o %s\n",FStat.st_mode & 511, GetBasename(Settings->InPath));
            break;

        case ENCODE_RBASE64:
            Tempstr=FormatStr(Tempstr, "begin-rbase64 %o %s\n",FStat.st_mode & 511, GetBasename(Settings->InPath));
            break;

        case ENCODE_IBASE64:
            Tempstr=FormatStr(Tempstr, "begin-ibase64 %o %s\n",FStat.st_mode & 511, GetBasename(Settings->InPath));
            break;

        case ENCODE_BECH32:
            Tempstr=FormatStr(Tempstr, "begin-bech32 %s\n",FStat.st_mode & 511, GetBasename(Settings->InPath));
            break;

        case ENCODE_YENCODE:
            Tempstr=FormatStr(Tempstr, "=ybegin mode=%o size=%lu name=%s\n",FStat.st_mode & 511, (unsigned long) FStat.st_size, GetBasename(Settings->InPath));
            break;
        }
        if (StrValid(Tempstr)) STREAMWriteLine(Tempstr, Out);
    }

    Destroy(Tempstr);
}


void AddEncodingTrailer(STREAM *Out, TSettings *Settings)
{
    char *Tempstr=NULL;
    struct stat FStat;

    if (Settings->Flags & FLAG_HEADERS)
    {
        stat(Settings->InPath, &FStat);

        switch (Settings->OutputEncode)
        {
        case ENCODE_UUENC:
            Tempstr=CopyStr(Tempstr, "`\nend\n");
            break;

        case ENCODE_XXENC:
            Tempstr=CopyStr(Tempstr, "end\n");
            break;

        case ENCODE_BASE64:
        case ENCODE_RBASE64:
        case ENCODE_IBASE64:
            Tempstr=CopyStr(Tempstr, "====\n");
            break;

        case ENCODE_YENCODE:
            Tempstr=FormatStr(Tempstr, "=yend size=%d\n",FStat.st_size);
            break;
        }
        if (StrValid(Tempstr)) STREAMWriteLine(Tempstr, Out);
    }

    Destroy(Tempstr);
}



int OutputData(STREAM *Out, const char *Decoded, int Len, int OutputEncode, int OutputLineLen)
{
    char *Output=NULL;
    const char *p_ToOutput;

    if (OutputEncode > 0)
    {
        Output=EncodeOutputLine(Output, Decoded, Len, OutputLineLen, OutputEncode);
        Len=StrLen(Output);
        p_ToOutput=Output;
    }
    else p_ToOutput=Decoded;

    if (Len > 0) STREAMWriteBytes(Out, p_ToOutput, Len);

    Destroy(Output);
    return(Len);
}


main(int argc, char *argv[])
{
    TSettings *Settings;
    STREAM *In, *Out;
    char *Decoded=NULL, *Path=NULL;
    const char *p_ToOutput;
    int len, LineLen;

    Settings=ParseCommandLine(argc, argv);
    LineLen=Settings->InputBlockSize * Settings->OutputBlockSize *100;


		#ifdef USE_SECCOMP
    ProcessApplyConfig("security=high ");
		#endif

    In=STREAMOpen(Settings->InPath, "r");
    if (In)
    {
        Path=GetOutputPath(Path, Settings, In);

        Out=STREAMOpen(Path, "w");
        if (Out)
        {
            STREAMSetFlushType(Out, FLUSH_FULL, 0, 0);
            AddEncodingHeaders(Out, Settings);
            Decoded=SetStrLen(Decoded, LineLen * 2);
            len=ReadInput(In, &Decoded, LineLen, Settings->InputEncode);
            while (len >= 0)
            {
                OutputData(Out, Decoded, len, Settings->OutputEncode, Settings->OutputLineLen);
                len=ReadInput(In, &Decoded, LineLen, Settings->InputEncode);
            }

            //flush remaining output
            while (OutputData(Out, NULL, -1, Settings->OutputEncode, Settings->OutputLineLen) > 0) ;

            AddEncodingTrailer(Out, Settings);
            STREAMClose(Out);
        }
        STREAMClose(In);
    }

    Destroy(Decoded);
    Destroy(Path);
}
