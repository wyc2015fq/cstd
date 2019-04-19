# RESTful API 设计思路及实践 - 文章 - 伯乐在线
原文出处： [TheAlchemist](http://www.kknotes.com/blog/restful-api%E8%AE%BE%E8%AE%A1%E6%80%9D%E8%B7%AF%E5%8F%8A%E5%AE%9E%E8%B7%B5/)
记得第一次写APP的时候，那时还完全不知道`REST`这个东西，对`Web Service`也是一知半解。我和另一个同学在讨论使用什么协议来交互时，通过各自充分的调研之后（其实就是搜索引擎找一找……），一致认为，`HTTP`这个东西本身就对带宽的消耗这么大了，这么多`Web Service`（当时还是`SOAP`当道）还是基于`HTTP`之上的，这得浪费多少带宽啊。最后一致决定使用`Socket`来通信，现在想想当时也是挺不容易的，我们硬是在`Socket`上搭了一套通信协议，还发展到了第二版。
![](http://ww1.sinaimg.cn/mw690/7cc829d3gw1f838lz9v8tj20k80k7t9u.jpg)
今天在移动应用普及、前后端分离的大浪潮下，`RESTful`风格的API大行其道，可是因为它本身就是一个比较模糊且宽泛的概念，所以每个人对它的理解都有千差万别。我觉得我们在技术选型的时候，在自己的技术积累以及参考已有的行业最佳实践的基础上，应当首先考虑自身系统的需求，思考「选择某一种技术」会对系统的开发和维护带来哪些好处与坏处，而不是人云亦云看着别人用什么自己就用什么。而且`RESTful API`设计目前并没有一个公认的行业最佳实践，故而开发者在设计一个API系统时，更应该根据自身的情况量身定制，千万不要说「我照着某某公司的开放API照搬」就好了。 本文将根据我使用REST的经验来总结一下`RESTful API`设计的一些知识和经验，自勉。本文将不讨论`Oauth`等安全问题。
##### 首先理清一些概念:
- `REST`（Representational State Transfer）
定义了一套基于Web的数据交互方式的设计风格。
- `RESTful`
符合REST风格的API就可以叫做RESTful API。注意，本文讲到的RESTful API设计方法将是基于HTTP和JSON实现方式，但不论HTTP还是JSON都不是REST的标准。REST只是风格，没有标准。
- 动词、`RPC`
在微信里搜索【RESTful API 设计】，出来好多文章都是说怎么在RESTful Uri里使用动词等等，这些人除了一部分是把文章拿来抄一抄的，其他的其实搞混了REST和RPC的概念了，REST强调资源，RPC强调动作，所以REST的Uri组成为名词，RPC多为动词短语。然而它们也并不是完全不相关的两个东西，本文中的实现就会参考一部分`JSON-RPC`的设计思想。
- Web Service
这个是一个更古老的概念，有一套它的理论，不过我更倾向于把它理解成任何基于Web提供的服务。
#### 设计方法及原则：
##### 1. 使用HTTP方法：
HTTP1.1的规范定义了8个动词，然而HTTP作为一个规范并没有被严格地遵守着，在大多数情况下POST是可以完成除任何种类的请求，所以现在很多的API设计都是只是用GET和POST来调用API，在这种情况下，一般的做法是使用GET用来获取资源，其他的行为都是用POST来完成，而为了区别不同的行为，往往在API的Uri中加入动词，如百度推送的如下API：
> 
##### [ `POST` ] /rest/3.0/app/del_tag
#### 功能
删除一个已存在的tag
#### 参数
|参数名|类型|必需|限制|描述|
|----|----|----|----|----|
|tag|string|是|1~128字节，但不能为‘default’|标签名称|
#### 返回值
|名称|类型|描述|
|----|----|----|
|tag|string|标签名称|
|result|number|状态 0：创建成功； 1：创建|
更清晰API设计的可能会使用GET POST PUT DELETE四种方法分别代表“查询、添加、更新、删除”等四个动作，这在概念上是符合HTTP规范的，如Google的如下API：
> 
##### Request
`DELETE`[https://www.googleapis.com/bigquery/v2/projects//datasets/?key={YOUR_API_KEY}](https://www.googleapis.com/bigquery/v2/projects//datasets/?key=%7BYOUR_API_KEY%7D)
##### Response
404 Not Found
– Show headers –
Not Found
在我看来，没有绝对的好与不好。如果使用第一种方法，那么只要保证Uri的语义清晰，其实和使用第二种方法没有太大的区别。
#### 2. `Uri`格式：
`Uri`在REST中标识了一个资源，但是在具体的API设计中，往往不能做到完全的对于资源的映射，本文中的设计将参考比较流行的`Uri`设计，大致有这么几条：
- `Uri`的根(`root`, `/`)应当能够标识这是一个RESTful API，以与同目录下其他可能存在的资源进行区分。
- 紧接着`Uri`的根，应当标识当前API的版本号。
- 如果方法是POST或者PUT，尽量避免使用URL编码的参数，尽量保持Uri的干净。
- 如果方法是DELETE，Uri应当完全标识了需要删除的对象或者对象的集合，避免在DELETE的请求中使用其他参数，因为某些服务器可能会丢弃伴随着DELETE发送的内容。
这里再次拿行业标杆Google的开放API来举例：
> 
`POST`[https://www.googleapis.com/books/v1/mylibrary/annotations](https://www.googleapis.com/books/v1/mylibrary/annotations)
`PUT`[https://www.googleapis.com/bigquery/v2/projects/p1/datasets/p2](https://www.googleapis.com/bigquery/v2/projects/p1/datasets/p2)
`DELETE`[https://www.googleapis.com/bigquery/v2/projects/{project-parameter}/datasets/{datasets-parameter}](https://www.googleapis.com/bigquery/v2/projects/%7Bproject-parameter%7D/datasets/%7Bdatasets-parameter%7D)
#### 3. 固定返回码
`REST`的大部分实现都是一个基于`HTTP`的，那么自然而然就少不了与返回码打交道，然而不幸的是，`HTTP`的返回码定义的看起来十分随意，很多错误信息语焉不详，而且在实际的开发中，API的使用者需要处理链路的问题（如超时等）、种类繁多的`HTTP`返回码、和实际的返回内容，不堪其繁琐。更严重的是，这些返回码大多最终依赖于服务端开发者的具体实现，而这种看似约定的东西分别在客户端和服务端开发者眼中的含义可能相去甚远。
那么从需求入手，我们在使用`RESTful API`时需要使用返回码的原因大致是这样的：客户端在调用一个API之后，需要在接收到的反馈必须要能够标识这次调用是否成功，如果不成功，客户端需要拿到失败的原因。我们可以在API设计时作一个小小的约定，就能完美的满足以上需求了。
服务端在成功接收到客户端的请求之后，永远返回200，具体成功与否及进一步的信息放入返回的内容。
在这个场景中，如果是链路出了问题或者服务器错误等（返回码不等于`200`），客户端很容易就能捕获这个错误，如果链路没问题，那么出错与否在获取到的反馈内容中会有详细的描述。
#### 4. 固定返回结构
现在越来越多的API设计会使用JSON来传递数据，本文中的设计也将使用JSON。`JSON-RPC`是一个基于JSON的广为人知的设计简洁的RPC规范，本文将借鉴`JSON-RPC`的响应对象的设计。
JSON-RPC中服务端响应对象的设计的基本理念是，只要调用成功，服务端必须响应数据（如在#3中讨论的那样），而响应数据的格式在任何情况下都应当是一致的，JSON-RPC的响应格式是这么设计的：


```
{"jsonrpc": "2.0", "result": 19, "id": 1}
{
    "jsonrpc": "2.0", 
    "error": 
        {
            "code": -32600, 
            "message": "Invalid Request"
        }, 
    "id": null
}
```
> 
###### jsonrpc
A String specifying the version of the JSON-RPC protocol. `MUST` be exactly “2.0”.
###### result
This member is `REQUIRED` on success.
This member `MUST NOT` exist if there was an error invoking the method.
The value of this member is determined by the method invoked on the Server.
###### error
This member is `REQUIRED` on error.
This member `MUST NOT` exist if there was no error triggered during invocation.
The value for this member `MUST` be an Object as defined in section 5.1.
###### id
This member is `REQUIRED`.
It `MUST` be the same as the value of the id member in the Request Object.
If there was an error in detecting the id in the Request object (e.g. Parse error/Invalid Request), it `MUST` be Null.
由于`JSON-RPC`的目标是建立一个通用的规范，所以响应格式的设计还是有些复杂，我们可以只取其中它对于`error`对象的设计，所有返回的格式必须是这样的：


```
{
    "code": -32600, 
    "message": "Invalid Request”, 
    “data”:{ }
}
```
这种格式的设计在许多大公司的开放API中也较为常见，比如作为行业标杆的Google，在调用Google开放平台的某API后获取到的错误数据如下，其设计思想与本文讨论的这种返回格式的思想如出一辙。


```
{"error": {
    "errors": [
            {
                "domain": "global",
                "reason": "required",
                "message": "Login Required",
                "locationType": "header",
                "location": "Authorization"
            }
        ],
    "code": 401,
    "message": "Login Required"
    }
}
```
###### 综上所述，本文所探讨的API设计是这样的：
- 所有API的Uri为基于HTTP的名词性短语，用来代表一种资源。
- Uri格式如文中所述。
- 使用GET POST PUT DELETE四种方法分别代表对资源的“查询、添加、更新、删除”。
- 服务端接收到客户端的请求之后，统一返回200，如果客户端获取到的返回码不是200，代表链路上某一个环节出了问题。
- 服务端所有的响应格式为:


```
{    
     “code”: -32600, 
     “message”: “Invalid Request”, 
     “data”:{ }
 }
```
他们的含义分别代表：- code为0代表调用成功，其他会自定义的错误码；
- message表示在API调用失败的情况下详细的错误信息，这个信息可以由客户端直接呈现给用户，否则为空；
- data表示服务端返回的数据，具体格式由服务端自定义，API调用错误为空
