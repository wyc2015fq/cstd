
# JSON Schema了解一下？ - 全栈空间 - CSDN博客

2019年03月20日 23:30:33[流浪的狗和一坨屎](https://me.csdn.net/github_38885296)阅读数：360


如果说typescript是JS with types，那json schema就是json with types
![](https://img-blog.csdnimg.cn/2019032022482784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9qaW1teS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
↑↑ 不是很理解这个logo(⊙﹏⊙)
## 为啥会出现json schema？
json schema也是理科生搞出来的一种标准，因为我们程序员没工夫搞这些标准，就都甩给计算机科学家来设计了，如果不好用的话不就可以甩锅了？作为工科生的我们，直接拿来即用，不用考虑信息熵，数据压缩，聚合度，功能性等要素。程序猿很开心。
json schema为静态的json数据结构提供了类型约束以及动态的逻辑运算功能，其实它只是将大家常用的约束操作用统一的数学语言描述了一遍。。。
虽然JSON schmea没有多高深，但是只要你仔细的按着它的教程学一遍，绝对能让你对软件行业有一些新的认识。
## JSON的理论基础：字节与字符
字节与字符不用说了吧，从数据的角度，一切都是字节与字符，字符是字节的抽象应用，其中字节的意义是数字，在此之上人们创造了列表（以数作为键）以及字典（字符作为键）。
所以JSON最伟大的地方在于，它既满足了人类可读也满足了机器可读的需求。json schema本身也是一个json，它所描述的json就是自己的一个instance。
## JSON Schema的应用
应用程序通常需要验证JSON对象，以确保存在所需的属性，并且满足其他约束（例如价格从不低于1美元）。验证通常在*JSON Schema*的上下文中执行。
JSON Schema是一种语义语言，用于定义JSON对象的结构，内容和（在某种程度上）语义。它允许您指定有关对象属性的含义以及对这些属性有效的值的元数据（有关数据的数据）。应用语法语言的结果是描述根据模式有效的JSON对象集的模式（蓝图）。
这个schema提供完整的结构验证，这对自动化测试和验证客户端提交的数据非常有用。
schema还为json写**注释**，因为json本身无法**注释**。
由于是一种语义语言，json schema允许扩展，自定义关键词，就像xml一样。
由于json schema比实例json复杂很多，解析并计算schema也需要专门的包，比如：
> .NET
> [Json.NET Schema](https://www.newtonsoft.com/jsonschema)
> draft-07, -06, -04, -03
> (AGPL-3.0-only)

> [Manatee.Json](https://github.com/gregsdennis/Manatee.Json)
> draft-07, -06, -04
> (MIT)

> C
> [WJElement](https://github.com/netmail-open/wjelement)
> draft-04, -03
> Draft-06+ progress: issue
> [17](https://github.com/netmail-open/wjelement/issues/17#issuecomment-390899432)
> (LGPL-3.0)

> C++
> [f5-json-schema](https://github.com/KayEss/json-schema)
> draft-07
> (Boost Software License 1.0)

> Clojure
> [json-schema](https://github.com/luposlip/json-schema)
> draft-07
> (Eclipse Public License 2.0)

> Elixir
> [Elixir JSON Schema validator](https://github.com/jonasschmidt/ex_json_schema)
> draft-04
> Draft-06+ progress: issue
> [24](https://github.com/jonasschmidt/ex_json_schema/issues/24)
> ; branch
> [multi-draft-support](https://github.com/jonasschmidt/ex_json_schema/tree/multi-draft-support)
> (MIT)

> [JsonXema](https://github.com/hrzndhrn/json_xema)
> draft-07, -06, -04
> (MIT)

> Go
> [gojsonschema](https://github.com/xeipuuv/gojsonschema)
> draft-07, -06, -04
> (Apache 2.0)

> [santhosh-tekuri/jsonschema](https://github.com/santhosh-tekuri/jsonschema)
> draft-07, -06, -04
> (BSD-3-Clause)

> [qri-io/jsonschema](https://github.com/qri-io/jsonschema)
> draft-07
> includes custom validator support, rich error returns (MIT)

> Java
> [everit-org/json-schema](https://github.com/everit-org/json-schema)
> draft-07, -06, -04
> (Apache License 2.0)

> [Justify](https://github.com/leadpony/justify)
> draft-07
> (Apache License 2.0)

> Kotlin
> [Medeia-validator](https://github.com/worldturner/medeia-validator)
> draft-07, -06, -04
> streaming validator for Kotlin and Java clients; works with Jackson and Gson (Apache License 2.0)

> JavaScript
> [ajv](https://github.com/epoberezkin/ajv)
> draft-07, -06, -04
> for Node.js and browsers - supports
> [custom keywords](https://github.com/epoberezkin/ajv-keywords)
> and
> [$data reference](https://github.com/json-schema-org/json-schema-spec/issues/51)
> (MIT)

> [djv](https://github.com/korzio/djv)
> draft-06, -04
> for Node.js and browsers (MIT)

> [vue-vuelidate-jsonschema](https://github.com/mokkabonna/vue-vuelidate-jsonschema)
> draft-06
> (MIT)

> PHP
> [Opis Json Schema](https://github.com/opis/json-schema)
> draft-07, -06
> (Apache License 2.0)

> [Swaggest Json Schema](https://github.com/swaggest/php-json-schema)
> draft-07, -06, -04
> (MIT)

> Python
> [jsonschema](https://github.com/Julian/jsonschema)
> draft-07, -06, -04, -03
> (MIT)

> Ruby
> [JSONSchemer](https://github.com/davishmcclurg/json_schemer)
> draft-07, -06, -04
> (MIT)

> Objective-C
> [DSJSONSchemaValidation](https://github.com/dashevo/JSONSchemaValidation)
> draft-07, -06, -04
> (MIT)

> Web (Online)
> [JSON Schema Validator](https://www.jsonschemavalidator.net/)
> draft-07, -06, -04, -03

> [JSON Schema Lint](http://jsonschemalint.com/)
> draft-06, -04, -03, -02, -01

> Command Line
> [ajv-cli](https://www.npmjs.com/package/ajv-cli)
> draft-07, -06, -04
> (MIT)

> [Polyglottal JSON Schema Validator](https://www.npmjs.com/package/pajv)
> draft-06, -04
> can be used with YAML and many other formats besides JSON (MIT)


## 项目状态
### 自2018年12月16日起更新
> 我们希望在2019年初发布新一轮草案（核心，验证，超模式，相对JSON指针），并积极致力于最后几个PR。虽然草案目前已经过期，但它们仍处于积极发展阶段。该项目由志愿者组成，生活偶尔会忽视IETF到期时间表。
该草案花费的时间超出预期，因为它解决了长期以来对JSON Schema构成挑战的长期问题。这包括构建正式的可扩展性机制，以便我们可以更轻松地绘制一条线来完成Core和Validation规范的内容。
可以在GitHub上跟踪下一组Internet草案的进度。该[草案-08](https://github.com/json-schema-org/json-schema-spec/milestone/6)的里程碑将跟踪草案的演变范围（虽然看[问题＃612](https://github.com/json-schema-org/json-schema-spec/issues/612)为“草案-08”是这个具有里程碑意义的正确名称的讨论）。
### 标准化之路
JSON Schema项目打算将所有四个系列草案都归为RFC状态。目前，我们正在继续改进我们自行发布的互联网草案。下一步将是获得IETF工作组通过的草案。我们正在积极研究如何实现这一目标。
与此同时，可以通过IETF跟踪Internet草案文档的发布：
[JSON Schema（核心）](https://datatracker.ietf.org/doc/draft-handrews-json-schema/)
[JSON模式验证](https://datatracker.ietf.org/doc/draft-handrews-json-schema-validation/)
[JSON Hyper-Schema](https://datatracker.ietf.org/doc/draft-handrews-json-schema-hyperschema/)
[相对JSON指针](https://datatracker.ietf.org/doc/draft-handrews-relative-json-pointer/)
互联网草案在六个月后到期，因此我们的目标是经常发布，以便始终提供一套未过期的草稿。在我们总结每个草案并最终确定文本时，可能会有短暂的差距。
目的，特别是对于已经广泛实施的验证等词汇表，应尽可能保持从草案到草案的兼容性。但是，这些仍然是草案，并且在用户社区验证了足够明确的需求后，可能会发生重大变化。
JSON Schema官网在[http://json-schema.org](http://json-schema.org/)。

## 相关资料

[JSON Schema and Hyper-Schema](http://json-schema.org/)
[Schema and metadata](https://en.wikipedia.org/wiki/JSON#JSON_Schema)
[draft-zyp-json-schema-04](http://tools.ietf.org/html/draft-zyp-json-schema-04)

