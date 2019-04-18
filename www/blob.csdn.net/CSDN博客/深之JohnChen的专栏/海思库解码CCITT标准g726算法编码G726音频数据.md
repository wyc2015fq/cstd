# 海思库解码CCITT标准g726算法编码G726音频数据 - 深之JohnChen的专栏 - CSDN博客

2017年11月02日 23:01:26[byxdaz](https://me.csdn.net/byxdaz)阅读数：1088


海思库解码CCITT标准g726算法编码G726音频数据，海思库解码需要使用MEDIA_G726_XXX格式解码音频数据。MEDIA_G726_XXX格式包括以下几种：

#define
MEDIA_G726_16KBPS             0x24  /* G726 16kbps forASF ... */

#define
MEDIA_G726_24KBPS             0x25  /* G726 24kbps forASF ... */

#define
MEDIA_G726_32KBPS             0x26  /* G726 32kbps forASF ... */

#defineMEDIA_G726_40KBPS             0x27 
/* G726 40kbps forASF ... */

//编码：使用CCITT标准g726算法将pcm编码成g726，再增加海思4字节头，将g726变成海思g726

g726_state_t*pSate726 = (g726_state_t*)malloc(sizeof(g726_state_t));

pSate726= 
g726_init(pSate726,16*1000);//16kps

CStringfilePath =
"";

CStringnewlFilePath =
"";

charszFilter[] = {"PcmFiles (*.pcm)|*.pcm||"};

CFileDialogdlg(TRUE,NULL,NULL,OFN_HIDEREADONLY
 | OFN_OVERWRITEPROMPT,szFilter,NULL);

if(dlg.DoModal() ==IDOK)

    {

filePath= 
dlg.GetPathName();

newlFilePath= 
filePath;

newlFilePath.Replace(".pcm","_ff.g726_hisiWithFFMpeg");

FILE* 
fpSrc = fopen(filePath.GetBuffer(filePath.GetLength()),"rb");

FILE* 
fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");

charszData[320] = {0};

unsignedcharszOutData[200]= {0};

int
nDataLen =320;

int
nOutDataLen= 200;

int
nReadedSize= 0;

unsignedshortusHisiHeader[2]= {0};

usHisiHeader[0]= 0x0100;

int
ret = 0;

if(fpSrc !=
NULL)

        {

while(TRUE)

            {

nReadedSize = 
fread(szData,sizeof(char),nDataLen,fpSrc);

if(nReadedSize<
nDataLen)

                {

break;

                }

int
iRet = g726_encode(pSate726,szOutData, (short*)szData,
nReadedSize/2);

nOutDataLen = 
iRet;

if (nOutDataLen> 0)

                {

usHisiHeader[1] = (usHisiHeader[1]& 0xff00) +
nOutDataLen/sizeof(unsignedshort);

//写入hisi头

fwrite(usHisiHeader,sizeof(unsignedshort),2,fpDst);

//写入数据

int
iiiii = fwrite(szOutData,1,
nOutDataLen, fpDst);

                }

            }

fclose(fpSrc);

fclose(fpDst);

        }

    }

if(pSate726 !=
NULL)

    {

free(pSate726);

pSate726= 
NULL;

    }

//解码

//hisig726 to pcm

CStringfilePath =
"";

CStringnewlFilePath =
"";

charszFilter[] = {"g726Files (*.g726_hisiWithFFMpeg)|*.g726_hisiWithFFMpeg||"};

CFileDialogdlg(TRUE,NULL,NULL,OFN_HIDEREADONLY
 | OFN_OVERWRITEPROMPT,szFilter,NULL);

if(dlg.DoModal()==
IDOK)

    {

filePath= 
dlg.GetPathName();

newlFilePath= 
filePath;

newlFilePath.Replace(".g726_hisiWithFFMpeg","_ff2hisi.pcm");

BOOLbRet = 0;

CG726ADecoderg726Decoder;

g726Decoder.DecoderInit(MEDIA_G726_16KBPS);

FILE * 
fpSrc= fopen(filePath.GetBuffer(filePath.GetLength()),"rb");

FILE* 
fpDst = fopen(newlFilePath.GetBuffer(newlFilePath.GetLength()),"wb+");

char
szData[104] = {0};

char
szOutData[320]= {0};

int
nDataLen = 104;

int
nOutDataLen = 320;

int
nReadedSize= 0;

unsignedshortusHisiHeader[2]= {0};

if(fpSrc !=
NULL)

        {

while(TRUE)

            {

//读取头标记

nDataLen = 
Hisi_AUDIO_HERDER_LEN;

nReadedSize = 
fread(szData,sizeof(char),nDataLen,fpSrc);

if(nReadedSize<
nDataLen)

                {

break;

                }

memcpy(usHisiHeader,szData,Hisi_AUDIO_HERDER_LEN);

int
nAudioFrameDataLen= (usHisiHeader[1] & 0x00ff) *
sizeof(unsigned
short);

nDataLen = 
nAudioFrameDataLen;

//读取音频帧数据

nReadedSize = 
fread(szData+Hisi_AUDIO_HERDER_LEN,sizeof(char),nDataLen,fpSrc);

if(nReadedSize<
nDataLen)

                {

break;

                }

bRet = 
g726Decoder.AFrameDecoder(szData,nReadedSize+Hisi_AUDIO_HERDER_LEN,szOutData,&nOutDataLen);

if(bRet)

                {

fwrite(szOutData,sizeof(char),nOutDataLen,fpDst);

                }

            }

fclose(fpSrc);

fclose(fpDst);

        }

    }

[代码下载](http://download.csdn.net/download/byxdaz/10051164)

