# Opencv学习笔记（九）光流法

2012年03月29日 15:58:56

crzy_sparrow

阅读数：100355

更多

 								所属专栏： 																[Opencv学习笔记](https://blog.csdn.net/column/details/opencv.html) 																 							

 									

​    原创文章，转贴请注明：http://blog.csdn.net/crzy_sparrow/article/details/7407604

​    

本文目录：

​      一.基于特征点的目标跟踪的一般方法

​      二.光流法

​      三.opencv中的光流法函数

​      四.用类封装基于光流法的目标跟踪方法

​      五.完整代码

​      六.参考文献



**一.基于特征点的目标跟踪的一般方法**

​      基于特征点的跟踪算法大致可以分为两个步骤：

​      1）探测当前帧的特征点；

​      2）通过当前帧和下一帧灰度比较，估计当前帧特征点在下一帧的位置；

​      3）过滤位置不变的特征点，余下的点就是目标了。

​       很显然，基于特征点的目标跟踪算法和1），2）两个步骤有关。特征点可以是Harris角点（见我的另外一篇博文），也可以是边缘点等等，而估计下一帧位置的方法也有不少，比如这里要讲的光流法，也可以是卡尔曼滤波法（咱是控制系的，上课经常遇到这个，所以看光流法看着看着就想到这个了）。

​      本文中，用改进的Harris角点提取特征点（见我另一篇博文：http://blog.csdn.net/crzy_sparrow/article/details/7391511），用Lucas-Kanade光流法实现目标跟踪。



二.光流法

​      这一部分《learing opencv》一书的第10章Lucas-Kanade光流部分写得非常详细，推荐大家看书。我这里也粘帖一些选自书中的内容。

​      另外我对这一部分附上一些个人的看法（谬误之处还望不吝指正）：

​      1.首先是假设条件：

​       （1）亮度恒定，就是同一点随着时间的变化，其亮度不会发生改变。这是基本光流法的假定（所有光流法变种都必须满足），用于得到光流法基本方程；

​       （2）小运动，这个也必须满足，就是时间的变化不会引起位置的剧烈变化，这样灰度才能对位置求偏导（换句话说，小运动情况下我们才能用前后帧之间单位位置变化引起的灰度变化去近似灰度对位置的偏导数），这也是光流法不可或缺的假定；

​       （3）空间一致，一个场景上邻近的点投影到图像上也是邻近点，且邻近点速度一致。这是Lucas-Kanade光流法特有的假定，因为光流法基本方程约束只有一个，而要求x，y方向的速度，有两个未知变量。我们假定特征点邻域内做相似运动，就可以连立n多个方程求取x，y方向的速度（n为特征点邻域总点数，包括该特征点）。

​      2.方程求解

​      多个方程求两个未知变量，又是线性方程，很容易就想到用最小二乘法，事实上opencv也是这么做的。其中，最小误差平方和为最优化指标。

​       3.好吧，前面说到了小运动这个假定，聪明的你肯定很不爽了，目标速度很快那这货不是二掉了。幸运的是多尺度能解决这个问题。首先，对每一帧建立一个高斯金字塔，最大尺度图片在最顶层，原始图片在底层。然后，从顶层开始估计下一帧所在位置，作为下一层的初始位置，沿着金字塔向下搜索，重复估计动作，直到到达金字塔的底层。聪明的你肯定发现了：这样搜索不仅可以解决大运动目标跟踪，也可以一定程度上解决孔径问题（相同大小的窗口能覆盖大尺度图片上尽量多的角点，而这些角点无法在原始图片上被覆盖）。



![img](https://img-my.csdn.net/uploads/201203/29/1332996948_1212.png)![img](https://img-my.csdn.net/uploads/201203/29/1332996959_6819.png)![img](https://img-my.csdn.net/uploads/201203/29/1332996971_5681.png)![img](https://img-my.csdn.net/uploads/201203/29/1332996978_4801.png)![img](https://img-my.csdn.net/uploads/201203/29/1332996991_9698.png)![img](https://img-my.csdn.net/uploads/201203/29/1332996998_4169.png)



三.opencv中的光流法函数

​      opencv2.3.1中已经实现了基于光流法的特征点位置估计函数（当前帧位置已知，前后帧灰度已知），介绍如下（摘自opencv2.3.1参考手册）：



```cpp
calcOpticalFlowPyrLK
Calculates an optical flow for a sparse feature set using the iterative Lucas-Kanade method with pyramids.
 
void calcOpticalFlowPyrLK(InputArray prevImg, InputArray nextImg, InputArray prevPts,
InputOutputArray nextPts, OutputArray status, OutputArray err,
Size winSize=Size(15,15), int maxLevel=3, TermCriteria crite-
ria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01),
double derivLambda=0.5, int flags=0 )
 
Parameters
prevImg – First 8-bit single-channel or 3-channel input image.
nextImg – Second input image of the same size and the same type as prevImg .
prevPts – Vector of 2D points for which the flow needs to be found. The point coordinates
must be single-precision floating-point numbers.
nextPts – Output vector of 2D points (with single-precision floating-point coordinates)
containing the calculated new positions of input features in the second image. When
OPTFLOW_USE_INITIAL_FLOW flag is passed, the vector must have the same size as in the
input.
status – Output status vector. Each element of the vector is set to 1 if the flow for the
corresponding features has been found. Otherwise, it is set to 0.
err – Output vector that contains the difference between patches around the original and
moved points.
winSize – Size of the search window at each pyramid level.
 
maxLevel – 0-based maximal pyramid level number. If set to 0, pyramids are not used
(single level). If set to 1, two levels are used, and so on.
criteria – Parameter specifying the termination criteria of the iterative search algorithm
(after the specified maximum number of iterations criteria.maxCount or when the search
window moves by less than criteria.epsilon .
derivLambda – Not used.
 
flags – Operation flags:
– OPTFLOW_USE_INITIAL_FLOW Use initial estimations stored in nextPts . If the
flag is not set, then prevPts is copied to nextPts and is considered as the initial estimate.
```



四.用类封装基于光流法的目标跟踪方法

​      废话少说，附上代码，包括特征点提取，跟踪特征点，标记特征点等。



```cpp
//帧处理基类
class FrameProcessor{
    public:
        virtual void process(Mat &input,Mat &ouput)=0;
};
 
//特征跟踪类，继承自帧处理基类
class FeatureTracker :  public FrameProcessor{
    Mat gray;  //当前灰度图
    Mat gray_prev;  //之前的灰度图
    vector<Point2f> points[2];//前后两帧的特征点
    vector<Point2f> initial;//初始特征点
    vector<Point2f> features;//检测到的特征
    int max_count; //要跟踪特征的最大数目
    double qlevel; //特征检测的指标
    double minDist;//特征点之间最小容忍距离
    vector<uchar> status; //特征点被成功跟踪的标志
    vector<float> err; //跟踪时的特征点小区域误差和
public:
    FeatureTracker():max_count(500),qlevel(0.01),minDist(10.){}
    void process(Mat &frame,Mat &output){
        //得到灰度图
        cvtColor (frame,gray,CV_BGR2GRAY);
        frame.copyTo (output);
        //特征点太少了，重新检测特征点
        if(addNewPoint()){
            detectFeaturePoint ();
            //插入检测到的特征点
            points[0].insert (points[0].end (),features.begin (),features.end ());
            initial.insert (initial.end (),features.begin (),features.end ());
        }
        //第一帧
        if(gray_prev.empty ()){
                gray.copyTo (gray_prev);
        }
        //根据前后两帧灰度图估计前一帧特征点在当前帧的位置
        //默认窗口是15*15
        calcOpticalFlowPyrLK (
                gray_prev,//前一帧灰度图
                gray,//当前帧灰度图
                points[0],//前一帧特征点位置
                points[1],//当前帧特征点位置
                status,//特征点被成功跟踪的标志
                err);//前一帧特征点点小区域和当前特征点小区域间的差，根据差的大小可删除那些运动变化剧烈的点
        int k = 0;
        //去除那些未移动的特征点
        for(int i=0;i<points[1].size ();i++){
            if(acceptTrackedPoint (i)){
                initial[k]=initial[i];
                points[1][k++] = points[1][i];
            }
        }
        points[1].resize (k);
        initial.resize (k);
        //标记被跟踪的特征点
        handleTrackedPoint (frame,output);
        //为下一帧跟踪初始化特征点集和灰度图像
        std::swap(points[1],points[0]);
        cv::swap(gray_prev,gray);
    }
 
    void detectFeaturePoint(){
        goodFeaturesToTrack (gray,//输入图片
                                 features,//输出特征点
                                 max_count,//特征点最大数目
                                 qlevel,//质量指标
                                 minDist);//最小容忍距离
    }
    bool addNewPoint(){
        //若特征点数目少于10，则决定添加特征点
        return points[0].size ()<=10;
    }
 
    //若特征点在前后两帧移动了，则认为该点是目标点，且可被跟踪
    bool acceptTrackedPoint(int i){
        return status[i]&&
                (abs(points[0][i].x-points[1][i].x)+
                  abs(points[0][i].y-points[1][i].y) >2);
    }
 
    //画特征点
    void  handleTrackedPoint(Mat &frame,Mat &output){
            for(int i=0;i<points[i].size ();i++){
                //当前特征点到初始位置用直线表示
                line(output,initial[i],points[1][i],Scalar::all (0));
                //当前位置用圈标出
                circle(output,points[1][i],3,Scalar::all(0),(-1));
            }
        }
};
```



五.完整代码
  完整的运行代码有300+行，粘上来太多了，大家去我传的资源里下载吧。

  下载地址：http://download.csdn.net/detail/crzy_sparrow/4183674

  运行结果：

![img](https://img-my.csdn.net/uploads/201203/29/1333015960_6036.png)![img](https://img-my.csdn.net/uploads/201203/29/1333015968_1379.png)



六.参考文献

【1】The classic article by B. Lucas and T.  Kanade, An iterative image registration technique with an application to  stereo vision in Int. Joint Conference in Artificial Intelligence, pp.  674-679,1981, that describes the original  feature point tracking algorithm.
 【2】The article by J. Shi and C. Tomasi, Good Features to Track in IEEE  Conference on Computer Vision and Pattern Recognition, pp. 593-600,  1994, that describes an improved version of the original feature point  tracking algorithm.

