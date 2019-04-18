# CNN结构：色彩特征提取-色彩属性HSV空间（色彩冷暖初始） - wishchinYang的专栏 - CSDN博客
2017年11月23日 17:49:13[wishchin](https://me.csdn.net/wishchin)阅读数：1377
         看完颜色的物理和数学描述基础，再来分析颜色的哲学基础，颜色的人文语义属性。颜色的基本三属性为**色相**、**明度**和**纯度**。
         来自于百科：色彩是通过眼、脑和我们的生活经验所产生的一种对光的视觉效应。人对颜色的感觉不仅仅由光的物理性质所决定，比如人类对颜色的感觉往往受到周围颜色的影响。有时人们也将物质产生不同颜色的物理特性直接称为颜色。
        人眼对色彩的感知一般来源于来自于光源的直射色和物体表面的反射色。
        基础理论，查看百度经验：[色彩基础](https://jingyan.baidu.com/article/ab0b5630c585e8c15bfa7d75.html)知识  。下面文章摘抄了一小部分。
        参考：[如何通俗地解释色彩三要素：色相、明度、纯度](https://www.zhihu.com/question/20656077)？        
        HSV(Hue, Saturation, Value)是根据颜色的直观特性由A. R. Smith在1978年创建的一种颜色空间, 也称六角锥体模型(Hexcone Model)。色彩的基本属性离开RGB色彩空间，重新定义了色彩的HSV空间，依然表示 单个色彩的唯一性，但有可能仍然没有包含提夫尼蓝。参考：[色彩的HSV表示](https://baike.baidu.com/item/HSV/547122?fr=aladdin)模型，Hue,
 Saturation, Value。HSV空间和RGB空间是双射映射。
       RGB为生成色，CMYK为反射色，RGB和CMY颜色模型都是面向硬件的，而HSV（Hue Saturation Value）颜色模型是面向用户的。
**色彩基本属性       **
**色相-hue**
色相是指色彩的相貌,是色彩最显著的特征,是不同波长的色彩被感觉的结果。光谱上的红、橙、黄、绿、青、蓝、紫就是七种不同的基本色相。
     色相变化
![](https://img-blog.csdn.net/20171127170938468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     参考：色相环
![](https://img-blog.csdn.net/20171123101042408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171123101058808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171123101130312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171123101154400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**明度-**value****
        明度是指色彩的明暗、深浅程度的差别,它取决于反射光的强弱。它包括两个含义:一是指一种颜色本身的明与暗,二是指不同色相之间存在着明与暗的差别。
        颜色有深浅、明暗的变化。比如，深黄、中黄、淡黄、柠檬黄等黄颜色在明度上就不一样，紫红、深红、玫瑰红、[大红](https://baike.baidu.com/item/%E5%A4%A7%E7%BA%A2)、[朱红](https://baike.baidu.com/item/%E6%9C%B1%E7%BA%A2)、桔红等红颜色在亮度上也不尽相同。这些颜色在明暗、深浅上的不同变化，也就是色彩的又一重要特征一一明度变化。
        色彩的明度变化有许多种情况，一是不同[色相](https://baike.baidu.com/item/%E8%89%B2%E7%9B%B8)之间的明度变化。如：在未调配过得原色黄色明度最高、黄比橙亮、橙比红亮、红比紫亮、紫比黑亮；二是在某种颜色中，加白色明度就会逐渐提高，加黑色明度就会变暗，但同时它们的[纯度](https://baike.baidu.com/item/%E7%BA%AF%E5%BA%A6)(颜色的[饱和度](https://baike.baidu.com/item/%E9%A5%B1%E5%92%8C%E5%BA%A6))就会降低，三是相同的颜色，因光线照射的强弱不同也会产生不同的明暗变化。
![](https://img-blog.csdn.net/20171127170957799?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**纯度-**saturation****
也称彩度、艳度、浓度、饱和度,是指色彩的纯净程度。
![](https://img-blog.csdn.net/20171127171016060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其他色彩名词
**相关关系及影响**
0.三要素环
![](https://img-blog.csdn.net/20171123172709133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
1.** 明度对比类型**
![](https://img-blog.csdn.net/20171123172806813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      为了细致的研究色彩明暗对比，可以把黑、灰、白划分为11个色阶。靠近白的3阶为高调色，靠近黑的三阶为低调色，中间三阶为中调色。
      高调：具有柔软、轻快、纯洁、淡雅之感；
      中调：具有柔和、含蓄、稳重、明确之感；
      低调：具有朴素、浑厚、沉重、压抑之感。
2. **纯度对比类型**.p76
![](https://img-blog.csdn.net/20171123095640342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        低纯度基调1-3：给人以平淡、消极、无力、陈旧的感觉，同时也能给人自然、简朴、柔和、超俗、宁静的感受。
        中纯度基调4-6：能够传达中庸、文雅、安详的感觉。
        高纯度基调7-9：由高纯色组成的基调，由鲜艳、冲动、热烈、活波的视觉感受，给人以积极、强烈而冲动的感觉，如图3-37所示，如运用不当也会产生残暴、恐怖、低俗、刺激的感觉。
![](https://img-blog.csdn.net/20171123174436098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                        图片：高纯度的红色照片，联想一下贞子![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)
**降低纯度的方法**：加白（产生色相偏差）、加黑（失去光亮感，感觉变得沉着冷静）、加灰（变得柔和柔软）。
**3.空间基底映射**
        RGB空间和HSV空间是双射的，转换矩阵如下所示。
![](https://img-blog.csdn.net/20171123171749097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       引用地址：[http://wenku.baidu.com/view/3ccc1ec58bd63186bcebbc0c.html###](https://link.zhihu.com/?target=http%3A//wenku.baidu.com/view/3ccc1ec58bd63186bcebbc0c.html%23%23%23)[](https://link.zhihu.com/?target=http%3A//wenku.baidu.com/view/3ccc1ec58bd63186bcebbc0c.html%23%23%23)
**金属光泽**
        金属光泽是光泽强度的等级之一。一般指反射率R>0.25者，宛如金属抛光后所产生的光泽。同时金属光泽也是矿物光泽的一种。一些硫化物和氧化物矿物。如黄铁矿、方铅矿、镜铁矿等就具有金属光泽。金属光泽矿物均属不透明矿物，很少用作宝石。[1]
        金属光泽是指如同金属[抛光](https://baike.baidu.com/item/%E6%8A%9B%E5%85%89)后的表面所反射的光泽，如同平滑的金属光洁表面所呈现的光泽，反光极强，同[非金属光泽](https://baike.baidu.com/item/%E9%9D%9E%E9%87%91%E5%B1%9E%E5%85%89%E6%B3%BD)、[半金属光泽](https://baike.baidu.com/item/%E5%8D%8A%E9%87%91%E5%B1%9E%E5%85%89%E6%B3%BD)并列。
** 特别之处**：
        金属光泽在RGB空间可以在三通道均匀分布，转化到HSV空间，会产生奇特的特征，表示为金属光泽物体只有V值，其H值和S值均为0.
![](https://img-blog.csdn.net/20171127171346740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**其他属性**
**冷暖对比**
       由于色彩的冷暖差别而形成的色彩对比,称为冷暖对比。红、橙、黄使人感觉温暖;蓝、蓝绿、蓝紫使人感觉寒冷;绿与紫介于期间,另外,色彩的冷暖对比还受明度与纯度的影响,白光反射高而感觉冷,黑色吸收率高而感觉暖。
![](https://img-blog.csdn.net/20171127171522456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        在色相环上，红橙黄为暖色，其中橙色称为“暖极”，绿青蓝为冷色，其中天蓝色被称为“冷极”。在色重上，暖色偏重，冷色偏轻。在湿度上，冷色湿润。纯度越高，冷暖感越强。纯度越低，冷暖感也随之降低。
![](https://img-blog.csdn.net/20171123093058489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**补色对比**
将红与绿、黄与紫、蓝与橙等具有补色关系的色彩彼此并置,使色彩感觉更为鲜明,纯度增加,称为补色对比。
![](https://img-blog.csdn.net/20171127171557622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**色调**
        色调指得是一幅画中画面色彩的总体倾向,是大的色彩效果。在大自然中,我们经常见到这样一种现象:不同颜色的物体或被笼罩在一片金色的阳光之中,或被笼罩在一片轻纱薄雾似的、淡蓝色的月色之中;或被秋天迷人的黄金色所笼罩;或被统一在冬季银白色的世界之中。这种在不同颜色的物体上,笼罩着某一种色彩,使不同颜色的物体都带有同一色彩倾向,这样的色彩现象就是色调。色调是画面色彩构成的总体效果。
**参考：《配色设计》**
![](https://img-blog.csdn.net/20171123174710830?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171123174723284?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171123174733600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
