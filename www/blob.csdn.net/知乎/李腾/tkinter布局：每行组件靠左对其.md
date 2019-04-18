# tkinter布局：每行组件靠左对其 - 知乎
# 

众多的工具函数放在一张GUI界面上，必然要分很多行。多数情况下，每行的宽度不同，如果每行组件总是居中对齐，会比较难看，例如下图：
![](https://pic2.zhimg.com/v2-740cb463360a9045a52e7838a86aca09_b.jpg)
我就一直想改为每行靠左对其，但一直没琢磨出来。后来动手专门试了半天才解决，关键就是要用好**grid()函数中的sticky这个参数。**

## 例程

```
# 例程：每行一组组件，每行组件都靠左对其
import tkinter as tk

class MyGUI():
    window = tk.Tk() # 生成一个窗口对象
    window.title('我的GUI') # 设置窗口标题
    
    # 第一组按钮
    frame1 = tk.Frame(window) # 生成第一组按钮的容器
    frame1.grid(row=0, column=0, sticky='w') # sticky='w'指定了组件在单元格中靠左对齐
    tk.Label(frame1, text = '第一组').pack(side='left') # 添加本组标题
    tk.Button(frame1, text="按钮1").pack(side='left') # 添加按钮
    tk.Button(frame1, text="按钮2").pack(side='left') # 添加按钮
    tk.Button(frame1, text="按钮3").pack(side='left') # 添加按钮
    
    # 第二组按钮
    frame2 = tk.Frame(window) # 生成第一组按钮的容器
    frame2.grid(row=1, column=0, sticky='w') # sticky='w'指定了组件在单元格中靠左对齐
    tk.Label(frame2, text = '第二组').pack(side='left') # 添加本组标题
    tk.Button(frame2, text="按钮4").pack(side='left') # 添加按钮
    tk.Button(frame2, text="按钮5").pack(side='left') # 添加按钮
    
    # 第三组按钮
    frame3 = tk.Frame(window) # 生成第一组按钮的容器
    frame3.grid(row=2, column=0, sticky='nw') # sticky='w'指定了组件在单元格中靠左对齐
    tk.Label(frame3, text = '第三组').pack(side='left') # 添加本组标题
    tk.Button(frame3, text="按钮6").pack(side='left') # 添加按钮
    tk.Button(frame3, text="按钮7").pack(side='left') # 添加按钮
    
    window.mainloop() # 创建事件循环（不必理解，照抄即可）

MyGUI() # 启动GUI
```

## **运行结果**
![](https://pic3.zhimg.com/v2-0f90d27f2c60c7bb65cbad4471b06b66_b.jpg)

