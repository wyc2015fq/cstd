# (C#基础) byte[] 之初始化, 赋值，转换。 - fighting！！！ - CSDN博客
2017年11月02日 21:59:54[dujiahei](https://me.csdn.net/dujiahei)阅读数：505
(C#基础) byte[] 之初始化, 赋值，转换。 
- fdyang - 博客园  http://www.cnblogs.com/fdyang/archive/2013/10/20/3378974.html?_t_t_t=0.2690988593207204
**byte[] 之初始化赋值**
用for loop 赋值当然是最基本的方法，不过在C#里面还有其他的便捷方法。
1. 创建一个长度为10的byte数组，并且其中每个byte的值为0.
```
byte[] myByteArray = new byte[10];
```
C# 在创建数值型(int, byte)数组时，会自动的把数组中的每个元素赋值为0.  （注：如果是string[], 则每个元素为的值为null. 
2. 创建一个长度为10的byte数组，并且其中每个byte的值为0x08. 
```
byte[] myByteArray = Enumerable.Repeat((byte)0x08, 10).ToArray();
```
用linq来赋值，语句只要一条， 当然我们还可以赋值不同的，但是有一定规律的值。
```
byte[] res= Enumerable.Range(1, 1000).Select(c=>Convert.ToByte(c)).ToArray();
```
3. 直接赋值
```
byte[] myByteArray = new byte[] { 0x01, 0x02, 0x03 };
```
**byte[] ---> ushort**
```
byte[] array = new byte[] { 0xFE, 0x00 };
            ushort register = BitConverter.ToUInt16(array, 0);
```
上述转换后register 的值为 0x00FE
```
byte[] array = new byte[] { 0x02, 0x01 ,0x04, 0x03 };
            ushort register = BitConverter.ToUInt16(array, 0);
```
上述转化后，其实只是取了array[0], array[1].的值，最后register 的值是 0x00010002， 即258
**byte[] -> string**
```
public static string ByteArrayToString(byte[] ba)
{
  string hex = BitConverter.ToString(ba);
  return hex.Replace("-","");
}
```
**ushort ---> byte[]**
```
ushort register = 0x00F0;
            byte[] arr = BitConverter.GetBytes(register);
```
在PC系统里, arr[0] = 0xF0(地位), arr[1] = 0x00 . 
**互换ushort中的两个字节**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
            ushort number = 0x00F0;
            byte[] temp = BitConverter.GetBytes(number);
            Array.Reverse(temp); 
            ushort a = BitConverter.ToUInt16(temp, 0);
            ushort b = (ushort)(number << 8 | ((number & 0xFF00) >> 8));
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
** byte[] => Struct**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public StructType ConverBytesToStructure<StructType>(byte[] bytesBuffer)
        {
            // 检查长度。
            if (bytesBuffer.Length != Marshal.SizeOf(typeof(StructType)))
            {
                throw new ArgumentException("bytesBuffer参数和structObject参数字节长度不一致。");
            }
            IntPtr bufferHandler = Marshal.AllocHGlobal(bytesBuffer.Length);
            for (int index = 0; index < bytesBuffer.Length; index++)
            {
                Marshal.WriteByte(bufferHandler, index, bytesBuffer[index]);
            }
            StructType structObject = (StructType)Marshal.PtrToStructure(bufferHandler, typeof(StructType));
            Marshal.FreeHGlobal(bufferHandler);
            return structObject;
        }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
代码
　　　　　/// <summary>
        /// 将byte[]还原为指定的struct,该函数的泛型仅用于自定义结构
        /// startIndex：数组中 Copy 开始位置的从零开始的索引。
        /// length：要复制的数组元素的数目。
        /// </summary>
        public static T BytesToStruct<T>(byte[] bytes, int startIndex, int length)
        {
            if (bytes == null) return default(T);
            if (bytes.Length <= 0) return default(T);
            IntPtr buffer = Marshal.AllocHGlobal(length);
            try//struct_bytes转换
            {
                Marshal.Copy(bytes, startIndex, buffer, length);
                return (T)Marshal.PtrToStructure(buffer, typeof(T));
            }
            catch(Exception ex)
            {
                throw new Exception("Error in BytesToStruct ! " + ex.Message);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**Struct => byte[]**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
代码 
　　　　　/// <summary>
        /// 将struct类型转换为byte[]
        /// </summary>
        public static byte[] StructToBytes(object structObj, int size)
        {
            IntPtr buffer = Marshal.AllocHGlobal(size);
            try//struct_bytes转换
            {
                Marshal.StructureToPtr(structObj, buffer, false);
                byte[] bytes = new byte[size];
                Marshal.Copy(buffer, bytes, 0, size);
                return bytes;
            }
            catch (Exception ex)
            {
                throw new Exception("Error in StructToBytes ! " + ex.Message);
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**byte[] 数组比较**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//You can use Enumerable.SequenceEqual method.
using System;
using System.Linq;
...
var a1 = new int[] { 1, 2, 3};
var a2 = new int[] { 1, 2, 3};
var a3 = new int[] { 1, 2, 4};
var x = a1.SequenceEqual(a2); // true
var y = a1.SequenceEqual(a3); // false
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**string类型转成byte[]：**
byte[] byteArray = System.Text.Encoding.Default.GetBytes ( str );
**byte[]转成string：**
string str = System.Text.Encoding.Default.GetString ( byteArray );
**string类型转成ASCII byte[]：**
（"01" 转成 byte[] = new byte[]{ 0x30,0x31}）
byte[] byteArray = System.Text.Encoding.ASCII.GetBytes ( str );
**ASCIIbyte[]转成string：**
（byte[] = new byte[]{ 0x30, 0x31} 转成"01"）
string str = System.Text.Encoding.ASCII.GetString ( byteArray );
参考： [http://www.dotnetperls.com/initialize-array](http://www.dotnetperls.com/initialize-array)
