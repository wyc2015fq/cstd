# Haskell语言学习笔记（66）Aeson - zwvista - 博客园

## [Haskell语言学习笔记（66）Aeson](https://www.cnblogs.com/zwvista/p/8053819.html)

### Data.Aeson

安装 aeson

```
$ cabal install aeson
Installed aeson-1.2.3.0
Prelude> :m +Data.Aeson
Prelude Data.Aeson>
```

Data.Aeson 是一个处理 JSON 数据的库。

### 处理 AST

```
Prelude Data.Aeson> decode "{\"foo\": 123}" :: Maybe Value
Just (Object (fromList [("foo",Number 123.0)]))
Prelude Data.Aeson> decode "{\"foo\": [\"abc\",\"def\"]}" :: Maybe Value
Just (Object (fromList [("foo",Array [String "abc",String "def"])]))
Prelude Data.Aeson> decode "[1,2,3]" :: Maybe [Int]
Just [1,2,3]
Prelude Data.Aeson> decode "{\"foo\":1,\"bar\":2}" :: Maybe (Data.Map.Map String Int)
Just (fromList [("bar",2),("foo",1)])
```

### 处理 Person（1）

```
{-# LANGUAGE DeriveGeneric #-}

import GHC.Generics
import Data.Aeson
import Data.Text

data Person = Person {
      name :: Text
    , age  :: Int
    } deriving (Generic, Show)

data Persons = Persons {
      persons :: [Person]
    } deriving (Generic, Show)

instance ToJSON Person
instance FromJSON Person

instance ToJSON Persons
instance FromJSON Persons
```

```
*Main> :set -XOverloadedStrings
*Main> encode (Person {name = "Joe", age = 12})
"{\"name\":\"Joe\",\"age\":12}"
*Main> decode "{\"name\":\"Joe\",\"age\":12}" :: Maybe Person
Just (Person {name = "Joe", age = 12})
*Main> encode (Persons {persons = [Person {name = "Joe", age = 12}]})
"{\"persons\":[{\"name\":\"Joe\",\"age\":12}]}"
*Main> decode "{\"persons\":[{\"name\":\"Joe\",\"age\":12}]}" :: Maybe Persons
Just (Persons {persons = [Person {name = "Joe", age = 12}]})
```
- instance ToJSON Persons

要将 Persons 类型的对象序列化成 JSON 字符串，Persons 类型必须是 ToJSON 类型类的实例
- instance FromJSON Persons

要将 JSON 字符串反序列化成 Persons 类型的对象，Persons 类型必须是 FromJSON 类型类的实例

### 处理 Person（2）

```
{-# LANGUAGE OverloadedStrings #-}

import Data.Aeson
import Data.Text

data Person = Person {
      name :: Text
    , age  :: Int
    } deriving Show

data Persons = Persons {
      persons :: [Person]
    } deriving Show

instance FromJSON Person where
    parseJSON = withObject "Person" $ \v -> Person
        <$> v .: "name"
        <*> v .: "age"

instance ToJSON Person where
    toJSON (Person name age) =
        object ["name" .= name, "age" .= age]

instance FromJSON Persons where
    parseJSON = withObject "Persons" $ \v -> Persons
        <$> v .: "persons"

instance ToJSON Persons where
    toJSON (Persons persons) =
        object ["persons" .= persons]
```

```
*Main> :set -XOverloadedStrings
*Main> encode (Person {name = "Joe", age = 12})
"{\"name\":\"Joe\",\"age\":12}"
*Main> decode "{\"name\":\"Joe\",\"age\":12}" :: Maybe Person
Just (Person {name = "Joe", age = 12})
*Main> encode (Persons {persons = [Person {name = "Joe", age = 12}]})
"{\"persons\":[{\"name\":\"Joe\",\"age\":12}]}"
*Main> decode "{\"persons\":[{\"name\":\"Joe\",\"age\":12}]}" :: Maybe Persons
Just (Persons {persons = [Person {name = "Joe", age = 12}]})
```

### 处理 Person（3）

```
{-# LANGUAGE OverloadedStrings #-}

import Data.Aeson
import Data.Text

data Person = Person {
      name :: Text
    , age  :: Int
    } deriving Show

instance FromJSON Person where
    parseJSON = withObject "Person" $ \v -> Person
        <$> v .: "name"
        <*> v .:? "age" .!= 20
```

```
*Main> :set -XOverloadedStrings
*Main> decode "{\"name\":\"Joe\"}" :: Maybe Person
Just (Person {name = "Joe", age = 20})
```

### 处理 Person（4）

```
{-# LANGUAGE DeriveGeneric #-}

import GHC.Generics
import Data.Aeson
import Data.Text (Text)

data Person = Person {
      field_NAME :: Text
    , field_AGE  :: Int
    } deriving (Generic, Show)

customOptions = defaultOptions
                { fieldLabelModifier = drop $ length ("field_" :: String)
                }

instance ToJSON Person where
    toJSON = genericToJSON customOptions
instance FromJSON Person where
    parseJSON = genericParseJSON customOptions
```

```
*Main> :set -XOverloadedStrings
*Main> encode (Person {field_NAME = "Joe", field_AGE = 12})
"{\"AGE\":12,\"NAME\":\"Joe\"}"
*Main> decode "{\"NAME\":\"Joe\",\"AGE\":12}" :: Maybe Person
Just (Person {field_NAME = "Joe", field_AGE = 12})
```


