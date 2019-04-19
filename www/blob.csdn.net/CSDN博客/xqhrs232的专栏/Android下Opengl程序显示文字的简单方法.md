# Android下Opengl程序显示文字的简单方法 - xqhrs232的专栏 - CSDN博客
2011年03月26日 13:18:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6926
原文地址::[http://xmchang.bokee.com/6922990.html](http://xmchang.bokee.com/6922990.html)
Android 文字绘制到Bitmap上 OpenGL ES中似乎不能输出文本.将文本写到Bitmap上,再作为贴图,则可实现文字输出. 
文字绘制到Bitmap上的方法为: 
String mstrTitle = "文字渲染到Bitmap!"; 
Bitmap bmp = Bitmap.createBitmap(256,256, Bitmap.Config.ARGB_8888); //图象大小要根据文字大小算下,以和文本长度对应 Canvas canvasTemp = new Canvas(bmp);
 canvasTemp.drawColor(Color.WHITE); 
Paint p = new Paint(); 
String familyName ="宋体"; 
Typeface font = Typeface.create(familyName,Typeface.BOLD); 
p.setColor(Color.RED); 
p.setTypeface(font); 
p.setTextSize(22); 
canvasTemp.drawText(mstrTitle,0,100,p); 
本例图是黑底红字我们可以用Bitmap.getPixel,和setPixel或getPixels,setPixels,取得点的颜色,根据是否有点,将Alpha通道清空,获得透明的字.
模拟器上显示中文无问题,不知道手机对中文的支持如何. 
