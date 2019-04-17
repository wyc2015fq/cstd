# 使用opencv从mp4视频中抽帧并保存 - zqjackking的博客 - CSDN博客





2017年11月10日 15:58:17[zqjackking](https://me.csdn.net/zqjackking)阅读数：2632








使用oepncv有两种方法，一种是使用cvGrabFrame()+cvRetrieveFrame()，另一种是使用使用cvQueryFrame()。 
[http://blog.csdn.net/augusdi/article/details/11042329](http://blog.csdn.net/augusdi/article/details/11042329)

```cpp
int main()  
{  
    CvCapture *capture = NULL;  
    IplImage *frame = NULL;  
    char *AviFileName = "D:\\clock.avi";//视频的目录  
    char *AviSavePath = "D:\\截图\\";//图片保存的位置  
    const int jiange = 2;//间隔两帧保存一次图片  
    capture = cvCaptureFromAVI(AviFileName);  
    cvNamedWindow("AVI player",1);  
    int count_tmp = 0;//计数总帧数  
    char tmpfile[100]={'\0'};  
    while(true)  
    {  
        if(cvGrabFrame(capture))  
        {  
            if (count_tmp % jiange == 0)  
            {  
                frame=cvRetrieveFrame(capture);  
                cvShowImage("AVI player",frame);//显示当前帧  
                sprintf(tmpfile,"%s//%d.jpg",AviSavePath,count_tmp);//使用帧号作为图片名  
                cvSaveImage(tmpfile,frame);  
            }                 
            if(cvWaitKey(10)>=0) //延时  
                break;  
            ++count_tmp;  
        }  
        else  
        {  
            break;  
        }  
    }  
    cvReleaseCapture(&capture);  
    cvDestroyWindow("AVI player");   
    std::cout<<"总帧数" << count_tmp << std::endl;  
    return 0;  
}
```

有几点需要说明： 

1 这里虽然调用的函数是cvCaptureFromAVI，但是也可以用来提取mp4视频，本人提取的就是mp4格式的，所有的帧都能不失真地提取出来（每一帧的分辨率和视频中一样） 

2  sprintf(tmpfile,”%s//%d.jpg”,AviSavePath,count_tmp)这个函数，这个函数中，tmpfile是一个char数组，如果是一个char*的话，那么会报错，因为char*是不可更改的。 

区别：[http://www.jb51.net/article/100181.htm](http://www.jb51.net/article/100181.htm)

3 C++中，可以用_mkdir函数创建一个文件夹。（头文件：#include 



