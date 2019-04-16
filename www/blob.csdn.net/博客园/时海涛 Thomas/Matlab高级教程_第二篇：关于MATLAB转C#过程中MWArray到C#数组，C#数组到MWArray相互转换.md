# Matlab高级教程_第二篇：关于MATLAB转C#过程中MWArray到C#数组，C#数组到MWArray相互转换 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：关于MATLAB转C#过程中MWArray到C#数组，C#数组到MWArray相互转换](https://www.cnblogs.com/noah0532/p/9901474.html)





Matlab传递数据时使用的方法，那么Matlab计算完成后在C#中应该怎么获取它的计算数据呢？

需要遵循两个基本步骤：

弄清楚Matlab函数传回的数据到底是什么格式？struct？cell？char？抑或是numeric？
将MWArray转化为对应的MW****Array并从MW****Array中获取所需的数据
如何传递数据
从MWArray到C#中的数字
MWNumericArray中的数据可以转化为Byte、double、float、integer、long和short类型，一般常用的就是int和double两种。

同样使用举例说明的方式来讲解：

MWArray mwArr = (MWNumericArray)new int[] { 1, 2, 3, 4 };
MWNumericArray arr = (MWNumericArray)mwArr;
int a = arr[1, 1].ToScalarInteger();
double b = arr[1, 2].ToScalarDouble();
1
2
3
4
ToScalar****这类函数是MWNumericArray内部数据转化为C#中数字类的数据类型所使用的方法，字面意思就是转化为某标量类型。

从MWArray到字符串
先从MWArray转化为MWCharArray，然后直接给string赋值

MWArray mwArr = "c# and matlab";
MWCharArray arr = (MWCharArray)mwArr;
string str = arr.ToString();
1
2
3
从MWArray到数组
先从MWArray转化为MWNumericArray，然后在C#中进行类型转化

对于二维数组：

MWArray mwArr = (MWNumericArray)new double[2, 2] { { 1.1, 2.2 }, { 3.3, 4.4 } };
Array arr = mwArr.ToArray();
double[,] dArr = (double[,])arr;
1
2
3
或者直接

double[,] dArr = (double[,])mwArr.ToArray();
1
对于一维数组，还有另外的方法可以使用：

MWArray mwArr2 = (MWNumericArray)new double[] { 1.1, 2.2 };
Array dArr3 = ((MWNumericArray)mwArr2).ToVector(MWArrayComponent.Real);
double[] arr = (double[])dArr3;
1
2
3
ToVector的参数MWArrayComponent.Real是指复数中的实部，与此相对的是MWArrayComponent.Imaginary，这个是虚部。如果二维数组使用上述方法转化，会变成一维数组。new double[2, 2] { { 1.1, 2.2 }, { 3.3, 4.4 } }会变成new double[]{1.1,3.3,2.2,4.4}。












