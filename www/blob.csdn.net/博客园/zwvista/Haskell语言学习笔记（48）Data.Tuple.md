# Haskell语言学习笔记（48）Data.Tuple - zwvista - 博客园

## [Haskell语言学习笔记（48）Data.Tuple](https://www.cnblogs.com/zwvista/p/7878376.html)

### Data.Tuple

```
fst                     :: (a,b) -> a
fst (x,_)               =  x

snd                     :: (a,b) -> b
snd (_,y)               =  y

curry                   :: ((a, b) -> c) -> a -> b -> c
curry f x y             =  f (x, y)

uncurry                 :: (a -> b -> c) -> ((a, b) -> c)
uncurry f p             =  f (fst p) (snd p)

swap                    :: (a,b) -> (b,a)
swap (a,b)              = (b,a)
```

Data.Tuple 模块提供了针对元组的五个函数。
- fst 取二元组的第一个成员。
- snd 取二元组的第二个成员。
- swap 交换二元组的两个成员。
- curry :: ((a, b) -> c) -> a -> b -> c

curry 柯里化，将原来接收二元组的函数改为接收两个参数的柯里化函数。

curry ((x,y) -> x+y) = (\x y -> x+y)
- uncurry :: (a -> b -> c) -> ((a, b) -> c)

uncurry 去柯里化，将原来接收两个参数的柯里化函数改为接收二元组的函数。

uncurry (\x y -> x+y) = ((x,y) -> x+y)

```
Prelude> fst (1,2)
1
Prelude> snd (1,2)
2
Prelude Data.Tuple> curry (\(x,y) -> x+y) 3 4
7
Prelude Data.Tuple> uncurry (\x y -> x+y) (3,4)
7
Prelude> uncurry max (1,2)
2
Prelude> uncurry min (1,2)
1
Prelude Data.Tuple> swap (1,2)
(2,1)
```

### Data.Tuple.All

安装 tuple 模块。

```
$ cabal install tuple
Installed tuple-0.3.0.2
Prelude> :m +Data.Tuple.All
Prelude Data.Tuple.All>
```

tuple 模块提供了对多达32个成员的元组的支持。

同时该模块还引进了单个成员的元组 OneTuple 类型。
- selN 取出元组的第N个成员。
- updN 修改元组的第N个成员。
- curryN, uncurryN 是标准库中curry, uncurry函数针对N个成员的元组的版本。
- sequenceT 是标准库中 sequence 的元组版本。

```
Prelude Data.Tuple.All> sel1 (1,2,3,4)
1
Prelude Data.Tuple.All> sel4 (1,2,3,4)
4
Prelude Data.Tuple.All> upd1 5 (1,2,3,4)
(5,2,3,4)
Prelude Data.Tuple.All> upd4 5 (1,2,3,4)
(1,2,3,5)
Prelude Data.Tuple.All> sel1 (OneTuple 1)
1
Prelude Data.Tuple.All> upd1 5 (OneTuple 1)
OneTuple 5
Prelude Data.Tuple.All> curryN (\(x,y,z) -> x+y+z) 3 4 5
12
Prelude Data.Tuple.All> uncurryN (\x y z -> x+y+z) (3,4,5)
12
Prelude Data.Tuple.All> sequenceT (print 3, print 4)
3
4
((),())
```

### Data.Tuple.Extra

安装 extra 模块。

```
$ cabal install extra
Installed extra-1.6
Prelude> :m +Data.Tuple.Extra
Prelude Data.Tuple.Extra>
```

```
Prelude Data.Tuple.Extra> first succ (1,"test")
(2,"test")
Prelude Data.Tuple.Extra> second reverse (1,"test")
(1,"tset")
Prelude Data.Tuple.Extra> (succ *** reverse) (1,"test")
(2,"tset")
Prelude Data.Tuple.Extra> (succ &&& pred) 1
(2,0)
Prelude Data.Tuple.Extra> dupe 12
(12,12)
Prelude Data.Tuple.Extra> both succ (1,2)
(2,3)
Prelude Data.Tuple.Extra> let a = (1,2,3)
Prelude Data.Tuple.Extra> print $ (fst3 a) + (snd3 a) + (thd3 a)
6
```


