# 如何遍历newtonsoft.json的JObject里的JSON数据 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月19日 10:14:18[左直拳](https://me.csdn.net/leftfist)阅读数：35670
这种问题，在网上搜，居然没有答案，又是一堆垃圾，连谷歌上都搜不到。老实说，我喜欢这边的工作环境，可以上谷歌，毕竟是大公司，有自己的VPN 。某组织整天禁这个禁那个，去年居然连谷歌都禁了，丧心病狂至此，让人无语。这件事，其意义与大清禁海可有一比，可以预见，中国的科技水平会越来越与主流文明脱节，其恶劣影响必将贻害万年。
还是经过自己摸索，有代码有J8：
```
JObject json = //获得一个JObject对象
/*
{
  "records": [
    {
      "ID": "ABC",
      "OperationPatchID": "1",
      "s05055396": "120.93900",
      "s05055099": "14.44790",
      "s04026131": "Open lot",
      "s04020195": "3334",
      "s03006029": "2014/7/23 0:00:00",
      "s03006045": "BAC"
    }
  ],
  "count": 1
}   Newtonsoft.Json.Linq.JObject
*/
StringBuilder sb = new StringBuilder("<div>");
JToken record = json["records"][0];
foreach(JProperty jp in record)
{
    sb.Append(String.Format(@"<span>{0}</span>：<span>{1}</span>"
        ,jp.Name
        ,jp.Value));
}
sb.Append("</div>");
```
