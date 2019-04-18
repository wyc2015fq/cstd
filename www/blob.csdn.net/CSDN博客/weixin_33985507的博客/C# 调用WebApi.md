# C# 调用WebApi - weixin_33985507的博客 - CSDN博客
2018年01月18日 14:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：73
## 1.WebRequest方式
**Post：**
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
private void button1_Click(object sender, EventArgs e)
        {
           string ss= HttpPost("http://localhost:41558/api/Demo/PostXXX", "{Code:\"test089\",Name:\"test1\"}");
        }
        public static string HttpPost(string url, string body)
        {
            //ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback(CheckValidationResult);
            Encoding encoding = Encoding.UTF8;
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Method = "POST";
            request.Accept = "text/html, application/xhtml+xml, */*";
            request.ContentType = "application/json";
       
            byte[] buffer = encoding.GetBytes(body);
            request.ContentLength = buffer.Length;
            request.GetRequestStream().Write(buffer, 0, buffer.Length);
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8))
            {
                return reader.ReadToEnd();
            }
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
**Get：**
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 private void button1_Click(object sender, EventArgs e)
        {
            string ss = HttpGet("http://localhost:41558/api/Demo/GetXXX?Name=北京");
        }
        public static string HttpGet(string url)
        {
            //ServicePointManager.ServerCertificateValidationCallback = new RemoteCertificateValidationCallback(CheckValidationResult);
            Encoding encoding = Encoding.UTF8;
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            request.Method = "GET";
            request.Accept = "text/html, application/xhtml+xml, */*";
            request.ContentType = "application/json";
           
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8))
            {
                return reader.ReadToEnd();
            }
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
## 2.HttpClient 方式
**Post:**
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
private async void button2_Click(object sender, EventArgs e)
{
     HttpClient client = new HttpClient();
     //由HttpClient发出Delete Method
     HttpResponseMessage response = await client.DeleteAsync("http://localhost:41558/api/Demo"+"/1");
     if (response.IsSuccessStatusCode)
         MessageBox.Show("成功");
}
private async void button3_Click(object sender, EventArgs e)
{
     //创建一个处理序列化的DataContractJsonSerializer
     DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(People));
     MemoryStream ms = new MemoryStream();
     //将资料写入MemoryStream
     serializer.WriteObject(ms, new People() { Id = 1, Name = "Hello ni" });
     //一定要在这设定Position
     ms.Position = 0;
     HttpContent content = new StreamContent(ms);//将MemoryStream转成HttpContent
     content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("application/json");
     HttpClient client = new HttpClient();
     //由HttpClient发出Put Method
     HttpResponseMessage response = await client.PutAsync("http://localhost:41558/api/Demo"+ "/1", content);
     if (response.IsSuccessStatusCode)
         MessageBox.Show("成功");
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
**Get:**
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
using (WebClient client = new WebClient())
{
     client.Headers["Type"] = "GET";
     client.Headers["Accept"] = "application/json";
     client.Encoding = Encoding.UTF8;
     client.DownloadStringCompleted += (senderobj, es) =>
     {
         var obj = es.Result;
     };
     client.DownloadStringAsync("http://localhost:41558/api/Demo");
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
