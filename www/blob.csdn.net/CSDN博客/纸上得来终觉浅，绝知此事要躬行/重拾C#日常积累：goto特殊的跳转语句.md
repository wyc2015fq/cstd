# 重拾C#日常积累：goto特殊的跳转语句 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 15:46:37[boonya](https://me.csdn.net/boonya)阅读数：258








##### goto跳转语句说明


- goto语句将程序控制直接传递给标记语句。
- goto的一个通常用法是将控制传递给特定的 switch-case 标签或switch语句中的默认标签。
- goto语句还用于跳出深嵌套循环。

##### 示例程序

1、在switch当中

```
class SwitchTest
{
    static void Main()
    {
        Console.WriteLine("Coffee sizes: 1=Small 2=Medium 3=Large");
        Console.Write("Please enter your selection: ");
        string s = Console.ReadLine();
        int n = int.Parse(s);
        int cost = 0;
        switch (n)
        {
            case 1:
                cost += 25;
                break;
            case 2:
                cost += 25;
                goto case 1;
            case 3:
                cost += 50;
                goto case 1;
            default:
                Console.WriteLine("Invalid selection.");
                break;
        }
        if (cost != 0)
        {
            Console.WriteLine("Please insert {0} cents.", cost);
        }
        Console.WriteLine("Thank you for your business.");

        // Keep the console open in debug mode.
        Console.WriteLine("Press any key to exit.");
        Console.ReadKey();
    }
}
/*
Sample Input:  2
 
Sample Output:
Coffee sizes: 1=Small 2=Medium 3=Large
Please enter your selection: 2
Please insert 50 cents.
Thank you for your business.
*/
```

2、goto跳出循环

```
public class GotoTest1
{
    static void Main()
    {
        int x = 200, y = 4;
        int count = 0;
        string[,] array = new string[x, y];

        // Initialize the array:
        for (int i = 0; i < x; i++)

            for (int j = 0; j < y; j++)
                array[i, j] = (++count).ToString();

        // Read input:
        Console.Write("Enter the number to search for: ");

        // Input a string:
        string myNumber = Console.ReadLine();

        // Search:
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                if (array[i, j].Equals(myNumber))
                {
                    goto Found;
                }
            }
        }

        Console.WriteLine("The number {0} was not found.", myNumber);
        goto Finish;

    Found:
        Console.WriteLine("The number {0} is found.", myNumber);

    Finish:
        Console.WriteLine("End of search.");


        // Keep the console open in debug mode.
        Console.WriteLine("Press any key to exit.");
        Console.ReadKey();
    }
}
/*
Sample Input: 44
 
Sample Output
Enter the number to search for: 44
The number 44 is found.
End of search.
*/
```
参考地址：[https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/goto](https://docs.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/goto)


