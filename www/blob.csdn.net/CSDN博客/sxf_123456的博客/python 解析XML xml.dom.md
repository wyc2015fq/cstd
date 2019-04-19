# python 解析XML xml.dom - sxf_123456的博客 - CSDN博客
2017年08月05日 09:58:30[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：183
# [python 解析XML xml.dom](http://www.cnblogs.com/lhj588/archive/2011/11/09/2242483.html)
一 、xml.dom 解析XML的API描述
minidom.parse(filename)
加载读取XML文件
doc.documentElement
获取XML文档对象
node.getAttribute(AttributeName)
获取XML节点属性值
node.getElementsByTagName(TagName)
获取XML节点对象集合
node.childNodes #返回子节点列表。
node.childNodes[index].nodeValue
获取XML节点值
node.firstChild
#访问第一个节点。等价于pagexml.childNodes[0]
doc = minidom.parse(filename)
doc.toxml('UTF-8')
返回Node节点的xml表示的文本
Node.attributes["id"]
a.name #就是上面的 "id"
a.value #属性的值 
访问元素属性
二、代码演示
1、创建user.xml文件，添加XMl节点
```
<?xml version="1.0" encoding="UTF-8" ?>
<users>
<user id="1000001">
<username>Admin</username>
<email>admin@live.cn</email>
<age>23</age>
<sex>男</sex>
</user>
<user id="1000002">
<username>Admin2</username>
<email>admin2@live.cn</email>
<age>22</age>
<sex>男</sex>
</user>
<user id="1000003">
<username>Admin3</username>
<email>admin3@live.cn</email>
<age>27</age>
<sex>男</sex>
</user>
<user id="1000004">
<username>Admin4</username>
<email>admin4@live.cn</email>
<age>25</age>
<sex>女</sex>
</user>
<user id="1000005">
<username>Admin5</username>
<email>admin5@live.cn</email>
<age>20</age>
<sex>男</sex>
</user>
<user id="1000006">
<username>Admin6</username>
<email>admin6@live.cn</email>
<age>23</age>
<sex>女</sex>
</user>
</users>
```
2、Demo.py解析user.xml文档数据
```
# -*- coding:utf-8 -*-
"""
* User: lhj588
* Date: 11-11-9
* Time: 13：20
* Desc:
""" 
from  xml.dom import  minidom
def get_attrvalue(node, attrname):
return node.getAttribute(attrname) if node else ''
def get_nodevalue(node, index = 0):
return node.childNodes[index].nodeValue if node else ''
def get_xmlnode(node,name):
return node.getElementsByTagName(name) if node else []
def xml_to_string(filename='user.xml'):
    doc = minidom.parse(filename)
return doc.toxml('UTF-8')
def get_xml_data(filename='user.xml'):
    doc = minidom.parse(filename) 
    root = doc.documentElement
    user_nodes = get_xmlnode(root,'user')
    user_list=[]
for node in user_nodes: 
        user_id = get_attrvalue(node,'id') 
        node_name = get_xmlnode(node,'username')
        node_email = get_xmlnode(node,'email')
        node_age = get_xmlnode(node,'age')
        node_sex = get_xmlnode(node,'sex')
        user_name =get_nodevalue(node_name[0]).encode('utf-8','ignore')
        user_email = get_nodevalue(node_email[0]).encode('utf-8','ignore') 
        user_age = int(get_nodevalue(node_age[0]))
        user_sex = get_nodevalue(node_sex[0]).encode('utf-8','ignore') 
        user = {}
        user['id'] , user['username'] , user['email'] , user['age'] , user['sex'] = (
            int(user_id), user_name , user_email , user_age , user_sex
        )
        user_list.append(user)
return user_list
def test_xmltostring():
print xml_to_string()
def test_laod_xml():
    user_list = get_xml_data()
for user in user_list :
#print user['sex']
        print '-----------------------------------------------------'
if user:
            user_str='编   号：%d\n用户名：%s\n性   别：%s\n年   龄：%s\n邮   箱：%s\n ' % (int(user['id']) , user['username'], user['sex'] , user['age'] , user['email'])
print user_str
print '====================================================='
if __name__ == "__main__":
    test_xmltostring()
    test_laod_xml()
```
3、测试效果
  A、测试toxml
demo.py 文件中修改成if__name__ == "__main__":
    test_xmltostring() 执行打印结果：
```
<user id="1000001">
<username>Admin</username>
<email>admin@live.cn</email>
<age>23</age>
<sex>男</sex>
</user>
<user id="1000002">
<username>Admin2</username>
<email>admin2@live.cn</email>
<age>22</age>
<sex>男</sex>
</user>
<user id="1000003">
<username>Admin3</username>
<email>admin3@live.cn</email>
<age>27</age>
<sex>男</sex>
</user>
<user id="1000004">
<username>Admin4</username>
<email>admin4@live.cn</email>
<age>25</age>
<sex>女</sex>
</user>
<user id="1000005">
<username>Admin5</username>
<email>admin5@live.cn</email>
<age>20</age>
<sex>男</sex>
</user>
<user id="1000006">
<username>Admin6</username>
<email>admin6@live.cn</email>
<age>23</age>
<sex>女</sex>
</user>
```
B、测试解析XMLdemo.py 文件中修改成
   if __name__ == "__main__": 
        test_laod_xml()
执行打印出结果：
```
-----------------------------------------------------
编   号：1000001
用户名：Admin
性   别：男
年   龄：23
邮   箱：admin@live.cn
=====================================================
-----------------------------------------------------
编   号：1000002
用户名：Admin2
性   别：男
年   龄：22
邮   箱：admin2@live.cn
=====================================================
-----------------------------------------------------
编   号：1000003
用户名：Admin3
性   别：男
年   龄：27
邮   箱：admin3@live.cn
=====================================================
-----------------------------------------------------
编   号：1000004
用户名：Admin4
性   别：女
年   龄：25
邮   箱：admin4@live.cn
=====================================================
-----------------------------------------------------
编   号：1000005
用户名：Admin5
性   别：男
年   龄：20
邮   箱：admin5@live.cn
=====================================================
-----------------------------------------------------
编   号：1000006
用户名：Admin6
性   别：女
年   龄：23
邮   箱：admin6@live.cn
=====================================================
```
