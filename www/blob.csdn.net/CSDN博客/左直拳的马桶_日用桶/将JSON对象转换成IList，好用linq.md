# 将JSON对象转换成IList，好用linq - 左直拳的马桶_日用桶 - CSDN博客
2015年01月24日 17:36:53[左直拳](https://me.csdn.net/leftfist)阅读数：2506
JObject
JToken
JProperty
IList<>
搞得头都大了，记而备忘：
```
JObject json = .....
JToken[] jps = json["records"][0].ToArray();
List<ItemInfo> cols = json["columns"].ToObject<List<ItemInfo>>();
int len = jps.Length;
int i = 0;
int limit = 2;
StringBuilder sb = new StringBuilder("");
sb.Append(@"<div><table><tr>");
for (int j = 1; j < len; j++)
{
    JProperty jp = jps[j] as JProperty;
    string displayname = cols.Where(m => (m.Name.CompareTo(jp.Name) == 0)).First().DisplayName;
    sb.Append(String.Format(@"<td>{0}:</td><td><input type=""text"" value=""{1}"" name=""{2}""/></td>"
        , displayname
        , jp.Value
        , jp.Name));
    i++;
    if (i % limit == 0)
    {
        sb.Append("</tr><tr>");
    }
}
if (i % limit != 0)
{
    for (i = i % limit; i < limit; i++)
    {
        sb.Append("<td> </td>");
    }
    sb.Append("</tr>");
}
else
{
    sb.Remove(sb.Length - 4, 4);
}
sb.Append("</table></div>");
```
