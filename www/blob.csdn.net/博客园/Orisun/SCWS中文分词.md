# SCWS中文分词 - Orisun - 博客园







# [SCWS中文分词](https://www.cnblogs.com/zhangchaoyang/articles/2034036.html)





说先中文分词。
- 默认的单字切分。举个例子，有以下句子：“我们在吃饭呢”，则按字切分为[我]、[们]、[在]、[吃]、[饭]、[呢]。按这种方法分词所得到的term是最少的，因为我们所使用的汉字就那么几千个。
- 
二元切分，即以句子中的每两个字都作为一个词语。继续拿“我们在吃饭呢”这个句子作例子，用二元切分法会得到以下词：[我们]、[们在]、[在吃]、[吃饭]、[饭呢]。
- 
按照词义切分。这种方法要用到词典，常见的有正向最大切分法和逆向最大切分法等。我们再拿“我们在吃饭呢”作为例子。使用正向切分法最终得到词语可能如下：[我们]、[在吃]、[饭]、[呢]，而使用逆向最大切分法则可能最终得到以下词语：[我们]、[在]、[吃饭]、[呢]。
- 
基于统计概率切分。这种方法根据一个概率模型，可以从一个现有的词得出下一个词成立的概率，也以“我们在吃饭呢”这个句子举个可能不恰当的例子，假设已经存在[我们]这个词语，那么根据概率统计模型可以得出[吃饭]这个词语成立的概率。当然，实际应用中的模型要复杂得多，例如著名的隐马尔科夫模型。
[http://www.google.com.hk/ggblog/googlechinablog/2006/04/blog-post_1583.html](http://www.google.com.hk/ggblog/googlechinablog/2006/04/blog-post_1583.html)

在实际的中文分词应用中，一般会将按词典切分和基于统计概率切分综合起来，以便消除歧义，提高精确率。

我在一篇博客也提到过中文分词采用的散列存储技术。
[http://www.cnblogs.com/zhangchaoyang/articles/2026070.html](http://www.cnblogs.com/zhangchaoyang/articles/2026070.html)


    google或百度等大公司的分词引擎都是自己开发或有专门的公司开发的，的确已经算比较成熟。但市场上提供免费甚至开源的分词引擎不多，中科院研发的ictclas30分词精确度和分词速度都非常不错，而且还有词性标注和自定义添加词的功能，可惜不开源。另外比较受欢迎的还有libmmseg和SCWS，因此都是开源的，不过经测试libmmseg的分词精度似乎不高，而SCWS由于使用了大量的递归，在生成词库的时候经常导致栈溢出，需要自己将递归修改为循环，从演示的情况来看，SCWS的分词精度来算可以。

![](https://pic002.cnblogs.com/images/2011/103496/2011050221034538.jpg)

关于中文分词,这里介绍了4款开源的中文分词系统http://witmax.cn/4-open-source-cws.html

ICTCLAS虽然很强大,准确率高,但由于只支持GBK编码,所以我选择了SCWS.

SCWS的详细文档请看http://www.ftphp.com/scws/docs.php#instscws,这里还有一个分词的在线演示http://www.ftphp.com/scws/demo/v48.php

在SCWS-1.1.6安装说明的第8点有个错误,编译test程序时指定的include文件路径应该是/usr/local/scws/include/scws

装好SCWS后用它的命令工具.

SCWS还有PHP扩展,我也懒得去写PHP代码,所以也就没装.其实到下一个环节,clucene也有PHP扩展,大不了到用clucene的时候我还用C++,就不用PHP.
Usage: scws [options] [input] [output]

-i <>: 要切分的字符串或者文件路径，如不指定则程序自动读取标准输入，每输入一次换行执行一次分词
-o <>: 切分结果输出保存的文件路径，若不指定直接输出到屏幕
-c <>: 指定分词的字符集，默认是 gbk
-r <>: 指定规则集文件（规则集用于数词、数字、专有名字、人名的识别）
-d <>: 指定词典文件路径（XDB格式，请在 -c 之后使用）
       *NOTICE*
       自 1.1.6 版起，开始支持多词典同时载入，词典格式也支持纯文本格式（必须是.txt结尾）。
       
       多词典路径之间用冒号(:)隔开，排在越后面的词典优先级越高。

       文本词典的数据格式参见 gen_scws_dict 所用的格式，但更宽松一些，允许用不定量的空格分开，
       只有<词>是必备项目，其它数据可有可无，当词性标注为“!”（叹号）时表示该词作废，即使在
       较低优先级的词库中存在该词也将作废。
-M <>: 复合分词的级别：1~15，按位异或的 1|2|4|8 依次表示 短词|二元|主要单字|全部单字。缺省不复合分词。
-I:    输出结果忽略跳过所有的标点符号
-A:    显示分词属性
-E:    将 xdb 词典读入内存 xtree 结构 (如果切分的文件很大才需要)
-N:    不显示切分时间和提示
-D:    debug 模式 (不必用, 需要编译时打开 --enable-developer)
-U:    将闲散单字自动调用二分法结合
-t <>: 取得高频词，参数为个数
-a [~]<attr1,..> 只显示某些词性的词，加~表示过滤该词性的词
-v:    查看版本
-h:    查看帮助

我是这么干的:
orisun@zcypc:~$ scws -i ./zcy -c utf-8 -o yyy -d /usr/local/scws/etc/dict.utf8.xdb

+--[scws(scws-cli/1.1.6)]----------+
| TextLen:   130                 |
| Prepare:   0.0009    (sec)     |
| Segment:   0.0008    (sec)     |
+--------------------------------+
注意输入文件名中不能含有".",否则会把它当成字符串处理.

下面给出一个SCWS   C编程实例：

#include <scws/scws.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
main()
{
	scws_t s;
	scws_res_t res, cur;
	char *text = "Hello, 我名字叫李那曲是一个中国人, 我有时买Q币来玩, 我还听说过C#语言";

	if (!(s = scws_new())) {
		printf("error, can't init the scws_t!\n");
		exit(-1);
	}
	scws_set_charset(s, "utf-8");
	scws_set_dict(s, "/usr/local/etc/dict.utf8.xdb", SCWS_XDICT_XDB);
	scws_set_rule(s, "/usr/local/etc/rules.utf8.ini");

	scws_send_text(s, text, strlen(text));
	while (res = cur = scws_get_result(s))
	{
		while (cur != NULL)
		{
			printf("Word: %.*s/%s (IDF = %4.2f)\n", cur->len, text+cur->off, cur->attr, cur->idf);
			cur = cur->next;
		}
		scws_free_result(res);
	}
	scws_free(s);
}

![](https://pic002.cnblogs.com/images/2011/103496/2011051109411316.png)

首先给Perl安装Text::Scws模块 perl Makefile.PL LIBS='-L/usr/local/lib' INC='-I/usr/local/include/scws'  make  make test  sudo make install
SCWS的Perl编程实例：
  #!/usr/bin/perl
  use Text::Scws;
  $scws = Text::Scws->new();
  $scws->set_charset('utf-8');
  $scws->set_dict('/usr/local/etc/dict.utf8.xdb');
  $scws->set_rule('/usr/local/etc/rules.utf8.ini');
  $scws->set_ignore(1);
  $scws->set_multi(1);

  $s = shift;
  $scws->send_text($s);
  while ($r = $scws->get_result()) {
    foreach (@$r) {
        print $_->{word}, " ";
    }
  }
  print "\n";

![](https://pic002.cnblogs.com/images/2011/103496/2011051115434457.png)












