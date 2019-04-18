# tkinter实现GUI：管理工具函数 - 知乎
# 

## **函数太多后的管理问题**

用过一段时间python后，文件夹里会堆积出大量的各种功能的工具函数。由于函数名只能是英文缩写，而且只能用文件夹来分类，所以经常发现以前写过的函数找不到，而且功能近似或存在上下游关系的函数不能按照逻辑关系展示在临近的位置，导致使用起来思维混乱，效率低下。

## **GUI可以解决这个问题**

GUI界面上，你可以为每个函数重新匹配一个中文功能名称的按钮；可以为每个函数设计合适数量的输入框，便于提交input；并且利用GUI的布局功能，将功能类似的函数放在同一区块，将上下游关系的函数按照合理的顺序展示；还可以利用弹出窗口的模式来处理更精细的逻辑关系。
![](https://pic1.zhimg.com/v2-7273ad56998cf1ecd2c7e65c2ef7c69c_b.jpg)
## **tkinter**

这是python的一个实现GUI的模块，使用起来非常简便（如果你对比用过MATLAB的GUI功能，就会有强烈共鸣）。简言之，就是每个函数对应GUI上一个按钮，鼠标点一下就调用对应的那个工具函数。你只需按需求设计好按钮和输入框的布局就可以了。

tkinter中，定义一个GUI界面，就是定义一个类，其中主题是一个window对象，之后pack上各种按钮和标签、输入框、画布等组件。最简单的tkinter的GUI如下示例，直接在示例代码上做修改即可。

## **示例代码(Python3)**

```python
import tkinter as tk # 导入tkinter模块

# 定义GUI：我的函数库
class MyGUI:
    def __init__(self):
        window = tk.Tk() # 生成一个窗口对象
        window.title('我的函数库') # 设置窗口标题
        
        # frame1：市场监控
        frame1 = tk.Frame(window) # 生成第一个框架
        frame1.pack() # 将框架1放入窗口
        tk.Label(frame1, text = '市场监控').pack(side='left') # 生成框架标题
        tk.Button(frame1, text="计算实时期指基差情况", command=calCurIndexFtrsBasis).pack(side='left') # 插入按钮
        tk.Button(frame1, text="绘制主要股指近20日走势图", command=plotMainStkIndexCurve20Day).pack(side='left') # 插入按钮
        tk.Button(frame1, text="估算主动股基平均仓位", command=estStkFundAvgPosition).pack(side='left') # 插入按钮

        # frame2：期货数据库管理
        frame2 = tk.Frame(window) # 生成第二个框架
        frame2.pack() # 将框架2放入窗口
        tk.Label(frame2, text = '期货数据库管理').pack(side='left') # 生成框架标题
        tk.Button(frame2, text="检查期货数据更新情况", command=checkFtrsData).pack(side='left') # 插入按钮
        tk.Button(frame2, text="更新期货合约列表", command=updateFtrsList).pack(side='left') # 插入按钮
        tk.Button(frame2, text="更新期货行情数据", command=updateFtrsQuote).pack(side='left') # 插入按钮
        
        # frame3：杂放工具
        frame3 = tk.Frame(window) # 生成第三个框架
        frame3.pack() # 将框架3放入窗口
        tk.Label(frame3, text = '我的杂放工具').pack(side='left') # 生成框架标题
        tk.Button(frame3, text="分形图绘制", command=plotFractal).pack(side='left') # 插入按钮
        tk.Button(frame3, text="多体运动轨迹数值模拟", command=plotMultiBodyMotion).pack(side='left') # 插入按钮
        tk.Button(frame3, text="抓取10大媒体财经头条", command=capHeaderOfTop10FinancialNews).pack(side='left') # 插入按钮
       
        window.mainloop() # 创建事件循环（不必理解，照抄即可）

MyGUI() # 启动GUI
```

注：每一个tk.Button(...)中的command参数就指向了拟调用的函数（请替换为自己的函数）。

