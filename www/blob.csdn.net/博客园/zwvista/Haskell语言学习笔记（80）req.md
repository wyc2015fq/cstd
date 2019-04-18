# Haskell语言学习笔记（80）req - zwvista - 博客园

## [Haskell语言学习笔记（80）req](https://www.cnblogs.com/zwvista/p/9291939.html)

### req

req 是一个好用，类型安全，可扩展，上层的HTTP客户端的库。

```
$ cabal install req
Installed req-1.1.0
Prelude> :m +Network.HTTP.Req
Prelude Network.HTTP.Req>
```

### 官方示例

```
{-# LANGUAGE OverloadedStrings, DeriveGeneric #-}

import Control.Monad
import Control.Monad.IO.Class
import Data.Aeson
import Data.Default.Class
import Data.Maybe (fromJust)
import Data.Monoid ((<>))
import Data.Text (Text)
import GHC.Generics
import Network.HTTP.Req
import qualified Data.ByteString.Char8 as B

main1 :: IO ()
main1 = runReq def $ do
  let n :: Int
      n = 5
  bs <- req GET (https "httpbin.org" /: "bytes" /~ n) NoReqBody bsResponse mempty
  liftIO $ B.putStrLn (responseBody bs)

main2 :: IO ()
main2 = runReq def $ do
  let n, seed :: Int
      n    = 5
      seed = 100
  bs <- req GET (https "httpbin.org" /: "bytes" /~ n) NoReqBody bsResponse $
    "seed" =: seed
  liftIO $ B.putStrLn (responseBody bs)
  
data MyData = MyData
  { size  :: Int
  , color :: Text
  } deriving (Show, Generic)

instance ToJSON MyData
instance FromJSON MyData

main3 :: IO ()
main3 = runReq def $ do
  let myData = MyData
        { size  = 6
        , color = "Green" }
  v <- req POST (https "httpbin.org" /: "post") (ReqBodyJson myData) jsonResponse mempty
  liftIO $ print (responseBody v :: Value)
  
main4 :: IO ()
main4 = runReq def $ do
  let params =
        "foo" =: ("bar" :: Text) <>
        queryFlag "baz"
  response <- req POST (https "httpbin.org" /: "post") (ReqBodyUrlEnc params) jsonResponse mempty
  liftIO $ print (responseBody response :: Value)

main5 :: IO ()
main5 = runReq def $ do
  -- This is an example of what to do when URL is given dynamically. Of
  -- course in a real application you may not want to use 'fromJust'.
  let (url, options) = fromJust (parseUrlHttps "https://httpbin.org/get?foo=bar")
  response <- req GET url NoReqBody jsonResponse $
    "from" =: (15 :: Int)           <>
    "to"   =: (67 :: Int)           <>
    basicAuth "username" "password" <>
    options                         <> -- contains the ?foo=bar part
    port 443 -- here you can put any port of course
  liftIO $ print (responseBody response :: Value)
```

```
*Main> main1
?+???
*Main> main2
J??Y?
*Main> main3
Object (fromList [("origin",String "xx.xxx.xxx.xx"),("args",Object (fromList [])),("json",Object (fromList [("color",String "Green"),("size",Number 6.0)])),("data",String "{\"color\":\"Green\",\"size\":6}"),("url",String "https://httpbin.org/post"),("headers",Object (fromList [("Content-Type",String "application/json; charset=utf-8"),("Accept-Encoding",String "gzip"),("Connection",String "close"),("Host",String "httpbin.org"),("Content-Length",String "26")])),("files",Object (fromList [])),("form",Object (fromList []))])
*Main> main4
Object (fromList [("origin",String "xx.xxx.xxx.xx"),("args",Object (fromList [])),("json",Null),("data",String ""),("url",String "https://httpbin.org/post"),("headers",Object (fromList [("Content-Type",String "application/x-www-form-urlencoded"),("Accept-Encoding",String "gzip"),("Connection",String "close"),("Host",String "httpbin.org"),("Content-Length",String "11")])),("files",Object (fromList [])),("form",Object (fromList [("foo",String "bar"),("baz",String "")]))])
*Main> main5
Object (fromList [("origin",String "xx.xxx.xxx.xx"),("args",Object (fromList [("to",String "67"),("from",String "15"),("foo",String "bar")])),("url",String "https://httpbin.org/get?from=15&to=67&foo=bar"),("headers",Object (fromList [("Authorization",String "Basic dXNlcm5hbWU6cGFzc3dvcmQ="),("Accept-Encoding",String "gzip"),("Connection",String "close"),("Host",String "httpbin.org")]))])
```

### API

```
req
  :: ( MonadHttp    m
     , HttpMethod   method
     , HttpBody     body
     , HttpResponse response
     , HttpBodyAllowed (AllowsBody method) (ProvidesBody body) )
  => method            -- ^ HTTP method
  -> Url scheme        -- ^ 'Url'—location of resource
  -> body              -- ^ Body of the request
  -> Proxy response    -- ^ A hint how to interpret response
  -> Option scheme     -- ^ Collection of optional parameters
  -> m response        -- ^ Response
req method url body Proxy options
```

req 函数实现 Http 请求。

req 函数共有5个参数
- method

GET POST PUT DELETE 之类的 HTTP 方法
- Url scheme

Url 地址，由以下3部分组合而成
- http 或 https
- 域名
- /: + 字符串 或者 /~ + 对象

```
https "httpbin.org" /: "encoding" /: "utf8"
-- https://httpbin.org/encoding/utf8
https "httpbin.org" /: "bytes" /~ (10 :: Int)
-- https://httpbin.org/bytes/10
```

- body

NoReqBody 没有请求体。

ReqBodyJson object 有JSON请求体。（自动将 object 转换成 JSON）
- Proxy response

bsResponse 应答是 ByteString。

jsonResponse 应答是 JSON。（自动将 object 转换成 JSON）
- Option scheme

mempty 没有查询参数。

a := 1 <> b := 2 有两个查询参数。
- m response

最后返回应答类型处于 Monad m 之中。

Monad m 是 MonadHttp 的实例。
- 在指定 ReqBodyJson 以及 jsonResponse 时，需要 Data.Aeson 来定义对象与 JSON 之间的转换。

```
runReq :: MonadIO m
  => HttpConfig        -- ^ 'HttpConfig' to use
  -> Req a             -- ^ Computation to run
  -> m a
runReq config (Req m) = liftIO (runReaderT m config)
```

runReq 函数将 req Monad 中的计算结果取出来。

runReq 函数共有2个参数
- config

Http 配置选项，包括代理服务器，重试次数等。

Data.Default.Class.def 提供了默认选项。

缺省情况下代理服务器的设置从 HTTP_PROXY 和 HTTPS_PROXY 两个环境变量中读取。
- Req m

req Monad，包括 req 函数的调用。
- m a

返回值处在 Monad m 之中。

### 示例2

```
{-# LANGUAGE OverloadedStrings, DeriveGeneric #-}

import Control.Monad
import Control.Monad.IO.Class
import Data.Aeson
import Data.Default.Class
import Data.Text (Text)
import GHC.Generics
import Network.HTTP.Req
import qualified Data.ByteString.Char8 as B

showPostAsString :: Int -> IO ()
showPostAsString n = runReq def $ do
  bs <- req GET (https "jsonplaceholder.typicode.com" /: "posts" /~ n) NoReqBody bsResponse mempty
  liftIO $ B.putStrLn (responseBody bs)

data Post = Post
  { userId  :: Int
  , id :: Int
  , title :: Text
  , body :: Text
  } deriving (Show, Generic)

instance ToJSON Post
instance FromJSON Post

showPostAsJson :: Int -> IO ()
showPostAsJson n = runReq def $ do
  v <- req GET (https "jsonplaceholder.typicode.com" /: "posts" /~ n) NoReqBody jsonResponse mempty
  liftIO $ print (responseBody v :: Post)

showPosts :: Int -> IO ()
showPosts n = runReq def $ do
  v <- req GET (https "jsonplaceholder.typicode.com" /: "posts") NoReqBody jsonResponse mempty
  liftIO $ mapM_ print $ take n (responseBody v :: [Post])

createPost :: IO ()
createPost = runReq def $ do
  let myPost = Post {
    userId = 101,
    Main.id = 102,
    title = "test title",
    body = "test body"
    }
  v <- req POST (https "jsonplaceholder.typicode.com" /: "posts") (ReqBodyJson myPost) jsonResponse mempty
  liftIO $ print (responseBody v :: Post)

updatePost :: Int -> IO ()
updatePost n = runReq def $ do
  let myPost = Post {
    userId = 101,
    Main.id = 0, -- unused
    title = "test title",
    body = "test body"
    }
  v <- req PUT (https "jsonplaceholder.typicode.com" /: "posts" /~ n) (ReqBodyJson myPost) jsonResponse mempty
  liftIO $ print (responseBody v :: Post)

deletePost :: Int -> IO ()
deletePost n = runReq def $ do
  v <- req DELETE (https "jsonplaceholder.typicode.com" /: "posts" /~ n) NoReqBody jsonResponse mempty
  liftIO $ print (responseBody v :: Value)
```

```
*Main> showPostAsString 1
{
  "userId": 1,
  "id": 1,
  "title": "sunt aut facere repellat provident occaecati excepturi optio reprehenderit",
  "body": "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"
}
*Main> showPostAsJson 1
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
*Main> showPosts 2
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
*Main> createPost
Post {userId = 101, id = 102, title = "test title", body = "test body"}
*Main> updatePost 1
Post {userId = 101, id = 1, title = "test title", body = "test body"}
*Main> deletePost 1
Object (fromList [])
```


