# C# 内存分配与释放示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年02月01日 15:24:18[boonya](https://me.csdn.net/boonya)阅读数：1398








原文地址：[https://msdn.microsoft.com/zh-cn/library/ms146631.aspx](https://msdn.microsoft.com/zh-cn/library/ms146631.aspx)




```
using System;
using System.Runtime.InteropServices;

class Example
{

    static void Main()
    {
        // Create a managed array.
        byte[] managedArray = { 1, 2, 3, 4 };

        // Initialize unmanaged memory to hold the array.
        int size = Marshal.SizeOf(managedArray[0]) * managedArray.Length;

        IntPtr pnt = Marshal.AllocHGlobal(size);

        try
        {
            // Copy the array to unmanaged memory.
            Marshal.Copy(managedArray, 0, pnt, managedArray.Length);

            // Copy the unmanaged array back to another managed array.

            byte[] managedArray2 = new byte[managedArray.Length];

            Marshal.Copy(pnt, managedArray2, 0, managedArray.Length);

            Console.WriteLine("The array was copied to unmanaged memory and back.");

        }
        finally
        {
            // Free the unmanaged memory.
            Marshal.FreeHGlobal(pnt);
        }



    }

}
```





