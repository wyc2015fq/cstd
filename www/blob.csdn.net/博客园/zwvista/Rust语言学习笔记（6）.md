# Rust语言学习笔记（6） - zwvista - 博客园

## [Rust语言学习笔记（6）](https://www.cnblogs.com/zwvista/p/9468895.html)

### Traits（特质）

```
// 特质
pub trait Summary {
    fn summarize(&self) -> String;
}
pub struct NewsArticle {
    pub headline: String,
    pub location: String,
    pub author: String,
    pub content: String,
}
// 实现特质
impl Summary for NewsArticle {
    fn summarize(&self) -> String {
        format!("{}, by {} ({})", self.headline, self.author, self.location)
    }
}
pub struct Tweet {
    pub username: String,
    pub content: String,
    pub reply: bool,
    pub retweet: bool,
}
// 实现特质
impl Summary for Tweet {
    fn summarize(&self) -> String {
        format!("{}: {}", self.username, self.content)
    }
}
// 使用特质
let tweet = Tweet {
    username: String::from("horse_ebooks"),
    content: String::from("of course, as you probably already know, people"),
    reply: false,
    retweet: false,
};
println!("1 new tweet: {}", tweet.summarize());
// 缺省实现
pub trait Summary {
    fn summarize(&self) -> String {
        String::from("(Read more...)")
    }
}
// 使用特质的缺省实现
let article = NewsArticle {
    headline: String::from("Penguins win the Stanley Cup Championship!"),
    location: String::from("Pittsburgh, PA, USA"),
    author: String::from("Iceburgh"),
    content: String::from("The Pittsburgh Penguins once again are the best
    hockey team in the NHL."),
};
println!("New article available! {}", article.summarize());
// 有缺省实现的方法可以调用没有缺省实现的方法
pub trait Summary {
    fn summarize_author(&self) -> String;
    fn summarize(&self) -> String {
        format!("(Read more from {}...)", self.summarize_author())
    }
}
impl Summary for Tweet {
    fn summarize_author(&self) -> String {
        format!("@{}", self.username)
    }
}
let tweet = Tweet {
    username: String::from("horse_ebooks"),
    content: String::from("of course, as you probably already know, people"),
    reply: false,
    retweet: false,
};
println!("1 new tweet: {}", tweet.summarize());
// 作为参数类型的特质
pub fn notify(item: impl Summary) {
    println!("Breaking news! {}", item.summarize());
}
pub fn notify<T: Summary>(item: T) {
    println!("Breaking news! {}", item.summarize());
}
// 两个特质类型的参数
pub fn notify(item1: impl Summary, item2: impl Summary) {
pub fn notify<T: Summary>(item1: T, item2: T) {
// 多个特质
fn some_function<T: Display + Clone, U: Clone + Debug>(t: T, u: U) -> i32 
// where 子句
fn some_function<T, U>(t: T, u: U) -> i32
    where T: Display + Clone,
          U: Clone + Debug
{
// 返回特质
fn returns_summarizable() -> impl Summary {
    Tweet {
        username: String::from("horse_ebooks"),
        content: String::from("of course, as you probably already know, people"),
        reply: false,
        retweet: false,
    }
}
// 根据特质选择性地实现方法
use std::fmt::Display;
struct Pair<T> {
    x: T,
    y: T,
}
impl<T> Pair<T> {
    fn new(x: T, y: T) -> Self {
        Self {
            x,
            y,
        }
    }
}
impl<T: Display + PartialOrd> Pair<T> {
    fn cmp_display(&self) {
        if self.x >= self.y {
            println!("The largest member is x = {}", self.x);
        } else {
            println!("The largest member is y = {}", self.y);
        }
    }
}
```

### 泛型

```
// 泛型函数
fn largest<T: PartialOrd + Copy>(list: &[T]) -> T {
    let mut largest = list[0];
    for &item in list.iter() {
        if item > largest {
            largest = item;
        }
    }
    largest
}
let number_list = vec![34, 50, 25, 100, 65];
let result = largest(&number_list);
println!("The largest number is {}", result);
let char_list = vec!['y', 'm', 'a', 'q'];
let result = largest(&char_list);
println!("The largest char is {}", result);
// 泛型结构体
struct Point<T> {
    x: T,
    y: T,
}
let integer = Point { x: 5, y: 10 };
let float = Point { x: 1.0, y: 4.0 };
// 带两个类型参数的泛型结构体
struct Point<T, U> {
    x: T,
    y: U,
}
let both_integer = Point { x: 5, y: 10 };
let both_float = Point { x: 1.0, y: 4.0 };
let integer_and_float = Point { x: 5, y: 4.0 };
// 标准库中的泛型枚举
enum Option<T> {
    Some(T),
    None,
}
enum Result<T, E> {
    Ok(T),
    Err(E),
}
// 泛型方法
struct Point<T> {
    x: T,
    y: T,
}
impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}
let p = Point { x: 5, y: 10 };
println!("p.x = {}", p.x());
// 针对具体类型实现泛型结构体的方法（==C++的全特化）
impl Point<f32> {
    fn distance_from_origin(&self) -> f32 {
        (self.x.powi(2) + self.y.powi(2)).sqrt()
    }
}
// 带类型参数的泛型方法
struct Point<T, U> {
    x: T,
    y: U,
}
impl<T, U> Point<T, U> {
    fn mixup<V, W>(self, other: Point<V, W>) -> Point<T, W> {
        Point {
            x: self.x,
            y: other.y,
        }
    }
}
let p1 = Point { x: 5, y: 10.4 };
let p2 = Point { x: "Hello", y: 'c'};
let p3 = p1.mixup(p2);
println!("p3.x = {}, p3.y = {}", p3.x, p3.y);
```

### 生命周期（Lifetimes）

```
// 带生命周期的引用类型
&i32        // a reference
&'a i32     // a reference with an explicit lifetime
&'a mut i32 // a mutable reference with an explicit lifetime
// 带生命周期注解的函数
fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}
// 调用带生命周期注解的函数
let string1 = String::from("long string is long");
{
    let string2 = String::from("xyz");
    let result = longest(string1.as_str(), string2.as_str());
    println!("The longest string is {}", result);
}
// 调用带生命周期注解的函数
let string1 = String::from("long string is long");
let result;
{
    let string2 = String::from("xyz");
    result = longest(string1.as_str(), string2.as_str()); // error
}
println!("The longest string is {}", result);
// 带生命周期注解的结构体
struct ImportantExcerpt<'a> {
    part: &'a str,
}
let novel = String::from("Call me Ishmael. Some years ago...");
let first_sentence = novel.split('.')
    .next()
    .expect("Could not find a '.'");
let i = ImportantExcerpt { part: first_sentence };
// 带生命周期注解的方法
impl<'a> ImportantExcerpt<'a> {
    fn level(&self) -> i32 {
        3
    }
}
// 静态生命周期
let s: &'static str = "I have a static lifetime.";
```

### 闭包（Closures）

```
// 闭包
fn  add_one_v1   (x: u32) -> u32 { x + 1 }
let add_one_v2 = |x: u32| -> u32 { x + 1 };
let add_one_v3 = |x|             { x + 1 };
let add_one_v4 = |x|               x + 1  ;
let add_one_v5: fn(u32) -> u32 = |x| x + 1;
// 没有泛型闭包
let example_closure = |x| x;
let s = example_closure(String::from("hello"));
let n = example_closure(5); // error
// 闭包可以捕获外层变量，局部函数不行
let x = 4;
let equal_to_x = |z| z == x;
assert!(equal_to_x(4));
fn equal_to_x(z: i32) -> bool { z == x } // error
// Move 捕获
let x = vec![1, 2, 3];
let equal_to_x = move |z| z == x;
println!("can't use x here: {:?}", x);  // error
```

### Iterator 特质

```
trait Iterator {
    type Item;
    fn next(&mut self) -> Option<Self::Item>;
    // methods with default implementations elided
}
// 使用 Iterator 特质
let v1 = vec![1, 2, 3];
let v1_iter = v1.iter();
for val in v1_iter {
    println!("Got: {}", val);
}
// 使用 Iterator 特质
let mut v1_iter = v1.iter();
assert_eq!(v1_iter.next(), Some(&1));
assert_eq!(v1_iter.next(), Some(&2));
assert_eq!(v1_iter.next(), Some(&3));
assert_eq!(v1_iter.next(), None);
// sum 方法
let v1_iter = v1.iter();
let total: i32 = v1_iter.sum();
assert_eq!(total, 6);
// map, collect, filter 方法
let v2: Vec<_> = v1.iter().map(|x| x + 1).collect();
assert_eq!(v2, vec![2, 3, 4]);
// 实现 Iterator 特质
struct Counter {
    count: u32,
}
impl Counter {
    fn new() -> Counter {
        Counter { count: 0 }
    }
}
impl Iterator for Counter {
    type Item = u32;
    fn next(&mut self) -> Option<Self::Item> {
        self.count += 1;

        if self.count < 6 {
            Some(self.count)
        } else {
            None
        }
    }
}
// 使用 Iterator 特质
let mut counter = Counter::new();
assert_eq!(counter.next(), Some(1));
assert_eq!(counter.next(), Some(2));
assert_eq!(counter.next(), Some(3));
assert_eq!(counter.next(), Some(4));
assert_eq!(counter.next(), Some(5));
assert_eq!(counter.next(), None);
let sum: u32 = Counter::new().zip(Counter::new().skip(1))
                             .map(|(a, b)| a * b)
                             .filter(|x| x % 3 == 0)
                             .sum();
assert_eq!(18, sum);
```


