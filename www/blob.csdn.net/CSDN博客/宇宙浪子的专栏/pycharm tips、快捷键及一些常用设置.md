# pycharm tips、快捷键及一些常用设置 - 宇宙浪子的专栏 - CSDN博客
2015年01月06日 16:53:15[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1439
转自：[http://blog.csdn.net/pipisorry/article/details/39909057](http://blog.csdn.net/pipisorry/article/details/39909057)
**pycharm Learning tips:**
**/pythoncharm/help/tip of the day:**
A special variant of the Code Completion feature invoked by pressing Ctrl+Space twice allows you to complete the name of any class no matter if it was imported in the current file or not. If the class is not imported yet, the import statement is generated automatically.
You can quickly find all places where a particular class, method or variable is used in the whole project by positioning the caret at the symbol's name or at its usage in code and pressing Alt+Shift+F7 (Find Usages in the popup menu).
To navigate to the declaration of a class, method or variable used somewhere in the code, position the caret at the usage and press F12. You can also click the mouse on usages with the Ctrl key pressed to jump to declarations.
You can easily rename your local variables with automatic correction of all places where they are used.
To try it, place the caret at the variable you want to rename, and press Shift+F6 (Refactor | Rename). Type the new name in the popup window that appears, or select one of the suggested names, and press Enter.
...
在PyCharm安装目录 /opt/pycharm-3.4.1/help目录下可以找到ReferenceCard.pdf快捷键英文版说明
or 打开pycharm > help > default keymap ref
PyCharm Default Keymap ...
**PyCharm3.0默认快捷键(翻译的)**
1、编辑（Editing）
Ctrl + Space    基本的代码完成（类、方法、属性）
Ctrl + Alt + Space  快速导入任意类
Ctrl + Shift + Enter    语句完成
Ctrl + P    参数信息（在方法中调用参数）
Ctrl + Q    快速查看文档
Shift + F1    外部文档
Ctrl + 鼠标    简介
Ctrl + F1    显示错误描述或警告信息
Alt + Insert    自动生成代码
Ctrl + O    重新方法
Ctrl + Alt + T    选中
Ctrl + /    行注释
Ctrl + Shift + /    块注释
Ctrl + W    选中增加的代码块
Ctrl + Shift + W    回到之前状态
Ctrl + Shift + ]/[     选定代码块结束、开始
Alt + Enter    快速修正
Ctrl + Alt + L     代码格式化
Ctrl + Alt + O    优化导入
Ctrl + Alt + I    自动缩进
Tab / Shift + Tab  缩进、不缩进当前行
Ctrl+X/Shift+Delete    剪切当前行或选定的代码块到剪贴板
Ctrl+C/Ctrl+Insert    复制当前行或选定的代码块到剪贴板
Ctrl+V/Shift+Insert    从剪贴板粘贴
Ctrl + Shift + V    从最近的缓冲区粘贴
Ctrl + D  复制选定的区域或行
Ctrl + Y    删除选定的行
Ctrl + Shift + J  添加智能线
Ctrl + Enter   智能线切割
Shift + Enter    另起一行
Ctrl + Shift + U  在选定的区域或代码块间切换
Ctrl + Delete   删除到字符结束
Ctrl + Backspace   删除到字符开始
Ctrl + Numpad+/-   展开折叠代码块
Ctrl + Numpad+   全部展开
Ctrl + Numpad-   全部折叠
Ctrl + F4   关闭运行的选项卡
 2、查找/替换(Search/Replace)
F3   下一个
Shift + F3   前一个
Ctrl + R   替换
Ctrl + Shift + F   全局查找
Ctrl + Shift + R   全局替换
 3、运行(Running)
Alt + Shift + F10   运行模式配置
Alt + Shift + F9    调试模式配置
Shift + F10    运行
Shift + F9   调试
Ctrl + Shift + F10   运行编辑器配置
Ctrl + Alt + R   运行manage.py任务
 4、调试(Debugging)
F8   跳过
F7   进入
Shift + F8   退出
Alt + F9    运行游标
Alt + F8    验证表达式
Ctrl + Alt + F8   快速验证表达式
F9    恢复程序
Ctrl + F8   断点开关
Ctrl + Shift + F8   查看断点
 5、导航(Navigation)
Ctrl + N    跳转到类
Ctrl + Shift + N    跳转到符号
Alt + Right/Left    跳转到下一个、前一个编辑的选项卡
F12    回到先前的工具窗口
Esc    从工具窗口回到编辑窗口
Shift + Esc   隐藏运行的、最近运行的窗口
Ctrl + Shift + F4   关闭主动运行的选项卡
Ctrl + G    查看当前行号、字符号
Ctrl + E   当前文件弹出
Ctrl+Alt+Left/Right   后退、前进
Ctrl+Shift+Backspace    导航到最近编辑区域
Alt + F1   查找当前文件或标识
Ctrl+B / Ctrl+Click    跳转到声明
Ctrl + Alt + B    跳转到实现
Ctrl + Shift + I查看快速定义
Ctrl + Shift + B跳转到类型声明
Ctrl + U跳转到父方法、父类
Alt + Up/Down跳转到上一个、下一个方法
Ctrl + ]/[跳转到代码块结束、开始
Ctrl + F12弹出文件结构
Ctrl + H类型层次结构
Ctrl + Shift + H方法层次结构
Ctrl + Alt + H调用层次结构
F2 / Shift + F2下一条、前一条高亮的错误
F4 / Ctrl + Enter编辑资源、查看资源
Alt + Home显示导航条F11书签开关
Ctrl + Shift + F11书签助记开关
Ctrl + #[0-9]跳转到标识的书签
Shift + F11显示书签
 6、搜索相关(Usage Search)
Alt + F7/Ctrl + F7文件中查询用法
Ctrl + Shift + F7文件中用法高亮显示
Ctrl + Alt + F7显示用法
 7、重构(Refactoring)
F5复制F6剪切
Alt + Delete安全删除
Shift + F6重命名
Ctrl + F6更改签名
Ctrl + Alt + N内联
Ctrl + Alt + M提取方法
Ctrl + Alt + V提取属性
Ctrl + Alt + F提取字段
Ctrl + Alt + C提取常量
Ctrl + Alt + P提取参数
 8、控制VCS/Local History
Ctrl + K提交项目
Ctrl + T更新项目
Alt + Shift + C查看最近的变化
Alt + BackQuote(’)VCS快速弹出
 9、模版(Live Templates)
Ctrl + Alt + J当前行使用模版
Ctrl +Ｊ插入模版
 10、基本(General)
Alt + #[0-9]打开相应的工具窗口
Ctrl + Alt + Y同步
Ctrl + Shift + F12最大化编辑开关
Alt + Shift + F添加到最喜欢
Alt + Shift + I根据配置检查当前文件
Ctrl + BackQuote(’)快速切换当前计划
Ctrl + Alt + S　打开设置页
Ctrl + Shift + A查找编辑器里所有的动作
Ctrl + Tab在窗口间进行切换 
**11.OTH**
ctrl单击引入模块   进入模块查看
**一些常用设置：**
file
 -> Setting ->Editor
1. 设置Python自动引入包，要先在 >general > autoimport ->
 python :show popup
     快捷键：Alt + Enter: 自动添加包
2. “代码自动完成”时间延时设置
  > Code Completion   -> Auto code completion in (ms):0  -> Autopopup in (ms):500
3. Pycharm中默认是不能用Ctrl+滚轮改变字体大小的，可以在〉Mouse中设置
4. 显示“行号”与“空白字符”
  > Appearance  -> 勾选“Show line numbers”、“Show whitespaces”、“Show method separators”
5. 设置编辑器“颜色与字体”主题
  > Colors & Fonts -> Scheme name -> 选择"monokai"“Darcula”
  说明：先选择“monokai”，再“Save As”为"monokai-pipi"，因为默认的主题是“只读的”，一些字体大小颜色什么的都不能修改，拷贝一份后方可修改！
  修改字体大小
> Colors & Fonts -> Font -> Size -> 设置为“14”
6. 设置缩进符为制表符“Tab”
  File -> Default Settings -> Code Style
  -> General -> 勾选“Use tab character”
  -> Python -> 勾选“Use tab character”
  -> 其他的语言代码同理设置
7. 去掉默认折叠
  > Code Folding -> Collapse by default -> 全部去掉勾选 
8. pycharm默认是自动保存的，习惯自己按ctrl + s  的可以进行如下设置：
    > General -> Synchronization -> Save files on frame deactivation  和 Save files automatically if application is idle for .. sec 的勾去掉
    > Editor Tabs -> Mark modified tabs with asterisk 打上勾
9.>file and code template>python scripts
#!/usr/bin/env python
# coding=gbk
"""
__title__ = 'Clustering – Finding Related Posts'
__author__ = '$USER'
__mtime__ = '$DATE'
# code is far away from bugs with the god animal protecting
              ┏┓      ┏┓
            ┏┛┻━━━┛┻┓
            ┃      ━      ┃
            ┃  ┳┛  ┗┳  ┃
            ┃      ┻      ┃
            ┗━┓      ┏━┛
                ┃      ┗━━━┓
                ┃  神兽保佑    ┣┓
                ┃　永无BUG！   ┏┛
                ┗┓┓┏━┳┓┏┛
                  ┃┫┫  ┃┫┫
                  ┗┻┛  ┗┻┛
"""
10
File Encodings> IDE Encoding: UTF-8;Project Encoding: UTF-8;
**File -> Settings -> appearance**
1. 修改IDE快捷键方案
  > Keymap
1) execute selection in console : add keymap > ctrl + enter
  系统自带了好几种快捷键方案，下拉框中有如“defaul”,“Visual Studio”,在查找Bug时非常有用,“NetBeans 6.5”,“Default for GNOME”等等可选项，
因为“Eclipse”方案比较大众，个人用的也比较多，最终选择了“Eclipse”。 
  还是有几个常用的快捷键跟Eclipse不一样，为了能修改，还得先对Eclipse方案拷贝一份： 
  (1).代码提示功能，默认是【Ctrl+空格】，现改为跟Eclipse一样，即【Alt+/】
  Main menu -> code -> Completion -> Basic -> 设置为“Alt+/”
  Main menu -> code -> Completion -> SmartType -> 设置为“Alt+Shift+/”
  不过“Alt+/”默认又被 
  Main menu -> code -> Completion -> Basic -> Cyclic Expand Word 占用，先把它删除再说吧（单击右键删除）！
  (2).关闭当前文档，默认是【Ctrl+F4】，现改为跟Eclipse一样，即【Ctrl+W】
  Main menu -> Window -> Active Tool Window -> Close Active Tab -> 设置为 “Ctrl+F4”;
  Main menu -> Window -> Editor -> Close -> 设置为 “Ctrl+W”;
2.设置IDE皮肤主题
 > Theme -> 选择“Alloy.IDEA Theme”
  或者在setting中搜索theme可以改变主题，所有配色统一改变
**File > settings > build.excution**
1 console > pyconsole
import sys;
print('Python %s on %s' % (sys.version, sys.platform))
sys.path.extend([WORKING_DIR_AND_PYTHON_PATHS])
import  numpy as  np
import  scipy as sp
import  matplotlib as mpl
**File > settings > Project : initial project**
project dependencies > LDA > project depends on these projects > 选择sim_cluster就可以在LDA中调用sim_cluster中的包
【[Configure
 PyCharm](http://www.fanli7.net/a/JAVAbiancheng/ANT/20140605/470658.html)】
Pycharm实用功能：
[[PyCharm中的那些实用功能](http://blog.jobbole.com/51498/)]
**NOTES:**
1 ctrl + shift + f10执行python时，cwd为run/debug configurations 中的working directory
可在edit configurations > project or defaults中配置
2 python console中的cwd为File > settings > build.excution > console > pyconsole中的working directory
并可在其中配置
from:[http://blog.csdn.net/pipisorry/article/details/39909057](http://blog.csdn.net/pipisorry/article/details/39909057)
ref:[pycharm的一些设置和快捷键](http://www.cnblogs.com/pigzhu/p/3486280.html)[](http://blog.csdn.net/thinkmore1314/article/details/9376547)
[pycharm 2.7 快捷键](http://blog.csdn.net/thinkmore1314/article/details/9376547)
