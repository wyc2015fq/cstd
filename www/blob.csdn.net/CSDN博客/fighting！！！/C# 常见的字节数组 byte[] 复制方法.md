# C# 常见的字节数组 byte[] 复制方法 - fighting！！！ - CSDN博客
2017年11月02日 22:01:16[dujiahei](https://me.csdn.net/dujiahei)阅读数：5196
源地址：C# 常见的字节数组 byte[] 复制方法 - 仆人 - 博客园  http://www.cnblogs.com/craigtao/p/4308143.html
```
byte[] src ={1,2,3,4,5};
byte[] dest = new byte[src.Length];
for(int i=0; i<src.Length; i++)
{
    dest[i] = src[i]
}
```
```
1 byte[] src ={1,2,3,4,5};
2 byte[] dest = new byte[src.Length];
3 Array.Copy(src, dest, src.Length);
```
```
byte[] src ={1,2,3,4,5};
byte[] dest;
dest =(byte[])src.Clone();
```
欢迎大家分享更好的拷贝方法，比如完全的复制（深拷贝）
2015年7月2日  craigtao  新增 Buffer.BlockCopy  方法
```
1 byte[] srcArray = new byte[] { 0x01, 0x02, 0x03, 0x04 };
2 byte[] dstArray = new byte[srcArray.Length];
3 
4 Buffer.BlockCopy(srcArray, 0, dstArray, 0, srcArray.Length);
```
