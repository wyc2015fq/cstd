# 整理一些C#中常用的一些类型转换_ts - 数据之美的博客 - CSDN博客
2017年08月16日 14:08:00[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：358
//在C#中采用Console.ReadLine()按行读取输入流，一般默认为string
//string 转换成 Char[]
        string str1 = "doyoulikeme";
        char[] str2 = str1.ToCharArray();
//char[] 转换成 string
char[] ans ={ 'y', 'e', 's', 'i', 'd', 'o'};
string str2 = new string(ans);
//string 转换成 int
string str = "123";
        int result = int.Parse(str);
//String 转换成 int
string str = "abcd";
        int result;
        int.TryParse(str, out result);
//输入时直接转换
int var=int.Parse(Console.ReadLine());
//int 转换成 string
int var = 1;
        string Str1 = Convert.ToString(var);
        string Str2 = var.ToString();
//string 转换成 List
string s = "a,b,c";
        List<string> list = new List<string>(s.Split(','));
//List转换成string
List<string> list = new List<string>();
        list.Add("abc");
        list.Add("d");
        string s = string.Join(",", list.ToArray());
        Console.WriteLine(s);

