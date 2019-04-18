# Rust语言学习笔记（5） - zwvista - 博客园

## [Rust语言学习笔记（5）](https://www.cnblogs.com/zwvista/p/9462355.html)

### Structs（结构体）

```
struct User {
    username: String,
    email: String,
    sign_in_count: u64,
    active: bool,
}
let mut user1 = User {
    email: String::from("someone@example.com"),
    username: String::from("someusername123"),
    active: true,
    sign_in_count: 1,
};
user1.email = String::from("anotheremail@example.com");
// 结构体初始化缩写形式
fn build_user(email: String, username: String) -> User {
    User {
        email, // email: email,
        username, // username: username,
        active: true,
        sign_in_count: 1,
    }
}
// 结构体更新语法
let user2 = User {
    email: String::from("another@example.com"),
    username: String::from("anotherusername567"),
    ..user1 // active: user1.active, ...
};
```

### Tuple Structs（元组结构体）

```
struct Color(i32, i32, i32);
struct Point(i32, i32, i32);
let black = Color(0, 0, 0);
let origin = Point(0, 0, 0);
```
- 元组结构体实质上是整体有名字但是成员没有名字的元组。
- 成员完全相同但是结构体名字不同的两个元组结构体不是同一个类型。

比如 Color 和 Point 是两个类型。
- 可以定义完全没有成员的单元元组结构体。

### Debug 特质

```
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}
fn main() {
    let rect1 = Rectangle { width: 30, height: 50 };
    println!("rect1 is {:?}", &rect1);
    println!("rect1 is {:#?}", &rect1);
}
/*
rect1 is Rectangle { width: 30, height: 50 }
rect1 is Rectangle {
    width: 30,
    height: 50
}
*/
```

### 方法

```
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}
impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }
    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}
impl Rectangle {
    fn square(size: u32) -> Rectangle {
        Rectangle { width: size, height: size }
    }
}
fn main() {
    let rect1 = Rectangle { width: 30, height: 50 };
    println!(
        "The area of the rectangle is {} square pixels.",
        rect1.area()
    );

    let rect2 = Rectangle { width: 10, height: 40 };
    let rect3 = Rectangle { width: 60, height: 45 };
    println!("Can rect1 hold rect2? {}", rect1.can_hold(&rect2));
    println!("Can rect1 hold rect3? {}", rect1.can_hold(&rect3));

    let sq = Rectangle::square(3);
}
/*
The area of the rectangle is 1500 square pixels.
Can rect1 hold rect2? true
Can rect1 hold rect3? false
*/
```
- 定义方法采用 impl 关键字。方法在结构体之外的 impl 块中定义。
- 方法签名中指向结构体自身的参数有3种形式：self, &self, &mut self。
- self 参数不需要标注类型。
- 调用方法采用 object.method() 语法。

object 形式固定，不受 self 参数形式的影响。
- 同一个结构体的方法可以定义在多个 impl 块中。
- impl 块中可以定义不带 self 参数的函数。这些函数通常用作构造器。

### Enums（枚举）

```
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}
impl Message {
    fn call(&self) {
        // method body would be defined here
    }
}
let m = Message::Write(String::from("hello"));
m.call();
```

枚举是一种变体类型。

### Option<T>

语言中没有空指针或 Null 引用，采用`Option<T>`类型。

```
// 标准库中的 Option 类型
enum Option<T> {
    Some(T),
    None,
}
```

```
let some_number = Some(5);
let some_string = Some("a string");
// None 的类型无法推导，需要标注
let absent_number: Option<i32> = None;
```

### match 表达式

```
enum UsState {
    Alabama,
    Alaska,
    // --snip--
}
enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}
fn value_in_cents(coin: Coin) -> u32 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => {
            println!("State quarter from {:?}!", state);
            25
        },
    }
}
value_in_cents(Coin::Quarter(UsState::Alaska)); // State quarter from Alaska!
```
- match 表达式用来实现模式匹配。

模式匹配语法：`pattern => code,`
- 模式匹配必须穷尽所有可能。（exhaustive）
- 存在相当于 else 的`_`模式。

```
fn plus_one(x: Option<i32>) -> Option<i32> {
    match x {
        None => None,
        Some(i) => Some(i + 1),
    }
}
let five = Some(5);
let six = plus_one(five);
let none = plus_one(None);
```

```
let some_u8_value = 0u8;
match some_u8_value {
    1 => println!("one"),
    3 => println!("three"),
    5 => println!("five"),
    7 => println!("seven"),
    _ => (),
}
```

### if let

```
let some_u8_value = Some(0u8);
match some_u8_value {
    Some(3) => println!("three"),
    _ => (),
}
// 相当于
if let Some(3) = some_u8_value {
    println!("three");
}
```

```
let mut count = 0;
match coin {
    Coin::Quarter(state) => println!("State quarter from {:?}!", state),
    _ => count += 1,
}
// 相当于
let mut count = 0;
if let Coin::Quarter(state) = coin {
    println!("State quarter from {:?}!", state);
} else {
    count += 1;
}
```

### Vec<T>

Vec<T> 是一种动态数组类型。

```
// 空
let v: Vec<i32> = Vec::new();
// 带初值
let v = vec![1, 2, 3];
// 更新
let mut v = Vec::new();
v.push(5);
v.push(6);
v.push(7);
v.push(8);
// 取元素的两种方法
let v = vec![1, 2, 3, 4, 5];
let third: &i32 = &v[2];
let third: Option<&i32> = v.get(2);
// 数组越界
let does_not_exist = &v[100]; // panic!
let does_not_exist = v.get(100); // None
// 遍历
let v = vec![100, 32, 57];
for i in &v {
    println!("{}", i);
}
let mut v = vec![100, 32, 57];
for i in &mut v {
    *i += 50;
}
// 枚举类型（变体）的数组
enum SpreadsheetCell {
    Int(i32),
    Float(f64),
    Text(String),
}
let row = vec![
    SpreadsheetCell::Int(3),
    SpreadsheetCell::Text(String::from("blue")),
    SpreadsheetCell::Float(10.12),
];
```

### String 类型

utf8 字符组成的数组。

```
let mut s = String::new();
// to_string 方法
let data = "initial contents";
let s = data.to_string();
// the method also works on a literal directly:
let s = "initial contents".to_string();
// from 方法
let hello = String::from("你好");
// push_str 和 push 方法
let mut s = String::from("foo");
s.push_str("bar");
s.push('l');
// 字符串相加
let s1 = String::from("Hello, ");
let s2 = String::from("world!");
let s3 = s1 + &s2; // note s1 has been moved here and can no longer be used
// fn add(self, s: &str) -> String
let s1 = String::from("tic");
let s2 = String::from("tac");
let s3 = String::from("toe");
let s = s1 + "-" + &s2 + "-" + &s3;
// 改用 format
let s = format!("{}-{}-{}", s1, s2, s3);
// 
let hello = "Здравствуйте";
let len = hello.len(); // 24
let s = &hello[0..4]; // Зд
println!("{}, {}", len, s);
for c in hello.chars() {
    print!("{} ", c); // З д р а в с т в у й т е 
}
```

### HashMap 类型

```
use std::collections::HashMap;
// 使用 Vec 创建 HashMap
let teams  = vec![String::from("Blue"), String::from("Yellow")];
let initial_scores = vec![10, 50];
let scores: HashMap<_, _> = teams.iter().zip(initial_scores.iter()).collect();
// 创建并插入键值对
let mut scores = HashMap::new();
scores.insert(String::from("Blue"), 10);
scores.insert(String::from("Yellow"), 50);
let team_name = String::from("Blue");
let score = scores.get(&team_name);
for (key, value) in &scores {
    println!("{}: {}", key, value);
}
// 更新
scores.insert(String::from("Blue"), 25);
scores.entry(String::from("Yellow")).or_insert(50);
scores.entry(String::from("Blue")).or_insert(50);
//
let text = "hello world wonderful world";
let mut map = HashMap::new();
for word in text.split_whitespace() {
    let count = map.entry(word).or_insert(0);
    *count += 1;
}
println!("{:?}", map);
```
- HashMap 的键值对类型为 Copy 特质类型时，使用 Copy 语义。
- HashMap 的键值对类型为非 Copy 特质类型（比如 String）时，使用 Move 语义。


