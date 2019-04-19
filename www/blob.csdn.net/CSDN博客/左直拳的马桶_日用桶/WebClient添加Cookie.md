# WebClient添加Cookie - 左直拳的马桶_日用桶 - CSDN博客
2017年09月25日 09:57:07[左直拳](https://me.csdn.net/leftfist)阅读数：3624
网上通常说的是重载WebClient。看起来好复杂的样子，不明觉厉。
也可以这样，直接将cookie写入。
```
using (WebClient webClient = new WebClient())
{
    webClient.Encoding = Encoding.GetEncoding("utf-8");
    webClient.Headers.Add("Content-Type", "application/json");
    webClient.Headers.Add(HttpRequestHeader.Cookie, $@"{CookieName}={CookieValue}");//<------------------------------
    byte[] responseData = webClient.UploadData(url, "POST", postData);
    re = JsonConvert.DeserializeObject<JObject>(Encoding.UTF8.GetString(responseData));
}
```
