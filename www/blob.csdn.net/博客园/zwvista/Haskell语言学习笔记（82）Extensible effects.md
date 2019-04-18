# Haskell语言学习笔记（82）Extensible effects - zwvista - 博客园

## [Haskell语言学习笔记（82）Extensible effects](https://www.cnblogs.com/zwvista/p/9313782.html)

### 安装 extensible-effects

```
$ cabal install extensible-effects
Installed extensible-effects-3.0.0.0
```

### Extensible effects

Extensible effects 是一种代替 Monad Transformers（转换器）的方法。

```
module Control.Eff.QuickStart
tooBig :: Member (Exc String) r => Int -> Eff r Int
tooBig i = do
  when (i > 100) $ throwError $ show i
  return i

runTooBig :: Int -> Either String Int
runTooBig i = run . runError $ tooBig i
```

```
Prelude> :m +Control.Eff.QuickStart
Prelude Control.Eff.QuickStart> runTooBig 1
Right 1
Prelude Control.Eff.QuickStart> runTooBig 200
Left "200"
```


