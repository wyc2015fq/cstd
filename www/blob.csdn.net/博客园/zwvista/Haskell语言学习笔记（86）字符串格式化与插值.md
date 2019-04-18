# Haskell语言学习笔记（86）字符串格式化与插值 - zwvista - 博客园

## [Haskell语言学习笔记（86）字符串格式化与插值](https://www.cnblogs.com/zwvista/p/9348270.html)

### String 的格式化
- Text.Printf

这个模块用来处理字符串格式化。
- printf :: PrintfType r => String -> r

printf 用于格式化字符串，注意这个函数的返回类型是多态的。

```
Prelude> :m +Text.Printf
Prelude Text.Printf> printf "%s, %d, %.4f\n" "hello" 123 pi
hello, 123, 3.1416
Prelude Text.Printf> :t printf "%s, %d, %.4f\n" "hello" 123 pi
printf "%s, %d, %.4f" "hello" 123 pi :: PrintfType t => t
Prelude Text.Printf> putStr $ printf "%s, %d, %.4f\n" "hello" 123 pi
hello, 123, 3.1416
Prelude Text.Printf> printf "%s, %d, %.4f\n" "hello" 123 pi :: IO()
hello, 123, 3.1416
Prelude Text.Printf> printf "%s, %d, %.4f\n" "hello" 123 pi :: String
"hello, 123, 3.1416\n"
Prelude Text.Printf> f = printf "%s, %d, %.4f\n" :: String -> Int -> Double -> String
Prelude Text.Printf> f "hello" 123 pi
"hello, 123, 3.1416\n"
```

### String 的插值

安装 interpolate

```
$ cabal install interpolate
Installed interpolate-0.2.0
Prelude> :set -XQuasiQuotes
Prelude> import Data.String.Interpolate
Prelude Data.String.Interpolate>
```

interpolate 是一个处理字符串插值的模块。

该模块使用语言扩展 QuasiQuotes，#{...} 是插值部分。

```
Prelude Data.String.Interpolate> let name = "Marvin"
Prelude Data.String.Interpolate> putStrLn [i|name: #{name}|]
name: Marvin
Prelude Data.String.Interpolate> let age = 23
Prelude Data.String.Interpolate> putStrLn [i|age: #{age}|]
age: 23
Prelude Data.String.Interpolate> let profession = "\955-scientist"
Prelude Data.String.Interpolate> putStrLn [i|profession: #{unwords [name, "the", profession]}|]
profession: Marvin the λ-scientist
```

### Text 的格式化

安装 formatting

```
$ cabal install formatting
Installed formatting-6.3.6
Prelude> :set -XOverloadedStrings
Prelude> import Formatting
Prelude Formatting>
```

formatting 是一个格式化 Text 字符串的库。

```
Prelude Formatting> format ("Person's name is " % text % ", age is " % hex) "Dave" 54
"Person's name is Dave, age is 36"
Prelude Formatting> sformat ("Person's name is " % stext % ", age is " % int) "Dave" 36
"Person's name is Dave, age is 36"
```

这里 format 和 sformat 分别用于结果为 lazy Text 和 strict Text 的格式化。

text, stext, hex, int 都是预定义的格式化占位符，分别代表参数为 lazy Text，strict Text，十六进制整数以及十进制整数类型。

(%) 则用于粘合字符串和格式化占位符。


