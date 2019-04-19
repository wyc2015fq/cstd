# sublime 格式化json - sxf_123456的博客 - CSDN博客
2017年12月27日 17:50:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：155
                
sublime 格式化json
1、使用Ctrl+`快捷键或者View->Show Console打开命令行，
Sublime Text3 
import urllib.request,os; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf), 'wb').write(urllib.request.urlopen(
 'http://sublime.wbond.net/' + pf.replace(' ','%20')).read())
Sublime Text2
import urllib2,os; pf='PackageControl.sublime-package'; ipp = sublime.installed_packages_path(); os.makedirs(ipp ) if not os.path.exists(ipp) else None; urllib2.install_opener(urllib2.build_opener( urllib2.ProxyHandler( ))); open( os.path.join( ipp, pf),'wb'
 ).write( urllib2.urlopen( 'http://sublime.wbond.net/' +pf.replace( '','%20' )).read()); print( 'Please restart Sublime Text to finishinstallation')
2、使用Ctrl+Shitf+p,输入Install Package
3、再输入pretty json
4、git clone https://github.com/dzhibas/SublimePrettyJson.git 下载pretty json
5、将SublimePrettyJson 命名为 Pretty JSON
6、将该文件放在sublime text安装目录：D:\Sublime\Sublime Text 3\Packages
7、格式化快捷键：Ctrl+windows+j
            
