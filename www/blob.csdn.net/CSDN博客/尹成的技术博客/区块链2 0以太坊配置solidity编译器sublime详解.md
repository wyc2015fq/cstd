
# 区块链2.0以太坊配置solidity编译器sublime详解 - 尹成的技术博客 - CSDN博客

置顶2018年04月30日 22:46:39[尹成](https://me.csdn.net/yincheng01)阅读数：1049


官网：[https://www.sublimetext.com/3](https://www.sublimetext.com/3)，下面使用最新3134
![](https://img-blog.csdn.net/20180430224103531?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
设置字体大小和默认编码
![](https://img-blog.csdn.net/20180430224127791?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 注册码：
Valild Keys
—– BEGIN LICENSE —–
TwitterInc
200 User License
EA7E-890007
1D77F72E 390CDD93 4DCBA022FAF60790
61AA12C0 A37081C5 D03164124584D136
94D7F7D4 95BC8C1C 527DA828560BB037
D1EDDD8C AE7B379F 50C9D69DB35179EF
2FE898C4 8E4277A8 555CE714E1FB0E43
D5D52613 C3D12E98 BC49967F7652EED2
9D2D2E61 67610860 6D338B725CF95C69
E36B85CC 84991F19 7575D828470A92AB
—— END LICENSE ——
----- BEGIN LICENSE -----
eldon
Single User License
EA7E-1122628
C0360740 20724B8A 30420C096D7E046F
3F5D5FBB 17EF95DA 2BA7BB27CCB14947
27A316BE 8BCF4BC0 252FB8FFFD97DF71
B11A1DA9 F7119CA0 31984BB97D71700C
2C728BF8 B952E5F5 B941FF646D7979DA
B8EB32F8 8D415F8E F16FE657A35381CC
290E2905 96E81236 63D2B06DE5F01A69
84174B79 7C467714 641A901394CA7162
------ END LICENSE ------
## 解决注册码失效：
最近Sublime Text 3 增加了注册码验证功能，如果你使用共享版本的注册码，可能会提示注册码失效，但是却可以正常激活。
只需要把下面的字段加入到你的hosts文件即可
Add the following to yourHost file：
127.0.0.1license.sublimehq.com
127.0.0.1 45.55.255.55
127.0.0.1 45.55.41.223
Windows： C:\Windows\System32\drivers\etc
Mac： /Private/etc
ubuntu下： sudo gedit /etc/hosts
Done.
PS：host推荐使用老D博客的：https://laod.cn/
编辑后，你需要重新启动一下你的网络。
/etc/init.d/networkingrestart

## Subl3安装Package Control
简单的安装方法
在sublime3中使用Ctrl+`快捷键或者通过View->Show Console菜单打开命令行，粘贴如下代码：
import urllib.request,os; pf = 'PackageControl.sublime-package'; ipp = sublime.installed_packages_path();urllib.request.install_opener( urllib.request.build_opener(urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf),'wb').write(urllib.request.urlopen( 'http://sublime.wbond.net/' + pf.replace('','%20')).read())
参考：https://sublime.wbond.net/installation\#st3
## solidity自动提示与代码高亮：
之后重启sublime text，Ctrl+Shift+P中输入Install Package搜索Ethereum安装，在语法里选择solidity。
![](https://img-blog.csdn.net/20180430224202816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180430224209711?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 支持中文：
gitclone[https://github.com/lyfeyaj/sublime-text-imfix.git](https://github.com/lyfeyaj/sublime-text-imfix.git)
下载完成之后使用命令
cd sublime-text-imfix
sudo ./sublime-text-imfix
注意，要配置路径：
export路径为libsublime-imfix.so文件的路径
exec为sublime执行文件的路径
![](https://img-blog.csdn.net/2018043022422829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018043022423569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 软件的个人配置参考
{
"auto_complete": true,                    //自动补全
"auto_find_in_selection":true,           //开启选中范围内搜索
"auto_match_enabled": true,               //自动匹配
"bold_folder_labels": true,               //侧边栏文件夹显示加粗，区别于文件
"color_scheme": "Packages/Theme- Brogrammer/brogrammer.tmTheme",
"default_line_ending":"unix",            //使用 unix 风格的换行符
"draw_minimap_border": true,              //用于右侧代码预览时给所在区域加上边框，方便识别
"ensure_newline_at_eof_on_save":true,    //文件末尾自动保留一个空行
"fade_fold_buttons": false,               //默认显示行号右侧的代码段闭合展开三角号
"font_size": 11,
"highlight_line": true,                   //当前行高亮
"highlight_modified_tabs":true,          //高亮未保存文件
"ignored_packages":
[
"Vintage"
],                                       //sublime自带配置，不知何意
"save_on_focus_lost": true,               //窗口失焦立即保存文件
"tab_size": 4,
"theme":"Brogrammer.sublime-theme",     //和上面那个设置一起，配置sublime的Brogrammer主题
"translate_tabs_to_spaces": true,         //把代码 tab 对齐转换为空格对齐
"trim_trailing_white_space_on_save": true,//自动移除行尾多余空格
"update_check": false,                    //关闭自动更新
"word_wrap":"true"                       //设置自动换行
}
## 将sublime设置为默认文本编辑器
将下面的那个文件中的所有gedit.desktop替换为sublime-text.desktop,重启配置生效
sudo subl/etc/gnome/defaults.list
![](https://img-blog.csdn.net/20180430224554793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180425001235188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180425001144107?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
网址：http://www.qukuailianxueyuan.io/
![](https://img-blog.csdn.net/20180426145827720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042614570887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
欲领取造币技术与全套虚拟机资料
区块链技术交流QQ群：756146052备注：CSDN
尹成学院微信：备注：CSDN
![](https://img-blog.csdn.net/20180425000635656?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




