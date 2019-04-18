# 思考：矩阵及变换，以及矩阵在DirectX和OpenGL中的运用问题：左乘/右乘,行优先/列优先,... - 长歌行 - CSDN博客





2012年03月08日 09:45:06[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1037








[引用来源](http://www.cnblogs.com/soroman/archive/2008/03/21/1115571.html)





思考：矩阵及变换，以及矩阵在DirectX和OpenGL中的运用
**1。矩阵和线性变换：一一对应**矩阵是用来表示线性变换的一种工具，它和线性变换之间是一一对应的。

考虑线性变换：

a11*x1 + a12*x2 + ...+a1n*xn = x1'

a21*x1 + a22*x2 + ...+a2n*xn = x2'

...

am1*x1 + am2*x2 + ...+amn*xn = xm'


对应地，用矩阵来表示就是：

|a11 a12 ... a1n   |   |x1|     |x1'| 

|a21 a22 ... a2n   |   |x2|     |x2'|

|...                      |* |...|=    |... |

|am1 am2 ... amn |   |xn|     |xm'|


也可以如下来表示：

                   |a11 a21 ... am1| 

                   |a12 a22 ... am2|  

|x1 x2...xn|*|...                   |= |x1' x2'... xm'|      

                   |a1n a2n ... amn| 


其中涉及到6个矩阵。分别为A[m*n],X[n*1],X'[m*1]以及X[1*n],A[n*m],X'[1*m]。

可以理解成向量x(x1,x2,...,xn)经过一个变换矩阵A[m*n]或A[n*m]后变成另外一个向量x'(x1',x2',...,xm'))。
**2。矩阵的表示法：行矩阵 vs. 列矩阵**


行矩阵和列矩阵的叫法是衍生自行向量和列向量。

其实，矩阵A[m*n]可以看成是m个n维的row vector构成的row matrix，也可看成是n个m维的column vector构成的column matrix。

其中，X[n*1]/X'[m*1]就等价于1个n/m维的column vector。X[1*n]/X'[1*m]就等价于1个n/m维的row vector。

Row matrix和Column matrix只是两种不同的表示法，前者表示把一个向量映射到矩阵的一行，后者表示把一个向量映射到矩阵的一列。

本质上体现的是同一线性变换。矩阵运算规定了它们可以通过转置运算来改变这个映射关系。
**3。矩阵的相乘顺序：前乘或左乘 vs. 后乘或右乘**


需要注意的是两种不同的表示法对应不同的运算顺序：

如果对一个column vector做变换，则变换矩阵(row matrix/vectors)必须出现在乘号的左边，即pre-multiply，又叫前乘或左乘。

如果对一个row vector做变换，则变换矩阵(column matrix/vectors)必须出现在乘号的右边，即post-multiply，又叫后乘或右乘。

一般不会弄错，因为矩阵乘法性质决定了相同的内维数的矩阵才能相乘。至于为什么是这个规律，为什么要row vector乘以column vector或column vector乘以row vector???想想吧。。。


所以左乘还是右乘，跟被变换的vector的表示形式相关，而非存储顺序决定。

**4。矩阵的存储顺序：按行优先存储 vs. 按列优先存储**


涉及到在计算机中使用矩阵时，首先会碰到存储矩阵的问题。

因为计算机存储空间是先后有序的，如何存储A[m*n]的m*n个元素是个问题，一般有两种：按行优先存储和按列优先存储。


row-major：存成a11,a12,...,amn的顺序。

column-major：存成a11,a21,...,amn的顺序。


这样问题就来了，给你一个存储好的矩阵元素集合，你不知道如何读取元素组成一个矩阵，比如你不知道a12该放在几行几列上。

所以，每个系统都有自己的规定，比如以什么规则存储的就以什么规则读取。DX使用Row-major,OGL使用Column-major.即一个相同的矩阵A[m*n]在DX和OGL中的存储序列是不一样的,这带来了系统间转换的麻烦。


不过,一个巧合的事情是:DX中，点/向量是用Row Vector来表示的，所以对应的变换矩阵是Column Matrix/Vectors,而OGL中，点/向量是用Column Vector来表示的，所以对应的变换矩阵是Row Matrix/Vectors.所以,如果在DX中对一个向量x(x1,x2,x3,1)或点(x(x1,x2,x3,1))应用A[4*4]的矩阵变换，就是x' = x(x1,x2,x3,1) * A[4*4],由于采用Row-major，所以它的存储序列是a11,a12,...,a43,a44。在OGL中，做同样的向量或点的变换，因为其使用Row
 Matrix/Vectors,其应用的变换矩阵应该是A'[4*4] ＝ A[4*4]( ' 表示Transpose/转置)，就是x' = A'[4*4] * x'(x1,x2,x3,1),但是由于采用Column-major,它的存储序列正好也是a11,a12,...,a43,a44!!!

所以实际上,对DX和OGL来讲,同一个变换,存储的矩阵元素序列是一样的.比如:都是第13,14,15个元素存储了平移变化量deltaZ,deltaY,deltaZ.


Refs:

http://mathworld.wolfram.com/Matrix.html

http://www.gamedev.net/community/forums/topic.asp?topic_id=321862






