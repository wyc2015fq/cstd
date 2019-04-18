# Haskell语言学习笔记（43）Parsec（2） - zwvista - 博客园

## [Haskell语言学习笔记（43）Parsec（2）](https://www.cnblogs.com/zwvista/p/7858308.html)

### 组合子 1

```
Prelude Text.Parsec Text.Parsec.String> parseTest (count 3 (char 'a')) "aaa"
"aaa"
Prelude Text.Parsec Text.Parsec.String> parseTest (between (char '(') (char ')') anyChar) "(3)"
'3'
Prelude Text.Parsec Text.Parsec.String> parseTest (option ' ' (char 'a')) "b"
' '
Prelude Text.Parsec Text.Parsec.String> parseTest (optionMaybe (char 'a')) "b"
Nothing
Prelude Text.Parsec Text.Parsec.String> parseTest (optionMaybe (char 'b')) "b"
(Just 'b')
Prelude Text.Parsec Text.Parsec.String> parseTest ((,) <$> optional (char 'a') <*> getInput) "b"
((),"b")
Prelude Text.Parsec Text.Parsec.String> parseTest ((,) <$> optional (char 'a') <*> getInput) "a"
((),"")
Prelude Text.Parsec Text.Parsec.String> parseTest (optional (string "ab")) "a"
parse error at (line 1, column 1):
unexpected end of input
expecting "ab"
```
- count n p 匹配 p 指定（n）次数
- between open close p依次匹配 open p close，但是只返回 p。
- option x p 尝试匹配 p，失败时返回 x。
- optionMaybe p 尝试匹配 p，失败时返回 Nothing，成功时返回 Just p。
- optional p 尝试匹配 p，失败时不消耗输入。输入不够时出错。不出错时返回空。
- (,) <$> p <*> getInput 匹配 p，然后用 (,) 运算符连接匹配的结果和剩余的输入。

这里 getInput 函数返回之前匹配完成之后剩余的输入。

### 组合子 2

```
Prelude Text.Parsec Text.Parsec.String> parseTest ((,) <$> skipMany1 (char 'a') <*> getInput) "aab"
((),"b")
Prelude Text.Parsec Text.Parsec.String> parseTest (sepBy (char 'a') (char ',')) "a"
"a"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepBy (char 'a') (char ',')) "a,a,a"
"aaa"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepBy (char 'a') (char ',')) ""
""
Prelude Text.Parsec Text.Parsec.String> parseTest (sepBy (char 'a') (char ',')) "a,"
parse error at (line 1, column 3):
unexpected end of input
expecting "a"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepBy1 (char 'a') (char ',')) ""
parse error at (line 1, column 1):
unexpected end of input
expecting "a"
Prelude Text.Parsec Text.Parsec.String> parseTest (endBy (char 'a') (char ',')) "a,"
"a"
Prelude Text.Parsec Text.Parsec.String> parseTest (endBy (char 'a') (char ',')) "a,a,"
"aa"
Prelude Text.Parsec Text.Parsec.String> parseTest (endBy (char 'a') (char ',')) ""
""
Prelude Text.Parsec Text.Parsec.String> parseTest (endBy (char 'a') (char ',')) "a,a"
parse error at (line 1, column 4):
unexpected end of input
expecting ","
Prelude Text.Parsec Text.Parsec.String> parseTest (endBy1 (char 'a') (char ',')) ""
parse error at (line 1, column 1):
unexpected end of input
expecting "a"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepEndBy (char 'a') (char ',')) "a"
"a"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepEndBy (char 'a') (char ',')) "a,"
"a"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepEndBy (char 'a') (char ',')) "a,a"
"aa"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepEndBy (char 'a') (char ',')) "a,a,"
"aa"
Prelude Text.Parsec Text.Parsec.String> parseTest (sepEndBy (char 'a') (char ',')) ""
""
Prelude Text.Parsec Text.Parsec.String> parseTest (sepEndBy1 (char 'a') (char ',')) ""
parse error at (line 1, column 1):
unexpected end of input
expecting "a"
```
- skipMany1 p 尝试匹配并跳过 p 1次或多次，返回空。
- sepBy p sep 尝试匹配0次或多次以 sep 分隔的 p 的序列（只能以 p 结尾），返回 p 的 list。

sepBy1 p sep 是1次以上的版本。
- endBy p sep 尝试匹配0次或多次以 sep 分隔的 p 的序列（只能以 sep 结尾），返回 p 的 list。

endBy1 p sep 是1次以上的版本。
- sepEndBy p sep 尝试匹配0次或多次以 sep 分隔的 p 的序列（p 或 sep 结尾均可），返回 p 的 list。

sepEndBy1 p sep 是1次以上的版本。

### 组合子 3

```
Prelude Text.Parsec Text.Parsec.String> parseTest eof ""
()
Prelude Text.Parsec Text.Parsec.String> parseTest eof "a"
parse error at (line 1, column 1):
unexpected 'a'
expecting end of input
Prelude Text.Parsec Text.Parsec.String> parseTest (notFollowedBy (char 'a')) "ab"
parse error at (line 1, column 2):
unexpected 'a'
Prelude Text.Parsec Text.Parsec.String> parseTest ((,) <$> notFollowedBy (char 'a') <*> getInput) "ba"
((),"ba")
Prelude Text.Parsec Text.Parsec.String> parseTest (manyTill (char 'a') (char 'b')) "aab"
"aa"
Prelude Text.Parsec Text.Parsec.String> parseTest (manyTill (char 'a') (char 'b')) "aac"
parse error at (line 1, column 3):
unexpected "c"
expecting "b" or "a"
Prelude Text.Parsec Text.Parsec.String> parseTest ((,) <$> lookAhead (char 'a') <*> getInput) "ab"
('a',"ab")
Prelude Text.Parsec Text.Parsec.String> parseTest (lookAhead (char 'a')) "ba"
parse error at (line 1, column 1):
unexpected "b"
expecting "a"
Prelude Text.Parsec Text.Parsec.String> parseTest anyToken "ab"
'a'
```
- eof 匹配结尾
- notFollowedBy p 仅当 p 匹配失败时成功。
- manyTill p end 匹配0次或多次 p，直到匹配 end 成功。返回 p 的 list。
- lookAhead p 尝试匹配 p，但是不消耗输入。
- anyToken 匹配任何一个 token。

### 参考链接

[Intro to Parsing with Parsec in Haskell](http://jakewheat.github.io/intro_to_parsing/#getting-started)


