# JS专题之深浅拷贝 - weixin_33985507的博客 - CSDN博客
2019年02月04日 15:31:33[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
#### 前言
在开发过程中，偶尔会遇到这种场景，拿到一个数据后，你打算对它进行处理，但是你又希望拷贝一份副本出来，方便数据对比和以后恢复数据。
那么这就涉及到了 JS 中对数据的深浅拷贝问题，所谓深浅拷贝，浅拷贝的意思就是，你只是复制了对象数据的引用，并没有把内存里的值另外复制一份，那么深拷贝就是把值完整地复制一份新的值。
在之前的文章《JS专题之数据类型和类型检测》中我有讲过，JS 中的数据类型分为两种，基本数据类型和引用数据类型，基本数据类型是保存在栈的数据结构中的,是按值访问，所以不存在深浅拷贝问题。
而比如对象，数组，函数，正则，时间对象这些都是引用数据类型，是保存在堆中的。所以，引用数据类型的复制，是内存地址的传递，并没有拷贝出一份新的数据。
那么深拷贝，浅拷贝的区别是什么呢？先给结论：
> 操作拷贝之后的数据不会影响到原数据的**值拷贝**，就是深拷贝，反正，有影响则为浅拷贝。
#### 一、应用场景
日常开发中，JS 拷贝大多会在 **数据保存，数据比对，数据同步** 时出现，所以，当你在这些场景的时候，要记得里面隐藏有一个数据深浅拷贝的问题。
#### 二、浅拷贝
我们来看一下浅拷贝：
```java
function clone(origin) {
    var result = {};
        for (var prop in origin) {
            if (origin.hasOwnProperty(prop)) {
                result[prop] = origin[prop];
            }
        }
        return result;
}
var jay = {
    name: "jayChou",
    age: 40,
    family: {
        wife: "Quinlivan"
    }
}
var otherJay = clone(jay);
otherJay.age = 18;
otherJay.family.wife = "otherGirl";
console.log(jay); 
// 
// {
//   name: "jayChou",
//   age: 40,  // 没被改变
//   family: {
//     wife: "otherGirl"  // 同时被改变，说明是同一个引用
//   }
// }
console.log(otherJay);
// 
// {
//   name: "jayChou",
//   age: 18,
//   family: {
//     wife: "otherGirl"  // 被改变了
//   }
// }
```
我们发现，首先，**浅拷贝不是直接赋值**，浅拷贝新建了一个对象，然后将源对象的属性都一一复制过来,复制的是值，而不是引用。
我们知道，对象都是按地址引用进行访问的，浅拷贝的复制只复制了第一层的属性，并没有递归将所有的值复制过来，所以，操作拷贝数据，对原数据产生了影响，故而为浅拷贝。
进而，那些可以直接返回原数组的方法就可以简单实现数组和对象浅拷贝。
```java
// 1、 数组浅拷贝 - slice
function shallowCopy1(origin) {
    return origin.slice();
}
// 2、 数组浅拷贝 - concat
function shallowCopy2(origin){
    return origin.concat();
}
// 3、 数组浅拷贝 - 遍历
function shallowCopy3(origin){
  var result = [];
  for(var i = 0; i < origin.length; i++) {
    result.push(origin[i]);
  }
  return result;
}
// 4、 对象浅拷贝 - Object.assign
function shallowCopy4(origin) {
    return Object.assign({},origin)
}
// 5、 对象浅拷贝 - 扩展运算符
// 扩展运算符（...）用于取出参数对象的所有可遍历属性，拷贝到当前对象之中
function shallowCopy5(origin) {
    return {
        ...origin
    }
}
```
[Object.assign](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Object/assign) 的拷贝，假如源对象的属性值是一个指向对象的引用，它也只拷贝那个引用值。MDN 有相应的实例和解释。
#### 二、深拷贝
> 深拷贝就完整复制数据的值（而非引用），目的在于避免拷贝后数据对原数据产生影响。
目前深拷贝的实现方法主要有递归复制，JSON 正反序列化:
```java
// 1. 深拷贝 - JSON 正反序列化
// 缺点就是无法拷贝 undefined、function、symbol 这类特殊的属性值。
function deepClone1(origin) {
    return JSON.parse(JSON.stringify(arr));
}
// 2. 深拷贝 - 递归;
function deepClone2(origin) {
  const result = origin.constructor === Array ? [] : {};
  for (let keys in origin) {
    // 不遍历原型链上的属性
    if (origin.hasOwnProperty(keys)) {
      if (origin[keys] && typeof origin[keys] === "object") {
        // 如果值是对象，就递归一下, 区分是一般对象还是数组对象
        result[keys] = origin[keys].constructor === Array ? [] : {};
        // 如果是引用数据类型，会递归调用
        result[keys] = deepClone(origin[keys]);
      } else {
        // 如果不是，就直接赋值
        result[keys] = origin[keys];
      }
    }
  }
  return result;
}
```
JS 的深拷贝的应用，需要根据你的使用场景进行使用，首先是有无必要深拷贝，其次是数据类型，是否直接使用 JSON 的 API 其实就可以。
JS 深浅拷贝在日常开发中使用频率还是较高的，其中考察的知识点，主要在于：  
1、是否遇到过深浅拷贝的问题，里面有什么坑  
2、是否了解 JS 的数据类型，数据在计算机中的存储机制   
3、是否了解数组、对象的一些常用的 API  
4、jquery、lodash、underscore 的相关工具函数使用
#### 总结
深浅拷贝主要考察了开发者对 JS 数据类型的了解，数组，对象常用方法的特点和应用，递归函数的封装。
> 春节快乐！  
写于大年三十，不写文章浑身不舒服~
