# Rust语言学习笔记（4） - zwvista - 博客园

## [Rust语言学习笔记（4）](https://www.cnblogs.com/zwvista/p/7223343.html)

### Variables and Mutability（变量和可变性）
- 变量声明有三种：不变量（运行期的常量），变量以及（编译期的）常量。
- 变量可以重复绑定，后声明的变量覆盖前面声明的同名变量，重复绑定时可改变类型。

```
// 运行期常量（不可变变量 immutable variable）
let x = 5;
x = 6; // compile error
// 变量（可变变量 mutable variable）
let mut x = 5;
x = 6;
// 编译期常量（constant）
const MAX_POINTS: u32 = 100_000;
// 重复绑定（不改变类型）
let x = 5;
let x = x + 1;
let x = x * 2;
// 重复绑定（改变类型）
let spaces = "   ";
let spaces = spaces.len();
```

### Data Types（数据类型）
- Scalar Types（标量类型）
- Integer Types（整形）
- i8, u8, i16, u16, i32, u32, i64, u64
- Decimal, Hex, Octal, Binary, Byte

- Floating-Point Types（浮点类型）
- f32, f64

- Boolean Type（布尔类型）
- Character Type（字符类型）

- Compound Types（复合类型）
- Tuples（元组）
- Arrays（数组）

```
// 元组（带类型声明 type annotation）
let tup: (i32, f64, u8) = (500, 6.4, 1);
// 元组（类型自动推导，解构 destructuring）
let tup = (500, 6.4, 1);
let (x, y, z) = tup;
println!("The value of y is: {}", y);
// 元组（使用下标）
let x: (i32, f64, u8) = (500, 6.4, 1);
let five_hundred = x.0;
let six_point_four = x.1;
let one = x.2;
```

```
// 数组，大小不可变，分配在栈上
let months = ["January", "February", "March", "April", "May", "June", "July",
              "August", "September", "October", "November", "December"];
let a = [1, 2, 3, 4, 5];
let first = a[0];
let second = a[1];
```

### How Functions Work（函数的运行机制）

```
// 赋值语句不返回值。
let y = {
    let x = 3;
    // 代码块的最后一句可以是表达式。
    // 表达式末尾不带分号。加了分号表达式变语句。
    x + 1
};
```

```
// 函数的最后一句可以是表达式，表示返回值。
fn five() -> i32 {
    5
}
fn plus_one(x: i32) -> i32 {
    x + 1
}
```

### Control Flow（控制流）
- if ... else if ... else 表达式（可以返回值）
- loop 语句
- while 语句
- for ... in 语句（实质是 foreach）

### Ownership（所有权）

```
let mut s = String::from("hello");
s.push_str(", world!"); // push_str() appends a literal to a String
println!("{}", s); // This will print `hello, world!`
```

```
let x = 5;
// copy
let y = x;
let s1 = String::from("hello");
// move
let s2 = s1;
// clone
let s2 = s1.clone();
```
- 有 Copy 特质的类型在赋值时执行 Copy。

Copy 特质类型包括整形，浮点型，布尔型等基本类型以及它们所组成的元组。
- 没有 Copy 特质的类型在赋值时执行 Move。（浅拷贝 + 被赋值的变量失效）
- 没有 Copy 特质的类型需要深拷贝时需要显式调用 clone 方法。
- 给函数传参数相当于赋值。

```
// 移动
let s1 = String::from("hello");
let (s2, len) = calculate_length(s1);
fn calculate_length(s: String) -> (String, usize) {
    let length = s.len(); // len() returns the length of a String
    (s, length)
}
// 不变引用
let s1 = String::from("hello");
let len = calculate_length(&s1);
fn calculate_length(s: &String) -> usize {
    s.len()
}
// 可变引用
let s = String::from("hello");
change(&s);
fn change(some_string: &mut String) {
    some_string.push_str(", world");
}
```

在同一个作用域对同一份数据
- 可以有多个不变引用
- 只能有一个可变引用
- 不变引用和可变引用不能同时存在

### Slices（片段）

存在字符串片段以及数组片段，片段是只读引用。

字符串常量是片段类型。

```
// 查找字符串中的第一个单词，返回字符串片段类型 &str
fn first_word(s: &String) -> &str {
    let bytes = s.as_bytes();
    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[0..i];
        }
    }
    &s[..]
}
// 省略下标
let s = String::from("hello");
let slice = &s[0..2];
let slice = &s[..2];
// 省略下标
let len = s.len();
let slice = &s[3..len];
let slice = &s[3..];
// 省略下标
let slice = &s[0..len];
let slice = &s[..];
// a 的类型是 &[i32] 
let a = [1, 2, 3, 4, 5];
let slice = &a[1..3];
```


