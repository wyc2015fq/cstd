# RGBA alpha 透明度混合算法 - xqhrs232的专栏 - CSDN博客
2013年06月07日 21:53:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1069
原文地址::[http://blog.csdn.net/xhhjin/article/details/6445460](http://blog.csdn.net/xhhjin/article/details/6445460)

Alpha 透明度混合算法，网上收集整理，分成以下三种：
一、 R1,G1,B1，Alpha1 为前景颜色值，R2,G2,B2，Alpha2 为背景颜色值，则
        前景色  R = R1 * Alpha1 + R2 * Alpha2 * (1-Alpha1) ；
                   G = G1 * Alpha1 + G2 * Alpha2 * (1-Alpha1) ；
                   B = B1 * Alpha1 + B2 * Alpha2 * (1-Alpha1) ；
        背景色 Alpha = 1 - (1 - Alpha1) * ( 1 - Alpha2) ；
                   R = R / Alpha ；
                   G = G / Alpha ；
                   B = B / Alpha ；
二、半透明算法:
      混合算法目前在常用到的算法是AlphaBlend。
      计算公式如下:假设一幅图象是A，另一幅透明的图象是B，那么透过B去看A，看上去的图象C就是B和A的混合图象，
      设B图象的透明度为alpha(取值为0-1，1为完全透明，0为完全不透明).
      Alpha混合公式如下：
                  R(C)=(1-alpha)*R(B) + alpha*R(A)
                  G(C)=(1-alpha)*G(B) + alpha*G(A)
                  B(C)=(1-alpha)*B(B) + alpha*B(A)
      R(x)、G(x)、B(x)分别指颜色x的RGB分量原色值。从上面的公式可以知道，Alpha其实是一个决定混合透明度的数值。
      改变这个 alpha 值可以得到一个渐变的效果。
      分离RGB色用"位与"、"移位"运算的方法；
      透明的比例按2的N次幂来分级，这样可以快速运算。
      如果是按32级
      Alpha = 1/32
      B图的权重 = (32-n)/32，则A图的权重= n/32，
      可以得到这样的算法：
      R(C)= (32-n) * R(B)+ n* R(A);
     再对R(C)右移5位(除以32)就可以了
    透明的处理：
       假设指定B上的黑色透明色，则碰到B上的颜色为黑色，则不显示黑色，改为显示A上这个位置的颜色。
三、简易Alpha混合算法：首先，要能取得上层与下层颜色的 RGB三基色，
      然后用r,g,b 为最后取得的颜色值；r1,g1,b1是上层的颜色值；r2,g2,b2是下层颜色值
      若Alpha=透明度，则
      当Alpha=50%时，
                    r = r1/2 + r2/2;
                    g = g1/2 + g2/2;
                    b = b1/2 + b2/2;
      当Alpha<50%时，
                    r = r1 - r1/ALPHA + r2/ALPHA;
                    g = g1 - g1/ALPHA + g2/ALPHA;
                    b = b1 - b1/ALPHA + b2/ALPHA;
      当Alpha>50%时，
                    r = r1/ALPHA + r2 - r2/ALPHA;
                    g = g1/ALPHA + g2 - g2/ALPHA;
                    b = b1/ALPHA + b2 - b2/ALPHA;
