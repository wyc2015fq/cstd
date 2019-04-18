# 你试过不用if撸代码吗？ - 算法与数学之美 - CSDN博客
2018年04月30日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：98
> 
原文: Coding Tip: Try to Code Without If-statements
译者: Fundebug
试着不用if撸代码，是件很有趣的事，而且，万一你领会了什么是“数据即代码，代码即数据”呢？
我在教新手编程时，喜欢给他们一些小小的挑战，比如：不使用if语句(或者三元运算符、switch语句等)解决一些编程问题。
这样做有什么意义吗？
事实上，它可以迫使你从不同的角度寻找解决方法，也许可以找到更好的方法。
当然，使用if语句没有任何不对的地方。但是，不使用if的话，有时候可以增加代码的可读性。这一点并不是绝对的，如果完全不使用if语句的话，代码可读性也许会更差。这需要你根据不同情况去判断。
而且，不用if的话不只是影响可读性。在这背后隐含着更加深刻的道理。通过了解本文的代码示例，你可以发现，如果不使用if语句的话，你的代码会更加接近代码即数据的概念。
另外，当你尝试不使用if语句去编程时，也是一件非常有意思的事情。
### 示例1: 统计数组中的奇数
假设我们有一个整数数组**arrayOfIntegers**，现在需要统计其中奇数的个数：
const arrayOfIntegers = [1, 4, 5, 9, 0, -1, 5];
#### 使用if
let counter = 0;
arrayOfIntegers.forEach((integer) => {
const remainder = Math.abs(integer % 2);
if (remainder === 1) {
counter++;
}
});
console.log(counter);
#### 不用if
let counter = 0;
arrayOfIntegers.forEach((integer) => {
const remainder = Math.abs(integer % 2);
counter += remainder;
});
console.log(counter);
不用if时，我们巧妙地利用了奇数与偶数的特性，它们除以2的余数分别是0和1。
### 示例2： 判断工作日和周末
给定一个日期(比如**new Date()**)，判断它是工作日还是周末，分别返回”weekend”和”weekday”。
#### 使用if
const weekendOrWeekday = (inputDate) => {
const day = inputDate.getDay();
if (day === 0 || day === 6) {
return'weekend';
}
return'weekday';
// Or, for ternary fans:
// return (day === 0 || day === 6) ? 'weekend' : 'weekday';
};
console.log(weekendOrWeekday(newDate()));
#### 不用if
const weekendOrWeekday = (inputDate) => {
const day = inputDate.getDay();
return weekendOrWeekday.labels[day] ||
weekendOrWeekday.labels['default'];
};
weekendOrWeekday.labels = {
0: 'weekend',
6: 'weekend',
default: 'weekday'
};
console.log(weekendOrWeekday(newDate()));
你是否发现if语句中其实隐含着一些信息呢？它告诉我们哪一天是周末，哪一天是工作日。因此，要去掉if语句的话，我们只需要把这些信息写入**weekendOrWeekday.labels**对象，然后直接使用它就好了。
### 示例3: doubler函数
写一个doubler函数，它会根据参数的类型，进行不同的操作：
- 
如果参数是数字，则乘以2(i.e. `5 => 10`, `-10 => -20`)；
- 
如果参数是字符串，则每个字符重复2次 (i.e. `'hello' => 'hheelloo'`)；
- 
如果参数是函数，则调用2次；
- 
如果参数是数组，则将每一个元素作为参数，调用doubler函数
- 
如果参数是对象，则将每个属性值作为参数，调用doubler函数
#### 使用switch
const doubler = (input) => {
switch (typeof input) {
case'number':
return input + input;
case'string':
return input
.split('')
.map((letter) => letter + letter)
.join('');
case'object':
Object.keys(input)
.map((key) => (input[key] = doubler(input[key])));
return input;
case'function':
input();
input();
}
};
console.log(doubler(-10));
console.log(doubler('hey'));
console.log(doubler([5, 'hello']));
console.log(doubler({ a: 5, b: 'hello' }));
console.log(
doubler(function() {
console.log('call-me');
}),
);
#### 不用switch
const doubler = (input) => {
return doubler.operationsByType[typeof input](input);
};
doubler.operationsByType = {
number: (input) => input + input,
string: (input) =>
input
.split('')
.map((letter) => letter + letter)
.join(''),
function: (input) => {
input();
input();
},
object: (input) => {
Object.keys(input)
.map((key) => (input[key] = doubler(input[key])));
return input;
},
};
console.log(doubler(-10));
console.log(doubler('hey'));
console.log(doubler([5, 'hello']));
console.log(doubler({ a: 5, b: 'hello' }));
console.log(
doubler(function() {
console.log('call-me');
}),
);
可知，我将每一种参数类型对应的操作绑定到了doubler.operationsByType，这样不需要switch语句，就可以实现doubler函数了。
∑编辑 | Gemini
*来源 | 校苑数模*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkz1SRWmm2kJgtV2NTQtdSgtyl7nJbJm8xS78Td2LBbJQKKqyE54oaOO9upMribZagMKYJVBaEDyKtA/640?wx_fmt=jpeg)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域
经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
