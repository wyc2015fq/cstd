# graphviz画图之张柏芝前夫是谢霆锋 - 囧囧有神的专栏 - CSDN博客





2014年01月03日 18:05:14[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：3163








张柏芝前夫是谢霆锋，谢霆锋前女友是王菲，王菲老公是李亚鹏，李亚鹏前女友叫瞿颖，瞿颖现男友是张亚东，朴树有个前女友叫周迅，周迅有个前男友叫李亚鹏，王菲前夫是窦唯，窦唯有个堂弟叫窦鹏，窦鹏有个前女友叫周迅，窦鹏堂姐是窦颖，窦颖前夫是张亚东，张亚东现女友是瞿颖，瞿颖前男友叫李亚鹏

请问张柏芝和李亚鹏什么关系？（本题目摘自2012年国家公务员考试--申论部分）
![](https://img-blog.csdn.net/20140103180259078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**一图胜千言啊**

dot源码如下：

```java
digraphg{edge[fontname="FangSong"];

node [fontname="FangSong" size="20,20"];

{

zhangbozhi [label="张柏芝"];

xietingfeng [label="谢霆锋"];

wangfei [label="王菲"];

liyapeng [label="李亚鹏"];

quying [label="瞿颖"];

zhangyadong [label="张亚东"];

pushu [label="朴树"];

zhouxun [label="周迅"];

douwei [label="窦唯"];

doupeng [label="窦鹏"];

douying [label="窦颖"];

}

zhangbozhi ->xietingfeng [label="前夫"];

xietingfeng ->wangfei [label="前女友"];

wangfei->liyapeng [label="老公"];

liyapeng->quying [label="前女友"];

quying->zhangyadong [label="现男友"];

pushu->zhouxun [label="前女友"];

zhouxun->liyapeng [label="前男友"];

wangfei->douwei [label="前夫"];

douwei->doupeng [label="堂弟"];

doupeng->zhouxun [label="前女友"] ;

doupeng->douying [label="堂姐"];

douying->zhangyadong [label="前夫"];

zhangyadong->quying [label="现女友"];

quying->liyapeng [label="前男友"];

}
```





安装 sudo apt-get install graphviz  命令即可


更详细使用说明请百度～




