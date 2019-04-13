
# superset之安装与部署（Ubuntu16.10) - OraYang的博客 - CSDN博客

2017年09月27日 10:10:59[OraYang](https://me.csdn.net/u010665216)阅读数：1795所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 引言
Superset 是Airbnb的数据科学部门开源的一套工具，它是一个自助式数据分析工具，它的主要目标是简化我们的数据探索分析操作，它的强大之处在于整个过程一气呵成，几乎不用片刻的等待。Superset通过让用户创建并且分享仪表盘的方式为数据分析人员提供一个快速的数据可视化功能。现在，我们就来安装部署下这个工具，看看到底好不好玩。
## 安装
### 下载地址
superset的源码可以在[github](https://github.com/apache/incubator-superset)上找到。
### pip安装
这种安装方式，很简单，但是会出现 required permissions，可能是pip的源不是最新的，所以我们放弃pip安装方式，采用源码编译安装,**关键是，最新源码编译，支持语言选择啊，这样就不用汉化了哈~**
### 源码编译安装
按照这种方法，就不会出现任何问题。（如有问题，请私信~）
> step1.下载源码，并解压，进入文件目录

> $
> cd
> superset/assets/
> step2.安装yarn

> $ curl -sS https://dl
> .yarnpkg
> .com
> /debian/pubkey
> .gpg
> | sudo apt-key
> add
> -
echo
> "deb https://dl.yarnpkg.com/debian/ stable main"
> | sudo tee /etc/apt/sources
> .list
> .d
> /yarn
> .list
> $ sudo apt-get update && sudo apt-get install yarn
> step3.检查依赖库

> $
> yarn
> step4.如果出现nodejs版本问题，升级nodejs

> $
> sudo
> yarn cache clean
> -f
> $
> sudo
> yarn install -g n
$
> sudo
> n stable
> step5.编译安装依赖库

> $
> yarn run build
> step6.安装superset

> $
> cd ../..
> $
> python setup.py install
> step7.设置用户名与密码

> $ fabmanager
> create
> -admin
> --app superset
> step8.初始化数据库

> $
> superset db upgrade
> step9.导入样本案例

> $
> superset load_examples
> step10.创建默认规则与权限（这一步就会发现pip安装与源码编译的区别，源码编译会增加三个missing的权限）

> $
> superset init
> step11.启动服务

> $
> superset runserver
> step12.找个浏览器，在地址栏输入<主机ip>:8088

> 大功告成！

> 结果

> 登录界面
![这里写图片描述](https://img-blog.csdn.net/20170927100833431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> dashboard
![这里写图片描述](https://img-blog.csdn.net/20170927101014954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

