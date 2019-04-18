# CNN结构：色彩特征提取-从RGB空间到HSV空间（色彩冷暖判断） - wishchinYang的专栏 - CSDN博客
2017年11月27日 17:34:33[wishchin](https://me.csdn.net/wishchin)阅读数：1479
~~  转自知乎和百度百科：[从零开始学后期](https://zhuanlan.zhihu.com/p/23291805)~~
~~  文章： [冷暖色区分？冷暖肤色适用于那些色系的彩妆](https://www.zhihu.com/question/27623525/answer/118496401)？   ~~
             文章：**干货 |[如何判断人体色冷暖？如何判断色彩冷暖](https://www.zhihu.com/question/22159518/answer/237263882)？（值得收藏研读！） -蒜苗的回答     太强大了，又非常地接地气，赞一个**
**             单独链接：**干货[|如何判断人体色冷暖？如何判断色彩冷暖？](https://zhuanlan.zhihu.com/p/29791327)（值得收藏研读！）****
**冷暖色对比**
       由于色彩的冷暖差别而形成的色彩对比,称为冷暖对比。红、橙、黄使人感觉温暖;蓝、蓝绿、蓝紫使人感觉寒冷;绿与紫介于期间,另外,色彩的冷暖对比还受明度与纯度的影响,白光反射高而感觉冷,黑色吸收率高而感觉暖。
![](https://img-blog.csdn.net/20171127171522456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        在色相环上，红橙黄为暖色，其中**橙 色**称为“暖极”，绿青蓝为冷色，其中**天蓝 色**被称为“冷极”。在色重上，暖色偏重，冷色偏轻。在湿度上，冷色湿润。纯度越高，冷暖感越强。纯度越低，冷暖感也随之降低。
![](https://img-blog.csdn.net/20171127173157098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171123093058489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       一个更详细的环
![](https://img-blog.csdn.net/20171127173330618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171127175936154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**分布特点：**
**色相**： 一般只与色相有关，色相直接定义了颜色的冷暖；
**明度**：[冷色调](https://baike.baidu.com/item/%E5%86%B7%E8%89%B2%E8%B0%83)的亮度越高--越偏暖，暖色调的亮度越高--越偏冷。
**一、色彩篇**
**       所以，色彩冷暖如何判断？**
先纠正误区：
❌所有的红色黄色都是暖色，所有的蓝色紫色都是冷色。
✅**不管是什么色相，都有冷暖之分，即使是蓝色也有偏暖的蓝，即使是红色也有偏冷的红。**
❌色彩的冷暖属性是绝对的
✅**色彩冷暖具有相对性**，同为偏冷的AB，A比B更冷，则B相较于A，偏暖。
另外再补充几点：
1⃣️**  色彩明度变高/变低的过程，色彩冷暖倾向会变得不明显。**
**2⃣️  色彩纯度变高，冷暖倾向变明显。**
进入正题，**教大家一个办法快速判断冷暖。**
      就拿我们平时PPT的调色工具来说，一个圆盘色相，光从色相冷暖上说，蓝色是冷极、黄色是暖极。那换个角度理解，**也就是说色相越靠近蓝色，色相越冷，越靠近黄色，色相越暖。**
     那我们通过这张生动的动图，哈哈，可以看到色相的整个变化过程。（像素有点渣，不过大概就是这个意思哈哈）
![](https://img-blog.csdn.net/20171124093431251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**     eg.**介于蓝绿之间冰蓝色，相较于蓝色而言，是偏暖的，也就是蓝色相中偏暖的蓝。那如果对于绿色相来说，蓝绿就是偏冷的。（别忘了冷暖的相对性。）中间还经历过几个色彩过渡，**eg.**由黄往红过渡的橙粉色 VS 由红紫红过渡的玫瑰粉，前者就更暖一些，后者更冷一些....以此类推。
然后上图：
       同为粉色**，左边偏暖，右边偏**冷
![](https://img-blog.csdn.net/20171124093441563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
同为蓝色相，**则左边偏暖，右边偏冷**。再结合前面补充的1⃣️2⃣️，**则下面冷暖倾向比上面更明显**。
![](https://img-blog.csdn.net/20171127180349211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

蓝色相**，上面偏冷、下面偏暖**。
![](https://img-blog.csdn.net/20171127180540064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171127180548021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
绿色相**，从上至下越来越**暖
![](https://img-blog.csdn.net/20171127180659357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171127180713867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171127180729750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好啦，现在对人体色及色彩冷暖思路应该清晰了吧～～～做个小...总结，应该算得上大总结了，哈哈，来回顾一下！
**大总结**
**     人体色冷暖判断 **
冷暖除了看视觉平面上的和谐之外，还要看气质上是否和谐：适合冷色调的人穿暖色，会让人看上去气质不协调，容易显土气。适合暖色调的人穿冷色，不能很好的表达自身的亲切感，容易让人产生距离感。
冷暖普遍规律
a.肤色
    （暖）微微泛黄、泛红、暖米色，温暖。
   （冷）显白/面无血色、泛粉，清冷、冷艳。
b.瞳孔色/眼神
   （暖）多以棕色调为主
   （冷）呈灰色调、蓝灰色调
c.发色
   （暖）多以咖棕黄调为主
   （冷）多以灰黑篮调为主，冷咖
如何判断人体色冷暖
   a.通过血管判断
   b.通过金银饰品判断
**   c.通过色布判断（划重点）**
**色彩冷暖判断 **
    不管是什么色相，都有冷暖之分，即使是蓝色也有偏暖的蓝，即使是红色也有偏冷的红。
    色彩冷暖具有相对性
    色彩明度变高/变低的过程，色彩冷暖倾向会变得不明显。
    色彩纯度变高，冷暖倾向变明显。
**     利用色相快捷判断冷暖：色相越靠近蓝色，色相越冷，越靠近黄色，色相越暖（划重点**）
**第二个回答：**
**其他话题：怎样配置出冷暖交替的图片？**
答案：叠加颜色、加过渡色。
![](https://img-blog.csdn.net/20171127175141228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
方法：
1.使用原图，设置不透明。
2.添加冷暖色模板，进行一定的透明度覆盖，即可得到冷暖交替的图片。
**参考：**
**wiki百科：[https://en.wikibooks.org/wiki/Color_Models:_RGB,_HSV,_HSL](https://en.wikibooks.org/wiki/Color_Models:_RGB,_HSV,_HSL)**
