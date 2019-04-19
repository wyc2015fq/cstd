# sublime text2--mac下初始配置(package control)和必备插件安装 - 数据之美的博客 - CSDN博客
2018年05月03日 15:29:44[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：204
- 要想装插件，需要先配置好package control 
1.按住ctrl+~，弹出的控制台中输入如下代码，windows和mac下都一样。注意：装完以后必须重启sublime方可生效噢~
`import urllib2,os; pf='Package Control.sublime-package'; ipp = sublime.installed_packages_path(); os.makedirs( ipp ) if not os.path.exists(ipp) else None; urllib2.install_opener( urllib2.build_opener( urllib2.ProxyHandler( ))); open( os.path.join( ipp, pf), 'wb' ).write( urllib2.urlopen( 'http://sublime.wbond.net/' +pf.replace( ' ','%20' )).read()); print( 'Please restart Sublime Text to finish installation') `- 1
2.验证是否安装好：command+shift+P，输入install package看是否可以调出来一个小小的新窗口
- 必备插件：装好package control就可以装各种插件啦~~ 
1.Emmet:自动补全html/css等代码和标签
