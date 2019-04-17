# 一小时学会开发一套学员管理系统demo - zhusongziye的博客 - CSDN博客





2018年03月03日 17:06:05[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1083











## 在pycharm中新建project demo1 添加app01 点击create按钮完成新建



## 在demo项目目录下新建目录static，并在settings.py中追加代码

`STATICFILES_DIRS``=``(os.path.join(BASE_DIR,  ``'static'``),)`

**在setting.py中添加模板路径：**



![](https://img-blog.csdn.net/20180303155846358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## 学员管理系统数据库设计：

在app01/model.py目录下建立 班级、老师、学生 、老师与班级关联表  四张表：



![](https://img-blog.csdn.net/20180303155910829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在终端Terminal 项目目录下执行数据表更新命令：
|12|`python manage.py makemigrations``python manage.py migrate`|
|----|----|
至此生成了四张数据表，可以在pycharm中，点开右上角的Database面板，然后将项目中templates目录下边的db.sqlite3鼠标拖拽到Database面板下，对新创建的数据表进行查看。





## 学员管理系统之班级管理：

为了方便分别操作班级、老师、学生相关的业务，将app01目录下的views.py 删掉，在app01目录下新建目录views，并在views目录下 新建classes.py teachers.py students.py。

### 1.在classes.py 中写 get_classes add_classes del_classes edit_classes四个函数，完成对 班级数据 的增删改查：

![](https://img-blog.csdn.net/20180303155954825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2.在urls.py 中配置url路由：　　

![](https://img-blog.csdn.net/20180303160008679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 3.在template目录下建立所需的html页面文件：

get_classes.html

![](https://img-blog.csdn.net/20180303160019793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

add_classes.html

![](https://img-blog.csdn.net/20180303160032589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


edit_classes.html



![](https://img-blog.csdn.net/20180303160045371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



## 学员管理系统之学员管理：

### 1.在students.py 中写 get_students add_students del_students edit_students 四个函数，完成对 学生数据 的增删改查：

![](https://img-blog.csdn.net/20180303160056418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 3.在template目录下建立所需的html页面文件：

get_students.html

![](https://img-blog.csdn.net/20180303160110594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

add_students



![](https://img-blog.csdn.net/20180303160122642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

edit_students.html




![](https://img-blog.csdn.net/20180303160137689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 学员管理系统之给班级分配老师：

在teachers数据表中增加一些老师信息：

　　在pycharm右上角的Database打开面板，然后将template目录下边的db.splte3鼠标拖入到Database面板中，打开db==》app01_teachers表

　　点击“+”，然后填入老师信息，然后点击有“DB”标志的向上箭头，进行数据保存。

### 1.在classes.py中增加set_teachers函数

![](https://img-blog.csdn.net/20180303160204564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2.在urls.py 中配置url路由：　

![](https://img-blog.csdn.net/20180303160219231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 3.在template目录下建立所需的html页面文件：

set_teachers.html

![](https://img-blog.csdn.net/20180303170258445?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对get_classes.html进行增添修改为：

![](https://img-blog.csdn.net/20180303170319918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 初识Ajax

Ajax是异步传输方式，偷偷的向后台发请求，不引起页面刷新，下面通过一个小例子来认识Ajax这种数据传输方式。

首先下载jQuery导入项目下的static目录下

### 1.在app01/Views目录下新建ajax.py

![](https://img-blog.csdn.net/20180303170338442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.在urls.py中配置相关路由

![](https://img-blog.csdn.net/20180303170351226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 3.在template目录下新建ajax1.html

![](https://img-blog.csdn.net/20180303170426416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 学员管理系统之Ajax删除学员：

### 1.在ajax.py中增加ajax4函数

![](https://img-blog.csdn.net/20180303170437780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.在urls.py中配置相关路由

![](https://img-blog.csdn.net/20180303170448565?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.对get_students.html进行添加修改：

![](https://img-blog.csdn.net/20180303170501361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)










## 在pycharm中新建project demo1 添加app01 点击create按钮完成新建

![](https://mmbiz.qpic.cn/mmbiz_gif/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWib59PAmc28UhpsJyRGjVCBIjWvC7QYaGF7W917R2UD0lrzjADOyTYHQ/640?wx_fmt=gif&tp=webp&wxfrom=5&wx_lazy=1)



## 在demo项目目录下新建目录static，并在settings.py中追加代码

`STATICFILES_DIRS``=``(os.path.join(BASE_DIR,  ``'static'``),)`

在setting.py中添加模板路径：



![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW8ic0v6BiaDsyQCQME2OlbPBasczRW3B5kk0OQpyB8TsUkTVojJTRLictA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


## 学员管理系统数据库设计：

在app01/model.py目录下建立 班级、老师、学生 、老师与班级关联表  四张表：



![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWZ7FojrotwWmy3qBtpAKuT1QIpNGhTPBZu8mmaOwVzsTznbGdtVzibJQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

在终端Terminal 项目目录下执行数据表更新命令：
|12|`python manage.py makemigrations``python manage.py migrate`|
|----|----|
至此生成了四张数据表，可以在pycharm中，点开右上角的Database面板，然后将项目中templates目录下边的db.sqlite3鼠标拖拽到Database面板下，对新创建的数据表进行查看。





## 学员管理系统之班级管理：

为了方便分别操作班级、老师、学生相关的业务，将app01目录下的views.py 删掉，在app01目录下新建目录views，并在views目录下 新建classes.py teachers.py students.py。

### 1.在classes.py 中写 get_classes add_classes del_classes edit_classes四个函数，完成对 班级数据 的增删改查：

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW2NmNGC8X8w6o3fs0FyiaT3SiceB3R2aQRub6ave1VXF3tW3picMQ20gsA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### 2.在urls.py 中配置url路由：　　

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWRBJ7lvRH0gtjSeRnSg46GeWbRSKHVHz8UUibCr8YfrqCPhFZ8iaM51rA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### 3.在template目录下建立所需的html页面文件：

get_classes.html

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWUyapV2xicqwY1vrX5jhfJK5kDAn0oKJL6ibHNEGqvORQttGlC1HZ2z3Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

add_classes.html

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW0A1PweqKuWTAaWFOaKq2jW4oIdKWUIX0w62Y430vFH0DK46cxZ1fPQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


edit_classes.html



![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWjX0DY0TDYIoaN5OUQia9s4UjD38P5aFrXiav76iaiaXv30ibCkvLQDkC4Xw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)



## 学员管理系统之学员管理：

### 1.在students.py 中写 get_students add_students del_students edit_students 四个函数，完成对 学生数据 的增删改查：

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW1yn2teiaNlxL8SZp16lv6PCfCdLfDw4TcIoVQnqJ27VVYr85hP1O8Sw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### 3.在template目录下建立所需的html页面文件：

get_students.html

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWSAHMoSjLnic9TEXrofNXfmicvTuGzDh8wmyoEia2s5w3SjEDPiarXO5oEw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

add_students



![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWE52A8m2ep8lQxK57TFHyeKPBNqLlxngShjriaaaCp6tJ3zcsU9qrhaQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

edit_students.html




![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW7E8ibOxq4awCC9cd1vCXBnTg9vUybJlWqrrpdRthOEjA1FNBia3otqww/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

## 学员管理系统之给班级分配老师：

在teachers数据表中增加一些老师信息：

　　在pycharm右上角的Database打开面板，然后将template目录下边的db.splte3鼠标拖入到Database面板中，打开db==》app01_teachers表

　　点击“+”，然后填入老师信息，然后点击有“DB”标志的向上箭头，进行数据保存。

### 1.在classes.py中增加set_teachers函数

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWh83a1DgKyHKdmgO8ZNszzOg41T4AM9rK82M6Xlrt5FiaD1yGaAUuLEQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### 2.在urls.py 中配置url路由：　

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW6zbicock8QIxKibkRicH5lbBxH7ibhWJ8EbsVZA9AHibrrIb7jKJbpciaHeQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

### 3.在template目录下建立所需的html页面文件：

set_teachers.html

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWcrYNbqicInLCz2WCJwHVjPKhIeMyYZssfctaiagUfrCBWYRbAdlPUib2A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

对get_classes.html进行增添修改为：

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWEe1PAPO1cibsNw5geV3bBEaOM3u4qreSaIgQoSWPZcd5sFrXCOQpiadQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

## 初识Ajax

Ajax是异步传输方式，偷偷的向后台发请求，不引起页面刷新，下面通过一个小例子来认识Ajax这种数据传输方式。

首先下载jQuery导入项目下的static目录下

### 1.在app01/Views目录下新建ajax.py

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWMH5QpE0kOhstoEHSKVJcPIeibfgBmtmBsHPQwWiaRUQRxy5E23oOPfMw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

## 2.在urls.py中配置相关路由

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWiau3dp7d27xg8tqrXpvibn5icnpUZGMVibRslVbv06ZKHamIe29kHQycRA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

 3.在template目录下新建ajax1.html

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWAzOvlEP4MbdMT7h6FhSFJLFndazLgoThY8K9picNiaV8asBoVkiatBPJg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

## 学员管理系统之Ajax删除学员：

### 1.在ajax.py中增加ajax4函数

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWwOOJXc9afvjD7jsYM5GHxYCbSXbjzzsIEafS9bm1x7R0xkBBP9b1IA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

## 2.在urls.py中配置相关路由

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIWOYxicaDzsjeasYkIuSLo5FicM6hrCNlZpiby1T4ttGJZSQqIt9fpoCLgA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

3.对get_students.html进行添加修改：

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n40TT60hNFb0BKkgRNwesIW4xSchkaiaBMj2q8hkpE1nFZiaxT3zqTlN9wTHzpI7oskDwrKhicwQpp0A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)







