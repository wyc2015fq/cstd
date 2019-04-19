# Python使用中错误(持续更新.....) - _天枢 - 博客园
## [Python使用中错误(持续更新.....)](https://www.cnblogs.com/yhleng/p/7526992.html)
2017-09-15 16:13 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7526992)
**1、在使用requests发送请求，响应数据转成json提示，没有可解码的json**
**解决办法：**
**这是由于，发送请求的数据，存在错误，响应出错比如404 400，所以找不到可以解码的json**
**示例如下：**
```
postdata = '''{'userName':'13718651996',
            'passwd':'yhlxxxx870120',
            'validateCode':'abc',
            'rememberMe':'true'}'''
url = 'https://secure.elong.com/passport/ajax/elongLogin'
response = Session().post(url,data=postdata)
print(response.json())
```
**其实从图上很明显能看出问题，因为postdata是字符串类型，不是dict字典**
**如上图代码如果执行报如下错误：**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170915155920172-1115138342.png)
**解决办法，eval()函数将字符串，转换成字典；如下所示**
```
postdata = '''{'userName':'13718651996',
            'passwd':'yhlxxxx870120',
            'validateCode':'abc',
            'rememberMe':'true'}'''
url = 'https://secure.elong.com/passport/ajax/elongLogin'
request = Session().post(url,data=**eval(postdata))**print(request.json())
```
**有人可能会说，你这不是多此一举吗？把postdata直接定义成字典不就行了；你说的没错确实可以这样**
```
postdata = {'userName':'13718651996',
            'passwd':'yhlxxxx870120',
            'validateCode':'abc',
            'rememberMe':'true'}
url = 'https://secure.elong.com/passport/ajax/elongLogin'
request = Session().post(url,data=postdata)
print(request.json())
```
**但是如果以下这串数据，存储在数据库中，或者excel中，读出来的时候默认就是字符串，如果读出来直接使用就会出现本文的错误，**
**所以哪里错了这个要知道，这是一个小细节；****调试输出，发现输出跟以下是一样的，**
**没有错呀这是字典呢{'xxxx':'xxx'..........}，但是它就是字符串。容易被忽略，所以还要使用eval(postdata)函数转成字典**
```
{'userName':'13718651996',
            'passwd':'yhlxxxx870120',
            'validateCode':'abc',
            'rememberMe':'true'}
```
**正确代码：**
```
postdata = '''{'userName':'13718651996',
            'passwd':'yhlxxxx870120',
            'validateCode':'abc',
            'rememberMe':'true'}'''
url = 'https://secure.elong.com/passport/ajax/elongLogin'
request = Session().post(url,data=**eval(postdata)**)
print(request.json())
```
**－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－我是华丽的分隔线－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－**
**2、join拼接路径**
**os.path.join拼接路径，遇到的问题，经常会把自己绕进去**
```
str = r'd:\PHJ\Reporter\LOGS'
#报告文件路径
reporPath = os.path.join(str,'report')
isExistPath(reporPath) #创建文件夹
print(reporPath)
imgPath = os.path.join(reporPath,'\\Image')
print(imgPath)
```
错误输出输出：
```
d:\PHJ\Reporter\LOGS\report
d:\Image
```
我们期待结果：
```
d:\PHJ\Reporter\LOGS\report
d:\PHJ\Reporter\LOGS\report\Image
```
是不是很奇怪，问题到底出在哪里，这个问题让我纠结了好久，一个不太注意的小问题。
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170920163611040-901480421.png)
**没错，我们就是拼接的时候，多了两个返斜扛。去掉后问题解决。**
**3、写完项目，直接在计算机中双击py文件，报文件中引用的包找不到**
**项目结构，要执行run文件夹下的run.py ；报找不到run文件中引用的包**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170926145955182-2119842309.png)
**原因：双击执行的时候，不会自动向上搜索包，只会在当前文件夹及子文件夹中搜索**
**那么怎么办呢，把执行文件，在项目根目录下建个run.py做为执行入口，问题解决。**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170926150342604-1715388922.png)
你可能会说了，这样做，我一些引用路径的地方不对了，那我估计是获取路径的方式是
os.getcur  获取当前路径，这样一来，你每个文件用的路径都是当前引用文件所在的路径
所以建议起个gl文件，用来存储路径。等信息。
os.path.abspath(os.path.dirname(__file__)) 用这个获取当前路径，而不要用os.getcur
否则，即使你把获取路径这些，配置到gl也就是单独一个py中，那么不同位置调用。路径也会不同。
