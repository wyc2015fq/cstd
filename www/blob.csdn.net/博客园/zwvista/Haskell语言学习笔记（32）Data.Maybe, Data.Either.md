# Haskell语言学习笔记（32）Data.Maybe, Data.Either - zwvista - 博客园

## [Haskell语言学习笔记（32）Data.Maybe, Data.Either](https://www.cnblogs.com/zwvista/p/7711359.html)

### Data.Maybe

```
Prelude Data.Maybe> maybe "" show (Just 5)
"5"
Prelude Data.Maybe> maybe "" show Nothing
""
Prelude Data.Maybe> isJust (Just 3)
True
Prelude Data.Maybe> isJust Nothing
False
Prelude Data.Maybe> isNothing (Just 3)
False
Prelude Data.Maybe> isNothing Nothing
True
Prelude Data.Maybe> fromJust (Just 1)
1
Prelude Data.Maybe> 2 * (fromJust Nothing)
*** Exception: Maybe.fromJust: Nothing
Prelude Data.Maybe> fromMaybe "" (Just "Hello, World!")
"Hello, World!"
Prelude Data.Maybe> fromMaybe "" Nothing
""
Prelude Data.Maybe> listToMaybe []
Nothing
Prelude Data.Maybe> listToMaybe [1,2,3]
Just 1
Prelude Data.Maybe> maybeToList (Just 7)
[7]
Prelude Data.Maybe> maybeToList Nothing
[]
Prelude Data.Maybe> catMaybes [Just 1, Nothing, Just 3]
[1,3]
Prelude Data.Maybe> let readMaybeInt = Text.Read.readMaybe :: String -> Maybe Int
Prelude Data.Maybe> mapMaybe readMaybeInt ["1", "Foo", "3"]
[1,3]
```

### Data.Either

```
Prelude Data.Either> either length (*2) $ Left "foo"
3
Prelude Data.Either> either length (*2) $ Right 3
6
Prelude Data.Either> lefts [ Left "foo", Right 3, Left "bar", Right 7, Left "baz" ]
["foo","bar","baz"]
Prelude Data.Either> rights [ Left "foo", Right 3, Left "bar", Right 7, Left "baz" ]
[3,7]
Prelude Data.Either> isLeft (Left "foo")
True
Prelude Data.Either> isLeft (Right 3)
False
Prelude Data.Either> isRight (Left "foo")
False
Prelude Data.Either> isRight (Right 3)
True
Prelude Data.Either> fromLeft 1 (Left 3)
3
Prelude Data.Either> fromLeft 1 (Right "foo")
1
Prelude Data.Either> fromRight 1 (Right 3)
3
Prelude Data.Either> fromRight 1 (Left "foo")
1
Prelude Data.Either> partitionEithers [ Left "foo", Right 3, Left "bar", Right 7, Left "baz" ]
(["foo","bar","baz"],[3,7])
```


