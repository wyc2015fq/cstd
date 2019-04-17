# 【Chrome-crx】键盘模拟鼠标点击网页按钮 - CD's Coding - CSDN博客





2017年10月11日 17:12:40[糖果天王](https://me.csdn.net/okcd00)阅读数：2922








## 0x00 前言

最近在做一个标注任务： 

简单的说，就是屏幕上有好多小方块，选中其中的部分， 

然后在页面下面的四个标注按钮（红绿蓝黑）上点一下， 

小方块们就会被染成这个按钮的颜色，需要把所有的小方块们染色完。
请各位想象一下：纯鼠标工作，在一个白底的网页上来回翻飞舞动，眼睛都看花啦！ 

所以我决定想个法子把这四个染色按钮绑在我键盘上的几个键上（QWER就是你们了）。 

（以后标注起来看起来在打DOTA一样哈哈哈）
## 0x01 配置使用

二话不说先贴解决方案：
- 装个插件： [ShortKey for Chrome](https://chrome.google.com/webstore/detail/shortkeys-custom-keyboard/logpjaacgmcbpdkdchjiaagddngobkck)
- 配置按键： 
- Keyboard Shortcut（选择按键）: `Q`
- Behavior（选择效果）: `Run JavaScript`
- Label as（取个名字）: `"按下C按钮"`
- Javascript code to run（跑一段javascript代码）:  
- `document.getElementsByClassName('btn btn-C')[0].click()`
- （当然可以写好多，这里我要求不多就写了一行）

- Active while in form fields（选择生效页面） 
- Only for specific sites（我选的是只在这个网域生效）
- www.blablablabla.com/mark/todo/* 

- 刷新页面，开始使用 
- 如上述配置的意思为： 
- 仅在标注页面的url域生效，
- 按下键盘中的Q键，
- 等效于点击class名称为’btn btn-C’的按钮一次。


## 0x02 详细说明

其实我觉得叫详细说明不如叫心路历程哈哈哈

首先这是我在Chrome商店搜索shortkey直接淘出来的一个扩展程序：

> 
**Shortkeys** (Custom Keyboard Shortcuts) 

  由Mike Crittenden提供


看起来还不赖，我就去check了一下支持哪些功能（要知道，扩展性是一个程序的生命线啊） 

然后看到这一行的时候我的眼镜就闪闪发光了——“Behavior: `Run JavaScript`” 

好的就决定是你了，能跑一个脚本语言比什么不强？
然后就是javascript了，哼╭(╯^╰)╮，本大爷怎么可能有五分钟内不能写出来的东西

```java
$('.btn-C').click()
//Uncaught ReferenceError: $ is not defined at <anonymous>:1:1
```

这……不科学，怎么连`$`都不认识呢，明明我在console里可以直接用的啊！ 

那这样行么：

```java
document.getElementById('btn btn-C').click()
//Uncaught TypeError: Cannot read property 'click' of null at <anonymous>:1:37
```

啥……Null？那就是说没拿到咯……然后进行了如下的尝试，看如何能获得那个button对象

```
document.getElementById('.btn')
null
document.getElementById('.btn-C')
null
document.getElementById('btn btn-C')
null

document.getElementsByClassName('.btn')
[]
length: 0
__proto__: HTMLCollection

document.getElementsByClassName('btn btn-C')
[button.btn.btn-C]
length: 1
0: button.btn.btn-C
__proto__: HTMLCollection
```

好好好，有对象了有对象了（总觉得这句话哪里不对） 

那就决定是你了！ 
`document.getElementsByClassName('btn btn-C').click()`
啊咧怎么又……哦对了是个list不好意思哈哈哈 
`document.getElementsByClassName('btn btn-C')[0].click()`

搞定，看我一个QEQWR连招带走对面（老师我真的在标注，真的真的~）









