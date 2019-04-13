
# 玩Python遇到的问题一二三及解决办法 - Magician的博客 - CSDN博客


2018年05月07日 14:28:15[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：507所属专栏：[Python总结](https://blog.csdn.net/column/details/26813.html)




### 文章目录
[问题一：python 2.7版本解决TypeError: 'encoding' is an invalid keyword argument for this function。](#python_27TypeError_encoding_is_an_invalid_keyword_argument_for_this_function_3)
[问题二：python读取文件时提示"UnicodeDecodeError: 'gbk' codec can't decode byte 0x80 in position 205: illegal multibyte sequence"](#pythonUnicodeDecodeError_gbk_codec_cant_decode_byte_0x80_in_position_205_illegal_multibyte_sequence_19)
[问题三：not enough arguments for format string](#not_enough_arguments_for_format_string_29)
[问题四：输出的信息为转义码](#_34)
[问题五：使用list的clear（）方法时的注意点](#listclear_47)
[问题六：Python下调用json.dumps中文显示问题解决办法](#Pythonjsondumps_89)
[问题七：报错：pymysql.err.OperationalError: (1040, 'Too many connections')](#pymysqlerrOperationalError_1040_Too_many_connections_102)
[问题八：ValueError: invalid literal for int() with base 10: “”](#ValueError_invalid_literal_for_int_with_base_10__107)
[问题九：解决python中TypeError: not enough arguments for format string](#pythonTypeError_not_enough_arguments_for_format_string_136)

最近要频繁的玩Python，在这里总结下遇到的一些问题，持续更新中。
# 问题一：python 2.7版本解决TypeError: ‘encoding’ is an invalid keyword argument for this function。
用Python2.7来打开一些文件的时候，经常出现以上的所表示的问题，如
`data_file = open("F:\\MyPro\\data.yaml", "r", encoding='utf-8')`运行的时候报错：TypeError: ‘encoding’ is an invalid keyword argument for this function。**但在Py3中运行却不会遇到这样的问题**。
解决办法：网上查找一番后，改成如下这样就可以搞定
`import io
data_file = io.open("F:\\MyPro\\data.yaml", "r", encoding='utf-8')`至于原因根据报错的信息看也许跟Py2和Py3的API规定不同有关吧。
# 问题二：python读取文件时提示"UnicodeDecodeError: ‘gbk’ codec can’t decode byte 0x80 in position 205: illegal multibyte sequence"
同样事读取文件时，碰到的编码问题。
解决办法：
`FILE_OBJECT= open('order.log','r', encoding='UTF-8')`只需要加入对应的encoding参数就OK了，对于Python中的编码问题，有时候也很麻烦。不过这类问题一般百度下就能找到对应的解决办法。
# 问题三：not enough arguments for format string
在Python中执行SQL的查询语句的时候爆出这样的错误，如
`sql='select * from Teacher where Tname like \'%%'+ keyword +'%%\''`。后来才知道出现这类问题，主要是字符串中包含了%号，python 认为它是转移符，而实际我们需要的就是%， 这个时候，可以使用%%来表示%。
# 问题四：输出的信息为转义码
`# ((u'01', u'\u5f20\u4e09'), (u'04', u'\u5f20\u4e09\u4e30')) # (u'01', u'\u5f20\u4e09')`这种情况也只有在Py2中才会出现，Py3中是不会出现的。
解决办法：将输出的转义码通过以下的eval方法转化为对应的中文。
`print(eval("u"+"\'"+exmple[1]+"\'"))`
# 问题五：使用list的clear（）方法时的注意点
最近在写一段如下代码时：
`for line in lines:
    # print(line)
    for x in jieba.lcut(line):
        # print(x)
        if x not in stopwords:
            text1.append(x)
    text2=text1
    corpora_documents.append(text2)
    # print(corpora_documents)
    text1.clear()`这里由于类型转化问题不能采用简便的方法来直接分词并剔除停词，所以只能用这种方式来实现相同的功能，但是在实现的过程中，原本期望返回的corpora_documents是下图所示的效果：
![这里写图片描述](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是却得到了一组空的List。这让博主很是郁闷（毕竟博主之前是写Java的，面相对象的思想“根深蒂固”，以为声明出来一个新的text2对象，就会开辟一块新的内存空间来存储text2的内容，这样text1和text2也就是值传递了而非引用传递，可是这是java的内存加载机制。。。），不过后来随着博主无意间将最后一行代码改为：
[
](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`text1=[]`[
](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)得到了自己想要的结果，之后再分析原因的时候，发现博主没有切换到Python的内存管理机制。其实整个过程对于Python来说内存管理如下：
[
](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`for line in lines:
    # print(line)
    for x in jieba.lcut(line):
        # print(x)
        if x not in stopwords:
            text1.append(x)
    text2=text1#相当于将声明的两个对象的指针指向同一块内存空间即text1所在的内存空间
    corpora_documents.append(text2)#Python采用的应该是延迟加载，也就是说这里corpora_documents只是将指针指向了text2的内存地址，并没有直接就将text2里面的内容加载进来。
    # print(corpora_documents)
    text1.clear()#clear()就相当于擦除了text1中的内容，即text2指向了一个空的内容，由于Python延迟加载的特性也就造成了后面打印出来的corpora_documents的内容为空
    print(corpora_documents)`[
](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)所以在使用clear（）的时候一定要注意对象之间的关系，一不留神就会造成很大的损失。感觉这个问题好像在那本书中有提到，还得复习下基础知识，很多问题只有不断的Coding才能碰到，还是实践太少，仍需继续努力。
[
](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 问题六：Python下调用json.dumps中文显示问题解决办法
[
](https://img-blog.csdn.net/20180525173138263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)最近在开发某个功能的时候，需要将前台的信息返回到后天处理后，以json字符串的形式返回，但是返回的内容显示在网页中确实ASCII码值，下面是这个问题的解决办法：
**配置下dumps方法中的ensure_ascii这个属性！**
`data={"url":"http:www.dianwe.com","content":"采集Test","CreateTime":"2014-07-08 23:29"}  
bizResult= json.dumps(data, ensure_ascii=False)   
print(bizResult)`这样便不会转为Ascii 编号了！
# 问题七：报错：pymysql.err.OperationalError: (1040, ‘Too many connections’)
mysql数据库 Too many connections
出现这种错误明显就是 mysql_connect 之后忘记 mysql_close；我的情况是实现了自动更新本地数据库在测试的时候每次更新后又忘记关闭对应的connect，当大量的connect之后，就会出现Too many connections的错误，mysql默认的连接为100个。只需在代码中添加对应的connect.close()方法即可。
# 问题八：ValueError: invalid literal for int() with base 10: “”
最近在写代码的时候，突然爆出这样错误，根据字面意思是类型转化的问题，但是仔细检查了代码，发现类型转化没有问题。无奈只能去求助度娘，很多博客也说是类型转换的问题。以下是某博客的一个实验：
`>>> int('')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: invalid literal for int() with base 10: ''
>>>
>>> int("x")
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: invalid literal for int() with base 10: 'x'`从中可以看出这类错误就是字符无法转换为int型。但是仍找不到对应的错误的位置在哪里，刚好看看到一个建议：“为了避免类似错误，以后进行int类型转换时，注意检查，或者直接加try 捕获下”
`# try:
        #     print(int(re.sub("\D", "", x)))
        # except ValueError:
        #     print("-"*40)
        #     print("异常值"+x)`一查竟让多出了一些nan值：
![在这里插入图片描述](https://img-blog.csdn.net/20181002170232926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是后来去和数据做比对的时候，实在是也没有发现有什么nan值，这就很奇怪了，所以为了能让程序跑下去，这里就先省去了这个int的强制变换。
[
](https://img-blog.csdn.net/20181002170232926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 问题九：解决python中TypeError: not enough arguments for format string
[
](https://img-blog.csdn.net/20181002170232926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)出现这类问题，主要是字符串中包含了%号，python 认为它是转移符，而实际我们需要的就是%， 这个时候，可以使用%%来表示
[
            ](https://img-blog.csdn.net/20181002170232926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

