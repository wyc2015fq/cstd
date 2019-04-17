# ASP.NET 或C# 中ASCII码含中文字符的编解码处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月23日 17:20:01[boonya](https://me.csdn.net/boonya)阅读数：1140








网上很多在线工具转出来的不能用，如“是是是”转出来的ASCII码是 :"\u662f\u662f\u662f”，如果是HTTP服务这个参数是有问题的因为包含特殊字符“\”。C#或ASP.NET中，如果是有中文字符，那么直接转是不行的，需要先将字符串转成16进制码才能转成想要的ASCII码，拿到的ASCII码字符串也可以进行解码了，不会再有中文乱码问题。

#### StringUtil工具类

```
using SQ.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Web;

namespace RTVSApi.Utils
{
    /// <summary>
    /// 字符串工具类
    /// </summary>
    public class StringUtil
    {
        /// <summary>
        /// 含中文字符串转ASCII
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        public static string Str2ASCII(String str)
        {
            try
            {
                //这里我们将采用2字节一个汉字的方法来取出汉字的16进制码
                byte[] textbuf = Encoding.Default.GetBytes(str);
                //用来存储转换过后的ASCII码
                string textAscii = string.Empty;

                for (int i = 0; i < textbuf.Length; i++)
                {
                    textAscii += textbuf[i].ToString("X");
                }
                return textAscii;
            }
            catch (Exception ex)
            {
                Log.WriteLog4("含中文字符串转ASCII异常" + ex.Message);
            }
            return "";
        }

        /// <summary>
        /// ASCII转含中文字符串
        /// </summary>
        /// <param name="textAscii">ASCII字符串</param>
        /// <returns></returns>
        public static string ASCII2Str(string textAscii)
        {
            try
            {
                int k = 0;//字节移动偏移量

                byte[] buffer = new byte[textAscii.Length / 2];//存储变量的字节

                for (int i = 0; i < textAscii.Length / 2; i++)
                {
                    //每两位合并成为一个字节
                    buffer[i] = byte.Parse(textAscii.Substring(k, 2), System.Globalization.NumberStyles.HexNumber);
                    k = k + 2;
                }
                //将字节转化成汉字
                return Encoding.Default.GetString(buffer);
            }
            catch (Exception ex)
            {
                Log.WriteLog4("ASCII转含中文字符串异常" + ex.Message);
            }
            return "";
        }
    }
}
```



获取到的ASCII字符串是：

"BBA65151373738312E312E312E312E37623531623234393738306434386363393433613130653339623062313361323534643833396366323264353434383739653838373064633538386339356462"。







![](https://img-blog.csdn.net/2018052317254281)

测试完美进行了转换。







