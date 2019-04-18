# Haskell语言学习笔记（45）Profunctor - zwvista - 博客园

## [Haskell语言学习笔记（45）Profunctor](https://www.cnblogs.com/zwvista/p/7863254.html)

### Profunctor

```
class Profunctor p where
  dimap :: (a -> b) -> (c -> d) -> p b c -> p a d
  dimap f g = lmap f . rmap g

  lmap :: (a -> b) -> p b c -> p a c
  lmap f = dimap f id

  rmap :: (b -> c) -> p a b -> p a c
  rmap = dimap id
```

Profunctor（逆变协变函子） 是个类型类。

Profunctor类型类带两个类型参数，第一个逆变，第二个协变。

Profunctor类型类包含三个函数。
- dimap :: (a -> b) -> (c -> d) -> p b c -> p a d

dimap函数同时修改 Profunctor 的两个参数。
- lmap :: (a -> b) -> p b c -> p a c

lmap函数只修改 Profunctor 的第一个参数。
- rmap :: (b -> c) -> p a b -> p a c

rmap函数只修改 Profunctor 的第二个参数。

Profunctor类型类实际上将输入输出的概念一般化。

Profunctor p b c 对于输入 b 是逆变，对于输出 c 则是协变。

### Profunctor 的法则

法则

```
dimap id id ≡ id
lmap id ≡ id
rmap id ≡ id
dimap f g ≡ lmap f . rmap g
```

推论

```
dimap (f . g) (h . i) ≡ dimap g h . dimap f i
lmap (f . g) ≡ lmap g . lmap f
rmap (f . g) ≡ rmap f . rmap g
```

### (->) 是个Profunctor

```
instance Profunctor (->) where
  dimap ab cd bc = cd . bc . ab
  lmap = flip (.)
  rmap = (.)
```

```
Prelude Data.Profunctor> lmap (+2) (*3) $ 4
18
Prelude Data.Profunctor> rmap (+3) (*3) $ 4
15
Prelude Data.Profunctor> dimap (+2) (+3) (*3) $ 4
21
```
- (->) 是个典型的Profunctor，它具有输入和输出。
- lmap (+2) (*3) $ 4 = (4+2)*3 = 18
- rmap (+3) (*3) $ 4 = 4*3+3 = 15
- dimap (+2) (+3) (*3) $ 4 = (4+2)*3+3 = 21
- dimap (+2) (+3) (*3) = (+3) . (*3) . (+2)

### Star 是个 Profunctor

```
newtype Star f d c = Star { runStar :: d -> f c }

instance Functor f => Profunctor (Star f) where
  dimap ab cd (Star bfc) = Star (fmap cd . bfc . ab)
  lmap k (Star f) = Star (f . k)
  rmap k (Star f) = Star (fmap k . f)
```

Star 类型将 Functor 正向提升为 Profunctor
- newtype Star f d c = Star { runStar :: d -> f c }

Star 封装了一个输出为 Functor 的函数： d -> f c。
- dimap ab cd (Star bfc) = Star (fmap cd . bfc . ab)

dimap 函数将第一个函数 ab 应用到函数的输入端，将第二个函数 cd 应用到函数的输出端（通过fmap）。

```
Prelude Data.Profunctor> runStar (dimap (+1) show (Star Just)) 5 
Just "6"
Prelude Data.Profunctor> runStar (dimap (+1) show (Star (\x -> [x, x+1]))) 5 
["6","7"]
```

runStar (dimap (+1) show (Star Just)) 5 = fmap show . Just . (+1) $ 5

### Costar 是个 Profunctor

```
newtype Costar f d c = Costar { runCostar :: f d -> c }

instance Functor f => Profunctor (Costar f) where
  dimap ab cd (Costar fbc) = Costar (cd . fbc . fmap ab)
  lmap k (Costar f) = Costar (f . fmap k)
  rmap k (Costar f) = Costar (k . f)
```

Costar 类型将 Functor 反向提升为 Profunctor。
- newtype Costar f d c = Costar { runCostar :: f d -> c }

Costar 封装了一个输入为 Functor 的函数： f d -> c。
- dimap ab cd (Costar fbc) = Costar (cd . fbc . fmap ab)

dimap 函数将第一个函数 ab 应用到函数的输入端（通过fmap），将第二个函数 cd 应用到函数的输出端。

```
Prelude Data.Profunctor> runCostar (dimap (+1) show (Costar sum)) [1,2,3]
"9"
Prelude Data.Profunctor> runCostar (dimap (+1) show (Costar Data.Maybe.fromJust)) (Just 3)
"4"
```

runCostar (dimap (+1) show (Costar sum)) [1,2,3] = show . sum . fmap (+1) $ [1,2,3]

### Forget 是个 Profunctor

```
newtype Forget r a b = Forget { runForget :: a -> r }

instance Profunctor (Forget r) where
  dimap f _ (Forget k) = Forget (k . f)
  lmap f (Forget k) = Forget (k . f)
  rmap _ (Forget k) = Forget k
```


