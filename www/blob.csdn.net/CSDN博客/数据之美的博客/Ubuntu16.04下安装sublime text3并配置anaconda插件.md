# Ubuntu16.04下安装sublime text3并配置anaconda插件 - 数据之美的博客 - CSDN博客
2018年01月05日 17:48:06[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：287

## 1. 安装sublime text 3
安装过程非常简单,在terminal中输入：
```
sudo add-apt-repository ppa:webupd8team/sublime-text-3 #添加sublime text 3的仓库
sudo apt-get update #更新软件库
sudo apt-get install sublime-text-installer #安装Sublime Text 3
```
- 1
- 2
- 3
如果需要启动sublime，那么在terminal中输入
`subl`- 1
注意，如果需要卸载，那么在terminal中输入
`sudo apt-get remove sublime-text-installer`- 1
## 2. 安装 Package Control
为了使用众多的 插件 来扩展 Sublime 的功能，你需要安装一个叫做 Package Control 的插件管理器——这个东西你必须要手动安装。但是一旦你安装好了以后，你就可以使用 Package Control 来安装，移除或者升级所有的 ST3 插件了。
- 点击 [这里](https://packagecontrol.io/installation#st3) 从
 Sublime Text 3 官方获取用于安装的代码。依次点击 View > Show Console 打开 ST3 的控制台。在控制台中粘贴刚才的代码，然后点击回车。最后重启 ST3。
- 现在你可以通过快捷键 ctrl+shift+P 打开 Package Control 来安装其他的插件了。输入 install 然后你就能看见屏幕上出现了 Package Control: Install Package，点击回车然后搜索你想要的插件。 
![这里写图片描述](https://img-blog.csdn.net/20161019135358128)
- 其他一些相关命令如下： 
List Packages 显示所有已安装的插件 
Remove Packages 移除一个指定的插件 
Upgrade Package 更新一个指定的插件 
Upgrade/Overwrite All Packages 更新所有已安装的插件
## 3. 安装 anaconda插件
### 3.1 介绍
Anaconda 是一个终极 Python 插件。它为 ST3 增添了多项 IDE 类似的功能，例如：
- Autocompletion 自动完成，该选项默认开启，同时提供多种配置选项。
- Code linting 使用支持 pep8 标准的 PyLint 或者 PyFlakes。因为我个人使用的是另外的 linting 工具，所以我会在 Anaconda 的配置文件 Anaconda.sublime-settings 中将 linting 完全禁用。操作如下: Sublime > Preferences > Package Settings > Anaconda
 > Settings – User： {“anaconda_linting”: false}
- McCabe code complexity checker 让你可以在特定的文件中使用 McCabe complexity checker. 如果你对软件复杂度检查工具不太熟悉的话，请务必先浏览上边的链接。
- Goto Definitions 能够在你的整个工程中查找并且显示任意一个变量，函数，或者类的定义。
- Find Usage 能够快速的查找某个变量，函数或者类在某个特定文件中的什么地方被使用了。
- Show Documentation： 能够显示一个函数或者类的说明性字符串(当然，是在定义了字符串的情况下)
### 3.2 安装
现在你可以通过快捷键 ctrl+shift+P 打开 Package Control 来安装其他的插件了。输入 install 然后你就能看见屏幕上出现了 Package Control: Install Package，点击回车然后搜索你想要的插件（比如Anaconda） 
![这里写图片描述](https://img-blog.csdn.net/20161019140044358)
进入安装界面，安装好后再在新窗口命令行中输入Anaconda并点击第一个备选项进行安装。 安装完成后会显示messages文档。 
安装Anaconda插件完成之后，会看到如下选项栏，说明Anaconda安装成功。 
![这里写图片描述](https://img-blog.csdn.net/20161019140358363)
### 3.3 配置
由于Anaconda插件本身无法知道Python安装的路径，所以需要设置Python主程序的实际位置。选择Preferences-Package Settings-Anacoda-Settings-Default选项，搜寻“python_interpreter” key， 
并将“python_interpreter”：”Python” 改为“python_interpreter”：”/usr/bin/python2.7” 
注意：这里的”/usr/bin/python2.7”是我的python路径，每个人需要根据自己电脑进行设置，python路径的查找方式是在terminal中输入:
`whereis python`- 1
即可以看到python路径 
选择Preferences-Package Settings-Anacoda-Settings-Users选项，键入以下json数据。保存，重启ST3即可。
```
{
    "python_interpreter": "/usr/bin/python2.7",
    "suppress_word_completions": true,
    "suppress_explicit_completions": true,
    "complete_parameters": true,
    "anaconda_linting":false
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
接下来，就会发现，ST3编写python代码时会有提示功能。 
![这里写图片描述](https://img-blog.csdn.net/20161019141203701)
## 参考文献
[1] [http://jingyan.baidu.com/article/fa4125acb8569b28ac7092ea.html](http://jingyan.baidu.com/article/fa4125acb8569b28ac7092ea.html)
[2] [http://python.jobbole.com/81312/](http://python.jobbole.com/81312/)
[3] [http://python.jobbole.com/81312/](http://python.jobbole.com/81312/)
版权声明：本文为博主原创文章，未经博主允许不得转载。
