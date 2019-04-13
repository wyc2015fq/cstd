
# c\#二分查找 - 追求卓越,做到专业 - CSDN博客


2012年03月15日 16:35:40[Waldenz](https://me.csdn.net/enter89)阅读数：2715标签：[c\#																](https://so.csdn.net/so/search/s.do?q=c#&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=class&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=string&t=blog)个人分类：[C\#.Net																](https://blog.csdn.net/enter89/article/category/877633)[算法																](https://blog.csdn.net/enter89/article/category/1104305)[
							](https://blog.csdn.net/enter89/article/category/877633)
[
				](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
二分查找又称折半查找，它是一种效率较高的查找方法。
二分查找要求：线性表是有序表，即表中结点按关键字有序，并且要用向量作为表的存储结构。不妨设有序表是递增有序的。

```python
public class Program
    {
    
        public static int Binsch(int[] a, int key)
        {
            int low = 0;
            int high = a.Length-1;
            while (low <= high)
            {
                int mid = (low + high) / 2;
                if (key == a[mid])
                {
                    return mid;  //返回找到的索引值
                }
                else
                {
                    if (key < a[mid])
                        high = mid - 1;
                    else
                        low = mid + 1;
                }
            }
            return -1; //查找失败
        }
        public static void Main(string[] args)
        {
          
                 int n = 100;//数组长度
                int[] arr = new int[n];
                for (int i = 1; i <= n; i++)
                {
                    arr[i - 1] = i;//初始化数组
                    Console.Write(i.ToString() + ",");//输出数组
                }
                
                Stopwatch sw = Stopwatch.StartNew();
                int result = Binsch(arr, 17);//查找17
                sw.Stop();
                Console.WriteLine(sw.Elapsed);//显示运行时间

                Console.WriteLine(result);//查询结果
        }
    }
```



