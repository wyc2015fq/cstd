
# 利用python解决mysql视图导入导出依赖的问题​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月22日 21:36:01[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：61


**[利用python解决mysql视图导入导出依赖的问题](http://cda.pinggu.org/view/26444.html)**
这篇文章主要给大家介绍了关于利用python解决mysql视图导入导出依赖的问题，文中通过示例代码介绍的非常详细，对大家学习或者使用python具有一定的参考学习价值，需要的朋友们下面随着小编来一起学习学习吧。
视图
视图是一个虚拟表（非真实存在），其本质是根据SQL语句获取动态的数据集，并为其命名，用户使用时只需使用名称即可获取结果集，并可以将其当作表来使用。
创建视图
创建一个名称为v1的视图，其功能就是查询color表中的所有数据
CREATE VIEW v1 AS SELECT * FROM color;
查看视图
使用视图时，将其当作表进行操作即可，由于视图是虚拟表，所以无法使用其对真实表进行创建、更新和删除操作，仅能做查询用。
select * from v1; -- 等于执行SELECT * FROM color
输出结果
+-----+--------+
| nid | title |
+-----+--------+
| 1 | red |
| 2 | yellow |
+-----+--------+
2 rows in set (0.00 sec)
修改视图
ALTER VIEW v1 AS SELECT * FROM color WHERE nid = 1;
删除视图
DROP VIEW v1;
引用
navicat是mysql可视化工具中最棒的，但是，在处理视图的导入导出方面，它是按照视图名称的字母顺序来处理的，若视图存在依赖，在导入过程中就会报错。这个问题一直困绕我，一度因为我使用docker来部署mysql而绕过了这个问题。最近不得不直面这个问题，因此，写了一个小工具来解决它。
整体思路
在mysql很容易查出所有视图和其定义，因此可以写一个视图导出工具，存储时对各视图的保存位置进行调整，处理好它们之间的依赖关系，被依赖的放前面，这样就解决了导入时的依赖问题。
获取视图信息
运行以下查询语句，就能获得该数据库中所有视图的信息。
select * from information_schema.VIEWS where TABLE_SCHEMA = DatabaseName
查询结果字段说明：
TABLE_NAME ： 数所库中视图名称
VIEW_DEFINITION ： 视图的定义代码，只有查询语句部分
DEFINER ： 视图定义（建立）者名称
SECURITY ： 安全级别
总之，所有视图的信息都在这个表中保存，我要完成任务，只需要TABLE_NAME和VIEW_DEFINITION就可以了。
算法描述
将查询结果放到dict中，视图名称为key；视图定义为value；
编写处理依赖关系的函数process_rely，输入参数中的rely_old为保存所有视图名称的数组；返回参数为按依赖关系调整顺序后的视图名称数组。之所以这样做，是一开始考虑到，依赖关系复杂时，可能一次迭代处理不好，需要递归调用或多次调用。
process_rely函数算法描述：
第一层循环，从rely_old中取一个视图名称
第二层循环，从dict中取出一个键值
若键值被第一层元素的定义所依赖
若键值还不在结果数组中
若第一层元素不在结果数组中
追加键值到结果数组中
第一层元素在结果数组中
将键值插入到第一层元素前
键值在结果数组中
第一层元素在结果数组中
查找各自在结果数组中的位置
若第一层元素在键值的后
将键值移动到第一层元素前
第二层循环结束时，若第一层元素还不在结果集中
将第一层元素追加到结果集中
返回结果集
上面的说明，是按python代码模式给出的。很幸运，算法一次就能将复杂的依赖关系处理好了。我在编写的过程中，刚开始依赖算法不完善时，通过多次迭代也能处理好复杂的依赖关系。因此，坚定了必胜的信心，完成了这个任务。
完整代码
import pymysql
conn = pymysql.connect(host='172.17.0.1', port=3306, user='root',
passwd='123456', db='database', charset='utf8mb4')
def process_rely(parmas={}, rely_old=[]):
_rely = []
_keys = list(parmas.keys())
for k in rely_old:
for bl in _keys:
if str(parmas[k]).find(bl) > -1:
if bl not in _rely:
if k not in _rely:
_rely.append(bl)
else:
i = _rely.index(k)
_rely.insert(i, bl)
else:
if k in _rely:
i = _rely.index(k)
j = _rely.index(bl)
if i < j:
del _rely[j]
_rely.insert(i, bl)
if k not in _rely:
_rely.append(k)
return _rely
cur = conn.cursor()
cur.execute('select TABLE_NAME, VIEW_DEFINITION from information_schema.VIEWS where TABLE_SCHEMA = %s ', 'database')
rs = cur.fetchall()
cur.close()
conn.close()
ps = {}
for al in rs:
ps['`' + al[0] + '`'] = al[1]
rely = process_rely(ps, list(ps.keys()))
\# rely = process_rely(ps, rely1)
file_object = open('view.sql', 'w')
for al in rely:
file_object.write('DROP VIEW IF EXISTS ' + al + ';\n')
file_object.write('CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`%` SQL SECURITY DEFINER VIEW ' + al +
' AS ' + ps[al] + ';\n\n')
file_object.close()
小结
思路要清晰，代码要一步步的向最终目标靠近，积跬步以至千里。在做这个工具时，一开始觉得很麻烦，依赖关系若是深层次的，可能一次处理不好，正因为采用的迭代的思想，最后才完成了一次迭代解决问题的完美结局。

