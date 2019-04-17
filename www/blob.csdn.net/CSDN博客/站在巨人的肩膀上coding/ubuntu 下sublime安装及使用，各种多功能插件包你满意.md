# ubuntu 下sublime安装及使用，各种多功能插件包你满意 - 站在巨人的肩膀上coding - CSDN博客





2018年04月12日 10:52:44[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：2186








## web前端的开发神器

> 
**WebStrom和Sublime堪称史上最强的web开发神器：**

提供了智能的代码提示，智能补全，各种强大的轻量级的插件支持。WebStrom相对智能一点，但是耗内存大，有时会卡顿现象，界面不简洁，Sublime相对灵活轻巧，界面清爽，功能齐全，插件快速安装等特点。


### ubuntu上安装教程：

> 
**1、安装[Sublime Text 3](http://www.sublimetext.com/3) 及常用的神器插件**
  首先添加sublime text 3的仓库：`sudo add-apt-repository ppa:webupd8team/sublime-text-3`
  根据提示按ENTER 继续，建立信任数据库 
  再添加sublime text 3的仓库：`sudo apt update`
  最后安装Sublime Text 3：`sudo apt install sublime-text-installer`
  软件安装成功。 
**2、安装Package Control**
  输入命令subl打开Sublime Text 3 
  按快捷键ctrl+`(Esc下面的按键)，在弹出的控制台输入一下代码回车 
  import urllib.request,os,hashlib; h = ‘2915d1851351e5ee549c20394736b442’ + ‘8bc59f460fa1548d1514676163dafc88’; pf = ‘Package Control.sublime-package’; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); by = urllib.request.urlopen( ‘[http://packagecontrol.io/](http://packagecontrol.io/)’ + pf.replace(’ ‘, ‘%20’)).read(); dh = hashlib.sha256(by).hexdigest(); print(‘Error validating download (got %s instead of %s), please try manual install’ % (dh, h)) if dh != h else open(os.path.join( ipp, pf), ‘wb’ ).write(by)  
  重启软件后可以在Preference菜单下找到Package Control命令，单击运行Packeage Control,输入


### 常用插件

#### *安装方法ctrl+shift+p>输入install>按下enter>查找插件>确认*

```
1.Git
常用的git社区有csdn、github、开源中国，还有自己搭建的git服务器，在开过程中我们习
惯了在软件里直接使用git,而不是去切换到终端运行相应的命令。

2.GitGutter
GitGutter 更好的帮助开发者查看文件之前的改动和差异，提升开发效率，
减少错误的提交，代码冲突。

3.Emmet
Emmet 项目的前身是前端开发人员熟知的 Zen Coding（快速编写 HTML/CSS 代码的方案）。
在 Sublime Text 编辑器中搭配 Emmet 插件真的是让你编码快上加快。

4.SublimeREPL
这个插件允许你在Sublime里面运行各种语言（如NodeJs,Python,Scala等等），
所以编程少不了它

5.DocBlockr
如果你遵循的编码的风格很严格，这款插件能够使你的任务更容易。DocBlokr 帮助你创造你的代码注释，
通过解析功能，参数，变量，并且自动添加基本项目

6.CSSComb
这是用来给css属性进行排序格式化的插件，如果你想让你的代码看起来简洁明了，清新飘逸，
那么这个插件就是一个有效的解决方案，特别是很多人一个开发的时候可以保持编码风格统一

7.Alignment
这个插件让你能对齐你的代码，包括 PHP、CSS 和 Javascript。代码看起来更简洁和可读，便于编辑。

8.Trmmer
你知道当你编写代码时，由于错误或别的某些原因，会产生一些不必要的空格。
需要注意的是多余的空格有时也会造成错误。这个插件会自动删除这些不必要的空格。

9.ColorPicker
如果你经常要查看或设置颜色值，这个插件可以很方便地调用你本机的调色板应用。
（译者扩充：）这是一个双向的功能，你既可以在调色板中选择一个颜色，
然后按“确定”按钮把该值填写到 SublimeText 中活动文档的当前位置，
也可以在活动文档中选择一个颜色的值，按此插件的快捷键就会在显示的调色板中定位到该值所对应的颜色

10.angularjs
在编写angularjs时可以快速提示补全，避免写错

11.reactjs
我们经常会忘记一些语法，需要去查找，为了避免不必要的麻烦，
装上reactjs就可以解决忘记react语法的问题

12.SpaceGray
一个好的主题可以让人心情愉悦，避免敲错代码，装上它，就可以拥有令人兴奋的主题。
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48

#### 等等，还有好多好用的插件，可根据自己的需要安装。

### Sublime常用快捷键：

> 
掌握基本的代码编辑器的快捷键，能让你打码更有效率,刚开始可能不大记得住，多敲几次就能熟悉并使用它

**精华键 :**

Ctrl+Shift+P：打开命令面板 
  Ctrl+P：搜索项目中的文件 
  Ctrl+G：跳转到第几行 
  Ctrl+W：关闭当前打开文件 
  Ctrl+Shift+W：关闭所有打开文件 
  Ctrl+Shift+V：粘贴并格式化 
  Ctrl+D：选择单词，重复可增加选择下一个相同的单词 
  Ctrl+L：选择行，重复可依次增加选择下一行 
  Ctrl+Shift+L：选择多行 
  Ctrl+Shift+Enter：在当前行前插入新行 
  Ctrl+X：删除当前行 
  Ctrl+M：跳转到对应括号 
  Ctrl+U：软撤销，撤销光标位置 
  Ctrl+J：选择标签内容 
  Ctrl+F：查找内容 
  Ctrl+Shift+F：查找并替换 
  Ctrl+H：替换 
  Ctrl+R：前往 method 
  Ctrl+N：新建窗口 
  Ctrl+K+B：开关侧栏 
  Ctrl+Shift+M：选中当前括号内容，重复可选着括号本身 
  Ctrl+F2：设置/删除标记 
  Ctrl+/：注释当前行 
  Ctrl+Shift+/：当前位置插入注释 
  Ctrl+Alt+/：块注释，并Focus到首行，写注释说明用的 
  Ctrl+Shift+A：选择当前标签前后，修改标签用的 
  F11：全屏 
  Shift+F11：全屏免打扰模式，只编辑当前文件 
  Alt+F3：选择所有相同的词 
  Alt+.：闭合标签 
  Alt+Shift+数字：分屏显示 
  Alt+数字：切换打开第N个文件 
  Shift+右键拖动：光标多不，用来更改或插入列内容 
  鼠标的前进后退键可切换Tab文件 
  按Ctrl，依次点击或选取，可需要编辑的多个位置 
  按Ctrl+Shift+上下键，可替换行

**选择类 :**

Ctrl+D 选中光标所占的文本，继续操作则会选中下一个相同的文本。 
  Alt+F3 选中文本按下快捷键，即可一次性选择全部的相同文本进行同时编辑。举个栗子：快速选中并更改所有相同的变量名、函数名等。 
  Ctrl+L 选中整行，继续操作则继续选择下一行，效果和 Shift+↓ 效果一样。 
  Ctrl+Shift+L 先选中多行，再按下快捷键，会在每行行尾插入光标，即可同时编辑这些行。 
  Ctrl+Shift+M 选择括号内的内容（继续选择父括号）。举个栗子：快速选中删除函数中的代码，重写函数体代码或重写括号内里的内容。 
  Ctrl+M 光标移动至括号内结束或开始的位置。 
  Ctrl+Enter 在下一行插入新行。举个栗子：即使光标不在行尾，也能快速向下插入一行。 
  Ctrl+Shift+Enter 在上一行插入新行。举个栗子：即使光标不在行首，也能快速向上插入一行。 
  Ctrl+Shift+[ 选中代码，按下快捷键，折叠代码。 
  Ctrl+Shift+] 选中代码，按下快捷键，展开代码。 
  Ctrl+K+0 展开所有折叠代码。 
  Ctrl+← 向左单位性地移动光标，快速移动光标。 
  Ctrl+→ 向右单位性地移动光标，快速移动光标。 
  shift+↑ 向上选中多行。 
  shift+↓ 向下选中多行。 
  Shift+← 向左选中文本。 
  Shift+→ 向右选中文本。 
  Ctrl+Shift+← 向左单位性地选中文本。 
  Ctrl+Shift+→ 向右单位性地选中文本。 
  Ctrl+Shift+↑ 将光标所在行和上一行代码互换（将光标所在行插入到上一行之前）。 
  Ctrl+Shift+↓ 将光标所在行和下一行代码互换（将光标所在行插入到下一行之后）。 
  Ctrl+Alt+↑ 向上添加多行光标，可同时编辑多行。 
  Ctrl+Alt+↓ 向下添加多行光标，可同时编辑多行。

**编辑类 :**
  Ctrl+J 合并选中的多行代码为一行。举个栗子：将多行格式的CSS属性合并为一行。 
  Ctrl+Shift+D 复制光标所在整行，插入到下一行。 
  Tab 向右缩进。 
  Shift+Tab 向左缩进。 
  Ctrl+K+K 从光标处开始删除代码至行尾。 
  Ctrl+Shift+K 删除整行。 
  Ctrl+/ 注释单行。 
  Ctrl+Shift+/ 注释多行。 
  Ctrl+K+U 转换大写。 
  Ctrl+K+L 转换小写。 
  Ctrl+Z 撤销。 
  Ctrl+Y 恢复撤销。 
  Ctrl+U 软撤销，感觉和 Gtrl+Z 一样。 
  Ctrl+F2 设置书签 
  Ctrl+T 左右字母互换。 
  F6 单词检测拼写

**搜索类 :**
  Ctrl+F 打开底部搜索框，查找关键字。 
  Ctrl+shift+F 在文件夹内查找，与普通编辑器不同的地方是sublime允许添加多个文件夹进行查找，略高端，未研究。 
  Ctrl+P 打开搜索框。举个栗子：1、输入当前项目中的文件名，快速搜索文件，2、输入@和关键字，查找文件中函数名，3、输入：和数字，跳转到文件中该行代码，4、输入#和关键字，查找变量名。 
  Ctrl+G 打开搜索框，自动带：，输入数字跳转到该行代码。举个栗子：在页面代码比较长的文件中快速定位。 
  Ctrl+R 打开搜索框，自动带@，输入关键字，查找文件中的函数名。举个栗子：在函数较多的页面快速查找某个函数。 
  Ctrl+： 打开搜索框，自动带#，输入关键字，查找文件中的变量名、属性名等。 
  Ctrl+Shift+P 打开命令框。场景栗子：打开命名框，输入关键字，调用sublime text或插件的功能，例如使用package安装插件。 
  Esc 退出光标多行选择，退出搜索框，命令框等。 
  显示类 
  Ctrl+Tab 按文件浏览过的顺序，切换当前窗口的标签页。 
  Ctrl+PageDown 向左切换当前窗口的标签页。 
  Ctrl+PageUp 向右切换当前窗口的标签页。 
  Alt+Shift+1 窗口分屏，恢复默认1屏（非小键盘的数字） 
  Alt+Shift+2 左右分屏-2列 
  Alt+Shift+3 左右分屏-3列 
  Alt+Shift+4 左右分屏-4列 
  Alt+Shift+5 等分4屏 
  Alt+Shift+8 垂直分屏-2屏 
  Alt+Shift+9 垂直分屏-3屏 
  Ctrl+K+B 开启/关闭侧边栏。 
  F11 全屏模式 
  Shift+F11 免打扰模式




