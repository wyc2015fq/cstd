# 国产InitPHP框架系列 - InitPHP3.2发布了，可能是一个不错的PHP框架哦.. - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月19日 15:46:18[initphp](https://me.csdn.net/initphp)阅读数：1233








**InitPHP3.2版本主要修改如下：**

1. 修改数据库多库链接BUG

2. 数据库参数配置做过变动，如果升级框架，需要修改数据库DB配置

3. 修改全局变量注册BUG

4. 修改COOKIE BUG

5. 引进异常

6. 多库使用方法：$this->init_db('xxx') 来切换不同的数据库

7. 建议采用$this->init_db()的方法来调用数据库对象

8. 注册默认全局变量$this->common 可以调用Controller下的所有接口，该全局变量在Service和Dao层都可以调用




下载地址：[直接下载](http://initphp.com/down.php?id=1)

官网地址：[http://initphp.com](http://initphp.com)

QQ群：52718731





