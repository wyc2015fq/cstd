# CImage 对话框初始化时候显示透明 PNG - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月23日 21:44:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：29









使用CImage的时候,发现显示出来的并不是透明背景的图片，而是白色背景的图片。

后发现原因如下：

> 
PNG图片的透明背景总是一片白色，后来才发现这其实是微软GDI+的设计问题，PNG图片是ARGB，使用GDI+载入图片的时候，GDI+会默认已经进行了预剩运算（PARGB），即每象素的实际值是已经和ALPHA值按比例相乘的结果，实际上它根本就没有做预乘，在使用透明图片的象素ALPHA通道的时候，CImage内部正是调用的AlphaBlend，没有预乘的图当作预乘的图片处理的结果就是这相当于一张和纯白背景进行了预剩，所以图象总是出现白色背景。


解决方法如下：

添加一个转换函数。

如下：



```cpp
CImage img;  
HRESULT result = img.Load(_T("icon.png"));     
for(int i = 0; i < img.GetWidth(); i++)
{
    for(int j = 0; j < img.GetHeight(); j++)
    {
        unsigned char* pucColor = reinterpret_cast<unsigned char *>(img.GetPixelAddress(i , j));
        pucColor[0] = pucColor[0] * pucColor[3] / 255;
        pucColor[1] = pucColor[1] * pucColor[3] / 255;
        pucColor[2] = pucColor[2] * pucColor[3] / 255;
    }
}
```





在这里贴上自己项目里面的代码。我的工程要求对话框启动的时候自动加载透明PNG图。开始的时候把加载函数添加到OnInitDialog()里面，但发现没有任何效果，网上查了半天原因，发现画图的函数要添加到OnPaint()里面，修改之后，就生效了。

启动自动加载透明PNG图片的代码（位于OnPaint() 中，待画图的控件是一个ID为IDC_V_H264_LOGO 的 Picture Control）：



```cpp
//画对话框不应该添加到OnInitDialog中，否则没有效果
	//应该添加到OnPaint中
	CRect rect;
	GetDlgItem(IDC_V_H264_LOGO)->GetClientRect(&rect);
	if(image.IsNull()==true)
	image.Load(".\\res\\264_ico.png");
	//转换函数？加上后背景为白色的PNG就变成了透明的------------
	//PNG图像自带Alpha通道，存储方式RGBA，代表了透明度，即pucColor[3]
	//当Alpha通道为0时，把RGB值都变成0
	for(int i = 0; i < image.GetWidth(); i++)
	{
		for(int j = 0; j < image.GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(image.GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
	//画一画
	image.Draw(GetDlgItem(IDC_V_H264_LOGO)->GetDC()->m_hDC,rect);
```


 结果如下图所示：



![](https://img-blog.csdn.net/20131023214357859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






