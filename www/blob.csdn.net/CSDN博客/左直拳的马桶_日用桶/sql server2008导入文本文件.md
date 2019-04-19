# sql server2008导入文本文件 - 左直拳的马桶_日用桶 - CSDN博客
2018年10月12日 18:59:49[左直拳](https://me.csdn.net/leftfist)阅读数：783
用sql server导入文本文件到数据库，我印象中是很麻烦的。多年不用sql server之后，今日再试试，果不其然，真TM麻烦啊。
导入的文件是IIS FTP的日志，很规范的一个文本文件了。但就是找不到一个日志分析工具，所谓的log parser 2.2并不支持FTP的日志。最后只能先导到SQL SERVER里。如果能导进去，也方便统计。
折腾。记录如下：
**1、乱码问题**
1）出现乱码，需要变换代码页，如图所示
![在这里插入图片描述](https://img-blog.csdn.net/20181012181125869?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）注意导到目标表的相关含中文字段，应为nvarchar，而不是默认的varchar
![在这里插入图片描述](https://img-blog.csdn.net/20181012184501876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
设置为nvarchar，可能会有警告，选择忽略
![在这里插入图片描述](https://img-blog.csdn.net/20181012184558333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2、字段不够长内容截断问题**
有两方面
1）读取文件时，字段要足够长
![在这里插入图片描述](https://img-blog.csdn.net/20181012185212905?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）目标表字段也要足够长
**3、字段命名问题**
从文本文件导入数据，系统可以自动创建表，但字段名如何命名是个问题。应该采取的办法是，将字段名按照数据行的格式，按顺序写在第一行，让系统自动识别。
![在这里插入图片描述](https://img-blog.csdn.net/2018101218572971?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这句话的意思就是：“系统你知道了吗，第一行是字段名，不是数据”
对应文本文件
```
date time c-ip cs-method cs-uri-stem sc-status sc-win32-status sc-substatus sc-bytes cs-bytes time-taken x-session x-fullpath
2018-10-11 03:58:06 30.3.4.105 ControlChannelOpened - - 0 0 0 0 0 17034547-ee1d-44f5-b27c-ed72f4ef2898 -
2018-10-11 03:58:06 30.3.4.105 USER administrator 331 0 0 42 20 0 17034547-ee1d-44f5-b27c-ed72f4ef2898 -
2018-10-11 03:58:06 30.3.4.105 PASS *** 230 0 0 21 19 0 17034547-ee1d-44f5-b27c-ed72f4ef2898 /
```
