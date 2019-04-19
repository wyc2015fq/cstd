# RGB和HSV颜色转换 - 逍遥剑客 - CSDN博客
2008年07月21日 21:50:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：15933
 H参数表示色彩信息，即所处的光谱颜色的位置。该参数用一角度量来表示，红、绿、蓝分别相隔120度。互补色分别相差180度。 
纯度S为一比例值，范围从0到1，它表示成所选颜色的纯度和该颜色最大的纯度之间的比率。S=0时，只有灰度。 
V表示色彩的明亮程度，范围从0到1。有一点要注意：它和光强度之间并没有直接的联系。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20080721/HSV_ColorModel.jpg)
RGB转化到HSV的算法: 
 1: maxmaxRGB 
 2: minminRGB 
 3:  R  max H  GBmaxmin 
 4:  G  max H    BRmaxmin 
 5:  B  max H    RGmaxmin 
 6:  7: H  H   
 8:  H   H  H   
 9: 10: VmaxRGB 
11: Smaxminmax 
HSV转化到RGB的算法 
 1:  s   
 2: RGBV 
 3:  4: H   
 5: i  INTEGERH 
 6:  7: f  H  i 
 8: a  V     s  
 9: b  V     s  f  
10: c  V     s    f   
11: 12: i 
13:   R  V G  c B  a 
14:   R  b G  v B  a 
15:   R  a G  v B  c 
16:   R  a G  b B  v 
17:   R  c G  a B  v 
18:   R  v G  a B  b 
HSV对用户来说是一种直观的颜色模型。我们可以从一种纯色彩开始，即指定色彩角H，并让V=S=1，然后我们可以通过向其中加入黑色和白色来得到我们需要的颜色。增加黑色可以减小V而S不变，同样增加白色可以减小S而V不变。例如，要得到深蓝色，V=0.4 S=1 H=240度。要得到淡蓝色，V=1 S=0.4 H=240度。 
一般说来，人眼最大能区分128种不同的色彩，130种色饱和度，23种明暗度。如果我们用16Bit表示HSV的话，可以用7位存放H，4位存放S，5位存放V，即745或者655就可以满足我们的需要了。 
由于HSV是一种比较直观的颜色模型，所以在许多图像编辑工具中应用比较广泛，如Photoshop（在Photoshop中叫HSB）等等，但这也决定了它不适合使用在光照模型中，许多光线混合运算、光强运算等都无法直接使用HSV来实现。 
顺便提一下，另外一种直观颜色模型是HSL模型，该模型中前两个参数和HSV一样，而L表示亮度。它的三维表示为一双棱锥。            
