# Haskell语言学习笔记（46）Parsec（3） - zwvista - 博客园

## [Haskell语言学习笔记（46）Parsec（3）](https://www.cnblogs.com/zwvista/p/7868767.html)

### Applicative Parsing

使用 Applicative 式的 Parser。

包括使用 (<$>), (<*>), (<$), (<*), (*>), (<|>), many 等运算符。

### 应用实例1

```
import Control.Monad
import Text.Parsec
import Control.Applicative hiding ((<|>))

number = many1 digit
plus = char '+' *> number
minus = (:) <$> char '-' <*> number
integer = plus <|> minus <|> number
float = fmap rd $ (++) <$> integer <*> decimal
    where rd      = read :: String -> Float
          decimal = option "" $ (:) <$> char '.' <*> number

main = forever $ do putStrLn "Enter a float: "
                    input <- getLine
                    parseTest float input
```

```
*Main> main
Enter a float: 
2.3
2.3
Enter a float: 
1
1.0
Enter a float: 
-1
-1.0
Enter a float: 
+6.98
6.98
```
- plus = char '+' *> number

p1 *> p2 依次匹配 p1 和 p2，但是 p1 被舍弃，只返回 p2。
- minus = (:) <$> char '-' <*> number

(:) <$> p1 <*> p2 依次匹配 p1 和 p2，然后联结两次匹配的结果。

这里 p1 匹配的结果是字符，p2 匹配的结果是字符串，所以用 (:) 来联结。
- float = fmap rd $ (++) <$> integer <*> decimal

(++) <$> p1 <*> p2 依次匹配 p1 和 p2，然后联结两次匹配的结果。

这里两次匹配的结果都是字符串，所以用 (++) 来联结。
- decimal = option "" $ (:) <$> char '.' <*> number

option "" p 尝试匹配 p 如果不成功则返回空字符串 ""。

### 应用实例2

```
import Text.Parsec
import Control.Applicative ((<$), (<*), (*>), liftA)
import Data.Char           (chr)

parseCSV :: String -> Either ParseError [[String]]
parseCSV = parse csvp ""

csvp :: Parsec String () [[String]]
csvp = line `endBy` newline <* eof

line :: Parsec String () [String]
line = cell `sepBy1` char ','

cell :: Parsec String () String
cell = cell' <|> many (noneOf ",\n")
    where cell' = between (char '"') (char '"') $ many chr
          chr   = noneOf "\"" <|> try ('"' <$ string "\"\"")
```

```
*Main> parseCSV "a,b,c\n"
Right [["a","b","c"]]
*Main> parseCSV "a,b,\"c\"\n"
Right [["a","b","c"]]
*Main> parseCSV "a,b,\"c\"\"d\"\n"
Right [["a","b","c\"d"]]
```
- csvp = line `endBy` newline <* eof

p1 <* p2 依次匹配 p1 和 p2，但是 p2 被舍弃，只返回 p1。

endBy p sep 匹配0次或多次 p + sep 所组成的序列，返回 p 的 list。
- line = cell `sepBy1` char ','

sepBy1 p sep 匹配 p 后接0次或多次 sep + p 组成的序列，返回 p 的 list。
- chr = noneOf "\"" <|> try ('"' <$ string "\"\"")

x <$ p 匹配 p，但是返回结果为 x。

noneOf s 匹配字符串 s 中的所有字符之外的字符，返回所匹配的字符。

### 参考链接

[Parsing Floats With Parsec](https://www.schoolofhaskell.com/school/to-infinity-and-beyond/pick-of-the-week/parsing-floats-with-parsec)
[A Gentle Introduction to Parsec](https://www.barrucadu.co.uk/posts/etc/2013-05-27-a-gentle-introduction-to-parsec.html)


