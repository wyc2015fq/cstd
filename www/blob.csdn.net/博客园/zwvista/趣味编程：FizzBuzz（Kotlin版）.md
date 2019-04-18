# 趣味编程：FizzBuzz（Kotlin版） - zwvista - 博客园

## [趣味编程：FizzBuzz（Kotlin版）](https://www.cnblogs.com/zwvista/p/7078841.html)

```
fun toFizzBuzzIf(n: Int) =
    if (n % 3 == 0 && n % 5 == 0) "FizzBuzz"
    else if (n % 3 == 0) "Fizz"
    else if (n % 5 == 0) "Buzz"
    else n.toString()

fun toFizzBuzzWhen1(n: Int) =
    when {
        n % 3 == 0 && n % 5 == 0 -> "FizzBuzz"
        n % 3 == 0 -> "Fizz"
        n % 5 == 0 -> "Buzz"
        else -> n.toString()
    }

fun toFizzBuzzWhen2(n: Int) =
    when (listOf(n % 3 == 0, n % 5 == 0)) {
        listOf(true, true) -> "FizzBuzz"
        listOf(true, false) -> "Fizz"
        listOf(false, true) -> "Buzz"
        else -> n.toString()
    }

fun main(args: Array<String>) {
    println((1..100).map(::toFizzBuzzIf))
    println((1..100).map(::toFizzBuzzWhen1))
    println((1..100).map(::toFizzBuzzWhen2))
}

// [1, 2, Fizz, 4, Buzz, Fizz, 7, 8, Fizz, Buzz, 11, Fizz, 13, 14, FizzBuzz, 16, 17, Fizz, 19, Buzz, Fizz, 22, 23, Fizz, Buzz, 26, Fizz, 28, 29, FizzBuzz, 31, 32, Fizz, 34, Buzz, Fizz, 37, 38, Fizz, Buzz, 41, Fizz, 43, 44, FizzBuzz, 46, 47, Fizz, 49, Buzz, Fizz, 52, 53, Fizz, Buzz, 56, Fizz, 58, 59, FizzBuzz, 61, 62, Fizz, 64, Buzz, Fizz, 67, 68, Fizz, Buzz, 71, Fizz, 73, 74, FizzBuzz, 76, 77, Fizz, 79, Buzz, Fizz, 82, 83, Fizz, Buzz, 86, Fizz, 88, 89, FizzBuzz, 91, 92, Fizz, 94, Buzz, Fizz, 97, 98, Fizz, Buzz]
```


