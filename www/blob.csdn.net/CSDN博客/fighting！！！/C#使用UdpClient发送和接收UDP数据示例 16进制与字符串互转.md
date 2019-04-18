# C#使用UdpClient发送和接收UDP数据示例 16进制与字符串互转 - fighting！！！ - CSDN博客
2017年10月29日 21:30:04[dujiahei](https://me.csdn.net/dujiahei)阅读数：987
C#使用UdpClient发送和接收UDP数据示例 16进制与字符串互转 - CSDN博客  http://blog.csdn.net/xuehuic/article/details/53812647
1.服务器
[csharp][view
 plain](http://blog.csdn.net/xuehuic/article/details/53812647#)[copy](http://blog.csdn.net/xuehuic/article/details/53812647#)
- using System;  
- using System.Net;  
- using System.Net.Sockets;  
- using System.Text;  
- 
- namespace ConsoleApplication1  
- {  
- class Program  
-     {  
- staticvoid Main(string[] args)  
-         {  
-             Server(10086); //监听10086端口
-         }  
- 
- /// <summary>
- /// 服务器端
- /// </summary>
- /// <param name="port"></param>
- staticvoid Server(int port)  
-         {  
- try
-             {  
-                 UdpClient udpclient = new UdpClient(port);  
- while (true)  
-                 {                      
-                     IPEndPoint ipendpoint = null;  
- byte[] bytes = udpclient.Receive(ref ipendpoint); //停在这等待数据
- string data = Encoding.Default.GetString(bytes, 0, bytes.Length);                      
- 
-                     Console.WriteLine("{0:HH:mm:ss}->接收数据(from {1}:{2})：{3}", DateTime.Now, ipendpoint.Address, ipendpoint.Port, data);  
-                 }  
-                 udpclient.Close();  
-             }  
- catch (Exception ex)  
-             {  
-                 Console.WriteLine("{0:HH:mm:ss}->{1}", DateTime.Now, ex.Message);  
-             }  
-             Console.ReadKey();  
-         }  
-     }  
- }  
2.客户端
[csharp][view
 plain](http://blog.csdn.net/xuehuic/article/details/53812647#)[copy](http://blog.csdn.net/xuehuic/article/details/53812647#)
- using System;  
- using System.Net;  
- using System.Net.Sockets;  
- using System.Text;  
- 
- namespace ConsoleApplication1  
- {  
- class Program  
-     {  
- staticvoid Main(string[] args)  
-         {  
-             Client("127.0.0.1", 10086, "play"); //发送数据
-         }  
- 
- /// <summary>
- /// 客户端
- /// </summary>
- /// <param name="ip"></param>
- /// <param name="port"></param>
- /// <param name="message"></param>
- staticvoid Client(string ip, int port, string message)  
-         {  
- try
-             {  
-                 UdpClient udpclient = new UdpClient();  
-                 IPEndPoint ipendpoint = new IPEndPoint(IPAddress.Parse(ip), port);  
- byte[] data = Encoding.Default.GetBytes(message);  
-                 udpclient.Send(data, data.Length, ipendpoint);  
-                 udpclient.Close();  
- 
-                 Console.WriteLine("{0:HH:mm:ss}->发送数据(to {1})：{2}", DateTime.Now, ip, message);  
-             }  
- catch (Exception ex)  
-             {  
-                 Console.WriteLine("{0:HH:mm:ss}->{1}", DateTime.Now, ex.Message);  
-             }  
-             Console.ReadKey();  
-         }  
-     }  
- }  
3. 字符串转16进制，然后直接发送16进制。
[csharp][view
 plain](http://blog.csdn.net/xuehuic/article/details/53812647#)[copy](http://blog.csdn.net/xuehuic/article/details/53812647#)
- publicstaticbyte[] HexStringToByteArray(string s)  
-         {  
- if (s.Length = 0)  
- thrownew Exception("将16进制字符串转换成字节数组时出错，错误信息：被转换的字符串长度为0。");  
-             s = s.Replace(" ", "");  
- byte[] buffer = newbyte[s.Length / 2];  
- for (int i = 0; i < s.Length; i += 2)  
-                 buffer[i / 2] = Convert.ToByte(s.Substring(i, 2), 16);  
- return buffer;  
-         }  
4.16进制转字符串，接收端用于识别显示
[csharp][view
 plain](http://blog.csdn.net/xuehuic/article/details/53812647#)[copy](http://blog.csdn.net/xuehuic/article/details/53812647#)
- /// <summary>
- /// 字节数组转换成十六进制字符串(不带空格)
- /// </summary>
- /// <param name="data">要转换的字节数组</param>
- /// <returns>转换后的字符串</returns>
- publicstaticstring ByteArrayToHexStringNoBlank(byte[] data)  
- {  
-     StringBuilder sb = new StringBuilder(data.Length * 3);  
- foreach (byte b in data)  
-         sb.Append(Convert.ToString(b, 16).PadLeft(2, '0'));  
- return sb.ToString().ToUpper();  
- }  
