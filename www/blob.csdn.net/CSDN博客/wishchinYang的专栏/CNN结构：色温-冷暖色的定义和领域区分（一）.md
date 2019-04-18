# CNN结构：色温-冷暖色的定义和领域区分（一） - wishchinYang的专栏 - CSDN博客
2017年11月22日 15:06:22[wishchin](https://me.csdn.net/wishchin)阅读数：3301
             转自知乎和百度百科：[从零开始学后期](https://zhuanlan.zhihu.com/p/23291805)  （色温的奥秘）       
             文章： [冷暖色区分？冷暖肤色适用于那些色系的彩妆](https://www.zhihu.com/question/27623525/answer/118496401)？          
             文章：**干货 |[如何判断人体色冷暖？如何判断色彩冷暖](https://www.zhihu.com/question/22159518/answer/237263882)？（值得收藏研读！） -蒜苗的回答            **
**百科定义**：
        色温是表示光线中包含颜色成分的一个[计量单位](https://baike.baidu.com/item/%E8%AE%A1%E9%87%8F%E5%8D%95%E4%BD%8D)。从理论上讲，色温是指绝对[黑体](https://baike.baidu.com/item/%E9%BB%91%E4%BD%93/5398327)从绝对零度(一273℃)开始加温后所呈现的颜色。黑体在受热后．逐渐由黑变红，转黄，发白，最后发出蓝色光。当加热到一定的温度．黑体发出的光所含的光谱成分．就称为这一温度下的色温，计量单位为“K”(开尔文)、如果某一光源发出的光，与某一温度下黑体发出的光所含的[光谱](https://baike.baidu.com/item/%E5%85%89%E8%B0%B1/757474)成分相同．即称为某K色温、如100 W灯泡发出光的颜色，与绝对黑体在2527K时的颜色相同，那么这只灯泡发出的光的色温就是：2527K+ 273K=2800K[1]
     （请大家联想一下打铁的画面）。就像加热铁块时，铁块先变成红色，然后是黄色，最后会变成白色。[2]使用这种方法标定的色温与普通大众所认为的 “暖”和“冷 ”正好相反。     
![](https://img-blog.csdn.net/20171122094056793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**色温量化**：
         色温的单位是开尔文。并可以在颜色对造表上查的，可以通过**分光计算**出来。检测工具为**色温计**。黑体辐射体的色温等于它表面的开尔文温度。
         色温（colo(u)r temperature）是[可见光](https://baike.baidu.com/item/%E5%8F%AF%E8%A7%81%E5%85%89)在摄影、录像、出版等领域具有重要应用的特征。光源的色温是通过对比它的[色彩](https://baike.baidu.com/item/%E8%89%B2%E5%BD%A9)和理论的热黑体辐射体来确定的。热黑体辐射体与光源的色彩相匹配时的[开尔文](https://baike.baidu.com/item/%E5%BC%80%E5%B0%94%E6%96%87)温度就是那个光源的色温，它直接和[普朗克黑体辐射定律](https://baike.baidu.com/item/%E6%99%AE%E6%9C%97%E5%85%8B%E9%BB%91%E4%BD%93%E8%BE%90%E5%B0%84%E5%AE%9A%E5%BE%8B)相联系。
**相关色温**：
        通过比较一个光源发射出的光色和某一温度下的黑体（如铂）辐射的光色相一致时，变把此时黑体的温度表示为光源的颜色温度（即色温）。
        做法的前提是光源的光谱分布与黑体轨迹比较接近。但实际上，绝大多数照明光源的光色并不能恰好在黑体辐射线上，于是Raymond Davis等人提出了相关色温的概念，其核心思想是在均匀**色品图**上用距离最短的温度来表示光源的相关色温，用K氏温度表示。
![](https://img-blog.csdn.net/20171122094632426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
某些光源，它发射光的颜色与黑体在各种温度下所发射的光颜色都不完全相同。所以在这种情况下用“相关色温”的概念。光源所发射的光的颜色与黑体在某一温度下发射的光的颜色**最接近**时，黑体的温度就称为该光源的相关色温。
 光源色温不同，光色也不同，带来的感觉也不相同：
|<3000K|温暖（带红的白色）|稳重、温暖|
|----|----|----|
|3000－5000K|中间（白色）|爽快|
|>5000K|清凉型（带蓝的白色）|冷|
**色温光谱**：
不同的光源具有不同的色温，例如：
> 
1700 K：火柴光
1850 K：蜡烛
2800 K：钨灯（白炽灯）的常见色温
3000 k：卤素灯及黄光日光灯的常见色温
3350 K：演播室“CP”灯
3400 K：演播室台灯,、照相泛光灯（不是闪光灯）等...
4100 K：月光、浅黄光日光灯
5000 K：日光
5500 K：平均日光、电子闪光（因厂商而异）
5770 K：有效太阳温度
6420 K：氙弧灯
6500 K：最常见的白光日光灯色温
当色温越高的时候，光源发出的颜色就越偏冷，大致是经历一个红——橙红——黄——黄白——白——蓝白的渐变过程。
![](https://img-blog.csdn.net/20171122100515909?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171122113711327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122113727832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122130939664?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122113737349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122113901403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       图2 分析：从热光源到冷光源   烛火——白炽灯 ——太阳光 —— LED灯——日光灯       
        我们一天中的色温也是在不停发生变化的，例如日出之前的色温是蓝色，日出之后的色温是橙色，中午的色温是白色，而晚上的色温是黄色等等，引起太阳光颜色变化的原因主要是因为**大气层的反射作用**，而并不是因为太阳的温度发生了大幅度的改变。
**色温与亮度**：
        高色温光源照射下，如亮度不高则给人们有一种阴冷的气氛；低色温光源照射下，亮度过高会给人们有一种闷热感觉。光色的对比：在同一空间使用两种光色差很大的光源，其对比将会出现层次效果，光色对比大时，在获得亮度层次的同时，又可获得光色的层次。
        这涉及到光源的热转换效率问题。
色温与光效：
        初中物理学学到过，热传递有三种方式：传导（不变式接触传导），对流（变介式接触传导），辐射（非接触电磁波传递）。一般来说，对于非真空中的热源能量交互，辐射所占能量比例与色温有正相关的关系。
        非真空中不同色温的热体辐射产生光的效率是不同的。以明显的热光源白炽灯，所谓的电灯泡为例。发光温度与发光效率成单调增长的关系。
**感觉色温**：
       地球上不同人种、不同种族的正常人，对颜色的感受是一样的，不要以为地区或者虹膜颜色不同。看到的颜色就会有差异。
**反射光**：自从热源作为光源开始，人类已经经历了几十瓦年的的发展，对光源温度的感受不仅来自于光，更多来源于介质温度传递和环境反射。从直觉上让人感知冷暖知觉的不是少人接触的“热光源”，而是无处不在的“反射光源”。
![](https://img-blog.csdn.net/20171122105118659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104112273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122105245397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171122105130560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104630933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104638986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171122105611155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104138531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104133015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     图片分析：从左到右，从上到下。色温逐渐升高，而感觉温度却随之降低。
     依次为：丹霞山——非洲草原——塔克拉玛干沙漠   甘肃月牙泉—黄河九曲 ——堰塞湖     纯净水 ——海洋 ——极地
**低热光源**：对太阳的感知是地球整个生命系统的共性，影响了地球生存系统的变更，每个物种以及个体的生死存亡。对于人类来说，自身掌握热源只有几十万年的历史，且工业革命之前，人类所能掌握的，只有热光源，且也只有低温热源——火，。
        而火作为人类直接感受的热源，作为近距离感知最热的发光体，发出的光芒，最高也是低色温。进化千万年的人类对高色温热源几乎无任何直观感觉。而高色温热源的光线更倾向于被低温物体反射，这就导致了人类感知光线冷暖与物理色温的反相关结果。
![](https://img-blog.csdn.net/20171122114041783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122114054875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122114239892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122114322773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122114332138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       图片分析：从左到右依次为热光源   烛火——油火把 ——木柴 —— 炭火—— 煤火
**水源**：作为反射的水源承载这对人类色温感知的重要角色。温度高的地方，水的液态和固态形式更少，环境反射更趋向于低色温。
![](https://img-blog.csdn.net/20171122105118659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104630933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171122104138531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**冷暖色**（色调）：
         根据图2 中的例子，从直觉上可以感受高色温的光在实际生存场景中一般以冷光源的方式产生。而低色温的光芒一般为热光源发出。依托色温，得到颜色的另外一个度量衡，色调。
         色调的评价是主观的吗？是否对色调冷暖的评价仍然有一个通用统一的量化标准呢？
参考：
1. 色温
2.划重点： [为什么4k的色温亮度 最高](https://www.cnledw.com/inter/show-3-100202.htm) https://www.cnledw.com/inter/show-3-100202.htm
3.实验：辐射效率和温度的关系
4.热辐射：https://baike.baidu.com/item/%E7%83%AD%E8%BE%90%E5%B0%84/120950?fr=aladdin
5. .............................
6. 豆瓣：[色温与亮度](https://www.douban.com/note/536547891/)  https://www.douban.com/note/536547891/
  色温         光色温 光效果　
3000K　黄色光，强穿透力
4200K　白中带黄，原车配氙气灯
5000K　光全白，欧规最高色温
6000K　光全白，略带蓝色
6500K　阳光下的白天
7000—8000K白中明显带蓝
8000K以上蓝光，穿透力极差推荐6000K左右的色温正好是最白略微开始转蓝的色温，人眼容易接受，不易疲劳，提高安全性，而卤素灯则较差。
