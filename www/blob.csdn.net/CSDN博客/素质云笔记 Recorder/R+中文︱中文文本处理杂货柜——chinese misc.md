
# R+中文︱中文文本处理杂货柜——chinese.misc - 素质云笔记-Recorder... - CSDN博客

2017年03月19日 21:58:25[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2059



> Package: chinese.misc Version: 0.1.3 Date: 2017-03-12 Authors：Wu Jiang

> (吴江)，微信号：theblackriver

> 受R语言中文社区的文章
> [《diRblo|中文文本分析方便工具包chinese.misc简介(附文本样例)》](http://mp.weixin.qq.com/s?__biz=MzA3MTM3NTA5Ng==&mid=2651055789&idx=1&sn=c951b4266939dd6645bae75c863ec20e&chksm=84d9c73ab3ae4e2ca14f8ebbc575a0ed52d682fb17ddab5ddc9330c4444195bd5fe0c7f698ce&mpshare=1&scene=1&srcid=0319mItvGGZ7TrkPh1XBpQ1m#rd)
> 启发，来看看这个刚刚发布的中文处理新包，感谢作者允许转载。因为作者在
> [github](https://github.com/githubwwwjjj/chinese.misc/blob/master/README.md)
> 上写的有点乱，笔者在此基础上进行整理。

> chinese.misc中的内容很多，简单罗列：

> 1、文本挖掘方向：

> ◎ 外部文档文件夹分词

> ◎ 生成文档-词语矩阵

> ◎ output_dtm-DTM变为矩阵

> ◎ sort_tf词语排序

> ◎ 文本清洗停用词

> ◎ 文本清洗词性筛选器slim_text

> ◎ 词条相关性word_cortmfindAssocs

> ◎ 话题时间趋势topic_trend

> 2、文件管理方向：

> ◎ 获取文件路径

> ◎ 获取文件夹下所有文件

> ◎ 读取文件scancn

> ◎ txt文件合并为csvtxt2csv

> ◎ csv变为txtcsv2txt

> 3、数据类型、格式转变：

> ◎ 正则匹配match_pattern

> ◎ tf2doc高级rep

> ◎ m2doc

> ◎ 两个逻辑判断is_character_vector和

> ◎ is_positive_integer

> ◎ 升级版ascharacter2批量转化字符型向量

> ◎ asnumeric2升级版批量转化为数值型

> 一、chinese.misc安装与数据准备

> 1、安装与依赖加载

> chinese.misc已经提交CRAN所以可以直接install。

> 同时注意，R的版本一定要3.3.2以上！

> install.packages(
> 'chinese
> .misc')
> library
> (chinese.misc)
> library
> (tm)
> library
> (jiebaR)
> 2、数据准备

> 生成2个文件夹，5个txt文件。

> a_new='hehe' \#If you want to change folder name, just change this.
gwd=getwd()
f=paste(gsub('/$', '', gwd), a_new, sep='/')
if (dir.exists(f)) stop ('Folder already exists. Please change a name.')
dir.
> create
> (f)
dir.
> create
> (paste(f,
> 'f1'
> , sep=
> '/'
> ))
dir.
> create
> (paste(f,
> 'f2'
> , sep=
> '/'
> ))
x=
> '以事件为选题的数据新闻最常出现在重大新闻事件的报道中。在这类事件中，数据报道可能是媒体精心制作的报道主体，也可能是媒体对事件的整个专题报道中的一个有机组成部分。可预见的重大新闻事件一般多指会议、活动、庆典或赛事，媒体可以把较为充足的时间投入到选题策划中。除了可预见的重大新闻事件以外，更多此类数据新闻的选题是突发新闻事件。近年来，越来越多的媒体将数据新闻运用于突发新闻事件的报道中，大量数据资源的整合和运用为此类新闻报道增添了更多科学性。'
> write
> .
> table
> (x, paste(f,
> 'f1/d1.txt'
> , sep=
> '/'
> ),
> row
> .
> names
> =
> FALSE
> , col.
> names
> =
> FALSE
> , quote=
> FALSE
> , fileEncoding=
> 'UTF-8'
> )
x=
> '人们对数据可视化的适用范围有着不同观点。例如，有专家认为数据可视化是可视化的一个子类目，主要处理统计图形、抽象的地理信息或概念型的空间数据。现代的主流观点将数据可视化看成传统的科学可视化和信息可视化的泛称，即处理对象可以是任意数据类型、任意数据特性，以及异构异质数据的组合。大数据时代的数据复杂性更高，如数据的流模式获取、非结构化、语义的多重性等。'
> write
> .
> table
> (x, paste(f,
> 'f1/d2.txt'
> , sep=
> '/'
> ),
> row
> .
> names
> =
> FALSE
> , col.
> names
> =
> FALSE
> , quote=
> FALSE
> , fileEncoding=
> 'UTF-8'
> )
x=
> '政治传播学是政治学与传播学的交叉学科，它是对政治传播现象的总结和政治传播规律的探索和运用，它包括政治传播的结构、功能、本质及技巧等方方面面。它的研究范围包括：政治传播行为，即政治传播的主体、客体及他们之间的相互关系体系；政治传播内容，即对政治的信息处理体系；政治传播途径，即政治符号和传播媒介体系；政治传播环境，即政治传播与相关社会现象；政治传播形态，即政治传播本体的形貌或表现体系。'
> write
> .
> table
> (x, paste(f,
> 'f2/d3.txt'
> , sep=
> '/'
> ),
> row
> .
> names
> =
> FALSE
> , col.
> names
> =
> FALSE
> , quote=
> FALSE
> , fileEncoding=
> 'GB18030'
> )
x=
> '改进社会治理方式。坚持系统治理，加强党委领导，发挥政府主导作用，鼓励和支持社会各方面参与，实现政府治理和社会自我调节、居民自治良性互动。坚持依法治理，加强法治保障，运用法治思维和法治方式化解社会矛盾。坚持综合治理，强化道德约束，规范社会行为，调节利益关系，协调社会关系，解决社会问题。坚持源头治理，标本兼治、重在治本，以网格化管理、社会化服务为方向，健全基层综合服务管理平台，及时反映和协调人民群众各方面各层次利益诉求。'
> write
> .
> table
> (x, paste(f,
> 'f2/d4.txt'
> , sep=
> '/'
> ),
> row
> .
> names
> =
> FALSE
> , col.
> names
> =
> FALSE
> , quote=
> FALSE
> , fileEncoding=
> 'GB18030'
> )
x=
> '所有这三种活动和它们的相应境况都与人存在的最一般状况相关：出生和死亡，诞生性和有死性。劳动不仅确保了个体生存，而且保证了类生命的延续。工作和它的产物——人造物品，为有死者生活的空虚无益和人寿的短促易逝赋予了一种持久长存的尺度。而行动，就它致力于政治体的创建和维护而言，为记忆，即为历史创造了条件。'
> write
> .
> table
> (x, paste(f,
> 'd5.txt'
> , sep=
> '/'
> ),
> row
> .
> names
> =
> FALSE
> , col.
> names
> =
> FALSE
> , quote=
> FALSE
> , fileEncoding=
> 'UTF-8'
> )
> .

> 二、chinese.misc-文本挖掘方向

> 1、外部文档、文件夹分词

> seg_file(
> ...
> ,
> \#必须是字符向量，一个或多个文件/文件夹名
> from =
> "dir"
> ,
> \#你的输入同文件/文件夹名，还是直接就是要处理的文本
> folder =
> NULL
> ,
> \#写入分词结果的文件夹
> mycutter = DEFAULT_cutter,
> \#分词器
> enc =
> "auto"
> ,
> \# 编码
> myfun1 =
> NULL
> ,
> \#定制函数1
> myfun2 =
> NULL
> ,
> \#定制函数2
> special =
> ""
> ,
> \#文件名需符合何种模式
> ext =
> "txt"
> \#若写入分词结果，以什么为后缀
> )
> 以上函数可以实现直接从文件夹里面读入文件并分词，分词时利用jiebaR来进行分词的。

> 懒人神器，不过具体项目应该不会这么随便。

> all_file <- dir_or_file(f)
> \#读入文件夹下所有文档
> y=seg_file(all_file)
> \# 读入文件+分词
> seg_file(all_file, folder=
> 'hehe/seg'
> )
> \# 读入数据+分词+写出并创建一个新文件夹
> all_text=unlist(lapply(all_file, scancn))
> \# 读入文件，并变为vector
> y=seg_file(all_text, from=
> 'v'
> )
> \#环境中的文本+分词
> seg_file(all_text, from=
> 'v'
> , folder=
> 'hehe/seg2'
> )
> \#环境中的文本+分词  +写出并创建新文件夹
> .

> 2、生成文档-词语矩阵

> corp_or_dtm(
> ...
> ,
> \#一个或多个文件/文件夹名，或待处理的文本
> from =
> "dir"
> ,
> \#你的输入是文件/文件夹名，还是待处理的文本
> type =
> "corpus"
> ,
> \#corpus为普通语料，默认值；DTM文档-词项矩阵；T词语-文档矩阵
> enc =
> "auto"
> , 
  mycutter = DEFAULT_cutter, 
  stop_word =
> NULL
> ,
> \#设定一个字符向量作为停用词，stop_word='jiebar'或'auto'，这样就直接使用jiebaR的停用词
> stop_pattern =
> NULL
> , 
  control = DEFAULT_control1, 
  myfun1 =
> NULL
> , 
  myfun2 =
> NULL
> , 
  special =
> ""
> )
> 可以实现，读入文件夹内容，分词，直接变为DTM/TDM

> dtm
> =
> corp_or_dtm(all_file,
> type
> =
> 'dtm'
> ,
> stop_word
> =
> 'jiebar'
> )
> mycorp
> =
> corp_or_dtm(all_text,
> from
> =
> 'v'
> ,
> stop_word
> =
> c('关系',
> '数据'))
> dtm
> =
> corp_or_dtm(all_text,
> from
> =
> 'v'
> ,
> type
> =
> 'tdm'
> ,
> control
> =
> 'auto2'
> )
> .

> 3、output_dtm-DTM变为矩阵

> output_dtm(
  x, 
  outputfile =
> NULL
> ,
> \#DTM/TDM转化成矩阵后写到什么文件里
> doc_name =
> NULL
> \#写入后的名称
> )
> 笔者觉得tm里面dtm不能查看是最不好用的地方了。

> x
> <- c(
> "Hello, what do you want to drink?"
> ,
> "drink a bottle of milk"
> ,
> "drink a cup of coffee"
> ,
> "drink some water"
> )
> dtm
> <- corp_or_dtm(x, from =
> "v"
> ,
> type
> = "dtm")
> m
> <- output_dtm(dtm, doc_name = paste(
> "doc"
> ,
> 1
> :
> 4
> ))
> m
> Terms
> Docs
> a bottle coffee cup
> do
> drink hello milk
> of
> doc
> 1
> 0
> 0
> 0
> 0
> 1
> 1
> 1
> 0
> 0
> doc
> 2
> 1
> 1
> 0
> 0
> 0
> 1
> 0
> 1
> 1
> doc
> 3
> 1
> 0
> 1
> 1
> 0
> 1
> 0
> 0
> 1
> doc
> 4
> 0
> 0
> 0
> 0
> 0
> 1
> 0
> 0
> 0
> Terms
> Docs
> some to want water what you
  doc
> 1
> 0
> 1
> 1
> 0
> 1
> 1
> doc
> 2
> 0
> 0
> 0
> 0
> 0
> 0
> doc
> 3
> 0
> 0
> 0
> 0
> 0
> 0
> doc
> 4
> 1
> 0
> 0
> 1
> 0
> 0
> .

> 4、sort_tf：词语排序

> 可以把词条进行排序。

> sort_tf (
  x,  
  top
> =
> 10
> ,
> type
> =
> "dtm"
> , 
  todf
> =
> FALSE
> ,
> \#todf
> 是指是否转化成数据框。
  must_exact
> =
> FALSE
> )
> x是DTM或TDM对象；但是也可以是matrix。如果是matrix，请务必用type指明你的这个是”dtm”还是”tdm”，分别用以”D/d“开头的，或以”T/t“开头的代表就行了。

> top 默认是10

> must_exact,代表top时候，如果末尾有相同排名的词，T代表删除

> 上案例：

> x <- c(
> "Hello, what do you want to drink?"
> ,
> "drink a bottle of milk"
> ,
> "drink a cup of coffee"
> ,
> "drink some water"
> ,
> "hello, drink a cup of coffee"
> )
dtm <- corp_or_dtm(x,
> from
> =
> "v"
> , type =
> "dtm"
> )
sort_tf(dtm, top =
> 5
> )
> sort_tf(dtm, top =
> 5
> )
In total, you have
> 15
> words
> .
drink&
> 5
> a
> &
> 3
> of
> &
> 3
> coffee&
> 2
> cup&
> 2
> hello&
> 2
> 其中&带的是词频。

> .

> 5、文本清洗：停用词

> make_stoplist(x=
> 'jiebar'
> ,
> print
> =
> TRUE
> )
> \# 直接调用jiebaR的停用词了，也可以自己定义
> \# 词语自动去重
> 自动获取了jiebar的停用词并print。

> .

> 6、文本清洗：词性筛选器slim_text

> 不过，不能自己修改词性，一些词的多词性问题不能得到解决。

> slim_text(
  x,
> \#字符
> mycutter = DEFAULT_cutter, 分词器
  rm_place =
> TRUE
> ,
> \#是否删去跟地点相关的，默认为是
> rm_time =
> TRUE
> ,
> \#是否删去跟时间相关的，默认为是
> rm_eng =
> FALSE
> ,
> \#是否删去含英文，默认为否
> rm_alpha =
> FALSE
> ,
> \#是否去除字母，默认为否，可以有FALSE, TRUE, "any"三个选项
> paste =
> TRUE
> \#是否把结果拼接成一个字符，默认为是
> )
> \# 要说明的是，有的词虽然只包含字母，但是并不会被标注为eng，所以不能通过rm_eng去掉，此时可通过rm_alpha=TRUE来去掉，且一旦如此，rm_eng的值会被无视。而如果用rm_alpha="any"，那么一个词只要包含一个英文字母，比如“A股”，就会被去掉。
> 先看个案例：

> x=
> '所有这三种活动和它们的相应境况都与人存在的最一般状况相关：出生和死亡，诞生性和有死性。劳动不仅确保了个体生存，而且保证了类生命的延续。工作和它的产物——人造物品，为有死者生活的空虚无益和人寿的短促易逝赋予了一种持久长存的尺度。而行动，就它致力于政治体的创建和维护而言，为记忆，即为历史创造了条件。'
> \#先看直接分词的效果
> seg_file(x, from=
> 'v'
> )
> \#再看用词性来删词的效果
> slim_text(x)
> \# 词语数减少四分之一的目的应该可以达到
> 按照词性删除带有地点、时间。当然这个基础是建立在词性标注准确之上的…

> .

> 7、词条相关性：word_cor==tm/findAssocs

> word_cor给出两种相关性：kendall（默认，词频不正态）、pearson

> 同时，可以实现：

> 给出相关性+显著性

> 可以系数限制和P值限制显示

> dtm=corp_or_dtm(all_file,
> type
> =
> 'dtm'
> , stop_word=
> 'jiebar'
> )
word_cor(dtm, word=c(
> '新闻'
> ,
> '数据'
> ,
> '可视化'
> ))
word_cor(dtm, word=c(
> '新闻'
> ,
> '数据'
> ,
> '可视化'
> ), p=
> 0.5
> )
> 来看看结果显示：

> > word_cor(dtm, word=c(
> '新闻'
> ,
> '数据'
> ,
> '可视化'
> ), p=
> 0.5
> )
$corMatrix
       可视化   数据   新闻
可视化
> NA
> 0.7454
> NA
> 数据
> 0.7454
> NA
> 0.4472
> 新闻
> NA
> 0.4472
> NA
> $pMatrix
       可视化   数据   新闻
可视化
> NA
> 0.0102
> 0.5071
> 数据
> 0.0102
> NA
> 0.1232
> 新闻
> 0.5071
> 0.1232
> NA
> .

> 8、话题时间趋势：topic_trend

> topic_trend(
  year,
> \#每个话题所在的年份
> topic,
> \#每个话题的类别
> relative
> =
> FALSE
> ,
> \#是计算相对数据还是绝对数据
> zero
> =
> 0
> \#NA或0，如果某一年某个话题一次都没出现，是算NA上是0，默认是0
> )
> 函数中，趋势的本质上就是简单lm()

> 其实这个功能要是有更多好的预测方式加入也不错。

> year=sample(2010: 2016, 200,
> replace
> =
> TRUE
> )
topic=sample(c(
> 'art'
> ,
> 'law'
> ,
> 'philosophy'
> ,
> 'literature'
> ,
> 'history'
> ,
> 'sociology'
> ),
> 200
> ,
> replace
> =
> TRUE
> )
topic_trend(
> year
> , topic)
> .

> 三、chinese.misc-文件管理方向

> 1、获取文件路径

> >
> dir
> (f)
[
> 1
> ]
> "d5.txt"
> "f1"
> "f2"
> 用dir(f) 返回文件夹名，跟python os.listdir

> .

> 2、获取文件夹下所有文件

> dir_or_file (
> ...
> ,
> \#一个或多个代表文件夹/文件名
> special =
> ""
> \#代表模式的正则表达式或字符
> )
> 目录下所有内容，默认是收集所有文件

> > dir_or_file(f)
 [
> 1
> ]
> "F:/R/Rstudy/hehe/d5.txt"
> [
> 2
> ]
> "F:/R/Rstudy/hehe/f1/d1.txt"
> [
> 3
> ]
> "F:/R/Rstudy/hehe/f1/d2.txt"
> [
> 4
> ]
> "F:/R/Rstudy/hehe/iamcsv.csv"
> > dir_or_file(f,special=
> 'txt$'
> )
 [
> 1
> ]
> "F:/R/Rstudy/hehe/d5.txt"
> [
> 2
> ]
> "F:/R/Rstudy/hehe/f1/d1.txt"
> [
> 3
> ]
> "F:/R/Rstudy/hehe/f1/d2.txt"
> 如果你的文件名有重复，也没事，反正最后会去重的。

> 同时，支持多个文件夹共同显示。

> x1
> <- find.package(
> "base"
> )
> x2
> <- find.package(
> "tm"
> )
> x3
> <- c(x1,x2)
> all_file
> <- dir_or_file(x3, special =
> "rds$"
> )
> all_file
> <- dir_or_file(x1, x2, special =
> "rds$"
> )
> 两种形式都可以，x1,x2，或者c（x1,x2）

> .

> 3、读取文件：scancn

> 不要用scancn来读.csv/.xls/.xlsx、.doc/.docx、.pdf/.caj文件，它不是用来读这些的！

> 它可以读的包括.txt、.rtf、.R、.py等等是普通文本的文件，以及没有后缀名的文件。

> scancn(
  x,
> \#一个文件名
> enc =
> "auto"
> ,
> \#编码，enc是你的文件的编码
> read
> _2nd = TRUE
> \#是否尝试读取两次
> )
> \# enc,尝试读入了日语
> \# UTF-8文件但实际上并不是且无法正常读取的文件进行二次读取。默认为TRUE
> 笔者用enc自动，可以正常读入日文。比read.table好一些，同时read_2nd可以设置重读。

> .

> 4、txt文件合并为csv：txt2csv

> txt2csv(
> ...
> ,
> \#一个或多个文件夹名或文件名
> csv,
> \#你要把形成表格的文本写到什么csv文件里
> must_txt =
> TRUE
> ,
> \#是否要读取的必须是txt文件
> na_in_txt =
> NULL
> \# 你的独立文件里的什么内容会被视为缺失值
> )
> 必须以csv结尾，不要尝试xls/xlsx

> na_in_txt，有的网页返回的是”404 ERROR”的话，你可以把这个算成缺失

> 案例：

> f1_and_f2=paste(f, c(
> 'f1'
> ,
> 'f2'
> ), sep=
> '/'
> )
txt2csv(f1_and_f2, csv=paste(f,
> 'iamcsv.csv'
> , sep=
> '/'
> ))
> \# 假如原文件没有实质内容，会写入NA。
> f1和f2合并为了iamcsv.csv文件了。批量化工作时候比较好。

> .

> 5、csv变为txt：csv2txt

> \# 自动命名：比如你有1234个文件，那么由于1234是四位数，所以会给文件加上0001, 0002, 0012, 0348, 1022这样的号码
> csv2txt(
  csv,
> \#你要读的CSV文件
> folder
> ,
> \#你要把分开来的文本写到什么文件夹里
> which,
> \#CSV里哪一列是文本，务必看以下说明
> header =
> TRUE
> ,
> \#CSV是否有表头
> row.names =
> NULL
> ,
> \#CSV是否有行标题
> na_in_csv = c(NA,
> ""
> ,
> " "
> ,
> "?"
> ,
> "NA"
> ,
> "999"
> ),
> \#CSV表格里哪些值被视为缺失值
> na_in_txt =
> " "
> ,
> \#若表格中的文本被视为缺失，在独立文本中被写成什么
> name_col =
> NULL
> ,
> \#CSV表格哪一列可用作文件名
> ext =
> "txt"
> \#独立文件是什么格式，只能是"txt"、"rtf"和""
> )
> .

> 四、chinese.misc-数据类型、格式转变

> 1、正则匹配：match_pattern 提取包含某个模式的字符

> match_pattern
> (pattern,
> where
> , vec_result =
> TRUE
> )
> \# vec_result为是否将结果转化成向量输出，默认是TRUE
> p
> <-
> "x.*?y"
> x
> <- c(
> "x6yx8y"
> ,
> "x10yx30y"
> ,
> "aaaaaa"
> ,
> NA
> ,
> "x00y"
> )
> y
> <- match_pattern(p, x)
> y
> <- match_pattern(p, x, vec_result =
> FALSE
> )
  \# 案例
> y
[
> 1
> ]
> "x6y"
> "x8y"
> "x10y"
> "x30y"
> "x00y"
> .

> 2、tf2doc：高级rep

> tf2doc (term, num)
> \#term是词语向量，list,数据库都可以
> 案例：

> \# 一个是矩阵，一个是因子，都没问题
> x
> <- matrix(c(
> "coffee"
> ,
> "milk"
> ,
> "tea"
> ,
> "cola"
> ), nrow =
> 2
> )
> y
> <- factor(c(
> 5
> :
> 8
> ))
> tf2doc
> (x, y)
> \# 结果
> > tf2doc (x, y)
[
> 1
> ]
> "coffee coffee coffee coffee coffee milk milk milk milk milk milk tea tea tea tea tea tea tea cola cola cola cola cola cola cola cola"
> .

> 3、m2doc

> m2doc (m, checks
> =
> FALSE)
> m是数值矩阵，案例：

> s <-
> sample(
> 1
> :
> 5
> ,
> 20
> , replace = TRUE)
> m <-
> matrix(s, nrow =
> 5
> )
> > m
     r
> text
> mining data
> [1,]
> 2    2      3    4
> [2,]
> 3    3      1    1
> [3,]
> 1    4      3    2
> [4,]
> 2    2      2    2
> [5,]
> 1    3      3    2
> colnames(m)
> <-
> c(
> "r"
> ,
> "text"
> ,
> "mining"
> ,
> "data"
> )
> m2doc(m)
> checks是是否检测m是正常输入，包括是否是数值，是否均是正数，是否无NA。为加快速度，可设为不检测，即FALSE，这也是默认值。

> .

> 4、两个逻辑判断：is_character_vector和is_positive_integer

> is_character_vector

> 判断对象是不是一个字符向量，且具有一定长度；is.character是最常用的判断是否是字符的函数，但是即使是对一个由字符组成的矩阵，它也仍然返回TRUE，因此并不能判断对象是否是一个向量。

> is_character_vector(
> character(
> 0
> )
> )
> is_character_vector(NA)
> is_character_vector(
> c(NA, NA)
> )
> is_character_vector(as.
> character(
> c(NA,NA)
> )
> )
> is_character_vector(as.
> character(
> c(NA, NA)
> )
> , allow_all_na = FALSE)
> is_character_vector(as.
> character(
> c(NA, NA)
> )
> , allow_all_na = TRUE)
> is_character_vector(
> c(
> "a"
> ,
> "b"
> ,
> "c"
> )
> , len =
> c(
> 1
> :
> 10
> )
> )
> is_positive_integer 这个函数用来判断对象是否是一个由有效正整数组成的符合一定长度的向量。

> is_positive_integer(NULL)
> is_positive_integer(as.
> integer(NA)
> )
> is_positive_integer(
> integer(
> 0
> )
> )
> is_positive_integer(
> 3.0
> )
> is_positive_integer(
> 3.3
> )
> is_positive_integer(-
> 1
> :
> 5
> )
> is_positive_integer(
> 1
> :
> 5
> , len =
> c(
> 2
> ,
> 10
> )
> )
> is_positive_integer(
> 1
> :
> 5
> , len =
> c(
> 2
> :
> 10
> )
> )
> .

> 5、升级版：as.character2：批量转化字符型向量

> 可一次转化多个对象，但更重要的是它的转化能力比as.character更强

> 可以转化列表，以及嵌套列表，即列表的列表、数据框、因子

> as
> .character2(
> NULL
> ,
> NULL
> )
null_list <-
> list
> (a =
> NULL
> , b =
> NULL
> , c =
> NULL
> )
> as
> .character2(null_list)
> \# 列表套数据框
> df <- data.frame(matrix(c(
> 66
> ,
> 77
> ,NA,
> 99
> ), nr =
> 2
> ))
l <-
> list
> (a =
> 1
> :
> 4
> , b = factor(c(
> 10
> ,
> 20
> ,NA,
> 30
> )), c = c(
> 'x'
> ,
> 'y'
> , NA,
> 'z'
> ), d = df)
> as
> .character2(l)
l2 <-
> list
> (l, l, cha = c(
> 'a'
> ,
> 'b'
> ,
> 'c'
> ))
> as
> .character2(l2)
> .

> 6、as.numeric2升级版：批量转化为数值型

> 因子、列表、数据框都可以

> a
> <- c(
> 55
> ,
> 66
> ,
> 77
> ,
> 88
> ,
> 66
> ,
> 77
> ,
> 88
> )
b <- factor(
> a
> )
df <- data.frame(
> a
> , b)
> as
> .numeric2(df,
> a
> *
> 2
> )
l <- list(
> a
> ,
> a
> *
> 2
> )
> as
> .numeric2(l)
l2 <- list(l, l)
> as
> .numeric2(l2)

