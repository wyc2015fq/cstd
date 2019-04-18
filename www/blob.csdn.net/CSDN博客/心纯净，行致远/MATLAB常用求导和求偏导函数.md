# MATLAB常用求导和求偏导函数 - 心纯净，行致远 - CSDN博客





2018年03月28日 15:51:40[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：27152标签：[MATLAB																[函数求导](https://so.csdn.net/so/search/s.do?q=函数求导&t=blog)](https://so.csdn.net/so/search/s.do?q=MATLAB&t=blog)
个人分类：[【MatLab/Simulink】](https://blog.csdn.net/zhanshen112/article/category/7184373)








matlab求导命令diff调用格式:
- diff(函数) ， 求的[一阶导数](http://zhidao.baidu.com/search?word=%E4%B8%80%E9%98%B6%E5%AF%BC%E6%95%B0&fr=qb_search_exp&ie=utf8)；
- diff(函数， n) ，求的n阶[导数](http://zhidao.baidu.com/search?word=%E5%AF%BC%E6%95%B0&fr=qb_search_exp&ie=utf8)(n是具体整数)；
- diff(函数，变量名)，求对的[偏导数](http://zhidao.baidu.com/search?word=%E5%81%8F%E5%AF%BC%E6%95%B0&fr=qb_search_exp&ie=utf8)；
- diff(函数， 变量名，n)，求对的n阶[偏导数](http://zhidao.baidu.com/search?word=%E5%81%8F%E5%AF%BC%E6%95%B0&fr=qb_search_exp&ie=utf8)；
- matlab求[雅可比矩阵](http://zhidao.baidu.com/search?word=%E9%9B%85%E5%8F%AF%E6%AF%94%E7%9F%A9%E9%98%B5&fr=qb_search_exp&ie=utf8)命令jacobian，调用格式:
- jacobian([函数；函数； 函数]，[])给出矩阵:



![Matlab函数求导教程](http://www.xitongzhijia.net/uploads/allimg/161020/70-161020153004-water.jpg)

**　　方法/步骤：**

　　1、diff（f（x）），一阶导数。

　　首先我们从最简单的一阶单变量函数进行求导，我们以f（x）=sin（x）+x^2为例，进行示例介绍，在Matlab中，按照以下次序输入命令。



![Matlab函数求导教程](http://www.xitongzhijia.net/uploads/allimg/161020/70-161020153004-50-water.jpg)

　　说明：

　　（1）syms x代表着声明符号变量x，只有声明了符号变量才可以进行符号运算，包括求导。

　　（2）f（x）=sin（x）+x^2 ，就是我们输入的要求导的函数。

　　（3）diff（f（x）），代表着对单变量函数求一阶导数。

　　（4）pretty（ans），将当前变量显示为我们常用的书面形式。

　　从以上结果我们可以看出，f（x）=2x+cos（x）。

　　2、diff（f（x），n），n阶导数。

　　我们还是以f（x）=sin（x）+x^2 为例，进行示例介绍，在Matlab中，按照以下次序输入命令。



![Matlab函数求导教程](http://www.xitongzhijia.net/uploads/allimg/161020/70-161020153004-51-water.jpg)

　　可以看到，f（x）=2-sin（x），N我们可以根据实际需要，取任意正整数。

　　3、diff（f（x1，x2，。。。。。），xi），对含有M个变量的函数求偏导数。

　　我们以函数f（x1，x2，x3）=sin（x1）+x2^2+exp（x3）为例进行介绍。

　　在Matlab主窗口中依次输入以下命令。



![Matlab函数求导教程](http://www.xitongzhijia.net/uploads/allimg/161020/70-161020153004-52-water.jpg)

　　我们可以看到，对于多元函数，我们需要将每一个变量声明为符号变量，如上，我们需要将x1，x2，x3声明为符号变量。然后diff（f（x1，x2，x3），x2）的意思是对x2求一阶偏导，结果为2*x2。

　　4、diff（f（x1，x2，。。。。。），xi，N），对多元函数的某一个变量求N阶偏导数。

　　我们还是以函数f（x1，x2，x3）=sin（x1）+x2^2+exp（x3）为例进行介绍。在MATLAB主窗口中依次输入以下命令。



![Matlab函数求导教程](http://www.xitongzhijia.net/uploads/allimg/161020/70-161020153005-water.jpg)




