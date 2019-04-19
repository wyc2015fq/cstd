# 两个喜欢的"新"C#语法 - 左直拳的马桶_日用桶 - CSDN博客
2016年08月19日 20:58:17[左直拳](https://me.csdn.net/leftfist)阅读数：1131
现在C#比较新的语法，我都十分喜欢。
比如属性可设默认值：
`public string Name { get; set; } = "张三";`
还有一个就是拼接字符串。
以往，通常都不会这样子写：
`string str = "abc：" + a.ToString() + "," + b.ToString() + "," + c.ToString();`
而是这样：
`string str = String.Format("abc:{0},{1},{2}",a,b,c);`
比第一种好很多，但就是不太直观，有时要数顺序才能看懂。
现在好了，可以这样：
`string str = $"abc:{a},{b},{c}";`
直接将变量写在里面，太直观了。
我们还可以和“@”符号结合起来一起用。众所周知，@符号，写在变量前，可以定义名字与系统保留字一样的变量，如
`string @public;//呵呵`
而@与字符串结合，可以直接在字符串里写转义符，例如
`string path = @"c:\temp\video\";`
