# Opencv学习笔记（十一）目标跟踪

2012年03月31日 11:12:49

crzy_sparrow

阅读数：70987

更多

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

​    原创文章，转载请注明：http://blog.csdn.net/crzy_sparrow/article/details/7414851



​     如果摄像机是固定的，那么我们可以认为场景（背景）大多数情况下是不变的，而只有前景（被跟踪的目标）会运动，这样就可以建立背景模型。通过比较当前帧和背景模型，就能轻松地跟踪目标运动情况了。这里，最容易想到的比较方式就是当前帧减去背景模型了，如opencv2.3.1里的以下函数可计算当前帧与背景之差的绝对值。

cv::absdiff(backgroundImage,currentImage,foreground);

​      背景模型的建立最容易想到的就是选取一张没有目标时的图片作为背景模型，但是很多情况下，背景中往往一直会有目标在运动，比如交通监控的时候可能一直车轮滚滚，而且程序在没有经验的情况下又不会知道哪幅图片是有目标的，哪幅图片是没有目标的。好吧，我承认我说慌了，建模是我们人去建立的，是可以根据我们的感知去选择建模图片的，程序只是根据我们人为建立的模型去检测目标而已。但我也说过了，没有目标的图片可能就是很难获得，而且你肯定也觉得把仅仅一张没有目标的图片作为背景模型实在是不靠普，再怎么说也得取多张背景图片求个均值吧。于是，不难想到，我们可以截取一段目标出现相对较少的视频段（也可叫做图片序列吧。。。），然后对帧求均值，将均值图片作为背景模型。这时候的背景模型总该靠谱一点了吧。

​     问题又来了，我相机是固定了，但是天气情况总是关于时间的函数吧，白天晚上的背景肯定不一样喽，另外，除了那些大楼，山，马路等不会移动的物体，背景中的物体位置可能也会随着时间变化，或移动或消失，比如说背景中的某块广告牌被风刮倒了，某棵树被砍了。。。这个时候我们就应该动态更新背景模型了。对前面的均值背景模型做个修改倒是也能解决这个问题：就是每隔一段时间重建一个背景模型。这样就会遇到几个问题：1）建模之前，保存的大量图片会占大量存储空间；2）建模时会消耗比较长的时间，此时无法跟踪目标；3）选取几帧图片建模是个问题。2）不难解决，先建立一个均值模型，记录均值模型和每一帧图片，然后每来一帧图片，求其前景目标，删除原来的第一帧图片，将当前帧背景图片插入到建模图片的末尾，更新背景模型即可。解决1）的方法是：先建立一个模型，不再保存建模图片，每来一帧，计算前景目标，用余下的背景图片直接更新原来的模型（model*N+I）/（N+1）（model为模型图片，N为建模图片数）。不过这样有一个弊端：就是随着时间的推移，背景发生了大的改变，也就是建模的图片中最后一帧与第一帧已经差别很大了，而最后一帧更接近与当前的背景，于是我们可以对建模的图片按次序赋以不同的权重，让后面的图片权重更大些。这就是本文要讲的目标跟踪算法。

​    如果model是当前背景模型，cur是当前帧去除背景后的图片，则新的模型为：

modelnew = (1-a)*model+a*cur            ;a为学习率，随着时间的推移，之前的建模图片权重越来越小

opencv2.3.1里

void accumulateWeighted(InputArray src, InputOutputArray dst, double alpha, InputArray mask=noArray() )
 函数实现该功能：

其参数介绍如下：

Parameters
 src – Input image as 1- or 3-channel, 8-bit or 32-bit floating point.
 dst – Accumulator image with the same number of channels as input image, 32-bit or 64-bit floating-point.
 alpha – Weight of the input image.
 mask – Optional operation mask.
 The function calculates the weighted sum of the input image src and the accumulator dst so that dst becomes a
 running average of a frame sequence:
 dst(x, y) ← (1 − alpha) · dst(x, y) + alpha · src(x, y) if mask(x, y) = 0



用该方法实现目标跟踪的opencv代码如下，我这里把第一帧图片作为初始背景模型了：

```cpp
//帧处理基类
class FrameProcessor{
    public:
        virtual void process(Mat &input,Mat &ouput)=0;
};
 
class BGFGSegmentor : public FrameProcessor{
    Mat gray;//当前帧灰度图
    Mat background;//背景图，格式为32位浮点
    Mat backImage;//CV_8U格式背景图
    Mat foreground;//前景图
    double learningRate;//学习率
    int threshold;//阈值，滤去扰动
    public:
    BGFGSegmentor():threshold(30),learningRate(0.6){}
    //帧处理函数
    void process(Mat &frame,Mat &output){
        //转化为灰度图
        cvtColor (frame,gray,CV_BGR2GRAY);
        if(background.empty ())
            //第一帧
            gray.convertTo (background,CV_32F);
            //背景转为CV_8U格式以便求取和当前帧差的绝对值
            background.convertTo (backImage,CV_8U);
            //求当前帧与背景的差别
            absdiff (backImage,gray,foreground);
            //过滤掉前景中与背景差别不大的扰动点
           cv:: threshold(foreground,output,threshold,255,THRESH_BINARY_INV);
           //更新背景，output作为掩码
            accumulateWeighted (gray,background,learningRate,output);
        }
};
```

结果如下：

![img](https://img-my.csdn.net/uploads/201203/31/1333163594_6269.png)![img](https://img-my.csdn.net/uploads/201203/31/1333163599_1869.png)



用到的视频处理类和main函数如下：

```cpp
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
                    cvtColor (output,output,CV_GRAY2BGR);
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
  //  FeatureTracker tracker;
    BGFGSegmentor tracker;
    //打开输入视频
    processor.setInput ("bike.avi");
    processor.displayInput ("Current Frame");
    processor.displayOutput ("Output Frame");
    //设置每一帧的延时
    processor.setDelay (1000./processor.getFrameRate ());
    //设置帧处理函数，可以任意
    processor.setFrameProcessor (&tracker);
    //   processor.setOutput ("./bikeout.avi");
    //    processor.setOutput ("bikeout",".jpg");
    processor.run ();
    return 0;
}
```