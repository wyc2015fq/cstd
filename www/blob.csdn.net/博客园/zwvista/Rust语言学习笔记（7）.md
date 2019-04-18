# Rust语言学习笔记（7） - zwvista - 博客园

## [Rust语言学习笔记（7）](https://www.cnblogs.com/zwvista/p/9503310.html)

### 模块

```
// 兄弟模块
mod network {
    fn connect() {
    }
}
mod client {
    fn connect() {
    }
}
// 父子模块
mod network {
    fn connect() {
    }
    mod client {
        fn connect() {
        }
    }
}
// 同一个文件
// src/lib.rs
mod client {
    fn connect() {
    }
}
mod network {
    fn connect() {
    }
    mod server {
        fn connect() {
        }
    }
}
// 两个文件
// src/lib.rs
mod client;
mod network {
    fn connect() {
    }
    mod server {
        fn connect() {
        }
    }
}
// client.rs
fn connect() {
}
// 三个文件
// src/lib.rs
mod client;
mod network;
// src/network.rs
fn connect() {
}
mod server {
    fn connect() {
    }
}
// 四个文件
// src/network/mod.rs
fn connect() {
}
mod server;
// src/network/server.rs
fn connect() {
}
```
- 如果模块 foo 没有子模块，在 foo.rs 中放置该模块。
- 如果模块 foo 有子模块，在 foo/mod.rs 中放置该模块。
- 模块必须在其父模块的文件中使用 mod 声明。

### 可见性

```
// 二进制箱（binary crate）
// src/main.rs（根模块）
extern crate communicator;
fn main() {
    communicator::client::connect();
}
// 库箱（library crate）
// src/lib.rs（根模块）
pub mod client;
// src/client.rs
pub fn connect() {
}
```

### Box<T>

```
let b = Box::new(5);
println!("b = {}", b);
// 递归类型
enum List {
    Cons(i32, Box<List>),
    Nil,
}
use List::{Cons, Nil};
let list = Cons(1,
    Box::new(Cons(2,
        Box::new(Cons(3,
            Box::new(Nil))))));
// Box<T>和引用类型一样支持解引用
// 原因是实现了 Deref 特质
let x = 5;
let y = &x;
let z = Box::new(x);
assert_eq!(5, x);
assert_eq!(5, *y);
assert_eq!(5, *z);
```

### Deref 特质

```
// 自定义类型实现 Deref 特质
struct MyBox<T>(T);
impl<T> MyBox<T> {
    fn new(x: T) -> MyBox<T> {
        MyBox(x)
    }
}
use std::ops::Deref;
impl<T> Deref for MyBox<T> {
    type Target = T;
    fn deref(&self) -> &T {
        &self.0
    }
}
let x = 5;
let y = MyBox::new(x);
assert_eq!(5, x);
assert_eq!(5, *y);
```

### 强制解引用（Deref coercion）

```
fn hello(name: &str) {
    println!("Hello, {}!", name);
}
let m = MyBox::new(String::from("Rust"));
hello(&m); // hello(&(*m)[..]);
```

强制解引用的条件：
- 当 `T: Deref<Target=U>` 时从 `&T` 到 `&U`。
- 当 `T: DerefMut<Target=U>` 时从 `&mut T` 到 `&mut U`。
- 当 `T: Deref<Target=U>` 时从 `&mut T` 到 `&U`。

### Drop 特质

```
// 自定义类型实现 Drop 特质
struct CustomSmartPointer {
    data: String,
}
impl Drop for CustomSmartPointer {
    fn drop(&mut self) {
        println!("Dropping CustomSmartPointer with data `{}`!", self.data);
    }
}
let c = CustomSmartPointer { data: String::from("my stuff") };
let d = CustomSmartPointer { data: String::from("other stuff") };
println!("CustomSmartPointers created.");
/*
CustomSmartPointers created.
Dropping CustomSmartPointer with data `other stuff`!
Dropping CustomSmartPointer with data `my stuff`!
*/
let c = CustomSmartPointer { data: String::from("some data") };
println!("CustomSmartPointer created.");
drop(c);
println!("CustomSmartPointer dropped before the end of main.");
/*
CustomSmartPointer created.
Dropping CustomSmartPointer with data `some data`!
CustomSmartPointer dropped before the end of main.
*/
```

### Rc<T>

```
enum List {
    Cons(i32, Rc<List>),
    Nil,
}
use self::List::{Cons, Nil};
use std::rc::Rc;
let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
println!("count after creating a = {}", Rc::strong_count(&a)); // strong_count == 1
let b = Cons(3, Rc::clone(&a));
println!("count after creating b = {}", Rc::strong_count(&a)); // strong_count == 2
{
    let c = Cons(4, Rc::clone(&a));
    println!("count after creating c = {}", Rc::strong_count(&a)); // strong_count == 3
}
println!("count after c goes out of scope = {}", Rc::strong_count(&a)); // strong_count == 2
```

### RefCell<T>

```
enum List {
    Cons(i32, Rc<List>),
    Nil,
}
use self::List::{Cons, Nil};
use std::rc::Rc;
let a = Rc::new(Cons(5, Rc::new(Cons(10, Rc::new(Nil)))));
println!("count after creating a = {}", Rc::strong_count(&a)); // strong_count == 1
let b = Cons(3, Rc::clone(&a));
println!("count after creating b = {}", Rc::strong_count(&a)); // strong_count == 2
{
    let c = Cons(4, Rc::clone(&a));
    println!("count after creating c = {}", Rc::strong_count(&a)); // strong_count == 3
}
println!("count after c goes out of scope = {}", Rc::strong_count(&a)); // strong_count == 2
```


