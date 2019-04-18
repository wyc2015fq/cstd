# Haskell语言学习笔记（83）Pipes - zwvista - 博客园

## [Haskell语言学习笔记（83）Pipes](https://www.cnblogs.com/zwvista/p/9314824.html)

### 安装 pipes

```
$ cabal install pipes
Installed pipes-4.3.9
Prelude> import Pipes
Prelude Pipes> import qualified Pipes.Prelude as P
Prelude Pipes P>
```

### Pipes

Pipes 是一个处理流的库。

```
Prelude Pipes P> runEffect $ P.stdinLn >-> P.takeWhile (/= "quit") >-> P.stdoutLn
Test
Test
Apple
Apple
quit
Prelude Pipes P>
```


