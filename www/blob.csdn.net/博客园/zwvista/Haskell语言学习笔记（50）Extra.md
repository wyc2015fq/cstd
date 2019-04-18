# Haskell语言学习笔记（50）Extra - zwvista - 博客园

## [Haskell语言学习笔记（50）Extra](https://www.cnblogs.com/zwvista/p/7884664.html)

### Extra

安装 extra 模块。

```
$ cabal install extra
Installed extra-1.6
Prelude> :m +Extra
Prelude Extra>
```

### Data.List.Extra String operations

```
Prelude Extra> lower "This is A TEST"
"this is a test"
Prelude Extra> upper "This is A TEST"
"THIS IS A TEST"
Prelude Extra> trim      "  hello   "
"hello"
Prelude Extra> dropEnd 3 "hello"
"he"
Prelude Extra> takeEnd 3 "hello"
"llo"
Prelude Extra> splitAtEnd 3 "hello"
("he","llo")
Prelude Extra Data.Char> spanEnd isUpper "youRE"
("you","RE")
Prelude Extra Data.Char> spanEnd isUpper "youRE"
("you","RE")
Prelude Extra Data.Char> dropWhileEnd isSpace "foo\n"
"foo"
Prelude Extra> takeWhileEnd even [2,3,4,6]
[4,6]
Prelude Extra> stripSuffix "bar" "foobar"
Just "foo"
Prelude Extra> breakOn "::" "a::b::c"
("a","::b::c")
Prelude Extra> breakOnEnd "::" "a::b::c"
("a::b::","c")
Prelude Extra> splitOn "aaa"  "aaaXaaaXaaaXaaa"
["","X","X","X",""]
Prelude Extra> split (== 'a') "aabbaca"
["","","bb","c",""]
Prelude Extra> chunksOf 3 "my test"
["my ","tes","t"]
```

### Data.List.Extra List Basics

```
Prelude Extra> unsnoc [1,2,3]
Just ([1,2],3)
Prelude Extra> drop1 "abc"
"bc"
Prelude Extra> mconcatMap Data.Monoid.Sum [1,2,3]
Sum {getSum = 6}
```

### Data.List.Extra List operations

```
Prelude Extra> groupSort [(1,'t'),(3,'t'),(2,'e'),(2,'s')]
[(1,"t"),(2,"es"),(3,"t")]
Prelude Extra> groupSortOn length ["test","of","sized","item"]
[["of"],["test","item"],["sized"]]
Prelude Extra Data.Function> groupSortBy (compare `on` length) ["test","of","sized","item"]
[["of"],["test","item"],["sized"]]
Prelude Extra> nubOrd "this is a test"
"this ae"
Prelude Extra Data.Function> nubOrdBy (compare `on` length) ["a","test","of","this"]
["a","test","of"]
Prelude Extra> nubOrdOn length ["a","test","of","this"] 
["a","test","of"]
Prelude Extra> nubOn length ["a","test","of","this"] 
["a","test","of"]
Prelude Extra> groupOn length ["test","item","of","sized"]
[["test","item"],["of"],["sized"]]
Prelude Extra> disjoint [1,2,3] [4,5] 
True
Prelude Extra> disjoint [1,2,3] [4,1]
False
Prelude Extra> allSame [1,1,2]
False
Prelude Extra> allSame [1,1,1]
True
Prelude Extra> anySame [1,1,2]
True
Prelude Extra> anySame [1,2,3]
False
Prelude Extra> repeatedly (splitAt 3) "my test"
["my ","tes","t"]
Prelude Extra> for [1,2,3] (+1)
[2,3,4]
Prelude Extra> firstJust id [Nothing,Just 3]
Just 3
Prelude Extra> concatUnzip [("a","AB"),("bc","C")] 
("abc","ABC")
Prelude Extra> concatUnzip3 [("a","AB",""),("bc","C","123")]
("abc","ABC","123")
Prelude Extra> zipFrom 1 "abc"
[(1,'a'),(2,'b'),(3,'c')]
Prelude Extra> zipWithFrom (+) 1 [3,4,5]
[4,6,8]
Prelude Extra> replace "el" "_" "Hello Bella"
"H_lo B_la"
```


