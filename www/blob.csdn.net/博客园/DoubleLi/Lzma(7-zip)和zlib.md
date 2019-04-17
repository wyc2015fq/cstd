# Lzma(7-zip)和zlib - DoubleLi - 博客园






**Lzma(7-zip) 使用：**

在C目录中有算法文件，进入Util\LzmaLib目录，编译生成LIB库，导出了以下两函数，LzmaCompress 为压缩函数，LzmaUncompress 为解压缩函数。

MY_STDAPI LzmaCompress(unsigned char *dest, size_t *destLen, const unsigned char *src, size_t srcLen,
  unsigned char *outProps, size_t *outPropsSize,
  int level,     
  unsigned dictSize, 
  int lc,       
  int lp,       
  int pb,       
  int fb,       
  int numThreads
  );

MY_STDAPI LzmaUncompress(unsigned char *dest, size_t *destLen, const unsigned char *src, SizeT *srcLen,
  const unsigned char *props, size_t propsSize);

导入Types.h 和 Lzmalib.h 到工程中。

代码如下：

#include "stdafx.h"
#include "LzmaLib.h"

#pragma comment(lib,"lzma.lib")

int _tmain(int argc, _TCHAR* argv[])
{
    FILE* pFile = _tfopen(_T("file.dat"), _T("rb"));

    if (pFile == NULL)
    {
        _ftprintf(stderr, _T("Error to Open the file!"));
        return  - 1;
    }

    fseek(pFile, 0, SEEK_END);
    size_t srcLen = ftell(pFile);
    rewind(pFile);
    size_t destLen = srcLen * 2;
    unsigned char* psrcRead = new unsigned char[srcLen]; //原始文件数据
    unsigned char* pDecomress = new unsigned char[srcLen]; //存放解压缩数据
    unsigned char* pLzma = new unsigned char[destLen]; //存放压缩数据

    fread(psrcRead, sizeof(char), srcLen, pFile);

    unsigned char prop[5] = 
    {
        0
    };
    size_t sizeProp = 5;

    if (SZ_OK != LzmaCompress(pLzma, &destLen, psrcRead, srcLen, prop,
        &sizeProp, 9, (1 << 24), 3, 0, 2, 32, 2))
    {
        //出错了
        _ftprintf(stderr, _T("压缩时出错！"));
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;
        fclose(pFile);
        return  - 1;
    }

    FILE* pCompressFile = _tfopen(_T("compress.dat"), _T("wb")); 
    //写入压缩后的数据
    if (pCompressFile == NULL)
    {
        _ftprintf(stderr, _T("创建文件出错!"));
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;
        fclose(pFile);
        return  - 1;
    }
    fwrite(pLzma, sizeof(char), destLen, pCompressFile);
    fclose(pCompressFile);

    FILE* pDecompressFile = _tfopen(_T("decompress.dat"), _T("wb")); 
    //写入解压缩数据
    if (pDecompressFile == NULL)
    {
        _ftprintf(stderr, _T("写入数据出错！"));
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;
        fclose(pFile);
        return  - 1;
    }

    //注意：解压缩时props参数要使用压缩时生成的outProps，这样才能正常解压缩
    if (SZ_OK != LzmaUncompress(pDecomress, &srcLen, pLzma, &destLen, prop, 5))
    {
        delete [] psrcRead;
        delete [] pDecomress;
        delete [] pLzma;

        fclose(pDecompressFile);
        fclose(pFile);
        return  - 1;
    }

    fwrite(pDecomress, sizeof(char), srcLen, pDecompressFile);

    delete [] psrcRead;
    delete [] pDecomress;
    delete [] pLzma;

    fclose(pDecompressFile);
    fclose(pFile);

    return 0;
}


**zlib使用：**
zlib 是通用的压缩库，提供了一套 in-memory 压缩和解压函数，并能检测解压出来的数据的完整性(integrity)。下面介绍两个最有用的函数——compress 和 uncompress。

int compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

compress函数将 source 缓冲区中的内容压缩到 dest 缓冲区。 sourceLen 表示source 缓冲区的大小(以字节计)。注意函数的第二个参数 destLen 是传址调用。当调用函数时，destLen表示 dest 缓冲区的大小，destLen > (sourceLen + 12)*100.1%。当函数退出后，destLen 表示压缩后缓冲区的实际大小。此时 destLen / sourceLen 正好是压缩率。

compress 若成功，则返回 Z_OK；若没有足够内存，则返回 Z_MEM_ERROR；若输出缓冲区不够大，则返回 Z_BUF_ERROR。

int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

uncompress 函数将 source 缓冲区的内容解压缩到 dest 缓冲区。sourceLen 是 source 缓冲区的大小(以字节计)。注意函数的第二个参数 destLen 是传址调用。当调用函数时，destLen 表示 dest 缓冲区的大小， dest 缓冲区要足以容下解压后的数据。在进行解压缩时，需要提前知道被压缩的数据解压出来会有多大。这就要求在进行压缩之前，保存原始数据的大小(也就是解压后的数据的大小)。这不是 zlib 函数库的功能，需要我们做额外的工作。当函数退出后， destLen 是解压出来的数据的实际大小。

uncompress 若成功，则返回 Z_OK ；若没有足够内存，则返回 Z_MEM_ERROR；若输出缓冲区不够大，则返回 Z_BUF_ERROR。若输入数据有误，则返回 Z_DATA_ERROR。

代码如下：

#include "stdafx.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "zlib.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    int err = 0;
 Byte compr[200] = {0}, uncompr[200] = {0}; // big enough
    uLong comprLen = 0, uncomprLen = 0;

 const char* hello = "12345678901234567890123456789012345678901234567890";

 uLong len = strlen(hello) + 1;
    comprLen = sizeof(compr) / sizeof(compr[0]);

 err = compress(compr, &comprLen, (const Bytef*)hello, len);

 if (err != Z_OK)
    {
        cerr << "compess error: " << err << '\n';
        exit(1);
    }

 cout << "orignal size: " << len 
     << " , compressed size : " << comprLen << '\n';

 strcpy((char*)uncompr, "garbage");

 err = uncompress(uncompr, &uncomprLen, compr, comprLen);

 if (err != Z_OK)
    {
        cerr << "uncompess error: " << err << '\n';
        exit(1);
    }

 cout << "orignal size: " << len 
     << " , uncompressed size : " << uncomprLen << '\n';

 if (strcmp((char*)uncompr, hello))
    {
        cerr << "BAD uncompress!!!\n";
        exit(1);
    }
    else
    {
        cout << "uncompress() succeed: \n" << (char*)uncompr;
    }
}









