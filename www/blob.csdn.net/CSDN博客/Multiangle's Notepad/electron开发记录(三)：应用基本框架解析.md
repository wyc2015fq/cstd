
# electron开发记录(三)：应用基本框架解析 - Multiangle's Notepad - CSDN博客


2016年12月02日 16:44:08[multiangle](https://me.csdn.net/u014595019)阅读数：5774



---这篇文章主要讲了electron应用的基本架构，并对之前下载的应用进行分析

---electron应用的基本架构

---在electron中，主要有两类进程。一类是主进程main，还有一类是渲染器进程renderer。

---主进程只有一个，负责对整个应用的管理，包括后台操作，创建GUI，以及处理GUI与后台的交互操作。但是光有主进程是无法显示应用窗口的，我们需要在主进程中调用BrowserWindow模块才能使用不同的窗口。每个窗口会调用各自的渲染器进程(renderer)来将页面渲染到窗口中。

---如果使用图形化表示的话，main进程和renderer进程的关系大概是这样

![64085546_4.jpg-79.7kB](http://static.zybuluo.com/multiangle/2n1yb0o4grndfgz5sji515ll/64085546_4.jpg)
---electron应用的基本文件结构

---了解了main和renderer各自的作用，我们再来看看它的文件结构。还是拿之前的electron-quick-start的demo做例子。

---打开工程文件夹，可以看到下面主要有如下几个文件：

---main.js

---package.json

---renderer.js

---index.html

---node_modules (npm install以后出现)

---这些文件组成了一个完整的electron应用。其他的文件都是一些辅助功能，例如.gitignore是用于git操作的。下面来一一解释上面这些文件的作用

---package.json

---这个文件基本可以看做是整个工程的参数文件，json格式。里面有很多参数，例如name,version等等。这么多的参数中，

---比较关键的是下面这几项：

---{
  "

---main

---":

---"main.js"

---,
  "

---scripts

---":

---{
    "

---start

---":

---"electron ."

---}

---,
  "

---devDependencies

---":

---{
    "

---electron

---":

---"^1.4.1"

---}

---,
  "

---dependencies

---":

---{
    "

---bootstrap

---":

---"^3.3.7"

---,
    "

---jquery

---":

---"^3.1.1"

---}

---}

---其中scripts-start指的是用什么程序来执行，main指的是程序的入口。所以这两项合起来看，就是告诉系统要用electron来执行main.js文件。

---后面的devDependencies和dependencies指的是依赖项。其中devDependencies是全局依赖，dependencies虽然demo中没出现，但是后来也是会用到的，表示局部依赖项。这两项不用自己写，使用npm安装的时候加–save参数就会自动写入该文件。

---npm

---install

---xxx

----

----

---save

---//

---局部依赖

---npm

---install

---xxx

----

----

---global

----

----

---save

---//

---全局依赖

---node_modules

---这个是一个文件夹，里面装的是应用所需要的依赖项内容。在写好package.json文件以后，使用

---npm

---install

---就能够自动下载并安装package.json文件中要求的所有依赖项了。非常的好用简单。唯一的问题是npm对依赖项之间相互调用的问题好像没什么优化，要下非常多的东西，光quick-start就需要下135M的内容，可以考虑将其他地方的node_modules直接复制过来。

---index.html

---是一个网页，我们的应用要做的就是要将这个网页渲染进窗口中去。

---main.js

---这个是对主进程的描述。这个值得好好讲一下。我这边只贴部分代码，全代码可以去

---[https://github.com/electron/electron-quick-start](https://github.com/electron/electron-quick-start)

---看

---首先是引入electron和其他有用的库。因为是引入的类，所以都用const作为常量。

---const

---electron =

---require

---(

---'electron'

---)

---// Module to control application life.

---const

---app = electron.app

---// Module to create native browser window.

---const

---BrowserWindow = electron.BrowserWindow

---const

---path =

---require

---(

---'path'

---)

---const

---url =

---require

---(

---'url'

---)

---现在常量app表示应用本身，可以用app来操作应用的行为。例如

---app.

---on

---(

---'ready'

---, createWindow)
app.

---on

---(

---'window-all-closed'

---,

---function

---() {

---//

---On OS X it

---is

---common

---for

---applications

---and

---their menu bar

---//

---to stay active

---until

---the user quits explicitly

---with

---Cmd + Q

---if

---(process.platform !==

---'darwin'

---) {
    app.quit()
  }
})

---由于nodejs中异步的频繁使用，所以个人觉得node的程序结构跟其他的似乎有点不太一样。这里xxx.on是一种很常见的用法，有点类似于触发器，一旦某个事件被触发，就调用某个指定的方法，例如

---app.

---on

---(

---'ready',createWindow)

---意思就是当app的ready事件发生以后，就调用createWindow函数。

---在electron的api中，管第一个参数叫’channel’，信道

---，那么也可以理解成当app监听到’ready’信道有新消息以后，就调用createWindow函数。

---那既然提到了createWindow，我们就来看看createWindow函数。

---function

---createWindow () {

---//

---Create the browser

---window

---.
  mainWindow =

---new

---BrowserWindow({

---width

---:

---800

---,

---height

---:

---600

---})

---//

---and

---load the index.html

---of

---the app.
  mainWindow.loadURL(url.format({

---pathname

---: path.join(__dirname,

---'index.html'

---),

---protocol

---:

---'file:'

---,

---slashes

---:

---true

---}))

---//

---Open the DevTools.
  mainWindow.webContents.openDevTools()

---//

---Emitted

---when

---the

---window

---is

---closed.
  mainWindow.

---on

---(

---'closed'

---,

---function

---() {

---//

---Dereference the

---window

---object, usually you would store windows

---//

---in

---an array

---if

---your app supports multi windows,

---this

---is

---the time

---//

---when

---you should

---delete

---the corresponding element.
    mainWindow =

---null

---})
}

---首先，使用

---new BrowserWindow

---来建立新的窗口。这边BrowserWindow还有很多其他的参数，这里就暂时不展开了。

---此时mainWindow还是一个空白的窗口，那么就需要用

---loadURL

---来载入之前提到过的index.html文件。

---接下来的

---mainWindow.webContents.openDevTools()

---方法，可以保留也可以注释掉，如果保留则在应用启动的时候会打开调试界面。

---最后的

---mainWindow.on('closed',...)

---熟悉吧，又是xxx.on的用法。关闭窗口以后的操作。

---renderer.js

---在这里是空的，也不是必须的文件，但是这是因为demo中页面很简单的缘故，也不需要与后台发生什么交互。如果页面复杂的话，则可以考虑将js交互部分放入renderer.js中，然后在页面中

---<script> require('renderer.js')</script>

---这样可以起到一定的精简作用。如果有多个页面，则可以分别放在多个renderer文件中。当然，这并不是必须的。

---electron应用的其他文件和一些建议

---以上只是一个最简单的应用的文件结构。当你要做一个相对复杂一些的项目的时候，文件的数量会快速增加，如何管理这些文件也是一个问题。一般来说，在electron项目中还会增加如下几项:

---sections文件夹

---：用于存放页面文件

---assets文件夹

---：用于存放图像，renderer进程的js文件，css文件等

---如果你使用了typings作为智能提示，那么还会有如下文件：

---typings.json

---: typings的配置文件

---typings文件夹

---: typings根据配置文件下载得到的各库接口文件

---jsconfig.json

---: 用于提示vscode开启智能提示

---关于typings的安装和使用，可以参照

---[这里](http://blog.csdn.net/u014595019/article/details/53401303)

---如果你还使用了git来进行版本管理，那么还会有如下文件：

---.git文件夹

---: 自动生成的

---.gitignore

---: 提示git应该忽略什么文件。例如上面的node_modules和typings文件夹,内容很多，而且是可以根据各自的配置文件package.json和typings.json生成的。那么就不应该将这两个文件夹加入git管理范围。这时.gitignore就应该写入

---/node_modules/

---*

---/typings/

---*


