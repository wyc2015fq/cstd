
# QQ头像变灰算法[图] - .NET博文收藏 - CSDN博客


2008年06月10日 12:48:00[hejishan](https://me.csdn.net/hejishan)阅读数：1158标签：[qq																](https://so.csdn.net/so/search/s.do?q=qq&t=blog)[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[工具																](https://so.csdn.net/so/search/s.do?q=工具&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=string&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=qq&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=qq&t=blog)

1.查看QQ不在线图标,目测由白灰黑,三种颜色.
2.打开画图工具，查看了灰色的RGB......惊喜.......原来灰色的RGB 3个参数多是相同的.
3.所以推断每个不在线头像的像素是在线头像R G B 的平均值.
网友"string"给了个比较专业的公式(R*0.299 + G*0.587 + B*0.114 )
4.操作:取得一个像数,然后取得R ,G ,B,计算平均,然后将平均值设置回像数.
5.以下是代码:
COLORREF cr = ::GetPixel(hDC,i,j);
int nR = GetRValue(cr);
int nG = GetGValue(cr);
int nB = GetBValue(cr);
//int nPer= (nR+nG+nB)/3;  //这两种方法，肉眼确实很难看的出差别
int nPer = nR*0.299 + nG*0.587 + nB*0.114
cr = RGB(nPer,nPer,nPer);
::SetPixel(hDC,i,j,cr);
6.效果图
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xuweiqun/touxiang.jpg)



