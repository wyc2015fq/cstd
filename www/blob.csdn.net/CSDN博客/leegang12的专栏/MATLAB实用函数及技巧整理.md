# MATLAB实用函数及技巧整理  - leegang12的专栏 - CSDN博客
2011年04月30日 14:18:00[leegang12](https://me.csdn.net/leegang12)阅读数：911
MATLAB实用函数及技巧整理 
===============================================
小整理：**MATLAB**常用的基本数学函数
abs(x)：纯量的绝对值或向量的长度
angle(z)：复数z的相角(Phase angle)
sqrt(x)：开平方
real(z)：复数z的实部
imag(z)：复数z的虚部
conj(z)：复数z的共轭复数
round(x)：四舍五入至最近整数
fix(x)：无论正负，舍去小数至最近整数
floor(x)：地板函数，即舍去正小数至最近整数
ceil(x)：天花板函数，即加入正小数至最近整数
rat(x)：将实数x化为分数表示
rats(x)：将实数x化为多项分数展开
sign(x)：符号函数 (Signum function)。
当x<0时，sign(x)=-1；
当x=0时，sign(x)=0;
当x>0时，sign(x)=1。
rem(x,y)：求x除以y的馀数
gcd(x,y)：整数x和y的最大公因数
lcm(x,y)：整数x和y的最小公倍数
exp(x)：自然指数
pow2(x)：2的指数
log(x)：以e为底的对数，即自然对数或
log2(x)：以2为底的对数
log10(x)：以10为底的对数
===============================================
小整理：**MATLAB**常用的三角函数
sin(x)：正弦函数
cos(x)：馀弦函数
tan(x)：正切函数
asin(x)：反正弦函数
acos(x)：反馀弦函数
atan(x)：反正切函数
atan2(x,y)：四象限的反正切函数
sinh(x)：超越正弦函数
cosh(x)：超越馀弦函数
tanh(x)：超越正切函数
asinh(x)：反超越正弦函数
acosh(x)：反超越馀弦函数
atanh(x)：反超越正切函数
====================================================
小整理：适用於向量的常用函数有：
min(x): 向量x的元素的最小值
max(x): 向量x的元素的最大值
mean(x): 向量x的元素的平均值
median(x): 向量x的元素的中位数
std(x): 向量x的元素的标准差
diff(x): 向量x的相邻元素的差
sort(x): 对向量x的元素进行排序（Sorting）
length(x): 向量x的元素个数
norm(x): 向量x的欧氏（Euclidean）长度
sum(x): 向量x的元素总和
prod(x): 向量x的元素总乘积
cumsum(x): 向量x的累计元素总和
cumprod(x): 向量x的累计元素总乘积
dot(x, y): 向量x和y的内积
cross(x, y): 向量x和y的外积
（大部份的向量函数也可适用於矩阵，详见下述。）
====================================================
小整理：**MATLAB**的查询命令
help：用来查询已知命令的**用法**。例如已知inv是用来计算反矩阵，键入
help inv即可得知有关inv命令的**用法**。（键入help help则显示help的用
法，请试看看！）
lookfor：用来寻找未知的命令。例如要寻找计算反矩阵的命令，可键入
lookfor
inverse，**MATLAB**即会列出所有和关键字inverse相关的指令。找到所需的
命令後，即可用help进一步找出其**用法**。（lookfor事实上是对所有在搜寻
路径下的M档案进行关键字对第一注解行的比对，详见後叙
===============================================
下表即为**MATLAB**常用到的永久常数。
小整理：**MATLAB**的永久常数
i或j：基本虚数单位（即）
eps：系统的浮点（Floating-point）精确度
inf：无限大， 例如1/0
nan或NaN：非数值（Not a number），例如0/0
pi：圆周率 p（= 3.1415926...）
realmax：系统所能表示的最大数值
realmin：系统所能表示的最小数值
nargin: 函数的输入引数个数
nargin: 函数的输出引数个数
====================================================
1-7、结束**MATLAB**
有三种方法可以结束**MATLAB**：
    1.键入exit
    2.键入quit
    3.直接关闭**MATLAB**的命令视窗（Command window）
====================================================
小整理：**MATLAB**基本绘图函数
plot: x轴和y轴均为线性刻度（Linear scale）
loglog: x轴和y轴均为对数刻度（Logarithmic scale）
semilogx: x轴为对数刻度，y轴为线性刻度
semilogy: x轴为线性刻度，y轴为对数刻度
====================================================
若要画出多条曲线，只需将座标对依次放入plot函数即可：
plot(x, sin(x), x, cos(x));
若要改变颜色，在座标对後面加上相关字串即可：
plot(x, sin(x), 'c', x, cos(x), 'g');
若要同时改变颜色及图线型态（Line style），也是在座标对後面加上相
关字串即可：
plot(x, sin(x), 'co', x, cos(x), 'g*');
====================================================
小整理：plot绘图函数的叁数
字元   颜色 字元   图线型态
y   黄色 .   点
k   黑色 o   圆
w   白色 x   x
b   蓝色 +   +
g   绿色 *   *
r   红色 -   实线
c   亮青色 :   点线
m   锰紫色 -.   点虚线
    --   虚线
====================================================
图形完成後，我们可用axis([xmin,xmax,ymin,ymax])函数来调整图轴的范
围：
axis([0, 6, -1.2, 1.2]);
此外，**MATLAB**也可对图形加上各种注解与处理：
xlabel('Input Value'); % x轴注解
ylabel('Function Value'); % y轴注解
title('Two Trigonometric Functions'); % 图形标题
legend('y = sin(x)','y = cos(x)'); % 图形注解
grid on; % 显示格线
我们可用subplot来同时画出数个小图形於同一个视窗之中：
subplot(2,2,1); plot(x, sin(x));
subplot(2,2,2); plot(x, cos(x));
subplot(2,2,3); plot(x, sinh(x));
subplot(2,2,4); plot(x, cosh(x));
**MATLAB**还有其他各种二维绘图函数，以适合不同的应用，详见下表。
====================================================
小整理：其他各种二维绘图函数
bar   长条图
**errorbar**   图形加上误差范围
fplot   较精确的函数图形
polar   极座标图
hist   累计图
rose   极座标累计图
stairs   阶梯图
stem   针状图
fill   实心图
feather   羽毛图
compass   罗盘图
quiver   向量场图
====================================================
以下我们针对每个函数举例。
当资料点数量不多时，长条图是很适合的表示方式：
close all; % 关闭所有的图形视窗
x=1:10;
y=rand(size(x));
bar(x,y);
如果已知资料的误差量，就可用**errorbar**来表示。下例以单位标准差来做
资料的误差量：
x = linspace(0,2*pi,30);
y = sin(x);
e = std(y)*ones(size(x));
**errorbar**(x,y,e)
对於变化剧烈的函数，可用fplot来进行较精确的绘图，会对剧烈变化处进
行较密集的取样，如下例：
fplot('sin(1/x)', [0.02 0.2]); % [0.02 0.2]是绘图范围
若要产生极座标图形，可用polar：
theta=linspace(0, 2*pi);
r=cos(4*theta);
polar(theta, r);
对於大量的资料，我们可用hist来显示资料的分　情况和统计特性。下面
几个命令可用来验证randn产生的高斯乱数分　：
x=randn(5000, 1); % 产生5000个 ?=0，?=1 的高斯乱数
hist(x,20); % 20代表长条的个数
rose和hist很接近，只不过是将资料大小视为角度，资料个数视为距离，?⒂眉 昊嬷票硎荆?
x=randn(1000, 1);
rose(x);
stairs可画出阶梯图：
x=linspace(0,10,50);
y=sin(x).*exp(-x/3);
stairs(x,y);
stems可产生针状图，常被用来绘制数位讯号：
x=linspace(0,10,50);
y=sin(x).*exp(-x/3);
stem(x,y);
stairs将资料点视为多边行顶点，并将此多边行涂上颜色：
x=linspace(0,10,50);
y=sin(x).*exp(-x/3);
fill(x,y,'b'); % 'b'为蓝色
feather将每一个资料点视复数，并以箭号画出：
theta=linspace(0, 2*pi, 20);
z = cos(theta)+i*sin(theta);
feather(z);
compass和feather很接近，只是每个箭号的起点都在圆点：
theta=linspace(0, 2*pi, 20);
z = cos(theta)+i*sin(theta);
compass(z);
--
3.基本XYZ立体绘图命令
在科学目视表示（Scientific visualization）中，三度空间的立体图是
一个非常重要的技巧。本章将介绍**MATLAB**基本XYZ三度空间的各项绘图命
令。
mesh和plot是三度空间立体绘图的基本命令，mesh可画出立体网状图，
plot则可画出立体曲面图，两者产生的图形都会依高度而有不同颜色。下
列命令可画出由函数 形成的立体网状图: 
x=linspace(-2, 2, 25); % 在x轴上取25点 
y=linspace(-2, 2, 25); % 在y轴上取25点 
[xx,yy]=meshgrid(x, y); % xx和yy都是21x21的矩阵 
zz=xx.*exp(-xx.^2-yy.^2); % 计算函数值，zz也是21x21的矩阵 
mesh(xx, yy, zz); % 画出立体网状图
surf和mesh的**用法**类似：
x=linspace(-2, 2, 25); % 在x轴上取25点 
y=linspace(-2, 2, 25); % 在y轴上取25点 
[xx,yy]=meshgrid(x, y); % xx和yy都是21x21的矩阵 
zz=xx.*exp(-xx.^2-yy.^2); % 计算函数值，zz也是21x21的矩阵 
surf(xx, yy, zz); % 画出立体曲面图
为了方便测试立体绘图，**MATLAB**提供了一个peaks函数，可产生一个凹凸有
致的曲面，包含了三个局部极大点及三个局部极小点，其方程式为：
要画出此函数的最快方法即是直接键入peaks：
peaks
z = 3*(1-x).^2.*exp(-(x.^2) - (y+1).^2) ... 
- 10*(x/5 - x.^3 - y.^5).*exp(-x.^2-y.^2) ... 
- 1/3*exp(-(x+1).^2 - y.^2)
我们亦可对peaks函数取点，再以各种不同方法进行绘图。meshz可将曲面
加上围裙：
[x,y,z]=peaks; 
meshz(x,y,z); 
axis([-inf inf -inf inf -inf inf]);
waterfall可在x方向或y方向产生水流效果：
[x,y,z]=peaks; 
waterfall(x,y,z); 
axis([-inf inf -inf inf -inf inf]);
下列命令产生在y方向的水流效果：
[x,y,z]=peaks; 
waterfall(x',y',z'); 
axis([-inf inf -inf inf -inf inf]);
meshc同时画出网状图与等高线：
[x,y,z]=peaks; 
meshc(x,y,z); 
axis([-inf inf -inf inf -inf inf]);
surfc同时画出曲面图与等高线：
[x,y,z]=peaks; 
surfc(x,y,z); 
axis([-inf inf -inf inf -inf inf]);
contour3画出曲面在三度空间中的等高线：
contour3(peaks, 20); 
axis([-inf inf -inf inf -inf inf]);
contour画出曲面等高线在XY平面的投影：
contour(peaks, 20);
plot3可画出三度空间中的曲线：
t=linspace(0,20*pi, 501); 
plot3(t.*sin(t), t.*cos(t), t);
亦可同时画出两条三度空间中的曲线：
t=linspace(0, 10*pi, 501); 
plot3(t.*sin(t), t.*cos(t), t, t.*sin(t), t.*cos(t), -t);
y(2:4)-1 % 取出y的第二至第四个元素来做运算
同样的方法可用於产生公差为1的等差数列：x = 7:16
x = 7:3:16 % 公差为3的等差数列
x = linspace(4, 10, 6) % 等差数列：首项为4,末项为10,项数为6
若要重新安排矩阵的形状，可用reshape命令：B = reshape(A, 4, 2) % 4是新矩阵的列数，2是新矩阵的行数
举例来说，下列命令会产生一个长度为6的调和数列（Harmonic 
sequence）：
x = zeros(1,6); % x是一个16的零矩阵
for i = 1:6,
x(i) = 1/i;
end
for 圈可以是多层的，下例产生一个16的Hilbert矩阵h，其中为於第i
列、第j行的元素为：
h = zeros(6);
for i = 1:6,
for j = 1:6,
h(i,j) = 1/(i+j-1);
end
end
format rat % 使用分数来表示数值
>>disp(x)
1 1/2 1/3 1/4 1/5 1/6
function output = fact(n)
% FACT Calculate factorial of a given positive integer.
output = 1;
for i = 1:n,
output = output*i;
end
其中fact是函数名，n是输入引数，output是输出引数，而i则是此函数用
到的暂时变数。要使用此函数，直接键入函数名及适当输入引数值即可：
**MATLAB**的函数也可以是递 式的（Recursive），也就是说，一个函数可以
呼叫它本身。举例来说，n! =
n*(n-1)!，因此前面的阶乘函数可以改成递 式的写法：
function output = fact(n)% FACT Calculate factorial of a given positive integer recursively.
if n == 1, % Terminating condition
output = 1;
return;
end
output = n*fact(n-1);
在写一个递 函数时，一定要包含结束条件（Terminating
condition），否则此函数将会一再呼叫自己，永远不会停止，直到电脑的
记忆体被耗尽为止。以上例而言，n==1即满足结束条件，此时我们直接将
output设为1，而不再呼叫此函数本身。
