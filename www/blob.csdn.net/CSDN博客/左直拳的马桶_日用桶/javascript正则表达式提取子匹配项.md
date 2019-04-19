# javascript正则表达式提取子匹配项 - 左直拳的马桶_日用桶 - CSDN博客
2017年03月21日 11:26:21[左直拳](https://me.csdn.net/leftfist)阅读数：8944
C#里所用的正则表达式，如果要提取字符串里的子匹配项（我都不知道那个叫啥名字，别名？）是很方便的，比如：
```
Regex rx = new Regex(@"<title>(?<title>[\s\S]+)?</title>", RegexOptions.Compiled | RegexOptions.IgnoreCase);
Match m = rx.Match(content);
if (m.Success)
{
    string title = m.Result("${title}");
}
```
但如果是javascript呢，咋整？两边语法好像稍有点不一样。我弄了下，只有一些支离破碎的经验值，先记录下来：
```java
str = "<html><head><title>呵呵呵</title></head><body></body></html>";
reg = /<title>([\s\S]+)<\/title>/gi
var result;
if ((result = reg.exec(str)) != null) {
    alert(result);//显示“<title>呵呵呵</title>”，“呵呵呵”
    alert(result[1]);//显示“呵呵呵”
    //
}
```
参考文章： 
[http://www.html-js.com/article/3420](http://www.html-js.com/article/3420)
