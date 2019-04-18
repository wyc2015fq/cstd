# matplotlib学习笔记.CookBook - wishchinYang的专栏 - CSDN博客
2013年08月08日 14:46:16[wishchin](https://me.csdn.net/wishchin)阅读数：8179
matplotlib 是Python下的一个高质量的画图库，可以简单的类似于MATLAB方法构建高质量的图表。
原始文章地址：[http://zanyongli.i.sohu.com/blog/view/195717824.htm](http://zanyongli.i.sohu.com/blog/view/195717824.htm)
学习心得：
笔记参照《用Python做科学计算》的matplotlib部分。
凡例：a. [float]表示对象类型是float，用时不用加'['和']'。b. 代码中的跳格表示换行。
Chap 5 matplotlib-绘制精美的图表
1. artist对象的set_*方法和plt.setp()函数：line.set_antialiased(False),plt.setp(lines,color='r')，其中antialiased指反锯齿效果。
2. plt.getp()只能对一个对象进行操作：指定属性名，返回对象的指定属性的值；以及不指定属性名，显示出对象的所有属性和其值。
3. Figure对象和Axes对象的关系，以前蒙出来过，但是这个说得更明白：“Figure对象有一个axes属性，其值为AxesSubplot对象的列表，每个AxesSubplot对象代表图表中的一个子图，前面所绘制的图表只包含一个子图，当前子图也可以通过plt.gca获得”。
4. Axes的背景颜色可通过axisbg这个关键字参数指定，同样是单字母的，不确定是否能有全称。奇怪的是用的是axis这个词而非axes，不过坐标轴是不能有什么“背景”颜色的，所以估计也无妨了。
5. 绘制面积不一样的subplot，例子：plt.subplot(221) plt.subplot(222) plt.subplot(212)，就是开始跟电脑说要按2*2的格子画这种格式下的第一张图，即左上角那张（先上后下先左右后），然后是右上角那张，然后耍赖说第二行咱按2*1的格子画这种格式下的第二张。可年的电脑！
6. 多张subplot轴情况下，调节之间的间距和轴与边框之间的距离用subplots_adjust函数，通过指定left, right, bottom, top, wspace, hspace等几个关键字参数进行调节，坐标均遵照坐标轴体系。
7. 查看用户定义的rc文件路径：matplotlib.get_configdir()。当前使用的rc文件的路径：matplotlib.matplotlib_fname()。
8. rc文件实际上是定义了一个字典，因此读取的配置文件返回的是一个配置字典，显示rc参数的的命令是：matplotlib.rc_params()。
9. 修改rc参数可以直接逐项修改字典中的键值，也可以使用rc函数，比如：matplotlib.rc("lines", marker="x", linewidth=2, color="red") 。
10. 恢复默认rc设置：matplotlib.rcdefaults()。重载rc文件（比如手动修改rc文件之后）：matplotlib.rcParams.update(matplotlib.rc_params())。
11. Text对象的_text属性即是指定或默认分配的字符字符串，比如：ax.xaxis.label._text，其中xlabel如果被我们指定为'time'，则返回'time'。也可以类方法来返回这个属性的值：ax.xaxis.label.get_text()。
12. Artist对象的共有属性。以前嫌逐个翻译和验证官方文档上的麻烦，不过不知道有什么关键字参数还真抓瞎，不如顺便抄录下来：alpha，不透明度，值在0到1之间；animated，是否开启动画效果；axes，当前这个对象所在的Axes对象，无或不适用则为None；clip_box，对象的裁剪框；clip_on，是否裁剪；clip_path，裁剪的路径；contains，判断指定点是否在对象上的函数；figure，所在的Figure对象，可能为None；label，文本标签；picker，控制Artist对象选取；transform，坐标体系；visible，是否可见；zorder，控制绘图顺序。
13. 一次设置多个Artist对象属性：fig.set(alpha=0.5, zorder=2)。逐个则类同1，set_*就好，比如：fig.set_alpha(0.5*fig.get_alpha())。
14. 为了支持pylab中的gca()等函数，Figure对象内部保存有当前Axes的信息，因此不建议直接对Figure.axes属性进行列表操作，而代之以add_subplot, add_axes, add_patch, delaxes等类方法的使用，来进行添加和删除操作。当然利用列表的特性有时也是有好处的，比如可以对Axes中的对象进行迭代操作。
15. plot返回的是一个Line2D对象的列表，因为我们可以传递多组X,Y轴的数据，一次绘制多条曲线，所以赋值的时候经常可见：line, = ax.plot(x, y, "-", color="blue", linewidth=2)，不然line就会是一个列表，而非Line2D对象。原来如此。
16. 这个……也太智能了吧？（Python shell下）>>> rects <a list of 50 Patch objects>。
17. [Axes].hist()返回三个对象，分别是样本空间，bin数目，和由此创建的Axes中的Rectangle对象组成的列表。
18. [Collections].get_size()：返回Collections对象的PolygonCollection的数目，用于散点图。
19. 现在开始说Axis对象。副刻度的对象缺省为matplotlib.ticker.NullLocator，不产生任何刻度线；而计算主刻度的对象为matplotlib.ticker.AutoLocator，根据当前的缩放等配置自动计算刻度的位置。
20. 还有很多预定义的Locator对象，比如MultipleLocator ，以指定值的整数倍为刻度放置刻度线。用法：MultipleLocator(np.pi/4)。
21. 另一个预定义的Locator，FuncFormatter，使用指定的函数计算刻度文本，他会传递给所指定的函数两个参数：刻度值和刻度序号，程序中通过比较笨的办法计算出刻度值所对应的刻度文本。用法：FuncFormatter(pi_formatter)，其中pi_formatter是一个自定义的函数。
至此看完，感觉对matplotlib的理解又加深了一点。下一步准备看几个官网上提供的recipe，然后开始整理实验数据，顺便练习用matplotlib作图。
**      这次学习matplotlib也是根据的matplotlib Release 0.99.1.1。除了Python略知一二其他编程语言我这个小白一概不晓，因此不妨采用巨细无遗的笔记方式，虽然可能因为罗嗦耽误功夫，不过为了学得比较扎实，也值了。笔记方法是大致按文中出现的顺序，提取并记录大意。**
凡例：a. [float]表示对象类型是float，用时不用加'['和']'。b. 代码中的跳格表示换行。
Chap 1 Introduction
1. matplotlib编写的缘起是想在Python环境下模仿MATLAB命令风格。matplotlib应该都是小写的，因为处在句首时都用的是小写的m。
2. matplotlib的设计理念：简单。跟Python的相符。
3. 对三维作图作者推荐VTK。
4. 作者认为任何有自尊的Python编程者对于困难的态度：挽起袖子大干一场。
5. matplotlib代码可分为三个部分：pylab界面(matplotlib.pylab)，给用户提供一套命令；前端/API，把命令转化为结果；后端，根据不同目的输出结果。
6. 作者推荐使用iPython作为matplotlib的Python交互解释器，据作者说是交互界面MATLAB风格的。目前我发现的好处是不用导入 matplotlib模块，可以直接使用各命令，不用加前缀。
Chap 3 Pyplot tutorial
7. 好习惯：import matplotlib.pylab as plt。认识到不便之处之前就如此沿用吧。
8. plt.ylabel([string])设置y轴文字。
9. plt.show()显示图。
10. plt([array1],[array2],[string],linewidth=[float])，依次代表x值，y值，数据点的样式。只有一个 array则默认为y值，x值为range(len(y))，点的样式为'colorshape'，默认为'b-'（蓝色实线），还比如'ro'（红色圆点），'k^'（黑色三角），'g--'（绿色虚线），默认都是填充的。可以串联参数一张图上作多条线。
11. plt.axis([array])，array内数字依次代表xmin, xmax, ymin, ymax。
12. import numpy as np——也不错，沿用之。np.arange()跟range()差不多，不过参数可以用小数，返回的也是一个array啦。
13. 设置线条属性。a. 参见9；b. plt.plot()返回matplotlib.lines.line2D实例，可使用相应的类方法；c. plot.setp(plt.plot()返回结果,color=[string],linewidth=[float])
14. line2D类的属性。可输入plt.setp(类实例)显示。
15. 作subplot。plt.subplot([integer][integer][integer])，依次代表行数，列数和数据点数（没搞明白具体是什么的行数）。
16. plt.axe([array])，array里数字依次代表左限，下限，宽度，长度。
17. plt.figure([integer])，生成figure。
18. plt.gca()返回当前轴（matplotlib.axes.Axes类实例），plt.gca()返回当前图（matplotlib.figure.Figure类实例）。
19. plt.title([string])设置标题。
20. plt.text(60, .025, r'$/mu=100,/ /sigma=15$')。前面两个是位置坐标，后面的是所显示的文字。r表示是raw string，转义符等等都需要特殊对待。用'$'包围的字串里相应字符显示为希腊字母。
21. matplotlib支持TeX语法，自带公式生成引擎。（没用，R和TeX分别排在要学的语言的单子的下两位……）
22. 对于文字（title，label，text）也可以赋值后用setp调整设置。（可能的问题是颜色是不是禁止缩写如'r'而只能用全称如'red'？）
23. hist()的facecolor是什么？
24. plt.grid([bool])，应该是显示网格。
25. 图上进一步显示注释：plt.annotate([string],[tuple1],[tuple2],arrowprops=[dict])，分别代表注释文字，注释位置目标点坐标，注释文字坐标，连接目标点和文字的箭头的属性。
26. plt.ylim([integer],[integer])：设置y轴上下限？
Chap 5 Customizing matplotlib
27. 配置matplotlib。利用matplotlibrc，里面的参数称为rc参数或者rc设置。
28. matplotlibrc文件位置：pwd, .matplotlib/matplotlibrc, （linux下）/usr/lib/python2.5/site-packages/matplotlib/mpl-data /matplotlibrc。
29. 查看当前起作用的rc文件：matplotlib.matplotlib_fname()。
30. 动态修改rc参数。import matplotlib as mpl（沿用！），mpl.rcParams包含rc参数，组织形式类似dict。
31. 一次修改一组参数：mpl.rc('lines', linewidth=2, color='r')。
32. mpl.rcdefaults()，恢复默认值。
33. rc文件包含的参数类型：backend，线条(line)，数据点样式(patch)，字体(font)，文字(test)，坐标轴(axes)，刻度(tick)，网格(grid)，图例(legend)，图(figure)，图像(image)等等。
34. 实在需要修改再深入吧，先用默认的设这把作图搞定再说。
Chap 7 Working with text
35. 字体属性：大小，粗细，文字位置和颜色等等，默认设置都在rc文件里。
36. 基本文字命令：text(), xlabel(), ylabel(), title(), figtext(), suptitle(), annotate()。均创建并返回一个matplotlib.text.Text类实例。
37. text()用法：text(x, y, [string], fontdict=None, **kwargs)，在坐标为x, y处添加string中的文本。fontdict是一个用于设置字体属性的dict。
38. xlable()用法：xlabel([string], *args, **kwargs)。设置x轴标签。默认覆盖设置（覆盖指函数里的参数覆盖rc参数——两层覆盖有必要么？）= {'fontsize' : 'small','verticalalignment' : 'top','horizontalalignment' : 'center'}
39. title()用法：title([string], *args, **kwargs)。设置当前轴的标题（嘛意思？），一定是在图所形成的框之上而非其中的。默认覆盖设置= {'fontsize': 'medium','verticalalignment': 'bottom','horizontalalignment': 'center'}
40. figtext()用法：figtext(x, y, [string], fontdict=None, **kwargs)。类似text()，x,y为相对值。
41. suptitle()用法：suptitle(*args, **kwargs)。在图上加标题。kwargs是matplotlib.text.Text类属性，默认值为x = 0.5, y = 0.98，均为相对值horizontalalignment = ‘center’, verticalalignment = ‘top’。
42. annotate()用法：annotate([string], xy, xytext=None, xycoords='data',
textcoords='data', arrowprops=None, **kwargs)。在坐标为xytext处添加坐标为x, y的点的注释。arrowprops为字典。
43. 样例代码中其他的命令：fig是plt.figure()返回的一个类实例。fig.add_subplot()，添加subplot；fig.subplots_adjust(top=0.85)，调整subplot在figure中的位置；ax.set_title('axes title')，ax是fig.add_subplot()返回的一个类实例（不知道是哪类），设置 title；ax.set_xlabel('xlabel')，设置xlabel；ax.text(3, 8, 'boxed italics text
 in data coords', style='italic',
bbox={'facecolor':'red', 'alpha':0.5, 'pad':10})，italic属于style而非weight，bbox应该是（矩形的）文本框属性，alpha是相对不透明度，facecolor应该是背景颜色，pad懒得查了……；ax.text(2, 6, r'an equation: $E=mc^2$', fontsize=15)，介绍TeX公式用法，参见entry 20；ax.text(3, 2, unicode('unicode: Institut f/374r Festk/366rperphysik',
 'latin-1'))，介绍unicode用法，前面的[string]是文本，后面是编码方式；
ax.text(0.95, 0.01, 'colored text in axes coords',verticalalignment='bottom',horizontalalignment='right',transform=ax.transAxes)， 介绍transform用法，把0.95和0.01当成相对坐标值；ax.annotate('annotate', xy=(2, 1), xytext=(3, 4),arrowprops=dict(facecolor='black', shrink=0.05))，介绍annotate()用法，arrowprops中的shrink似乎比较重要，指arrow起点不变，终点从坐标为
 x, y的点回缩xy和xytext的距离的shrink倍。
44. fig.add_axes([0,0,1,1])添加坐标轴。
45. matplotlib.text.Text类的属性，如rotation属性（'vertical'或者45）；multialignment控制多行的对齐方式（没太明白）。
46. 简单的TeX语法。字符串前加r，特殊转换前后加$（英文叫dollar sign），需要转化的前加转义符，$里的字符串必须是数学表达式，不然不予转义。
47. 表示上下标的语法。如r'$/alpha_i > /beta_i$'中，上标用'_'连接；如下标用'^'连接，如r'$/sum_{i=0}^ /infty x_i$'（/infty是无穷大）。分数：r'$/frac{3}{4}$'，3是分子。分数里可以套分数：r'$/frac{5 - /frac{1}{x}}{4}$'，表达式是(5-1/x)/4。根号：r'$/sqrt[3]{x}$'，3表示3次方根。一些华丽丽的字体，默认值由rc参数中的mathtext.default指定：r'$s(t) = /mathcal{A}/mathrm{sin}(2
 /omega t)$'，mathcal和mathrm分别指定了字体，cal是书法体，rm是roman体，it是斜体，tt是等宽体。还有一些注音符 (accent)和常用字符比如希腊字母的转义名。
48. plt.savefig([string])，保存图，名为[string]。
49. 更多annotate设置。ax.annotate('local max', xy=(3, 1),xycoords='data',xytext=(0.8, 0.95), textcoords='axes fraction',arrowprops=dict(facecolor='black',shrink=0.05))。xycoords为目标点的坐标系统，默认为按照data，也可以是'axes fraction'，指按照坐标轴相对位置。
50. arrowprops的属性：shrink, facecolor, width（箭头宽度），headwidth（箭头宽度），frac（箭头部分的相对长度）。
51. ax = fig.add_subplot(111, polar=True)。polar：是否使用极坐标。
Chap 8 Image tutorial
52. 图像处理模块：import matplotlib.image as mpimg（沿用~）。
53. 读取图像：img=mpimg.imread('stinkbug.png')（关于对stinkbug的认识还挺cross-cultural 的:p）。返回一个array，dtype为float32。matplotlib可以处理float32和uint8，非PNG格式的仅限于 uint8。对于RGB和RGBA图像支持float32和uint8，对于灰度格式的图像只支持 float32。
54. 用imshow命令render图像。img = plt.imshow(img)，img是53中赋值的那个对象。
55. 使用假颜色。假颜色主要用于增强对比，仅用于单channel图像如灰度图。可以用array slicing赋值，如lum_img = img[:,:,0]，之后imshow来render图像。
56. 更改假颜色设置。默认colormap为jet，可用set_cmap()更改：imgplot.set_cmap('hot')，还可以改成spectral。
57. 设置颜色所代表的数值，称为color bar，可用plt.colorbar()生成。
58. 利用分布信息调整图像对比度，暂时用不着，过。
暂时先到此为止吧，以上是两天的进度。目前打算还看章：9, 10, 12, 13和14，还有sciPy官网的cookbook。估计看完这个还是不太会写，所以最好借助cookbook熟悉作图的套路。
**Chap 9 Artist tutorial**59. matplotlib的API从低到高可以分成三个层次：a. matplotlib.backend_bases.FigureCanvas，即画布，各种图形所在之处；b. matplotlib.backend_bases.Renderer，即renderer，在画布上render图像对象的接口，c. matplotlib.artist.Artist，renderer用来在画布上画图的接口。
60. Artist API可以分成两类：primitive（暂翻作“基本类”）和container（暂翻作“容器类”）。基本类是图像上常用的组件，比如Line2D, Rectangle, Text, AxesImage之类的，容器类是这些对象所在之处，比如Axis, Axes and Figure。一般的做法是先创建一个Figure类实例，然后利用类的方法创建Axes或者Subplot类实例，然后利用Axes类方法创建基本类对象。
61. Axes类是最常用的matplotlib API，因为它是最常用的container，包含很多方法，比如plot(), text(), hist(), imshow()，可以创建大部分primitives，比如Line2D, Text, Rectangle, Image等等（分别对应前面的四种方法）。Axes指的是各种坐标轴围起来的区域，Axis指坐标轴本身。
62. Subplot是Axes的子类——真相大白。:)
63. 创建Axes：fig2.add_axes([0.15, 0.1, 0.7, 0.3])。四个数字分别为左、底、宽、高，坐标为相对值。
64. 设置坐标轴标签：xtext = ax.set_xlabel('my xdata')。
65. Axes类包括XAxis和YAxis类实例，可以搞定大部分刻度、刻度标签和坐标轴标签等坐标轴属性。
66. Figure和Axes都可以看成有一个外框，负责调整某些属性，分别是Figure.patch and Axes.patch类实例。
67. 属性的获取和设置：object.get_属性(),object.set_属性(属性值)。据说Pythonistas对这个做法不满意（Pythonista是什么人？），正在修改中。
68. 列举属性：matplotlib.artist.getp([object])（或者pylab中的getp([object])）。
69. 顶层的容器类：matplotlib.figure.Figure，背景是Figure.patch类实例的Rectangle对象。添加Subplot和Axes的时候都添加到Figure.Axes里。
70. 删除坐标轴：delaxes()。
71. Figure的默认坐标系统使用像素作为单位，可以用fig.transFigure改成相对坐标系统。
72. Figure.lines是一个列表，元素是图中的线条。
73. fig.lines.extend([l1, l2])，把创建好的两条line2D类实例l1和l2添加到图中。
74. fig.canvas.draw()。这个没明白。用plt.show()不就行了么？
75. plot()传输array之类的对象, 并创建matplotlib.lines.Line2D()类实例，类似地，bar()创建矩形并添加到Axes.patches这个列表里。也可以用add_line()和add_patch()方法创建。
76. 一些对科研作图比较有用的方法（原文叫helper method，不知有没有什么深意。）：ax.annotate，创建Annotate基本类，容器是ax.texts；ax.bar，创建Rectangle基本类，容器是ax.patches；ax.errorbar，创建Line2D和Rectangle基本类，容器是ax.lines和ax.patches；ax.fill，创建Polygon基本类，容器是ax.patches，这个应该是填充属性；ax.hist，创建Rectangle基本类，容器是ax.patches，不错，可以直接画直方图；ax.legend，创建Legend基本类，容器是ax.legends；ax.plot，创建Line2D基本类，容器是ax.lines；ax.scatter，创建PolygonCollection基本类，容器是ax.collections，应该指散点图，把所有点作为一个整体处理。
77. Axes可容纳的各种Artist：artists（一个所有primitive和container的列表），patch（注意是单数，应该单指Axes的那个框）, collections, images, legends, lines, patches, texts, xaxis, yaxis。
78. Axis类的一些方法：axis.get_ticklocs()，显示刻度坐标；axis.get_ticklabels()，返回刻度标签对象（Text类实例）组成的列表；axis.get_ticklines()，返回刻度线对象（line2D类实例）组成的列表，注意刻度线默认在上下方轴上都有；axis.get_ticklines(minor=True)，副刻度线。get_scale，显示刻度scale属性，比如linear或者log；get_major_locator，get_major_formattor，不知道是干嘛用的，都是类实例，可能用于设置属性；get_major_ticks，返回Tick类实例组成的列表；grid，网格线开关；get_gridlines，返回网格线组成的列表。
79. lightgoldenrodyellow和lightslategray：好纠结的颜色呀
80. label.set_rotation(45)，设置旋转度数。
81. line.set_markersize(25), line.set_markeredgewidth(3)里markersize大概指的刻度线的长度，markeredgewidth指刻度线宽度。
82. Tick类属性：tick1line（Line2D类实例，下/左轴刻度线），tick2line（Line2D类实例，上/右轴刻度线），gridline（Line2D类实例，网格线），label1（Text类实例，下/左轴刻度标签），label2，gridOn（boolean，网格线的开关，类似的还有tick1/2On，label1/2On）。
83. 示例代码：import matplotlib.ticker as tickertick（沿用之~），label1On = False, tick.label2.set_color('green')。
Chap 10 Legend guide
84. 阅读本章之前需要把legend()命令和matplotlib.legend.Legend类的说明看完，ft……
85. 好吧，先跳到legend()命令的说明。生成图例：legend((line1, line2, line3), ('label1', 'label2', 'label3'))，第一个tuple是Line2D，第二个里是各line的名字。要是省略第一个tuple应该就默认分配line的样式了吧。legend( ('label1', 'label2', 'label3'), loc='upper left')，loc指定锚定点在文本框中的位置。loc可用代码表示，从0到10共11种，用bbox_to_anchor调整所指定的loc的坐标，bbox_transform转换坐标系统。也可以用数字表示loc，标定图例框左下角的位置，此时无视bbox属性。
86. 其他的kwargs：title[string]，标题；markerscale，marker的相对大小；shadow，阴影；mode，延伸覆盖Axes区域的开关；还有numpoints和scatterpoints，没整明白。
87. Padding（pad到底指的是什么？）和spacing各属性的kwarg，估计对我这种control freak比较重要：borderpad，应该是文字到框的距离；labelspacing，标签之间的行距；handlelength，legend handles（图例图像本身？）的长度；handletextpad，legend handle和文字的间距；borderaxespad，注释框和坐标轴的间距；columnspacing，栏间距。Spacing用于描述同类对象的间距，padding描述不同类型对象的间距？
88. 然后俺再跳到Legend类的说明。类的全部属性和默认设置：class Legend(parent, handles, labels, loc=None, numpoints=None, markerscale=None, scatterpoints=3,scatteryosets=None,prop=None,pad=None,labelsep=None, handlelen=None,handletextsep=None,axespad=None,borderpad=None, labelspacing=None,handlelength=None,handletextpad=None,borderaxespad=None,
 columnspacing=None,ncol=1,mode=None,fancybox=None,shadow=None,title=None, bbox_to_anchor=None,bbox_transform=None)，其中parent是包含Legend的Artist，handle是图例中的图像，label是图例标签，其他的跟legend()命令说明里的相同。如果值为'None'则沿用rc参数。
89. 再跳回来开始看Chap 10。如果不指定图例样式和标签则legend()自动调用get_legend_handles_labels()，返回一个tuple，包括artist列表和标签列表，作为图例标签。artist列表里包括ax.lines, ax.patches和LineCollection和RegularPolyCollection类，由get_label()返回的这些对象的标签属性值作为图例中的标签。如果标签空缺或者以"_"开头，则该artist不进入返回结果。如果想要附加图例的对象不在这几类之内只能用笨办法自己写代码定制。
90. reverse列表：[list][::-1]。
91. zip([list1],[list2])太可爱了！怎么以前都不知道有这么好的东西……作用是生成新的list，元素是tuple，tuple的元素依次是list1, list2的第1，2，……项。支持n个列表。zip(*[list])将列表去zip化。
92. operator模块下itemgetter()。可以和zip配合，用于指定排序中的key。比如zip了两个列表，指定key=operator.itemgette(1)，则按tuple里第二个元素大小对tuple的列表排序。
91. 为不默认支持的类添加图例，看不懂，过。
92. 多栏图例。上不了网不能看例子，先存着吧。
93. 多块图例。问题在于使用legend()命令时创建了新的Legend类实例，原有的自动被删除。因此，需要手动添加被删掉的图例。gca().add_artist(l1)。gca()返回当前Axes/Subplot，add_artist添加已创建好的artist。
94. 图例的loc代码对应表。刚认识到似乎比较重要，所以特此补记。0代表best（不知道是哪里，可能每个准儿吧）；1，右上；2，左上；3，左下；4，右下；5，右边；6，center left，可能是垂直置中水平右对齐；7，center right，垂直置中水平右对齐；8，lower center，可能是水平置中垂直置底；9，upper center，水平置中垂直置顶；10，心儿。
Chap 12 Transformations tutorial
95. 四种坐标系统：data, axes, figure, display。一般（95%）情况下不用考虑（太好了）。
96. data坐标系统，即沿用数据中的坐标尺度体系。transformation对象是ax.transData（嘛意思？），可用方法：set_xlim(xmin,xmax)。可以用ax.transData类实例将data体系的坐标转化为display体系的（单位是像素），如ax.transData.transform((5, 0))，返回一个array。倒转化：inv = ax.transData.inverted()，inv.transform((335.175, 247.))，将display体系转化为data体系。
97. enumerate感觉像减弱版的zip，只接受一个参数，生成一个type为enumerate的对象，不可切片，可以迭代，返回一个个tuple，其元素为0和参数里的各项。
98. 作多个panel的图，终于明白subplot参数的含义了。第一个是多个subplot形成的阵列的行数，第二是列数，第三是当前sbuplot的位置，默认应该是按行排列的，即(223)指左下角那个subplot。
99. 混合坐标体系，多为data和axes的混用，通常用于highlight某个范围，缩放时想想让y轴随图缩放，但是保持x的范围。有专门的方法：axhline(), axvline(), axhspan(), axvspan()。不适用于极坐标。
100. 用法：建立Transformation类实例，trans=matplotlib.transforms.blended_transform_factory(ax.transData, ax.transAxes)，x轴用data体系，y用axes体系。然后在创建基本类的时候指定transform=trans。
101. 后面的就当前用途而言太高级，过。
Chap 14 Annotating axes
102. 又得跳，就当已经读过了吧。
103. 定义bbox：bbox_props = dict(boxstyle="rarrow,pad=0.3", fc="cyan", ec="b", lw=2)，或者直接用图中的：text.get_bbox_patch()，然后bb.set_boxstyle("rarrow", pad=0.6)。bbox的内置style：larrow（向左箭头），rarrow（向右箭头），round（圆角矩形），round4（中央较宽圆角矩形），roundtooth（平滑线锯齿外框），sawtooth（直线锯齿外框），square（矩形外框）。
104. 除了bbox还可以用实线箭头：arrowprops=dict(arrowstyle="->")。箭头作图包括四步：connect两个点（由属性connectstyle控制），clip以避免patch，shrink，箭杆部分mutate成多种多样（由属性arrowstyle控制）。
105. arrowstyle多种键值：angle（angleA=90,angleB=0,rad=0.0），angle3（angleA=90,angleB=0），arc（angleA=0,angleB=0,armA=None,armB=None,rad=0.0），arc3（rad=0.0），bar（armA=0.0,armB=0.0,fraction=0.3,angle=None）。各kwarg控制具体样式，angleA控制出发点的角度，逆时针方向旋转。angleB没猜出来。arc3和angle3都借用双曲线的一截，rad大概控制弧度。arc是三截折线，一个arm=0的话则为两截线，rad是控制弧度的。
106. 箭头样式太fancy，过。
107. matplotlib.patches.ConnectionPatch：连接两张图中的点。加插页的时候非常有用！用法：con = ConnectionPatch(xyA=xy, xyB=xy, coordsA="data", coordsB="data",axesA=ax1, axesB=ax2)，创建ConnectionPatch类实例，用于连接axesA在coordsA体系下的坐标点xyA和axesB在coordsB体系下的坐标点xyB；ax2.add_artist(con)，加到Axes上。
108. 在Axes的锚定点位置添加注释：导入mpl_toolkits.axes_grid.anchored_artists.AnchoredText；创建AnchoredText类实例；add_artist加入axes。
109. 在Axes的锚定点位置添加绘图区域，里面可以继续加artist：导入mpl_toolkits.axes_grid.anchored_artists.AnchoredDrawingArea；创建AnchoredText类实例；drawing_area.add_artist添加图形，add_artist添入axes。加入绘图区域的artist的transformation参数会被目标axes覆盖。
110. 在Axes的锚定点位置添加绘图区域，里面可以继续加artist，允许自带transformation：导入mpl_toolkits.axes_grid.anchored_artists.AnchoredAuxTransformBox；
111. 图中某些区域的缩放，同样，加插页的时候非常有用！方法：利用mpl_toolkits.axes_grid.inset_locator（从名字上看似乎这个才是用于作插页图的）。
到这里算看完了，感觉还是不会写代码，对于某个具体的对象不能马上想到需要定制的属性的键和值，
说明两点：看**cookbook是很重要的，还有i'm a bad programmer...**
