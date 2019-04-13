
# Python 标准库 —— json - Zhang's Wikipedia - CSDN博客


2018年03月03日 22:41:36[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：281个人分类：[python](https://blog.csdn.net/lanchunhui/article/category/5912917)



当请求 headers 中，添加一个name为 Accept，值为 application/json 的 header（也即“我”（浏览器）接收的是 json 格式的数据），这样，向服务器请求返回的未必一定是 HTML 页面，也可能是 JSON 文档。
## 1. 数据交换格式 —— JSON（JavaScript Object Notation）
http 1.1 规范
请求一个特殊编码的过程在 http1.1 规范中称为内容协商（content negotiation）
JSON 特点
JSON 是互联网上使用的最为成功的数据交换格式；
易于理解，轻量级；
与更早的 XML 相比，其能够更有效地传输；
JSON 不但易于人读写，也易于机器生成与解析；
2. python 下的 json 标准库
>>importjsonload/dump
loads/dumps
>> s =json.dumps('香气')
>> s'"\\u9999"'>>json.loads(s)'香'

