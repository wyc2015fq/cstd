# 正则表达式（Swift） - zwvista - 博客园

## [正则表达式（Swift）](https://www.cnblogs.com/zwvista/p/8324371.html)

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

### Swift

```
import Foundation

let s = "123-4567-89"
let r = try NSRegularExpression(pattern: "\\d{3}-(\\d{4})-\\d{2}")
let results = r.matches(in: s, options: [], range: NSRange(s.startIndex..., in: s))
results.flatMap { m in (0..<m.numberOfRanges).map {
    String(s[Range(m.range(at: $0), in: s)!])
}}.enumerated().forEach{print("group \($0) : \($1)")}

let r2 = try NSRegularExpression(pattern: "(\\d+)-(\\d+)-(\\d+)")
let s2 = r2.stringByReplacingMatches(in: s, options: [], range: NSRange(s.startIndex..., in: s), withTemplate: "$3-$1-$2")
print(s2)

let r3 = try NSRegularExpression(pattern: "\\d+")
let results2 = r3.matches(in: s, options: [], range: NSRange(s.startIndex..., in: s))
var s3 = s
for i in (0..<results2.count).reversed() {
    let rng = Range(results2[i].range, in: s3)!
    s3.replaceSubrange(rng, with: s3[rng].reversed())
}
print(s3)

// https://stackoverflow.com/questions/25818197/how-to-split-a-string-in-swift
extension String {
    
    func split(regex pattern: String) -> [String] {
        
        guard let re = try? NSRegularExpression(pattern: pattern, options: [])
            else { return [] }
        
        let nsString = self as NSString // needed for range compatibility
        let stop = "<SomeStringThatYouDoNotExpectToOccurInSelf>"
        let modifiedString = re.stringByReplacingMatches(
            in: self,
            options: [],
            range: NSRange(location: 0, length: nsString.length),
            withTemplate: stop)
        return modifiedString.components(separatedBy: stop)
    }
}

let r4 = "%(begin|next|end)%"
let s4 = "%begin%hello%next%world%end%"
print(s4.split(regex: r4))

/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
["", "hello", "world", ""]
*/
```

### 使用第三方库

使用标准库解决正则表达式问题过于繁琐

下面使用第三方库 CrossroadRegex
[CrossroadRegex - Advanced regular expressions for Swift](https://github.com/crossroadlabs/Regex)

```
import Foundation

let s = "123-4567-89"
let r = "\\d{3}-(\\d{4})-\\d{2}".r!
r.findAll(in: s).forEach { m in
    for i in 0..<m.ranges.count {
        print("group \(i) : \(m.group(at: i)!)")
    }
}

let r2 = "(\\d+)-(\\d+)-(\\d+)".r!
let s2 = r2.replaceAll(in: s, with: "$3-$1-$2")
print(s2)

let r3 = "\\d+".r!
let s3 = r3.replaceAll(in: s, using: { String($0.matched.reversed()) })
print(s3)

let r4 = "%(?:begin|next|end)%".r!
let s4 = "%begin%hello%next%world%end%"
print(r4.split(s4))

/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
["", "hello", "world", ""]
*/
```


