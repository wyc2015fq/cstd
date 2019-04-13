
# Jmeter接口测试实例图文示例 - anlcy - 博客园






# [Jmeter接口测试实例图文示例](https://www.cnblogs.com/camilla/p/7127015.html)
以getObjectByCode接口为例，用jmeter2.13来进行**接口测试**。
**测试前准备：**
测试工具及版本：jmeter 2.13 r1665067（须包含__MD5函数）
示例接口：8.1根据单品编码查询上架状态的单品详情
接口方法：/single/v1/getObjectByCode
1.准备jmeter免安装包，把...\bin\jmeter.bat添加到桌面快捷方式。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125501djUY.jpg)
2.点击电脑桌面jmeter快捷方式，进入jmeter界面。界面默认会有一个测试计划和一个**工作**台，测试计划下默认会有一个线程组。
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255011ZPyR.jpg)
3.测试计划命名为商品服务，右击测试计划->添加->Threads(Users)->线程组，新建一个线程组，线程组命名为根据单品编码查询上架状态的单品详情。
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255013dVX2.jpg)
4.在线程组下添加HTTP请求，路径为右击“根据单品编码查询上架状态的单品详情”->添加->Sampler->HTTP请求。
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255014Xp5W.jpg)
5.根据wiki的“根据单品编码查询上架状态的单品详情”接口文档和环境文档在HTTP请求界面填写详细信息。
5.1在Web服务器填写
服务器名称或IP：172.18.0.120              端口号：7989
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255015BvCV.jpg)
5.2在HTTP请求填写
Implementation：HttpClient4                方法：POST
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255016B8HX.jpg)
路径：/single/v1/getObjectByCode
5.3在请求参数区域，点击“添加”按钮添加请求参数
请求参数列表如下：
名称               值
appId            ${appId}
channel         ${channel}
channelIp      ${channelIp}
code             ${code}
t                   ${time}
terminalType  ${terminalType}
h ${__MD5(appId=${appId}&channel=${channel}&channelIp=${channelIp}&code=${code}&t=${time}&terminalType=${terminalType}&appKey=b320de0549a24ff6995dc0e2c38ff491,h)}
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255017Bp6h.jpg)
**注意：**
请求的所有参数，需要根据参数名=参数值的格式，按照字符字典顺序(asscii值大小)排序，若遇到相同首字符，则判断第二个字符，以此类推，待签名字符串需要以“参数名1=参数值1&参数名2=参数值2&..&参数名N=参数值N”的规则进行拼接。
在对请求的参数做签名时，对于请求参数列表中那些可空的参数，如果选择使用它们，那么这些参数的参数值必须不能为空或空值。否则请从参数列表中移除参数值为空的参数键值对。
在对请求的参数做签名时，这些参数必须来源于请求参数列表，并且去除列表中的参数h。
用签名密钥(appKey)对请求参数字符串MD5算法进行签名后，将得到的摘要添加到请求参数字符尾部，签名字符串与请求参数字符串之间用&分隔。
6.在线程组下添加用户参数，路径为右击“根据单品编码查询上架状态的单品详情”->添加->前置处理器->用户参数。
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255018BTSe.jpg)
7.把time参数化，在用户参数界面点击添加变量，在“名称”填写time,在“用户_1”内填写${__time(/1000,)}。
这样做的目的是保证请求参数中的t的值与MD5加密的t值保持一致。
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411283311giV.jpg)
8.添加CSV文件，路径为右击“根据单品编码查询上架状态的单品详情”->添加->配置元件->CSV Data Set Config。![](http://www.51testing.com/attachments/2016/07/14982672_201607041125502o6VI.jpg)
9.在CSV Data Set Config界面填写文件名、编码格式、参数名、分隔符,其它的取默认值。
**注意：**jmx文件和.txt(.csv)文件须放在同一目录。示例中的jmx文件和CSV文件放在D:\apache-jmeter-2.13\testplan，与load runner不同的是jmeter不判断字段名，所以如果数据文件带有字段名称就要删除字段名称所在的第一行，分隔符不填写时以,分隔，填写时以填写的符号进行分隔。
Filename: getObjectByCode.txt/getObjectByCode.csv
File encoding:utf-8
Variable Names(comma-delimited): appId,channel,channelIp,code,terminalType
Delimiter(use ‘\t’ for tab):,
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125503Ck7x.jpg)
10.在getObjectByCode.txt文件中填写有代表性的请求参数的值，包括正常值、异常值、空值。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125504UDsV.jpg)
11.getObjectByCode.txt文件中有12条请求参数值，设置循环次数为12，点击“根据单品编码查询上架状态的单品详情”，设置循环次数为12，其它的取默认值。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125505JmbO.jpg)
12.添加察看结果树，路径为右击“商品服务”->添加->监听器->察看结果树。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125506AXC1.jpg)
13.点击启动按钮（绿色三角形），执行接口测试。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125507rv8g.jpg)
14.如果有多个线程组时，方便更清楚地查看结果，可以选择禁用其它线程组，启用当前线程组。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125508U3D9.jpg)
15.点击察看结果树->设置响应数据显示格式为JSON->点击任意一个HTTP请求->点击响应数据，这样可以查看结果。
点击请求可以查看请求的参数是否正确。
点击清除和清除全部按钮都可以清除结果。
![](http://www.51testing.com/attachments/2016/07/14982672_201607041125509y7lf.jpg)
16.去测试数据库（172.18.1.149）查询HWG_PRT_SINGLE表，验证jmeter响应数据是正确：参数的数量、参数的值。
对应SQL命令为：SELECT * FROM HWG_PRT_SINGLE WHERE HWG_PRT_SINGLE.`SINGLE_CODE`='S0002938490'
![](http://www.51testing.com/attachments/2016/07/14982672_20160704112550106WMb.jpg)
17.jmeter接口测试过程中，可以在172.18.0.120上查看对应日志。对应的命令为：
cd /home/tomcat/tomcat-product/logs
tail -f catalina.out
![](http://www.51testing.com/attachments/2016/07/14982672_2016070411255012TE38.jpg)





