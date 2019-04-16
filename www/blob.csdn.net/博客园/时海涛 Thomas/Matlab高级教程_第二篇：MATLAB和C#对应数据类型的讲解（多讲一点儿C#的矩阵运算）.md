# Matlab高级教程_第二篇：MATLAB和C#对应数据类型的讲解（多讲一点儿C#的矩阵运算） - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Matlab高级教程_第二篇：MATLAB和C#对应数据类型的讲解（多讲一点儿C#的矩阵运算）](https://www.cnblogs.com/noah0532/p/9841037.html)





1. MATLAB对应C#的数据类型主要在引入的父类库MWArray当中。有如下对应规则

.NET TYPE　　　　　　MWArrayTYPE　　　　MATLAB Type

System.Double　　　　 MWNumericArray　　　　double

System.Number　　　　MWNumericArray　　　　double

System.Float　　　　　MWNumericArray　　　　 single

System.Byte　　　　　 MwNumbericArray　　　　int8

System.Short　　　　　MWNumbericArray　　　  int16

System.int32　　　　　 MWNumbericArray　　　  int32

system.int64　　　　　 MWNumbericArray　　　  int64

System.Char　　　　　MWCharArray　　　　      char

System.String　　　　   MWCharArray　　　　     char

System.Boolean　　　　MWLogicArray　　　　   logical

N/A　　　　　　　　　  MWStructArray　　　　  structure

N/A　　　　　　　　　　MWCellArray　　　　   cell



2. MWArray是MathWorks公司专门为.Net开发的一个.Net组件，用于条用MCR进行m函数进行计算。是.Net和matlab混编的桥梁之一。

3. C#怎么向MATLAB传递参数？怎么把MATLAB计算好的结果，转换到C#中？

           //混合编程传递参数测试
            //1. 假设M函数已经写好，并且编译完成
            //传递参数之前要搞清楚什么问题？
            //要传递的参数实际类型：N=5，double[] 
            //MWArray，怎么把.net中的数据传递给MWArray？

            //2.要传递数值类型：double，int，byte...
            //单个值可以隐式转换
            MWArray m1 = 5;
            int N = 5;
            MWArray m2 = N;
            MWArray str = "my name is matlab.net";


            //3.如果M函数需要一个数组（一维数组）
            //多个值不能通过隐式转换，因此这个是错误的，也不能强制转换！！
            //MWArray m3 = new int[] {1,2,3,4 };
            //因此！要通过子类的MWNumericArray的数组类型先要进行转换。
            //因此！.net数值类型的数组可以隐式转换为MWNumericArray。
            //MWNumericArray他是MWArray的子类
            MWNumericArray array1 = new int[] { 112, 22, 44, 88 };
            //通过做子类型再转换成父类型。
            MWArray array2 = array1;
            //因此！可以通过用子类型的数组进行强制转换
            MWArray array3 = (MWNumericArray)new int[] { 112, 22, 44, 88 };


            //4.如果M函数需要一个多维数组
            MWNumericArray array4 = new int[,] { {112, 22}, {44, 88} };
            MWArray array5 = array4;
            //或者
            MWArray array6 = (MWNumericArray)new int[,] { { 112, 22 }, { 44, 88 } };
            //Console.WriteLine(array5);

            //5. MWNumericArray的构造函数
            MWNumericArray array7 = new MWNumericArray(new int[] { 2, 35, 6 }); //全0
            MWNumericArray array8 = new MWNumericArray(2,3,new double[] { 1, 3, 5, 4, 5, 6 }); //两行2列的double数组
            //Console.WriteLine(array8);
            //1 3 5
            //4 5 6


4. 矩阵运算导入类Matrix，这个有现成的写好的类，可以联系作者要（不过写的有问题，后面生成MATLAB的矩阵运算方式）

            //6. 矩阵运算
            //6.1 实例化矩阵，并规定是几行几列的矩阵
            //注意：矩阵维度是从0开始的
            Console.WriteLine("这是第一个矩阵");
            Matrix matrix = new Matrix(2,2);  //2行 2列
            //赋值方式
            matrix[0, 0] = 3;
            matrix[0, 1] = 4;
            matrix[1, 0] = 8;
            matrix[1, 1] = 6;
            //Console.WriteLine(matrix[0, 0]);
            //Console.WriteLine(matrix[0, 1]);
            //Console.WriteLine(matrix[1, 0]);
            //Console.WriteLine(matrix[1, 1]);
            //6.2打印矩阵
            matrix.Print();
            Console.WriteLine("\n");
            //6.3 矩阵的四则运算
            Console.WriteLine("这是第二个矩阵");
            Matrix matrix1 = new Matrix(2, 2);  //2行 2列
            //赋值方式
            matrix1[0, 0] = 4;
            matrix1[0, 1] = 40;
            matrix1[1, 0] = 80;
            matrix1[1, 1] = 60;
            matrix1.Print();
            Console.WriteLine("\n");

            Console.WriteLine("这是第三个矩阵");
            Matrix matrix2 = new Matrix(3, 3);  //3行 3列
            //赋值方式
            matrix2[0, 0] = 4;
            matrix2[0, 1] = 40;
            matrix2[0, 2] = 40;
            matrix2[1, 0] = 30;
            matrix2[1, 1] = 10;
            matrix2[1, 2] = 60;
            matrix2[2, 0] = 80;
            matrix2[2, 1] = 50;
            matrix2[2, 2] = 60;
            matrix2.Print();
            Console.WriteLine("\n");

            //加法
            Console.WriteLine("矩阵的加法的结果");
            var result = matrix + matrix1;
            result.Print();
            Console.WriteLine("\n");

            //减法
            Console.WriteLine("矩阵的减法的结果");
            var result2 = matrix - matrix1;
            result2.Print();
            Console.WriteLine("\n");

            //乘法
            Console.WriteLine("矩阵的乘法的结果");
            var result3 = matrix * matrix2;
            result3.Print();
            Console.WriteLine("\n");

            //除法没有




5.继续进行讲解

            //6. 定义字符
            MWCharArray ch = "Hello Matlab";
            MWCharArray ch1 = new MWCharArray(new string[] { "Hellow","World!"}); //传递一个字符串数组

            //7. 定义struct结构体
            MWStructArray stru = new MWStructArray(new int[] { 2, 2 }, new string[] { "Name", "Age" });
            stru["Name", new int[] { 1, 1 }] = "Tom";
            stru["Age", new int[] { 1, 1 }] = 18;
            Console.WriteLine(string.Format("Name:{0},Age:{1}",stru["Name",new int[] { 1,1}].ToString(),stru["Age",new int[] {1,1 }]));
            //MWArray下标访问方式跟MATLAB一样。

            //8.元组的自己看吧

















