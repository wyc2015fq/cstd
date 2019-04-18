# Haskell语言学习笔记（41）Parsec（1） - zwvista - 博客园

## [Haskell语言学习笔记（41）Parsec（1）](https://www.cnblogs.com/zwvista/p/7841428.html)

### Parsec

Parsec是一个词法及语法分析器。

### 匹配字符与字符串

```
Prelude Text.Parsec> parseTest anyChar "a"
'a'
Prelude Text.Parsec> parseTest (char 'a') "a"
'a'
Prelude Text.Parsec> parseTest (satisfy (=='a')) "a"
'a'
Prelude Text.Parsec> parseTest (oneOf "abc") "a"
'a'
Prelude Text.Parsec> parseTest (string "a") "a"
"a"
```
- anyChar 匹配任意字符
- char c 匹配特定字符 c
- satisfy p 匹配谓词 p
- oneOf s 匹配字符串 s 中的任意字符
- string s 匹配特定字符串 s

### 匹配0次，1次或多次

```
Prelude Text.Parsec> parseTest (many digit) "3"
"3"
Prelude Text.Parsec> parseTest (many digit) ""
""
Prelude Text.Parsec> parseTest (many1 digit) ""
parse error at (line 1, column 1):
unexpected end of input
expecting digit
Prelude Text.Parsec> parseTest (many1 digit) "3"
"3"
Prelude Text.Parsec> parseTest (many1 digit) "34"
"34"
Prelude Text.Parsec> parseTest (many digit) "34"
"34"
```
- many 匹配0次1次或多次
- many1 匹配1次或多次
- digit 匹配数字

### 在 Parser Monad 中改变返回值类型

```
Prelude Text.Parsec Text.Parsec.String> num = do{ n <- many1 digit; return $ read n } :: Parser Integer
Prelude Text.Parsec Text.Parsec.String> parseTest num "123"
123
```

### 忽略空白

```
Prelude Text.Parsec Text.Parsec.String> whitespace = Control.Monad.void $ many $ oneOf " \n\t" :: Parser ()
Prelude Text.Parsec Text.Parsec.String> parseTest (whitespace >> anyChar) "  a"
'a'
Prelude Text.Parsec Text.Parsec.String> lexeme = (\p -> whitespace >> p) :: Parser a -> Parser a
Prelude Text.Parsec Text.Parsec.String> parseTest (lexeme anyChar) "  a"
'a'
```

### sequence 和 choice

```
Prelude Text.Parsec Text.Parsec.String> parseTest (sequence [anyChar, anyChar]) "ab"
"ab"
Prelude Text.Parsec Text.Parsec.String> parseTest (anyChar >>  anyChar) "ab"
'b'
Prelude Text.Parsec Text.Parsec.String> parseTest (choice [anyChar, digit]) "a"
'a'
Prelude Text.Parsec Text.Parsec.String> parseTest (choice [anyChar, digit]) "3"
'3'
Prelude Text.Parsec Text.Parsec.String> parseTest (anyChar <|> digit) "a"
'a'
Prelude Text.Parsec Text.Parsec.String> parseTest (anyChar <|> digit) "3"
'3'
```
- sequence 匹配并且，与 (>>) 操作符有所不同。
- choice 匹配或者，也可用 (<|>) 操作符

### 使用 try 来回溯

```
Prelude Text.Parsec Text.Parsec.String> parseTest (sequence [char 'a', char 'b'] <|> sequence [char 'a', char 'c']) "ac"
parse error at (line 1, column 2):
unexpected "c"
expecting "b"
Prelude Text.Parsec Text.Parsec.String> parseTest (try (sequence [char 'a', char 'b']) <|> sequence [char 'a', char 'c']) "ac"
"ac"
Prelude Text.Parsec Text.Parsec.String> parseTest (string "ab" <|> string "ac") "ac"
parse error at (line 1, column 1):
unexpected "c"
expecting "ab"
Prelude Text.Parsec Text.Parsec.String> parseTest (try (string "ab") <|> string "ac") "ac"
"ac"
```
- 当 choice 的成员parser由多个parser组成时，该成员的某一部分parser成功某一部分parser失败将会导致该成员匹配失败，从而失去匹配其他成员parser的机会。
- 此时应该使用 try 来强制回溯。

### 参考链接

[Intro to Parsing with Parsec in Haskell](http://jakewheat.github.io/intro_to_parsing/#getting-started)


