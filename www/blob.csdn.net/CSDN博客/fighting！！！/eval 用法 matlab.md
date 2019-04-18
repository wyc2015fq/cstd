# eval 用法 matlab - fighting！！！ - CSDN博客
2018年07月08日 18:51:05[dujiahei](https://me.csdn.net/dujiahei)阅读数：1385
转自： https://bbs.sjtu.edu.cn/bbsanc,path,%2Fgroups%2FGROUP_4%2FMathTools%2FD547E59D4%2FD50A23A11%2FD611483FC%2FM.1328235870.A.html
help eval 将看到matlab自带的说明
eval Execute string with MATLAB expression. eval(s), where s is a string, caus
es MATLAB to execute the string as an expression or statement.
翻译一下，就是说eval函数的功能是将字符串转换为matlab可执行语句。通俗而言，比如
你输入
a='b=1';
会在workspace里看见生成了变量a，a的类型是字符串，字符串的内容是b=1
然后你输入eval(a)
就会看见变量里生成了变量b，b是一个1乘1的double型矩阵，元素的值为1
也就是说，执行eval(a)相当于执行a的内容，相当于执行b=1
如果说for语句可以将数字进行循环，那么eval结合for语句后，则大大提高其循环范围，
比如你要载入一些数据，m1.mat，m2.mat等等一直到m100.mat
普通青年可能会这样写程序
load m1.mat
load m2.mat
然后一直写100行，这显然太笨了！但普通的for语句又无法完成
于是eval函数结合for语句就体现出其价值了
for i=1:100
    eval(['load ' num2str(i) '.mat'])
end
只需要三行！
除此之外，eval还广泛用于人机交互，因为matlab的gui中，get命令得到的str格式的字符
串，通常，会通过str2num将字符串转换为数字，供后续处理。而如果读入的str字符串是
cos，sin之类的指令呢？那么就需要结合eval函数，将字符串转换为指令。
有人问，我想生成100个变量，取名分别为A1~A100，怎么弄？
大部分人可能会认为无法实现，其实不然。这里以几个实例说明此类功能的实现。
针对上面提及的问题，给出一段代码，生成100个3*3随机变量，取名A1~A100：
- for i=1:100
- eval(['A',num2str(i),'=rand(3);'])
- end
- 
复制代码
实例2
论坛之前有童鞋问，有变量a='b',能不能将'b'变成一个变量名，将值赋给变量b。
这里就可以用eval命令解决。
- a='b';
- temp=200;%先将值赋给临时变量temp
- eval([a,'=temp;']); %注意！这里a是不带引号的！生成变量b，将temp的值赋给b
复制代码
