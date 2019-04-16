# reportlab教程1--第一个pdf生成 - lyx的专栏 - CSDN博客





2017年07月09日 21:05:45[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：5079








        实际生活工作中，我们会希望有些报告、图表可以自动生成，然后变成pdf，甚至直接发邮件到某个制定邮箱lib。这个时候有几种方式可以来实现，譬如用latex，但是这个似乎还要在电脑上装很多东西。还有一个pdfkit的东西，直接把html转成pdf，不过也要装一个插件。虽然实现起来可能pdfkit更简单，但是从功能角度来讲，似乎是report更加强，而且文档也丰富。

1.第一个reportlib的demo

        安装就不说了，pip即可。



```python
from reportlab.pdfgen import canvas

def hello(c):
    c.drawString(100, 100, "Hello World")
c = canvas.Canvas("hello.pdf")
hello(c)
c.showPage()
c.save()
```


        首先第一步是建立一个pdf对象，用canvas.Canvas实例化即可，然后我们就获得了一个Cancas对象。这个对象有很多方法。这些方法分为两类，一类用于设置、操作，譬如设置颜色，设置字体。例如这里的showPage()就表示此页完毕，转到下一页。另一类则用于在pdf上绘制或者说写入东西，例如上面的drawString()。当我们对pdf编辑完成了，只要调用save方法，reportlib就会把pdf文件输出保存，我们就可以打开看了。


        上面的例子在实例化Canvas的时候，我们只是传入了一个pdf的文件名，而我们知道，实现一个类的时候，我们可以看一下这个类的构造函数，来看一下实例化的时候可以设置哪些参数？



```python
def __init__(self,filename,
                 pagesize=None,#页面尺寸
                 bottomup = 1,
                 pageCompression=None,
                 invariant = None,
                 verbosity=0,
                 encrypt=None,
                 cropMarks=None,
                 pdfVersion=None,
                 enforceColorSpace=None,
                 initialFontName=None,
                 initialFontSize=None,
                 initialLeading=None,
                 cropBox=None,
                 artBox=None,
                 trimBox=None,
                 bleedBox=None,
                 ):
```

         这里我们只讨论一下pagesize这个参数。默认下，是A4大家，我们可以改变这个限制。首先，我们要知道，pagesize的参数类型是一个tuple()，(595.27,841.89)类似于这样。




```python
from reportlab.lib.pagesizes import letter, A4
myCanvas = Canvas('myfile.pdf', pagesize=letter)
width, height = letter #keep for later#获得tuple，后面可以使用，譬如可以利用这个设置一个可通用的边际留白大小
```
bottomup 这个变量是用于改变坐标系的。pdf绘制或者书写的坐标系是最左下方为(0,0)，这和很多GUI保持一致，笔者不介意大家在这里改变这个参数。



       pageCompression是用于选择我们是否对pdf进行压缩，以至于可以减少存储的空间。verbosity是用于选择pdf生成的运行过程中，信息的输出多少。encrypt自然是选择是否对pdf进行加密。

2.复杂一点的demo

        我们把上面的hello函数变成下面这个。



```python
def hello(c):
    from reportlab.lib.units import inch
    # move the origin up and to the left
    c.translate(inch,inch)
    # define a large font
    c.setFont("Helvetica", 14)
    # choosesome colors
    c.setStrokeColorRGB(0.2, 0.5, 0.3)
    c.setFillColorRGB(1, 0, 1)
    # draw some lines
    c.line(0, 0, 0, 1.7 * inch)
    c.line(0, 0, 1 * inch, 0)
    # draw a rectangle
    c.rect(0.2 * inch, 0.2 * inch, 1 * inch, 1.5 * inch, fill=1)
    # make text go straight up
    c.rotate(90)
    # change color
    c.setFillColorRGB(0, 0, 0.77)
    # say hello (note after rotate the y coord needs to be negative!)
    c.drawString(0.3 * inch, -inch, "Hello World")
```
        就可以看到在pdf的左下角，有这样的一个图案：


![](https://img-blog.csdn.net/20170708114505487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        很多方法看一下名称就可以知道了。关键是，我们要知道有哪些方法可以用。

        线条绘制，给出起始点坐标就可以绘制线条，



```python
canvas.line(x1,y1,x2,y2)
canvas.lines(linelist)
```
        还可以绘制很多多边形 或者网格：




```python
canvas.grid(xlist, ylist)
canvas.bezier(x1, y1, x2, y2, x3, y3, x4, y4)
canvas.arc(x1,y1,x2,y2)
canvas.rect(x, y, width, height, stroke=1, fill=0)
canvas.ellipse(x1,y1, x2,y2, stroke=1, fill=0)
canvas.wedge(x1,y1, x2,y2, startAng, extent, stroke=1, fill=0)
canvas.circle(x_cen, y_cen, r, stroke=1, fill=0)
canvas.roundRect(x, y, width, height, radius, stroke=1, fill=0)
```
        还有在pdf上写字符：




```python
canvas.drawString(x, y, text):
canvas.drawRightString(x, y, text)
canvas.drawCentredString(x, y, text)
```
        上面的drawStrinng的方法都是写一行，而我们可以用text方法写入多行，相对而言就复杂一些：




```python
textobject = canvas.beginText(x, y)
canvas.drawText(textobject)
```


















