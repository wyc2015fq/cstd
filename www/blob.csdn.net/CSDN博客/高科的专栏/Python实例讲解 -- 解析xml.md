
# Python实例讲解 -- 解析xml - 高科的专栏 - CSDN博客

2011年08月30日 12:32:53[高科](https://me.csdn.net/pbymw8iwm)阅读数：1217标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[xml																](https://so.csdn.net/so/search/s.do?q=xml&t=blog)[list																](https://so.csdn.net/so/search/s.do?q=list&t=blog)[encoding																](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[游戏																](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=file&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=list&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=list&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=python&t=blog)
Xml代码![收藏代码](http://justcoding.iteye.com/images/icon_star.png)
<?xmlversion="1.0"encoding="utf-8"?>
<info>
<intro>信息</intro>
<listid='001'>
<head>auto_userone</head>
<name>Jordy</name>
<number>12345678</number>
<age>20</age>
<sex>Man</sex>
<hobby>看电影</hobby>
</list>
<listid='002'>
<head>auto_usertwo</head>
<name>tester</name>
<number>34443678</number>
<age>18</age>
<sex>Man</sex>
<hobby>玩游戏</hobby>
</list>
</info>
main.py
Python代码![收藏代码](http://justcoding.iteye.com/images/icon_star.png)
\# -*- coding: utf-8 -*-
importxml.etree.ElementTree as et
\#读取xml文件
defload_xml_file(fileName):
root = et.parse(fileName).getroot()
\#获取文件描述
intro = root.find('intro').text.encode('gb2312')
printintro
\#获取所有list节点
all_users = root.findall('list')
\#遍历list节点的子元素
foruserinall_users:
\#得到head节点的文本
head = user.find('head').text
\#得到name节点的文本
name = user.find('name').text
\#得到sex节点的文本
sex = user.find('sex').text
\#得到age节点的文本
age = user[3].text
\#得到hobby节点的文本
hobby = user.find('hobby').text.encode('gb2312')
printhead,name,sex,hobby,age
if__name__ =='__main__':
load_xml_file('doc.xml')



