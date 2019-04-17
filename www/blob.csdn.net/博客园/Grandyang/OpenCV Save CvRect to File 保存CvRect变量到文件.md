# OpenCV Save CvRect to File 保存CvRect变量到文件 - Grandyang - 博客园







# [OpenCV Save CvRect to File 保存CvRect变量到文件](https://www.cnblogs.com/grandyang/p/4679695.html)







在OpenCv中，我们有时候需要查看CvRect变量的值，我们可以通过将其保存到文件来查看，保存的代码如下：



```
void writeCvRectToFile(CvRect &rect, const char *filename) {    
    CvFileStorage *fs = cvOpenFileStorage(filename, 0, CV_STORAGE_WRITE);
    if (!fs) {
        fprintf(stderr, "Could not open file ..\n");
        return;
    }
    cvStartWriteStruct(fs, "rect", CV_NODE_SEQ | CV_NODE_FLOW);
    cvWriteInt(fs, 0, rect.x); 
    cvWriteInt(fs, 0, rect.y); 
    cvWriteInt(fs, 0, rect.width); 
    cvWriteInt(fs, 0, rect.height);
    cvEndWriteStruct(fs);
    cvReleaseFileStorage(&fs);
}
```














