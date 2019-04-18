# Graphql学习(三)-Schema和类型 - z69183787的专栏 - CSDN博客
2019年02月03日 10:33:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：66
这一篇介绍一下构建GraphQL的类型及定义，会被GraphQL整体结构有个大致的了解
为了能够准确描述每一个查询会返回的结果、对象、字段，故引入了Schema。
Schema的主要用途是定义所有可供查询的字段（field），它们最终组合成一套完整的GraphQL API.
## 类型系统
每一个GraphQL服务都会定义一套类型，来描述服务可能返回的所有数据。当执行一个查询时，服务会根据定义的**Schema**验证并执行查询。
## 类型语言
GraphQL 服务可以用任何语言编写，因为我们并不依赖于任何特定语言的句法句式（譬如 JavaScript）来与 GraphQL schema 沟通，我们定义了自己的简单语言，称之为 “GraphQL schema language” —— 它和 GraphQL 的查询语言很相似，让我们能够和 GraphQL schema 之间可以无语言差异地沟通。
## 对象类型和字段（Object Types and Fields）
对象类型定义：
|```12345```|```type Person{  name:String!  age:Int  dpgs:[dog]!}```|
- Person是一个GraphQL对象类型，表示一个拥有一些字段的类型，类比于Java的Class
- name和age是Person类型上的字段，代表在Person类型中的查询只能有name和age
- String!是内置变量，!代表非空
- dogs代表是一个dog类型的数组,也可以用非空表示
## 参数（Arguments）
可以在对象类型的字段上定义参数，比如
|```123```|```type Person{  weight:(unit:weightUnit=KG):Int}```|
weight定义一个参数unit，可以定义一个默认值，当查询没有传递时，会被设置为KG
## 查询和变更类型（Query and Mutation Types）
Schema里除了普通的对象类型外，有2个特殊类型：
|```1234```|```schema {  query: Query  mutation: Mutation}```|
这2个类型定义了每一个GraphQL查询的入口，如下一个查询
|```12345```|```query{  person(id:"2222"){    name  }}```|
标识这个GraphQL服务需要一个Query类型，并且有person字段：
|```123```|```type Query{  person(id: ID): Person}```|
同样的变更（Mutation）操作也类似。
## 标量类型（Scalar Types）
一个对象类型有自己的名字和字段，这些字段在一定情况下回解析到具体数据。这就是标量类型的来源：表示对应GraphQL查询的叶子节点。标量类型没有次级字段，是整棵查询树的叶子节点。后面会提到，查询字段数据的执行器直到遇到标量类型才会停止继续下挖。
GraphQL自带一组默认标量类型：
- Int、Float、String、Boolean 都与其他语言雷同
- ID：代表一个唯一标识符，序列化方式与String一致
	我们可以使用GraphQL服务自定义标量类型，比如Date类型
	scalar Date，实现中需要保证序列化、反序列化、验证方式都是可逆的
## 其他类型
- 枚举 enum
- 列表和为空 [xxx] !
- 接口（Interfaces）：类比Java的interface，一个抽象类型，包含某些字段可以被实现
	|```123456789```|```interface animal{  id:ID!  name:String}type human implements human{  id:ID!  name:String  think:String}```|
当要返回一组对象，但类型不同时，可以使用，但需要配合内联片段（…）。
- 联合类型（union search = Male|Female|Child）可以返回对象类型的组合
- 输入类型（Input Types）
	有时候你需要在mutation操作中传入一个复杂对象，比如在Rest API中常用的json对象。输入对象看上去和常规对象一样，只是关键字不同
	|```1234```|```input ActInput{  actId:Int  actName:String!}```|
你可以在变更操作中使用Input Type
|```12345678910111213141516171819```|```mutation createAct($act:ActInput!){  create(act:$act){    actId    actName  }}Variable：{  "actId":123,  "actName":"asdsadsa"}{  "data":{    "create":{      "actId":123,      "actName":"asdsadsa"    }  }}```|
在schema中不能混淆输入与输出对象类型。
## 验证
当我们查询一个字段时，如果该字段的返回值不是标量类型或者枚举类型，那么我们就需要显示的指名需要从这个字段中获取的具体数据。
|```123```|```{  person}```|
person是一个对象类型，如果不指名其中的name或者其他标量类型字段，这个查询就是无效的。
|```12345678910111213```|```{  "errors": [    {      "message": "Field \"person\" of type \"Person\" must have a selection of subfields. Did you mean \"person { ... }\"?",      "locations": [        {          "line": 3,          "column": 3        }      ]    }  ]}```|
整体来说，GraphQL的验证规则及语义十分繁琐，这方面的问题可以在实际使用中再去深入了解
## 总结
这一章简单介绍了Schema的创建及一些GraphQL中的基础类型，为后续服务端编写提供了基础
