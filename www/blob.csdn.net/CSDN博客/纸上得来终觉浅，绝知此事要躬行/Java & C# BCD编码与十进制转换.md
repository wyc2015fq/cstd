# Java & C# BCD编码与十进制转换 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月30日 10:34:52[boonya](https://me.csdn.net/boonya)阅读数：240








**目录**

[BCD码介绍](#BCD%E7%A0%81%E4%BB%8B%E7%BB%8D)

[Java版本互转工具](#%C2%A0Java%E7%89%88%E6%9C%AC%E4%BA%92%E8%BD%AC%E5%B7%A5%E5%85%B7)

[C#版本互转工具](#C%23%E7%89%88%E6%9C%AC%E4%BA%92%E8%BD%AC%E5%B7%A5%E5%85%B7)

### BCD码介绍
- BCD码（Binary-Coded Decimal‎）亦称二进码十进数或二-十进制代码。用4位二进制数来表示1位十进制数中的0~9这10个数码。是一种二进制的数字编码形式，用二进制编码的十进制代码。BCD码这种编码形式利用了四个位元来储存一个十进制的数码，使二进制和十进制之间的转换得以快捷的进行。这种编码技巧最常用于会计系统的设计里，因为会计制度经常需要对很长的数字串作准确的计算。相对于一般的浮点式记数法，采用BCD码，既可保存数值的精确度，又可免却使电脑作浮点运算时所耗费的时间。此外，对于其他需要高精确度的计算，BCD编码亦很常用。
- BCD码是四位二进制码, 也就是将十进制的数字转化为二进制, 但是和普通的转化有一点不同, 每一个十进制的数字0-9都对应着一个四位的二进制码,对应关系如下: 十进制0 对应 二进制0000 ;十进制1 对应二进制0001 ....... 9 1001 接下来的10就有两个上述的码来表示 10 表示为00010000 也就是BCD码是遇见1001就产生进位,不象普通的二进制码,到1111才产生进位10000
- 在Java通信应用中，往往我们接收到数据流，而流是BCD编码后的。因此我们要转码来获得我们想要的十进制数据。下面的代码是BCD编码的数据流与十进制的转换，仅供参考。

**常用BCD码**
|十进制数|8421码|5421码|2421码|余3码|余3循环码|
|----|----|----|----|----|----|
|0|0000|0000|0000|0011|0010|
|1|0001|0001|0001|0100|0110|
|2|0010|0010|0010|0101|0111|
|3|0011|0011|0011|0110|0101|
|4|0100|0100|0100|0111|0100|
|5|0101|1000|1011|1000|1100|
|6|0110|1001|1100|1001|1101|
|7|0111|1010|1101|1010|1111|
|8|1000|1011|1110|1011|1110|
|9|1001|1100|1111|1100|1010|

###  Java版本互转工具

```java
package com.util;  
  
/** 
 * 编码工具类 
 *  
 * @author 崔素强 
 * @see BCD与十进制的转换 
 */  
public class BCDDecode {  
    /** 
     * @功能:测试用例 
     * @参数: 参数 
     */  
    public static void main(String[] args) {  
        byte[] b = str2Bcd("2010");  
        System.out.println(bcd2Str(b));  
    }  
  
    /** 
     * @功能: BCD码转为10进制串(阿拉伯数据) 
     * @参数: BCD码 
     * @结果: 10进制串 
     */  
    public static String bcd2Str(byte[] bytes) {  
        StringBuffer temp = new StringBuffer(bytes.length * 2);  
        for (int i = 0; i < bytes.length; i++) {  
            temp.append((byte) ((bytes[i] & 0xf0) >>> 4));  
            temp.append((byte) (bytes[i] & 0x0f));  
        }  
        return temp.toString().substring(0, 1).equalsIgnoreCase("0") ? temp  
                .toString().substring(1) : temp.toString();  
    }  
  
    /** 
     * @功能: 10进制串转为BCD码 
     * @参数: 10进制串 
     * @结果: BCD码 
     */  
    public static byte[] str2Bcd(String asc) {  
        int len = asc.length();  
        int mod = len % 2;  
        if (mod != 0) {  
            asc = "0" + asc;  
            len = asc.length();  
        }  
        byte abt[] = new byte[len];  
        if (len >= 2) {  
            len = len / 2;  
        }  
        byte bbt[] = new byte[len];  
        abt = asc.getBytes();  
        int j, k;  
        for (int p = 0; p < asc.length() / 2; p++) {  
            if ((abt[2 * p] >= '0') && (abt[2 * p] <= '9')) {  
                j = abt[2 * p] - '0';  
            } else if ((abt[2 * p] >= 'a') && (abt[2 * p] <= 'z')) {  
                j = abt[2 * p] - 'a' + 0x0a;  
            } else {  
                j = abt[2 * p] - 'A' + 0x0a;  
            }  
            if ((abt[2 * p + 1] >= '0') && (abt[2 * p + 1] <= '9')) {  
                k = abt[2 * p + 1] - '0';  
            } else if ((abt[2 * p + 1] >= 'a') && (abt[2 * p + 1] <= 'z')) {  
                k = abt[2 * p + 1] - 'a' + 0x0a;  
            } else {  
                k = abt[2 * p + 1] - 'A' + 0x0a;  
            }  
            int a = (j << 4) + k;  
            byte b = (byte) a;  
            bbt[p] = b;  
        }  
        return bbt;  
    }  
}
```

### C#版本互转工具

```
using System;
using System.Collections.Generic;
using System.Text;

namespace Base
{
    public class BCDHelper
    {
        /// <summary>
        /// 用BCD码压缩数字字符串
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static byte[] StrToBCD(string str)
        {
            return StrToBCD(str, str.Length);
        }
        public static byte[] StrToBCD(string str, int numlen)
        {
            while (str.Length < numlen)
            {
                str = "0" + str;
            }
            return ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes(str), 0, numlen);
        }

        /// <summary>
        /// 用BCD码压缩数字字符串
        /// </summary>
        /// <param name="NumBitByte"></param>
        /// <param name="offset">偏移量</param>
        /// <param name="numlen">数字字符串位数</param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static byte[] ByteArrayToBCD(byte[] NumBitByte, int offset, int numlen)
        {
            //8位的ascii码
            int Num4Len = (numlen + 1) / 2 - 1;
            byte[] Num4bitByte = new byte[Num4Len + 1];
            Num4bitByte[Num4Len] = 0;
            for (int i = 0; i <= numlen - 1; i++)
            {
                byte num = 0;

                if (i + offset >= NumBitByte.Length)
                {
                    //num = 255;
                    num = 0;
                }
                else
                {
                    num = Convert.ToByte(NumBitByte[i + offset] - 0x30);
                }

                if (i % 2 == 0)
                {
                    Num4bitByte[i / 2] = Convert.ToByte((Num4bitByte[i / 2] & 0xf) | ((num << 4) & 0xFF));
                }
                else
                {
                    Num4bitByte[i / 2] = Convert.ToByte((Num4bitByte[i / 2] & 0xf0) | num);
                }
            }

            return Num4bitByte;
        }
        /// <summary>
        /// BCD转int
        /// </summary>
        /// <param name="bcdNum"></param>
        /// <param name="offset">偏移量</param>
        /// <param name="numlen">数字字符串位数</param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static int bcdToInt(byte[] bcdNum, int offset, int numlen)
        {
            return Convert.ToInt32(bcdToString(bcdNum, offset, numlen));
        }



        /// <summary>
        /// BCD转字符串
        /// </summary>
        /// <param name="bcdNum"></param>
        /// <param name="offset">偏移量</param>
        /// <param name="numlen">数字字符串位数</param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static string bcdToString(byte[] bcdNum, int offset, int numlen)
        {
            string retString = "";
            int res;
            var len = Math.DivRem(numlen, 2, out res);
            if (res != 0)
            {
                len++;
            }

            for (int i = 0; i < len; i++)
            {
                retString += ((bcdNum[i + offset] & 0xf0) >> 4).ToString("x");
                retString += (bcdNum[i + offset] & 0xf).ToString("x");
            }



            //Dim byteChar As Byte() = New Byte(length - 1) {}
            //Dim tempHigh As Byte = 0, tempLow As Byte = 0
            //Dim i As Integer = 0
            //While tempHigh <> &HF AndAlso tempLow <> &HF0
            //    tempHigh = Convert.ToByte(bcdNum(i + offset) And &HF0)
            //    '取出高四位；
            //    tempHigh = Convert.ToByte(tempHigh >> 4)
            //    tempLow = Convert.ToByte((bcdNum(i + offset) And &HF) << 4)
            //    byteChar(i) = Convert.ToByte(tempLow Or tempHigh)
            //    i += 1
            //End While
            //Dim HexString As String() = BitConverter.ToString(byteChar).Trim().Split("-"c)
            //For Each str As String In HexString
            //    retString += str.Trim()
            //Next
            //Dim LastIndex As Integer = retString.IndexOf("F"c)
            //retString = retString.Substring(0, LastIndex)

            return retString.ToLower().Replace("f", "");
        }

        /// <summary>
        /// BCD码转小时分钟
        /// date:2012-12-18
        /// author:guozh
        /// </summary>
        /// <param name="data"></param>
        /// <param name="offset">偏移位</param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static System.DateTime Bcd2ToDateTime(byte[] data, int offset)
        {
            Int32 hour = default(Int32);
            Int32 minute = default(Int32);
            hour = Convert.ToInt32(bcdToString(data, offset, 2));
            minute = Convert.ToInt32(bcdToString(data, offset + 1, 2));
            return new System.DateTime(0000, 00, 00, hour, minute, 00);
        }

        /// <summary>
        /// BCD码转日期
        /// </summary>
        /// <param name="data"></param>
        /// <param name="offset">偏移位</param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static System.DateTime Bcd3ToDateTime(byte[] data, int offset)
        {
            Int32 year = default(Int32);
            Int32 month = default(Int32);
            Int32 day = default(Int32);
            year = Convert.ToInt32("20" + bcdToString(data, offset, 2));
            month = Convert.ToInt32(bcdToString(data, offset + 1, 2));
            day = Convert.ToInt32(bcdToString(data, offset + 2, 2));
            if (day == 0 || month == 0)
            {
                return DateTime.MinValue;
            }
            return new System.DateTime(year, month, day);
        }
        public static System.DateTime Bcd4ToDateTime(byte[] data, int offset)
        {
            Int32 year = default(Int32);
            Int32 month = default(Int32);
            Int32 day = default(Int32);
            try
            {
                year = Convert.ToInt32(bcdToString(data, offset, 4));
                month = Convert.ToInt32(bcdToString(data, offset + 2, 2));
                day = Convert.ToInt32(bcdToString(data, offset + 3, 2));
            }
            catch
            {
            }
            if (day == 0 || month == 0)
            {
                return DateTime.MinValue;
            }
            return new System.DateTime(year, month, day);
        }
        public static System.DateTime Bcd5ToDateTime(byte[] data, int offset)
        {
            var year = Convert.ToInt32("20" + bcdToString(data, offset, 2));
            var month = Convert.ToInt32(bcdToString(data, offset + 1, 2));
            var day = Convert.ToInt32(bcdToString(data, offset + 2, 2));
            var hour = Convert.ToInt32(bcdToString(data, offset + 3, 2));
            var minute = Convert.ToInt32(bcdToString(data, offset + 4, 2));
            if (day == 0 || month == 0)
            {
                return DateTime.MinValue;
            }
            return new DateTime(year, month, day, hour, minute, 0);
        }
        /// <summary>
        /// BCD码转时间格式
        /// </summary>
        /// <param name="data"></param>
        /// <param name="offset">偏移位</param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static System.DateTime Bcd6ToDateTime(byte[] data, int offset)
        {
            try
            {
                if (data.Length < offset + 6)
                {
                    return DateTime.MinValue;
                }
                int year;// = Convert.ToInt32("20" + bcdToString(data, offset, 2));
                if (!int.TryParse("20" + bcdToString(data, offset, 2), out year))
                {
                    return DateTime.MinValue;
                }
                int month;// = Convert.ToInt32(bcdToString(data, offset + 1, 2));
                if (!int.TryParse(bcdToString(data, offset + 1, 2), out month))
                {
                    return DateTime.MinValue;
                }
                int day;// = Convert.ToInt32(bcdToString(data, offset + 2, 2));
                if (!int.TryParse(bcdToString(data, offset + 2, 2), out day))
                {
                    return DateTime.MinValue;
                }
                int hour;// = Convert.ToInt32(bcdToString(data, offset + 3, 2));
                if (!int.TryParse(bcdToString(data, offset + 3, 2), out hour))
                {
                    return DateTime.MinValue;
                }
                int minute;// = Convert.ToInt32(bcdToString(data, offset + 4, 2));
                if (!int.TryParse(bcdToString(data, offset + 4, 2), out minute))
                {
                    return DateTime.MinValue;
                }
                int second;// = Convert.ToInt32(bcdToString(data, offset + 5, 2));
                if (!int.TryParse(bcdToString(data, offset + 5, 2), out second))
                {
                    return DateTime.MinValue;
                }
                if (day == 0 || month == 0 || month > 12 || day > 31 || hour > 24 || minute > 60 || second > 60)
                {
                    return DateTime.MinValue;
                }
                return new DateTime(year, month, day, hour, minute, second);
            }
            catch
            {
                return DateTime.MinValue;
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="data"></param>
        /// <param name="offset"></param>
        /// <returns></returns>
        public static System.DateTime Bcd7ToDateTime(byte[] data, int offset)
        {

            var year = Convert.ToInt32(bcdToString(data, offset, 4));
            var month = Convert.ToInt32(bcdToString(data, offset + 2, 2));
            var day = Convert.ToInt32(bcdToString(data, offset + 3, 2));
            var hour = Convert.ToInt32(bcdToString(data, offset + 4, 2));
            var minute = Convert.ToInt32(bcdToString(data, offset + 5, 2));
            var second = Convert.ToInt32(bcdToString(data, offset + 6, 2));
            if (day == 0 || month == 0 || year == 0)
            {
                return DateTime.MinValue;
            }
            return new DateTime(year, month, day, hour, minute, second);
        }
        /// <summary>
        /// 小时分钟转BCD码
        /// date:2012-12-18
        /// author:guozh
        /// </summary>
        /// <param name="dt"></param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static byte[] DateTimeToBcd2(System.DateTime dt)
        {
            List<byte> lst = new List<byte>();
            //StringBuilder sb = new StringBuilder();
            //sb.Append(dt.Hour.ToString("D2"));
            //sb.Append(dt.Minute.ToString("D2"));
            lst.AddRange(StrToBCD(dt.ToString("HHmm")));
            return lst.ToArray();
        }
        /// <summary>
        /// 日期转BCD码
        /// </summary>
        /// <param name="dt"></param>
        /// <returns></returns>
        /// <remarks></remarks>
        public static byte[] DateTimeToBcd3(System.DateTime dt)
        {

            List<byte> lst = new List<byte>();

            //StringBuilder sb = new StringBuilder();
            //sb.Append((dt.Year - 2000).ToString("D2"));
            //sb.Append(dt.Month.ToString("D2"));
            //sb.Append(dt.Day.ToString("D2"));
            lst.AddRange(StrToBCD(dt.ToString("yyMMdd")));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Year - 2000).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Month).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Day).ToString()), 0, 2));

            return lst.ToArray();
        }
        public static byte[] DateTimeToBcd4(System.DateTime dt)
        {
            List<byte> lst = new List<byte>();
            StringBuilder sb = new StringBuilder();
            sb.Append((dt.Year).ToString("D4"));
            sb.Append(dt.Month.ToString("D2"));
            sb.Append(dt.Day.ToString("D2"));
            lst.AddRange(StrToBCD(sb.ToString()));
            return lst.ToArray();
        }
        public static byte[] DateTimeToBcd5(System.DateTime dt)
        {
            List<byte> lst = new List<byte>();
            var strTime = dt.ToString("yyMMddHHmm");
            lst.AddRange(StrToBCD(strTime));

            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Year - 2000).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Month).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Day).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Hour).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Minute).ToString()), 0, 2));
            return lst.ToArray();
        }
        public static byte[] DateTimeToBcd6(System.DateTime dt)
        {
            try
            {
                List<byte> lst = new List<byte>();
                var strTime = dt.ToString("yyMMddHHmmss");
                lst.AddRange(StrToBCD(strTime));

                //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Year - 2000).ToString()), 0, 2));
                //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Month).ToString()), 0, 2));
                //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Day).ToString()), 0, 2));
                //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Hour).ToString()), 0, 2));
                //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Minute).ToString()), 0, 2));
                //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Second).ToString()), 0, 2));

                return lst.ToArray();
            }
            catch (System.Exception ex)
            {
                
                return new byte[] { 0, 0, 0, 0, 0, 0 };
            }
        }
        public static byte[] DateTimeToBcd7(System.DateTime dt)
        {
            List<byte> lst = new List<byte>();


            StringBuilder sb = new StringBuilder();
            sb.Append(dt.Year.ToString("D4"));
            sb.Append(dt.Month.ToString("D2"));
            sb.Append(dt.Day.ToString("D2"));
            sb.Append(dt.Hour.ToString("D2"));
            sb.Append(dt.Minute.ToString("D2"));
            sb.Append(dt.Second.ToString("D2"));
            lst.AddRange(StrToBCD(sb.ToString()));

            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes(dt.Year.ToString()), 0, 4));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Month).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Day).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Hour).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Minute).ToString()), 0, 2));
            //lst.AddRange(ByteArrayToBCD(System.Text.Encoding.ASCII.GetBytes((dt.Second).ToString()), 0, 2));

            return lst.ToArray();
        }
    }
}
```

参考文章：[http://cuisuqiang.iteye.com/blog/1429956](http://cuisuqiang.iteye.com/blog/1429956)



