# Eclipse的使用 - 博客堂 - CSDN博客





2017年02月21日 11:25:10[最小森林](https://me.csdn.net/u012052268)阅读数：592








# Eclipse的使用



- [Eclipse的使用](#eclipse的使用)- [Eclipse空间的基本配置](#eclipse空间的基本配置)
- [Eclipse中快捷键的使用](#eclipse中快捷键的使用)
- [Eclipse中如何提高开发效率](#eclipse中如何提高开发效率)
- [删除项目和导入项目](#删除项目和导入项目)
- [Eclipse中代码的高级Debug调试](#eclipse中代码的高级debug调试)





## Eclipse空间的基本配置
- 程序的编译和运行的环境配置(一般不改)

```java
window - Preferences - Java
    编译环境：Compiler   默认选中的就是最高版本。
    运行环境：Installed JREs 默认会找你安装的那个JDK。建议配置了Java的环境变量。
问题：
    低编译，高运行。可以。
    高编译，低运行。不可以。
建议，编译和运行的版本一致。
```
- 如何去掉默认注释?

```
window - Preferences - Java - Code Style - Code Templates
        选择你不想要的内容，通过右边Edit编辑。
        注意：请只删除注释部分，不是注释部分的不要删除。
```
- 行号的显示和隐藏

```
显示：在代码区域的最左边的空白区域，右键 -- Show Line Numbers即可。
隐藏：把上面的动作再做一次。
```
- 字体大小及颜色

```
a:Java代码区域的字体大小和颜色：
    window - Preferences - General - Appearance - Colors And Fonts - Java修改 - Java Edit Text Font
b:控制台
    window - Preferences - General - Appearance - Colors And Fonts - Debug - Console font
c:其他文件
    window - Preferences - General - Appearance - Colors And Fonts - Basic - Text Font
```
- 窗体给弄乱了，怎么办?  
`window - Reset Perspective`- 控制台找不到了，怎么办?
`Window - Show View — Console`
## Eclipse中快捷键的使用
- 
内容辅助键
- Alt+/ 起提示作用
- main+alt+/,sout+alt+/,给出其他提示 

（ 新eclipse不content   assist 的快捷键变成 ctrl   +   space了，在Windows-> Preferences-> General-> Keys改回；特别注意：其中when的选择不要弄错选择Editing text（包含in java， in jsp））

- 
常用快捷键
- 格式化  ctrl+shift+f
- 导入包  ctrl+shift+o 

> 
如果该类仅仅在一个包中有，就自己显示了。 

      如果该类在多个包中有，会弹出一个框框供你选择
- 注释  

单行：注释 ctrl+/，取消注释再来一次。 

  多行：ctrl+shift+/,ctrl+shift+\ 

  JavaDoc注释：alt + shift + j- 代码上下移动 选中代码alt+上/下箭头
- 查看源码  选中类名(F3或者Ctrl+鼠标点击)

## Eclipse中如何提高开发效率
- A:自动生成构造方法 
- 无参构造方法 在代码区域右键–source–Generate Constructors from Superclass
- 带参构造方法 在代码区域右键–source–Generate Constructors using fields..–finish

- 
B:自动生成getXxx()/setXxx()方法
- 在代码区域右键–source–Generate Getters and Setters…
- 提供了对应的快捷键操作。        alt+shift+s 

按下带有下划线的那个字母即可。

- 
C:如何继承抽象类和实现接口。

- D:Override的作用 

    表示该方法是重写父类的。如果方法声明和父类不匹配，就会报错。

## 删除项目和导入项目
- 
删除项目  
- 选中项目 – 右键 – 删除 

从项目区域中删除 

从硬盘上删除- 
导入项目
- 在项目区域右键找到import 

找到General，展开，并找到        Existing Projects into Workspace 

点击next,然后选择你要导入的项目 

注意：这里选择的是项目名称  
## Eclipse中代码的高级(Debug)调试
- 
作用：       调试程序        查看程序执行流程

- 
如何查看程序执行流程 

要想看程序流程，就必须设置断点。

- 
什么是断点：    就是一个标记，从哪里开始。 

如何设置断点： 你想看哪里的程序，你就在那个有效程序的左边双击即可。

- 
在哪里设置断点：  哪里不会点哪里。 

目前：我们就在每个方法的第一条有效语句上都加。

- 
如何运行设置断点后的程序： 

右键 – Debug as – Java Application

- 
看哪些地方： 

        Debug：断点测试的地方 

        在这个地方，记住F6，或者点击也可以。一次看一行的执行过程。 

        Variables：查看程序的变量变化 

        ForDemo：被查看的源文件 

        Console：控制台  - 
如何去断点： 

a:再次双击即可 

b:找到Debug视图，Variables界面，找到Breakpoints，并点击，然后看到所有的断点，最后点击那个双叉。













