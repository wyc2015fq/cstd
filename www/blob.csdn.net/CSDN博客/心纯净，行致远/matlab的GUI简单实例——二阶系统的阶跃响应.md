# matlab的GUI简单实例——二阶系统的阶跃响应 - 心纯净，行致远 - CSDN博客





2018年01月22日 16:35:48[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：3229标签：[matlab](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)
个人分类：[【MatLab/Simulink】](https://blog.csdn.net/zhanshen112/article/category/7184373)









GUI，Graphical User Interfaces，适合向别人提供某种新的设计分析工具，体现某种新的设计分析理念，进行某种技术、方法的演示，在这方面，图形化用户界面可能是最好的选择之一。

下面是今日刚学的一个例子， 演示二阶系统G（s）=1/(s^2+2*θ*s+1)中阻尼比θ对单位阶跃响应的影响，制作如图所示的GUI。

![](https://img-blog.csdn.net/20180122144801788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

实例很简单，主要目的是通过这个简单的实例学习GUI的各个功能，以及GUI.m文件中函数的各个功能，希望可以达到能够按照自己预期编写完善的GUI。

首先分析上面的GUI，主要包含四个部分：标题部分，函数图像展示部分，可编辑输入部分和右下角备注部分。其中标题和右下角由于不需要变动，可以使用“静态文本”，用户输入部分使用”可编辑文本”，函数图像展示使用“轴”。

在创建空白GUI之后，拖动四个框图进行排列很简单。排列之后，一些界面组件的参数设置很重要，列举如下：

1. 先点击左上方的“选择（select）”，使光标处于选择状态。然后，双击设计工作区“空白处”，引出“Inspector”，在“窗属性检查器”中，设置下列属性值：

Resize on  %该设置很重要。它决定图形窗是否可以缩放，并且可以随时调整大小，“run”之后GUI也会更新

Unitsnormalized   %采用“归一化”单位计量窗口大小

2. 在“静态文本”和“可编辑文本”组件，双击可以进入参数设置界面，其中string 中可以输入需要显示的文本，frontsize，units等参数可以根据需要进行调节。

3.  调整完之后，点击运行按钮，保存初步设计界面。会生成两个文件，一个.m文件，一个.fig文件，前者适合输入GUI的函数，后者适合调整以看实际展示的GUI。

4.  也是最核心的一步，就是回调程序的编写。回调程序的目的是调用用户输入的数据，在.m文件中通过函数语句执行，再将结果输入到GUI界面进行展示。.m文件中回调函数的命令如下：



```
function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
get(hObject,'string');  %从可编辑框读取输入字符
zeta=str2double(get(hObject,'string')); %把字符转换成双精度
handles.t=0:0.05:15;   %定义时间采样数组
handles.y=step(tf(1,[1,2*zeta,1]),handles.t);   %计算响应
**cla   %清空坐标轴**
line(handles.t,handles.y)   %在已有轴上绘制曲线
```
然后保存，重新运行就可以实现开头所说的功能。

注意，加粗的指令“cla”，如果改成“hold on”，绘制的函数图像将不会擦除，这样也便于比较不同阻尼比的阶跃响应。


![](https://img-blog.csdn.net/20180122155839123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下面附文，关于GUI运行原理的解释：

[Matlab的GUI的工作原理及流程](http://blog.csdn.net/yizhou2010/article/details/50379398)







