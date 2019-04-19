# 作为PHP开发者请务必了解Composer - 文章 - 伯乐在线
原文出处： [虞大胆](http://www.jianshu.com/p/98c5b254a79e)
Composer是一个非常流行的PHP包依赖管理工具,已经取代PEAR包管理器,对于PHP开发者来说掌握Composer是必须的.
对于使用者来说Composer非常的简单,通过简单的一条命令将需要的代码包下载到`vendor`目录下,然后开发者就可以引入包并使用了.
其中的关键在于你项目定义的`composer.json`,可以定义项目需要依赖的包(可能有多个),而依赖的包可能又依赖其他的包(这就是组件的好处),这些都不用你烦心,Composer会自动下载你需要的一切,一切在于`composer.json`的定义.
Composer对于使用者来说是很透明,但是其背后的理念还是需要了解一下的,其的诞生也不是偶然的,得益于Github的快速发展,PHP语言也越来越现代化,显得更高大上了.
为了理解Composer,先大概了解下其结构:
#### Composer的结构
- **Composer命令行工具**:
这个理解就比较简单了,通过使用者定义的`Composer.json`去下载你需要的代码,假如只是简单的使用Composer,那么掌握一些具体命令就完全可以了
- **Autoloading代码加载器**:
通过Composer,开发者可以通过多种方式去使用,而其中的关键在于PHP的命名空间概念,以及PSR-4标准的发展,Composer只是根据这二者开发了一个代码自动加载器
- **Github**:
有了Github,PHP开发人员可以将开源的代码托管在这上面,而Composer的发展源于Github,Composer本质上就是将Github上的代码下载到本地.
- **Packagist**:
对于使用者来说使用的是Composer的命令行工具,那么命令行工具怎么知道有多少包可以被用户使用呢,这主要就是依赖于Packagist,Packagist是Composer主要的一个包信息存储库,包开发者将具体代码托管到Github上,将包信息提交到Packagist上,这样使用者就可以通过Composer去使用.
Composer根据本地定义的`composer.json`信息去查询Packagist,Packagist根据`Composer.json/Package.json`信息解析,最终对应到github仓库,Composer最终下载代码的时候还要依赖于Github仓库上的`Composer.json`,这里涉及到三种类型的`composer.json`,含义是不一样的.
- **Composer.json**:
这是Composer的核心,是Composer的规则,上面也提到了三种类型的`Composer.json`,在使用的时候一定要注意区分,我初学的时候就总是搞乱.
#### Composer命令行工具
**composer init**
使用者可以在自己的项目下创建`composer.json`以便定义你项目的依赖包,也可以通过`composer init`交互式的创建`composer.json`.
**composer install**
应该是最常用的命令,composer会根据本地的`composer.json`安装包,将下载的包放入项目下的`vendor`目录下,同时将安装时候的包版本信息放入到`composer.lock`,以便锁定版本.
其实在install的时候,假如发现`composer.lock`版本和目前`vendor`目录下的代码版本是一致的,则Composer会什么也不做,`composer.lock`的目的就是让你安心在目前这个版本下工作,而不获取最新版本的包.
**composer update**
那么如何更新`composer.lock`以便获取到最新版本的包呢?通过这个命令即可更新最新版本的包
**composer config**
这个命令还是建议了解下,全局的配置保存在`COMPOSER_HOME/config.json`,非全局的配置信息则存储在本项目目录下.


```
composer config --list -g
composer config -g notify-on-install false
composer global config bin-dir --absolute
```
**composer create-project**
这个命令不常用,但是个人觉得还是很重要的,使用普通的install命令是将项目所有的依赖包下载到本项目`vendor`目录下.而通过这个命令则是将所有的代码及其依赖的包放到一个目录下,相当于执行了一个`git clone`命令,一般是包的开发者可能为了修复bug会使用该命令.
**composer global**
这是一个全局的安装命令,它允许你在`COMPOSER_HOME`目录下执行Composer的命令,比如`install`,`update`.当然你的`COMPOSER_HOME`要在`$PATH`环境下.
比如执行`composer global require fabpot/php-cs-fixer`,现在php-cs-fixer命令行可以全局运行了,如果稍后想更新它,只需要运行`composer global update`
**composer dump-autoload**
当你修改项目下的`composer.json`的文件,并不一定要运行`composer update`命令进行更新,有的时候可以使用该命令来更新加载器,比如你要引用本地自定义的包(不是来自于packagist),后面会通过实践来说明该命令.
**composer require**
假如手动或者交互式创建`composer.json`文件,可以直接使用该命令来安装包


```
composer require  cerdic/css-tidy:1.5.2
composer require "ywdblog/phpcomposer:dev-master"
```
**–prefer-source和–prefer-dist参数**
–prefer-dist:对于稳定的包来说,一般Composer安装默认使用该参数,这也能加快安装,比如有可能直接从packagist安装了相应的包,而不用实际去Github上下载包.
–prefer-source:假如使用该参数,则会直接从Github上安装,安装包后`vendor`目录下还含有`.git信息`


```
composer require "ywdblog/phpcomposer:dev-master" --prefer-source 
#在vendor/ywdblog/phpcomposer目录下含有.git信息
```
**如何给Composer添加代理**
在国内使用Composer下载特别慢,可以通过二个方法进行加速
- composer config repo.packagist composer “[https://packagist.phpcomposer.com](https://packagist.phpcomposer.com)“
- 编辑composer.json


```
"repositories": {
  "packagist": {
      "type": "composer",
      "url": "https://packagist.phpcomposer.com"
  }
}
```
#### Autoloading代码加载器
composer本身集成一个`autoloader`,支持`PSR-4`,`PSR-0`,`classmap`,`files autoloading`.
这里通过一个例子来说明通过Composer如何引用`classmap`,`files`,`本地符合PSR-4标准的代码`
- 编辑[composer.json](https://github.com/ywdblog/mycode/blob/master/composer/composer.json)


```
"autoload": {
  "classmap": ["othsrc/","classsrc.php"],
  "files": ["othsrc/filesrc.php"],
  "psr-4": {"Foo\Bar\": "src"} 
}
```
- composer dump-autoload
通过上述的操作,对于PSR-4来说等同注册了一个PSR-4 autoloader(从`FooBar`命名空间)
假如不想使用Composer的autoloader,可以直接包含`vendor/composer/autoload_*.php` 文件,配置自己的加载器.
具体的例子托管在[github](https://github.com/ywdblog/mycode/tree/master/composer)上,可参考.
#### Repositories
关于Repositories,了解其不是必须的,但是假如掌握则更能理解Composer,对于Repositories,其[中文文档](http://docs.phpcomposer.com/05-repositories.html)和[英文文档](https://getcomposer.org/doc/05-repositories.md)解释的很好,这里也进行了一些**摘抄**.
**基本概念**
包:
Composer是一个依赖管理工具,它在本地安装一些资源包和包的描述(比如包名称和对应的版本),比较重要的元数据描述是`dist`和`source`,`dist`指向一个存档,该存档是对一个资源包的某个版本的数据进行的打包.`source`指向一个开发中的源,这通常是一个源代码仓库(比如git)
资源库:
一个资源库是一个包的来源.它是一个`packages/versions`的列表.
Composer将查看所有你定义的`repositories`以找到项目需要的资源包(这句话很重要).
默认情况下已经将Packagist.org注册到Composer(或者理解为Packagist.org是Composer资源库默认的仓库类型)
**Composer资源库类型**
Composer资源库包括四种类型,默认的是`composer类型`,也就是packagist.org所使用的资源类型.
它使用一个单一的`packages.json`文件,包含了所有的资源包元数据.当你将包发布到pckagist.org上,则默认系统会创建一个`packages.json`,不过我没有找到[我的包](https://packagist.org/packages/ywdblog/phpcomposer)对应的文件.
**VCS资源库类型**
假如你想构建一个私有的Composer私有资源库类型,可以使用该类型,这里举一个例子,比如你在自己项目的`composer.json`定义如下,则就可以使用对应的Github上的代码了.


```
{
    "repositories": [
    {
        "type": "vcs",
        "url": "https://github.com/ywdblog/phpcomposer"
    }
    ],
    "require": {
        "ywdblog/phpcomposer": "dev-master"
    }
}
```
当运行`composer update`的时候,Comoser实际上是从Github上下载包而不是从pckagist.org上下载.
另外假如需要使用`Package资源库类型`或者`PEAR资源库类型`,参考官方文档即可,一般在`composer.json`中定义name、version属性即可.
#### Composer.json
在本文上面也多次提到了`composer.json`,比如你希望使用第三方包则需要在本地定义`composer.json`,Composer安装第三方包后,也会在第三方包目录下发现`composer.json`,那么这二者都叫`composer.json`,有什么区别呢?理解这非常的重要.
假如你在自己的项目下面定义一个`composer.json`,则这个包称之为`ROOT包`,这个`composer.json`定义你项目需要的条件(比如你的项目可能依赖一个第三方包).
`composer.json`中有些属性只能被`ROOT包`使用,比如`config`属性只在`ROOT包`中生效.
一个资源包是不是`ROOT包`,取决于它的上下文,比如你`git clone ywdblog/phpcomposer`,则这时候本地phpcomposer目录就是`ROOT包`,假如你在本地phpcomposer目录下`composer require ywdblog/phpcomposer`,则这时候你的项目phpcomposer就是`ROOT包`.
了解`composer-schema.json`可[参考该网址](https://github.com/composer/composer/blob/master/res/composer-schema.json),Laravel作为一个成熟的框架,其定义的[composer.json](https://github.com/laravel/laravel/blob/master/composer.json)非常经典
**关于包的版本**
当使用者在本地配置`composer.json`的时候,可以指定需要包的特定版本,Composer支持从Github仓库中下载`Tag`或者`分支`下的包.
对于Github上的Tag来说,Packagist会创建对应包的版本,它符合`X.Y.Z`,`vX.Y.Z`,`X.Y.Z-包类型`,就是说Github上虽然只有一个特定版本的包,但Composer支持多种形式的引用方式,比如:


```
composer require monolog/monolog  1.0.0-RC1 
composer require monolog/monolog  v1.0.0-RC1 
composer require monolog/monolog  1.0.*
composer require monolog/monolog  ~1.10
```
对于Github上的分支来说,Packagist会创建对应包的版本,假如分支名看起来像一个版本,将创建`{分支名}-dev`的包版本号,如果分支名看起来不像一个版本号,它将会创建`dev-{分支名}`形式的版本号


```
composer require monolog/monolog  master-dev
composer require monolog/monolog  master.x-dev
```
#### 总结:
理解Composer,最重要的是实践,最后也能明白`PSR-4`和命名空间,也可以尝试将你的项目发布到pckagist.org上.
