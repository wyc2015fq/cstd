# 使用OpenExif修改jpeg图片信息 - 小平子的专栏 - CSDN博客





2015年05月01日 12:43:45[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：3459








![版權聲明](https://img-blog.csdn.net/20160117172627181)

## 简介

目前的手机是小米手机，使用两年多里面总共存储了将近4000张照片。拍摄时系统会记录当时拍摄的时间，这个拍摄时间会记录在jpg文件的exif数据结构中，windows系统下右键单击文件可以看到这个时间。但是不知什么原因有一些照片的拍摄时间都变成了2002年12月8日，然而照片文件的命名还是按照当时实际拍摄的时间来命名。而我的手机相册设置又是按照拍摄时间来排序，因此相册的照片排序实在太混乱，后来终于忍受不了决定修改照片文件元数据里的拍摄时间。

照片是jpg格式，内置exif数据结构，Exif按照JPEG的规格在JPEG中插入一些 图像/数字相机 的信息数据以及缩略图像，于是能通过与JPEG兼容的互联网浏览器/图片浏览器/图像处理等一些软件来查看Exif格式的图像文件。就跟浏览通常的JPEG图像文件一样。关于exif的信息和结构可以通过如下两个链接查看： 
[Exif文件格式描述](http://www.cppblog.com/lymons/archive/2010/02/23/108266.html)
[EXIF Tags](http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/EXIF.html)

第一个链接是关于exif的详细介绍，第二个是exif标签的索引，写程序时主要参考的是第二个链接，查找标签比较方便。
## OpenExif

在网上找了好久都没找到相关修改照片exif信息的库，很多库都是只能读取exif信息但不能修改。最后只找到C++的OpenExif。本想用python写可能会比较简单，后来只能用麻烦的C++了。

OpenExif是处理exif的一个开源工具，可以从这里下载：[OpenExif](http://openexif.sourceforge.net/)

OpenExif下载后里面有示例程序，可以方便看到代码是如何运行的。

## 修改方案

小米手机拍摄的照片都是以IMG开头，命名格式为IMG_YYYYMMDD_HHmmSS。文件命名的时间是照片生成的时间，一般情况下和拍摄时间非常相近。因此只需判断文件名中的时间和元数据中的拍摄时间是否一样，比较时只比较年月日，若不一样就用文件名中的时间替换掉拍摄时间。

因为手机设置了拍照自动记录GPS的功能，因此多做了一步，提取照片时间信息时顺便也提取GPS信息。

## 代码处理过程

OpenExif有一个操作照片的类ExifImageFile和操作Exif tag的类ExifTagEntry。ExifTagEntry是一个抽象基类。整个过程基本上只用到了这两个类，比较麻烦的是需要知道各类tag的名称。基本的定义和操作如下。

```bash
ExifImageFile   inImageFile;
ExifTagEntry*   genericTagEntry = NULL;
ExifStatus      status;

status = inImageFile.open(chImgPath, "r+");
if (status != EXIF_OK)
{
    // Error processing...
    inImageFile.close();
}

genericTagEntry = inImageFile.getGenericTag(EXIFTAG_DATETIMEORIGINAL, EXIF_APP1_EXIFIFD, status);
if (status != EXIF_OK || NULL == genericTagEntry)
{
    //Error processing...
}
else
{
    // do something...
}
```

其中getGenericTag函数的原型是
`ExifTagEntry* getGenericTag(exiftag_t tag, ExifTagLocation tagLocation, ExifStatus& errRtn);`
拍摄时间的tag名称是EXIFTAG_DATETIMEORIGINAL，而tagLocation可以在[ExifTags](http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/EXIF.html)中查到。

处理的代码如下：

```cpp
#include "ExifImageDescUtils.h"
#include "ExifImageFile.h"
#include "ExifMisc.h"
#include <io.h>
#include <string>
#include <direct.h>
#include <fstream>
#include <strstream>  
#include <sstream>
using namespace std;

// 修改照片的時間
void ModifyMetaTime(vector<string> &PathVector)
{
    if(PathVector.size() <= 0)
    {
        cout<<"No jpg file."<<endl;
        return;
    }

    int                         nModifyCount = 0; // 记录修改过时间的照片数量
    int                         nTotalCount = 0; // 小米手机拍的照片数量，IMG开头
    ofstream                    file;
    ExifImageFile               inImageFile;
    ExifTagEntry*               genericTagEntry = NULL;
    ExifStatus                  status;
    vector<string>::iterator    iter = PathVector.begin();

    file.open("K:\\00-Mobile Phone Pics Test\\modifylog.txt", ios::out | ios::_Noreplace | ios::trunc);
    cout << "OriginalTime" << "         " << "ModifiedTime" << endl;
    file << "FileName,OriginalTime,ModifyedTime,status" << endl;
    while (iter != PathVector.end())
    {       
        string str = *iter;
        const char* chPath = str.c_str();

        // 目录格式固定：K:\00-Mobile Phone Pics Test
        char chTag[4] = "\0";
        strncpy_s(chTag, chPath+29, 3);

        // 不是本手机的照片
        if (strcmp(chTag, "IMG") != 0)
        {
            iter++;
            continue;
        }
        nTotalCount++;

        char chYear[6] = "\0";
        char chMonth[4] = "\0";
        char chDay[4] = "\0";
        char chHour[4] = "\0";
        char chMinute[4] = "\0";
        char chSecond[4] = "\0";
        char chTime[20] = "\0";
        strncpy_s(chYear, chPath+33, 4);
        strncpy_s(chMonth, chPath+37, 2);
        strncpy_s(chDay, chPath+39, 2);
        strncpy_s(chHour, chPath+42, 2);
        strncpy_s(chMinute, chPath+44, 2);
        strncpy_s(chSecond, chPath+46, 2);
        strncat_s(chTime, chYear, 4);
        strncat_s(chTime, ":", 1);
        strncat_s(chTime, chMonth, 2);
        strncat_s(chTime, ":", 1);
        strncat_s(chTime, chDay, 2);
        strncat_s(chTime, " ", 1);
        strncat_s(chTime, chHour, 2);
        strncat_s(chTime, ":", 1);
        strncat_s(chTime, chMinute, 2);
        strncat_s(chTime, ":", 1);
        strncat_s(chTime, chSecond, 2);

        ExifStatus openStatus = inImageFile.open(chPath, "r+");
        if (openStatus != EXIF_OK) // 不能用w打开照片，打开后会重新创建一张照片并将原来的照片覆盖掉
        {
            cout << "Error: could not open " << chPath << endl;
            file << *(iter) << " , ," << "1" << endl;
            iter++;
            inImageFile.close();
            continue;
        }

        // 用getGenericTag函数获取指定的时间信息
        // 然后实例化一个ExifTagEntryT，用setGenericTag设置时间
        // 定义一个基类指针指向ExifTagEntryT的对象，然后该指针就可以调用setValue
        // 之后用setGenericTag设置时间

        genericTagEntry = inImageFile.getGenericTag(EXIFTAG_DATETIMEORIGINAL, EXIF_APP1_EXIFIFD, status);
        if (status != EXIF_OK || NULL == genericTagEntry) // exif信息有误,重新赋值
        {
            genericTagEntry = new ExifTagEntryT<std::string>(EXIFTAG_DATETIMEORIGINAL, EXIF_ASCII, 20, chTime);
            if (inImageFile.setGenericTag(*genericTagEntry, EXIF_APP1_EXIFIFD) != EXIF_OK)
            {
                cout << *(iter);
            }
            else
            {
                cout << chTime << endl;
                file << *(iter) << ", ," << chTime << ",0" << endl;
            }
            inImageFile.close();
            nModifyCount++;     
            iter++;
            continue;
        }
        std::string OriginalTime = ((ExifTagEntryT<std::string>*)genericTagEntry)->getValue();

        // 文件名的时间和拍摄时间可能不一样，文件名的时间可能比拍摄时间短，因此只比较年月日
        // 如果时间不一样就用文件名的时间代替拍摄时间
        char metaTimeYMD[16] = "\0";
        char metaTime[20] = "\0";
        if (OriginalTime.length() > 0)
        {
            strcpy(metaTime, OriginalTime.c_str());     
            strncpy_s(metaTimeYMD, metaTime, 10);
        }

        char fileTime[16] = "\0";
        strncpy_s(fileTime, chTime, 10);
        if (strcmp(metaTimeYMD, fileTime) != 0)
        {
            genericTagEntry = new ExifTagEntryT<std::string>(EXIFTAG_DATETIMEORIGINAL, EXIF_ASCII, 20, chTime);
            inImageFile.setGenericTag(*genericTagEntry, EXIF_APP1_EXIFIFD);
            inImageFile.close();
            nModifyCount++;
            cout << metaTime << "  " << chTime << endl;
            file << *(iter) << "," << metaTime << "," << chTime << "," << endl;
        }
        inImageFile.close();
        iter++;
    }

    cout << nTotalCount << " out of " << PathVector.size() << " pictrues have been chosen." << endl;
    cout << nModifyCount << " out of " << nTotalCount << " chosen pictures have been modified." << endl;
    file.close();

}

// 获取需要修改的照片路径列表
vector<string> ModifyPicsTime(void)
{
    char PicDir[] = "K:\\00-Mobile Phone Pics Test\\";
    vector<string> PathVector;
    PathVector.clear();

    if (_access(PicDir, 06) == -1)
    {
        cout << "Directory does not exit" << endl;
        return PathVector;
    }

    if (_chdir(PicDir) != 0)
    {
        cout << "Can't change the work dirctroy." << endl;
        return PathVector;
    }

    _finddata_t fileinfo;
    memset(&fileinfo, 0x0, sizeof(fileinfo));
    intptr_t iFind = _findfirst("*.jpg", &fileinfo);
    if (iFind == -1)
    {
        cout << "Can't find any jpg file." << endl;
        return PathVector;
    }

    char jpgFile[512] = "\0";
    strcat_s(jpgFile, PicDir);
    strcat_s(jpgFile, fileinfo.name);
    string str;
    str = jpgFile;
    PathVector.push_back(str);

    while (_findnext(iFind, &fileinfo) == 0)
    {
        memset(jpgFile, 0, 512);
        strcat_s(jpgFile, PicDir);
        strcat_s(jpgFile, fileinfo.name);
        str = jpgFile;
        PathVector.push_back(str);
    }

    return PathVector;
}

// 获取GPS信息并将其写入文本文件中
void GetGPSInfo(void)
{
    vector<string> PathVector = ModifyPicsTime();

    if (PathVector.size() <= 0)
    {
        cout << "No jpg file." << endl;
        return;
    }

    int                         nCount = 0;
    ofstream                    file;
    ExifImageFile               inImageFile;
    ExifTagEntry*               genericTagEntry = NULL;
    ExifStatus                  status;
    vector<string>::iterator    iter = PathVector.begin();

    file.open("K:\\00-Mobile Phone Pics Test\\GPSInfo.txt", ios::out | ios::_Noreplace | ios::trunc);

    while (iter != PathVector.end())
    {
        string str = *iter;
        const char* chPath = str.c_str();
        ExifStatus openStatus = inImageFile.open(chPath, "r+");
        if (openStatus != EXIF_OK) // 不能用w打开照片，打开后会重新创建一张照片并将原来的照片覆盖掉
        {
            iter++;
            inImageFile.close();
            continue;
        }

        genericTagEntry = inImageFile.getGenericTag(EXIFTAG_GPSLATITUDE, EXIF_APP1_IFD0_GPSINFO_IFD, status);
        if (status != EXIF_OK || NULL == genericTagEntry) 
        {
            iter++;
            inImageFile.close();
            continue;
        }
        std::vector<float> fLatitudeArray = ((ExifTagEntryT<std::vector<float>>*)genericTagEntry)->getValue();
        float fLatitude = fLatitudeArray[0] + fLatitudeArray[1] / 60.0 + fLatitudeArray[2] / 3600.0;

        genericTagEntry = inImageFile.getGenericTag(EXIFTAG_GPSLONGITUDE, EXIF_APP1_IFD0_GPSINFO_IFD, status);
        if (status != EXIF_OK || NULL == genericTagEntry) 
        {
            iter++;
            inImageFile.close();
            continue;
        }
        std::vector<float> fLongitudeArray = ((ExifTagEntryT<std::vector<float>>*)genericTagEntry)->getValue();
        float fLongitude = fLongitudeArray[0] + fLongitudeArray[1] / 60.0 + fLongitudeArray[2] / 3600.0;

        // 取時間
        std::string OriginalTime = " ";
        genericTagEntry = inImageFile.getGenericTag(EXIFTAG_DATETIMEORIGINAL, EXIF_APP1_EXIFIFD, status);
        if (status == EXIF_OK && NULL != genericTagEntry)
        {
            OriginalTime = ((ExifTagEntryT<std::string>*)genericTagEntry)->getValue();
        }

        nCount++;
        cout << fLatitude << "," << fLongitude << "," << OriginalTime << "," << nCount << endl;
        file << fLatitude << "," << fLongitude << "," << OriginalTime << endl;
        iter++;
        inImageFile.close();
    }

    file.close();
    cout << "total pics: " << nCount << endl;
}


// Main
int main(int argc, char* argv[])
{
    // 获取GPS信息
    GetGPSInfo();

    // 取得要修改的照片路径列表
    vector<string> PathVector = ModifyPicsTime();

    // 修改拍摄时间
    ModifyMetaTime(PathVector);

    return 0;
}
```

最后终于将绝大部分照片的时间都改了回来，只有几张因为当时拍照时刚好将SIM卡拔出，导致系统时间为1970年，使得拍摄的照片文件名和元数据显示的都是1970年，未能更改。最后的输出结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20150501132033891)




