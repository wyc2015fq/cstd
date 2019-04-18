# BugFix: 编码错误 - 知乎
# 

首先是外部编码错误
![](https://pic2.zhimg.com/v2-1b4150b9e24f166aade00ffdd3cb5bc1_b.jpg)
然后还有内部编码错误
![](https://pic3.zhimg.com/v2-1edac2a3ab7a3c8f1a85239b0e71c9b2_b.jpg)
问题复现: 

`Import["https://api.ctext.org/gettext?urn=ctp:dao-de-jing", "RawJSON"]`

67行168个字节处出现错误, 我们定位一下发现了:

**知此兩者亦𥡴式。常知𥡴式，是謂玄德。**

一看就是𥡴这个字有问题, 这个字可以显示但是却不能导入
![](https://pic1.zhimg.com/v2-b11c60967d7bb693b1b3666b0197e1c4_b.jpg)
可以看到这个字是扩展 B 区的, 
![](https://pic4.zhimg.com/v2-4f0a60fdd0790be59a399c4eb94fa4bb_b.jpg)
我们可以用 .Net 或者 Java 的成熟类库来搞定这个, 一般 win10 肯定预装了 .net, 我用 .net 作为演示, 调用java 也是一样的  

```
<<NETLink`
InstallNET[]
LoadNETType["System.Text.Encoding"]
```

然后看一下有哪些 Class
![](https://pic1.zhimg.com/v2-44a4ae70d89122a6e99a9fcc8e330ab0_b.jpg)
然后用 `NETTypeInfo["System.Text.Encoding"]` 看一下接口

有个叫 GetString 的方法, 我们可以看到它的接口声明是:

`virtual string GetString (byte[] bytes)`

我们需要的是 UTF8 Class, 所以就 New 一个, 然后调用 GetString.

```
reader = Encoding`UTF8;
json = ImportString[FromCharacterCode@ToCharacterCode[#, "UTF-8"], "RawJSON"]&;
askT[url_String] := Block[
	{link = "https://api.ctext.org/gettext?if=zh&urn=" <> url},
	ask = json@reader@GetString[URLRead[link, "BodyBytes"]];
	If[!ListQ@ask["fulltext"], Pause@RandomReal[, ];askT[url], ask]
];
```

然后用

`askT["ctp:dao-de-jing"]["fulltext"] // TableForm`

就可以成功的导入数据了.



