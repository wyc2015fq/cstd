# Ueditor1.3.6 setContent的一个bug - =朝晖= - 博客园
# [Ueditor1.3.6 setContent的一个bug](https://www.cnblogs.com/dhcn/p/7115263.html)
      Baidu Uedtior这个版本的占位标签为script标签，UE.get("editor")操作初始化编辑器，这个初始化动作似乎是个异步动作，在这个语句之后如果setContent就会报错：
```
Uncaught TypeError: Cannot read property 'innerHTML' of undefined
```
     可以用setTimeout语句把setContent操作时间线网往后移，以上错误就不会发生了。

