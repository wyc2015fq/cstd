# 趣味编程：静夜思（Kotlin版） - zwvista - 博客园

## [趣味编程：静夜思（Kotlin版）](https://www.cnblogs.com/zwvista/p/6914627.html)

```
import java.util.*

fun verticalWriting(txt:String, offset:Int) =
    txt.mapIndexed { i, c -> Pair(i, c) }
        .groupByTo(TreeMap(), { it.first % offset }, { it.second.toString() })
        .forEach { println(it.value.reversed().joinToString("|")) }

fun main(args: Array<String>) =
    verticalWriting("床前明月光疑是地上霜举头望明月低头思故乡", 5)

/*
低|举|疑|床
头|头|是|前
思|望|地|明
故|明|上|月
乡|月|霜|光
*/
```


