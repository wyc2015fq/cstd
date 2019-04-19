# sublime几款常用插件 - 三少GG - CSDN博客
2013年12月31日 15:12:30[三少GG](https://me.csdn.net/scut1135)阅读数：2009
Sublime Text
sublime text 2 有一个注释插件很好用，这里推荐一下，[**DocBlockr**](https://github.com/spadgos/sublime-jsdocs)这个插件可以很好的生成js ,php 等语言函数注释,只需要在函数上面输入/** ,然后按tab 就会自动生成注释
![](http://www.mythou.com/wp-content/uploads/2012/03/213.jpg)
![](http://www.mythou.com/wp-content/uploads/2012/03/212.jpg)
然后自己补充好函数功能，参数说明，返回类型可以自动生成，如果不能可以手动输入。太细的点这里就不说了。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
最近看到[异次元](http://www.iplaysoft.com/sublimetext.html)推荐的代码编辑器[Sublime
 Text2](http://www.sublimetext.com/)，感觉超好用，而且还有很多插件。不仅如此，还可以自己写个小插件。看到有人写了可以[插入时间](http://www.fantxi.com/blog/archives/sublime-text2-datetime-pugin/)的小插件，于是想写一个可以插入代码文件头的小插件。在用eclipse和pycharm中可以在新建文件的头部加上作者和创建时间等信息。而在sublime中也想有这样的功能，没找到使用方法，于是自然想到自己写。sublime的插件开发是基于Python的，写起来很方便。另附上[Sublime
 Text不完全使用手册](http://www.liuxiaofan.com/?p=1081)的传送门。
sublime插件开发的方法还可以参考：
[http://www.welefen.com/how-to-develop-sublime-text-plugin.html](http://www.welefen.com/how-to-develop-sublime-text-plugin.html)
[http://www.cnblogs.com/Lvkun/archive/2012/03/28/plugin-eval-sel.html](http://www.cnblogs.com/Lvkun/archive/2012/03/28/plugin-eval-sel.html)
1. 在菜单栏的Tools下有New Plugin，点击后就会给出一个可以往编辑器插入hello world的小插件。
![](http://images.cnitblog.com/blog/343468/201301/07221300-00f71b480fbc4869aef80807e6be9108.png)
2. 写好自己的插件代码，代码就不过多解释了，可以参考[sublime的API](http://www.sublimetext.com/docs/2/api_reference.html#sublime.View)。具体功能是可以记录文件创建时间和更新时间。
```
1 #-*- encoding: utf-8 -*-
 2 '''
 3 Created on 2013-01-07 22:05:01
 4 
 5 @author: Neil
 6 '''
 7 
 8 import sublime, sublime_plugin, datetime
 9 
10 class insertSignatureCommand(sublime_plugin.TextCommand):
11     def run(self, edit):
12         date = datetime.datetime.now()
13         dateStr = date.strftime("%Y-%m-%d %X")
14         text_encode = """#-*- encoding: utf-8 -*-\n'''\n"""
15         text_author = """\n\n@author: Neil\n'''\n"""
16         text = text_encode + 'Created on ' + dateStr + text_author
17 
18         #for region in the selection
19         #一个region是一个选择块，一次可以选择多个块
20         for r in self.view.sel():
21             str_r = self.view.substr(r)#str_r是所选择块的文本内容
22             if 'Created on ' in str_r:
23                 if 'Updated on ' in str_r:
24                     text = str_r[0:str_r.find('Updated on ')] + 'Updated on ' + dateStr + text_author
25                 else:
26                     text = str_r.replace(text_author, '\nUpdated on ' + dateStr + text_author)
27             self.view.erase(edit, r)
28             self.view.insert(edit, r.begin(), text)
```
3.保存文件时，默认会让选择保存在“...\Sublime Text 2\Packages\User\” 文件夹下，取好文件名insert_signature.py，会发现保存后会自动生成insert_signature.pyc。
按下快捷键Ctrl + ‘ 打开SublimeText的控制台，执行如下命令就可以运行刚刚写的小插件：
```
view.run_command('insert_signature')
```
4.为了能够用快捷键来运行，需要额外设置快捷键映射文件，Default (Windows).sublime-keymap，Default (Linux).sublime-keymap，Default (OSX).sublime-keymap
分别对应不同的平台，最好是都输入快捷键的映射。这些文件与insert_signature.py保存在同以目录。就windows而言，在Default (Windows).sublime-keymap中输入
```
[
    {
        "keys": ["ctrl+f7"], "command": "insert_signature",
        "args": {}
    }
]
```
以后只要按下ctrl + F7就可以自动插入文件头部内容了。插入时可以先选择以前插入的内容，然后按下Ctrl + F7，就可以添加或更新文件的更新时间了。
![](http://images.cnitblog.com/blog/343468/201301/07222634-af59c55ba86043d094448a29560a867f.png)
