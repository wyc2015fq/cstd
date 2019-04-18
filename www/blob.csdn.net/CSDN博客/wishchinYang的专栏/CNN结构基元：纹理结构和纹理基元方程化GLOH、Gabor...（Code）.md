# CNN结构基元：纹理结构和纹理基元方程化GLOH、Gabor...（Code） - wishchinYang的专栏 - CSDN博客
2017年12月26日 11:02:28[wishchin](https://me.csdn.net/wishchin)阅读数：2319
       模式识别专注于寻找相同模式的共性和不同模式的分离。CNN把特征提取全局化，其中重要的一个是纹理特征，利用卷积核来表示纹理基元，用以重现模式，应如何显示表示。
       第一次使用纹理特征，2012年，使用了灰度共生矩阵：[灰度共生矩阵](http://blog.csdn.net/wishchin/article/details/9355771?locationNum=8&fps=1)-/Laws特征。此后LBP特征也是一种常用的特征：[局部二元模式](http://blog.csdn.net/wishchin/article/details/34815093)LBP特征。  此外Gabor变换用以提取纹理，可以使用变换后的能量和方差进行计算特征， Gabor函数变换：[Gabor滤波器学习](https://blog.csdn.net/jinshengtao/article/details/17797641)-。
       纹理模式分析CNN化的可行性分析：纹理基于子对象模式，纹理从直觉上即可分解为基元的基元，即分层化。纹理模式识别，模型构建方法即为寻找纹理模式的基元分布模式，可通过多层CNN结构堆叠而成，且描述直觉上比类别模式更为简单清晰。
**百度百科：**
      纹理分析指通过一定的图像处理技术提取出纹理[特征参数](https://baike.baidu.com/item/%E7%89%B9%E5%BE%81%E5%8F%82%E6%95%B0/8305827)，从而获得纹理的定量或定性描述的处理过程。纹理分析方法按其性质而言，可分为4大类：统计分析方法、结构分析方法、信号处理方法和模型方法。
本质属性-共性：
      定义1  纹理是一种反映图像中**同质现象**的视觉特征，体现了物体表面共有的内在属性，包含了物体表面结构组织排列的重要信息以及它们与周围环境的联系。
      定义2  按一定规则对元素（elements）或基元（primitives）进行排列所形成的**重复模式**。
      定义3  如果图像函数的一组**局部属性是恒定的**，或者是**缓变的**，或者是**近似周期性的**，则图象中的对应区域具有恒定的纹理。
      本质属性的共性定义，表示纹理一旦确定，一定可以形式化，必然能找到一个特定形式的数学方程来描述此种纹理模式的唯一性，在CNN中，可以表示为特诊个提取的特定卷积核的组合模式。
**纹理属性：**
        纹理基元和分布模式、影调。一个纹理基元（不严格地说）是一个具有一定的不变特性的视觉基元。这些不变特性在给定区域内的不同位置上，以不同的变形和不同的方向重复出现。纹理基元最基本的不变特性之一是区域内象素的色彩/灰度分布。
        分布模式，可以以空间描述的纹理结构描述，特定纹理基元通过不同的空间组合组合成不同纹理。
      影调也是表示灰度的明暗分布。因此，我们认为影调和纹理不是独立的概念：当在图象的一定面积区域中影调基元的变化很小时，这个区域的主导特性是影调。当在小面积区域中含大量不同的影调，这个区域占主导的特性是纹理。
**纹理模式构架**（[纹理分析](http://blog.csdn.net/langb2014/article/details/45619819)）：
      纹理特征建立在子对象基础之上，这就意味着，必须有一个对象等级架构去使用他们，精确的分割是使用形状纹理特征的基础，子对象的分割要尽量有意义。
     1. 考虑子对象光谱特征的纹理
     2. 考虑子对象形状特征的纹理
     3. 基于灰度共生矩阵的纹理
**方法简述文章**
参考文章：[纹理图像分析的基本方法](http://www.dataguru.cn/article-11537-1.html)简述 。
      纹理是物体表面固有的一种特性，所以图像中的区域常体现出纹理性质。纹理可以认为是灰度（颜色）在空间以一定的形式变化而产生的团（模式）。纹理与尺度有密切的关系，一般仅在一定的尺度上可以观察到，对纹理的分析需要在恰当的尺度上进行。纹理还具有区域性质的特点，通常被看做对局部区域中像素之间关系的一种度量，对于单个像素来说讨论纹理是没有意义的。一把情况下目前常用的纹理分析方法中有以下三种：统计法，结构法，频谱法。下面分别介绍。
**1. 纹理描述的统计方法**
      最简单的统计法借助于灰度直方图的矩来描述纹理，比如直方图的二阶矩是灰度对比度的度量，可以用于描述直方图的相对平滑程度；三阶矩表示了直方图的偏度；四阶矩表示的直方图的相对平坦型等等。但是仅借助灰度直方图的矩来描述纹理没能利用像素相对位置的空间信息，为了利用这些信息，我们可以建立区域的灰度共生矩阵。
      一个带有详细GLOH讲解和代码的页面：[http://blog.csdn.net/guanyuqiu/article/details/53117507](http://blog.csdn.net/guanyuqiu/article/details/53117507)
1.1** 灰度共生矩阵**
设 S 为目标区域 R 中具有特定空间联系（可由位置算子确定）的象素对的集合，共生矩阵 P 中的元素（ #代表数量）
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115253lfgfw34nwlu2gzl7.png)
分子：具有某种空间关系、灰度值分别为g1和g2的象素对的个数
分母：象素对的总和个数
上面提到了一个概念，位置算子，位置算子其实就是象素对的特定空间联系，比如向右1个象素和向下1个象素。共生矩阵的大小一般为k x k矩阵（k为所求图像的灰度级数）。举个栗子如下：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115253wo2ox5w6sg6yzxpg.jpg)
上图a为3个灰度级的图象（ g1 = 0， g2 = 1， g3 = 2），位置算子为：向右1个象素和向下1个象素，b图按照位置算子计算得到的灰度共生矩阵，c图为共生矩阵归一化的结果。然而，为了更好的对图像分析，一般常用由共生矩阵产生的纹理描述符，比如：二阶矩，对应图像的均匀性或平滑性；熵，给出图像内容随机性的度量；对比度，反应紧邻像素间的反差等。
1.2. 基于**能量的纹理描述符**
      通过利用**模板（也称核）**计算局部纹理能量可以获得灰度变化的信息，如果设图象为I，模板为M1, M2, …, MN，则卷积 Jn = I * Mn, n = 1, 2, …, N 给出了各个象素邻域中的纹理能量分量，如果采用尺寸为k × k的模板，则对应第n个模板的纹理图像（的元素）为：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115253neipapm8k8taeu8h.png)
常见的一维模板有：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115253p04668ncr2m6zer9.png)
其中L代表层（level），E代表边缘（edge），S代表形状（shape），W代表波（wave），R代表纹（ripple），例如L5给出中心加权的局部平均， E5检测边缘。对应的二维模板常用两个一维模板（行模板和列模板）的卷积得到。这里不再赘述。
**2. 纹理描述的结构方法**
2.1 **结构描述法基础**
      一般认为纹理是由许多相互接近的，互相编织的元素构成（它们常具有周期性），所以纹理描述可提供图像区域的平滑，稀疏，规则性等特性。**结构法是一种空域的方法**，其基本思想是认为复杂的纹理可由一些简单的纹理基元（基本纹理元素）以一定的有规律的形式重复排列组合而成。
结构描述的关键点有两个：一是确定纹理基元；二是建立排列规则。一个纹理基元是由一组属性所刻画的相连通的像素集合，设纹理基元为h(x, y)，排列规则为r(x, y)，则纹理t(x, y)可表示为：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115253ibsa8vihaz06uarh.png)
       为了用结构法描述纹理，在获得纹理基元的基础上，还要建立将它们及逆行排列的规则，排列规则和方式可用形式语法来定义，其中t表示纹理基元，a表示向右移动，b表示向下移动：
(1) S -> aS（变量S可用aS来替换）
(2) S -> bS（ 变量S可用bS来替换）
(3) S -> tS（变量S可用tS来替换）
(4) S -> t（变量S可用t来替换）
     例如，t是下图a的一个纹理基元，它也可以直接由上述规则(4)得到。如果依次使用规则(3),(1),(3),(1),(3),(1),(4),可得到tatatat,即生成如图b的图案。如果依次使用规则(3),(1),(3),(2),(3),(1),(3),(1),(4),即可得到下图c的图案。
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115254paonk7xndnakdi7n.jpg)
比较规则的纹理在空间中可以用有次序的形式通过纹理镶嵌来构建，比如下图，通过使用一种正多边形进行拼接组合。
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115254i8j16w6j9t6k34s7.png)
2.2 **局部二值模式（LBP）**
      局部二值模式（LBP）是一种纹理分析算子，是一个借助局部邻域定义的纹理测度。它属于点样本的估计方式，具有尺度不变性，旋转不变性和计算复杂度低等优点。
      对一个象素的3 x 3邻域里的象素按顺序阈值化，将结果看作一个二进制数，并作为中心象素的标号，由256个不同标号得到的直方图可进一步用作区域的纹理描述符 ,如下图：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115255bg790xy414g0000x.jpg)
当然也可以使用不同尺寸的邻域对基本LBP算子进行扩展。用(P, R)代表一个象素的邻域，在这个邻域里有P个象素 圆半径为R。如下图：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115256vtzbt0lkzbtkxxk7.jpg)
将一个邻域中的象素按顺序循环考虑，如果它包含最多两个从0到1或从1到0的过渡，则这个二值模式就是均匀的，根据LBP的标号可以获得不同的局部基元。如下：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115256t8jioqzdjzd33qga.jpg)
**3. 纹理描述的频谱方法**
       一般来说，纹理和图像频谱中的高频分量是密切联系的。光滑的图像（主要包含低频分量）一般不当做纹理图像看待。频谱法对应变换域的方法，着重考虑的是纹理的周期性。
3.1** 傅里叶频谱**
     傅里叶频谱可借助傅里叶变换得到，它有三个合适描述纹理的性质：
(1) 傅里叶频谱中突起的峰值对应纹理模式的主方向
(2) 这些峰在频域平面的位置对应模式的基本周期
(3) 利用滤波把周期性成分除去，用统计方法描述剩下的非周期性部分
在实际的特征检测中，为简便起见可把频谱转换到极坐标中。此时频谱函数可用S(r, θ )表示，比如：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115256izjq16611g5q1vvm.jpg)
如果纹理具有空间周期性，或具有确定的方向性，则能量谱在对应的频率处会有峰。以这些峰为基础课组建模式识别所需的特征。
3.2 ** Gabor频谱**
      Gabor滤波器用于人脸检测：[人脸识别的Gabor特征](http://blog.csdn.net/hlx371240/article/details/49101035)
      Gabor滤波器代码：[OpenCV实现Gabor滤波器](http://blog.csdn.net/lichengyu/article/details/24534245)2     
      Gabor函数变换：[Gabor滤波器学习](https://blog.csdn.net/jinshengtao/article/details/17797641)--
      Gabor频谱，也有成为盖伯频谱，源自于Gabor变换。如果在**傅里叶变换中加上窗函数**，就构成短时傅里叶变换，再进一步，如果短时傅里叶变换的窗函数为高斯函数，则构成Gabor变换。由于高斯函数的傅里叶变换仍为高斯函数，所以，Gabor变换再空域和频域都具有局部性，或者说可以将能量进行集中。
实际中常使用两个成对的实Gabor滤波器，其中对称的和反对称的滤波器响应分别为：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115256mhsqqq2eqf21skkk.jpg)
将上述两个盖伯滤波器旋转和放缩，可分别获得一组朝向和带宽均不同的滤波器，并覆盖整个平面，如下图：
![](http://attachbak.dataguru.cn/attachments/portal/201706/12/115256tg59wefwh446eeei.jpg)
通过利用一组基于Gabor变换的滤波器可将图像分别转换到一系列的频率带中。
**4. 小结**
      本文主要从统计方法，结构方法以及频谱的方法对纹理图像的描述进行了初步的概述，以便读者进行关于对纹理图像的分析方面有一个初步的了解。本文部分内容参考章毓晋的图像工程（中册）之图像分析，感谢！
**5. Code**
//获取一定参数下的纹理特征
```
//产生纹理特征：图片熵、灰度共生矩阵、Gabor滤波器
//输入：RGB彩色图像  输出：纹理特征
//计算结果：得到GLOH直方图；
//得到四个值：对比度、能量、熵、逆方差、相关性
int ColorML::GenFeatTexture( cv::Mat &image, fvTexture &ftexture )
{
	Mat src; image.copyTo(src);
	Mat src2; image.copyTo(src2);
	if (src.empty()){
		return -1;
	}
	//1.得到灰度共生矩阵 函数值，此程序中有四个方向，共16个特征，后16维则为0
	getFeatGLOH( src, ftexture.fvGLOH );
	//2.得到Gabor滤波器的特征:32维
	//不能修改频率和尺度，需要再在最后结果上做文章
	int U = 3;//尺度：-1~3;频率：0~4;
	int V = 4;//论文U,V取7,4共得到（4+1）*（7+1）= 40个卷积核
	//修改为4个频率，四个尺度，得到16个图像，最后得到64个特征，必须与fvGabor 对应
	//在此计算中，只使用能量和熵//有两个值为空值
	getFeatGabor(src2, ftexture.fvGabor,U,V);
	ftexture.combine();
	ftexture.normal();
	return 1;
}
```
//获取Gabor核和GLOH特征
```
//从多个图像中获取灰度共生矩阵
int ColorML::getFeatGLOH(cv::Mat &src, std::vector<float> &featV){
	//代码地址：http://blog.csdn.net/yanxiaopan/article/details/52356777
	//更详细的理论和代码：http://blog.csdn.net/guanyuqiu/article/details/53117507
	Mat dst_horison, dst_vertical, dst_45, dst_135;
	//使用HSV的灰度共生矩阵，直接使用H矩阵
	cv::Mat  hm, sm, vm, hsvm;
	vector<cv::Mat>  hsvs;
	cvtColor(src, hsvm, COLOR_BGR2HSV);
	cv::split(hsvm, hsvs);//分为3个通道   
	//hm = hsvs[0].clone();
	//sm = hsvs[1].clone();
	//vm = hsvs[2].clone();
	Mat src_gray;
	//cvtColor(src, src_gray, COLOR_BGR2GRAY);
	src_gray = hsvs[0].clone();
	//四个方向的直方图//计算特征
	//ColorML* cm = new ColorML();
	double eng_horison = 0, con_horison = 0, idm_horison = 0, asm_horison = 0;
	ColorML::getglcm_horison(src_gray, dst_horison);
	ColorML::getglcm_45(src_gray, dst_45);
	ColorML::getglcm_vertical(src_gray, dst_vertical);
	ColorML::getglcm_135(src_gray, dst_135);
	
	bool isTest = g_mlTest;
	if (isTest)	{
		//ColorML::feature_computer(dst_horison, asm_horison, eng_horison, con_horison, idm_horison);
		//cout << "asm_horison:" << asm_horison << endl;
		//cout << "eng_horison:" << eng_horison << endl;
		//cout << "con_horison:" << con_horison << endl;
		//cout << "idm_horison:" << idm_horison << endl;
		int f = 1;
		//mutilM(dst_horison, f);
		//mutilM(dst_45, f);
		//mutilM(dst_vertical, f);
		//mutilM(dst_135, f);
		printM(dst_horison);
		printM(dst_45);
		printM(dst_vertical);
		printM(dst_135);
		cv::imshow("RGB", src);
		cv::imshow("H-Image", src_gray);
		//cv::imshow("dst_horison", dst_horison);
		//cv::imshow("dst_45", dst_45);
		//cv::imshow("dst_vertical", dst_vertical);
		//cv::imshow("dst_135", dst_135);
		cv::waitKey();
	}
	//装填入ftexture特征中
	//取灰度矩阵的每一列的值（每一行都会相同)，4个方向共64维
	std::vector<cv::Mat> gMats;
	gMats.push_back(dst_horison);
	gMats.push_back(dst_45);
	gMats.push_back(dst_vertical);
	gMats.push_back(dst_135);
	//int consLen = fvTexture::lenFeat;//限制长度
	//std::vector<int>  featV;
	//getFeatGLOH(gMats, featV, consLen);//此函数未必正确，只是截取前端，不能进行滤波
	//
	//得到灰度共生矩阵直方图
	//getFeatHistGLOH(gMats, ftexture.fvGLOH, consLen);
	//1.得到每个方向灰度共生矩阵的 5个函数：对比度、能量、熵、逆方差、相关性
	//本程序中使用四个方向，则有后10位为0 的空值，相关性未获得
	getFeatGLOH( gMats, featV, fvTexture::lenFeat );
	return 1;
}
```
```
//从多个图像中获取灰度共生矩阵
//取每一行的均值
int ColorML::getFeatGLOH(std::vector<cv::Mat> &gMats, std::vector< float>  &featV, int consLen){
	std::vector< std::vector< float>>  feat(gMats.size());
	for (int k = 0; k < gMats.size();++k){
		cv::Mat src = gMats[k].clone();
		double Asm = 0;
		double Eng = 0;
		double Con = 0;
		double Idm = 0;
		feature_computer(src,  Asm, Eng, Con, Idm);
		feat[k].push_back(Asm);
		feat[k].push_back(Eng);
		feat[k].push_back(Con);
		feat[k].push_back(Idm);
	}
	for ( int i = 0; i < featV.size() / 4 && i< feat.size(); ++i ){
		featV[4*i] = feat[i][0];
		featV[4*i+1] = feat[i][1];
		featV[4 * i + 2] = feat[i][2];
		featV[4 * i + 3] = feat[i][3];
	}
	return 1;
}
```
```
//获取Gabor图像
int ColorML::getFeatGabor( cv::Mat &image, std::vector<float> &featV ,int U ,int V)
{
	int type = 0;
	if (!image.data){
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	//缩放图像
	int nr = image.rows / 640;
	int nc = image.cols / 640;
	if (nr > 1 || nc > 1){
		int maxFactor = std::max(nr, nc) + 1;
		cv::resize(image, image, cv::Size(image.rows / maxFactor, image.cols / maxFactor), cv::INTER_LINEAR);
	}
	//使用HSV的灰度共生矩阵，直接使用H矩阵
	cv::Mat  hm, sm, vm, hsvm;
	vector<cv::Mat>  hsvs;
	cvtColor(image, hsvm, COLOR_BGR2HSV);
	cv::split(hsvm, hsvs);//分为3个通道   
	//hm = hsvs[0].clone();
	//sm = hsvs[1].clone();
	//vm = hsvs[2].clone();
	//Mat src_gray;
	//cvtColor(src, src_gray, COLOR_BGR2GRAY);
	image = hsvs[0].clone();
	//建议不使用归一化，为了方便能量计算
	//garbor kernel的参数，包括kernel_size 和 v的范围，都是为了适应不同图像的大小。
	//v 越小，Gabor函数的宽度越小，越能刻画细节信息，适应于较小的图像
	//19x19, 25x25, 35x35, 49x49, 69x69.
	const int kernel_size = 19;
	vector<Mat > gaborMats;
	Mat filterd_image;////filterd_image = GaborFilter(image, type, kernel_size, U, V);
	filterd_image = GaborFilter(image, gaborMats, type, kernel_size, U, V);
	bool isTest = g_mlTest;
	if (isTest)	{
		//printM(gaborMats[0]);//char型正常
		imshow("origin image", image);
		cv::namedWindow("filtered image");
		cv::resize(filterd_image, filterd_image, cv::Size(1280, 800));
		imshow("filtered image", filterd_image);
		//normalize(filterd_image, filterd_image, 0, 255, CV_MINMAX);  
		//filterd_image.convertTo(filterd_image, CV_8U);  
		//imwrite("filterd_image.jpg",filterd_image);  
		waitKey();
	}
	//计算Gabor图像的特征：5个函数：对比度、能量、熵、逆方差、相关性；相关性未计算
	//这样共得到160个特征，太多了！最多只能40个
	double eng = 0, con = 0, idm = 0, asmf = 0;
	
	for (int i = 0; i < gaborMats.size();++i){
		int idx = 4 * i;
		float factor = 1.0;// / gaborMats[i].rows * gaborMats[i].cols;
		factor *= (gray_level*gray_level);
		cv::Mat m = gaborMats[i];
		downSampleTo16(m);
		downSampleV16( m );
		ColorML::feature_computer( m, asmf, eng, con, idm );
		featV[idx + 0] = asmf / factor;
		featV[idx + 1] = eng / factor;
		//featV[idx + 2] = con / factor;//只取能量和熵
		//featV[idx + 3] = idm / factor;
	}
	
	return 1;
}
```
```
//对实数和复数进行分解，还是只使用实数核的好！
cv::Mat  ColorML::GaborFilter(
	Mat& image, std::vector<cv::Mat> &matsGabor, 
	int type, 
	const int kernel_size, int U, int V)
{
	// Ref: Mian Zhou. Thesis. Gabor-Boosting Face Recognition.  
	// https://code.google.com/p/gaborboosting/ 
	//const int kernel_size = 69; // should be odd  
	cv::Mat img;
	if (image.type() == CV_8UC3){
		cv::cvtColor(image, img, COLOR_BGR2GRAY);
	}else{
		image.copyTo(img);
	}
	// variables for gabor filter  
	double Kmax = M_PI / 2;
	double f = sqrt(2.0);
	double sigma = 2 * M_PI;
	//int U = 7;
	//int V = 4;
	int GaborH = kernel_size;
	int GaborW = kernel_size;
	int UStart = 0, UEnd = U+1;
	int VStart = 0, VEnd = V;//int VStart = -1, VEnd = V;
	// variables for filter2D  
	Point archor(-1, -1);
	int ddepth = CV_64F;//CV_64F  
	double delta = 0;
	// filter image with gabor bank
	Mat kernel_re, kernel_im;
	Mat dst_re, dst_im, dst_mag;
	Mat totalMat, totalMat_re, totalMat_im;
	//把Bank里面的抽取出来，得到不同方向U和不同频率V上的小波核，每一个进行滤波
	for (U = UStart; U < UEnd; U++){
		Mat colMat, colMat_re, colMat_im;
		for (V = VStart; V < VEnd; V++){
			switch (type)
			{
			case 0://计算实数
				kernel_re = getGaborKer(GaborW, GaborH, U, V, Kmax, f, sigma, CV_64F, "real");
				//printM(kernel_re);//uchar型无误
				filter2D(img, dst_re, ddepth, kernel_re);
				//normalize(dst_re, dst_re, 0, 1, CV_MINMAX);
				//printM(dst_re);//char型无误
				matsGabor.push_back(dst_re);
				break;
			case 1://计算虚数
				kernel_im = getGaborKer(GaborW, GaborH, U, V, Kmax, f, sigma, CV_64F, "imag");
				filter2D(img, dst_im, ddepth, kernel_im);
				//normalize(dst_im, dst_im, 0, 1, CV_MINMAX);
				//matsGabor.push_back(dst_mag);
				break;
			default://计算全部
				kernel_re = getGaborKer(GaborW, GaborH, U, V, Kmax, f, sigma, CV_64F, "real");
				kernel_im = getGaborKer(GaborW, GaborH, U, V, Kmax, f, sigma, CV_64F, "imag");
				// normalize kernel ??//normalize(kernel_re, kernel_re, 0, 255, CV_MINMAX);  
				// flip kernel// Gabor kernel is symmetric, so do not need flip //flip(kernel_re, kernel_re, -1);  
				filter2D(img, dst_re, ddepth, kernel_re);
				filter2D(img, dst_im, ddepth, kernel_im);
				dst_mag.create(img.rows, img.cols, CV_64FC1);
				cv::magnitude(Mat_<float>(dst_re), Mat_<float>(dst_im), dst_mag);
				//show gabor kernel  
				//cv::normalize(dst_mag, dst_mag, 0, 1, CV_MINMAX);
				//normalize(dst_re, dst_re, 0, 1, CV_MINMAX);
				//normalize(dst_im, dst_im, 0, 1, CV_MINMAX);
				matsGabor.push_back(dst_im);
				break;
			}
			//链接图像，删除
			if (V == VStart){
				colMat = dst_mag;
				colMat_re = dst_re;
				colMat_im = dst_im;
			}
			else{
				switch (type)
				{
				case 0:
					vconcat(colMat_re, dst_re, colMat_re);
					break;
				case 1:
					vconcat(colMat_re, dst_re, colMat_re);
					break;
				default:
					cv::vconcat(colMat, dst_mag, colMat);
					vconcat(colMat_re, dst_re, colMat_re);
					vconcat(colMat_im, dst_im, colMat_im);
					break;
				}
			}
		}
		//for U
		if (U == UStart){
			totalMat = colMat;
			totalMat_re = colMat_re;
			totalMat_im = colMat_im;
		}
		else{
			switch (type)
			{
			case 0:
				hconcat(totalMat_re, colMat_re, totalMat_re);
				break;
			case 1:
				hconcat(totalMat_im, colMat_im, totalMat_im);
				break;
			default:
				cv::hconcat(totalMat, colMat, totalMat);
				hconcat(totalMat_re, colMat_re, totalMat_re);
				hconcat(totalMat_im, colMat_im, totalMat_im);
				break;
			}
		}
	}
	//return  matsGabor.size();
	switch (type){
	case 0:
		return totalMat_re;
	case 1:
		return totalMat_im;
	default:
		return totalMat;
	}
}
```
//获取纹理能量特征
```
//计算特征值：对比度Con、能量Asm、熵Eng、逆差矩Idm、相关性
void ColorML::feature_computer( Mat&src, double& Asm, double& Eng, double& Con, double& Idm )
{
	int height = src.rows;
	int width = src.cols;
	int total = 0;
	for (int i = 0; i < height; i++)	{
		int*srcdata = src.ptr<int>(i);
		for (int j = 0; j < width; j++)	{
			total += srcdata[j];//求图像所有像素的灰度值的和
		}
	}
	Mat copy;
	copy.create(height, width, CV_64FC1);
	for (int i = 0; i < height; i++){
		int*srcdata = src.ptr<int>(i);
		double*copydata = copy.ptr<double>(i);
		for (int j = 0; j < width; j++)	{
			copydata[j] = (double)srcdata[j] / (double)total;//图像每一个像素的的值除以像素总和
		}
	}
	for (int i = 0; i < height; i++){
		double*srcdata = copy.ptr<double>(i);
		for (int j = 0; j < width; j++){
			Asm += srcdata[j] * srcdata[j];//能量
			if (srcdata[j]>0)
				Eng -= srcdata[j] * log(srcdata[j]);//熵             
			Con += (double)(i - j)*(double)(i - j)*srcdata[j];//对比度
			Idm += srcdata[j] / (1 + (double)(i - j)*(double)(i - j));//逆差矩
		}
	}
}//计算特征值：对比度、能量、熵、逆方差、相关性
```
实验结果：
我的实验结果，调节参数uv
![](https://img-blog.csdnimg.cn/20181108180840373.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
他人结果：
![](https://img-blog.csdnimg.cn/20181108181333831.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
简单的纹理分析，以用于CNN的初始化工作，根据Gabor滤波器的参数调节得到的实验结果，来寻找CNN的初始结构和相对靠谱的结构参数。
