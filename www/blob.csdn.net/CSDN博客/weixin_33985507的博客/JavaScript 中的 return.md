# JavaScript 中的 return - weixin_33985507的博客 - CSDN博客
2018年11月15日 20:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**Return**
```
[1, 3, 4, 'abc'].map((item: number, index: number) => {
    // 定义标识, 用以判断是否存在符合条件项
    let _flag: boolean = false;
    ['a', 'b', 'abc'].map((value: number, i: numebr) => {
        if (value === item) {
            _flag = true;
        }
    });
    if (_flag) {
        console.log('对象存在重复，请检查');
        return;
    }    
});
console.log('不存在重复项, 继续往下执行');
```
上面所写代码的原目的是遍历两个数组对象进行对比，若存在某一项重复，则停止后续代码程序的运行。
但上述代码无论是否存在重复项，后续的程序都会运行下去。
原因：[`return`语句终止函数的执行,并返回一个指定的值给函数调用](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Statements/return)
使用 return 跳出的只是当前的函数体，当 return 所处的函数体处于其他的函数体内部的时候，使用 return 语句只是会终止当前函数体的执行，而不会终止外层函数的执行。
改写上面错误的代码：
```
// 定义标识, 用以判断是否存在符合条件项
let _flag: boolean = false;
[1, 3, 4, 'abc'].map((item: number, index: number) => {
    ['a', 'b', 'abc'].map((value: number, i: numebr) => {
        if (value === item) {
            _flag = true;
        }
    });    
});
if (_flag) {
    console.log('对象存在重复，请检查');
    return;
}
console.log('不存在重复项, 继续往下执行');
```
