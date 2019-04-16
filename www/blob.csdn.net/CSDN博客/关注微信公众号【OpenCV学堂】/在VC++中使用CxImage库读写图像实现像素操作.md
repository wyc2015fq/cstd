# 在VC++中使用CxImage库读写图像实现像素操作 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年07月14日 11:51:28[gloomyfish](https://me.csdn.net/jia20003)阅读数：4682







最近做了一个项目最后要把算法整成C++的DLL方式给别人调用，朋友给我推荐了用CxImage这个库来读写图像文件，所以我就用了，基于CxImage做了调用算法的Demo程序，已经给别人测试了，现在总结一下CxImage使用中遇到的那些坑。

**坑一：**
我是VS2015上面用CxImage这个库的，而且是别人已经编译好的LIB与头文件直接给我了，他还是VC6.0上面搞出来的，所以我一使用这个库遇到第一个问题就是在Link的时候给我报缺少DLL文件mfc42d.dll和msvcrtd.dll这两个DLL文件找不到，我的是VS2015当然没有这个两个东西，还好CSDN博客专家群里面有个哥们很好，传给我了，帮我解决了这个问题。后来我找到CxImage上code project上面的文章，发现它果然是用VC6.0编译的。http://www.codeproject.com/Articles/1300/CxImage

**坑二：**
然后我就开始使用这个库了，只要加上如下的头文件：
#include "ximage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib,"cximaged.lib")

发现我编译怎么也不通过，后来我才发现原因了，我是好久不搞C++了，设置Link的路径不对。可见我已经把我知道的东西全部还给我的大学老师了。解决这个问题就可以使用了，CxImage可以读写几何所有常见格式图像文件，我主要用它来读写JPG和PNG文件，这样就可以省去我很多力气。读JPG文件的代码如下：



```cpp
AfxMessageBox("从这里打开图像文件...");
CString strFile = _T("");
CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"), NULL);
if (dlgFile.DoModal())
{
	strFile = dlgFile.GetPathName();
}
AfxMessageBox(strFile);
bool blLoad = this->image.Load(strFile, CXIMAGE_FORMAT_JPG);
if (this->image.IsValid()) {
	AfxMessageBox("文件装载成功...");
	this->Invalidate();
}
```

**坑三：**
也是因为我好久不搞C++了，我有个统计直方图的来了个数组定义，然后就直接开始统计直方图了，其实C++中数组一定要初始化，不然它的初始值真的不是0啊，我把它当成Java来搞了，发现统计出来的直方图数据居然有负值，经过这个血泪教育我知道VC++一定要初始化数组，不然结果很吓人。

**坑四：**
然后我就读像素了，CxImage中读图像像素数据的我找了好久发现只有一个GetBits()官方的API上面说这个可以获得像素数组的指针，指向第0个像素地址，可是API注释上面还有一句英文说这个API非常危险，要小心使用。还好我胆子比较大，不管它就直接用了，发现这个里面有个大坑，就是它的像素指针的第一个是我们倒是第一行，我们要读一幅图像从上到下，从左到右顺序的话，在图像高度的方向就要从height-1读到0，而不是从0读到height-1.最终我实现了利用CxImage读取图像像素数据，实现图像灰度化，同时还画了两条红色的线。

代码如下：



```cpp
if (this->image.IsValid()) {
	AfxMessageBox("图像灰度化...");

	int maxY = image.GetHeight(), 
	int maxX = image.GetWidth();
	int size = maxX*maxY;
	BYTE* bs = image.GetBits();
	int w=((((24 * image.GetWidth()) + 31) / 32) * 4); // 4字节对齐
	for (int j=0; j<maxY; j++) {
		for (int i=0; i<maxX; i++) {
			BYTE *b = bs+(image.GetHeight()-1-j)*w +i*3;
			int blue =*(b);
			int green =*(b+1);
			int red =*(b+2);
			int grayVal=(int)red*0.3+(int)green*0.59+(int)blue*0.11;
			CString csStr;
			csStr.Format("%d", grayVal);
			if(i >10 && i<100) {
				if(j == 50 || j== 100) {
					*b = 0; //blue
					*(b+1) = 0; // green
					*(b+2) = 255; // red
				}
				else {
					*b = grayVal;
					*(b+1) = grayVal;
					*(b+2) = grayVal;
				}
			}
			else {
				*b = grayVal;
				*(b+1) = grayVal;
				*(b+2) = grayVal;
			}
		}
	}
	this->Invalidate();
}
```
显示图像在View中，VC++中显示图像一定要放到OnDraw方法中


不然就得自己做各种窗口事件响应处理实现重绘制。代码如下：



```cpp
// TODO: 在此处为本机数据添加绘制代码
if (this->image.IsValid()) {
	int h = this->image.GetHeight(),	// + 256
	int w = this->image.GetWidth();

	CRect rc;
	GetClientRect(&rc);
	rc.right = rc.left +w;
	rc.bottom = rc.top + h;
	this->image.Draw(pDC->GetSafeHdc(), rc);
}
```
这样就搞定了CxImage读写图像文件的全部内容，需要说明一下


我这里的图像都是RGB三通道的，在读取像素之前，最好检查一

下它是几通道的图像。载入一张图片：

![](https://img-blog.csdn.net/20160714114833366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


利用CxImage读取图像实现灰度图像效果：

![](https://img-blog.csdn.net/20160714114917804)


发现这样我就可以把精力放到图像处理算法本身了，用VC++搞图像

处理的算法也是可以的，没想象中那么恶搞！



