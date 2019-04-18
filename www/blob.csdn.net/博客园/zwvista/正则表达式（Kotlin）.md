# 正则表达式（Kotlin） - zwvista - 博客园

## [正则表达式（Kotlin）](https://www.cnblogs.com/zwvista/p/8846725.html)

### 课题
- 使用正则表达式匹配字符串

使用正则表达式 "\d{3}-(\d{4})-\d{2}" 匹配字符串 "123-4567-89"

返回匹配结果：’"123-4567-89" 以及 "4567"
- 使用正则表达式替换字符串（模式）

使用正则表达式 "(\d+)-(\d+)-(\d+)" 匹配字符串 "123-4567-89"

使用模式字符串 "$3-$1-$2" 替换匹配结果，返回结果 "89-123-4567"。
- 使用正则表达式替换字符串（回调）

使用正则表达式 "\d+" 匹配字符串 "123-4567-89"

将匹配结果即三个数字串全部翻转过来，返回结果 "321-7654-98"。
- 使用正则表达式分割字符串

使用正则表达式 "%(begin|next|end)%" 分割字符串"%begin%hello%next%world%end%"

返回正则表达式分隔符之间的两个字符串 "hello" 和 "world"。

### Kotlin

```
fun main(args: Array<String>) {
    val s = "123-4567-89"
    val r = Regex("\\d{3}-(\\d{4})-\\d{2}")
    val m = r.find(s)
    if (m != null)
        for (kv in m.groupValues.withIndex())
            println("group ${kv.index} : ${kv.value}")

    val r2 = Regex("(\\d+)-(\\d+)-(\\d+)")
    println(r2.replace(s, "\$3-\$1-\$2"))

    val r3 = Regex("\\d+")
    val s3 = r3.replace(s) {
        it.groupValues[0].reversed()
    }
    println(s3)

    val r4 = Regex("%(begin|next|end)%")
    val s4 = "%begin%hello%next%world%end%"
    println(r4.split(s4))
}

/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
[, hello, world, ]
*/
```


