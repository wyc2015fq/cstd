# 服务器数据库系列 - sphinx配置 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月30日 16:52:53[initphp](https://me.csdn.net/initphp)阅读数：867
个人分类：[服务器数据库系列](https://blog.csdn.net/initphp/article/category/2609107)










sphinx是以sphinx.conf为配置文件，索引与搜索均以这个文件为依据进行，要进行全文检索，首先就要配置好sphinx.conf，告诉sphinx哪些字段需要进行索引，哪些字段需要在where,orderby,groupby中用到。


安装完Sphinx后，在D:/sphinx目录有一个sphinx.conf.in，这个相当于sphinx的配置例子文件，我们以这个文件为蓝本，重新创建一个空白内容的sphinx.conf，存放在d:/sphinx根目录。


sphinx.conf的内容组成 


source 源名称1{

…

}

index 索引名称1{

source=源名称1

…

}

source 源名称2{

…

}

index 索引名称2{

source = 源名称2

…

}

indexer{

…

}

searchd{

…

}提示 


从组成我们可以发现sphinx可以定义多个索引与数据源，不同的索引与数据源可以应用到不同表或不同应用的全文检索。 


根据前面的实例，我们配置出我们需要的sphinx.conf，如下： 


source cgfinal

{

type = mysql

strip_html = 0

index_html_attrs =

sql_host = localhost

sql_user = root

sql_pass = admin

sql_db = test

sql_port= 3306 # optional, default is 3306

sql_query_pre=   SET NAMES utf8


sql_query = SELECT ARTICLESID,TITLE,CONTENTS,AUTHOR,CATALOGID,ADDTIME,EDITUSERID,\

HITS FROM a.eht_news_articles 

#sql_query = SELECT * FROM a.eht_news_articles 

sql_attr_uint= CATALOGID

sql_attr_uint= EDITUSERID

sql_attr_uint = HITS

sql_attr_timestamp = ADDTIME


sql_query_post   =

sql_ranged_throttle= 0

#sql_query_info = SELECT * FROM a.eht_news_articles WHERE ARTICLESID=$id

}

index cgfinal

{

source = cgfinal

path = d:/sphinx/data/cgfinal

docinfo = extern

mlock = 0

morphology = none

stopwords =

min_word_len   = 1

charset_type   = utf-8

charset_table = U+FF10..U+FF19->0..9, 0..9, U+FF41..U+FF5A->a..z, U+FF21..U+FF3A->a..z,\

A..Z->a..z, a..z, U+0149, U+017F, U+0138, U+00DF, U+00FF, U+00C0..U+00D6->U+00E0..U+00F6,\

U+00E0..U+00F6, U+00D8..U+00DE->U+00F8..U+00FE, U+00F8..U+00FE, U+0100->U+0101, U+0101,\

U+0102->U+0103, U+0103, U+0104->U+0105, U+0105, U+0106->U+0107, U+0107, U+0108->U+0109,\

U+0109, U+010A->U+010B, U+010B, U+010C->U+010D, U+010D, U+010E->U+010F, U+010F,\

U+0110->U+0111, U+0111, U+0112->U+0113, U+0113, U+0114->U+0115, U+0115, \

U+0116->U+0117,U+0117, U+0118->U+0119, U+0119, U+011A->U+011B, U+011B, U+011C->U+011D,\

U+011D,U+011E->U+011F, U+011F, U+0130->U+0131, U+0131, U+0132->U+0133, U+0133, \

U+0134->U+0135,U+0135, U+0136->U+0137, U+0137, U+0139->U+013A, U+013A, U+013B->U+013C, \

U+013C,U+013D->U+013E, U+013E, U+013F->U+0140, U+0140, U+0141->U+0142, U+0142, \

U+0143->U+0144,U+0144, U+0145->U+0146, U+0146, U+0147->U+0148, U+0148, U+014A->U+014B, \

U+014B,U+014C->U+014D, U+014D, U+014E->U+014F, U+014F, U+0150->U+0151, U+0151, \

U+0152->U+0153,U+0153, U+0154->U+0155, U+0155, U+0156->U+0157, U+0157, U+0158->U+0159,\

U+0159,U+015A->U+015B, U+015B, U+015C->U+015D, U+015D, U+015E->U+015F, U+015F, \

U+0160->U+0161,U+0161, U+0162->U+0163, U+0163, U+0164->U+0165, U+0165, U+0166->U+0167, \

U+0167,U+0168->U+0169, U+0169, U+016A->U+016B, U+016B, U+016C->U+016D, U+016D, \

U+016E->U+016F,U+016F, U+0170->U+0171, U+0171, U+0172->U+0173, U+0173, U+0174->U+0175,\

U+0175,U+0176->U+0177, U+0177, U+0178->U+00FF, U+00FF, U+0179->U+017A, U+017A, \

U+017B->U+017C,U+017C, U+017D->U+017E, U+017E, U+0410..U+042F->U+0430..U+044F, \

U+0430..U+044F,U+05D0..U+05EA, U+0531..U+0556->U+0561..U+0586, U+0561..U+0587, \

U+0621..U+063A, U+01B9,U+01BF, U+0640..U+064A, U+0660..U+0669, U+066E, U+066F, \

U+0671..U+06D3, U+06F0..U+06FF,U+0904..U+0939, U+0958..U+095F, U+0960..U+0963, \

U+0966..U+096F, U+097B..U+097F,U+0985..U+09B9, U+09CE, U+09DC..U+09E3, U+09E6..U+09EF, \

U+0A05..U+0A39, U+0A59..U+0A5E,U+0A66..U+0A6F, U+0A85..U+0AB9, U+0AE0..U+0AE3, \

U+0AE6..U+0AEF, U+0B05..U+0B39,U+0B5C..U+0B61, U+0B66..U+0B6F, U+0B71, U+0B85..U+0BB9, \

U+0BE6..U+0BF2, U+0C05..U+0C39,U+0C66..U+0C6F, U+0C85..U+0CB9, U+0CDE..U+0CE3, \

U+0CE6..U+0CEF, U+0D05..U+0D39, U+0D60,U+0D61, U+0D66..U+0D6F, U+0D85..U+0DC6, \

U+1900..U+1938, U+1946..U+194F, U+A800..U+A805,U+A807..U+A822, U+0386->U+03B1, \

U+03AC->U+03B1, U+0388->U+03B5, U+03AD->U+03B5,U+0389->U+03B7, U+03AE->U+03B7, \

U+038A->U+03B9, U+0390->U+03B9, U+03AA->U+03B9,U+03AF->U+03B9, U+03CA->U+03B9, \

U+038C->U+03BF, U+03CC->U+03BF, U+038E->U+03C5,U+03AB->U+03C5, U+03B0->U+03C5, \

U+03CB->U+03C5, U+03CD->U+03C5, U+038F->U+03C9,U+03CE->U+03C9, U+03C2->U+03C3, \

U+0391..U+03A1->U+03B1..U+03C1,U+03A3..U+03A9->U+03C3..U+03C9, U+03B1..U+03C1, \

U+03C3..U+03C9, U+0E01..U+0E2E,U+0E30..U+0E3A, U+0E40..U+0E45, U+0E47, U+0E50..U+0E59, \

U+A000..U+A48F, U+4E00..U+9FBF,U+3400..U+4DBF, U+20000..U+2A6DF, U+F900..U+FAFF, \

U+2F800..U+2FA1F, U+2E80..U+2EFF,U+2F00..U+2FDF, U+3100..U+312F, U+31A0..U+31BF, \

U+3040..U+309F, U+30A0..U+30FF,U+31F0..U+31FF, U+AC00..U+D7AF, U+1100..U+11FF, \

U+3130..U+318F, U+A000..U+A48F,U+A490..U+A4CF 

min_prefix_len   = 0

min_infix_len   = 1

ngram_len = 1


ngrams_chars = U+4E00..U+9FBF, U+3400..U+4DBF, U+20000..U+2A6DF, U+F900..U+FAFF,\

U+2F800..U+2FA1F, U+2E80..U+2EFF, U+2F00..U+2FDF, U+3100..U+312F, U+31A0..U+31BF,\

U+3040..U+309F, U+30A0..U+30FF, U+31F0..U+31FF, U+AC00..U+D7AF, U+1100..U+11FF,\

U+3130..U+318F, U+A000..U+A48F, U+A490..U+A4CF

}

indexer

{

mem_limit = 32M

}

searchd

{

# address = 0.0.0.0

port = 3312

log     = d:/sphinx/log/searchd.log

query_log = d:/sphinx/log/query.log

read_timeout   = 5

max_children   = 30

pid_file = d:/sphinx/log/searchd.pid

max_matches = 1000

seamless_rotate   = 1

}相关配置项说明： 


Source部分配置项说明


#type 数据库类型，目前支持mysql与pgsql

#strip_html 是否去掉html标签

#sql_host 数据库主机地址

#sql_user 数据库用户名

#sql_pass 数据库密码

#sql_db 数据库名称

#sql_port 数据库采用的端口

#sql_query_pre 执行sql前要设置的字符集，用utf8必须SET NAMES utf8

#sql_query   全文检索要显示的内容，在这里尽可能不使用where或group by，将where与groupby的内容交给sphinx，由sphinx进行条件过滤与groupby效率会更高

#注意：select 出来的字段必须至少包括一个唯一主键(ARTICLESID)以及要全文检索的字段，你计划原本在where中要用到的字段也要select出来

#这里不用使用orderby

#sql_attr_开头的表示一些属性字段，你原计划要用在where,orderby,groupby中的字段要在这里定义

#根据我们原先的SQL:

#select * from eht_articles where title like ? and catalogid=? And edituserid=?   And addtime between ? and ? order by hits desc

#我们需要对catalogid,edituserid,addtime,hits进行属性定义(这四个字段也要在select的字段列表中)，定义时不同的字段类型有不同的属性名称，具体可以见sphinx.conf.in中的说明


index部分配置项说明


#source 数据源名

#path 索引记录存放目录，如d:/sphinx/data/cgfinal,实际存放时会存放在d:/sphinx/data目录，然后创建多个cgfinal名称，不同扩展名的索引文件。

#其他的配置如min_word_len,charset_type,charset_table,ngrams_chars,ngram_len这些则是支持中文检索需要设置的内容。

#如果检索的不是中文，则charset_table,ngrams_chars,min_word_len就要设置不同的内容，具体官方网站的论坛中有很多，大家可以去搜索看看。5. 运行Sphinx


首先要对数据进行索引或重建索引 


进入命令行，运行d:/sphinx/bin/release/indexer –config d:/sphinx/sphinx.conf cgfinal 


如果您在sphinx.conf中配置了多个数据源，想一次性全部索引则d:/sphinx/bin/release/indexer –config d:/sphinx/sphinx.conf –all


如果只是想对某个数据源进行索引，则d:/sphinx/bin/release/indexer –config d:/sphinx/sphinx.conf 索引名称(这里的索引名称是你在sphinx.conf中定义的索引名称)


运行检索守护进程searchd 


进入命令行，运行d:/sphinx/bin/release/searchd –config d:/sphinx/sphinx.conf，此时系统会在3312端口侦听mysql的全文检索请求，所以如果您的mysql与sphinx不在同一台 机器，要保证3312端口不被防火墙阻隔。


6. 搜索（翻译） 


6.1. 匹配模式 


SPH_MATCH_ALL，匹配所有查询词（缺省模式） 


SPH_MATCH_ANY，匹配任意查询词 


SPH_MATCH_PHRASE，短语匹配 


SPH_MATCH_BOOLEAN，布尔表达式匹配 


SPH_MATCH_EXTENDED，查询匹配一个Sphinx内部查询语言表达式 


6.2. 布尔查询语法(Boolean query syntax) 


布尔查询允许使用下面特殊操作符： 


AND：hello & world 


OR：hello | world 


NOT：hello -world或hello !world 


Grouping：(hello world) 


举一个使用这些操作符的例子： 


( cat -dog ) | ( cat -mouse) 


AND是一个隐式操作符，“hello world”就相当于“hello & world”。 


OR的优先级高于AND，所以“looking for cat | dog | mouse”的意思是“looking for (cat | dog | mouse)”而不是“(looking for cat) | dog | mouse”


象“-dog”这种隐式地包含了所有查询记录，是不会被执行的。这主要是考虑到技术上与性能上的原因，从技术上来说，sphinx不能总保持所有文章的ID列表，性能上来说，当结果集巨大（10-100M），执行这样的查询将费耗较长时间。


6.3. 扩展查询语法(Extended query syntax) 


扩展查询允许合我下面特殊操作符： 


操作符OR：hello | world 


操作符NOT：hello -world或hello !world 


字段搜索操作符：@title hello @body world 


短语(phrase)搜索符：”hello world” 


临近(proximity)搜索符：”hello world”~10 


举例： 


“hello world” @title “example program”~5 @body python -(php|perl) 


AND是一个隐式操作符，”hello world”表示hello与world都要出现在匹配的记录中。 


OR的优先级高于AND，所以“looking for cat | dog | mouse”的意思是“looking for (cat | dog | mouse)”而不是“(looking for cat) | dog | mouse”


临近距离在串中标明了，主要是用来调整单词数量，应用在引号中的所有查询字串。”cat dog mouse”~5表示包括这三个单词在内，总共不能多于8个单词的间隔。比如”CAT aaa bbb ccc DOG eee fff MOUSE”就不能匹配这个查询，因为单词间隔刚好是8个。


象aaa | ( bbb ccc | ( ddd eee ) )这样的括号嵌套查询目前还不支持，但以后会修正的。 


否定（如NOT）只允许出现在顶层，不允许出现在括号内（如groups）。这点是不会改变的。因为支持否定嵌套查询会让短语排序(phrase ranking)的实现变得过于复杂。


6.4. 权重(匹配度，Weight) 


采用什么权重功能取决于搜索模式（Search mode） 


在权重函数中，有两个主要部分：（短语排名）phrase rank和statistical rank（统计排名） 


短语排名是基于搜索词在文档和查询短语中的最长公共子序列(LCS)的长度。所以如果在记录中有切确的短语匹配，记录的短语排名将有可能是最高的，等于查询单词的总个数。 


统计排名是建立在经典的BM25算法基础之上，它只考虑词频。词在全部文档集合中以低的频度出现或高频度出现在匹配的文档中，那么它获得的权重就越大，最终的BM25权重是一个介于0到1之间的小数。


好的子短语匹配得到好的排名,最好的匹配放到最顶端。Sphinx作者的经验是：基于排名的密切短语比其它任何单独的统计方式表现出较好的搜索质量。 


在SPH_MATCH_BOOLEAN 模式中，不需要计算权重，每条匹配记录的权重都是1 


在SPH_MATCH_ALL和SPH_MATCH_PHRASE模式中，最终的权重是短语排名权重的总和 


（TOFIX:翻译不畅）在SPH_MATCH_ANY模式中，本质上是一样的，但它也增加了每个字段的匹配单词数量，在这之前，短语排名权重乘以一个足够大的值以保证在任意一个字段的较高短语排名可以匹配排名较高者，即使它的字段权重比较低。


在SPH_MATCH_EXTENDED模式中，最终的权重是短语权重和BM25权重的总和，再乘以1000取整
            


