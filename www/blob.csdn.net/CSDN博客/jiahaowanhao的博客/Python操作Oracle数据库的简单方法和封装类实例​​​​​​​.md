
# Python操作Oracle数据库的简单方法和封装类实例​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月16日 20:53:47[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：106


**[Python操作Oracle数据库的简单方法和封装类实例](http://cda.pinggu.org/view/26387.html)**
文实例讲述了Python操作Oracle数据库的简单方法和封装类。分享给大家供大家参考，具体如下：
最近工作有接触到Oracle，发现很多地方用Python脚本去做的话，应该会方便很多，所以就想先学习下Python操作Oracle的基本方法。
考虑到Oracle的使用还有一个OracleClient的NetConfig的存在，我觉得连接起来就应该不是个简单的事情。
果然，网上找了几个连接方法，然后依葫芦却画了半天，却也不得一个瓢。
方法1：用户名，密码和监听分别作为参数
conn=cx_Oracle.connect('用户名','密码','数据库地址:数据库端口/SID')
根据我看的几篇文章和我写代码出错的提示，我找到了python连接数据库的配置项应该和Oracle客户端的配置文件tnsnames.ora有关。但是我的配置项并没有SID项，而且一开始我也不知SID是个什么东西，网上写什么我就跟着写，所以这种方法失败了。后来想通了要在配置项加一个SID，然后想想这个东西配置完了我系统是不是要重启啊。所以，先看看其他方法吧….
方法2：用户名，密码和监听共同作为一个参数
conn=cx_Oracle.connect('用户名/密码@数据库地址:数据库端口/SID')
这个方法基本和方法一一样，换汤不换药…
方法3：使用tns配置信息
conn=cx_Oracle.connect('用户名','密码',tns)
网上的代码获取tns是用函数做的，依然会使用SID，但是..我已经可以用的配置项并没有SID啊，所以使用
tns=cx_Oracle.makedsn('数据库地址','数据库端口', 'SID')
依旧不行，但是看看这个tns的生成方法和上面的两种方法差不多的。但是我发现我随便输入一个SID后生成的数据是这样的。
(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521)))(CONNECT_DATA=(SID=XE)))
然而，我的客户端的配置项大概是这样的，
(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=KGDB)))
我擦，好像长得一样的，类型也都是字符串类型的，试试直接把我文件里的配置项赋值给tns试试。
tns = '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=KGDB)))'
conn = cx_Oracle.connect('nicker', '123456', tns)
嗯。成功了~
最后，贴一个基本使用方法全代码
\#coding:utf-8
import cx_Oracle
\# 创建数据库连接
\# cx_Oracle.connect('username','pwd','ora的tns信息')
\# oracle数据库的tns信息，从tnsnames.ora中找到plsql可用的配置项，将该配置项直接拷贝过来即可
ora_tns = '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=KGDB)))'
conn = cx_Oracle.connect('nicker', '123456', ora_tns)
\# 操作游标
cursor = conn.cursor()
\# 执行查询
cursor.execute("SELECT * FROM inst_info")
\# 获取返回信息
rs = cursor.fetchall()
\# 输出信息
for v in rs:
print v
\#关闭连接，释放资源
cursor.close()
conn.close()
观察发现总结很重要，理解也需要
贴上一个自己封装Oracle的类
\#coding:utf-8
import cx_Oracle
\# 封装的类
class cxOracle:
'''
tns的取值tnsnames.ora对应的配置项的值，如：
tns = '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=10.16.18.23)(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=MYDB)))'
'''
def __init__(self ,uname, upwd,tns ):
self ._uname = uname
self ._upwd = upwd
self ._tns = tns
self ._conn = None
self ._ReConnect()
def _ReConnect(self ):
if not self._conn :
self ._conn = cx_Oracle.connect (self. _uname, self ._upwd, self._tns)
else:
pass
def __del__(self ):
if self. _conn:
self ._conn. close()
self ._conn = None
def _NewCursor(self ):
cur = self. _conn.cursor ()
if cur:
return cur
else:
print "\#Error\# Get New Cursor Failed."
return None
def _DelCursor(self , cur):
if cur:
cur .close()
\# 检查是否允许执行的sql语句
def _PermitedUpdateSql(self ,sql):
rt = True
lrsql = sql. lower()
sql_elems = [ lrsql.strip ().split()]
\# update和delete最少有四个单词项
if len( sql_elems) < 4 :
rt = False
\# 更新删除语句，判断首单词，不带where语句的sql不予执行
elif sql_elems[0] in [ 'update', 'delete']:
if 'where' not in sql_elems :
rt = False
return rt
\# 导出结果为文件
def Export(self , sql, file_name, colfg ='||'):
rt = self. Query(sql )
if rt:
with open( file_name, 'a') as fd:
for row in rt:
ln_info = ''
for col in row:
ln_info += str( col) + colfg
ln_info += '\n'
fd .write( ln_info)
\# 查询
def Query(self , sql, nStart=0 , nNum=- 1):
rt = []
\# 获取cursor
cur = self. _NewCursor()
if not cur:
return rt
\# 查询到列表
cur .execute(sql)
if ( nStart==0 ) and (nNum==1 ):
rt .append( cur.fetchone ())
else:
rs = cur. fetchall()
if nNum==- 1:
rt .extend( rs[nStart:])
else:
rt .extend( rs[nStart:nStart +nNum])
\# 释放cursor
self ._DelCursor(cur)
return rt
\# 更新
def Exec(self ,sql):
\# 获取cursor
rt = None
cur = self. _NewCursor()
if not cur:
return rt
\# 判断sql是否允许其执行
if not _PermitedUpdateSql(sql ):
return rt
\# 执行语句
rt = cur. execute(sql )
\# 释放cursor
self ._DelCursor(cur)
return rt
\# 类使用示例
tns = '(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=10.16.17.46)(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=MYDB)))'
ora = cxOracle ('nicker', '123456', tns)
\# 导出结果为文件
rs = ora .Export("SELECT * FROM org", '1.txt')
\# 查询结果到列表
rs = ora.Query("SELECT * FROM org")
print rs
\# 更新数据
ora.Exec("update org set org_name='NewNameForUpdate' where org_id=123456;")

