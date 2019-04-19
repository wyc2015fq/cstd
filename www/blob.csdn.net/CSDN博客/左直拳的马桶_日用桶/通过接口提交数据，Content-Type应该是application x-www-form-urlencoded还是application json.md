# 通过接口提交数据，Content-Type应该是application/x-www-form-urlencoded还是application/json? - 左直拳的马桶_日用桶 - CSDN博客
2018年01月13日 19:56:36[左直拳](https://me.csdn.net/leftfist)阅读数：7711
通过POST方式提交数据到接口的时候，常常会遇到这种句子：
`webClient.Headers.Add("Content-Type", "application/json");`
或者
`webClient.Headers.Add("Content-Type", "application/x-www-form-urlencoded");`
那究竟应该用哪种方式？
答案是跟我们提交的参数格式有关。
如果是通过页面表单方式提交，那就是`"application/x-www-form-urlencoded"`；如果是json（要反序列化成字符串），那就是`"application/json"`。
对于`"application/x-www-form-urlencoded"`，其参数组织形式，是键值对，类似于get方式的参数形式：`name=张三&sex=男&tel=5354169`，而json大家都知道，是这样的：
```
{
    name:"张三",
    sex:"男",
    tel:"5354169",
}
```
所以，如果你传递的参数是json，而你声明的header是`"application/x-www-form-urlencoded"`，或者没有声明这个”Content-Type”（似乎默认就是”application/x-www-form-urlencoded”），则服务器拿不到提交的数据。数据是传过去了，但读不出来。
反之应该也一样。
2018.7.20 
另外，如果提交的数据中含有汉字，编码跟服务器不同的话，也有可能导致服务器接收不到提交的数据。
