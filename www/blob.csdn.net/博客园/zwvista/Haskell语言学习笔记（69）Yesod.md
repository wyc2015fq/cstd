# Haskell语言学习笔记（69）Yesod - zwvista - 博客园

## [Haskell语言学习笔记（69）Yesod](https://www.cnblogs.com/zwvista/p/8254299.html)

### Yesod

Yesod 是一个使用 Haskell 语言的 Web 框架。

### 安装 Yesod

首先更新 Haskell Platform 到最新版

（Yesod 依赖的库非常多，版本不一致的话很容易安装失败）

```
$ cabal install yesod
Installed yesod-1.4.5
```

### Hello World

```
-- helloworld.hs

{-# LANGUAGE OverloadedStrings     #-}
{-# LANGUAGE QuasiQuotes           #-}
{-# LANGUAGE TemplateHaskell       #-}
{-# LANGUAGE TypeFamilies          #-}
import           Yesod

data HelloWorld = HelloWorld

mkYesod "HelloWorld" [parseRoutes|
/ HomeR GET
|]
 
instance Yesod HelloWorld
 
getHomeR :: Handler Html
getHomeR = defaultLayout [whamlet|Hello World!|]
 
main :: IO ()
main = warp 3000 HelloWorld
```

```
$ runhaskell helloworld.hs
[Info#yesod-core] Application launched @(yesod-core...
```

在Web Browser里面键入

http://localhost:3000

得到如下网页

```
<!DOCTYPE html>
<html><head><title></title></head><body>Hello World!</body></html>
```


