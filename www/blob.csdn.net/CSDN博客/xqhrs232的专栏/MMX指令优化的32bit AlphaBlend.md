# MMX指令优化的32bit AlphaBlend - xqhrs232的专栏 - CSDN博客
2012年11月06日 22:07:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1095
原文地址::[http://bbs.sjtu.edu.cn/bbscon?board=GameDesign&file=M.1168164234.A](http://bbs.sjtu.edu.cn/bbscon?board=GameDesign&file=M.1168164234.A)
前一段时间再进行一个目前保密的项目：Prototype A （哈哈，知道的人不要说阿）
其中需要较高的运行效率，所以就写了一个将32bit位图渲染到目标32bit位图的AlphaBle
nd。支持Alpha通道。
代码指令参照了[http://dev.gameres.com/Program/Visual/2D/mmxaddalpha.htm](http://dev.gameres.com/Program/Visual/2D/mmxaddalpha.htm)提供的代
码，在此表示感谢。由于第一次写汇编优化。所以不知道这样写是不是最高效的。
如果对位深没有要求，可以采用intel提供的16bit的alphablend,网上很多了，这个号称是
目前最快的.
参数：
pDest：目标渲染buffer,32bit的，通道情况：ARGB
wlined：pDest的扫描线宽（即横向的像素个数，即实际宽度/4）
hlined：pDest的扫描线行数，可以理解成实际高度
startX：pDest的开始坐标X
startY：pDest的开始坐标Y，采用倒置坐标系
pSrc：要渲染得图片，通道情况：ARGB
wlines，hlines和前面类似
Alpha：对pSrc做得整体alphaBlend，0-255级
返回：
如果图片得到渲染，返回TRUE
用在Prototype A中效果还好，采用GDI渲染一个100*100的图片可以有1000fps*
代码：
BOOL AlphaBlt(BYTE *pDest,DWORD wlined,DWORD hlined,int startX,int startY,BYTE
 *pSrc,DWORD wlines,DWORD hlines,DWORD Alpha) 
{ 
 int Xd,Yd,Xs=0,Ys=0; 
 int loopH,loopW; 
 if (startX>=(int)wlined) return FALSE; 
 if (startY>=(int)hlined) return FALSE; 
  
 if (startX + (int)wlines <=0) return FALSE; 
 if (startY + (int)hlines <=0) return FALSE; 
  
 Xd = startX; 
 Yd = startY; 
 loopW = (startX + wlines); 
 loopH = (startY + hlines); 
 if (loopH>hlined) loopH = hlined; 
 if (loopW>wlined) loopW = wlined; 
 if (startX<0) 
 { 
  Xd = 0; 
  Xs = -startX; 
 } 
 if (startY<0) 
 { 
  Yd = 0; 
  Ys = -startY;  
 } 
  
 DWORD factorA = hlines-Ys+Yd-1; 
 for (DWORD j= Yd ; j< loopH ; j++) 
 { 
  DWORD dwOffSrc,dwOffDest; 
  dwOffSrc = (factorA-j)*wlines; 
   
  dwOffDest = (hlined-j-1)*wlined; 
  DWORD srcPosX = Xs; 
  for (DWORD i = Xd ; i< loopW ; i++) 
  { 
   
   BYTE *BufSrc = pSrc + (dwOffSrc + srcPosX)*4; 
   DWORD dwSrc =((DWORD)BufSrc[3] << 24) |((DWORD)BufSrc[2] << 16) | ((WORD)Bu
fSrc[1] << 8) | (BufSrc[0]); 
   DWORD *dwpDest = (DWORD *)pDest + dwOffDest + i; 
    __asm{ 
     pxor mm2,mm2  
     mov edx,dwpDest 
     movd mm0,[edx] 
     movd mm1,dwSrc 
     punpcklbw mm0,mm2  
     punpcklbw mm1,mm2  
     movq mm3,mm1 
     punpckhwd mm3,mm3  
     punpckhdq mm3,mm3  
     movd mm4,Alpha  
     punpcklwd mm4,mm4 
     punpckldq mm4,mm4  
     pmullw mm3,mm4 //Alpha * SrcAlpha 
     psrlw mm3,8 
     movq mm4,mm0  
     movq mm5,mm1  
     psubusw mm4,mm1  
     psubusw mm5,mm0  
     pmullw mm4,mm3  
     pmullw mm5,mm3  
     psrlw mm4,8  
     psrlw mm5,8 
     paddusw mm0,mm5  
     psubusw mm0,mm4 
     packuswb mm0,mm0 
     movd [edx],mm0 //保存结果 
     emms  
   }  
   srcPosX++; 
  } 
 } 
 return TRUE; 
}
--
[http://www.csksoft.net/heaven200.jpg](http://www.csksoft.net/heaven200.jpg)
欢迎访问我的网站以及BLOG:
[http://blog.csksoft.net](http://blog.csksoft.net/)[http://www.csksoft.net](http://www.csksoft.net/)
{AI,flash,Crack,Hack,Demo sence,etc.}
