# 趣味编程：静夜思（Swift版） - zwvista - 博客园

## [趣味编程：静夜思（Swift版）](https://www.cnblogs.com/zwvista/p/6911140.html)

```
func verticalWriting(txt:String, offset:Int) {
    Dictionary(grouping: txt.enumerated(), by: {$0.0 % offset})
        .sorted{$0.0 < $1.0}
        .forEach{print(
            $0.1.map{String($0.element)}.reversed().joined(separator: "|")
        )}
}

verticalWriting(txt: "床前明月光疑是地上霜举头望明月低头思故乡", offset: 5)

/*
低|举|疑|床
头|头|是|前
思|望|地|明
故|明|上|月
乡|月|霜|光
*/
```


