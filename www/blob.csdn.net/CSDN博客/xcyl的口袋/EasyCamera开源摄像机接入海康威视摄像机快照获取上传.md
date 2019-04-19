# EasyCamera开源摄像机接入海康威视摄像机快照获取上传 - xcyl的口袋 - CSDN博客
2016年10月08日 11:19:30[xcyl](https://me.csdn.net/cai6811376)阅读数：1509
EasyCamera开源摄像机拥有获取摄像机实时快照并上传至EasyDarwin云平台的功能，那么我们如何使用海康的摄像机SDK获取实时快照呢？
我们看海康SDK的说明文档中有
![这里写图片描述](https://img-blog.csdn.net/20161008111244134)
我们使用这个接口进行快照获取
```cpp
//EasyCamera-HK EasyCameraSource.cpp
bool EasyCameraSource::getSnapData(unsigned char* pBuf, UInt32 uBufLen, int* uSnapLen)
{
    //如果摄像机未登录，返回false
    if (!cameraLogin()) return false;
    //调用SDK获取数据
    LPNET_DVR_JPEGPARA jpegPara = new NET_DVR_JPEGPARA;
    jpegPara->wPicQuality = 0;
    jpegPara->wPicSize = 9;
    LPDWORD Ret = 0;
    NET_DVR_SetCapturePictureMode(JPEG_MODE);
    /*{
        cout << "Set Capture Picture Mode error!" << endl;
        cout << "The error code is " << NET_DVR_GetLastError() << endl;
    }*/
    bool capture = NET_DVR_CaptureJPEGPicture_NEW(m_u32Handle, 1, jpegPara, (char*)pBuf, uBufLen, (LPDWORD)uSnapLen);
    if (!capture)
    {
        printf("Error: NET_DVR_CaptureJPEGPicture_NEW = %d", NET_DVR_GetLastError());
        delete jpegPara;
        return false;
    }
    delete jpegPara;
    return true;
}
```
将获取到的快照数据进行Base64编码，将编码后的数据上传至云平台。
## 源码及视频教程
源码地址：[https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK](https://github.com/EasyDarwin/EasyDarwin/tree/easycamera/EasyCamera-HK)
视频教程地址：[http://edu.csdn.net/course/detail/3029](http://edu.csdn.net/course/detail/3029)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
