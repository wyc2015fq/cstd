# 【ES6】字符串的常用方法 - weixin_33985507的博客 - CSDN博客
2018年12月05日 09:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### includes(), startsWith(), endsWith()
ES6 提供了三种新方法来确定一个字符串是否包含在另一个字符串中：
- `includes()`：返回布尔值，表示是否字符串字符串。
- `startsWith()`：返回布尔值，表示是否包含字符串开头。
- `endsWith()`：返回布尔值，表示是否包含字符串结尾。
```
let s = 'Hello world!';
s.startsWith('Hello') // true
s.endsWith('!') // true
s.includes('o') // true
```
`includes()`比`indexOf()`的优势在于，`indexOf()`的返回结果不是布尔值，须与-1对比，不够直观。
这三个方法都支持第二个参数，表示**开始搜索的位置**。
```
let s = 'Hello world!';
s.startsWith('world', 6) // true
s.endsWith('Hello', 5) // true
s.includes('Hello', 6) // false
```
使用第二个参数`n`时，`endsWith`的行为与其他两个方法有所不同。它针对前`n`个字符，而其他两个方法针对从第`n`个位置直到字符串结束。
### repeat()
`repeat`方法返回一个新字符串，表示将原字符串重复`n`次。
```
'x'.repeat(3)  // "xxx"
'hello'.repeat(2)  // "hellohello"
'na'.repeat(0)  // ""
```
- 如果参数是正小数，会取整。
- 如果参数是负数，会报错。
- 如果参数是0，会得到空串。
- 如果参数是字符串、布尔型或其他类型，会先换算成整数。
### padStart()，padEnd()
ES2017 引入了**字符串补全长度**的功能。
- `padStart()`用于头部补全;`padEnd()`用于尾部补全。
- `padStart`和`padEnd`一共接受两个参数，第一个参数是**字符串补全生效的最大长度**，第二个参数是**用来补全的字符串**。
```
'x'.padStart(5, 'ab') // 'ababx'
'x'.padStart(4, 'ab') // 'abax'
'x'.padEnd(5, 'ab') // 'xabab'
'x'.padEnd(4, 'ab') // 'xaba'
```
- 如果原字符串的长度，等于或大于最大长度，则字符串补全不生效，返回原字符串。
```
'xxx'.padStart(2, 'ab') // 'xxx'
'xxx'.padEnd(2, 'ab') // 'xxx'
```
- 如果用来补全的字符串与原字符串，两者的长度之和超过了最大长度，则会截去超出位数的补全字符串。
```
'abc'.padStart(10, '0123456789')
// '0123456abc'
```
- 如果省略第二个参数，默认使用空格补全长度。
```
'x'.padStart(4) // '   x'
'x'.padEnd(4) // 'x   '
```
- **`padStart()`的常见用途**
```
// 为数值补全指定位数
'1'.padStart(10, '0') // "0000000001"
'12'.padStart(10, '0') // "0000000012"
'123456'.padStart(10, '0') // "0000123456"
// 提示字符串格式
'12'.padStart(10, 'YYYY-MM-DD') // "YYYY-MM-12"
'09-12'.padStart(10, 'YYYY-MM-DD') // "YYYY-09-12"
```
### 模板字符串
- 模板字符串（template string）是增强版的字符串，用反引号（`）标识。
- 可以当作普通字符串使用，也可以用来定义多行字符串，或者在字符串中嵌入变量。
- 在字符串中嵌入变量需要使用`${变量名}`的形式，`$`前面可以紧贴其他字符，`$`也可以加`\`转义。
```
// 普通字符串
`In JavaScript '\n' is a line-feed.`
// 多行字符串
`In JavaScript this is
 not legal.`
console.log(`string text line 1
string text line 2`);
// 字符串中嵌入变量
let name = "Bob", time = "today";
`Hello ${name}, how are you ${time}?`
```
- 
`${}`里面可以放任意表达式，可以理解为，先计算这个表达式，无论它返回什么，无论它是什么类型，都转换为字符串，然后再跟前、后字符串相连。
- 
`${}`里面还可以放函数调用，以返回值作为自己的值。
