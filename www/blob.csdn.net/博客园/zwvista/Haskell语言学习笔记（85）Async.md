# Haskell语言学习笔记（85）Async - zwvista - 博客园

## [Haskell语言学习笔记（85）Async](https://www.cnblogs.com/zwvista/p/9341839.html)

### 安装 async

```
$ cabal install async
async-2.2.1 installed
```

### async / wait / concurrently
- async :: IO a -> IO (Async a)

启动新线程，执行异步操作。
- wait :: Async a -> IO a

等待异步操作完成，并返回值。
- concurrently :: IO a -> IO b -> IO (a, b)

并发（concurrently）运行两个 IO 操作，返回两个结果。

### 示例

安装 http-conduit

```
$ cabal install http-conduit
Installed http-conduit-2.3.2
```

[Sample code to accompany the book "Parallel and Concurrent Programming in Haskell"](https://github.com/simonmar/parconc-examples)

```
module GetURL (getURL) where

import Network.HTTP.Conduit
import Data.ByteString (ByteString)
import qualified Data.ByteString.Lazy as L

getURL :: String -> IO ByteString
getURL url = L.toStrict <$> simpleHttp url
```

一个下载模块 getURL

```
import GetURL

import Control.Concurrent.Async
import qualified Data.ByteString as B

main = do
  a1 <- async (getURL "http://www.wikipedia.org/wiki/Shovel")
  a2 <- async (getURL "http://www.wikipedia.org/wiki/Spade")
  r1 <- wait a1
  r2 <- wait a2
  print (B.length r1, B.length r2) -- (87653,58155)
```
- 使用 async 开启两个线程同时下载两个网页。
- 使用 await 等待下载结束并得到结果。

```
import GetURL

import Control.Concurrent.Async
import qualified Data.ByteString as B

main = do
  (r1, r2) <- concurrently 
    (getURL "http://www.wikipedia.org/wiki/Shovel") 
    (getURL "http://www.wikipedia.org/wiki/Spade")
  print (B.length r1, B.length r2) -- (87653,58155)
```
- 使用 concurrently 开启两个线程同时下载两个网页。下载结束时返回结果。


