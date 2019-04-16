# Matlab高级教程_第二篇：MATLAB和C#一些常用的矩阵运算方法的转换 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：MATLAB和C#一些常用的矩阵运算方法的转换](https://www.cnblogs.com/noah0532/p/9843848.html)





1.相关方法已经生产引用，直接调用的结果如下：

![](https://img2018.cnblogs.com/blog/1328368/201810/1328368-20181024153538629-799767733.png)

![](https://img2018.cnblogs.com/blog/1328368/201810/1328368-20181024153558825-1449506232.png)

![](https://img2018.cnblogs.com/blog/1328368/201810/1328368-20181024153654579-957267460.png)

2. 相关调用代码如下：

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using MathWorks.MATLAB.NET.Arrays;
using MathWorks.MATLAB.NET.Utility;

using MatrixCalc;

namespace 矩阵运算转换测试
{
    class Program
    {
        static void Main(string[] args)
        {
            //生成一个double类型的矩阵，用内置的MATLAB父类MWArray方法
            //第一个矩阵
            Console.WriteLine("第一个矩阵");//2行2列
            MWArray matrix1 = (MWNumericArray)new double[2, 2] { { 10.5, 25.6 }, { 55.4, 66.8 } };
            Console.WriteLine(matrix1);
            //第二个矩阵
            Console.WriteLine("第二个矩阵");//2行2列
            MWArray matrix2 = (MWNumericArray)new double[2, 2] { { 70.5, 28.6 }, { 35.3, 63.9 } };
            Console.WriteLine(matrix2);
            //第三个矩阵
            Console.WriteLine("第三个矩阵");//3行3列
            MWArray matrix3 = (MWNumericArray)new double[3, 3] { { 70.5, 28.6,77.88 }, { 35.3, 63.9,55.66 },{22.58,66.98,44.77 } };
            Console.WriteLine(matrix3);
            //第四个向量
            Console.WriteLine("第三个向量");//3行3列
            MWArray array = (MWNumericArray)new double[1, 3] { { 70.5, 28.6, 77.88 }};
            Console.WriteLine(array);
            Console.WriteLine("\n");

            //进行矩阵的加法运算
            Console.WriteLine("第一个矩阵和第二个矩阵的加法运算结果是：");
            MatrixCalc.MatrixCalc Add = new MatrixCalc.MatrixCalc(); //实例化
            var AddResult = Add.AddMatrixs(matrix1,matrix2);//调用加法方法
            //var AddResult = Add.AddMatrixs(matrix1, matrix3);//调用加法方法//此处报错，因为两个矩阵的维度不一样
            Console.WriteLine(AddResult);
            Console.WriteLine("\n");

            //进行矩阵的加法运算
            Console.WriteLine("第一个矩阵和第二个矩阵的减法运算结果是：");
            MatrixCalc.MatrixCalc Sub = new MatrixCalc.MatrixCalc(); //实例化
            var SubResult = Sub.SubMatrixs(matrix1, matrix2);//调用加法方法(上面一样注意矩阵维度要一致)
            Console.WriteLine(SubResult);
            Console.WriteLine("\n");

            //进行矩阵的乘法运算
            MatrixCalc.MatrixCalc Mul = new MatrixCalc.MatrixCalc(); //实例化
            var MulResult1 = Mul.MulMatrixs(matrix1, matrix2);//调用加法方法
            //var MulResult2 = Mul.MulMatrixs(matrix1, matrix3);//调用加法方法
            Console.WriteLine("第一个矩阵和第二个矩阵的乘法运算结果是：");
            Console.WriteLine(MulResult1);
            //Console.WriteLine("第一个矩阵和第三个矩阵的乘法运算结果是：");//错误维度不一样
            //Console.WriteLine(MulResult2);
            Console.WriteLine("第四个向量和第三个矩阵的乘法运算结果是：");
            var MulResult3 = Mul.MulMatrixs(array, matrix3);//调用加法方法
            Console.WriteLine(MulResult3);
            Console.WriteLine("\n");

            //进行矩阵的乘法运算（点乘运算）
            Console.WriteLine("第一个矩阵和第二个矩阵的点乘法运算结果是：");
            MatrixCalc.MatrixCalc DotMul = new MatrixCalc.MatrixCalc(); //实例化
            var DotMulResult = DotMul.DotMulMatrixs(matrix1, matrix2);//调用加法方法(注意矩阵维度)
            Console.WriteLine(DotMulResult);
            Console.WriteLine("\n");

            //进行矩阵的除法运算(正向除法)
            Console.WriteLine("第一个矩阵和第二个矩阵的除法运算结果是：");
            MatrixCalc.MatrixCalc Div = new MatrixCalc.MatrixCalc(); //实例化
            var DivResult = Div.DivMatrixs(matrix1, matrix2);//调用加法方法(注意矩阵维度)
            Console.WriteLine(DivResult);
            Console.WriteLine("\n");

            //进行矩阵的点除法运算(正向点除法)
            Console.WriteLine("第一个矩阵和第二个矩阵的点除法运算结果是：");
            MatrixCalc.MatrixCalc DotDiv = new MatrixCalc.MatrixCalc(); //实例化
            var DotDivResult = DotDiv.DotDivMatrixs(matrix1, matrix2);//调用加法方法(注意矩阵维度)
            Console.WriteLine(DotDivResult);
            Console.WriteLine("\n");

            //进行矩阵的平方法运算(第二个值是平方值，不是两个矩阵的平方运算)
            Console.WriteLine("第一个矩阵的平方法运算结果是：");
            MatrixCalc.MatrixCalc Square = new MatrixCalc.MatrixCalc(); //实例化
            var SquareResult = Square.SquareMatrixs(matrix1, 2);//调用加法方法
            Console.WriteLine(SquareResult);
            Console.WriteLine("\n");

            //进行矩阵的逆矩阵运算
            Console.WriteLine("第一个矩阵逆矩阵运算结果是：");
            MatrixCalc.MatrixCalc Inv = new MatrixCalc.MatrixCalc(); //实例化
            var InvResult = Inv.InvMatrix(matrix1);//调用加法方法(注意矩阵维度)
            Console.WriteLine(InvResult);
            Console.WriteLine("\n");

            //进行矩阵的转置运算
            Console.WriteLine("第一个矩阵转置矩阵运算结果是：");
            MatrixCalc.MatrixCalc Trans = new MatrixCalc.MatrixCalc(); //实例化
            var TransResult = Trans.TransMatrix(matrix1);//调用加法方法(注意矩阵维度)
            Console.WriteLine(TransResult);
            Console.WriteLine("\n");

            //进行矩阵的行列式运算
            Console.WriteLine("第一个矩阵行列式运算结果是：");
            MatrixCalc.MatrixCalc Det= new MatrixCalc.MatrixCalc(); //实例化
            var DetResult = Det.DetMatrix(matrix1);//调用加法方法
            Console.WriteLine(DetResult);
            Console.WriteLine("\n");

            //进行矩阵的秩运算
            Console.WriteLine("第一个矩阵秩运算结果是：");
            MatrixCalc.MatrixCalc Rank = new MatrixCalc.MatrixCalc(); //实例化
            var RankResult = Rank.RankMatrix(matrix1);//调用加法方法
            Console.WriteLine(RankResult);
            Console.WriteLine("\n");

            //进行矩阵的特征值运算
            Console.WriteLine("第一个矩阵特征值运算结果是：");
            MatrixCalc.MatrixCalc Eig = new MatrixCalc.MatrixCalc(); //实例化
            var EigResult = Eig.EigMatrix(matrix1);//调用加法方法
            Console.WriteLine(EigResult);
            Console.WriteLine("\n");

        }


    }
}


3. 和MATLAB运行结果一致　　














