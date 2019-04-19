# sql里的in对应linq的写法 及 IQueryable转化为Dictionary - 左直拳的马桶_日用桶 - CSDN博客
2015年12月04日 09:17:15[左直拳](https://me.csdn.net/leftfist)阅读数：3587
```
string[] arID = { "0001", "0002" };
var dict = this.service.GetMyList(m => arID.Contains(m.ID))//等同于SQL里的 id in('0001','0002')
    .Select(m => new { m.ID, m.Name,m.Age })
    .ToDictionary(s => s.ID);//以ID作为key,{ID,Name,Age}作为Value，转化为Dictionary
foreach (var kvp in dict)
{
    string user = "姓名：" + kvp.Value.Name;
}
```
====================== 
有同事提示这样也可以： 
string str=”0001,0002” 
str.Contains(m.ID) 这样来也一样，亲
