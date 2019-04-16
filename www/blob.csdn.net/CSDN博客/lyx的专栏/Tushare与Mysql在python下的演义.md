# Tushare与Mysql在python下的演义 - lyx的专栏 - CSDN博客





2016年07月01日 21:07:47[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：6460








        首先给大家介绍的是一个很强大的财经数据接口库，是专门为python准备的哦。不过唯一的缺点是有比较大的数据缺失，这个库就是Tushare财经数据接口，官网如下：

http://tushare.waditu.com/index.html

        做的相当不错，使用pandas的框架做的。熟悉pandas的同学可以迅速上手哦。

        这次我们就从这个接口里面获取一些东西之后存到数据库里面。一方面是学习使用Tushare（其实很明了，不用学习），另外一方面是学习用Python去操作Mysql数据库。

## 1.Mysql安装

        Mysql安装的教程网上有很多，这里就不说了，推荐大家下载msi的，双击就可以直接安装，不过是直接装在c盘下的。

        装好Mysql之后呢，会在目录下有一个配置文件，这个很重要。默认名字叫做defaut-my.ini，打开之后我们做一些修改，


[mysql]

default-character-set = utf8

[mysqld]

character-set-server = utf8

        增加这样两个部分。这两个是干什么的呢？就是修改客户端和服务器端的字符编码，免得到时候因为字符编码的事情大费周章。

        结束了安装之后，我们开启Mysql服务，只有开启了这个服务，那么我们的python才能连上数据库。

## 2.Mysql开启

        打开dos窗口，输入 net start mysql（这个部分根据安装的时候的命名来）， 然后就会提示正在打开服务。打开之后，我们登录进去，mysql -u root -p 你的密码

        然后输入\s，如果发现和下图一样有四个utf8，那么就说明配置文件起作用了，咱们的字符编码改成功了。

![](https://img-blog.csdn.net/20160701204013322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 3、Tushare获取股票数据。

        这里我们就先介绍一下如何用Tushare获取沪深300指数成分股的代码，名称和权重吧。安装Tushare很简单，和一般的python包安装一样

pip install tushare

就可以安装好了。



```python
import tushare

print(tushare.__version__)
```
        这样就可以查看当前Tushare的版本号了。





```python
import tushare as ts
hs300 = ts.get_hs300s()
```
        这样，hs300就是一个包含沪深300成分股信息的一个pandas中dataframe数据结构的变量了。

        大家可以print 出来看一下。



## 4、存入Mysql



```python
try:
        conn=MySQLdb.connect(user='root')#cennect the database
        cur=conn.cursor()#get the cur
        cur.execute('create database if not exists Stock')
        conn.select_db('Stock')
        cur.execute('create table if not exists hs300(code varchar(10),weight integer)')
        hs300 = ts.get_hs300s()#get all the data and other data will be add to this d
        for cnt in range(0,len(hs300)):  #将hs300中的数据存储到数据库中      
            SQL = 'INSERT INTO hs300 (code,weight) VALUES (%s,%s)'%(hs300['code'][cnt],float(hs300['weight'][cnt]))
            cur.execute(SQL)    
        conn.commit() #执行上诉操作  
        cur.close()
        conn.close()
  
    except MySQLdb.Error,e:
        print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```
        注意，将数据插入的sql语句不是一运行就开始执行，而是要执行
```python
conn.commit()
```
之后才会生效。



        对于读取数据库的数据，

```python
SQL = 'SELECT code FROM hs300'
        cur.execute(SQL)
        results = cur.fetchall()
```
sql2.0数据库接口的标准是执行完sql语句之后，从cur之中fetch数据，在python中，往往是tuple的格式。




5.DB API 2.0标准


        如果没有接触过Mysql，或者在别的地方使用过的话，可能对DB API 2.0标准并不了解。

        无论是python、C++、java还是C#，只要是支持这个标准接口的，对数据库的操作其实是大同小异的。

        DB API 2.0想要快速入门，只要知道两种对象，一种就是连接对象，一种是游标对象，就是上面出现的conn和cur。conn是由于与数据库连接而产生的，他有四种方法
- cursor（）用于返回游标对象
- commit（）事物提交，sql语句真正被执行
- rollback（）回滚语句，前面的sql语句不执行
- close（）关闭连接。

        其实大家可以发现和操作文件对象很像。

        由连接对象产生的游标对象则复杂一些，他负责完整具体的操作任务，执行具体的sql语句。

        常用的就是之前我们提到的excute（sql语句），以及对其的封装excutemany（sqlstr，seq），这个其实通常通过高级语言自己写code实现。

        还有就是fetch类，可以是fetchone（）获取一行的结果数据，也可以是fetchall（）返回所有结果记录，就像上面说的那样。




