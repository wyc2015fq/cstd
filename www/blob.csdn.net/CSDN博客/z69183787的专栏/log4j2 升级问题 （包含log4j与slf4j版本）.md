# log4j2 升级问题 （包含log4j与slf4j版本） - z69183787的专栏 - CSDN博客
2017年07月04日 13:41:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5067
所需版本及jar：
|jar包|版本|备注|
|----|----|----|
|slf4j-api|1.7.2|jcl-over-slf4j 也需要统一使用 1.7.2 版本|
|log4j-slf4j-impl|2.3||
|log4j-api|2.3||
|log4j-core|2.3||
|log4j-1.2-api|2.3| |
.
需清理jar：
|需删除jar包|说明|
|----|----|
|**slf4j-log4j12**|slf4j到log4j1的桥接包|
|**slf4j-jdk14**|slf4j到java.util.logging的桥接包|
|**log4j-to-slf4j**|将log4j2的接口适配到slf4j上，不能和log4j-slf4j-impl同时存在|
|**slf4j-jcl**|将slf4j适配到JCL上|
|**logback-classic**|将slf4j适配到logback上|
|**slf4j-simple**|一个简单桥接实现|
|jar包|版本|备注|
|----|----|----|
|slf4j-api|1.7.2|jcl-over-slf4j 也需要统一使用 1.7.2 版本|
|log4j-slf4j-impl|2.3||
|log4j-api|2.3||
|log4j-core|2.3||
|log4j-1.2-api|2.3| |
