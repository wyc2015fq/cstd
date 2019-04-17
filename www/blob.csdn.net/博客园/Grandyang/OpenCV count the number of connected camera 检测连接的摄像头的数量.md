# OpenCV count the number of connected camera 检测连接的摄像头的数量 - Grandyang - 博客园







# [OpenCV count the number of connected camera 检测连接的摄像头的数量](https://www.cnblogs.com/grandyang/p/4331421.html)







有时候在项目中我们需要检测当前连接在机子上的摄像头的数量，可以通过下面的代码实现，其中连接摄像头的最大数量maxCamNum可以任意修改：



```
/**
 * Count current camera number
 */
int countCamera() {
    int maxCamNum = 5;
    int count = 0;
    for(int device = 0; device < maxCamNum; device++) 
    {
        CvCapture* capture;
        if (_capture[device]) {
            ++count;
        }
        else {
            capture = cvCaptureFromCAM(CV_CAP_DSHOW + device);
            if (capture) {
                ++count;
            }
            cvReleaseCapture(&capture);
        }
    }
    return count;
}
```



OpenCV中没有能返回摄像头设备名称的函数，有些时候也不太方便，但是没有办法，将就的用着吧~~














