# go语言的循环语句for - 左直拳的马桶_日用桶 - CSDN博客
2018年11月29日 13:45:00[左直拳](https://me.csdn.net/leftfist)阅读数：239
一般来说，每种语言的循环语句都有两三种，什么for,foreach,while,do—until之类，应有尽有，总有一款适合您。但go是一款比较新的语言，语言的作者可能有一些自己的想法，循环语句就只提供了for一种。
```
suffixs := [6]string{".js",".css",".png",".jpg",".gif",".html"}
	for i := 0; i < len(suffixs); i++ {
		fmt.Println(suffixs[i])
	}
```
并且这个for是将foreach合而为一的。其他语言，像C#，foreach是很方便，但如果又想用到序号，就要在外面声明一个int i;然后循环体里i++之类，讨厌得很。但go版本的就挺好，可以将每轮迭代的序号和元素返回：
```
suffixs := [6]string{".js",".css",".png",".jpg",".gif",".html"}
	for i,sf := range suffixs{
		fmt.Printf("序号：%d，元素：%s\n\r",i,sf)	//注意%是写在前面的
	}
```
