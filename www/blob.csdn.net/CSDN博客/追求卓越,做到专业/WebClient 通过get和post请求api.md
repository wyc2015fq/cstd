
# WebClient 通过get和post请求api - 追求卓越,做到专业 - CSDN博客


2017年11月01日 17:37:04[Waldenz](https://me.csdn.net/enter89)阅读数：1486


//get 请求
string url = string.Format("http://localhost:28450/api/values?str1=a&str2=b");
WebClient wc = new WebClient();
Encoding enc = Encoding.GetEncoding("UTF-8");
Byte[] pageData = wc.DownloadData(url);
string re = enc.GetString(pageData);
//post 请求
string postData = "value=a";
byte[] bytes = Encoding.UTF8.GetBytes(postData);
WebClient client = new WebClient();
client.Headers.Add("Content-Type", "application/x-www-form-urlencoded");
client.Headers.Add("ContentLength", postData.Length.ToString());
Encoding enc = Encoding.GetEncoding("UTF-8");
byte[] responseData = client.UploadData("http://localhost:28450/api/values", "POST", bytes);
string re = enc.GetString(responseData);

