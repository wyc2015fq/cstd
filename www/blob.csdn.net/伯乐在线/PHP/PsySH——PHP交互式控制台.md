# PsySH——PHP交互式控制台 - 文章 - 伯乐在线
原文出处： [Vergil](http://vergil.cn/archives/psysh)
**PsySH**
PsySH是一个PHP的运行时开发平台，交互式调试器和Read-Eval-Print Loop (REPL)。
说的简单点，就像你用firebug的console调试你的JavaScript代码一样。
- [PsySH官网](http://psysh.org)
- [GitHub](https://github.com/bobthecow/psysh)
- [Packagist](https://packagist.org/packages/psy/psysh)
# 安装
官网介绍了3种安装方式：
- 直接下载
- Composer安装
- 直接cloneGitHub仓库的代码
我比较建议选择Composer安装，因为这个项目还有其他的依赖项目，用Composer很好解决这个问题。
以下教程以OS X和Windows 10为例，假定已经安装`php`和`Composer`并已经将它们设置为系统环境变量：
### OS X
先用Composer下载PsySH，这里用的是全局安装：

PHP
```
$ composer global require psy/psysh
```
安装完毕后，PsySH已经安装到`/Users/{用户名}/.composer/vendor/psy/psysh`目录下
此时，你可以直接运行：

PHP
```
$ /Users/{用户名}/.composer/vendor/psy/psysh/bin/psysh
```
为了使用方便，建议将它加入到环境变量：

PHP
```
$ echo 'export PATH="/Users/{用户名}/.composer/vendor/psy/psysh/bin:$PATH"' >> ~/.bashrc
$ source ~/.bashrc
```
### Windows
同样地，使用Composer安装，按win键+R键，输入cmd，打开windows控制台，然后：

PHP
```
composer global require psy/psysh
```
安装完成后，PsySH被安装到`C:Users{用户名}AppDataRoamingComposervendorpsypsysh`
因为bin/psysh文件并不是windows的可执行文件，所以需要使用以下命令运行PsySH

PHP
```
php C:Users{用户名}AppDataRoamingComposervendorpsypsyshbinpsysh
```
为了使用方便，在`C:Users{用户名}AppDataRoamingComposervendorpsypsyshbin`目录下新建一个名为`psysh.bat`的文件，其内容如下：

PHP
```
<a href="http://www.jobbole.com/members/echo">@ECHO</a> OFF
php "%~dp0psysh" %*
```
此时，把`C:Users{用户名}AppDataRoamingComposervendorpsypsyshbin`加入到系统的环境变量PATH，以后可以直接在cmd下运行`psysh`了：

PHP
```
C:UsersVergil>psysh
Psy Shell v0.6.1 (PHP 5.6.8 — cli) by Justin Hileman
>>>
```
# 特性
上面说过，PsySH是一个交互式的PHP运行控制台，在这里，你可以写php代码运行，并且可以清楚看到每次的返回值：
![](http://jbcdn2.b0.upaiyun.com/2016/03/d39d281e7f7de07c3b89c2b376ea7802.jpg)
并且，它很智能地知道你的代码是否已经结束
![](http://jbcdn2.b0.upaiyun.com/2016/03/0a9d778b38ed24064dd61e6f4408fd77.jpg)
## 自动完成
PsySH可以像控制台那样，按下两次``键自动补全，帮你自动完成变量名，函数，类，方法，属性，甚至是文件：
![](http://jbcdn2.b0.upaiyun.com/2016/03/8a4c1020256355eff6aa9e4acae2d783.jpg)
![](http://jbcdn2.b0.upaiyun.com/2016/03/55523180dd49df38ed8a1a5f86e1f95c.jpg)
![](http://jbcdn2.b0.upaiyun.com/2016/03/a7eed412a9e5cc1f4be3ff50e3643459.jpg)
## 文档
在运行时忘记参数怎么办？PsySH的文档功能可以让你即时查看文档。
PsySH的文档存放在`~/.local/share/psysh/`。（windows系统存放在`C:Users{用户名}AppDataRoamingPsySH`）
文档下载地址：
- **[English](http://psysh.org/manual/en/php_manual.sqlite)**
- [Brazilian Portuguese](http://psysh.org/manual/pt_BR/php_manual.sqlite)
- [Chinese (Simplified)](http://psysh.org/manual/zh/php_manual.sqlite)
- [French](http://psysh.org/manual/fr/php_manual.sqlite)
- [German](http://psysh.org/manual/de/php_manual.sqlite)
- [Italian](http://psysh.org/manual/it/php_manual.sqlite)
- [Japanese](http://psysh.org/manual/ja/php_manual.sqlite)
- [Polish](http://psysh.org/manual/pl/php_manual.sqlite)
- [Romanian](http://psysh.org/manual/ro/php_manual.sqlite)
- [Russian](http://psysh.org/manual/ru/php_manual.sqlite)
- [Persian](http://psysh.org/manual/fa/php_manual.sqlite)
- [Spanish](http://psysh.org/manual/es/php_manual.sqlite)
- [Turkish](http://psysh.org/manual/tr/php_manual.sqlite)
下载中文文档：

PHP
```
$ cd ~/.local/share 
$ mkdir psysh
$ cd psydh
$ wget http://psysh.org/manual/zh/php_manual.sqlite
```
OK，完成后重新打开PsySH
![](http://jbcdn2.b0.upaiyun.com/2016/03/dfb64ccf696ffedb7f4a2a014a48077e.jpg)
![](http://jbcdn2.b0.upaiyun.com/2016/03/6723b3f8375ec08a0e1b670bbb1d5eb9.jpg)
而且还自带分页功能（和`more`命令一样）
当然，自定义的对象也可以有相关功能：
比如，`test.php`这个文件有如下内容：
## 查看源代码
轻松展现任何用户级的对象，类，接口，特质，常数，方法或属性的源代码：
接着上一个例子：
![](http://jbcdn2.b0.upaiyun.com/2016/03/2dac99fa2d133aa652ce582b8585d94b.jpg)
![](http://jbcdn2.b0.upaiyun.com/2016/03/89df3d6599c8101c53f08d5470bbbed8.jpg)
查看show的帮助，使用`help show`命令：
![](http://jbcdn2.b0.upaiyun.com/2016/03/61277b46c684b82af61e251f5016a36f.jpg)
## 反射列表
`list`命令知道所有关于你的代码 – 和其他人的。轻松地列出并搜索所有的变量，常量，类，接口，特点，功能，方法和属性。
![](http://jbcdn2.b0.upaiyun.com/2016/03/b16054e687b5dd7b2bcceeaac0e8a39c.jpg)
![](http://jbcdn2.b0.upaiyun.com/2016/03/b57d6b8f5978db1a31e1dfc99d351ebf.jpg)
更多用法，可以输入`help list`命令查看。
## 获取最后的异常信息
如果忘记catch异常，可以使用`wtf`命令（wtf是what the fuck的意思么？）查看异常的信息：
![](http://jbcdn2.b0.upaiyun.com/2016/03/52bc8d96a6db8aef60609f405071447a.jpg)
## 历史记录
可以像类Unix系统的`history`命令一样，在PsySH可以查看你运行过的PHP代码或命令。详情运行`help history`命令查看。
## 退出
使用`exit`命令退出你的PsySH
## help
最好用的命令，`help`或者`?`，它能帮助你了解一切的命令
![28.png](http://jbcdn2.b0.upaiyun.com/2016/03/7752a0416d29c7ecdbb1c06c7707de79.jpg)
# 在内置web server中调试
本来想用Laravel做示例，但debugging的内容经常过多要分页查看。
而且说好的中文文章，好吧，那就用ThinkPHP为例：
首先，安装一下ThinkPHP
![16.png](http://jbcdn2.b0.upaiyun.com/2016/03/62159aa0dffd5734404bdee2c46747a0.jpg)
然后，用Composer局部安装一下PsySH:
![17.png](http://jbcdn2.b0.upaiyun.com/2016/03/7591aedb50e3409c20f919a1298d9abb.jpg)
因为ThinkPHP的`dump()`函数和`symfony/var-dumper`的`dump()`函数有重名冲突，所以需要把其中一个重命名。个人建议把TP的改掉。为啥？因为symfony的好用多了。。。
![19.png](http://jbcdn2.b0.upaiyun.com/2016/03/844e4f5f30be09ed3a047324cfdbc362.jpg)
在入口文件`index.php`添加如下3句代码：
![20.png](http://jbcdn2.b0.upaiyun.com/2016/03/65c7081901732e74a9ac8a38eae5f11f.jpg)
在项目根目录下，用PHP的内置web服务器启动：

PHP
```
php -S localhost:8080
```
然后即可在浏览器访问你的应用: http://localhost:8080
你将会看到以下画面，PsySH启动了
![21.png](http://jbcdn2.b0.upaiyun.com/2016/03/9d1046dbc061505fdb9d6398f7324f24.jpg)
其他操作，和CLI操作差不多。例如：

PHP
```
ls -al ThinkModel
```
![22.png](http://jbcdn2.b0.upaiyun.com/2016/03/98174a5b77deeb1c12db2131eaf457c0.jpg)

PHP
```
show \Think\Model::find
```
![23.png](http://jbcdn2.b0.upaiyun.com/2016/03/32990baf69755f9116f16351355254dc.jpg)

PHP
```
doc \Think\Model::save
```
![24.png](http://jbcdn2.b0.upaiyun.com/2016/03/0a7a31a60f486527433caf259deb48fa.jpg)
当然，那句`PsyShell::debug()`不一定要放在入口文件，它可以放在你一切想要debug的地方
例如，我放在默认的首页控制器：
![25.png](http://jbcdn2.b0.upaiyun.com/2016/03/8c978ef252c1922adc32e9a7766fe8c2.jpg)
重新运行php内置server并访问页面，然后输入命令：

PHP
```
ls -al
```
![26.png](http://jbcdn2.b0.upaiyun.com/2016/03/d1c81bfd013c4c34f13023f7978893b6.jpg)
打开PsySH后，输入get_defined_vars()试试？
![27.png](http://jbcdn2.b0.upaiyun.com/2016/03/5120b6a4d4587af27ba395fed134498a.jpg)
