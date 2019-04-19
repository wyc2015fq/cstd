# Mysql_real_query报错表示不同步 - 三少GG - CSDN博客
2013年11月08日 23:39:57[三少GG](https://me.csdn.net/scut1135)阅读数：1161
[Mysql_real_query报错表示不同步](http://ganlu.name/blog/archives/mysql_real_query%E6%8A%A5%E9%94%99%E8%A1%A8%E7%A4%BA%E4%B8%8D%E5%90%8C%E6%AD%A5)
这两天写内核的时候碰到一个问题,主要是原因藏得很隐蔽,让我好一顿找,于是mark一下.
在添加一个模块时调用了mysql_real_query函数,我勒个去一个劲地报错,代码前后检查不下5次,跟以前写的米啥两样啊,于是去google半天,错误原因是mysql_store_result()这个东西没清空,大概如此,但为虾米会没清空呢?我仔细检查过,在每个mysql_store_resule后面我都有用mysql_free_result的啊,调试加google得出的结论是,当调用一个存储过程或者SQL语句的时候,如果返回值是一个空记录集,尽管row数为零,但是mysql_store_result()的返回值并不为NULL!
 也就是说继续执行下去了,但是这样子继续执行不知为何就是会出错?原因不得而知,苦搜半天未果T_T.
比如我原先是这么写的.
[?](http://www.ericbess.com/ericblog/2008/03/03/wp-codebox/#examples)[Copy to
 clipboard]View Code CPP
|12345678|```cppif( res = mysql_store_result(&Mysql) ){	while( row = mysql_fetch_row( res ) )	{               .....        }        mysql_free_result(res);}```|
改成
[?](http://www.ericbess.com/ericblog/2008/03/03/wp-codebox/#examples)[Copy to
 clipboard]View Code CPP
|12345678|```cppif((res = mysql_store_result(&Mysql)) && **mysql_num_rows(res) > 0 **){	while( row = mysql_fetch_row( res ) )	{               .....        }        mysql_free_result(res);}```|
问题解决了 =,=..
而我在存储过程中判断如果为空记录集则返回NULL,也可行.
