# Haskell语言学习笔记（78）fix - zwvista - 博客园

## [Haskell语言学习笔记（78）fix](https://www.cnblogs.com/zwvista/p/9267657.html)

### fix 函数

fix 是一个在 Data.Function 模块中定义的函数，它是对于递归的封装，可以用于定义不动点函数。

```
fix :: (a -> a) -> a
fix f = let x = f x in x
```

fix 函数的定义使用了递归绑定，比较难以理解：

```
fix f
= let x = f x in x
= let x = f x in f x
= let x = f x in f (f x)
= let x = f x in f (f (f x))
= let x = f x in f (f .. (f (f x)) ..)
= let x = f x in f . f . ... . f . f $ x
```

即 fix 函数的实质是无限多次调用函数 f，直至函数 f 的返回值不依赖于参数时递归调用终止。

```
Prelude Data.Function> fix (const "hello")
"hello"
Prelude Data.Function> fix (1:)
[1, 1, ...
```

```
fix (const "hello")
= let x = const "hello" x in x
= let x = "hello" in x
= "hello"

fix (1:)
= let x = 1 : x in x
= let x = 1 : x in 1 : x
= let x = 1 : x in 1 : 1 : x
= let x = 1 : x in 1 : 1 : ... 1 : x
= [1, 1, ...]
```

### fix 函数与递归

借助于 fix 函数我们可以将递归函数改写为非递归函数。

以下是计算阶乘的函数的递归版本和使用 fix 函数的非递归版本。

```
Prelude Data.Function> factorial n = if n == 0 then 1 else n * factorial (n-1)
Prelude Data.Function> factorial 3
6
Prelude Data.Function> factorial' = fix (\rec n -> if n == 0 then 1 else n * rec (n-1))
Prelude Data.Function> factorial' 3
6
```

这里 `fix (\rec n -> if n == 0 then 1 else n * rec (n-1))` 就是非递归版本，下面在解释器里检查它的类型

```
Prelude Data.Function> :t (\rec n -> if n == 0 then 1 else n * rec (n-1))
(\rec n -> if n == 0 then 1 else n * rec (n-1))
  :: (Eq p, Num p) => (p -> p) -> p -> p
Prelude Data.Function> :t fix (\rec n -> if n == 0 then 1 else n * rec (n-1))
fix (\rec n -> if n == 0 then 1 else n * rec (n-1))
  :: (Eq p, Num p) => p -> p
```

类型分析：
- `(\rec n -> ...)` 的类型为 `(p -> p) -> p -> p`

参数 rec 的类型为 `(p -> p)`，参数 n 的类型为 p

(Eq p, Num p) 说明类型 p 是可以比较的数值类型。
- 非递归版本 `fix (\rec n -> ...)` 的类型为 `p -> p`
- 即非递归版本中 fix 的类型为`((p -> p) -> p -> p) -> p -> p`
- 对比 fix 函数的定义`fix :: (a -> a) -> a`，可知原先定义中的类型 a 被替换成了 `(p -> p)`

手动计算：

```
fix (\rec n -> if n == 0 then 1 else n * rec (n-1)) 3
= (let x = (\rec n -> if n == 0 then 1 else n * rec (n-1)) x in x) 3
= let x = (\rec n -> if n == 0 then 1 else n * rec (n-1)) x in x 3
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in x 3
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in (\n -> if n == 0 then 1 else n * x (n-1)) 3
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * (x 2)
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * ((\n -> if n == 0 then 1 else n * x (n-1)) 2)
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * (2 * (x 1))
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * (2 * ((\n -> if n == 0 then 1 else n * x (n-1)) 1))
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * (2 * (1 * (x 0)))
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * (2 * (1 * ((\n -> if n == 0 then 1 else n * x (n-1)) 0)))
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 3 * (2 * (1 * 1))
= let x = (\n -> if n == 0 then 1 else n * x (n-1)) in 6
= 6
```

通过手工计算，可以看出 rec 实质上是由 fix 函数所传入的计算阶乘的函数 factorial。

rec 的类型就是递归版本 factorial 的类型。

事实上，递归版本和使用 fix 函数的非递归版本是非常相似的。

```
-- 递归版本 1
factorial 0 = 1
factorial n = n * factorial (n-1)
-- 递归版本 2
factorial n = if n == 0 then 1 else n * factorial (n-1)
-- 递归版本 3
factorial = \n -> if n == 0 then 1 else n * factorial (n-1)
-- 非递归版本 1
factorial' = fix (\rec n -> if n == 0 then 1 else n * rec (n-1))
-- 非递归版本 2
factorial' = fix factorial_ where
  factorial_ rec 0 = 1
  factorial_ rec n = n * rec (n-1)
```

两相对比，不难发现只需要履行一定的步骤就可将递归版本转化为使用 fix 函数的非递归版本。

使用 lambda 的改写方法：
- 如果递归版本 factorial 的函数定义分段进行，我们需要使用 if else 语句或者 case of 语句将所有定义式合成为一个。
- 将递归版本 factorial 的全部参数（这里只有 n）都移到函数定义式的右边，也就是将函数定义改写为一个lambda。
- 在这个 lambda 的所有参数（这里只有 n）之前添加一个 rec 参数，它的类型应该和递归版本 factorial 函数的类型相同。
- 将这个 lambda 中所有对于递归版本 factorial 函数的调用改为对 rec 的调用。
- 将这个改写完毕的 lambda 作为参数传给 fix 即可生成非递归版本 factorial'。

使用具名函数的改写方法：
- 将递归版本的函数名 factorial 按照一定规则（比如加下划线）改名为 factorial_。
- 在这个函数的所有参数（这里只有 n）之前添加一个 rec 参数，它的类型应该和递归版本 factorial 函数的类型相同。
- 将这个函数中所有对于递归版本 factorial 函数的调用改为对 rec 的调用。
- 将这个改写完毕的函数 factorial_ 作为参数传给 fix 即可生成非递归版本 factorial'。
- 如果需要将函数 factorial_ 合并进入非递归版本，可以使用 where 子句将函数 factorial_ 改为局部函数。

### 使用 fix 改写递归函数的例子

map 函数

```
map :: (a -> b) -> [a] -> [b]
map _ []     = []
map f (x:xs) = f x : map f xs
```

改写过程1（使用 lambda）：

```
首先合成所有定义，得到
map f list =
    case list of
        [] -> []
        (x:xs) -> f x : map f xs
将参数 f 和 list 移到右边，得到
map = \f list ->
    case list of
        [] -> []
        (x:xs) -> f x : map f xs
添加 rec 参数，替换 map 可得到
\rec f list ->
    case list of
        [] -> []
        (x:xs) -> f x : rec f xs
将 lambda 传给 fix，可得到
map2 = fix $ \rec f list ->
    case list of
        [] -> []
        (x:xs) -> f x : rec f xs
```

改写过程2（使用具名函数）：

```
改名为 map_ 
添加 rec 参数，替换 map 可得到
map_ rec _ []     = []
map_ rec f (x:xs) = f x : rec f xs
将 map_ 传给 fix，可得到
map3 = fix map_
使用局部函数的话，可以将两者合并
map3 = fix map_ where
  map_ rec _ []     = []
  map_ rec f (x:xs) = f x : rec f xs
```

改写过程3（使用 lambda）：

```
首先合成所有定义，得到
map f list =
    case list of
        [] -> []
        (x:xs) -> f x : map f xs
将map f 看成一个函数，只将参数 list 移到右边，得到
map f = \list ->
    case list of
        [] -> []
        (x:xs) -> f x : map f xs
添加 rec 参数，替换 map f 可得到
\rec list ->
    case list of
        [] -> []
        (x:xs) -> f x : rec xs
将 lambda 传给 fix，可得到
map4 f = fix $ \rec list ->
    case list of
        [] -> []
        (x:xs) -> f x : rec xs
```

### fix 函数与不动点

```
f (fix f)
= f (let x = f x in x)
= let x = f x in f x
= let x = f x in f . f . ... . f . f $ x
= fix f

所以 fix 函数也可以定义为
fix f = f (fix f)
```

即 fix 函数的意义在于寻找函数 f 的不动点 y = fix f，使得 f y == y。

一阶函数的不动点是个常数，二阶以上的高阶函数的不动点是低一阶的函数。

在 fix 函数的定义中，等式右边只有对参数的引用，所以 fix 函数是一个组合子（combinator）。这也是它被定义在Data.Function 这个组合子专用模块的原因。

在计算机科学中，fix 函数这个用来求函数 f 的不动点的组合子被称为不动点组合子或 Y 组合子。

下面计算 y，使得 cos y == y。

递归版本cosFixpointExplicit

```
cosFixpointExplicit x =
    if cos x == x
       then x
       else cosFixpointExplicit (cos x)
```

经过改写可得到使用 fix 函数的非递归版本

```
cosFixpoint x =
    fix (\f b -> 
            if cos b == b 
               then b
               else f (cos b)
         ) x
或者
cosFixpoint2 x =
    ($ x) . fix $ \f b -> 
            if cos b == b 
               then b
               else f (cos b)
或者
cosFixpoint3 x =
    flip fix x $ \f b -> 
            if cos b == b 
               then b
               else f (cos b)
```

```
*Main> cosFixpoint 3
0.7390851332151607
*Main> cosFixpoint 4
0.7390851332151607
*Main> cos it
0.7390851332151607
*Main> cos it == it
True
```

即当 y == 0.7390851332151607 时，cos y == y。

### 不动点与递归

fix 函数将不动点和递归这两者结合了起来。

下面证明上述由递归版本转向使用 fix 函数的非递归版本的改写过程是有效的。

```
factorial = \n -> if n == 0 then 1 else n * factorial (n-1)
等价于
factorial = (\rec n -> if n == 0 then 1 else n * rec (n-1)) factorial
也就是说 factorial 是 (\rec n -> if n == 0 then 1 else n * rec (n-1)) 这个函数的不动点。
对比 fix 函数的定义
fix f = f (fix f)
令 f = (\rec n -> if n == 0 then 1 else n * rec (n-1)) 可得以下等式：
factorial = fix (\rec n -> if n == 0 then 1 else n * rec (n-1))
于是
factorial = \n -> if n == 0 then 1 else n * factorial (n-1)
等价于
factorial = fix (\rec n -> if n == 0 then 1 else n * rec (n-1))
```

由递归版本转向使用 fix 函数的非递归版本的改写过程的实质是：
- 通过给递归版本 factorial 函数添加参数 rec 形成高一阶的非递归函数 factorial_。
- 参数 rec 实质上就是 factorial 函数自身，所以参数 rec 与 factorial 函数类型相同。
- 递归函数 factorial 是高一阶的非递归函数 factorial_ 的不动点。
- 将非递归函数 factorial_ 作为参数传递给 fix 函数形成非递归版本 factorial'。
- 在非递归版本中 factorial' 由 fix 函数带动非递归函数 factorial_ 不断进行递归求解。

手动计算（采用 fix函数的第二个定义`fix f = f (fix f)`）：

```
factorial = \n -> if n == 0 then 1 else n * factorial (n-1)
factorial_ = (\rec n -> if n == 0 then 1 else n * rec (n-1))
factorial = fix factorial_

factorial 3
= fix factorial_ 3
= factorial_ (fix factorial_) 3
= (\rec n -> if n == 0 then 1 else n * rec (n-1)) (fix factorial_) 3
= if 3 == 0 then 1 else 3 * fix factorial_ 2
= 3 * fix factorial_ 2
= 3 * factorial_ (fix factorial_) 2
= 3 * (\rec n -> if n == 0 then 1 else n * rec (n-1)) (fix factorial_) 2
= 3 * (if 2 == 0 then 1 else 2 * fix factorial_ 1)
= 3 * (2 * fix factorial_ 1)
= 3 * (2 * factorial_ (fix factorial_) 1)
= 3 * (2 * (\rec n -> if n == 0 then 1 else n * rec (n-1)) (fix factorial_) 1)
= 3 * (2 * (if 1 == 0 then 1 else 1 * fix factorial_ 0))
= 3 * (2 * (1 * fix factorial_ 0))
= 3 * (2 * (1 * factorial_ (fix factorial_) 0))
= 3 * (2 * (1 * (\rec n -> if n == 0 then 1 else n * rec (n-1)) (fix factorial_) 0))
= 3 * (2 * (1 * (if 0 == 0 then 1 else 0 * fix factorial_ -1)))
= 3 * (2 * (1 * 1))
= 6
```

### 参考链接

[Haskell/Fix and recursion](https://en.wikibooks.org/wiki/Haskell/Fix_and_recursion)
[How do I use fix, and how does it work?](https://stackoverflow.com/questions/4787421/how-do-i-use-fix-and-how-does-it-work)
[Grokking Fix](http://www.parsonsmatt.org/2016/10/26/grokking_fix.html)


