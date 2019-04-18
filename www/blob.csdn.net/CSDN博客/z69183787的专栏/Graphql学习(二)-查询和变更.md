# Graphql学习(二)-查询和变更 - z69183787的专栏 - CSDN博客
2019年02月03日 10:27:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：88
这一篇介绍一下GraphQL实际使用时的语法和一些要素
## 字段（Fields）
请求对象上的特定字段，返回结果的数据字段
可以是基本类型（String，Int），也可以是自定义对象类型
|```123456789```|```{  hero {    name    # 查询可以有备注！    friends {      name    }  }}```|
|```1234567891011121314151617```|```{  "data": {    "hero": {      "name": "R2-D2",      "friends": [        {          "name": "Luke Skywalker"        },        {          "name": "Han Solo"        },        {          "name": "Leia Organa"        }      ]    }  }```|
可以看到，请求的时候可以对字段进行层级选择，使客户端可以一次性取的级联关系的相关数据。
## 参数（Arguments）
在字段上还可以增加input参数，比如指定请求ID为1000的某个对象
|```123456```|```{  human(id: "1000") {    name    height  }}```|
|```12345678```|```{  "data": {    "human": {      "name": "Luke Skywalker",      "height": 1.72    }  }}```|
同时参数可以是不同的类型，例子中的id是String。GraphQL提供了一套默认类型，Int、Enum等等（类比于java的基础类型），也可以自己定制类型（java的class），如果参数类型是定制类型的话，可以配合另一个关键词 **Variables** ，以Json形式传入。
## 别名（Aliases）
简单说来就是对字段及返回用任意的名字进行变更
|```12345678```|```{  empireHero: hero(episode: EMPIRE) {    name  }  jediHero: hero(episode: JEDI) {    name  }}```|
|```12345678910```|```{  "data": {    "empireHero": {      "name": "Luke Skywalker"    },    "jediHero": {      "name": "R2-D2"    }  }}```|
原本请求2个hero字段会有冲突，用别名就解决了这个问题
## 操作名称（Operation name）
上面的查询使用了简写，其实都省略了 query 关键字及具体的查询名称。推荐在使用中，定义查询名称减少歧义，而且当传递动态参数及变量时，需要在名称后声明
|```12345678```|```query QueryUsers {  user {    userName    dogs {      dogName    }  }}```|
上例中声明了**操作类型**为query，并且**操作名称**是QueryUsers
GraphQL中操作类型有：**query**（查询）、**mutation**（增删改）、**subscription**（订阅）。
## 变量（Variables）
在**参数**演示了将参数值加在字段上，但很多情况下请求参数都是动态的，如何适配这一场景呢？使用前需要：
- 使用$vname 替代查询中的静态值
- 在操作名称后声明$vname为查询接受的变量之一
- 使用json格式传输变量
|```123456789101112```|```query QueryUsers($vname:String = "zzz") {  user(userName:$vname) {    userName    dogs {      dogName    }  }}Variable：{    "vname":"zzz"}```|
这样通过不同的变量就可以进行动态的查询了。注意也可以在操作名称后的变量后定义默认值。如果想传递一个复杂对象，需要知道Schema的定义，并以对应的json形式传入，这边不做赘述。
## 指令（Directives）
和变量可以动态操作我们的查询一样，指令可以帮助我们动态的改变我们查询的结构。常见于列表视图及详情视图中。
|```12345678910111213```|```query QueryUsers($vname:String = "zzz",$needShow:Boolean) {  user(userName:$vname) {    userName    dogs @include(if: $needShow){      dogName    }  }}Variable：{    "vname":"zzz"    "needShow":false}```|
这样整体的返回结果中就不会包含dogs字段，可以使用的指令有：
@include(if: Boolean) 仅在参数为 true 时，包含此字段。
@skip(if: Boolean) 如果参数为 true，跳过此字段。
## 变更（Mutation）
之前大部分涉及的都是数据查询，GraphQL中约定任何的写入操作通过Mutation来进行操作。和查询一样，如果一个变更操作后需要返回数据，也可以请求字段来作数据返回
|```1234567891011121314151617181920```|```mutation CreateUser($vname: String!, $dogs: DogInput!) {  create(userName: $vname, dogs: $dogs) {    userName    dogs{        dogName    }  }}Variable：{    "vname":"testUserName",    "dogs":[                {                    "dogName":12345                },                {                    "dogName":12345                }            ]}```|
上例中的dog变量并非GraphQL自带类型，它是一个**输入对象类型**，一种特殊的对象类型，可以当做参数传递，可以在Schema中了解更多自定义类型的信息。
注：一次查询中，可以发起多个Query（并行），也可以发起多个Mutation（串行）
## 内联片段（Inline Fragments）
参见官网，用的比较少，如果你查询的字段返回的是接口或者联合类型，那么你可能需要使用内联片段来取出下层具体类型的数据
|```123456789101112131415```|```query HeroForEpisode($ep: Episode!) {  hero(episode: $ep) {    name    ... on Droid {      primaryFunction    }    ... on Human {      height    }  }}Variables:{    "ep":"dasdsad"}```|
如果要请求具体类型上的字段，你需要使用一个类型条件内联片段。因为第一个片段标注为 … on Droid，primaryFunction 仅在 hero 返回的 Character 为 Droid 类型时才会执行。同理适用于 Human 类型的 height 字段。
## 片段（Fragement）
与内连片段类似，通常在查询中如果有大量重复字段的话，会进行使用
|```12345678910111213141516```|```{  leftComparison: hero(episode: EMPIRE) {    ...comparisonFields  }  rightComparison: hero(episode: JEDI) {    ...comparisonFields  }}fragment comparisonFields on Character {  name  appearsIn  friends {    name  }}```|
你可以看到上面的查询如何漂亮地重复了字段。片段的概念经常用于将复杂的应用数据需求分割成小块，特别是你要将大量不同片段的 UI 组件组合成一个初始数据获取的时候。
## 元字段（Meta fields）
顾名思义，有一些内置参数，在我们不知道GraphQL服务会返回的数据结构，数据类型等、
## 总结
介绍了基本的查询语法及可以使用到的一些常用功能，应该不难理解，而且在数据层面也都和一般的Rest API差不多，学习成本较低。下一次说一下我觉得最为重要的Schema。。
