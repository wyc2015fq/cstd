# TypeScript语言学习笔记（1） - zwvista - 博客园

## [TypeScript语言学习笔记（1）](https://www.cnblogs.com/zwvista/p/8453396.html)

### 基本类型

```
// 布尔型（Boolean）
let isDone: boolean = false;
// 数值型（Number）
let decimal: number = 6;
let hex: number = 0xf00d;
let binary: number = 0b1010;
let octal: number = 0o744;
// 字符串（String）
let color: string = "blue";
color = 'red';
// 多行字符串和模板字符串（template strings）使用反引号（backtick/backquote）
let fullName: string = `Bob Bobbington`;
let age: number = 37;
let sentence: string = `Hello, my name is ${ fullName }.

I'll be ${ age + 1 } years old next month.`;
// 上面这句等价于使用加号连接字符串
let sentence: string = "Hello, my name is " + fullName + ".\n\n" +
    "I'll be " + (age + 1) + " years old next month.";
// 数组（Array）
let list: number[] = [1, 2, 3];
let list: Array<number> = [1, 2, 3];
// 元组（Tuple）
// Declare a tuple type
let x: [string, number];
// Initialize it
x = ["hello", 10]; // OK
// Initialize it incorrectly
x = [10, "hello"]; // Error
// 使用下标
console.log(x[0].substr(1)); // OK
console.log(x[1].substr(1)); // Error, 'number' does not have 'substr'
// 下标越界时使用联合类型（union）
x[3] = "world"; // OK, 'string' can be assigned to 'string | number'
console.log(x[5].toString()); // OK, 'string' and 'number' both have 'toString'
x[6] = true; // Error, 'boolean' isn't 'string | number'
// 枚举（Enum）
enum Color {Red, Green, Blue}
let c: Color = Color.Green;
// 改变枚举成员的缺省值
enum Color {Red = 1, Green, Blue}
let c: Color = Color.Green;
// 手动设置枚举所有成员的值
enum Color {Red = 1, Green = 2, Blue = 4}
let c: Color = Color.Green;
// 通过值倒推枚举成员的名字
enum Color {Red = 1, Green, Blue}
let colorName: string = Color[2];
alert(colorName); // Displays 'Green' as its value is 2 above
// 任意类型（Any）
let notSure: any = 4;
notSure = "maybe a string instead";
notSure = false; // okay, definitely a boolean
// Object类型作用不同
let notSure: any = 4;
notSure.ifItExists(); // okay, ifItExists might exist at runtime
notSure.toFixed(); // okay, toFixed exists (but the compiler doesn't check)
let prettySure: Object = 4;
prettySure.toFixed(); // Error: Property 'toFixed' doesn't exist on type 'Object'.
// any类型的数组
let list: any[] = [1, true, "free"];
list[1] = 100;
```
- void类型只能接受 null 和 undefined
- null类型只能接受 null
- undefined类型只能接受 undefined
- null 和 undefined 是其他类型的子类型
- 使用 --strictNullChecks 编译选项（flag）时，null 和 undefined 不是其他类型的子类型
- 即不使用该编译选项时的 string 相当于使用该编译选项时的联合类型 string | null | undefined
- never类型是所有类型的子类型，never 类型只接受 never

```
// 虚无类型（Void）
function warnUser(): void {
    alert("This is my warning message");
}
let unusable: void = undefined;
// 空值和未定义（Null and Undefined）
// Not much else we can assign to these variables!
let u: undefined = undefined;
let n: null = null;
// 不可能类型（Never）
// Function returning never must have unreachable end point
function error(message: string): never {
    throw new Error(message);
}
// Inferred return type is never
function fail() {
    return error("Something failed");
}
// Function returning never must have unreachable end point
function infiniteLoop(): never {
    while (true) {
    }
}
```
- 类型断言相当于其他语言中的类型转换
- 类型断言只在编译期起作用，在运行期没有任何效果
- 类型断言有两种语法形式，尖括号和 as 。

```
// 类型断言（Type assertions）
let someValue: any = "this is a string";
let strLength: number = (<string>someValue).length;
let someValue: any = "this is a string";
let strLength: number = (someValue as string).length;
```

### 变量声明

var声明

```
// var声明
var a = 10;
// 在函数中使用var声明
function f() {
    var message = "Hello, world!";
    return message;
}
// 在其他函数中访问var变量
function f() {
    var a = 10;
    return function g() {
        var b = a + 1;
        return b;
    }
}
var g = f();
g(); // returns '11'
//
function f() {
    var a = 1;

    a = 2;
    var b = g();
    a = 3;

    return b;

    function g() {
        return a;
    }
}
f(); // returns '2'
// 作用域
function f(shouldInitialize: boolean) {
    if (shouldInitialize) {
        var x = 10;
    }
    return x;
}
f(true);  // returns '10'
f(false); // returns 'undefined'
// 可以反复声明同一变量
function sumMatrix(matrix: number[][]) {
    var sum = 0;
    for (var i = 0; i < matrix.length; i++) {
        var currentRow = matrix[i];
        for (var i = 0; i < currentRow.length; i++) {
            sum += currentRow[i];
        }
    }
    return sum;
}
//
for (var i = 0; i < 10; i++) {
    setTimeout(function() { console.log(i); }, 100 * i);
}
10
10
10
10
10
10
10
10
10
10
for (var i = 0; i < 10; i++) {
    // capture the current state of 'i'
    // by invoking a function with its current value
    (function(i) {
        setTimeout(function() { console.log(i); }, 100 * i);
    })(i);
}
0
1
2
3
4
5
6
7
8
9
```

let声明

```
// let声明
let hello = "Hello!";
// 代码块作用域
function f(input: boolean) {
    let a = 100;
    if (input) {
        // Still okay to reference 'a'
        let b = a + 1;
        return b;
    }
    // Error: 'b' doesn't exist here
    return b;
}
// catch代码块
try {
    throw "oh no!";
}
catch (e) {
    console.log("Oh well.");
}
// Error: 'e' doesn't exist here
console.log(e);
// 不能在声明之前使用
a++; // illegal to use 'a' before it's declared;
let a;
// 不能在声明之前使用
function foo() {
    // okay to capture 'a'
    return a;
}
// illegal call 'foo' before 'a' is declared
// runtimes should throw an error here
foo();
let a;
// 重新声明
function f(x) {
    var x;
    var x;
    if (true) {
        var x;
    }
}
// 不能重复声明
let x = 10;
let x = 20; // error: can't re-declare 'x' in the same scope
// 不能重复声明
function f(x) {
    let x = 100; // error: interferes with parameter declaration
}
function g() {
    let x = 100;
    var x = 100; // error: can't have both declarations of 'x'
}
// 在不同代码块中再次声明
function f(condition, x) {
    if (condition) {
        let x = 100;
        return x;
    }
    return x;
}
f(false, 0); // returns '0'
f(true, 0);  // returns '100'
// 在不同代码块中再次声明
function sumMatrix(matrix: number[][]) {
    let sum = 0;
    for (let i = 0; i < matrix.length; i++) {
        var currentRow = matrix[i];
        for (let i = 0; i < currentRow.length; i++) {
            sum += currentRow[i];
        }
    }
    return sum;
}
// 捕获代码块中的变量
function theCityThatAlwaysSleeps() {
    let getCity;
    if (true) {
        let city = "Seattle";
        getCity = function() {
            return city;
        }
    }
    return getCity();
}
for (let i = 0; i < 10 ; i++) {
    setTimeout(function() { console.log(i); }, 100 * i);
}
0
1
2
3
4
5
6
7
8
9
// const声明
const numLivesForCat = 9;
// 不变量的声明
const numLivesForCat = 9;
const kitty = {
    name: "Aurora",
    numLives: numLivesForCat,
}
// Error
kitty = {
    name: "Danielle",
    numLives: numLivesForCat
};
// all "okay"
kitty.name = "Rory";
kitty.name = "Kitty";
kitty.name = "Cat";
kitty.numLives--;
```

解构声明

```
// 数组解构
let input = [1, 2];
let [first, second] = input;
console.log(first); // outputs 1
console.log(second); // outputs 2
// 语义
first = input[0];
second = input[1];
// 交换变量
[first, second] = [second, first];
// 在参数中解构
function f([first, second]: [number, number]) {
    console.log(first);
    console.log(second);
}
f([1, 2]);
// 剩余项目
let [first, ...rest] = [1, 2, 3, 4];
console.log(first); // outputs 1
console.log(rest); // outputs [ 2, 3, 4 ]
// 只取第一项
let [first] = [1, 2, 3, 4];
console.log(first); // outputs 1
// 只取某几项
let [, second, , fourth] = [1, 2, 3, 4];
// 对象解构
let o = {
    a: "foo",
    b: 12,
    c: "bar"
};
let { a, b } = o;
// 不用声明也能解构
({ a, b } = { a: "baz", b: 101 });
// 剩余项目
let { a, ...passthrough } = o;
let total = passthrough.b + passthrough.c.length;
// 重命名
let { a: newName1, b: newName2 } = o;
// 语义
let newName1 = o.a;
let newName2 = o.b;
// 带类型的解构声明
let { a, b }: { a: string, b: number } = o;
//
function keepWholeObject(wholeObject: { a: string, b?: number }) {
    let { a, b = 1001 } = wholeObject;
}
// 在函数声明中解构
type C = { a: string, b?: number }
function f({ a, b }: C): void {
    // ...
}
function f({ a, b = 0 } = { a: "" }): void {
    // ...
}
f({ a: "yes" }); // ok, default b = 0
f(); // ok, default to { a: "" }, which then defaults b = 0
f({}); // error, 'a' is required if you supply an argument
// 展开（spread）运算符
let first = [1, 2];
let second = [3, 4];
let bothPlus = [0, ...first, ...second, 5]; // 0,1,2,3,4,5
// 展开对象
let defaults = { food: "spicy", price: "$$", ambiance: "noisy" };
let search = { ...defaults, food: "rich" }; // { food: "rich", price: "$$", ambiance: "noisy" }
// 后面覆盖前面
let defaults = { food: "spicy", price: "$$", ambiance: "noisy" };
let search = { food: "rich", ...defaults }; // { food: "spicy", price: "$$", ambiance: "noisy" };
// 对象展开只展开对象的属性不展开对象的方法
class C {
  p = 12;
  m() {
  }
}
let c = new C();
let clone = { ...c };
clone.p; // ok
clone.m(); // error!
```


