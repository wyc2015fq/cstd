# 【sublime text3】安装插件 - Big Smile - CSDN博客
2018年02月10日 21:59:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：182标签：[sublime](https://so.csdn.net/so/search/s.do?q=sublime&t=blog)
个人分类：[工具用法](https://blog.csdn.net/tr1912/article/category/5987305)
        听说sublime这个文本编辑工具是业内程序员的利器，并且公司搞安卓和IOS的同事也在用，于是起了浓厚的兴趣，想要来研究研究，但是在装插件的时候遇到了很多坑，和大家分享一下。
首先是包管理插件的引入：
        我们在引入插件的时候，实际上就是引入了这个软件目录下所谓的packages，那么这个包的管理还有一个工具的支持，我们可以通过在控制台输入命令的方式安装。
       这里有个小坑就是网上一般都是说用“Ctrl+`”就可以调出控制台，说那个字符是单引号，可是我按了离冒号最近的那个单引号N次都不管用，然后一个偶然的机会，我终于知道这个符号是哪个键按出来了的了，那就是字母键盘的上方数字左面的~符号对应的键才是这个快捷键的组合键，` '  这两个键的输出方式差别需要自己体会（英文状态下。。。）
![](https://img-blog.csdn.net/20180210223622431)
     控制台是这个样子的，从网上找了半天才知道这个是控制台的。。。
然后我们可以输入一串代码来添加包管理器，如果是sublime text2的话，可以写如下代码：
`import urllib2,os,hashlib; h = '6f4c264a24d933ce70df5dedcf1dcaee' + 'ebe013ee18cced0ef93d5f746d80ef60'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); os.makedirs( ipp ) if not os.path.exists(ipp) else None; urllib2.install_opener( urllib2.build_opener( urllib2.ProxyHandler()) ); by = urllib2.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); open( os.path.join( ipp, pf), 'wb' ).write(by) if dh == h else None; print('Error validating download (got %s instead of %s), please try manual install' % (dh, h) if dh != h else 'Please restart Sublime Text to finish installation')`
如果是sublime text3的话，输入下面的代码：
`import urllib.request,os,hashlib; h = '6f4c264a24d933ce70df5dedcf1dcaee' + 'ebe013ee18cced0ef93d5f746d80ef60'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); by = urllib.request.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); print('Error validating download (got %s instead of %s), please try manual install' % (dh, h)) if dh != h else open(os.path.join( ipp, pf), 'wb' ).write(by)`
然后重启我们的sublime，就可以在 首选项这里看到Package Control这个菜单项，就说明我们安装成功了。
![](https://img-blog.csdn.net/20180210225020501)
这个时候我们就可以安装插件了（需要网络）
**安装插件**
        安装插件其实很简单，只要在网上搜到插件的名字就可以找到，或者是直接下载之后，放到软件安装目录下的packages文件夹下，然后重启就可以了，这里我们用第一种做演示。
首先用组合键Ctrl+shift+P打开管理器：
![](https://img-blog.csdn.net/20180210225807182)
然后输入install，选中刚装的install package这个
然后输入想要安装的插件名称就可以了：
![](https://img-blog.csdn.net/20180210231135280)
比如这个就是一个sublime汉化的插件，回车就会自动安装了。
