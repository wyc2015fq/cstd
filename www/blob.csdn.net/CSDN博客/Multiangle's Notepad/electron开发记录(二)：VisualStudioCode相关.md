
# electron开发记录(二)：VisualStudioCode相关 - Multiangle's Notepad - CSDN博客


2016年11月29日 22:29:09[multiangle](https://me.csdn.net/u014595019)阅读数：5749



---这篇文章主要写的是跟vscode有关的内容.毕竟这次是打算拿vscode来开发应用的

---主要内容:

---1.vscode的安装

---2.使用vscode运行electron的demo

---3.安装typings开启智能提示功能

---4.vs的快捷键和其他设置

---1.VSCode安装

---VSCode的安装比较简单，去官网

---[https://code.visualstudio.com](https://code.visualstudio.com)

---下载deb包，然后

---$

---sudo

---dpkg -i code_1.

---7.2

----

---1479766213

---_amd64.deb

---来进行安装. 注意下deb包的名字应该是你自己下载得到的deb文件的名字.

---如果碰到dpkg安装失败的(我是没碰到,不过用dpkg安装软件有时候会出现这种情况),可能是因为缺少依赖包,可以尝试一下

---$

---sudo

---apt-get

----f

---install

---安装一下缺失的依赖包,然后再运行上面的代码

---vscode安装成功以后,可以在shell中通过

---code

---指令来打开vscode.

---2.使用vscode运行electron

---在之前的内容中已经实现了在shell中运行electron的demo.现在我们想办法能够在vscode中将electron程序运行起来.还是拿之前下好的demo工程为例.找到demo工程所在的文件夹,使用vscode打开文件夹

---$ code electron

----quick

----start

---首先来看看vscode的侧边栏

![选区_048.png-14.9kB](http://static.zybuluo.com/multiangle/kaguqgayjf74ij0ky8x5vy4e/%E9%80%89%E5%8C%BA_048.png)
---[ ](http://static.zybuluo.com/multiangle/kaguqgayjf74ij0ky8x5vy4e/%E9%80%89%E5%8C%BA_048.png)

---这边有5个按钮，一般箭头指的3个是比较常用的。

---文件管理

---用来管理文件，比如新建，删除等。

---git操作

---用来进行git的各项操作，可以将文件提交到本地，还可以对比查看

---debug

---则用来运行、调试程序。我们现在就是要用debug功能。

---按ctrl shift d或者点击左侧蜘蛛状按钮【1】，跳转到调试界面，再点击上面的齿轮按钮，打开launch.json【2】

![选区_045.png-13.5kB](http://static.zybuluo.com/multiangle/0b9791rl3y3isfw21uiqr1rt/%E9%80%89%E5%8C%BA_045.png)
---之后将如下launch.json文件中的configuration选项改成如下形式

---"configurations"

---: [
        {

---"name"

---:

---"Debug Main Process"

---,

---"type"

---:

---"node"

---,

---"request"

---:

---"launch"

---,

---"program"

---:

---"

---${workspaceRoot}

---/main.js"

---,

---"cwd"

---:

---"

---${workspaceRoot}

---"

---,

---"runtimeExecutable"

---:

---"

---${workspaceRoot}

---/node_modules/.bin/electron"

---}
    ]

---其实主要是添加一条runtimeExecutable的参数，告诉vscode应该通过这个文件来运行。

---保存后，点击上图中的绿色三角形，就能运行啦

---3.安装typings开启智能提示功能

---由于nodejs是弱类型语言，所以vscode自己是无法完成代码的智能提示的。最多只能根据当前文件中已经出现的方法来提示。但是这样又容易引起混淆。为了解决这个问题，我们需要安装typings。

---typings全称是TypeScript Definition Manager(虽然我怎么看都觉得缩写跟全称不符)，用来取代tsd。

---我是参照这里的：

---[http://www.cnblogs.com/IPrograming/p/VsCodeTypings.html](http://www.cnblogs.com/IPrograming/p/VsCodeTypings.html)

---操作流程如下：

---假设你现在正在项目的根目录下。也就是当前目录下有node_modules文件夹,package.json这些。然后输入如下代码

---npm install

----g

---typings

---//　全局安装typings

---typings install dt~node

-----

---global

-----

---save

---//　安装node的类型接口信息文件。其中dt表示源，node表示名称

---typings install dt~electron

-----

---global

-----

---save

---// 安装electron接口信息。

---关于什么时候要加–global参数：

---如果安装的包使用script标记来引用(如jQuery)(也就是在浏览器中使用)

---这个包是属于环境的一部分(如node)时

---该包没有使用 –global 安装失败时（或者提示你要加global参数时）

---安装好以后，可以看到在根目录下会多出两样东西：typings.json文件和typings文件夹。前者类似于package.json，后者类似于node_modules.　即typings.json是一个索引。多人协作时，只要获得typings.json，然后使用

---typings install

---就能够生成typings文件夹。此外，还可以使用

---typings search packname

---来看是否支持对某个包的智能提示

---接下来，还需要开启vscode的智能提示功能。方法有两种：

---是在需要进行只能提示的文件最上行增加提示信息文件所在目录，格式如下:

---/// <reference path="./typings/index.d.ts" />

---在项目的根目录增加一个名为jsconfig.json的空文件,往其中写入：

---{
    "

---compilerOptions

---":

---{
        "

---target

---":

---"ES6"

---}

---,
    "

---exclude

---":

---[

---"node_modules"

---]

---}

---更多jsconfig.json文件的内容可以参考：

---[https://code.visualstudio.com/docs/languages/javascript](https://code.visualstudio.com/docs/languages/javascript)

---4.vscode的快捷键和其他设置

---文件->首选项->更换主题 改变配色

---ctrl+/       注释当前行

---ctrl+d       选中当前光标所在单词

---ctrl+shift+k 删除当前行

---ctrl+shift+i reformat代码

---shift+alt+up/down 选中多行

---shift+alt+鼠标拖动 选中一整块

---个人体验

---优点：

---1. 可以直接在界面上进行git部分操作,可以存到本地。提交到github的话，还是要在shell中进行

---2. 可以直接运行node程序，这个点赞

---3. VS风格界面，很喜欢

---缺点：

---1. 比之sublime，打开速度稍稍有些慢

---2. 写html的时候，自动补全不够智能(跟idea相比)

---3. 写nodejs补全不够智能，不过可以安装typings来弥补


