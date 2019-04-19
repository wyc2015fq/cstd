# Yii2的安装 - LC900730的博客 - CSDN博客
2016年12月06日 18:34:01[lc900730](https://me.csdn.net/LC900730)阅读数：221标签：[yii2-安装](https://so.csdn.net/so/search/s.do?q=yii2-安装&t=blog)
Yii2的安装 
1.首先需要安装好composer，并配置好composer的环境变量 
2.在需要安装的目录下，以D：www\test为例 
//安装在test目录里，命名为yii2 
composer create-project yiisoft/yii2=2.0.10 ./yii2  
出现的问题： 
bower-asset/jquery …… 
好几个问题都是由于这个asset问题造成的； 
packagist搜索bower出现yidas/yii2-composer-bower-skip,查看描述是A meta package that allows you to install yii2 without bower-asset (without composer-asset-plugin)，需要在安装yii2安装之前安装3.因此删除yii2目录，重现安装 
在test目录下，composer require yidas/yii2-composer-bower-skip=2.0.0  
然后composer install 
4.composer create-project yiisoft/yii2=2.0.10 ./yii2 
## 出现如下所示]([https://img-blog.csdn.net/20161206182744314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast](https://img-blog.csdn.net/20161206182744314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast))
此时是需要一个token认证，登录github，进入settings，点击OAUth application，点击register a new application，勾选repo生成一个token，粘贴到命令行下，认证通过后等待即可
