# Kotlin语言编程技巧集 - zwvista - 博客园

## [Kotlin语言编程技巧集](https://www.cnblogs.com/zwvista/p/8452388.html)

### 空语句

Kotlin 语言中的空语句有
- {}
- Unit

```
when (x) {
    1 -> ...
    2 -> ...
    else -> {}
    // else -> Unit
}
```

### When 表达式

使用不带判断条件的 when 表达式来改写多路分支

```
val v = if (x < y) 1 else if (x == y) 2 else 3

val v = when {
    x < y -> 1
    x == y -> 2
    else -> 3
}
```

使用带判断条件的 when 表达式来模拟模式匹配

```
val v = if (x == 1) 1 else if (x == 2) 3 else 5

val v = when (x) {
    1 -> 1
    2 -> 3
    else -> 5
}
```

### ?. 与 ?:

```
// n的值为a,b,c,4当中第一个不是null的数
val n = a ?: b ?: c ?: 4
```
|a|b|c|n|
|----|----|----|----|
|1|/|/|1|
|null|2|/|2|
|null|null|3|3|
|null|null|null|4|

```
// n的值为a.b.c，条件是a,a.b,a.b.c都不是null。否则n的值为4。
val n = a?.b?.c ?: 4
```
|a|a.b|a.b.c|n|
|----|----|----|----|
|null|/|/|4|
|!= null|null|/|4|
|!= null|!= null|null|4|
|!= null|!= null|3|3|

### 使用解构声明来声明两个带值的变量

```
var (a, b) = listOf(1, 2) // a == 1, b == 2
var (a, b) = Pair(1, 2) // a == 1, b == 2
var (a, b) = 1 to 2 // a == 1, b == 2
```

### let

[the tldr; on Kotlin’s let, apply, also, with and run functions](https://proandroiddev.com/the-tldr-on-kotlins-let-apply-also-with-and-run-functions-6253f06d152b)

```
// Calls the specified function [block] with `this` value as its argument and returns its result.
public inline fun <T, R> T.let(block: (T) -> R): R = block(this)
```

调用代码块，代码块中调用方 this 为参数 it，返回代码块的结果。

```
// using 'let' to convert from one type to another
val answerToUniverse = strBuilder.let {
    it.append("Douglas Adams was right after all")
    it.append("Life, the Universe and Everything")
    42
}
// using 'let' to only print when str is not null
str?.let { print(it) }
```

### apply

```
// Calls the specified function [block] with `this` value as its receiver and returns `this` value.
public inline fun <T> T.apply(block: T.() -> Unit): T { block(); return this }
```

调用代码块，代码块中调用方 this 为隐式调用方 receiver，返回调用方 this。

```
// old way of building an object
val andre = Person()
andre.name = "andre"
andre.company = "Viacom"
andre.hobby = "losing in ping pong"
// after applying 'apply' (pun very much intended)
val andre = Person().apply {
    name = "Andre"
    company = "Viacom"
    hobby = "losing in ping pong"
}
```

### also

```
// Calls the specified function [block] with `this` value as its argument and returns `this` value.
public inline fun <T> T.also(block: (T) -> Unit): T { block(this); return this }
```

调用代码块，代码块中调用方 this 为参数 it，返回调用方 this 。

```
// transforming data from api with intermediary variable
val rawData = api.getData()
Log.debug(rawData)
rawData.map {  /** other stuff */  }
// use 'also' to stay in the method chains
api.getData()
    .also { Log.debug(it) }
    .map { /** other stuff */ }
```

### with

```
// Calls the specified function [block] with the given [receiver] as its receiver and returns its result.
public inline fun <T, R> with(receiver: T, block: T.() -> R): R = receiver.block()
```

调用代码块，代码块中指定参数为隐式调用方 receiver，返回代码块的结果。

```
// Every Android Developer ever after Wednesday May 17th 2017
    messageBoard.init(“https://url.com”)
    messageBoard.login(token)
    messageBoard.post("Kotlin’s a way of life bro")

// using 'with' to avoid repetitive references to identifier
with(messageBoard) {
    init(“https://url.com”)
    login(token)
    post(“Kotlin’s a way of life bro")
}
```

### run

```
// Calls the specified function [block] with `this` value as its receiver and returns its result.
public inline fun <T, R> T.run(block: T.() -> R): R = block()
```

调用代码块，代码块中调用方 this 为隐式调用方 receiver，返回代码块的结果。

```
// GoT developers after season 7
aegonTargaryen = jonSnow.run {
    makeKingOfTheNorth()
    swearsFealtyTo(daenerysTargaryen)
    realIdentityRevealed(“Aegon Targaryen”)
}
```

### let, apply, also, with & run
|代码块／函数|let|apply|also|with|run|
|----|----|----|----|----|----|
|参数或调用方 this 为隐式调用方 receiver||○||○|○|
|调用方 this 为参数 it|○||○|||
|返回调用方 this||○|○|||
|返回代码块的结果|○|||○|○|

### takeIf / takeUnless

[difference between kotlin also, apply, let, use, takeIf and takeUnless in Kotlin](https://stackoverflow.com/questions/45582732/difference-between-kotlin-also-apply-let-use-takeif-and-takeunless-in-kotlin)

```
// Returns this value if it satisfies the given predicate or null, if it doesn't.
inline fun <T> T.takeIf(predicate: (T) -> Boolean): T? = if (predicate(this)) this else null
// Returns this value if it does not satisfy the given predicate or null, if it does.
inline fun <T> T.takeUnless(predicate: (T) -> Boolean): T? = if (!predicate(this)) this else null
```

takeIf：调用方 this 如果符合某个条件则返回调用方 this，否则返回 null。

takeUnless：调用方 this 如果不符合某个条件则返回调用方 this，否则返回 null。

```
println(myVar.takeIf { it is Person } ?: "Not a person!")
println(myVar.takeUnless { it is Person } ?: "It's a person!")
```


