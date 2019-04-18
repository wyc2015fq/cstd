# IEEE-754标准 - z69183787的专栏 - CSDN博客
2018年08月13日 10:30:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：385
[https://blog.csdn.net/wallc/article/details/72674712](https://blog.csdn.net/wallc/article/details/72674712)
### 历史
> 
IEEE-754标准的主要起草者是来自UCB的数学教授William Kahan.。他帮助Intel设计完成了8087浮点运算单元(FPU)。由于设计的浮点标准是如此的出色，使得IEEE将此标准作为IEEE浮点格式的基础。 
为了浮点数据处理对于硬件、软件或者两者的结合都能产生独立的结果，不受平台的影响，IEEE为执行浮点运算提供了一个统一标准，其规定了浮点数的表示格式、操作方式、舍入模式及异常处理。
### 浮点数表示格式
> 
在IEEE-754标准下，浮点格式主要分为四种类行，即单精度格式、双精度格式、扩展单精度格式和扩展双精度格式。其中32位单精度格式与64位双精度格式作为基础格式更为常用，扩展格式则有特殊目的，一般对用户透明。
![浮点格式参数](https://img-blog.csdn.net/20170524111255109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsbGM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
浮点格式可分为符号位s，指数位e以及尾数位f三部分。 
其中真实的指数E相对于实际的指数有一个偏移量，所以E的值应该为e-Bias，Bias即为指数偏移量。**这样做的好处是便于使用无符号数来代替有符号的真实指数。**尾数f字段代表纯粹的小数，它的左侧即为小数点的位置。规格化数的隐藏位默认值为1，不在格式中表达。
![IEEE-754 标准浮点格式](https://img-blog.csdn.net/20170524112208777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsbGM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
在IEEE-754 标准下，浮点数一共分为：
- **NaN**：即Not a Number。非数的指数位全部为1 同时尾数位不全为0。在此前提下，根据尾数位首位是否为1，NaN 还可以分为SNaN 和QNaN 两类。前者参与运算时将会发生异常。
- **无穷数**：指数位全部为1 同时尾数位全为0。大。
- **规格化数**：指数位不全为1 同时尾不全为0。此时浮点数的隐含位有效，其值为1。
- **非规格化数**：指数位全为0 且尾数位不全为0。此时隐含位有效，值为0。*另外需要**注意**，以单精度时为例，真实指数E 并非0-127=-127，而是-126，这样一来就与规格化下最小真实指数E=1-127=-126 达成统一，形成过度。*
- **0** ：指数位与尾数位都全为0，根据符号位决定正负。
![IEEE-754 标准的浮点数表示](https://img-blog.csdn.net/20170524112528329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsbGM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 浮点的舍入模式
> 
在存储单元的物理限制下，无限精度的浮点数需要根据需求进行舍入操作，一般 
可分为四类：
- 1．最近舍入，即向距离最近的浮点数舍入，若存在两个同样接近的数，则选择偶数作为舍入值。
- 2．向零舍入，又称截断舍入，将多余的精度位截掉，即取舍入后绝对值较小的值。
- 3．正向舍入，也称正无穷舍入，即舍入后结果大于原值。
- 4．负向舍入：也称负无穷舍入，即舍入后结果小于原值。
