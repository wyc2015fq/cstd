
# electron开发记录(四)：electron中组件的一些用法 - Multiangle's Notepad - CSDN博客


2016年12月08日 12:06:27[multiangle](https://me.csdn.net/u014595019)阅读数：5419



---之前讲了electron应用的基本框架和结构。现在来讲讲其中一些组件(BrowserWindow，Menu等)的一些用法。这些方法我是从

---[electron-api-demo](https://github.com/electron/electron-api-demos)

---以及electron官网上的

---[文档](http://electron.atom.io/docs/)

---中看来的，自己整理了下，挑了点我觉得有用和项目中用到的用法说一下

---主要内容：

---1.BrowserWindow的用法

---2.main进程与renderer进程间通信

---1. BrowserWindow

---BrowserWindow的功能主要是建立新窗口。

---一般流程

---是：建立对象->加载网页。建立对象以后，只是一个空的窗口，只有页面加载完以后才能显示内容。

---win =

---new

---BrowserWindow(

---// 建立window对象

---width:

---1300

---,
    height:

---600

---）
win.loadURL(

---'file:'

---+__dirname+

---'index.html'

---)

---// 加载项目文件夹下的index.html页面

---BrowserWindow的常用属性

---在之前的demo中，创建BrowserWindow对象只是用了width和height两个属性。其实，还有很多其他的属性可用

---1.

---width

---: 宽度

---2.

---height

---: 高度

---3

---backgroundColor

---: 背景颜色，例如\#2e2c29

---4.

---parent

---: 是否指定其父窗口。如果要指定，则需要输入父窗口的变量名。如果父窗口被关闭，则子窗口也被关

---5.

---show

---: 是否立即显示。如果=true，则立即显示；如果=false，则在以后调用win.show()显示。一般来讲，可以在放在’ready-to-show’的情况下

---win.once

---(

---'ready-to-show'

---,()=>{
    win.show()
    })

---6.

---icon

---: 如果要指定应用的图标，则需要指定图标位置

---当然了，上面这些属性也只是一部分，其他还有x,y,movable,closable,alwaysOnTop,fullscreenable等多种属性，具体可见

---[http://electron.atom.io/docs/api/browser-window/](http://electron.atom.io/docs/api/browser-window/)

---BrowserWindow对象的使用

---这里假设win即为BrowserWindow创建的对象。

---win.on(channel,callback)

---这个on方法其实不只是BrowserWindow独有的，很多其他地方都有，之前也提到过。event指的是事件名称，而callback指的是接收到这个事件以后的响应。win.on可以响应很多事件，我用到的有两种，分别是’closed’和’window-all-closed’，分别表示当前窗口关闭和所有窗口关闭时的行为。其他的还有responsive, focus等很多其他事件。

---win.once(channel,callback)

---这个我看到的只能响应’ready-to-show’信道。

---static方法

---这个静态方法的我的理解就是跟java中的static方法类似，是属于类本身的方法而非类对象的方法。BrowserWindow类的方法有以下这些:

---BrowserWindow.getAllWindows()

---返回BrowserWindow[]，返回所有窗口。

---BrowserWindow.getFocusedWindow()

---返回当前锁定的窗口

---BrowserWindow.fromWebContents(webContents)

---看样子是从网页获得一个窗口？

---其他还有一些用法，也不全列了，感兴趣的可以去上面列出的网址看

---win.show()

---之前已经出现过了，如果在创建窗口的时候show=false, 则调用win.show()能够让窗口显示。

---win的其他方法

---BrowserWindow的对象还有很多其他方法，例如destory(),close(),focus(),isFocused(),hide()等方法。

---main进程与renderer进程通信

---在写应用的时候，难免会出现前端(renderer)进程要与后台(main)进程之间相互通信的情况。在electron下，renderer进程与main进程相互通信要通过ipc(Inter-Process Communication，进程间通信)模块来完成。注意，

---main与renderer进程调用的ipc模块是不一样的

---。main进程需要调用electron.ipcMain,而renderer进程则要调用electron.ipcRenderer。具体的示例代码如下：

---renderer进程内：

---const

---ipcr =

---require

---(

---'electron'

---).ipcRenderer ;

---const

---btn =

---document

---.getElementById(

---'msg-button'

---) ;
btn.addEventListener

---(

---'click'

---, ()=>{
    ipcr.send(

---'event-msg'

---,

---'ping'

---) ;
})

---;

---ipcr

---.

---on

---(

---'event-reply'

---, (event, arg)=>{

---const

---message = `

---message reply: ${arg}

---` ;

---document

---.getElementById(

---'msg-reply'

---).innerHTML = message ;
})

---;

---main进程内:

---const

---ipcm =

---require

---(

---'electron'

---).ipcMain
ipcm.

---on

---(

---'event-msg'

---, (event, arg)=>{
    event.sender.send(

---'event-reply'

---,

---'pong'

---)
})

---在运行的时候，顺序是这样:

---首先在renderer进程内，有一个id=msg-button的组件被按下，ipcRenderer向ipcMain发送了一个名叫event-msg的事件，附带参数’ping’

---ipcMain收到了ipcRenderer发来的event-msg事件，并做出响应：向ipcRenderer返回一个叫做event-reply的事件，也附带参数’pong’

---ipcRenderer收到了event-reply事件，同样做出响应：找到页面内id=msg-reply的元素，将其值更改为:message reply: pong

---这样，就完成了renderer与main之间一来一回的两次通信。


