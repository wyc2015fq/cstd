# Haskell语言学习笔记（57）Parsec（4） - zwvista - 博客园

## [Haskell语言学习笔记（57）Parsec（4）](https://www.cnblogs.com/zwvista/p/8001592.html)

### Parser 类型

```
data ParsecT s u m a
type Parsec s u = ParsecT s u Identity
type Parser = Parsec String ()
```
- data ParsecT s u m a

ParsecT 带四个类型参数：数据流类型 s，用户状态类型 u，底层Monad类型 m，返回类型 a。

ParsecT 是一个Monad转换器。
- type Parsec s u = ParsecT s u Identity

Parsec 类型是 ParsecT 类型的特化，底层Monad类型特化为 Identity。
- type Parser = Parsec String ()

Parser 类型是 Parsec 类型的特化，数据流类型特化为 String，用户状态类型为空类型 ()。

### parseTest 函数

```
parseTest :: (Stream s Identity t, Show a)
          => Parsec s () a -> s -> IO ()
parseTest p input
    = case parse p "" input of
        Left err -> do putStr "parse error at "
                       print err
        Right x  -> print x
```
- parseTest 函数调用 parse 函数对数据流进行解析。
- 如果匹配失败，打印出错信息。
- 如果匹配成功，输出返回值。


