# Opencv学习笔记（八）视频流2

2012年03月28日 21:53:06

crzy_sparrow

阅读数：6431

 								标签： 																[codec																](https://so.csdn.net/so/search/s.do?q=codec&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[extension																](https://so.csdn.net/so/search/s.do?q=extension&t=blog)[delay																](https://so.csdn.net/so/search/s.do?q=delay&t=blog)[图像处理																](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog) 							更多

 								个人分类： 																[OpenCV																](https://blog.csdn.net/crzy_sparrow/article/category/1091158) 							

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

​    用类封装了下视频的读取（可以是视频文件也可以是图片序列），处理，以及写入文件（可以是视频文件也可以是图片序列）。

​    代码如下：

```cpp
#include "opencv2/opencv.hpp"
#include <sstream>
#include <iomanip>
using namespace std;
using namespace cv;
class FrameProcessor;
class FrameProcessor{
    public:
        virtual void process(Mat &input,Mat &ouput);
};
class VideoProcessor{
private:
    VideoCapture caputure;
    //写视频流对象
    VideoWriter writer;
    //输出文件名
    string Outputfile;
 
    int currentIndex;
    int digits;
    string extension;
    FrameProcessor *frameprocessor;
    //图像处理函数指针
    void (*process)(Mat &,Mat &);
    bool callIt;
    string WindowNameInput;
    string WindowNameOutput;
    //延时
    int delay;
    long fnumber;
    //第frameToStop停止
    long frameToStop;
    //暂停标志
    bool stop;
    //图像序列作为输入视频流
    vector<string> images;
    //迭代器
public:
    VideoProcessor() : callIt(true),delay(0),fnumber(0),stop(false),digits(0),frameToStop(-1){}
   //设置图像处理函数
    void setFrameProcessor(void (*process)(Mat &,Mat &)){
        frameprocessor = 0;
        this->process = process;
        CallProcess ();
    }
    //打开视频
    bool setInput(string filename){
        fnumber = 0;
        //若已打开，释放重新打开
        caputure.release ();
        return caputure.open (filename);
    }
    //设置输入视频播放窗口
    void displayInput(string wn){
        WindowNameInput = wn;
        namedWindow (WindowNameInput);
    }
    //设置输出视频播放窗口
    void displayOutput(string wn){
        WindowNameOutput = wn;
        namedWindow (WindowNameOutput);
    }
    //销毁窗口
    void dontDisplay(){
        destroyWindow (WindowNameInput);
        destroyWindow (WindowNameOutput);
        WindowNameInput.clear ();
        WindowNameOutput.clear ();
    }
 
    //启动
    void run(){
        Mat frame;
        Mat output;
        if(!isOpened())
            return;
        stop = false;
        while(!isStopped()){
            //读取下一帧
            if(!readNextFrame(frame))
                break;
            if(WindowNameInput.length ()!=0)
                imshow (WindowNameInput,frame);
            //处理该帧
            if(callIt){
                if(process)
                    process(frame,output);
                else if(frameprocessor)
                    frameprocessor->process (frame,output);
            }
            else{
                output = frame;
            }
            if(Outputfile.length ()){
                    cvtColor (output,output,CV_GRAY2RGB);
                    writeNextFrame (output);
              }
            if(WindowNameOutput.length ()!=0)
                imshow (WindowNameOutput,output);
            //按键暂停，继续按键继续
            if(delay>=0&&waitKey (delay)>=0)
                waitKey(0);
            //到达指定暂停键，退出
            if(frameToStop>=0&&getFrameNumber()==frameToStop)
                stopIt();
        }
    }
    //暂停键置位
    void stopIt(){
        stop = true;
    }
    //查询暂停标志位
    bool isStopped(){
        return stop;
    }
    //返回视频打开标志
    bool isOpened(){
       return  caputure.isOpened ()||!images.empty ();
    }
    //设置延时
    void setDelay(int d){
        delay = d;
    }
    //读取下一帧
    bool readNextFrame(Mat &frame){
        if(images.size ()==0)
            return caputure.read (frame);
        else{
            if(itImg!=images.end()){
                frame = imread (*itImg);
                itImg++;
                return frame.data?1:0;
            }
            else
                return false;
        }
    }
 
    void CallProcess(){
        callIt = true;
    }
    void  dontCallProcess(){
        callIt = false;
    }
    //设置停止帧
    void stopAtFrameNo(long frame){
        frameToStop = frame;
    }
   // 获得当前帧的位置
    long getFrameNumber(){
        long fnumber = static_cast<long>(caputure.get ((CV_CAP_PROP_POS_FRAMES)));
        return fnumber;
    }
 
     //获得帧大小
       Size getFrameSize() {
        if (images.size()==0) {
            // 从视频流获得帧大小
            int w= static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_WIDTH));
            int h= static_cast<int>(caputure.get(CV_CAP_PROP_FRAME_HEIGHT));
            return Size(w,h);
            }
        else {
                //从图像获得帧大小
                cv::Mat tmp= cv::imread(images[0]);
                return (tmp.data)?(tmp.size()):(Size(0,0));
            }
          }
 
   //获取帧率
    double getFrameRate(){
        return caputure.get(CV_CAP_PROP_FPS);
    }
    vector<string>::const_iterator itImg;
    bool setInput (const vector<string> &imgs){
        fnumber = 0;
        caputure.release ();
        images = imgs;
        itImg = images.begin ();
        return true;
    }
 
    void  setFrameProcessor(FrameProcessor *frameprocessor){
        process = 0;
        this->frameprocessor = frameprocessor;
        CallProcess ();
    }
 
    //获得编码类型
    int getCodec(char codec[4]) {
        if (images.size()!=0)
            return -1;
        union { // 数据结构4-char
            int value;
            char code[4];
        } returned;
        //获得编码值
        returned.value= static_cast<int>(
        caputure.get(CV_CAP_PROP_FOURCC));
        // get the 4 characters
        codec[0]= returned.code[0];
        codec[1]= returned.code[1];
        codec[2]= returned.code[2];
        codec[3]= returned.code[3];
        return returned.value;
    }
 
 
    bool setOutput(const string &filename,int codec = 0,double framerate = 0.0,bool isColor = true){
        //设置文件名
        Outputfile = filename;
        //清空扩展名
        extension.clear ();
        //设置帧率
        if(framerate ==0.0){
            framerate = getFrameRate ();
        }
        //获取输入原视频的编码方式
        char c[4];
        if(codec==0){
            codec = getCodec(c);
        }
        return writer.open(Outputfile,
                           codec,
                           framerate,
                           getFrameSize(),
                           isColor);
    }
 
    //输出视频帧到图片fileme+currentIndex.ext,如filename001.jpg
    bool setOutput (const string &filename,//路径
                    const string &ext,//扩展名
                    int numberOfDigits=3,//数字位数
                    int startIndex=0 ){//起始索引
           if(numberOfDigits<0)
               return false;
           Outputfile = filename;
           extension = ext;
           digits = numberOfDigits;
           currentIndex = startIndex;
           return true;
    }
 
    //写下一帧
    void writeNextFrame(Mat &frame){
        //如果扩展名不为空，写到图片文件中
        if(extension.length ()){
            stringstream ss;
            ss<<Outputfile<<setfill('0')<<setw(digits)<<currentIndex++<<extension;
            imwrite (ss.str (),frame);
        }
        //反之，写到视频文件中
        else{
            writer.write (frame);
        }
    }
 
};
 
//帧处理函数：canny边缘检测
void canny(cv::Mat& img, cv::Mat& out) {
    //灰度变换
    if (img.channels()==3)
        cvtColor(img,out,CV_BGR2GRAY);
    // canny算子求边缘
    Canny(out,out,100,200);
    //颜色反转，看起来更舒服些
    threshold(out,out,128,255,cv::THRESH_BINARY_INV);
}
 
 
int main(int argc, char *argv[])
{
    VideoProcessor processor;
    //打开输入视频
    processor.setInput ("bike.avi");
    processor.displayInput ("Current Frame");
    processor.displayOutput ("Output Frame");
    //设置每一帧的延时
    processor.setDelay (1000./processor.getFrameRate ());
    //设置帧处理函数，可以任意
    processor.setFrameProcessor (canny );
    processor.setOutput ("./bikeout.avi");
//    processor.setOutput ("bikeout",".jpg");
    processor.run ();
    return 0;
}
```