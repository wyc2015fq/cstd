# Laravel 执行流程（一）之自动加载 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月19日 14:28:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1
## 定位
- 从 public/index.php 定位到 bootstrap/autoload.php
- 从 bootstrap/autoload.php 定位到 vendor/autoload.php
- 从 vendor/autoload.php 定位到 `__DIR__ . '/composer' . '/autoload_real.php';`
定位完毕，你会看到这样的代码：
`return ComposerAutoloaderInit2bf9ae5262a23b284c9433508d41d77f::getLoader();`
**ComposerAutoloaderInit2bf9ae5262a23b284c9433508d41d77f** 简称本类。
那我们就从 **getLoader()** 方法入手。
## getLoader()
文件位于： `__DIR__ . '/composer' . '/autoload_real.php';`
逻辑顺序：
一、如果静态变量 `$loader` 不为空则返回 `$loader`，
二、注册一个自动加载程序，加载程序为本类的 `loadClassLoader()` 方法，
**[loadClassLoader 方法逻辑]**
静态方法，含有一个 `$class` 参数，判断如果 `$class` 等于 `Composer\Autoload\ClassLoader`，则载入当前目录下
的 ClassLoader.php 文件，实际上是在为这句代码工作：`self::$loader = $loader = new \Composer\Autoload\ClassLoader();`。
三、 `$loader` 得到 `ClassLoader` 类（`\Composer\Autoload\ClassLoader`）的一个实例，卸载自动加载程序 loadClassLoader，
四、载入路径信息，设置路径信息，
五、载入一些 autoload_x.php 形式的文件，
分别有：
- autoload_namespaces.php
- autoload_psr4.php
- autoload_classmap.php
并进行各自的循环 set 操作，如 `$loader->set($namespace, $path);`。
【set 函数】2 个参数，一个前缀，一个路径。如果前缀非真，将 paths 转为数组类型赋值给类成员变量 fallbackDirsPsr0，如果前缀为真，则将路径赋值给
`$this->prefixesPsr0[$prefix[0]][$prefix]` ，这个写法的意思等同于字母索引，比如 phpDocumentor ，则数组就图所示：
![file](https://iocaffcdn.phphub.org/uploads/images/201506/18/1795/JTaL9Qxyp9.png)
六、执行一个 `$loader->register(true);`，
**[register 方法逻辑]**
一个布尔值参数，将传给 spl_autoload_register 第三个参数中。
而自动加载程序为：`array($this, 'loadClass')`，也就是本类的 `loadClass()` 方法。
**[loadClass 方法逻辑]**
一个 `$class` 参数，用了 `findFile()` 方法判断文件是否存在，存在则调用函数 `includeFile()` 载入文件。
![file](https://iocaffcdn.phphub.org/uploads/images/201506/18/1795/kUNIefLSL4.png)
*注意：这个函数也在本类中直接定义了的。*
七、还载入了一个 autoload_files.php，而里面也是一组文件数组，貌似预加载一些函数库文件吧，没有继续深入这里了。
八、最后返回一个 `$loader` 变量，也就是 ClassLoader 类的实例。
好了，现在看看 `$loader` 这个实例到此拥有些什么？部分截图所示：
![file](https://iocaffcdn.phphub.org/uploads/images/201506/18/1795/yqHs8C9jDt.png)
可以看出类属性包含了具有字母索引的一些命名空间，文件路径等信息。这和刚才载入那几个文件进行 set 操作有关，想起来了吗？
到此 `getLoader()` 方法逻辑结束。
## 总结
实现自动化的关键代码是 vendor/autoload.php 的 `::getLoader()` 静态方法，
利用此方法内部的 `$loader->register(true);` 方法注册自动化载入方法，这样，当 new 对象的时候自动触发 `loadClass()` 了，而上面提到的 set 一些路径信息，正是自动化的必备条件，如有兴趣可以自行查看 vendor/composer/ClassLoader.php 的 `loadClass()` 方法代码细节。
上面如果没懂的，请打开文件代码，跟着慢慢走，慢慢看，一定能懂。
在返回到 vendor/autoload.php，在把 `return $loader` 返回到上一层。
即 bootstrap/autoload.php，
这行的代码 `require __DIR__.'/../vendor/autoload.php';`
我们 `var_dump()` 下 require 的返回值，和刚才 `$loader` 的部分截图完全一致。
其实有从 aotuload_real.php 文件开始，我尝试过删除 return，也没有任何报错，不知道这里的 return 意义为何。
可是还没有看到真正 new, 我们知道了原理，得看看它在哪调用了？
在看 index.php 定位到了 bootstrap/app.php
打开就看到第一个
$app = new Illuminate\Foundation\Application(
    realpath(__DIR__.'/../')
);
