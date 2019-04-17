# 重拾C#日常积累：Json数据的序列化和反序列化 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 16:49:11[boonya](https://me.csdn.net/boonya)阅读数：131








Json数据的序列化和反序列化，在很多语言里面都会涉及到，尤其是对外提供的接口。

JSon工具包下载：[https://www.newtonsoft.com/json](https://www.newtonsoft.com/json)

##### 添加引用工具包


`using Newtonsoft.Json;`
##### 对象序列化为JSon string

官方示例：

```
Product product = new Product();
product.Name = "Apple";
product.Expiry = new DateTime(2008, 12, 28);
product.Sizes = new string[] { "Small" };

string json = JsonConvert.SerializeObject(product);
// {
//   "Name": "Apple",
//   "Expiry": "2008-12-28T00:00:00",
//   "Sizes": [
//     "Small"
//   ]
// }
```


个人示例：

```
// 将对象序列化为JSON数据
var videoListinfoJson = JsonConvert.SerializeObject(videoListinfo);
```

##### JSon string反序列化为对象

官方示例：

```
string json = @"{
  'Name': 'Bad Boys',
  'ReleaseDate': '1995-4-7T00:00:00',
  'Genres': [
    'Action',
    'Comedy'
  ]
}";

Movie m = JsonConvert.DeserializeObject<Movie>(json);

string name = m.Name;
// Bad Boys
```

个人示例：



```
// 处理Redis消息，封装消息发送>>终端上传音视频资源列表数据格式====指令代码===（0x1205）
VideoOrderAck videoOrderAck = JsonConvert.DeserializeObject<VideoOrderAck>(resultJson);
```

##### 数组JSON化

官方示例：

```
JArray array = new JArray();
array.Add("Manual text");
array.Add(new DateTime(2000, 5, 23));

JObject o = new JObject();
o["MyArray"] = array;

string json = o.ToString();
// {
//   "MyArray": [
//     "Manual text",
//     "2000-05-23T00:00:00"
//   ]
// }
LI
```




