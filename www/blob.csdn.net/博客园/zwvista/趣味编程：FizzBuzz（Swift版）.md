# 趣味编程：FizzBuzz（Swift版） - zwvista - 博客园

## [趣味编程：FizzBuzz（Swift版）](https://www.cnblogs.com/zwvista/p/7078702.html)

```
func toFizzBuzzExpr(n: Int) -> String {
    return n % 3 == 0 && n % 5 == 0 ? "FizzBuzz" :
        n % 3 == 0 ? "Fizz" :
        n % 5 == 0 ? "Buzz" :
        String(n);
}
func toFizzBuzzIf(n: Int) -> String {
    if n % 3 == 0 && n % 5 == 0 {
        return "FizzBuzz"
    } else if n % 3 == 0 {
        return "Fizz"
    } else if n % 5 == 0 {
        return "Buzz"
    } else {
        return String(n)
    }
}
func toFizzBuzzSwitch(n: Int) -> String {
    switch (n % 3, n % 5) {
    case (0, 0): return "FizzBuzz"
    case (0, _): return "Fizz"
    case (_, 0): return "Buzz"
    default: return String(n)
    }
}
print([Int](1...100).map(toFizzBuzzExpr))
print([Int](1...100).map(toFizzBuzzIf))
print([Int](1...100).map(toFizzBuzzSwitch))

// ["1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz", "31", "32", "Fizz", "34", "Buzz", "Fizz", "37", "38", "Fizz", "Buzz", "41", "Fizz", "43", "44", "FizzBuzz", "46", "47", "Fizz", "49", "Buzz", "Fizz", "52", "53", "Fizz", "Buzz", "56", "Fizz", "58", "59", "FizzBuzz", "61", "62", "Fizz", "64", "Buzz", "Fizz", "67", "68", "Fizz", "Buzz", "71", "Fizz", "73", "74", "FizzBuzz", "76", "77", "Fizz", "79", "Buzz", "Fizz", "82", "83", "Fizz", "Buzz", "86", "Fizz", "88", "89", "FizzBuzz", "91", "92", "Fizz", "94", "Buzz", "Fizz", "97", "98", "Fizz", "Buzz"]
```


