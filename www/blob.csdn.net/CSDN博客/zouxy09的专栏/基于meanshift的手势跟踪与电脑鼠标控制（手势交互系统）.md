
# 基于meanshift的手势跟踪与电脑鼠标控制（手势交互系统） - zouxy09的专栏 - CSDN博客


2014年01月06日 11:12:00[zouxy09](https://me.csdn.net/zouxy09)阅读数：27197


**基于meanshift的手势跟踪与电脑鼠标控制（手势交互系统）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
一年多前开始接触计算机视觉这个领域的时候，年幼无知，倍感吃力。当年惶恐，从而盲从。挣扎了不少时日，感觉自己好像还是处于领域的门外汉一样，在理论与实践的鸿沟中无法挣脱，心里空落落的。在这种挥之不去的烦忧中，某个时候豁然开朗，觉得要看一个系统的代码了，看看别人是怎么写的，理论又是怎么用在实践上的。然后自己就瞄准了TLD这个被炒作地很火的跟踪算法。花了点时间做了详细的[源码解读和注释](http://blog.csdn.net/zouxy09/article/details/7893011)。也发到了博客上，光荣的成为了我第一篇博客的主题。当年还年少气盛，欲借TLD与开源之力，顺势而为，扬言要捣腾一个[人机交互系统](http://blog.csdn.net/zouxy09/article/details/7919618)出来，其中包括手势跟踪、TTS语音合成、语音识别、手势和语音控制鼠标和键盘等等。还naive到要移植到嵌入式设备上。不过，当年也的确开始了点工作，包括将[TLD用以手势跟踪](http://blog.csdn.net/zouxy09/article/details/7930025)、[鼠标控制](http://blog.csdn.net/zouxy09/article/details/7920253)、[Ekho（余音）TTS语音合成](http://blog.csdn.net/zouxy09/article/details/7921032)和[PocketSphinx语音识别](http://blog.csdn.net/zouxy09/article/details/7942784)等。后来因为语音识别效果不好，就卡住停滞不前了。
这篇博文记录的其实和之前的将[TLD用以手势跟踪](http://blog.csdn.net/zouxy09/article/details/7930025)有异曲同工之妙，哈哈。不同的点在于跟踪用的不是TLD了，而是简单的meanshift，当然了，我们也可以使用其他的目标跟踪算法，但这里需要实时的算法。例如我们之前分析的[压缩跟踪CT算法](http://blog.csdn.net/zouxy09/article/details/8118360)，和[时空上下文跟踪STC算法](http://blog.csdn.net/zouxy09/article/details/16889905)都是OK的。连我之前写的最简单的[模板匹配](http://blog.csdn.net/zouxy09/article/details/13358977)和[感知哈希](http://blog.csdn.net/zouxy09/article/details/17471401)的跟踪都在这个平台上可以发挥光和热。但其实，真正的手势控制或者手势交互需要的跟踪算法要比这些要求高，它需要更准确和稳定的跟踪，还得抗干扰，克服复杂背景，光照等等。还有一点就是得避免跟踪框的抖动，否则鼠标也会跟着抖动，那多不爽啊。所以俺们的这个系统也是孤独下的催生物而已，也许没有太大的利用价值。哦，还有一点不同是，之前是在Linux平台下的，这次改在windows阵地，所以鼠标控制的接口就不同了。另外，本博文的代码比之前的要规范一点，需要替换跟踪器的时候，也可以较容易的替换。
总的来说，这篇博文只是一个基于2D摄像头的粗糙的手势控制系统（不是一般的粗糙哦，哈哈），包括以下功能：
1）跟踪人手，然后映射到电脑的鼠标，也就是用人手取代鼠标去控制电脑光标（鼠标）的移动。
2）通过检测用户的握拳来模拟鼠标的左键点击功能，也就是我们用手控制鼠标移动到一个地方的时候，握拳，就相当于单击鼠标左键，也就是确定的功能了。
**一、手势跟踪**
人手的跟踪我们采用的是简单的meanshift算法。因为人手是肤色的，所以如果背景是没有类肤色的东西的话，跟踪是很有效的。当然了，如果人手移动到人脸的地方，很大可能会影响跟踪结果。这是该算法的弱点之一。
**1.1、meanshift跟踪**
meanshift均值漂移算法可谓是经典之经典。以至于在江湖上，视觉派和图像派的人如果不认得该秘笈，都不好意思出来混江湖了。对于这么经典的算法，江湖上的解说也一搜一大箩，大家也可以参考文献[2][3][4]。这里就简短介绍下算法主要思想和工作过程。
meanshift算法本质上是最优化理论中的最速下降法。即沿着梯度下降方法寻找目标函数的极值。在跟踪中，就是为了寻找到相似度值最大的候选目标位置。meanshift方法沿着概率密度的梯度方向进行迭代移动，最终达到密度分布的最值位置。其迭代过程本质上是的最速下降法，下降方向为一阶梯度方向，步长为固定值。比较经典的meanshift算法的过程图示要数下面这个了：
在d维空间中，任选一个点，然后以这个点为圆心，h为半径做一个高维球，因为有d维，d可能大于2，所以是高维球。落在这个球内的所有点和圆心都会产生一个向量，向量是以圆心为起点落在球内的点为终点。然后把这些向量都相加。相加的结果就是meanshift向量。如下图，其中黄色箭头就是meanshift向量。
![](https://img-blog.csdn.net/20140106110643375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再以meanshift向量的终点为圆心，再做一个高维的球。如下图所以，重复以上步骤，就可得到一个meanshift向量。如此重复下去，meanshift算法可以收敛到概率密度最大得地方。也就是最稠密的地方。
![](https://img-blog.csdn.net/20140106110658156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最终的结果如下：
![](https://img-blog.csdn.net/20140106110711093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
对meanshift的更多理解和推导请参考网络上的更多资料。下面我们分析下meanshift如何用在我们系统的手势跟踪过程中。我们要跟踪的是人手，而人手一般是比较统一的肤色，那么我们在图像中人手的这个区域统计每一种颜色的像素个数，这样我们会得到肤色的像素个数是最多的。然后对于新来的一帧图像，我们怎么找到手的位置呢？我们需要寻找满足人手肤色的地方。具体是通过用上面得到的直方图来计算整幅图像的反向投影。实际上就是一个肤色的概率图，图像中像素越像肤色，那该点属于肤色的概率就越大，在反向投影图中的值越大。例如：在整幅图中，假设某个像素的H分量值为10，然后我们到刚才从人手中统计得到的直方图中查找H=10对应的纵坐标值为50（代表搜索窗口中有50个H值为10的像素），则将整幅图中该像素的值置为50。对所有像素对应查找赋值便得到了反向投影图。可以理解为，它放映的是人手在图像中的位置的概率图（当然了，严格说不是这么理解的，这是像素级别和模板级别的差别）。概率图中值最大的地方也就是手的地方。然后我们通过meanshift算法来找到这个地方。
我们的手势跟踪过程大概描述如下；
（1）初始化：我们检测到图像中人手区域后，统计得到该区域HSV颜色空间H分量的直方图。得到的直方图横坐标为0-255的灰度值（H分量的值），纵坐标为人手图像区域中某个H分量的值的像素个数。
（2）反向投影：利用（1）的直方图计算整幅图像的概率分布。实际计算中不必计算整幅图像，只需计算比搜索窗口大一些的范围。
（3）利用meanshift算法迭代找到概率分布图的重心。也就是人手的位置了。
以上三个步骤，OpenCV都提供的简便的接口可以利用。具体使用方式见后面的代码。
**1.2、运动信息的融合**
单纯地用肤色来跟踪的话，如果背景有肤色的东西meanshift算法就比较容易跟丢。所以我们可以加入运动信息来辅助，排除背景中静态肤色的干扰，同时人脸一般也是不动的，也可以一定程度的排除。
那我们怎么获得运动信息呢？我之前的博文有稍微总结下现有的[运动检测算法](http://blog.csdn.net/zouxy09/article/details/9622285)。但这里我们为了秉承简单的理念，使用了帧差这个简单到令人发指的运动检测算法。它原理就是前后的两帧图像相减，然后如果是运动的区域的话，那么前后两帧的图像就会不一样，相减的话，结果就会不为0，那如果静止的区域，前后两帧的图像就是一样的，相减的话，值就是0。那么不为0的地方就是图像运动的区域了。
当然了，该算法的缺点也是不言而喻的了。
得到运动信息之后，最简单的和肤色信息的融合方法就是加权融合了。将两个概率图进行加权，得到新的概率图，然后交给meanshift来找到概率最大的地方。
**二、鼠标控制**
鼠标控制包括两部分：一是手的跟踪需要映射到鼠标的移动，二是当用户握拳的时候需要激活鼠标单击的指令。之前的那个交互系统是在Linux平台下的，这次改在windows阵地。所以鼠标控制的接口有所不同。那下面就介绍下windows下的鼠标控制接口。
**2.1、鼠标移动：手移动**
Windows提供mouse_event()这个函数给用户来模拟鼠标事件。它的使用方法如下：
VOID mouse_event(
DWORD dwFlags,              // 鼠标动作标识。
DWORD dx,                        // 鼠标水平方向位置。
DWORD dy,                       // 鼠标垂直方向位置。
DWORD dwData,              //  鼠标轮子转动的数量。
DWORD dwExtraInfo        //  一个关联鼠标动作辅加信息。
);
其中，各参数的使命如下：
dwFlags：标志位集，指定点击按钮和鼠标动作的多种情况。此参数里的各位可以是下列值的任何合理组合：
MOUSEEVENTF_ABSOLUTE：表明参数dx，dy含有规范化的绝对坐标。
MOUSEEVENTF_MOVE：表明发生移动，相对于上次位置的改动位置。
MOUSEEVENTF_LEFTDOWN：表明接按下鼠标左键。
MOUSEEVENTF_LEFTUP：表明松开鼠标左键。
MOUSEEVENTF_RIGHTDOWN：表明按下鼠标右键。
MOUSEEVENTF_RIGHTUP：表明松开鼠标右键。
MOUSEEVENTF_MIDDLEDOWN：表明按下鼠标中键。
MOUSEEVENTF_MIDDLEUP：表明松开鼠标中键。
MOUSEEVENTF_WHEEL：在Windows NT中如果鼠标有一个轮，表明鼠标轮被移动。移动的数量由dwData给出。
dx：当dwFlags设置为MOUSEEVENTF_ABSOLUTE的时候，这里的dx就表示在屏幕中的绝对位置的x坐标。也就是说，这个值是多少，鼠标就位于屏幕的这个点处（相对于左上角（0,0））。需要注意的是，dX和dy是标准化的绝对坐标，其值在0到65535之间。事件程序将此坐标映射到屏幕上。坐标（0，0）映射到显示表面的左上角，（65535，65535）映射到右下角。所以我们的绝对坐标是要规范化到（65535，65535）这个范围再传入这个函数。当dwFlags设置为MOUSEEVENTF_MOVE的时候，dx表示从上次鼠标事件产生以来移动的数量。例如，上次坐标是在屏幕的（100,200）处，如果dx=10，那么这时候就处于（110,200）处了（不过其实系统会乘以一个倍率的，好像在控制面板的鼠标选项的灵敏度中选）。正值表示鼠标向右（或下）移动；负值表示鼠标向左（或上）移动。
dy：和上面的dx一样，只是dy是y轴上的绝对坐标或者相对移动值。
dwData：如果dwFlags为MOUSEEVENTF_WHEEL，则dwData指定鼠标轮移动的数量。正值表明鼠标轮向前转动，即远离用户的方向；负值表明鼠标轮向后转动，即朝向用户。一个轮击定义为WHEEL_DELTA，即120。如果dwFlagsS不是MOUSEEVENTF_WHEEL，则dWData应为零。
dwExtralnfo：指定与鼠标事件相关的附加32位值。
在我们的系统中，使用的是相对坐标。我们得到上一帧对手的跟踪点，然后得到当前帧的手的跟踪点，两个跟踪点在x和y方向的距离dx和dy就是手的移动距离，然后我们乘以一个倍率k，再传入mouse_event()函数中，如下：
mouse_event(MOUSEEVENTF_MOVE,-k * dx, k * dy, 0, 0);
**2.2、鼠标点击：检测握拳**
我们通过训练adaboost拳头分类器类检测拳头。为了减少耗时，我们跟踪到手后，只在手的位置去检测握拳就可以了。为了避免误检，当重复检测到三次拳头后，我们才相信用户是握拳了。另外，用户的一次握拳可能会被多次检测到，所以当检测到一次握拳后，会等待一定的时间，在这个时间内检测到拳头都会被当成是上次的握拳动作，而被无情却又智能的忽略掉。
mouse_event 函数同时可以模拟鼠标的左键按下：我们设置dwFlags 标志为MOUSEEVENTF_LEFTDOWN时表示左键按下，为MOUSEEVENTF_LEFTUP表示左键松开，向系统发送相应消息。需要注意的是，激活鼠标按下的事件后，都要注意还原，即按下键之后要松开，也就是：
mouse_event(MOUSEEVENTF_LEFTDOWN,0, 0, 0, 0);
mouse_event(MOUSEEVENTF_LEFTUP,0, 0, 0, 0);
完成一个单击动作。
**三、人机交互系统的实现**
**3.1、系统初始化：手掌的检测**
在这里，我们也需要训练一个adaboost手掌分类器。这样，当用户举起手准备控制的时候，我们可以获知，并将人手的位置告诉meanshift跟踪算法，吩咐他要跟踪这么个东西。另外，为了一定程度的避免误检，我们需要在手掌分类器检测得到手的区域的时候，我们分析这个区域是否包含足够的肤色区域，因为是人手的，如果它检测的是人手，那么很大的区域肯定是肤色的。当然了，太小的手或者检测到的目标，或者手离图像的边缘太近，系统也会放弃跟踪的，不会理会这种很大可能会出错的跟踪。
**3.2、系统实现代码**
我的代码是基于VS2010+ OpenCV2.4.2的。在这里贴上代码，至于里面的手掌和拳头分类器，鉴于某些原因，无法公开，大家可以下载腾讯的[QQ手势达人forPPT](http://labs.qq.com/product.php?id=1)。安装软件后，在软件的安装目录里面有dat后缀的手掌和拳头分类器，同样可以用opencv的接口来加载和使用。在我的代码里面直接替换我的xml后缀的分类器即可。
**main.cpp**

```cpp
// Hand tracking using meanshift and mouse control
// Author : zouxy
// Date   : 2014-01-06
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "opencv2/opencv.hpp"
#include "handTracker.h"
using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
	Mat frame;	
	Rect trackBox;
	Point prePoint, curPoint;
	VideoCapture capture;
	bool gotTrackBox = false;
	int interCount = 0;
	int continue_fist = 0;
	capture.open(0);
	if (!capture.isOpened())
	{
		cout<<"No camera!\n"<<endl;
		return -1;
	}
	HandTracker hand_tracker;
	while (1)
	{
		/************ Get hand which need to be tracked ************/
		while (!gotTrackBox)
		{
			capture >> frame;	
			if(frame.empty())
				return -1;
            if (hand_tracker.init(frame, trackBox))
			{
                gotTrackBox = true;
				prePoint = Point(trackBox.x + 0.5 * trackBox.width, trackBox.y + 0.5 * trackBox.height);
			}
			imshow("handTracker", frame);
				
			if (waitKey(2) == 27)
				return -1;
		}
		capture >> frame;
		double t = (double)cvGetTickCount();
		
		/****************** Tracking hand **************************/
		if (!hand_tracker.processFrame(frame, trackBox))
			gotTrackBox = false;

		/****************** Control the mouse *********************/
		curPoint = Point(trackBox.x + 0.5 * trackBox.width, trackBox.y + 0.5 * trackBox.height);
		int dx = curPoint.x - prePoint.x;
		int dy = curPoint.y - prePoint.y;
		float k = 1.5;
		mouse_event(MOUSEEVENTF_MOVE, -k * dx, k * dy, 0, 0);
		prePoint = curPoint;
		// When you made a fist, means you click left button of mouse
		// To avoid successive active within short time, when you had actived a single 
		// we will ingnore the next 10 frames
		interCount++;
		if(interCount > 10 && hand_tracker.detectFist(frame, trackBox))
		{
			// To avoid detecting error, need to successive detect three times successfully
			continue_fist++;
			if (continue_fist > 3)
			{
				interCount = 0;
				cout << "YOU active a single click command!" << endl;
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
		}
		else
			continue_fist = 0;

		/******************* Show image ****************************/
		rectangle(frame, trackBox, Scalar(0, 0, 255), 3);
		imshow("handTracker", frame);

		/******************* Show cost time ************************/
		t = (double)cvGetTickCount() - t;
        cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;
		if ( cvWaitKey(3) == 27 )
			break;
	}
	return 0;
}
```
**handTracker.h**

```cpp
// hand tracking using meanshift
#pragma once
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
class HandTracker
{
public:
	HandTracker(void);
	~HandTracker(void);
    bool init(Mat frame, Rect &trackBox);
	bool processFrame(Mat frame, Rect &trackBox);
	bool detectFist(Mat frame, Rect trackBox);
private:
	bool isHand(const Mat frame);
	void detectPalm( Mat img, Rect &box);
	void frameDiff(const Mat image, Mat &diff);
	void calSkinPro(Mat frame);
    void getSkinModel(const Mat img, Rect rect);
    
private:
	Mat backProject;
	MatND hist;
    CascadeClassifier palmCascade;
	 CascadeClassifier fistCascade;
	Mat preGray;
    int successiveDetect;
};
```
**handTracker.cpp**

```cpp
// Hand tracking algorithm using meanshift and mouse control
// Author : zouxy
// Date   : 2014-01-06
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "handTracker.h"
HandTracker::HandTracker()
{
    successiveDetect = 0;
    const char *palmCascadeName = "palmCascadeClassifier.xml";
	const char *fistCascadeName = "fistCascadeClassifier.xml";
    if (!palmCascade.load(palmCascadeName) || !fistCascade.load(fistCascadeName))
    {
        cout<<"Can not load cascade!"<<endl;
    }
}
HandTracker::~HandTracker()
{
}
// init function: detect hand region and init meanshift
bool HandTracker::init(Mat frame, Rect &trackBox)
{
    trackBox = Rect(0, 0, 0, 0);
	// detect hand
    detectPalm(frame, trackBox);
	// The detected box should large enough and not near the boundary of image
    if (trackBox.area() > 900 && 0.3 * frame.cols < trackBox.x + 0.5 * trackBox.width 
							&& trackBox.x + 0.5 * trackBox.width < 0.7 * frame.cols 
							&& 0.3 * frame.rows < trackBox.y + 0.5 * trackBox.height 
							&& trackBox.y + 0.5 * trackBox.height < 0.7 * frame.rows)
    {
		// Check skin area of the detected box to make sure it is a hand
        if (isHand(frame(trackBox)))
        {
			// To avoid detecting error, need to successive detect twice successfully
            successiveDetect++;
            if (successiveDetect > 2)
            {
                // Calculate skin probability model for meanshift
                getSkinModel(frame, trackBox);
                successiveDetect = 0;
                return true;
            }
        }
    }
    return false;
}
// detect hands and return the biggest hand
void HandTracker::detectPalm( Mat img, Rect &box)
{
    double scale = 1.3;
    Mat small_img, gray;
    vector<Rect> boxs;
    gray.create(img.rows, img.cols, CV_8UC1);
    small_img.create( cvRound (gray.rows/scale), cvRound(gray.cols/scale), CV_8UC1 );
    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, small_img, small_img.size(), 0, 0, INTER_LINEAR );
    equalizeHist( small_img, small_img );
    palmCascade.detectMultiScale( small_img, boxs, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    //Get the bigest face
    Rect maxBox(0, 0, 0, 0);
    for (vector<Rect>::const_iterator r = boxs.begin(); r != boxs.end(); r++)
    {
        if (r->area() > maxBox.area())
            maxBox = *r;
    }
    if (boxs.size() > 0)
    {
        box.x = cvRound(maxBox.x * scale);
        box.y = cvRound(maxBox.y * scale);
        box.width = cvRound(maxBox.width * scale);
        box.height = cvRound(maxBox.height * scale);
    }
}
// check skin area of our tracking box to make sure it is a hand
bool HandTracker::isHand(const Mat frame)
{
    Mat YCbCr;
    vector<Mat> planes;
    int count = 0;
    cvtColor(frame, YCbCr, CV_RGB2YCrCb);
    split(YCbCr, planes);
    MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
                        it_Cb_end = planes[1].end<uchar>();
    MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	// skin satisfy: 138 <= Cr <= 170 and 100 <= Cb <= 127 (empirical value)
    for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb)
    {
        if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)
            count++;
    }
    // It is a hand when contains large enough skin area
    return (count > 0.4 * frame.cols * frame.rows);
}
// Calculate skin probability model (histogram) for meanshift
void HandTracker::getSkinModel(const Mat img, Rect rect)
{
	int hue_Bins = 50;	
	float hue_Ranges[] = { 0, 180 }; 
	const float *ranges= hue_Ranges;
	Mat HSV, hue, mask;
	cvtColor(img, HSV, CV_RGB2HSV);
	inRange(HSV, Scalar(0, 30, 10), Scalar(180, 256, 256), mask);
	vector<Mat> planes;
	split(HSV, planes); 
	hue = planes[0];
	Mat roi(hue, rect), maskroi(mask, rect);
    calcHist(&roi, 1, 0, maskroi, hist, 1, &hue_Bins, &ranges);
    normalize(hist, hist, 0, 255, CV_MINMAX);
}
// Calculate skin probability image (back project map) for meanshift
void HandTracker::calSkinPro(Mat frame)
{
	Mat mask, hue, HSV;
	cvtColor(frame, HSV, CV_RGB2HSV);
	inRange(HSV, Scalar(0, 30, 10), Scalar(180, 256, 256), mask);
	vector<Mat> planes;
	split(HSV, planes); 
	hue = planes[0];
	// hue varies from 0 to 179, see cvtColor
	float hue_Ranges[] = { 0, 180 };  
	const float *ranges= hue_Ranges;
	calcBackProject(&hue, 1, 0, hist, backProject, &ranges, 1.0, true);
	backProject &= mask;
}
//  Detect motion using frame differece
void HandTracker::frameDiff(const Mat image, Mat &diff)
{
	int thresValue = 20;
	Mat curGray;
	cvtColor(image, curGray, CV_RGB2GRAY);
	if (preGray.size != curGray.size)
		curGray.copyTo(preGray);
	absdiff(preGray, curGray, diff);
	threshold(diff, diff, thresValue, 255, CV_THRESH_BINARY);
	erode(diff, diff, Mat(3, 3, CV_8UC1), Point(-1,-1));
	dilate(diff, diff, Mat(3, 3, CV_8UC1), Point(-1,-1));
	curGray.copyTo(preGray);
}
// Tracking hand using meanshift
bool HandTracker::processFrame(Mat frame, Rect &trackBox)
{
	float rate = 0.9;
	Mat diff;
	// tracking hand
	calSkinPro(frame);        // skin information
	frameDiff(frame, diff);   // motion information
	// fusing skin and motion information using a weighted rate
	Mat handProMap = backProject * rate + (1 - rate) * diff;
	meanShift(handProMap, trackBox, TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
	
	// ensure the tracking result is a hand
	Mat skin = backProject(trackBox) > 100;
	return countNonZero(skin) > 0.4 * trackBox.area();
}
// Detect fist for the command: click the mouse
bool HandTracker::detectFist(Mat frame, Rect palmBox)
{
	Rect detectFistBox;
	detectFistBox.x = (palmBox.x - 40) > 0 ? (palmBox.x - 40) : 0;
	detectFistBox.y = (palmBox.y - 20) > 0 ? (palmBox.y - 20) : 0;
    detectFistBox.width = palmBox.width + 80;
	detectFistBox.height = palmBox.height + 40;
	detectFistBox &= Rect(0, 0, frame.cols, frame.rows);
    
	Mat gray;
	cvtColor(frame, gray, CV_BGR2GRAY );
	Mat tmp = gray(detectFistBox);
	vector<Rect> fists;
    fistCascade.detectMultiScale(tmp, fists, 1.1, 2, CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    return fists.size();
}
```

**四、总结**
人机交互是技术发展的一个趋势。包括语音交互、体感交互、增强现实等等。例如之前的kinect、siri和科大讯飞等等，还有当下沸沸扬扬的可穿戴设备。无数创业新星的诞生也都瞄准了这块蛋糕。无愧为创业的十大方向之一。当然了，就当下的技术而言，语音交互还是蛮成熟的了，至少从应用来讲，已经遍及大地了。这也得益于语音识别技术的提升和云计算的支撑。但对于2D摄像头的手势控制来说，做得比较好的就是以色列的公司了，包括三星智能电视所采用的PointGrab。还有目前国内彩电巨头，创维、康佳、TCL等，在今年都纷纷与这些手势控制的公司合作，以将其嵌入到他们的智能电视中去。然而，手势交互虽然是未来的趋势，但目前的手势操作使用起来还是很疲劳的，手势交互还没上升到真正自然和轻松的交互，这也是它暂时无法取代鼠标的原因之一。如果能像科幻片所刻画的那样，那才是它造福人类之日了。
人工智能的目标也是让计算机能听、能说、能看、能感觉、能做和“听话”，用的越多越听话。可以像人一样，感知这个美好的世界。通过模拟人类五个感官：视觉（手势）、声音（语音）、触觉（触屏）、味觉和嗅觉），为用户提供了包括光、声、力、嗅、味等全方位、多角度的真实感觉与优秀的体验。
人工智能的未来在哪里？在那里！
**五、参考文献**
[1][ 反向投影图](http://blog.csdn.net/poiiy333/article/details/9051409)
[2][Meanshift，聚类算法](http://www.cnblogs.com/liqizhou/archive/2012/05/12/2497220.html)
[3][mean-shift算法概述](http://wenku.baidu.com/link?url=p-weE1NtT_P9LKUnBVj0RLwIDm4iD8zXnn8e6Q20j9kMS8ctjJ1JZEg5d6Gs9U0vzpLmS0PRRgJtzPqQ9IqQG0IQEIq9oBrrO0eHGsM_vPy)
[4][meanShift算法介绍](http://blog.csdn.net/carson2005/article/details/7337432)
[5][mouse_event函数说明](http://baike.baidu.com/link?url=lra25POPdtYtdHN6Kbu6Jc6t6XVoKOmeMG3W_ThcwjFb2Nlr2IDJ5zUYFm_-SLQS4o3iax2K_RsgFC6-62L0dK)


