# R软件图形参数的设置 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:26:50[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1559









在R中，设置图形参数是使用par()函数，其中有必须在par里设置的

## 坐标系

参数 cex

cex=cex_type,#绘图符号大小，1是正常大小

cex.axis=1.1,#坐标轴刻度数字大小

cex.lab=1.2,#坐标轴标签大小

cex.main=1.4,#标题文字大小

cex.sub=1.3,#副标题文字大小

font.axis=3,#坐标轴刻度数字字体，斜体

font.lab=2,#坐标轴标签字体，粗体

font.main=4,#标题字体，粗斜体

font.sub=1,#副标题字体，正常

画图时，可以使用axes=FALSE，去掉坐标轴，也可以通过axis函数来添加坐标轴。例如：

axis(1，at=...,labels=...)

其中1代表下方，2代表左方，3代表上方，4代表右方。

at为刻度值，是等间隔的。labels加上数据标签。这样可以通过axis函数自行设置刻度内容。

或者直接在plot()函数里加上xaxt或yaxt="n"，这样就不显示坐标刻度，再使用axis（）加上自己设置的刻度。

坐标刻度方向是使用las表示的，等于0 表示总是平行于坐标轴，1 表示总是水平，以及2 表示总是垂直于坐标轴

图形边框的设置参数为bty=...，参数”o“ 为默认设置，为四边框，”l“"7""c""u"或"]"，对应边框和这些字母形状相似，当为"n"时为无边框

tck参数是标记刻度的长度，当值>0时，刻度指向图形内，<0时，指向图形外，默认值<0.

2. fin  绘图区的宽和高，mar设置图形边界，这两项只有在par函数里才可设置，就像mfrow一样。



## legend函数

legend(x, y = NULL, legend, fill = NULL, col = par("col"),
       border = "black", lty, lwd, pch,
       angle = 45, density = NULL, bty = "o", bg = par("bg"),
       box.lwd = par("lwd"), box.lty = par("lty"), box.col = par("fg"),
       pt.bg = NA, cex = 1, pt.cex = cex, pt.lwd = lwd,
       xjust = 0, yjust = 1, x.intersp = 1, y.intersp = 1,
       adj = c(0, 0.5), text.width = NULL, text.col = par("col"),
       text.font = NULL, merge = do.lines && has.pch, trace = FALSE,
       plot = TRUE, ncol = 1, horiz = FALSE, title = NULL,
       inset = 0, xpd, title.col = text.col, title.adj = 0.5,
       seg.len = 2)

legend函数参数比较多，但最主要的有几个。如例：
legend("top",legend=c("w","d"),lty=1:2,lwd=2,col=c("red","blue") 
其中，lty不可少，否则图形不显示。
其他图形参数比较简单，不再陈述。



