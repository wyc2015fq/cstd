# Haskell语言学习笔记（52）正则表达式 - zwvista - 博客园

## [Haskell语言学习笔记（52）正则表达式](https://www.cnblogs.com/zwvista/p/7894789.html)

### Text.Regex.PCRE.Heavy

```
$ brew install pcre
$ cabal install pcre-heavy
Installed pcre-heavy-1.0.0.2
Prelude> :m +Text.Regex.PCRE.Heavy
Prelude Text.Regex.PCRE.Heavy>
```

```
Prelude Text.Regex.PCRE.Heavy> :set -XQuasiQuotes
Prelude Text.Regex.PCRE.Heavy> :set -XFlexibleContexts
Prelude Text.Regex.PCRE.Heavy> "123-4567-89" =~ [re|\d{3}-(\d{4})-\d{2}|]
True
Prelude Text.Regex.PCRE.Heavy> scan [re|\d{3}-(\d{4})-\d{2}|] "123-4567-89"
[("123-4567-89",["4567"])]
Prelude Text.Regex.PCRE.Heavy> scanRanges [re|\d{3}-(\d{4})-\d{2}|] "123-4567-89"
[((0,11),[(4,8)])]
Prelude Text.Regex.PCRE.Heavy> sub [re|(\d+)-(\d+)-(\d+)|] (\(d1:d2:d3:_) -> d3 ++ "-" ++ d1 ++ "-" ++ d2)  "123-4567-89"
"89-123-4567"
Prelude Text.Regex.PCRE.Heavy> :set -XScopedTypeVariables
Prelude Text.Regex.PCRE.Heavy> sub [re|\d+|] (\(s::String) -> reverse s) "123-4567-89"
"321-4567-89"
Prelude Text.Regex.PCRE.Heavy> gsub [re|\d+|] (\(s::String) -> reverse s) "123-4567-89"
"321-7654-98"
Prelude Text.Regex.PCRE.Heavy> split [re|%(begin|next|end)%|] ("%begin%hello%next%world%end%" :: String)
["","hello","world",""]
```
- (=~) :: (ConvertibleStrings SBS a, ConvertibleStrings a SBS) => a -> Regex -> Bool

(=~) 操作符函数检查字符串是否匹配指定正则表达式。
- scan :: (ConvertibleStrings SBS a, ConvertibleStrings a SBS) => Regex -> a -> [(a, [a])]

scan 函数查找并返回字符串中所有出现的指定正则表达式。
- scanRanges :: (ConvertibleStrings SBS a, ConvertibleStrings a SBS) => Regex -> a -> [((Int, Int), [(Int, Int)])]

scanRanges 函数查找字符串中所有出现的指定正则表达式，返回出现的所有位置。
- sub :: (ConvertibleStrings SBS a, ConvertibleStrings a SBS, RegexReplacement r) => Regex -> r -> a -> a

sub 函数替换字符串中第一次出现的指定正则表达式。
- gsub :: (ConvertibleStrings SBS a, ConvertibleStrings a SBS, RegexReplacement r) => Regex -> r -> a -> a

gsub 函数替换字符串中所有出现的指定正则表达式。
- split :: (ConvertibleStrings SBS a, ConvertibleStrings a SBS) => Regex -> a -> [a]

split 函数使用指定正则表达式分割字符串。


