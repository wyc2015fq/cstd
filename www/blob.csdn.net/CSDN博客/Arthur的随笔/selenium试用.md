# selenium试用 - Arthur的随笔 - CSDN博客
2013年02月28日 14:32:16[largetalk](https://me.csdn.net/largetalk)阅读数：1399
个人分类：[杂七杂八																[python](https://blog.csdn.net/largetalk/article/category/715661)](https://blog.csdn.net/largetalk/article/category/823511)
作为一个小公司的程序员命有点苦阿，啥都要自己作。
最近网站有个文件下载功能需要测试，文件下载先需要登录，而且下载的url是计算出来的，所以手工方式测试太累了，只能用程序来实现了。虽然我知道用urllib2和cookielib能非常简单的解决这个问题，但看见selenium特别好玩，在界面上能自动的操作非常酷哈，就用selenium来解决这个问题了。
先是安装selenium:
```python
pip install selenium
```
然后selenium官网首页的例子已经基本包含我所需要功能了，所使用的操作不过是get, find_element_by_xx, 和send_keys, 
get就是http get请求， find_element_by_xx 就是通过某种方式查找DOM元素了， 而send_keys就是键盘操作了，很简单是吧。
如果事情永远是这么顺利，我就很开心了，也就不会有这篇文章了。
如我所说，我要测的是下载文件功能，但这个selenium没有提供好的测试方法，我只能走另一种思路： selenium自动下载文件到本地，然后去本地目录看该文件是否下载，由此来判断测试的成功与否。
selenium自动下载文件可看这篇文章： [http://seleniumcn.cn/read.php?tid=578](http://seleniumcn.cn/read.php?tid=578)
这边也记录一下：
1. edit -> preference 点击General 
然后 取消勾选Show
 the Downloads window when downloading a file 
save file to /tmp
2. 找一个可下载zip的网址，然后下载时选择
 Save File, 勾选Do
 this automatically for files like this from now on
3.
 copy firefox profile 目录(我的是~/.mozilla/firefox/zrxxxx.default)下 mimeTypes.rdf 和 pref.js到selenium profile目录下
4.
 vim mimeTypes.rdf 替换所有zip为你的文件后缀名(如egs) 。 这样基本就ok了
这里要说一下selenium profile目录，如果不特别指定的话，selenium每次会创建一个profile目录，为了一些特殊需要（如上面下载特殊格式文件），需要创建指定profile, 我创建profile目录的方法是直接拷贝系统的profile目录， 然后将firebug目录和extension目录下文件都删掉（我直接使用系统profile时出现过一些问题，删除这些目录之后可以了，原因未明）。
然后在代码这边指定一下：
```python
home_dir = os.path.expandvars('$HOME')
 profile_directory = os.path.join(home_dir, '.mozilla/firefox/selenium_profile/')
 ff_profile = webdriver.FirefoxProfile(profile_directory)
 browser = webdriver.Firefox(firefox_profile=ff_profile) # Get local session of firefox
```
that's all!

