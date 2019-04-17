# 使用opencv把图片拼接成视频 - zqjackking的博客 - CSDN博客





2017年11月18日 16:16:42[zqjackking](https://me.csdn.net/zqjackking)阅读数：556标签：[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)
个人分类：[C++](https://blog.csdn.net/zqjackking/article/category/7281388)









int joint() 

{ 

    //声明IplImage指针 

    IplImage* Image = NULL; 

    CvVideoWriter* video = cvCreateVideoWriter(“F:\jdd\Traindepth2.avi”, 0, 10, cvSize(960, 540), 1); 

    char filename[200];//保存图像名的数组 

    int m; 

    for (int m = 0; m < 732; m++) //图像命名规则为00*.png 

    { 

        { 

            sprintf(filename, “F:\jdd\Train\cxs\%.6d.jpg”, m); 

            Image = cvLoadImage(filename); 

        //  cvShowImage(“读取”, Image); 

            cvWriteFrame(video, Image); 

            char c = cvWaitKey(150); 

            if (c == 27) break;//按下ESC停止 

        } 

    } 

    cvReleaseVideoWriter(&video); 

    return 0; 

} 

有一点需要注意，cvCreateVideoWriter这个函数的size必须和图片的大小一致。如果不一致则要先resize再拼接



