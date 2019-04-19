# Haskell语言学习笔记（93）Data.Text - zwvista - 博客园
## [Haskell语言学习笔记（93）Data.Text](https://www.cnblogs.com/zwvista/p/9943837.html)
### Data.Text.Read
```
Prelude> :set -XOverloadedStrings
Prelude> :m +Data.Text.Read
Prelude Data.Text.Read> decimal "123"
Right (123,"")
Prelude Data.Text.Read> decimal "abc"
Left "input does not start with a digit"
Prelude Data.Text.Read> rational "123.4"
Right (123.4,"")
Prelude Data.Text.Read> signed double "-3.4"
Right (-3.4,"")
```
```
decimal :: Integral a => Reader a
type Reader a = IReader Text a
type IReader t a = t -> Either String (a, t)
```
- Data.Text.Read 中的函数用于解析 Text 类型的字符串，将其装换为数值类型。
- decimal 函数负责解析整数值，它接收一个 Text 类型的字符串作为参数，返回 Either 类型的值。
成功时返回一个二元组，字符串所包含的数值以及剩余的字符串
失败时返回错误信息：该输入不以数字开头
- rational 函数与 decimal 相类似，它负责解析有理数的值。
### Data.Text.IO
Data.Text.IO 模块负责 Text 的输入和输出。
```
Prelude> :set -XOverloadedStrings
Prelude> import qualified Data.Text.IO as TIO
Prelude TIO> TIO.putStrLn "Хорошо!"
Хорошо!
Prelude TIO> TIO.putStrLn "哈斯克尔7.6.1的力量"
哈斯克尔7.6.1的力量
Prelude TIO> TIO.putStrLn "感じる"
感じる
```
### Data.Text.Encoding
```
Prelude> :set -XOverloadedStrings
Prelude> :m +Data.Text
Prelude Data.Text> :m +Data.Text.Encoding
Prelude Data.Text Data.Text.Encoding> encodeUtf8 "Хорошо!"
"\208\165\208\190\209\128\208\190\209\136\208\190!"
Prelude Data.Text Data.Text.Encoding> encodeUtf8 "哈斯克尔7.6.1的力量"
"\229\147\136\230\150\175\229\133\139\229\176\148\&7.6.1\231\154\132\229\138\155\233\135\143"
Prelude Data.Text Data.Text.Encoding> encodeUtf8 "感じる"
"\230\132\159\227\129\152\227\130\139"
Prelude Data.Text Data.Text.Encoding> import qualified Data.Text.IO as TIO
Prelude Data.Text Data.Text.Encoding TIO> TIO.putStrLn $ decodeUtf8 "\230\132\159\227\129\152\227\130\139"
感じる
```
