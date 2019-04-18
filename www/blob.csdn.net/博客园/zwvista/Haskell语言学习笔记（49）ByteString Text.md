# Haskell语言学习笔记（49）ByteString Text - zwvista - 博客园

## [Haskell语言学习笔记（49）ByteString Text](https://www.cnblogs.com/zwvista/p/7884268.html)

### Data.ByteString

String 是 [Char] 的同义词，在使用上存在List的惰性所带来的性能问题。

在处理大型二进制文件时，可以使用 ByteString 来代替 String。

ByteString 类型分为以下两种：
- Lazy 模块 Data.ByteString.Lazy 中的 Data.ByteString.Lazy.ByteString

Lazy 模块内部使用 chunks（64K数据块）。
- Strict 模块 Data.ByteString 中的 Data.ByteString.ByteString

```
Prelude> import qualified Data.ByteString.Lazy as B
Prelude B> import qualified Data.ByteString as S
Prelude B S> B.pack [99,97,110]
"can"
Prelude B S> B.fromChunks [S.pack [40,41,42], S.pack [43,44,45], S.pack [46,47,48]]
"()*+,-./0"
Prelude B S> B.cons 85 $ B.pack [80,81,82,84]
"UPQRT"
Prelude B S> foldr B.cons' B.empty [50..60] 
"23456789:;<"
```

### Data.Text

在处理大型文本文件时，可以使用 Text 来代替 String。

Text 是一个 Unicode 友好的字符串类型。

Text 类型分为以下两种：
- Lazy 模块 Data.Text.Lazy 中的 Data.Text.Lazy.Text

Lazy 模块内部使用 chunks。
- Strict 模块 Data.Text 中的 Data.Text.Text

```
Prelude> import qualified Data.Text.Lazy as T
Prelude T> :t T.pack "abc"
T.pack "abc" :: T.Text
Prelude T> T.replace (T.pack "a") (T.pack "b") (T.pack "aXaXaX")
"bXbXbX"
Prelude T> T.splitOn (T.pack "a") (T.pack "aXaXaX")
["","X","X","X"]
```
- pack :: String -> Text

pack 函数将 String 转换成 Text。

### Data.Text.Encoding

Encoding 模块用于 ByteString 和 Text 之间类型转换。

Text 包含 Lazy 模块 Data.Text.Encoding.Lazy 和 Strict 模块 Data.Text.Encoding。

其中 Lazy 模块提供 Data.ByteString.Lazy.ByteString 和 Data.Text.Lazy.Text 之间的类型转换功能。

而 Strict 模块则提供 Data.ByteString.ByteString 和 Data.Text.Text 之间的类型转换功能。

### Data.String.Conversions

```
$ cabal install string-conversions
Installed string-conversions-0.4.0.1
Prelude> :m +Data.String.Conversions
Prelude Data.String.Conversions>
```

Conversions 模块提供以下五种类型之间的类型转换功能。
- String
- Data.ByteString.ByteString

缩写为 StrictByteString 或 SBS
- Data.ByteString.Lazy.ByteString

缩写为 LazyByteString 或 LBS
- Data.Text.Text

缩写为 StrictText 或 ST
- Data.Text.Lazy.Text

缩写为 LazyText 或 LT

```
Prelude Data.String.Conversions> cs "abc" :: ST
"abc"
```
- ConvertibleStrings

ConvertibleStrings 是个类型类，五种字符串类型都被定义成了 ConvertibleStrings 类型类的实例。
- cs :: ConvertibleStrings a b => a -> b

cs 函数根据输入输出类型自动实施五种字符串类型之间的类型转换功能。

### OverloadedStrings

GHC 提供了语言扩展 OverloadedStrings。
- 不使用这个语言扩展，所有的字符串字面量都属于 String 也就是 [Char] 类型。
- 如果使用这个语言扩展，那么所有的字符串字面量就都属于 `IsString a => a` 类型。
- String, ByteString, Text 都是 IsString 类的实例。

```
Module Data.String
class IsString a where
    fromString :: String -> a
```

```
{-# LANGUAGE OverloadedStrings #-}
import qualified Data.Text.IO as T

a :: Text
a = "Hello World"

b :: ByteString
b = "Hello World"

c :: String
c = "Hello World"

main = do putStrLn   "Hello as String!"
          T.putStrLn "Hello as Text!"
```

### Text 是 Monoid

```
Prelude Data.Text> ("abc" :: Text) <> ("def" :: Text)
"abcdef"
```


