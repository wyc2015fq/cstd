# 转载和积累系列 - eclispce 常用快捷键 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年10月12日 11:27:18[initphp](https://me.csdn.net/initphp)阅读数：884








【Ctrl+T】 

搜索当前接口的实现类



1. 【ALT+/】 

   此快捷键为用户编辑的好帮手，能为用户提供内容的辅助，不要为记不全方法和属性名称犯愁，当记不全类、方法和属性的名字时，多体验一下【ALT+/】快捷键带来的好处吧。

   2. 【Ctrl+O】 

   显示类中方法和属性的大纲，能快速定位类的方法和属性，在查找Bug时非常有用。

   3. 【Ctrl+/】 

   快速添加注释，能为光标所在行或所选定行快速添加注释或取消注释，在调试的时候可能总会需要注释一些东西或取消注释，现在好了，不需要每行进行重复的注释。

   4. 【Ctrl+D】 

   删除当前行，这也是笔者的最爱之一，不用为删除一行而按那么多次的删除键。

   5. 【Ctrl+M】 

   窗口最大化和还原，用户在窗口中进行操作时，总会觉得当前窗口小（尤其在编写代码时），现在好了，试试【Ctrl+M】快捷键。

   查看和定位快捷键

   在程序中，迅速定位代码的位置，快速找到Bug的所在，是非常不容易的事，Eclipse提供了强大的查找功能，可以利用如下的快捷键帮助完成查找定位的工作。

   1. 【Ctrl+K】、【Ctrl++Shift+K】

   快速向下和向上查找选定的内容，从此不再需要用鼠标单击查找对话框了。

   2. 【Ctrl+Shift+T】

   查找工作空间（Workspace）构建路径中的可找到Java类文件，不要为找不到类而痛苦，而且可以使用“*”、“？”等通配符。

   3. 【Ctrl+Shift+R】查找文件

   【Ctrl+Shift+T】查找类，查找工作空间（Workspace）中的所有文件（包括Java文件），也可以使用通配符。

   4. 【Ctrl+Shift+G】

   查找类、方法和属性的引用。这是一个非常实用的快捷键，例如要修改引用某个方法的代码，可以通过【Ctrl+Shift+G】快捷键迅速定位所有引用此方法的位置。

   5. 【Ctrl+Shift+O】

快速生成import，当从网上拷贝一段程序后，不知道如何import进所调用的类，试试【Ctrl+Shift+O】快捷键，一定会有惊喜。

   6. 【Ctrl+Shift+F】

   格式化代码，书写格式规范的代码是每一个程序员的必修之课，当看见某段代码极不顺眼时，选定后按【Ctrl+Shift+F】快捷键可以格式化这段代码，如果不选定代码则默认格式化当前文件（Java文件）。

   7. 【ALT+Shift+W】

   查找当前文件所在项目中的路径，可以快速定位浏览器视图的位置，如果想查找某个文件所在的包时，此快捷键非常有用（特别在比较大的项目中）。

   8. 【Ctrl+L】 

   定位到当前编辑器的某一行，对非Java文件也有效。

   9. 【Alt+←】、【Alt+→】

   后退历史记录和前进历史记录，在跟踪代码时非常有用，用户可能查找了几个有关联的地方，但可能记不清楚了，可以通过这两个快捷键定位查找的顺序。

   10. 【F3】 

快速定位光标位置的某个类、方法和属性。

   11. 【F4】 

   显示类的继承关系，并打开类继承视图。

   调试快捷键

   Eclipse中有如下一些和运行调试相关的快捷键。

   1. 【Ctrl+Shift+B】：在当前行设置断点或取消设置的断点。

   2. 【F11】：调试最后一次执行的程序。 

   3. 【Ctrl+F11】：运行最后一次执行的程序。 

   4. 【F5】：跟踪到方法中，当程序执行到某方法时，可以按【F5】键跟踪到方法中。 

   5. 【F6】：单步执行程序。 

   6. 【F7】：执行完方法，返回到调用此方法的后一条语句。 

   7. 【F8】：继续执行，到下一个断点或程序结束。

   常用编辑器快捷键

   通常文本编辑器都提供了一些和编辑相关的快捷键，在Eclipse中也可以通过这些快捷键进行文本编辑。 

   1. 【Ctrl+C】：复制。 

   2. 【Ctrl+X】：剪切。 

   3. 【Ctrl+V】：粘贴。 

   4. 【Ctrl+S】：保存文件。 

   5. 【Ctrl+Z】：撤销。 

   6. 【Ctrl+Y】：重复。 

   7. 【Ctrl+F】：查找。

   其他快捷键

   Eclipse中还有很多快捷键，无法一一列举，可以通过帮助文档找到它们的使用方式，另外还有几个常用的快捷键如下。 

   1. 【Ctrl+F6】：切换到下一个编辑器。 

   2. 【Ctrl+Shift+F6】：切换到上一个编辑器。

   3. 【Ctrl+F7】：切换到下一个视图。 

   4. 【Ctrl+Shift+F7】：切换到上一个视图。

   5. 【Ctrl+F8】：切换到下一个透视图。 

   6. 【Ctrl+Shift+F8】：切换到上一个透视图。



行注释/销注释 Ctrl+/  



块注释/销注释 Ctrl+Shift+/   Ctrl+Shift+\

查找 查找替换 Ctrl+H  Ctrl+F

查找相关信息   ctrl  +  shift  + t   通过类名来查找对应的类，包括在那个包，还有提示。

查找下一个/往回找 Ctrl+K   Ctrl+Shift+K

跳到某行 Ctrl+L，哈用惯了Editplus，不时会敲下Ctrl+G，

查找当前元素的声明 Ctrl+G 或者 Ctrl加鼠标左键

查找当前元素的所有引用 Ctrl+Shift+G

重新组织Import
Ctrl+Shift+O，能帮你一次去掉所有未使用的Import声明！

返回上一次查看的点 Ctrl+Q 看源码时很好用

复制一份代码 Ctrl+Alt+下

快速修正 Ctrl+1

引入某个类（接口）ctrl + shift + m

加头注释 shift + alt + j




ctrl  +  shift  +  g：查看引用  

ctrl  +  shift  +  n：重命名  

ctrl  +  shift  +  o：导入类  

ctrl  +  shift  +  r：启动上次运行  

ctrl  +  shift  +  f：格式化代码  

ctrl  +  c：复制  

ctrl  +  v：粘贴  

ctrl  +  x：切剪  

ctrl  +  a：全选  

ctrl  +  f：查找  

ctrl  +  z：撤销  

ctrl  +  y：重做  

ctrl  +  s：保存  

---------------------------------------------------------------  


用的最多的应该就是CTRL+SHIFT+S  

还有格式化用的也挺多。  

---------------------------------------------------------------  


ctrl  +  shift  +  f  格式化代码  

ctrl  +  shift  +  o  组织导入  

F3    打开声明  

Alt  +  shift  +  r    重命名变量  


---------------------------------------------------------------  


up  

---------------------------------------------------------------  


Alt+/        


---------------------------------------------------------------  


alt  +  left

alt  +  right

ctrl  +  q

---------------------------------------------------------------  


ctrl  +  shift  +  g：查看引用  

ctrl  +  shift  +  n：重命名  

ctrl  +  shift  +  f：格式化代码  

ctrl  +  c：复制  

ctrl  +  v：粘贴  

ctrl  +  a：全选  

ctrl  +  f：查找  

ctrl  +  z：撤销  

ctrl  +  s：保存  

Alt    +  /  智能提示  

---------------------------------------------------------------  


ctrl  +  shift  +  g：查看引用  

ctrl  +  shift  +  n：重命名  

ctrl  +  shift  +  o：导入类  

ctrl  +  shift  +  r：启动上次运行  

ctrl  +  shift  +  f：格式化代码  

ctrl  +  c：复制  

ctrl  +  v：粘贴  

ctrl  +  x：切剪  

ctrl  +  a：全选  

ctrl  +  f：查找  

ctrl  +  z：撤销  

ctrl  +  y：重做  

ctrl  +  s：保存  

Alt    +  /  智能提示  

F3    打开声明  

Alt  +  shift  +  r    重命名变量  

其实最常用的就是下面几个：  


ctrl  +  shift  +  o：导入类  


ctrl  +  shift  +  f：格式化代码  

ctrl  +  c：复制  

ctrl  +  v：粘贴  

ctrl  +  x：切剪  

ctrl  +  z：撤销  

ctrl  +  s：保存  

Alt    +  /  智能提示  


---------------------------------------------------------------  


Ctrl+M:  工作区最大化/最小化  

Alt+/:    智能提示  

F3:          察看声明  

Crtl+1:  修正错误  


Shift+Alt+T:  重构  

Shift+Alt+M:  提取函数  

Shift+Alt+R:  重命名  

Shift+Alt+C:  更改函数标记  


Ctrl+Shitf+F:  格式化代码  


---------------------------------------------------------------  


ctrl  +  shift  +  o：导入类  

atl  +  /：提示  

ctrl  +  shift  +  t：查找相关信息  

---------------------------------------------------------------  

Eclipse快捷键指南                                                                          

编辑  

作用域            功能            快捷键  

全局            查找并替换            Ctrl+F  

文本编辑器            查找上一个            Ctrl+Shift+K  

文本编辑器            查找下一个            Ctrl+K  

全局            撤销            Ctrl+Z  

全局            复制            Ctrl+C  

全局            恢复上一个选择            Alt+Shift+↓  

全局            剪切            Ctrl+X  

全局            快速修正            Ctrl1+1  

全局            内容辅助            Alt+/  

全局            全部选中            Ctrl+A  

全局            删除            Delete  

全局            上下文信息            Alt+？  

Alt+Shift+?  

Ctrl+Shift+Space  

Java编辑器            显示工具提示描述            F2  

Java编辑器            选择封装元素            Alt+Shift+↑  

Java编辑器            选择上一个元素            Alt+Shift+←  

Java编辑器            选择下一个元素            Alt+Shift+→  

文本编辑器            增量查找            Ctrl+J  

文本编辑器            增量逆向查找            Ctrl+Shift+J  

全局            粘贴            Ctrl+V  

全局            重做            Ctrl+Y  


查看  

作用域            功能            快捷键  

全局            放大            Ctrl+=  

全局            缩小            Ctrl+-  


窗口  

作用域            功能            快捷键  

全局            激活编辑器            F12  

全局            切换编辑器            Ctrl+Shift+W  

全局            上一个编辑器            Ctrl+Shift+F6  

全局            上一个视图            Ctrl+Shift+F7  

全局            上一个透视图            Ctrl+Shift+F8  

全局            下一个编辑器            Ctrl+F6  

全局            下一个视图            Ctrl+F7  

全局            下一个透视图            Ctrl+F8  

文本编辑器            显示标尺上下文菜单            Ctrl+W  

全局            显示视图菜单            Ctrl+F10  

全局            显示系统菜单            Alt+-  


导航  

作用域            功能            快捷键  

Java编辑器            打开结构            Ctrl+F3  

全局            打开类型            Ctrl+Shift+T

全局            打开类型层次结构            F4  

全局            打开声明            F3  

全局            打开外部javadoc            Shift+F2  

全局            打开资源            Ctrl+Shift+R  

全局            后退历史记录            Alt+←  

全局            前进历史记录            Alt+→  

全局            上一个            Ctrl+,  

全局            下一个            Ctrl+.  

Java编辑器            显示大纲            Ctrl+O  

全局            在层次结构中打开类型            Ctrl+Shift+H

全局            转至匹配的括号            Ctrl+Shift+P  

全局            转至上一个编辑位置            Ctrl+Q

Java编辑器            转至上一个成员            Ctrl+Shift+↑  

Java编辑器            转至下一个成员            Ctrl+Shift+↓  

文本编辑器            转至行            Ctrl+L  


搜索  

作用域            功能            快捷键  

全局            出现在文件中            Ctrl+Shift+U  

全局            打开搜索对话框            Ctrl+H

全局            工作区中的声明            Ctrl+G  

全局            工作区中的引用            Ctrl+Shift+G  


文本编辑  

作用域            功能            快捷键  

文本编辑器            改写切换            Insert

文本编辑器            上滚行            Ctrl+↑  

文本编辑器            下滚行            Ctrl+↓  


文件  

作用域            功能            快捷键  

全局            保存            Ctrl+X    

Ctrl+S  

全局            打印            Ctrl+P  

全局            关闭            Ctrl+F4  

全局            全部保存            Ctrl+Shift+S  

全局            全部关闭            Ctrl+Shift+F4  

全局            属性            Alt+Enter  

全局            新建            Ctrl+N  


项目  

作用域            功能            快捷键  

全局            全部构建            Ctrl+B  


源代码  

作用域            功能            快捷键  

Java编辑器            格式化            Ctrl+Shift+F  

Java编辑器            取消注释            Ctrl+  

Java编辑器            注释            Ctrl+/  

Java编辑器            添加导入            Ctrl+Shift+M  

Java编辑器            组织导入            Ctrl+Shift+O  

Java编辑器            使用try/catch块来包围            未设置，太常用了，所以在这里列出,建议自己设置。  

也可以使用Ctrl+1自动修正。  


运行  

作用域            功能            快捷键  

全局            单步返回            F7  

全局            单步跳过            F6  

全局            单步跳入            F5  

全局            单步跳入选择            Ctrl+F5  

全局            调试上次启动            F11  

全局            继续            F8  

全局            使用过滤器单步执行            Shift+F5  

全局            添加/去除断点            Ctrl+Shift+B  

全局            显示            Ctrl+D  

全局            运行上次启动            Ctrl+F11  

全局            运行至行            Ctrl+R  

全局            执行            Ctrl+U  


重构  

作用域            功能            快捷键  

全局            撤销重构            Alt+Shift+Z  

全局            抽取方法            Alt+Shift+M  

全局            抽取局部变量            Alt+Shift+L  

全局            内联            Alt+Shift+I  

全局            移动            Alt+Shift+V  

全局            重命名            Alt+Shift+R  

全局            重做            Alt+Shift+Y 






