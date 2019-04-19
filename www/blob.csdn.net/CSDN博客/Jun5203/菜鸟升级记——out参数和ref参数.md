# 菜鸟升级记——out参数和ref参数 - Jun5203 - CSDN博客
2019年01月27日 10:25:34[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：81
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
【**背景**】
在学习的道路上，学的越多，迷糊的也越多。当然，如果通过对比学习，我们也更容易掌握其中的逻辑和道理。一生二二生三三生万物，学习这件小事儿，且行且珍惜。
借着今天天气不太好的原因，自己把有点模糊的点复习了一下，真的是不复习不知道，一复习吓一跳。由此我深有感触，以后学东西一定要及时总结，不能再天马行空的过去。好了，吐槽到这儿，下面就跟着小编来巩固新知识吧。
【**芝士**】
我们知道，**函数**参数默认是通过值传递的。
**例子**
```
int age = 18;
Console.WriteLine(age);
```
那么，**方法**的参数是如何做的呢？
不用out
```
class Program
    {
        static void Main(string[] args)
        {
            int number1 = 10;
            int number2 = 20;
            int sum = AddNumber(number1, number2);
            Console.WriteLine(number1);
            Console.WriteLine(number2);
            Console.WriteLine(sum);
        }
        public static int AddNumber(int num1, int num2)
        {
            return num1 + num2;
        }
    }
```
**结果**
```
10
20
30
```
使用out
```
class Program
    {
        static void Main(string[] args)
        {
            int number1=999;//赋不赋值都可，如果参数有out修饰，赋值也会被覆盖
            int number2 = 90;//因为参数number2前没有out修饰，所以在方法外必须给number2赋初值
            int sum = AddNumber(out number1, number2);//调方法
            Console.WriteLine(number1);
            Console.WriteLine(number2);
            Console.WriteLine(sum);
            Console.ReadKey();
        }
		//如果方法参数前边有out修饰，在方法外边调这个方法时参数前边也得加out
        public static int AddNumber(out int num1, int num2)
        {
            num1 = 100;//因为参数num1有out修饰，所以在方法里给num1赋值，并传到方法外
            num2 = 200;//传不出去，只能在方法中使用
            return num1 + num2;
        }
    }
```
通过以上例子，我们可以得出
1、如果方法的参数被out修饰，那么这个参数必须在方法**里**进行赋值
2、参数被赋的值会传到方法外，相当于给方法外的参数重新赋值
3、如果方法的参数被out修饰，如果在方法外调用这个方法，则对应的参数前也得用out修饰
使用ref
```
class Program
    {
        static void Main(string[] args)
        {
            int number1 = 10;
            int number2 = 23;
            int sum = Show(ref number1, ref number2);
            Console.WriteLine(number1);
            Console.WriteLine(number2);
            Console.WriteLine(sum);
        }
        public static int Show(ref int num1, ref int num2)
        {
            num1 = 100;
            num2 = 200;
            return num1 + num2;
        }
    }
```
**结果**
```
100
200
300
```
方法里没有赋值
```
    class Program
    {
        static void Main(string[] args)
        {
            int number1 = 10;
            int number2 = 23;
```
int sum = Show(ref number1, ref number2);
        Console.WriteLine(number1);
        Console.WriteLine(number2);
        Console.WriteLine(sum);
    }
    public static int Show(ref int num1, ref int num2)
    {
        //num1 = 100;
        //num2 = 200;
        return num1 + num2;
    }
}
```
**结果**
```
10
23
33
```
通过以上例子，我们可以得出
1、如果方法的参数被ref修饰，那么这个参数必须在方法**外**进行赋值
2、参数被赋的值会传到方法外，相当于给方法外的参数重新赋值
3、如果方法的参数被ref修饰，如果在方法外调用这个方法，则对应的参数前也得用ref修饰
4、如果在方法里有赋值，则会被传到方法外进行重新赋值；如果在方法里没有赋值，则会把传到方法里的值再传到方法外。
【**小编有话说**】
别在乎你付出的时间与精力，别担心你为了这项事业用去的金钱像是打水漂似的，因为你只要付出了，努力了，便会有收获，而这收获的成果往往比付出的要更好。
