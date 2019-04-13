
# LTP 命名实体识别标注集 - 追求卓越,做到专业 - CSDN博客


2019年01月08日 11:45:21[Waldenz](https://me.csdn.net/enter89)阅读数：382


## 命名实体识别标注集
NE识别模块的标注结果采用O-S-B-I-E标注形式，其含义为
|标记|含义|
|---|---|
|O|这个词不是NE|
|S|这个词单独构成一个NE|
|B|这个词为一个NE的开始|
|I|这个词为一个NE的中间|
|E|这个词位一个NE的结尾|
LTP中的NE 模块识别三种NE，分别如下：
|标记|含义|
|---|---|
|Nh|人名|
|Ni|机构名|
|Ns|地名|
举例：中国进出口银行与中国银行加强合作
下图中第一行为分词结果，第二行为词性标注，第三行为依存句法，第四行为命名实体识别，最后一行为语义角色标注
![](https://img-blog.csdnimg.cn/20190108110658304.png)
图解
![](https://img-blog.csdnimg.cn/20190108111701675.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)


