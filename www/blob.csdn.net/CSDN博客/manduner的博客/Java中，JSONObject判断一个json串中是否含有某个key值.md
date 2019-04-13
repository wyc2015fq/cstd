
# Java中，JSONObject判断一个json串中是否含有某个key值 - manduner的博客 - CSDN博客


2018年03月20日 09:50:16[Manduner_TJU](https://me.csdn.net/manduner)阅读数：20882


调用has()方法
举例如下：
```python
JSONObject obj = JSONObject.fromObject(str); //将字符串数据转化成JSON对象
String text1 = "";
if (obj.has("retweeted_status")) {  //判断JSONObject是否包含含有retweeted_status属性值
       
    text1 = "这样解释，大家看懂了吗0.0"
}
```


