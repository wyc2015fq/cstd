# Opencv学习笔记（七）视频流1

2012年03月28日 11:38:45

crzy_sparrow

阅读数：7193

更多

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

​     看了TLD的视频，热血沸腾啊。测试了一下C++版本的，发现速度有点慢，matlab版本的还未测试，不知道速度如何，不过看作者的视频实时性真他妈够好的。过段时间研究研究，C++的速度改快一点，不过其涉及的东西甚多，任重而道远。不过感觉速度快了的话这货前景非常广啊。

​    言归正转，opencv cookbook看到最后一章了，传个用c++类封装的opencv视频读取处理的代码。



```cpp
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
 
class VideoProcessor{
private:
    VideoCapture caputure;
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
public:
    VideoProcessor() : callIt(true),delay(0),fnumber(0),stop(false),frameToStop(-1){}
   //设置图像处理函数
    void setFrameProcess(void (*process)(Mat &,Mat &)){
        this->process = process;
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
                process(frame,output);
            }
            else{
                output = frame;
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
       return  caputure.isOpened ();
    }
    //设置延时
    void setDelay(int d){
        delay = d;
    }
    //读取下一帧
    bool readNextFrame(Mat &frame){
        return caputure.read (frame);
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
   //获取帧率
    double getFrameRate(){
        return caputure.get(CV_CAP_PROP_FPS);
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
    processor.setFrameProcess (canny );
    processor.run ();
    return 0;
}
```

 视频处理效果贴图附上一张： 

![img](https://img-my.csdn.net/uploads/201203/28/1332906079_6056.png)