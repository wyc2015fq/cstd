# 【C# String】Prepare For CodeHunt - CD's Coding - CSDN博客





2015年03月15日 01:39:04[糖果天王](https://me.csdn.net/okcd00)阅读数：580标签：[string																[c#																[String类常用函数记忆](https://so.csdn.net/so/search/s.do?q=String类常用函数记忆&t=blog)




```
//String 字符串的常见操作
        public static void Fun1()
        {
            string MyStr = " Hello World! ";

            //length长度属性
            Console.WriteLine(MyStr.Length);

            //Substring()截取
            Console.WriteLine(MyStr.Substring(0, 5));

            //ToCharArray()转换为字符数组
            char[] charr = MyStr.ToCharArray();

            //ToUpper()转换大写
            string UpperStr = MyStr.ToUpper();
            Console.WriteLine("转换为大写：" + UpperStr + "\n");

            //ToLower()转换小写
            Console.WriteLine("转换为小写：" + MyStr.ToLower());

            //TrimStart() and TrimEnd() 去掉字符左右边的空格 TrimStart--去掉左边
            string LefStr = MyStr.TrimStart();
            string RigStr = MyStr.TrimEnd();

            Console.WriteLine("左边 " + LefStr + "\n" + "右边 " + RigStr + "\n");

            //Replace()替换字符
            string ReplaceStr = MyStr.Replace('W', 'M');
            Console.WriteLine("替换字符 " + ReplaceStr + "\n");

            //Concat()连接字符
            string concatStr = string.Concat(MyStr, "asas", "asasa");
            Console.WriteLine("连接字符 " + concatStr + "\n");

            //IsNullOrEmpty()判断空值
            bool NullorEmpty = string.IsNullOrEmpty(MyStr);
            Console.WriteLine("判断空值 " + NullorEmpty.ToString() + "\n");

            //StartsWith()开头
            Console.WriteLine(MyStr.StartsWith(" ") == true);

            //EndsWith()结尾
            bool endwith = MyStr.EndsWith(" ");
            Console.WriteLine("结尾 " + endwith.ToString() + "\n");

            //IndexOf() and LastIndexOf()字符串的位置
            string strLocation = "123abc4c56efd";
            Console.WriteLine("IndexOf:" + strLocation.IndexOf('1') + "\n");
            Console.WriteLine("LastIndexOf:" + strLocation.LastIndexOf('c') + "\n");

            //插入字符
            Console.WriteLine(MyStr.Insert(2, "W"));

            //MyStr.PadLeft();MyStr.PadRight()

            //Remove()移除
            Console.WriteLine("Remove:" + strLocation.Remove(0, 1) + "\n");

            //Format()格式化
            string str = string.Format("{0:X4}", 3);
            Console.WriteLine(str);

            //Compare()和CompareTo() 比较方法
            string strA = "File.exe";
            string strB = "0";

            int numA = string.Compare(strA, strB);
            Console.WriteLine("Compare: " + numA.ToString() + "\n");

            int numB = strA.CompareTo(strB);
            Console.WriteLine("CompareTo: " + numB.ToString());


            //比较两个指定的 System.String 对象。
            // 返回结果:
            // 一个 32 位有符号整数，指示两个比较数之间的词法关系。
            //值条件小于零 strA 小于 strB。 零strA 等于 strB。 大于零 strA大于 strB。 

        }
```](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)




