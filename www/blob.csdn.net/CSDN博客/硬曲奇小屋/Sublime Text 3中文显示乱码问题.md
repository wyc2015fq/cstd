# Sublime Text 3中文显示乱码问题 - 硬曲奇小屋 - CSDN博客





2017年09月14日 16:31:50[HardCookies](https://me.csdn.net/james_616)阅读数：153








工欲善其事，必先利其器。 

对于我们码代码的人来说，选择一款好用的编辑器就好比一把宝剑之于剑客。也不是没有考虑过Vim/Emacs，但那确实是对高手而言，非我等平凡剑客所能轻易驾驭的了的。经过多方较量，还是选择Sublime Text 3作为我的编辑器。原因有下： 

 -  小巧便捷 

 - 支持多种语言，语法高亮 

 - 跨平台使用
因为我平时会用C、python、matlab来写写代码，也要看看markdown文件。但是Sublime Text原生不支持中文，如果打开的文件含有中文的话会显示成乱码。以下为解决方案：

1.安装Sublime Package Control。 

在Sublime的控制台上输入以下代码，如果是Sublime Text 2的话则选择另外相应的代码。
`import urllib.request,os,hashlib; h = '6f4c264a24d933ce70df5dedcf1dcaee' + 'ebe013ee18cced0ef93d5f746d80ef60'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); by = urllib.request.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); print('Error validating download (got %s instead of %s), please try manual install' % (dh, h)) if dh != h else open(os.path.join( ipp, pf), 'wb' ).write(by)`
2.重启Sublime Text 3 

3.在Preference -> Package Control 点击Install Package，然后搜索“ConvertToUTF8”，点击即可下载。 

4.但是在打开后还是有问题，会有提示建议你下载**Codecs33**这个包，下载方式与之前相同。
按照这个作者的说法是 
*由于 Sublime Text 3 内嵌的 Python 限制，ConvertToUTF8 可能无法正常工作。你可以安装本插件来解决这一问题。*

好了之后就能正常显示中文啦！效果如图 


![Sublime Text](https://img-blog.csdn.net/20170914163304643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







