# mapreduce中分割字符串 - 一个跳popping的quant的博客 - CSDN博客





2017年08月16日 21:10:18[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1261








刚学mapreduce没多久，在默认的输入分片中，经常要对一行的数据进行划分，如果数据的格式划分的清除，在分割这一行数据的时候也就好划分了，但是有时候，数据的格式并不规整，所以划分起来就得多写些代码了。 

例如：204001 01 02 03 07 10 25 07这种数据之间的间距是一样的都是一个空格的间距，所以在map中接受的数据可以直接通过value.split(” “)就可以获取这一行所有的数据了。 

如果数据的间隔不是一样的，例如：Apr 23 11:49:54 hostapd: wlan0: STA 14:7d:c5:9e:fb:84这些数据之间的间距不是一样的，如果通过value.split(” “)来划分
```
String line = "Apr  23    11:49:54  hostapd: wlan0: STA 14:7d:c5:9e:fb:84";
        String[] strs = line.split(" ");
        for(String str : strs){
           System.out.println(str);
        }
```

那么输出的结果就是

```
Apr

23



11:49:54

hostapd:
wlan0:
STA
14:7d:c5:9e:fb:84
```

那么在获取的String数组中取数据的时候就会得到一些空的字符串。而split中的参数还可以传递一个正则表达式，所以这里可以通过正则表达式来解决。

```
String line = "Apr  23    11:49:54  hostapd: wlan0: STA 14:7d:c5:9e:fb:84";
        String[] strs = line.split("\\s++");
        for(String str : strs){
           System.out.println(str);
        }
```

这里的\s是空格需要两个\来转移，++也就是多个的意思，这样就可以划分多个不同间隔的数据了。 

结果：

```
Apr
23
11:49:54
hostapd:
wlan0:
STA
14:7d:c5:9e:fb:84
```




