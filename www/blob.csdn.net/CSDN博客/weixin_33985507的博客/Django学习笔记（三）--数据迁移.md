# Django学习笔记（三）--数据迁移 - weixin_33985507的博客 - CSDN博客
2017年10月25日 01:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
接着上节我们叙述[运行第一个Django项目](https://www.jianshu.com/p/b04b15673927)的内容，这篇文章我重点叙述关于Django连接数据库的问题，这里我使用的是Mac系统，Python的版本是系统默认的2.7.10版本，连接的数据库我选择的是：使用MySQL数据库，因为它既开源又免费。
## （一）安装数据库驱动
Django要连接MySQL数据库，首先我们必须要有连接数据库的驱动，因此我们既要安装好**数据库驱动**，并且也要安装好**数据库软件**，这样才能连接上数据库。现在我们默认大家都已经安装好数据库软件了，当然没有安装的，可以到[MySQL官方网站](https://link.jianshu.com?t=https://www.mysql.com/)上下载对应版本的**数据库软件**进行安装，下面我们就需要安装数据库驱动。
**Windows系统：**关于Windows系统的，请大家自行百度，因为我在开发中不喜欢用Windows系统，而且它没有Mac系统流畅。
**Mac系统：** 我们需要运行下面的命令：
```
pip install MySQL-python # 安装MySQL数据库库驱动
```
但是在运行的过程中，我们会遇到下面的错误：
```
mysql_config not found # 这是没有找到MySQL的安装路径
```
遇到这个错误的时候，我们可以有很多种解决的办法，我自己使用的是创建软连接的方式，即运行下面的命令即可：
```
sudo ln -s /Application/MAMP/Library/bin/mysql_config /usr/local/bin/mysql_config # 这是在/usr/local/bin目录下为mysql_config创建一个软连接
```
运行上面的命令就可以安装好数据库驱动。
**Linux系统：** 对于Linux系统来说，我们只需运行下面的命令即可。
```
sudo pip install MySQL-python #安装MySQL数据库驱动
```
之所以运行上面的命令不会报错，是因为我们在安装数据库软件的时候，已经默认在/usr/local/bin目录下面，为mysql_config创建了软连接了，运行结果如下图：
![2152073-55b131a3ca17cb4d.png](https://upload-images.jianshu.io/upload_images/2152073-55b131a3ca17cb4d.png)
Linux系统安装数据库驱动成功示意图
驱动安装成功，下面我们就执行数据迁移。
## （二）数据库迁移
在进行数据库的迁移之前，我们先要做点准备工作，当然是进行数据库信息的配置。我们在setting文件中设置如下图的配置：
![2152073-5d6d6227696d204d.png](https://upload-images.jianshu.io/upload_images/2152073-5d6d6227696d204d.png)
MySQL数据库的配置
具体的代码如下：
```
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.mysql',
        'NAME': "blog",
        'USER': "root",
        'PASSWORD': "root",
        'HOST': "127.0.0.1",
        'PORT': "3306",
    }
}
```
下面我们来运行一下数据库迁移的命令，就可以在数据库表中看到已经创建了很多的表，命令如下：
```
python manage.py migrate # 进行数据库的迁移
```
就可以看到创建了表的命令和之后创建完成之后表名，如下图：
![2152073-be713095d0528bb3.png](https://upload-images.jianshu.io/upload_images/2152073-be713095d0528bb3.png)
数据库迁移生成表
![2152073-2dacbc9a9d987302.png](https://upload-images.jianshu.io/upload_images/2152073-2dacbc9a9d987302.png)
迁移之后的表
这样的操作，我们只是执行了默认的已经安装的数据库的迁移，我们还可以使用创建模型的方式来进行迁移数据库。
### 创建模型
我们先要创建一个应用，然后在应用创建模型，运行下面的命令：
```
python manage.py startapp polls # 创建一个投票应用
```
然后我们就可以在目录中看到生成了一个polls的文件夹，如下图：
![2152073-33e2d5cfde2a1141.png](https://upload-images.jianshu.io/upload_images/2152073-33e2d5cfde2a1141.png)
应用的目录
进入到polls目录下，会有一个models.py的文件，我们需要编辑这个文件，具体步骤如下：
```
# 1. 在models文件中编辑下面的内容
class Question(models.Model):
    question_text = models.CharField(verbose_name="问题", max_length=200)
    pub_date = models.DateTimeField(verbose_name="发布时间")
class Choice(models.Model):
    question = models.ForeignKey(Question)
    choice_text = models.CharField(verbose_name="选项", max_length=200)
    votes = models.IntegerField(verbose_name="投票数", default=0)
# 2. 在mysite的文件夹下面编辑setting文件内容
INSTALLED_APPS = (
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    # 下面这个是新加入的投票的数据库迁移文件名称
    'polls',
)
# 3. 创建新的迁移
python manage.py makemigrations polls # 为投票应用创建迁移文件
# 4. 更新数据库
python manage.py migrate # 将新的迁移文件更新到数据库中
```
以上就是Django基本的数据库迁移操作，后面我们会继续进行讲解。欢迎指正。
