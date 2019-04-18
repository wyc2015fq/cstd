# VC++笔记七 - L_Andy的专栏 - CSDN博客

2013年07月16日 13:28:26[卡哥](https://me.csdn.net/L_Andy)阅读数：1101


张明奇(卡哥)

动态链接库修改文件名问题？

eg：GetRadarFS.dll->RadarModel.dll;

注意修改：打开文件GetRadarFs.def

修改 LIBRARY  "GetRadarFs" -> LIBRARY "RadarModel"

这样在test应用程序中添加RadarModel.dll和RadarModel.lib文件就不会有问题了。

![](https://img-blog.csdn.net/20130716132624218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130716132646984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

/////////////////////////////////////////////////////////////////////////

Radio控件使用技巧？

Radio控件互斥。Ctrl+D，查看控件的顺序，使Radio控件的顺序升序排列。eg：

1,2,3,4这样Radio控件就互斥了。

////////////////////////////////////////////////////////////////////////////

前一个对话框中定义的变量和函数如何在后来的对话框类中使用？？

我用MFC建立了一个基于对话框的程序，并在该类定义了一些成员变量和成员函数。 

我在该对话框是放上一个按钮，想要单击此按钮后重新生成一个新的对话框，并根据原对话框中定义的一些变量和函数在新的对话框中画出图形。 

不知在新的对话框类中如何使用前一个对话框类中的变量和函数？ 

谢谢！

------解决方案--------------------------------------------------------

把dialog的this指针传进去 

------解决方案--------------------------------------------------------

CWnd::GetMainWnd() 可以返回主对话框的指针，通过这个指针访问你的数据就可以了！ 

------解决方案--------------------------------------------------------

在新对话框中把基本对话框的头文件加入进去。 

然后，定义基本对话框的变量。直接赋值就可以得到 那些值 

------解决方案--------------------------------------------------------

你可以把主对话框的实例指针传给子对话框，在子对话框中通过这个指针访问。

----------------------------------------------------------------------

CThresholdSetDlg dlg(this);

//CThresholdSetDlg::CThresholdSetDlg(CWnd* pParent /*=NULL*/)

//: CDialog(CThresholdSetDlg::IDD, pParent)

//, m_Radio_Threshold(0)

//{

//

//}

该为如下

CThresholdSetDlg::CThresholdSetDlg(CCoverageParaDlg *pcoverCalPara)

:CDialog(CThresholdSetDlg::IDD,pcoverCalPara)
, m_Radio_Threshold(0)

{
m_CCoverParaDlg = pcoverCalPara;

}

////////////////////////////////////////////////////////////////////////////

求救！！！！！怎样让CFileDialog打开指定的文件夹?

CFileDialog的构造函数中将参数lpszFilter设定为你需要的路径即可。

或者这样也行：

CFileDialog dlg(TRUE);

dlg.m_ofn.lpstrFilter = _T("C:\\");  // 输入你的Path

设置初始路径 dlg.m_ofn.lpstrInitialDir

注意2000与NT、98的搜索方式不太一样，在2000下，如果m_ofn.lpstrFile设置了且包含一个路径，

则取该路径，然后再取m_ofn.lpstrInitialDir的值；而其他则首先取m_ofn.lpstrInitialDir的值。

CFileDialog dlg(TRUE,NULL,"C:\\TEST",OFN_HIDEREADONLY ¦ OFN_OVERWRITEPROMPT,"*.txt",NULL);

dlg.DoModal();

设置的初始值为"c:\test",运行后目录显示为c:\，缺省文件为test文件。 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

新建完项目，软件时的布局：

如图：![](https://img-blog.csdn.net/20131010164019765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

怎么实现：

第一种是手动修改：

用记事本打开,test.sln,然后手动修改一下路径：

如 "test.vcproj"->"test\test.vcproj"就可以了。

第二种是编译器修改：

重新打开编辑器：

文件-打开-文件；

如图![](https://img-blog.csdn.net/20131010164114843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

将所有文件整理到 新建一个文件件test中。

如图![](https://img-blog.csdn.net/20131010164122640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后删去test.sln

文件-打开-项目，打开test.vcproj,运行找一下路径就可以了。

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

# 时间复杂度

时间复杂度简单的理解就是执行语句的条数。如果有循环和递归，则忽略简单语句，直接算循环和递归的语句执行次数。

比如：

```java
int x = 1;//时间复杂度为O(1)
		for(int i=0; i<n; i++) {
			System.out.println(i);
		}//时间复杂度为O(n)
```

具体例子： 

1、O(1)

```java
int x = 1;
```

2、O(n) 

```java
for(int i=0; i<n; i++) {
			System.out.println(i);
		}
```

3、O(![](https://img-blog.csdn.net/20131021013937406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWlhbnR1amF2YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast))


```
int count = 0;;
		for(int i=1; i<=n; i *= 2) {
			count++;
		}
```


4、![](https://img-blog.csdn.net/20131021014450890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWlhbnR1amF2YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
int n = 8, count = 0;;
		for(int i=1; i<=n; i++) {
			for(int j=1; j<=n; j++) {
				count++;
			}
		}
```

5、![](https://img-blog.csdn.net/20131021014834515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWlhbnR1amF2YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```java
int n = 8, count = 0;;
		for(int i=1; i<=n; i *= 2) {
			for(int j=1; j<=n; j++) {
				count++;
			}
		}
```

所举例子都比较简单。 

# 空间复杂度

空间复杂度也很简单的理解为临时变量占用的存储空间。一个简单例子：

```java
//交换两个变量x和y
		int x=1, y=2;
		int temp = x;
		x = y;
		y = temp;
```

一个临时变量temp，所以空间复杂度为O(1)。

//////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );

BOOL Arc( LPCRECT lpRect, POINT ptStart, POINT ptEnd );

x1为有效矩形左上角X坐标点.y1为有效矩形左上角y坐标点.x2为有效矩形右下角x坐标点.y2为有效矩形右下角y坐标点.

x3为画圆弧起始x坐标点.y3为画圆弧起始y坐标点.x4为画圆弧终点x点,y4为画圆弧终点y点.

x1,y1,x2,y2为画圆弧矩形区域.x3,y3,x4,y4为画圆弧起始点,终点. Arc 画圆弧是按逆时针方向.

LPCRECT lpRect 为矩形区域. POINT ptStart 指坐标系上一点.即圆弧起始点.POINT ptEnd为终点.

如题:两圆弧组成一个圆

例1.

int cx = 192, cy = 192; //圆心

int p = 64; //半径

dc.Arc(cx-p,cy-p,cy+p,cy+p,cx-p,cy,cx+p,cy);//下半圆弧.

dc.Arc(cx-p,cy-p,cy+p,cy+p,cx+p,cy,cx-p,cy);//下半圆弧

例2.

dc.Arc(CRect(128,128,256,256),CPoint(128,192),CPoint(256,192));

dc.Arc(CRect(128,128,256,256),CPoint(256,192),CPoint(128,192));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

