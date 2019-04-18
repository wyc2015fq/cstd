# 霍夫变换(Hough) - 心纯净，行致远 - CSDN博客





2018年01月30日 21:46:33[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：295








# 霍夫变换Hough

霍夫变换(Hough)是一个非常重要的检测间断点边界形状的方法。它通过将图像坐标空间变换到参数空间，来实现直线与曲线的拟合。

## 1.直线检测

### 1.1 直线坐标参数空间

在图像坐标空间中，经过点的直线表示为：







通过的直线有无数条，且对应于不同的值。

如果将和视为常数，而将原本的参数a和b看作变量，则式子(1)可以表示为：





这样就变换到了参数平面。这个变换就是直角坐标中对于点的Hough变换。

该直线是图像坐标空间中的点(在参数空间的唯一方程。考虑到图像坐标空间中的另一袋奶，它在参数空间中也有相应的一条直线，表示为：





这条直线与点在参数空间的直线相交于一点，如图所示：

![这里写图片描述](https://img-blog.csdn.net/20160506113821513)

图像坐标空间中过点和点的直线上的每一点在参数空间上各自对应一条直线，这些直线都相交于点,而就是图像坐标空间中点和点所确定的直线的参数。


反之，在参数空间相交于同一点的所有直线，在图像坐标空间都有共线的点与之对应。根据这个特性，给定图像坐标空间的一些边缘点，就可以通过Hough变换确定连接这些点的直线方程。

具体计算时，可以将参数空间视为离散的。建立一个二维累加数组,第一维的范围是图像坐标空间中直线斜率的可能范围，第二维的范围是图像坐标空间中直线截矩的可能范围。开始时初始化为0，然后对图像坐标空间的每一个前景点,将参数空间中每一个的离散值代入式子(2)中，从而计算出对应的值。每计算出一对,都将对应的数组元素加1，即。所有的计算结束之后，在参数计算表决结果中找到的最大峰值，所对应的就是源图像中共线点数目最多(共个共线点)的直线方程的参数；接下来可以继续寻找次峰值和第3峰值和第4峰值等等，它们对应于原图中共线点略少一些的直线。

```
注意：由于原图中的直线往往具有一定的宽度，实际上相当于多条参数极其接近的单像素宽直线，往往对应于参数空间中相邻的多个累加器。因此每找到一个当前最大的峰值点后，需要将该点及其附近点清零，以防算法检测出多条极其邻近的“假”直线。
```
- 1
- 2

对于上图的Hough变换空间情况如下图所示。 
![这里写图片描述](https://img-blog.csdn.net/20160506113845279)

这种利用二维累加器的离散方法大大简化了Hough变换的计算，参数空间上的细分程度决定了最终找到直线上点的共线精度。上述的二维累加数组A也被称为Hough矩阵。

```
注意：使用直角坐标表示直线，当直线为一条垂直直线或者接近垂直直线时，该直线的斜率为无限大或者接近无限大，从而无法在参数空间$a-b$上表示出来。为了解决这个问题，可以采用极坐标。
```
- 1
- 2

### 1.2 极坐标参数空间

极坐标中用如下参数方程表示一条直线。





其中，代表直线到原点的垂直距离，代表x轴到直线垂线的角度，取值范围为，如图所示。

![这里写图片描述](https://img-blog.csdn.net/20160506113908716)

与直角坐标类似，极坐标中的Hough变换也将图像坐标空间中的点变换到参数空间中。 

在极坐标表示下，图像坐标空间中共线的点变换到参数空间中后，在参数空间都相交于同一点，此时所得到的即为所求的直线的极坐标参数。与直角坐标不同的是，用极坐标表示时，图像坐标空间的共线的两点映射到参数空间是两条正弦曲线，相交于点,如上图所示。

具体计算时，与直角坐标类似，也要在参数空间中建立一个二维数组累加器A，只是取值范围不同。对于一副大小为的图像，通常的取值范围为,的取值范围为。计算方法与直角坐标系中累加器的计算方法相同，最后得到最大的A所对应的。

## 2.曲线检测

Hough变换同样适用于方程已知的曲线检测。

图像坐标空间中的一条已知的曲线方程也可以建立其相应的参数空间。由此，图像坐标空间中的一点，在参数空间中就可以映射为相应的轨迹曲线或者曲面。

若参数空间中对应各个间断点的曲线或者曲面能够相交，就能找到参数空间的极大值以及对应的参数；若参数空间中对应各个间断点的曲线或者曲面不能相交，则说明间断点不符合某已知曲线。

Hough变换做曲线检测时，最重要的是写出图像坐标空间到参数空间的变换公式。

例如，对于已知的圆方程，其直角坐标的一般方程为： 





其中，(a,b)为圆心坐标，r为圆的半径。 

那么，参数空间可以表示为(a,b,r)，图像坐标空间中的一个圆对应参数空间中的一个点。

具体计算时，与前面讨论的方法相同，只是数组累加器为三维A(a,b,r)。 

计算过程是让a,b在取值范围内增加，解出满足上式的r值，每计算出一个(a,b,r)值，就对相应的数组元素A(a,b,r)加1.计算结束后，找到的最大的A(a,b,r)所对应的a,b,r就是所求的圆的参数。

与直线检测一样，曲线检测也可以通过极坐标形式计算。 

注意：通过Hough变换做曲线检测，参数空间的大小将随着参数个数的增加呈指数增长的趋势。所以在实际使用时，要尽量减少描述曲线的参数数目。因此，这种曲线检测的方法只对检测参数较少的曲线有意义。

## 3.任意形状的检测

这里所说的任意形状的检测，是指应用广义Hough变换去检测某一任意形状边界的图形。它首先选取该形状中的任意点(a,b)为参考点，然后从该任意形状图形的边缘每一点上，计算其切线方向和到参考点(a,b)位置的偏移矢量r，以及r与x轴的夹角。

参考点(a,b)的位置可由下式算出： 







# 2.Hough变换直线检测的Matlab实现

通过Hough在二值图像中检测直线需要以下3个步骤。 

·>(1)利用hough()函数执行霍夫变换，得到霍夫矩阵。 

·>(2)利用houghpeaks()函数在霍夫矩阵中寻找峰值点。 

·>(3)利用houghlines()函数在之前2步结果的基础上得到原二值图像中的直线信息。

## 2.1 霍夫变换–Hough

调用形式： 

[H,theta,rho]=hough(BW,param1,value1,param2,value2)

参数说明： 

·BW是边缘检测后的二值图像； 

·可选参数对param1,value1和param2,value2的合法取值如下：
|param取值|含义|
|----|----|
|ThetaResolution|Hough矩阵中轴方向上单位区间的长度(以“度”为单位)，可取(0,90)区间上的实数，默认为1|
|RhoResolution|Hough矩阵中\rho轴方向上单位区间的长度，可取(0,norm(size(BW)))区间上的实数，默认为1|

返回值： 

·H是变换得到的霍夫矩阵 

·theta,rho分别对应于Hough矩阵每一列和每一行的\theta和\rho值组成的向量。

```
补充：[m,n] = size(X)

补充：norm的用法，matlab  help norm 
NORM   Matrix or vector norm.
For matrices...
对于矩阵...
NORM(X) is the largest singular value of X, max(svd(X)).
NORM(X)是X的最大奇异值
NORM(X,2) is the same as NORM(X).
NORM(X,1) is the 1-norm of X, the largest column sum,
          = max(sum(abs(X))).
NORM(X,inf) is the infinity norm of X, the largest row sum,
          = max(sum(abs(X'))).
NORM(X,'fro') is the Frobenius norm, sqrt(sum(diag(X'*X))).
NORM(X,P) is available for matrix X only if P is 1, 2, inf or 'fro'.

For vectors...
对于向量...
NORM(V,P) = sum(abs(V).^P)^(1/P).
返回向量A的p范数
NORM(V) = norm(V,2).
返回向量A的2范数，即欧几里德范数。二范数等价于平方和开平方，Sqrt(X1^2+X2^2+...+Xn^2)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23

## 2.2 寻找峰值–houghpeaks

调用形式： 

peaks=houghpeaks(H,numpeaks,param1,value1,param2,value2)

参数说明： 

·H是hough()函数得到的霍夫矩阵 

·numpeaks是要寻找的峰值数目，默认为1 

·可选参数对param1,value1和param2,value2的合法取值如下：
|param取值|含义|
|----|----|
|Threshold|峰值的阈值，只有大于该阈值的店才被认为是可能的阈值，取值>0,默认为|
|NHoodSize|在每次检测出一个峰值后，NhoodSize指出了在该峰值周围需要清零的领域信息。以[m,n]的形式给出，其中m、n均为正的奇数。默认为大于等于size(H)/50的最小奇数。|

返回值： 

·peaks是一个的矩阵，每行的两个元素分别是某一峰值点再hough矩阵中的行、列索引，Q为找到的峰值点的数目。

## 2.3 提取直线段–houghlines

调用形式： 

lines=houghlines(BW,theta,rho,peaks,param1,value1,param2,value2)

参数说明： 

·BW是边缘检测后的二值图像 

·theta,rho分别对应于Hough矩阵每一列和每一行的值组成的向量。有hough()函数返回。


·peaks是一个包含峰值点信息的的矩阵，由houghpeaks()函数返回。


·可选参数对param1,value1和param2,value2的合法取值如下：
|param取值|含义|
|----|----|
|FillGap|线段合并的阈值：如果对应于Hough矩阵某一个单元格(相同的)的两条线段之间的距离小于 FillGap，则合并为一个直线段。默认值为20.|
|MinLength|检测的直线段的最小长度阈值：如果检测出的直线段长度大于MinLength，则保留；丢弃小于MinLength的直线段。默认值为40.|

返回值： 

·lines是一个结构体数组，数组长度是找到的直线条数，而每一个数组元素(直线段结构体)的内部结构如下：
|域|含义|
|----|----|
|point1|直线段的端点1|
|point2|直线段的端点2|
|thea|对应在霍夫矩阵中的|
|rho|对应在霍夫矩阵中的|

## 2.4 Hough变换的matlab实现

matlab中输入帮助命令： 

help hough; help houghpeaks; help houghlines 

就可以看到代码示例，很详细，其实用好帮助文档一切问题都可以解决。

Example

Search for line segments corresponding to five peaks in the Hough 

transform of the rotated circuit.tif image. Additionally, highlight 

the longest segment.

```
I  = imread('circuit.tif');
    rotI = imrotate(I,33,'crop');
    BW = edge(rotI,'canny');
    [H,T,R] = hough(BW);
    imshow(H,[],'XData',T,'YData',R,'InitialMagnification','fit');
    xlabel('\theta'), ylabel('\rho');
    axis on, axis normal, hold on;
    P  = houghpeaks(H,5,'threshold',ceil(0.3*max(H(:))));
    x = T(P(:,2)); 
    y = R(P(:,1));
    plot(x,y,'s','color','white');

    % Find lines and plot them
    lines = houghlines(BW,T,R,P,'FillGap',5,'MinLength',7);
    figure, imshow(rotI), hold on
    max_len = 0;
    for k = 1:length(lines)
        xy = [lines(k).point1; lines(k).point2];
        plot(xy(:,1),xy(:,2),'LineWidth',2,'Color','green');

        % plot beginnings and ends of lines
        plot(xy(1,1),xy(1,2),'x','LineWidth',2,'Color','yellow');
        plot(xy(2,1),xy(2,2),'x','LineWidth',2,'Color','red');

        % determine the endpoints of the longest line segment 
        len = norm(lines(k).point1 - lines(k).point2);
        if ( len > max_len)
          max_len = len;
          xy_long = xy;
        end
    end

    % highlight the longest line segment
    plot(xy_long(:,1),xy_long(:,2),'LineWidth',2,'Color','cyan');
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34

# 3.参考书籍

```
1.《数字图像处理与机器视觉-Visual C++与Mat lab实现 （第2版）》  
    第12章  图像分割  Page409
2.《数字图像处理（第三版）》
```

bo博客出处——[霍夫变换(Hough)](http://blog.csdn.net/sudohello/article/details/51335237)



