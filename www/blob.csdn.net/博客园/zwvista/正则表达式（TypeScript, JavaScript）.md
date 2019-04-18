# 正则表达式（TypeScript, JavaScript） - zwvista - 博客园

## [正则表达式（TypeScript, JavaScript）](https://www.cnblogs.com/zwvista/p/9011445.html)

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

### TypeScript

```
const s = '123-4567-89';
const r = /\d{3}-(\d{4})-\d{2}/g;
const m = r.exec(s);
m.forEach((value, index) => console.log(`group ${index} : ${value}`));

const r2 = /(\d+)-(\d+)-(\d+)/g;
console.log(s.replace(r2, '$3-$1-$2'));

const r3 = /\d+/g;
const s3 = s.replace(r3, substring => substring.split('').reverse().join(''));
console.log(s3);

const r4 = /%(?:begin|next|end)%/g;
const s4 = '%begin%hello%next%world%end%';
console.log(s4.split(r4).join(','));

/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
,hello,world,
*/
```

### JavaScript

```
var s = '123-4567-89';
var r = /\d{3}-(\d{4})-\d{2}/g;
var m = r.exec(s);
m.forEach(function (value, index) { return console.log("group " + index + " : " + value); });
var r2 = /(\d+)-(\d+)-(\d+)/g;
console.log(s.replace(r2, '$3-$1-$2'));
var r3 = /\d+/g;
var s3 = s.replace(r3, function (substring) { return substring.split('').reverse().join(''); });
console.log(s3);
var r4 = /%(?:begin|next|end)%/g;
var s4 = '%begin%hello%next%world%end%';
console.log(s4.split(r4).join(','));
/*
group 0 : 123-4567-89
group 1 : 4567
89-123-4567
321-7654-98
,hello,world,
*/
```


