
# jmeter 使用白皮书 - anlcy - 博客园






# [jmeter 使用白皮书](https://www.cnblogs.com/camilla/p/10037011.html)
**一、jmeter中的响应端言**
断言就类似LoadRunner中的检查点。对上一个请求返回的信息，做字符串、数据包大小、HTML、XML、图片等做判断，确保返回的信息的准确性。
以下只讲解下响应断言步骤，新建一个线程组，线程组中添加一个http请求，添加响应断言：添加-》断言-》响应断言：
**配置说明：**
apply to：是应用范围，设定匹配的范围
Main sample and sub-samples:匹配范围为当前父取样器，及子取样器
Main sample only ：仅当前父取样器
Sub samples only:仅子取样器
JMeter Variable：变量值进行匹配
要测试的响应文字：针对响应数据不同部分进行匹配
(1)响应文本：响应服务器返回的文本内容，http协议排除header部分
(2)响应代码：匹配响应代码，比如http请求中‘200’代表成功
(3)响应信息：匹配响应信息，处理成功返回‘成功’或者“ok”字样
(4)Response Header:匹配响应头中的信息
(5)Document(Text)：一切Apache Tika 支持服务器响应，包括文本响应，还支持 PDF, Office, Audio, Video formats。
(6) url样本：是对sample的url进行断言。如果请求没有重定向（302），那么就是这个就是请求url。 如果有重定向（且跟随重定向），那么url就包含请求url 和 重定向url
匹配规则：
包括：响应内容包括需要匹配的内容就算成功
匹配：响应内容要完全匹配匹配内容，不区分大小写
equals：完全相等，区分大小写
substring：响应内容包括匹配内容即为成功。
否：就相当于取反。 如果上面断言结果为true，勾选“否”后，最终断言结果为false。如果上面断言结果为false，勾选“否”后，则最终断言结果为 true。
要测试的模式：这里就需要输入 我们要匹配的响应接口(即接口测试结果的期望值或者其他期望值)
可以通过添加断言结果来查看断言的执行情况。
**二：正则表达式提取器**
允许用户从作用域内的sampler请求的服务器响应结果中通过正则表达式提取值所需值，生成模板字符串，并将结果存储到给定的变量名中。
新建一个线程组，线程组中添加一个http请求，添加正则表达式提取器：添加-》后置处理器-》正则表达式提取器：
**配置说明：**
APPly to:作用范围（返回内容的断言范围）
1、Main sample and sub-samples:作用于主节点的取样器及对应子节点的取样器
2、Main sample only：仅作用于主节点的取样器
3、Sub-samples only:仅作用于子节点的取样器
4、JMeter Variable:作用于jmeter变量(输入框内可输入jmeter的变量名称)，从指定变量值中提取需要的值。
Field to check：要检查的响应报文的范围
1、主体：响应报文的主体，最常用
2、Body(unescaped)：主体，是替换了所有的html转义符的响应主体内容，注意html转义符处理时不考虑上下文，因此可能有不正确的转换，不太建议使用
3、Body as a Document：从不同类型的文件中提取文本，注意这个选项比较影响性能
4、Response Headers：响应信息头（如果你使用的是中文版的Jmeter，会看到这一项是信息头，这是中文翻译问题，应以英文为准）
5、Request Headers:请求信息头
6、URL：请求url
7、Response Code:响应状态码，比如200、404等
8、Response Message:响应信息
（1）引用名称：下一个请求要引用的参数名称，如填写title，则可用${title}引用它。
（2）正则表达式：
()：括起来的部分就是要提取的。
.：匹配任何字符串。
+：一次或多次。
?：不要太贪婪，在找到第一个匹配项后停止。
（3）模板：用
$$
引用起来，如果在正则表达式中有多个正则表达式，则可以是$2
$$
3$等等，表示解析到的第几个值给title。如：$1$表示解析到的第1个值
（4）匹配数字：0代表随机取值，1代表全部取值，通常情况下填0
（5）缺省值：如果参数没有取得到值，那默认给一个值让它取。
举例说明：
1、提取单个字符串：
假如想匹配Web页面的如下部分：name = "file" value = "readme.txt">并提取readme.txt。一个合适的正则表达式：name = "file" value = "(.+?)">。
()：封装了待返回的匹配字符串。
.：匹配任何单个字符串。
+：一次或多次。
?：不要太贪婪，在找到第一个匹配项后停止。
2、提取多个字符串：
假如想匹配Web页面的如下部分：name = "file.name" value = "readme.txt">并提取file.name和readme.txt。一个合适的正则表达式：name = "(.+?)" value = "(.+?)"。这样就会创建2个组，分别用于$1$和$2$
比如：
引用名称：MYREF
模板：$1
$$
2$
如下变量的值将会被设定：
MYREF: 　　　　file.namereadme.txt
MYREF_g0:　　 name = "file.name"value = "readme.txt"
MYREF_g1:　　file.name
MYREF_g2:　　readme.txt
在需要引用地方可以通过：${MYREF}, ${MYREF_g1进行使用。
**三、简单分析**
a）结果树：
取样器结果：Response code：200 表示执行接口调试成功；
请求：发送的数据；
响应数据：返回的数据；
Thread Name:线程名称组；
Sample Start：启动开始时间；
Load time：加载时长；
Latency：等待时长；
Size in bytes： 发送的数据总大小；
Headers size in bytes：发送数据的其余部分大小；
Sample Count：发送统计；
Error Count：交互错误统计；
Response code：返回码；
Response message:返回信息；
Response headers：返回的头部信息；
b）聚合报告：
Label：httpRequset Name 属性值；
Samples：测试过程中一共发出了多少个请求即总线程数，（如果模拟10个用户，每个用户迭代10次，这里就显示100），对应图形报表中的样本数目；
Average：单个reques的平均响应时间，计算方法是总运行时间除以发送到服务器的总请求数，对应图形报表中的平均值；
Median：50%用户的响应时间;
90%Line:90%用户的响应时间；
Min：服务器响应的最短时间；
Max：服务器响应的最长时间；
Error%：本次测试中出错率，请求的数量除以请求的总数；
Throughput：吞吐量--默认情况下表示每秒完成的请求数；
KB/Sec：每秒从服务器接收到的数据量，即每秒钟请求的字节数，时间单位均为ms；
c）图形结果：
样本数目：测试过程中一共发送了多少个请求即总线程数；
最新样本：代表时间的数字，是服务器响应最后一个请求的时间；
吞吐量：服务器每分钟处理的请求数；
平均值：总运行时间除以发送到服务器的请求数；
中间值：有一半的服务器响应时间低于改值而另一半高于该值；
偏离：表示服务器响应时间变化、离散程度测量值的大小；





