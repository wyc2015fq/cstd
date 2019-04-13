
# TLD（Tracking-Learning-Detection）学习与源码理解之（四） - zouxy09的专栏 - CSDN博客


2012年08月21日 20:25:52[zouxy09](https://me.csdn.net/zouxy09)阅读数：32599


**TLD（Tracking-Learning-Detection）学习与源码理解之（四）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
下面是自己在看论文和这些大牛的分析过程中，对代码进行了一些理解，但是由于自己接触图像处理和机器视觉没多久，另外由于自己编程能力比较弱，所以分析过程可能会有不少的错误，希望各位不吝指正。而且，因为编程很多地方不懂，所以注释得非常乱，还海涵。
run_tld.cpp
```cpp
#include <opencv2/opencv.hpp>
#include <tld_utils.h>
#include <iostream>
#include <sstream>  //c++中的sstream类，提供了程序和string对象之间的I/O，可以通过ostringstream
					//和instringstream两个类来声明对象，分别对应输出流和输入流
#include <TLD.h>
#include <stdio.h>
using namespace cv;
using namespace std;
//Global variables
Rect box;
bool drawing_box = false;
bool gotBB = false;
bool tl = true;
bool rep = false;
bool fromfile=false;
string video;
//读取记录bounding box的文件，获得bounding box的四个参数：左上角坐标x，y和宽高
/*如在\datasets\06_car\init.txt中：记录了初始目标的bounding box，内容如下
142,125,232,164   
*/
void readBB(char* file){
  ifstream bb_file (file);  //以输入方式打开文件
  string line;
  //istream& getline ( istream& , string& );
  //将输入流is中读到的字符存入str中，终结符默认为 '\n'（换行符） 
  getline(bb_file, line);
  istringstream linestream(line); //istringstream对象可以绑定一行字符串，然后以空格为分隔符把该行分隔开来。
  string x1,y1,x2,y2;
  
  //istream& getline ( istream &is , string &str , char delim ); 
  //将输入流is中读到的字符存入str中，直到遇到终结符delim才结束。
  getline (linestream,x1, ',');
  getline (linestream,y1, ',');
  getline (linestream,x2, ',');
  getline (linestream,y2, ',');
  
  //atoi 功 能： 把字符串转换成整型数
  int x = atoi(x1.c_str());// = (int)file["bb_x"];
  int y = atoi(y1.c_str());// = (int)file["bb_y"];
  int w = atoi(x2.c_str())-x;// = (int)file["bb_w"];
  int h = atoi(y2.c_str())-y;// = (int)file["bb_h"];
  box = Rect(x,y,w,h);
}
//bounding box mouse callback
//鼠标的响应就是得到目标区域的范围，用鼠标选中bounding box。
void mouseHandler(int event, int x, int y, int flags, void *param){
  switch( event ){
  case CV_EVENT_MOUSEMOVE:
    if (drawing_box){
        box.width = x-box.x;
        box.height = y-box.y;
    }
    break;
  case CV_EVENT_LBUTTONDOWN:
    drawing_box = true;
    box = Rect( x, y, 0, 0 );
    break;
  case CV_EVENT_LBUTTONUP:
    drawing_box = false;
    if( box.width < 0 ){
        box.x += box.width;
        box.width *= -1;
    }
    if( box.height < 0 ){
        box.y += box.height;
        box.height *= -1;
    }
    gotBB = true;   //已经获得bounding box
    break;
  }
}
void print_help(char** argv){
  printf("use:\n     %s -p /path/parameters.yml\n",argv[0]);
  printf("-s    source video\n-b        bounding box file\n-tl  track and learn\n-r     repeat\n");
}
//分析运行程序时的命令行参数
void read_options(int argc, char** argv, VideoCapture& capture, FileStorage &fs){
  for (int i=0;i<argc;i++){
      if (strcmp(argv[i],"-b")==0){
          if (argc>i){
              readBB(argv[i+1]);  //是否指定初始的bounding box
              gotBB = true;
          }
          else
            print_help(argv);
      }
      if (strcmp(argv[i],"-s")==0){   //从视频文件中读取
          if (argc>i){
              video = string(argv[i+1]);
              capture.open(video);
              fromfile = true;
          }
          else
            print_help(argv);
      }
	  //Similar in format to XML, Yahoo! Markup Language (YML) provides functionality to Open 
	  //Applications in a safe and standardized fashion. You include YML tags in the HTML code
	  //of an Open Application.
      if (strcmp(argv[i],"-p")==0){   //读取参数文件parameters.yml
          if (argc>i){
		  //FileStorage类的读取方式可以是：FileStorage fs(".\\parameters.yml", FileStorage::READ);  
              fs.open(argv[i+1], FileStorage::READ);
          }
          else
            print_help(argv);
      }
      if (strcmp(argv[i],"-no_tl")==0){  //To train only in the first frame (no tracking, no learning)
          tl = false;
      }
      if (strcmp(argv[i],"-r")==0){  //Repeat the video, first time learns, second time detects
          rep = true;
      }
  }
}
/*
运行程序时：
%To run from camera
./run_tld -p ../parameters.yml
%To run from file
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg
%To init bounding box from file
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt
%To train only in the first frame (no tracking, no learning)
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt -no_tl 
%To test the final detector (Repeat the video, first time learns, second time detects)
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt -r
*/
//感觉就是对起始帧进行初始化工作，然后逐帧读入图片序列，进行算法处理。
int main(int argc, char * argv[]){
  VideoCapture capture;
  capture.open(0);
  
  //OpenCV的C++接口中，用于保存图像的imwrite只能保存整数数据，且需作为图像格式。当需要保存浮
  //点数据或XML/YML文件时，OpenCV的C语言接口提供了cvSave函数，但这一函数在C++接口中已经被删除。
  //取而代之的是FileStorage类。
  FileStorage fs;
  //Read options
  read_options(argc, argv, capture, fs);  //分析命令行参数
  //Init camera
  if (!capture.isOpened())
  {
	cout << "capture device failed to open!" << endl;
    return 1;
  }
  //Register mouse callback to draw the bounding box
  cvNamedWindow("TLD",CV_WINDOW_AUTOSIZE);
  cvSetMouseCallback( "TLD", mouseHandler, NULL );  //用鼠标选中初始目标的bounding box
  //TLD framework
  TLD tld;
  //Read parameters file
  tld.read(fs.getFirstTopLevelNode());
  Mat frame;
  Mat last_gray;
  Mat first;
  if (fromfile){  //如果指定为从文件读取
      capture >> frame;   //读当前帧
      cvtColor(frame, last_gray, CV_RGB2GRAY);  //转换为灰度图像
      frame.copyTo(first);  //拷贝作为第一帧
  }else{   //如果为读取摄像头，则设置获取的图像大小为320x240 
      capture.set(CV_CAP_PROP_FRAME_WIDTH,340);  //340？？
      capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
  }
  ///Initialization
GETBOUNDINGBOX:   //标号：获取bounding box
  while(!gotBB)
  {
    if (!fromfile){
      capture >> frame;
    }
    else
      first.copyTo(frame);
    cvtColor(frame, last_gray, CV_RGB2GRAY);
    drawBox(frame,box);  //把bounding box 画出来
    imshow("TLD", frame);
    if (cvWaitKey(33) == 'q')
	    return 0;
  }
  //由于图像片（min_win 为15x15像素）是在bounding box中采样得到的，所以box必须比min_win要大
  if (min(box.width, box.height)<(int)fs.getFirstTopLevelNode()["min_win"]){
      cout << "Bounding box too small, try again." << endl;
      gotBB = false;
      goto GETBOUNDINGBOX;
  }
  //Remove callback
  cvSetMouseCallback( "TLD", NULL, NULL );  //如果已经获得第一帧用户框定的box了，就取消鼠标响应
  printf("Initial Bounding Box = x:%d y:%d h:%d w:%d\n",box.x,box.y,box.width,box.height);
  //Output file
  FILE  *bb_file = fopen("bounding_boxes.txt","w");
  
  //TLD initialization
  tld.init(last_gray, box, bb_file);
  ///Run-time
  Mat current_gray;
  BoundingBox pbox;
  vector<Point2f> pts1;
  vector<Point2f> pts2;
  bool status=true;  //记录跟踪成功与否的状态 lastbox been found
  int frames = 1;  //记录已过去帧数
  int detections = 1;  //记录成功检测到的目标box数目
  
REPEAT:
  while(capture.read(frame)){
    //get frame
    cvtColor(frame, current_gray, CV_RGB2GRAY);
    //Process Frame
    tld.processFrame(last_gray, current_gray, pts1, pts2, pbox, status, tl, bb_file);
    //Draw Points
    if (status){  //如果跟踪成功
      drawPoints(frame,pts1);
      drawPoints(frame,pts2,Scalar(0,255,0));  //当前的特征点用蓝色点表示
      drawBox(frame,pbox);
      detections++;
    }
    //Display
    imshow("TLD", frame);
    //swap points and images
    swap(last_gray, current_gray);  //STL函数swap()用来交换两对象的值。其泛型化版本定义于<algorithm>;
    pts1.clear();
    pts2.clear();
    frames++;
    printf("Detection rate: %d/%d\n", detections, frames);
    if (cvWaitKey(33) == 'q')
      break;
  }
  if (rep){
    rep = false;
    tl = false;
    fclose(bb_file);
    bb_file = fopen("final_detector.txt","w");
    //capture.set(CV_CAP_PROP_POS_AVI_RATIO,0);
    capture.release();
    capture.open(video);
    goto REPEAT;
  }
  fclose(bb_file);
  return 0;
}
```
tld_utils.cpp
```cpp
#include <tld_utils.h>
using namespace cv;
using namespace std;
/*vector是C++标准模板库STL中的部分内容，它是一个多功能的，能够操作多种数据结构和算法的
模板类和函数库。vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，
简单地说，vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。
为了可以使用vector，必须在你的头文件中包含下面的代码：
#include <vector>
vector属于std命名域的，因此需要通过命名限定，如下完成你的代码：
using std::vector;
*/
void drawBox(Mat& image, CvRect box, Scalar color, int thick){
  rectangle( image, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),color, thick);
} 
//函数 cvRound, cvFloor, cvCeil 用一种舍入方法将输入浮点数转换成整数。
//cvRound 返回和参数最接近的整数值。 cvFloor 返回不大于参数的最大整数值。
//cvCeil 返回不小于参数的最小整数值。
void drawPoints(Mat& image, vector<Point2f> points,Scalar color){
  for( vector<Point2f>::const_iterator i = points.begin(), ie = points.end(); i != ie; ++i )
      {
      Point center( cvRound(i->x ), cvRound(i->y));  //类似于int i(3)的初始化，但center为何没用到？
      circle(image,*i,2,color,1);
      }
}
Mat createMask(const Mat& image, CvRect box){
  Mat mask = Mat::zeros(image.rows,image.cols,CV_8U);
  drawBox(mask,box,Scalar::all(255),CV_FILLED);
  return mask;
}
//STL中的nth_element()方法找出一个数列中排名第n的那个数。
//对于序列a[0:len-1]将第n大的数字，排在a[n],同时a[0:n-1]都小于a[n],a[n+1:]都大于a[n],
//但a[n]左右的这两个序列不一定有序。
//用在中值流跟踪算法中，寻找中值
float median(vector<float> v)
{
    int n = floor(v.size() / 2);
    nth_element(v.begin(), v.begin()+n, v.end());
    return v[n];
}
//<algorithm> //random_shuffle的头文件
//shuffle 洗牌  首先简单的介绍一个扑克牌洗牌的方法，假设一个数组 poker[52] 中存有一副扑克
//牌1-52的牌点值，使用一个for循环遍历这个数组，每次循环都生成一个[0，52)之间的随机数RandNum，
//以RandNum为数组下标，把当前下标对应的值和RandNum对应位置的值交换，循环结束，每个牌都与某个
//位置交换了一次，这样一副牌就被打乱了。 理解代码如下：
/*
for (int i = 0; i < 52; ++i)  
{  
    int RandNum = rand() % 52;    
    int tmp = poker[i];  
    poker[i] = poker[RandNum];  
    poker[RandNum] = tmp;  
} 
*/
//需要指定范围内的随机数，传统的方法是使用ANSI C的函数random(),然后格式化结果以便结果是落在
//指定的范围内。但是，使用这个方法至少有两个缺点。做格式化时，结果常常是扭曲的,且只支持整型数。
//C++中提供了更好的解决方法，那就是STL中的random_shuffle()算法。产生指定范围内的随机元素集的最佳方法
//是创建一个顺序序列（也就是向量或者内置数组），在这个顺序序列中含有指定范围的所有值。
//例如，如果你需要产生100个0-99之间的数，那么就创建一个向量并用100个按升序排列的数填充向量.
//填充完向量之后，用random_shuffle()算法打乱元素排列顺序。
//默认的random_shuffle中, 被操作序列的index 与 rand() % N 两个位置的值交换，来达到乱序的目的。
//index_shuffle()用于产生指定范围[begin:end]的随机数，返回随机数数组
vector<int> index_shuffle(int begin,int end){
  vector<int> indexes(end-begin);
  for (int i=begin;i<end;i++){
    indexes[i]=i;
  }
  random_shuffle(indexes.begin(),indexes.end());
  return indexes;
}
```



