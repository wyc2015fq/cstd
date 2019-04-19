# FLV文件格式分析（附源码） - 建建的博客 - CSDN博客
2017年08月14日 08:54:04[纪建](https://me.csdn.net/u013898698)阅读数：593
## [FLV文件格式分析（附源码）](http://www.cnblogs.com/Kingfans/p/7137322.html)
FLV文件主要由两部分组成：Header和Body。
![](http://images2015.cnblogs.com/blog/693621/201707/693621-20170711011418572-1033866158.png)
# **1. Header**
header部分记录了flv的类型、版本等信息，是flv的开头，一般都差不多，占9bytes。具体格式如下：
|文件类型|3 bytes|“FLV”|
|----|----|----|
|版本|1 byte|一般为0x0000 0001 (1)|
|流信息|1 byte|倒数第一位是1表示有视频，倒数第三位是1表示有音频.其余备用字段必须为0, 音视频都具备为0x0000 0101 (5)|
|header长度|4 bytes|整个header的长度，一般为9； 大于9表示下面还有扩展信息 (9)|
# **2. Body**
body部分由一个个Tag组成，每个Tag的下面有一块4bytes的空间，用来记录这个tag的长度，这个后置用于逆向读取处理。
## **2.1.Tag**
每个Tag由也是由两部分组成的：Tag Header和Tag Data。Tag Header里存放的是当前Tag的类型、数据区（Tag Data）长度等信息，具体如下：
|**名称**|**长度**|**介绍**|
|----|----|----|
|Tag类型|1 bytes|8：音频9：视频18：脚本 (这里是一些描述信息)其他：保留|
|数据区长度|3 bytes|数据区(Tag Data)的长度|
|时间戳|3 bytes|整数，单位是毫秒。对于脚本型的tag总是0.相对于FLV文件的第一个TAG时戳。第一个tag的时戳总是0。——不是时戳增量，rtmp中是时戳增量。|
|时间戳扩展|1 bytes|将时间戳扩展为4bytes，代表高8位。很少用到|
|StreamsID|3 bytes|总是0|
## **2.2.Tag Data**
数据区根据Tag类型的不同可分为三种，音频数据、视频数据和脚本数据。
### **2.2.1.音频数据**
第一个byte是音频的信息，格式如下:
|名称|长度|介绍|
|----|----|----|
|音频格式|4 bits|0 = Linear PCM, platform endian1 = ADPCM2 = MP33 = Linear PCM, little endian4 = Nellymoser 16-kHz mono5 = Nellymoser 8-kHz mono6 = Nellymoser7 = G.711 A-law logarithmic PCM8 = G.711 mu-law logarithmic PCM9 = reserved10 = AAC11 = Speex14 = MP3 8-Khz15 = Device-specific sound|
|采样率|2 bits|0 = 5.5-kHz1 = 11-kHz2 = 22-kHz3 = 44-kHz对于AAC总是3|
|采样的长度|1 bit|0 = snd8Bit1 = snd16Bit压缩过的音频都是16bit|
|音频类型|1 bit|0 = sndMono1 = sndStereo对于AAC总是1|


第2byte开始就是音频流数据了。
### **2.2.2.视频数据**
和音频数据一样，第一个byte是视频信息，格式如下：
|名称|长度|介绍|
|----|----|----|
|帧类型|4 bits|1: keyframe (for AVC, a seekable frame)2: inter frame (for AVC, a non-seekable frame)3: disposable inter frame (H.263 only)4: generated keyframe (reserved for server use only)5: video info/command frame|
|编码ID|4 bits|1: JPEG (currently unused)2: Sorenson H.2633: Screen video4: On2 VP65: On2 VP6 with alpha channel6: Screen video version 27: AVC|
#### 2.2.3脚本数据
脚本Tag一般只有一个，是flv的第一个Tag，用于存放flv的信息，比如duration、audiodatarate、creator、width等等信息。
首先介绍下脚本的数据类型。所有数据都是以**数据类型+（数据长度）+数据**的格式出现的，数据类型占1byte，数据长度看数据类型是否存在，后面才是数据。
可参考AMF数据格式。
其中数据类型的种类有：
- 0 = Number type
- 1 = Boolean type
- 2 = String type
- 3 = Object type
- 4 = MovieClip type
- 5 = Null type
- 6 = Undefined type
- 7 = Reference type
- 8 = ECMA array type
- 10 = Strict array type
- 11 = Date type
- 12 = Long string type
如果类型为String，后面的2bytes为字符串的长度（Long String是4bytes），再后面才是字符串数据；如果是Number类型，后面的8bytes为Double类型的数据；Boolean类型，后面1byte为Bool类型。
知道了这些后再来看看flv中的脚本，一般开头是0x02，表示String类型，后面的2bytes为字符串长度，一般是0x000a（“onMetaData”的长度），再后面就是字符串“onMetaData”。好像flv格式的文件都有onMetaData标记，在运行ActionScript的时候会用到它。后面跟的是0x08，表示ECMA Array类型，这个和Map比较相似，一个键跟着一个值。键都是String类型的，所以开头的0x02被省略了，直接跟着的是字符串的长度，然后是字符串，再是值的类型，也就是上面介绍的那些了。
文件内存显示：
![](http://images2015.cnblogs.com/blog/693621/201707/693621-20170711102552212-1943994959.png)
# **3. 源码解析**
flv的格式还是比较简单的，header部分很简洁，body部分都是由一个个tag，tag的话也就三种，脚本tag一般只有一个的，我想这也是flv能成为在线视频格式的原因吧。
只要了解了格式，我们就可以写个程序来解析flv文件了，下面我们看源码解析：
FLVFile.h
```
#pragma once
#include "VideoFile.h"
/*      PACKET_TYPE_...                0x00 */
#define PACKET_TYPE_CHUNK_SIZE         0x01
/*      PACKET_TYPE_...                0x02 */
#define PACKET_TYPE_BYTES_READ_REPORT  0x03
#define PACKET_TYPE_CONTROL            0x04
#define PACKET_TYPE_SERVER_BW          0x05
#define PACKET_TYPE_CLIENT_BW          0x06
/*      PACKET_TYPE_...                0x07 */
#define PACKET_TYPE_AUDIO              0x08
#define PACKET_TYPE_VIDEO              0x09
/*      PACKET_TYPE_...                0x0A */
/*      PACKET_TYPE_...                0x0B */
/*      PACKET_TYPE_...                0x0C */
/*      PACKET_TYPE_...                0x0D */
/*      PACKET_TYPE_...                0x0E */
#define PACKET_TYPE_FLEX_STREAM_SEND   0x0F
#define PACKET_TYPE_FLEX_SHARED_OBJECT 0x10
#define PACKET_TYPE_FLEX_MESSAGE       0x11
#define PACKET_TYPE_SCRIPT             0x12
#define PACKET_TYPE_SHARED_OBJECT      0x13
#define PACKET_TYPE_INVOKE             0x14
/*      PACKET_TYPE_...                0x15 */
#define PACKET_TYPE_FLASH_VIDEO        0x16
/************************************************************************************************************
header部分记录了flv的类型、版本等信息，是flv的开头，一般都差不多，占9bytes.
具体格式如下;
文件类型    　　 3 bytes        "FLV";
版本            1 byte        一般为0x01;
流信息          1 byte        倒数第一位是1表示有视频，倒数第三位是1表示有音频，倒数第二、四位必须为0;
header长度      4 bytes       整个header的长度，一般为9；大于9表示下面还有扩展信息;
************************************************************************************************************/
struct FlvHeader
{
    byte Type[3];
    byte Version;
    byte StreamInfo;
    byte HeaderSize[4];
};
/************************************************************************************************************
每个Tag由也是由两部分组成的：Tag Header和Tag Data;
Tag Header里存放的是当前Tag的类型、数据区（Tag Data）长度等信息;
具体如下;
名称              长度                      介绍;
Tag类型           1 bytes                    8：音频 \ 9：视频 \ 18：脚本 \  其他：保留;
数据区长度         3 bytes                    在数据区的长度;
时间戳            3 bytes                    整数，单位是毫秒 对于脚本型的tag总是0;
时间戳扩展         1 bytes                    将时间戳扩展为4bytes,代表高8位 很少用到;
StreamsID        3 bytes                    总是0;
************************************************************************************************************/
struct TagHeader
{
    byte TagType;
    byte DataSize[3];
    byte Timestamp[3];
    byte TimeExtend;
    byte StreamsID[3];
};
class CFLVFile: public CVideoFile
{
public:
    CFLVFile();
    virtual ~CFLVFile();
    virtual bool ParseFile();
protected:
    bool ParseFLVHeader();
    bool ParseFLVBody();
    bool ParseAudioData(int iDataSize);
    bool ParseVideoData(int iDataSize);
    bool ParseScriptData(int iDataSize);
};
```
FLVFile.cpp
```
#include "FLVFile.h"
#include "amf.h"
CFLVFile::CFLVFile()
{
}
CFLVFile::~CFLVFile()
{
}
bool CFLVFile::ParseFile()
{
    if (!ParseFLVHeader())
    {
        return false;
    }
    if (!ParseFLVBody())
    {
        return false;
    }
    return true;
}
bool CFLVFile::ParseFLVHeader()
{
    printf("******************************FLV Header******************************\n");
    // FLV Header;
    {
        FlvHeader flvHeader;
        int iSize = sizeof(FlvHeader);
        if (iSize != fread(&flvHeader, 1, iSize, m_pFile))
        {
            printf("Read FLV Header is Error. \n");
            return false;
        }
        printf("\t File Type     \t : %c %c %c \n", flvHeader.Type[0], flvHeader.Type[1], flvHeader.Type[2]);
        printf("\t Version       \t : %d \n", flvHeader.Version);
        printf("\t Stream Info   \t : %d \n", flvHeader.StreamInfo);
        printf("\t Header Length \t : %d \n\n", ByteToInt(flvHeader.HeaderSize, sizeof(flvHeader.HeaderSize)));
    }
    return true;
}
bool CFLVFile::ParseFLVBody()
{
    printf("******************************FLV Body******************************\n");
    bool bStop = false;
    while (!bStop)
    {
        printf("\n      ************************Tag Header************************      \n");
        _getw(m_pFile);
        TagHeader tagHeader;
        int iTagHeaderSize = sizeof(TagHeader);
        if (iTagHeaderSize != fread(&tagHeader, 1, iTagHeaderSize, m_pFile))
        {
            // 读完了;
            printf("\t Read File Finished. \n");
            bStop = true;
        }
        printf("\t Tag Type   \t : %d \n", tagHeader.TagType);
        printf("\t DataSize   \t : %d \n", ByteToInt(tagHeader.DataSize, sizeof(tagHeader.DataSize)));
        printf("\t Timestamp  \t : %d \n", ByteToInt(tagHeader.Timestamp, sizeof(tagHeader.Timestamp)));
        printf("\t TimeExtend \t : %d \n", tagHeader.TimeExtend);
        printf("\t StreamsID  \t : %d \n", ByteToInt(tagHeader.StreamsID, sizeof(tagHeader.StreamsID)));
        printf("      ************************Tag Body************************      \n");
        const int iDataSize = ByteToInt(tagHeader.DataSize, sizeof(tagHeader.DataSize));
        switch (tagHeader.TagType)
        {
        case PACKET_TYPE_AUDIO:
            {
                // 音频数据;
                ParseAudioData(iDataSize);
            }
            break;
        case PACKET_TYPE_VIDEO:
            {
                // 视频数据;
                ParseVideoData(iDataSize);
            }
            break;
        case PACKET_TYPE_SCRIPT:
            {
                // 类型数据;
                ParseScriptData(iDataSize);
            }
            break;
        default:
            {
                printf("\t Read Tag Body %d bytes. \n\n", iDataSize);
                fseek(m_pFile, iDataSize, SEEK_CUR);
            }
            break;
        }
    }
    return true;
}
/************************************************************************************************************
    第一个字节是音频信息格式,格式如下;
    名称            长度                    介绍;
    
    音频格式        4 bits                    
                                            0 = Linear PCM, platform endian
                                            1 = ADPCM
                                            2 = MP3
                                            3 = Linear PCM, little endian
                                            4 = Nellymoser 16-kHz mono
                                            5 = Nellymoser 8-kHz mono
                                            6 = Nellymoser
                                            7 = G.711 A-law logarithmic PCM
                                            8 = G.711 mu-law logarithmic PCM
                                            9 = reserved
                                            10 = AAC
                                            11 = Speex
                                            14 = MP3 8-Khz
                                            15 = Device-specific sound
    采样率            2 bits                    (对于AAC总是3)
                                            0 = 5.5-kHz
                                            1 = 11-kHz
                                            2 = 22-kHz
                                            3 = 44-kHz
    采样的长度        1 bit                    (压缩过的音频总是16bit)
                                            0 = snd8Bit
                                            1 = snd16Bit
    音频类型        1 bit                    (对于AAC总是1)
                                            0 = sndMono
                                            1 = sndStereo
    第二个字节开始就是音频流数据了;
************************************************************************************************************/
bool CFLVFile::ParseAudioData(int iDataSize)
{
    // 解析音频格式;
    byte audioHeader;
    fread(&audioHeader, 1, sizeof(audioHeader), m_pFile);
    char strAudioInfo[100] = { 0 };
    {
        byte audioFormat = audioHeader >> 4;
        switch (audioFormat)
        {
        case 0:strcat(strAudioInfo, "Linear PCM, platform endian"); break;
        case 1:strcat(strAudioInfo, "ADPCM"); break;
        case 2:strcat(strAudioInfo, "MP3"); break;
        case 3:strcat(strAudioInfo, "Linear PCM, little endian"); break;
        case 4:strcat(strAudioInfo, "Nellymoser 16-kHz mono"); break;
        case 5:strcat(strAudioInfo, "Nellymoser 8-kHz mono"); break;
        case 6:strcat(strAudioInfo, "Nellymoser"); break;
        case 7:strcat(strAudioInfo, "G.711 A-law logarithmic PCM"); break;
        case 8:strcat(strAudioInfo, "G.711 mu-law logarithmic PCM"); break;
        case 9:strcat(strAudioInfo, "reserved"); break;
        case 10:strcat(strAudioInfo, "AAC"); break;
        case 11:strcat(strAudioInfo, "Speex"); break;
        case 14:strcat(strAudioInfo, "MP3 8-Khz"); break;
        case 15:strcat(strAudioInfo, "Device-specific sound"); break;
        default:strcat(strAudioInfo, "UNKNOWN"); break;
        }
        strcat(strAudioInfo, "| ");
    }
    
    {
        byte sampBits = audioHeader << 4;
        sampBits = sampBits >> 6;
        switch (sampBits)
        {
        case 0:strcat(strAudioInfo, "5.5-kHz"); break;
        case 1:strcat(strAudioInfo, "1-kHz"); break;
        case 2:strcat(strAudioInfo, "22-kHz"); break;
        case 3:strcat(strAudioInfo, "44-kHz"); break;
        default:strcat(strAudioInfo, "UNKNOWN"); break;
        }
        strcat(strAudioInfo, "| ");
    }
    {
        byte sampLen = audioHeader << 6;
        sampLen = sampLen >> 7;
        switch (sampLen)
        {
        case 0:strcat(strAudioInfo, "8Bit"); break;
        case 1:strcat(strAudioInfo, "16Bit"); break;
        default:strcat(strAudioInfo, "UNKNOWN"); break;
        }
        strcat(strAudioInfo, "| ");
    }
    {
        byte audioType = audioHeader << 7;
        audioType = audioType >> 7;
        switch (audioType)
        {
        case 0:strcat(strAudioInfo, "Mono"); break;
        case 1:strcat(strAudioInfo, "Stereo"); break;
        default:strcat(strAudioInfo, "UNKNOWN"); break;
        }
        strcat(strAudioInfo, "| ");
    }
    printf("\t %s audio data: %d bytes \n", strAudioInfo, iDataSize - 1);
    fseek(m_pFile, iDataSize - 1, SEEK_CUR);
    return true;
}
/************************************************************************************************************
第一个字节是视频信息格式,格式如下;
    名称            长度                    介绍;
    帧类型            4 bits 
                                            1: keyframe(for AVC, a seekable frame)
                                            2 : inter frame(for AVC, a non - seekable frame)
                                            3 : disposable inter frame(H.263 only)
                                            4 : generated keyframe(reserved for server use only)
                                            5 : video info / command frame
    编码ID            4 bits                    
                                            1 : JPEG(currently unused)
                                            2 : Sorenson H.263
                                            3 : Screen video
                                            4 : On2 VP6
                                            5 : On2 VP6 with alpha channel
                                            6 : Screen video version 2
                                            7 : AVC
第二个字节开始就是视频流数据了;
************************************************************************************************************/
bool CFLVFile::ParseVideoData(int iDataSize)
{
    // 解析音频格式;
    byte videoHeader;
    fread(&videoHeader, 1, sizeof(videoHeader), m_pFile);
    char strVideoInfo[100] = { 0 };
    {
        byte frameType = videoHeader >> 4;
        switch (frameType)
        {
        case 1:strcat(strVideoInfo, "key frame  "); break;
        case 2:strcat(strVideoInfo, "inter frame"); break;
        case 3:strcat(strVideoInfo, "disposable inter frame"); break;
        case 4:strcat(strVideoInfo, "generated keyframe"); break;
        case 5:strcat(strVideoInfo, "video info/command frame"); break;
        default:strcat(strVideoInfo, "UNKNOWN"); break;
        }
        strcat(strVideoInfo, "| ");
    }
    {
        byte sampBits = videoHeader << 4;
        sampBits = sampBits >> 4;
        switch (sampBits)
        {
        case 1:strcat(strVideoInfo, "JPEG (currently unused)"); break;
        case 2:strcat(strVideoInfo, "Sorenson H.263"); break;
        case 3:strcat(strVideoInfo, "Screen video"); break;
        case 4:strcat(strVideoInfo, "On2 VP6"); break;
        case 5:strcat(strVideoInfo, "On2 VP6 with alpha channel"); break;
        case 6:strcat(strVideoInfo, "Screen video version 2"); break;
        case 7:strcat(strVideoInfo, "AVC"); break;
        default:strcat(strVideoInfo, "UNKNOWN"); break;
        }
        strcat(strVideoInfo, "| ");
    }
    printf("\t %s audio data: %d bytes \n", strVideoInfo, iDataSize - 1);
    fseek(m_pFile, iDataSize - 1, SEEK_CUR);
    return true;
}
/************************************************************************************************************
    脚本Tag一般只有一个，是flv的第一个Tag;
    用于存放flv的信息，比如duration、audiodatarate、creator、width等;
    首先介绍下脚本的数据类型;
    所有数据都是以数据类型+(数据长度)+数据的格式出现的，数据类型占1byte，数据长度看数据类型是否存在，后面才是数据;
    AMF数据格式解析;
************************************************************************************************************/
bool CFLVFile::ParseScriptData(int iDataSize)
{
    // 解析类型信息;
    byte* pData = new byte[iDataSize];
    byte* pDataEnd = pData + iDataSize;
    fread(pData, 1, iDataSize, m_pFile);
    while (pDataEnd - pData > 0)
    {
        switch (pData[0])
        {
        case AMF_NUMBER:        // 数字(double);
            {
                //double dVal = AMF_DecodeNumber(pFileFlv);
            }
            break;
        case AMF_BOOLEAN:        // 布尔;
            {
            }
            break;
        case AMF_STRING:        // 字符串;
            {
                pData = pData + 1;
                AVal valName;
                AMF_DecodeString((char*)pData, &valName);
                pData = pData + (valName.av_len + 2);
            }
            break;
        case AMF_OBJECT:        // 对象;
            {
            }
            break;
        case AMF_MOVIECLIP:        // 保留,未使用;
            break;
            // AMF_NULL,                // null;
            // AMF_UNDEFINED,            // 未定义;
            // AMF_REFERENCE,            // 引用;
        case AMF_ECMA_ARRAY:            // 数组;
            {
                pData = pData + 1;
                // 数组元素个数;
                int iArrLen = ByteToInt(pData, 4);
                pData = pData + 4;
                AMFObject obj;
                int iSize = pDataEnd - pData;
                int nRes = AMF_DecodeArray(&obj, (char*)pData, iSize, iArrLen, TRUE);
                if (nRes == -1)
                {
                    return false;
                }
                for (int i = 0; i < iArrLen; i++)
                {
                    std::string strName(obj.o_props[i].p_name.av_val, obj.o_props[i].p_name.av_len);
                    switch (obj.o_props[i].p_type)
                    {
                    case AMF_NUMBER:            // 数字(double);
                        {
                            printf("\t %s : %.0f\n", strName.c_str(), obj.o_props[i].p_vu.p_number);
                        }
                        break;
                    case AMF_STRING:            // 字符串; 
                        {
                            std::string strValue(obj.o_props[i].p_vu.p_aval.av_val, obj.o_props[i].p_vu.p_aval.av_len);
                            printf("\t %s : %s\n", strName.c_str(), strValue.c_str());
                        }
                        break;
                    default:
                        break;
                    }
                }
                pData = pData + iSize;
            }
            break;
            // AMF_OBJECT_END,            // 对象结束(0x09);
            // AMF_STRICT_ARRAY,        // 严格的数组;
            // AMF_DATE,                // 日期;
            // AMF_LONG_STRING,            // 长字符串;
            // AMF_UNSUPPORTED,            // 未支持;
            // AMF_RECORDSET,            // 保留,未使用;
            // AMF_XML_DOC,                // xml文档;
            // AMF_TYPED_OBJECT,        // 有类型的对象;
            // AMF_AVMPLUS,                // 需要扩展到AMF3;
            // AMF_INVALID = 0xff        // 无效的;
        default:
            break;
        }
    }
    
    return true;
}
```
main.cpp
```
#include <windows.h>
#include "FLVFile.h"
#include <assert.h>
// FLV 文件解析;
bool FLVParse_Test()
{
    CFLVFile flvFile;
    return flvFile.LoadFile("../testfile/flv_test.flv");
}
void main()
{
    // FLV;
    bool bRes = FLVParse_Test();
    assert(bRes);
}
```
运行结果：
![](http://images2015.cnblogs.com/blog/693621/201707/693621-20170711100740665-1002978645.png)
项目源码下载地址：[https://github.com/kingsunc/AVFileParse](https://github.com/kingsunc/AVFileParse)
