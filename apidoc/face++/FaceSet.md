# FaceSet

# GetFaceSets API

## 版本

 3.0

## 描述

获取某一 API Key 下的 FaceSet 列表及其 faceset_token、outer_id、display_name 和 tags 等信息。

注意：2017年8月16日后，调用本接口将不会一次性返回全量的 FaceSet。单次查询最多返回 100 个 FaceSet。如需获取全量数据，需要配合使用 start 和 next 参数。请尽快修改调整您的程序。

## 调用URL

https://api-cn.faceplusplus.com/facepp/v3/faceset/getfacesets

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选 | 参数名     | 类型   | 参数说明                                                     |
| :------- | :--------- | :----- | :----------------------------------------------------------- |
| 必选     | api_key    | String | 调用此API的API Key                                           |
| 必选     | api_secret | String | 调用此API的API Secret                                        |
| 可选     | tags       | String | 包含需要查询的FaceSet标签的字符串，用逗号分隔                |
| 可选     | start      | Int    | 一个数字 n，表示开始返回的 faceset_token 在传入的 API Key 下的序号。通过传入数字 n，可以控制本 API 从第 n 个 faceset_token 开始返回。返回的 faceset_token 按照创建时间排序。每次返回1000个FaceSets。默认值为1。注意：2017年8月16日后，每次返回的 FaceSet 数量上限从 1000 改为 100。并且您可以输入上一次请求本 API 返回的 next 值，用以获得接下来的 100 个 faceset_token。请尽快修改调整您的程序。 |

## 返回值说明

| 字段                        | 类型   | 说明                                                         |
| :-------------------------- | :----- | :----------------------------------------------------------- |
| request_id                  | String | 用于区分每一次请求的唯一的字符串。                           |
| facesets                    | Array  | 该 API Key 下的 FaceSet 信息。包含的元素见下文。注：如果该 API Key 下没有 FaceSet，则返回空数组。 |
| next（2017年8月16日起返回） | String | 用于进行下一次请求。返回值表示排在此次返回的所有 faceset_token 之后的下一个 faceset_token 的序号。如果返回此字段，则说明未返回完此 API Key 下的所有 faceset_token。可以将此字段的返回值，在下一次调用时传入 start 字段中，获取接下来的 faceset_token。如果没有返回该字段，则说明已经返回此 API Key 下的所有 faceset_token。 |
| time_used                   | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message               | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### facesets 数组中单个元素的结构

| 字段          | 类型   | 说明                                  |
| :------------ | :----- | :------------------------------------ |
| faceset_token | String | FaceSet 的标识                        |
| outer_id      | String | 用户提供的FaceSet标识，如果未提供为空 |
| display_name  | String | FaceSet的名字，如果未提供为空         |
| tags          | String | FaceSet的标签，如果未提供为空         |

## 返回值示例

成功请求返回值示例：

```
{
	"time_used": 18,
	"facesets": [{
		"faceset_token": "c0096aad1e5b76a8653f5bfb12ac997d",
		"outer_id": null,
		"display_name": "",
		"tags": ""
	}, {
		"faceset_token": "d203a107c943d47bb0efbb25a16cf84b",
		"outer_id": null,
		"display_name": "",
		"tags": ""
	}],
	"request_id": "1470478468,e288a50c-6606-490f-8c7d-9cf68f6ea496"
}
```

失败请求返回值示例：

```
{
	"time_used": 5,
	"error_message": "INTERNAL_ERROR",
	"request_id": "1469761051,ec285c20-8660-47d3-8b91-5dc2bffa0049"
} 
```

## 当前API特有的ERROR_MESSAGE

无

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/getfacesets" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" 
```







# Delete API

## 版本

 3.0

## 描述

删除一个人脸集合。

## 调用URL

https://api-cn.faceplusplus.com/facepp/v3/faceset/delete

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名        | 类型                  | 参数说明                                                     |
| :------------- | :------------ | :-------------------- | :----------------------------------------------------------- |
| 必选           | api_key       | String                | 调用此API的API Key                                           |
| 必选           | api_secret    | String                | 调用此API的API Secret                                        |
| 必选（二选一） | faceset_token | String                | FaceSet的标识                                                |
| outer_id       | String        | 用户提供的FaceSet标识 |                                                              |
| 可选           | check_empty   | Int                   | 删除时是否检查FaceSet中是否存在face_token，默认值为10：不检查1：检查如果设置为1，当FaceSet中存在face_token则不能删除 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| faceset_token | String | FaceSet的标识                                                |
| outer_id      | String | 用户自定义的FaceSet标识，如果未定义则返回值为空              |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

正确返回结果：

```
{
	"time_used": 43,
	"faceset_token": "fe314f398779ce5b1c5787c7fef3f4fd",
	"outer_id": "",
	"request_id": "1470375034,ed492d83-682f-4029-85fa-954b3a661eee"
}
```

错误返回结果：

```
{
	"time_used": 7,
	"error_message": "FACESET_NOT_EMPTY",
	"request_id": "1470375034,f36d4050-aeeb-4936-b27d-a015255a9a81"
} 
```

## 当前API特有ERROR_MESSAGE

| HTTP状态代码 | 错误信息              | 说明                                           |
| :----------- | :-------------------- | :--------------------------------------------- |
| 400          | INVALID_FACESET_TOKEN | faceset_token不存在                            |
| 400          | INVALID_ OUTER_ID     | outer_id不存在                                 |
| 400          | FACESET_NOT_EMPTY     | FaceSet不为空，不能删除（设置check_empty=1时） |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/delete" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \ 
-F "outer_id=newfaceset"
```

 

 

# GetDetail API

## 版本

 3.0

## 描述

获取一个 FaceSet 的所有信息，包括此 FaceSet 的 faceset_token, outer_id, display_name 的信息，以及此 FaceSet 中存放的 face_token 数量与列表。

注意：2017年8月16日后，调用本接口将不会一次性返回全部的 face_token 列表。单次查询最多返回 100 个 face_token。如需获取全量数据，需要配合使用 start 和 next 参数。请尽快修改调整您的程序。

## 调用URL

https://api-cn.faceplusplus.com/facepp/v3/faceset/getdetail

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选                      | 参数名        | 类型                  | 参数说明                                                     |
| :---------------------------- | :------------ | :-------------------- | :----------------------------------------------------------- |
| 必选                          | api_key       | String                | 调用此API的API Key                                           |
| 必选                          | api_secret    | String                | 调用此API的API Secret                                        |
| 必选(二选一)                  | faceset_token | String                | FaceSet的标识                                                |
| outer_id                      | String        | 用户提供的FaceSet标识 |                                                              |
| 可选（2017年8月16日起可使用） | start         | Int                   | 一个数字 n，表示开始返回的 face_token 在本 FaceSet 中的序号， n 是 [1,10000] 间的一个整数。通过传入数字 n，可以控制本 API 从第 n 个 face_token 开始返回。返回的 face_token 按照创建时间排序，每次返回 100 个 face_token。默认值为 1。您可以输入上一次请求本 API 返回的 next 值，用以获得接下来的 100 个 face_token。 |

## 返回值说明

| 字段                        | 类型   | 说明                                                         |
| :-------------------------- | :----- | :----------------------------------------------------------- |
| request_id                  | String | 用于区分每一次请求的唯一的字符串。                           |
| faceset_token               | String | FaceSet的标识                                                |
| outer_id                    | String | 用户提供的FaceSet标识                                        |
| display_name                | String | 人脸集合的名字                                               |
| user_data                   | String | 自定义用户信息                                               |
| tags                        | String | 自定义标签                                                   |
| face_count                  | Int    | FaceSet中的face_token总数量                                  |
| face_tokens                 | Array  | face_token的数组注：如果该 FaceSet 下没有 face_token，则返回空数组。 |
| next（2017年8月16日起返回） | String | 用于进行下一次请求。返回值表示排在此次返回的所有 face_token 之后的下一个 face_token 的序号。如果返回此字段，则说明未返回完此 FaceSet 下的所有 face_token。可以将此字段的返回值，在下一次调用时传入 start 字段中，获取接下来的 face_token。如果没有返回该字段，则说明已经返回此 FaceSet 下的所有 face_token。 |
| time_used                   | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message               | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

### 正确返回格式：

```
{
	"faceset_token": "d8d1257e3e99a37437ee6ddbbcdcf64e",
	"display_name": "qa_test1470367697.2",
	"face_tokens": ["ad248a809408b6320485ab4de13fe6a9", "3b0327c24c195a7c7937348f26b9e0eb"],
	"time_used": 22,
	"tags": "clouddevelop,megvii,cloudqa",
	"user_data": "rCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbMrCimxzuUlIq4hYbM",
	"face_count": 2,
	"request_id": "1470367720,39be3e5e-0288-4e54-8b6c-b8a03b943e2b",
	"outer_id": ""
} 
```

### 错误返回格式：

```
{
	"time_used": 3,
	"error_message": "INVALID_FACESET_TOKEN",
	"request_id": "1470367720,491b2b4e-7886-4700-82f2-ea003d5922c7"
} 
```

## 当前API特有ERROR_MESSAGE

| HTTP状态代码 | 错误信息              | 说明                 |
| :----------- | :-------------------- | :------------------- |
| 400          | INVALID_FACESET_TOKEN | 无效的 faceset_token |
| 400          | INVALID_OUTER_ID      | outer_id 无效        |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/getdetail" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "outer_id=newfaceset"
```

 

 

 



# Update API

## 版本

 3.0

## 描述

更新一个人脸集合的属性

## 调用URL

https://api-cn.faceplusplus.com/facepp/v3/faceset/update

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名        | 类型                                                         | 参数说明                                                     |
| :------------- | :------------ | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key       | String                                                       | 调用此API的API Key                                           |
| 必选           | api_secret    | String                                                       | 调用此API的API Secret                                        |
| 必选（二选一） | faceset_token | String                                                       | FaceSet的标识                                                |
| outer_id       | String        | 用户自定义的FaceSet标识                                      |                                                              |
| 最少提供一个   | new_outer_id  | String                                                       | 在api_key下全局唯一的FaceSet自定义标识，可以用来管理FaceSet对象。最长255个字符，不能包括字符^@,&=*'" |
| display_name   | String        | 人脸集合的名字，256个字符                                    |                                                              |
| user_data      | String        | 自定义用户信息，不大于16KB, 1KB=1024B 且不能包括字符^@,&=*'" |                                                              |
| tags           | String        | FaceSet自定义标签组成的字符串，用来对FaceSet分组。最长255个字符，多个tag用逗号分隔，每个tag不能包括字符^@,&=*'" |                                                              |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| faceset_token | String | FaceSet的标识                                                |
| outer_id      | String | 用户自定义的FaceSet标识，如果未定义则返回值为空              |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

成功请求返回值示例：

```
{
	"faceset_token": "d203a107c943d47bb0efbb25a16cf84b",
	"request_id": "1470480633,21ce7aa1-02cd-4137-a92b-26697769a18f",
	"time_used": 20,
	"outer_id": "myourterid"
}
```

失败请求返回值示例：

```
{
	"time_used": 5,
	"error_message": "INVALID_OUTER_ID",
	"request_id": "1469761051,ec285c20-8660-47d3-8b91-5dc2bffa0049"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息              | 说明                                 |
| :----------- | :-------------------- | :----------------------------------- |
| 400          | NEW_OUTER_ID_EXIST    | 提供的new_outer_id与已有outer_id重复 |
| 400          | INVALID_FACESET_TOKEN | 无效的faceset_token                  |
| 400          | INVALID_OUTER_ID      | 无效的outer_id                       |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/update" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "outer_id=newfaceset" \
-F "new_outer_id=replacefacesetid" 
```

 

 

 



# RemoveFace API

## 版本

 3.0

## 描述

移除一个FaceSet中的某些或者全部face_token

## 调用URL

 https://api-cn.faceplusplus.com/facepp/v3/faceset/removeface

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名        | 类型                    | 参数说明                                                     |
| :------------- | :------------ | :---------------------- | :----------------------------------------------------------- |
| 必选           | api_key       | String                  | 调用此API的API Key                                           |
| 必选           | api_secret    | String                  | 调用此API的API Secret                                        |
| 必选（二选一） | faceset_token | String                  | FaceSet的标识                                                |
| outer_id       | String        | 用户自定义的FaceSet标识 |                                                              |
| 必选           | face_tokens   | String                  | 需要移除的人脸标识字符串，可以是一个或者多个face_token组成，用逗号分隔。最多不能超过1,000个face_token注：face_tokens字符串传入“RemoveAllFaceTokens”则会移除FaceSet内所有的face_token |

## 返回值说明

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| request_id     | String | 用于区分每一次请求的唯一的字符串。除非发生404（API_NOT_FOUND ) 或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| faceset_token  | String | FaceSet的标识                                                |
| outer_id       | String | 用户自定义的FaceSet标识，如果未定义则返回值为空              |
| face_removed   | Int    | 成功从FaceSet中移除的face_token数量                          |
| face_count     | Int    | 操作完成后FaceSet中的face_token数量                          |
| failure_detail | Array  | 无法从FaceSet中移除的face_token以及原因face_token：人脸标识reason：不能被移除的原因，包括 INVALID_FACE_TOKEN 人脸标识不存在 |
| time_used      | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| error_message  | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

成功请求示例：

```
{
	"faceset_token": "d203a107c943d47bb0efbb25a16cf84b",
	"face_removed": 1,
	"time_used": 927,
	"face_count": 1,
	"request_id": "1470480301,b05e0080-fa65-406e-9b78-64c4387904ee",
	"outer_id": "",
	"failure_detail": []
}
```

失败请求示例：

```
{
	"time_used": 1,
	"error_message": "MISSING_ARGUMENTS: face_tokens",
	"request_id": "1470292158,bac6a318-9393-425a-8187-a393a3e6beb4"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                 | 说明                          |
| :----------- | :----------------------- | :---------------------------- |
| 400          | INVALID_FACESET_TOKEN    | 所传的faceset_token不存在     |
| 400          | INVALID_OUTER_ID         | 所传的outer_id不存在          |
| 400          | INVALID_FACE_TOKENS_SIZE | face_tokens数组长度不符合要求 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。 注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/removeface" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "outer_id=newfaceset" \
-F "face_tokens=c2fc0ad7c8da3af5a34b9c70ff764da0,ad248a809408b6320485ab4de13fe6a9" 
```

 

 





# AddFace API

## 版本

 3.0

## 描述

为一个已经创建的 FaceSet 添加人脸标识 face_token。一个 FaceSet 最多存储1,000个 face_token。

注意：2017年8月16日后，一个 FaceSet 能够存储的 face_token 数量将从 1000 提升至 10000。

## 调用 URL

 https://api-cn.faceplusplus.com/facepp/v3/faceset/addface

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名        | 类型                    | 参数说明                                                     |
| :------------- | :------------ | :---------------------- | :----------------------------------------------------------- |
| 必选           | api_key       | String                  | 调用此API的API Key                                           |
| 必选           | api_secret    | String                  | 调用此API的API Secret                                        |
| 必选（二选一） | faceset_token | String                  | FaceSet 的标识                                               |
| outer_id       | String        | 用户提供的 FaceSet 标识 |                                                              |
| 必选           | face_tokens   | String                  | 人脸标识 face_token 组成的字符串，可以是一个或者多个，用逗号分隔。最多不超过5个face_token |

## 返回值说明

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| request_id     | String | 用于区分每一次请求的唯一的字符串。                           |
| faceset_token  | String | FaceSet 的标识                                               |
| outer_id       | String | 用户自定义的 FaceSet 标识，如果未定义则返回值为空。          |
| face_added     | Int    | 成功加入 FaceSet 的 face_token 数量。                        |
| face_count     | Int    | 操作结束后 FaceSet 中的 face_token 总数量。                  |
| failure_detail | Array  | 无法被加入FaceSet的face_token以及原因face_token：人脸标识reason：不能被添加的原因，包括 INVALID_FACE_TOKEN 人脸标识不存在 ，QUOTA_EXCEEDED 已达到FaceSet存储上限 |
| time_used      | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message  | String | 当请求失败时才会返回此字符串，否则此字段不存在。具体返回内容见后续错误信息章节。 |

## 返回值示例

成功请求示例：

```
{
	"faceset_token": "42fb0d5bf81c5ac57c52344dddc3e7c9",
	"time_used": 479,
	"face_count": 1,
	"face_added": 1,
	"request_id": "1470293555,78637cd1-f773-47c6-8ba4-5af7153e4e00",
	"outer_id": "uabREDWZvshpHISwVsav",
	"failure_detail": []
} 
```

失败请求示例：

```
{
	"time_used": 1,
	"error_message": "MISSING_ARGUMENTS: api_key",
	"request_id": "1470292158,bac6a318-9393-425a-8187-a393a3e6beb4"
} 
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                 | 说明                          |
| :----------- | :----------------------- | :---------------------------- |
| 400          | INVALID_FACESET_TOKEN    | faceset_token无效             |
| 400          | INVALID_ OUTER_ID        | outer_id无效                  |
| 400          | INVALID_FACE_TOKENS_SIZE | face_tokens数组长度不符合要求 |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/addface" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "outer_id=newfaceset" \
-F "face_tokens=c2fc0ad7c8da3af5a34b9c70ff764da0,ad248a809408b6320485ab4de13fe6a9"
```

 

 

#  Create API

## 版本

3.0

## 描述

创建一个人脸的集合 FaceSet，用于存储人脸标识 face_token。一个 FaceSet 能够存储 1,000 个 face_token。

注意：2017年8月16日后，一个 FaceSet 能够存储的 face_token 数量将从 1000 提升至 10000。

## 调用URL

https://api-cn.faceplusplus.com/facepp/v3/faceset/create

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选 | 参数名       | 类型   | 参数说明                                                     |
| :------- | :----------- | :----- | :----------------------------------------------------------- |
| 必选     | api_key      | String | 调用此 API 的 API Key                                        |
| 必选     | api_secret   | String | 调用此 API 的 API Secret                                     |
| 可选     | display_name | String | 人脸集合的名字，最长256个字符，不能包括字符^@,&=*'"          |
| 可选     | outer_id     | String | 账号下全局唯一的 FaceSet 自定义标识，可以用来管理 FaceSet 对象。最长255个字符，不能包括字符^@,&=*'" |
| 可选     | tags         | String | FaceSet 自定义标签组成的字符串，用来对 FaceSet 分组。最长255个字符，多个 tag 用逗号分隔，每个 tag 不能包括字符^@,&=*'" |
| 可选     | face_tokens  | String | 人脸标识 face_token，可以是一个或者多个，用逗号分隔。最多不超过5个 face_token |
| 可选     | user_data    | String | 自定义用户信息，不大于16 KB，不能包括字符^@,&=*'"            |
| 可选     | force_merge  | Int    | 在传入 outer_id 的情况下，如果 outer_id 已经存在，是否将 face_token 加入已经存在的 FaceSet 中0：不将 face_tokens 加入已存在的 FaceSet 中，直接返回 FACESET_EXIST 错误1：将 face_tokens 加入已存在的 FaceSet 中默认值为0 |

## 返回值说明

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| request_id     | String | 用于区分每一次请求的唯一的字符串。除非发生404（API_NOT_FOUND ) 或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| faceset_token  | String | FaceSet 的标识                                               |
| outer_id       | String | 用户自定义的 FaceSet 标识，如果未定义则返回值为空            |
| face_added     | Int    | 本次操作成功加入 FaceSet的face_token 数量                    |
| face_count     | Int    | 操作结束后 FaceSet 中的 face_token 总数量                    |
| failure_detail | Array  | 无法被加入 FaceSet 的 face_token 以及原因face_token：人脸标识reason：不能被添加的原因，包括 INVALID_FACE_TOKEN 人脸表示不存在 ，QUOTA_EXCEEDED 已达到 FaceSet 存储上限 |
| time_used      | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| error_message  | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

正确的返回结果：

```
{
	"faceset_token": "6110df81aff5fd5d20e7d2571b743bac",
	"time_used": 1721,
	"face_count": 5,
	"face_added": 5,
	"request_id": "1470369312,911b9802-732c-4a4c-a3f4-e2735ab59bc0",
	"outer_id": "",
	"failure_detail": [{
		"reason": "INVALID_FACE_TOKEN",
		"face_token": "testtest1"
	}, {
		"reason": "INVALID_FACE_TOKEN",
		"face_token": "testtest2"
	}]
} 
```


错误的返回结果：

```
{
	"time_used": 22,
	"error_message": "BAD_ARGUMENTS: tags",
	"request_id": "1470369557,47326d02-c42f-4b71-a5e0-f848a1443430"
} 
```

## 当前 API 特有的 ERROR_MESSAGE

| HTTP状态代码 | 错误信息                 | 说明                                                         |
| :----------- | :----------------------- | :----------------------------------------------------------- |
| 400          | INVALID_FACE_TOKENS_SIZE | face_tokens 数组长度不符合要求                               |
| 400          | FACESET_EXIST            | 想要创建的 FaceSet 已经存在（当提供了一个当前存储空间内下已存在的 outer_id，且 force_merge 为0时） |
| 400          | FACESET_QUOTA_EXCEEDED   | FaceSet 数量达到上限，不能继续创建 FaceSet                   |

## 通用的 ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key 没有调用本 API 的权限，具体原因为：用户自己禁止该 api_key 调用、管理员禁止该 api_key 调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该 api_key 调用）Denied by Admin（管理员禁止该 api_key 调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该 API Key 的 QPS 已经达到上限。如需要提高 API Key 的 QPS 配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是 json 格式。 |
| 404           | API_NOT_FOUND                | 所调用的 API 不存在。                                        |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/facepp/v3/faceset/create" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "outer_id=newfaceset" \
-F "tag=person,male" \
-F "face_tokens=c2fc0ad7c8da3af5a34b9c70ff764da0,ad248a809408b6320485ab4de13fe6a9"
```











# FaceTaskQuery API

## 版本

 3.0

## 描述

查询之前调用的异步添加/删除人脸请求，异步任务当前的状态

注意：2017年8月16日后，一个 FaceSet 能够存储的 face_token 数量将从 1000 提升至 10000。

##  调用URL

 [https://api-cn.faceplusplus.com/facepp/v3/faceset/async/](https://api-cn.faceplusplus.com/facepp/v3/faceset/async/addface)task_status

## 调用方法

POST

## 权限

不需要单独申请权限。

## 请求参数

| 是否必选 | 参数名     | 类型   | 参数说明                 |
| :------- | :--------- | :----- | :----------------------- |
| 必选     | api_key    | String | 调用此 API 的 API Key    |
| 必选     | api_secret | String | 调用此 API 的 API Secret |
| 必选     | task_id    | String | 异步任务的唯一标识       |

## 返回值说明

异步任务完成的返回

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| request_id     | String | 用于区分每一次请求的唯一的字符串。除非发生 404（API_NOT_FOUND ) 或 403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| task_id        | String | 标示当前异步请求的唯一task标识，之后调用任务状态查询接口时，使用当前值作为参数，如果发生错误，此字段不返回。 |
| status         | Int    | 1: 标示当前异步任务已经完成                                  |
| faceset_token  | String | FaceSet 的标识                                               |
| outer_id       | String | 用户自定义的 FaceSet 标识，如果未定义则返回值为空            |
| face_added     | Int    | 成功加入 FaceSet 的 face_token 数量（如果当前任务类型为添加人脸，返回此字段） |
| face_removed   | Int    | 成功从FaceSet中移除的face_token数量（如果当前任务类型为删除人脸，返回此字段） |
| face_count     | Int    | 操作结束后 FaceSet 中的 face_token 总数量                    |
| failure_detail | Array  | 无法被加入/删除FaceSet的face_token以及原因face_token：人脸标识不存在reason：不能被添加的原因，包括 INVALID_FACE_TOKEN 人脸标识不存在 ，QUOTA_EXCEEDED 已达到FaceSet存储上限 |
| time_used      | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| error_message  | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

异步任务非完成状态的返回

| 字段                | 类型   | 说明                                                         |
| :------------------ | :----- | :----------------------------------------------------------- |
| request_id          | String | 用于区分每一次请求的唯一的字符串。除非发生 404（API_NOT_FOUND ) 或 403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| status              | Int    | -1: 标示当前异步任务失败0：表示当前异步任务未完成            |
| time_used           | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| task_failure_detail | String | 异步任务失败的原因。可能的值为：INVALID_FACESET_TOKENINVALID_FACESET_OUTER_IDINTERNAL_ERROR |
| error_message       | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 本API特有的错误返回值

 

| HTTP状态代码 | 错误信息                 | 说明                                                         |
| ------------ | ------------------------ | ------------------------------------------------------------ |
| 400          | INVALID_TASK_ID          | 所传入的task_id不存在，或者传入的task_id与apikey不匹配       |
| 400          | INVALID_FACESET_TOKEN    | 异步任务所传入的faceset token不存在，或者传入的faceset token不可用 |
| 400          | INVALID_FACESET_OUTER_ID | 异步任务所传入的outer id不存在，或者传入的outer id不可用     |
| 500          | INTERNAL_ERROR           | 异步任务发生服务器内部错误                                   |

## 通用的错误返回值

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |









# AddFaceAsync API

## 版本

 3.0

## 描述

为一个已经创建的 FaceSet 添加人脸标识 face_token。一个 FaceSet 最多存储1,000个 face_token。

注意：2017年8月16日后，一个 FaceSet 能够存储的 face_token 数量将从 1000 提升至 10000。

##  调用URL

 https://api-cn.faceplusplus.com/facepp/v3/faceset/async/addface

## 调用方法

POST

## 权限

不需要单独申请权限。

## 请求参数

| 是否必选       | 参数名        | 类型                    | 参数说明                                                     |
| :------------- | :------------ | :---------------------- | :----------------------------------------------------------- |
| 必选           | api_key       | String                  | 调用此 API 的 API Key                                        |
| 必选           | api_secret    | String                  | 调用此 API 的 API Secret                                     |
| 必选（二选一） | faceset_token | String                  | FaceSet 的标识                                               |
| outer_id       | String        | 用户提供的 FaceSet 标识 |                                                              |
| 必选           | face_tokens   | String                  | 人脸标识 face_token 组成的字符串，可以是一个或者多个，用逗号分隔。最多不超过 5 个 face_token |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。除非发生 404（API_NOT_FOUND ) 或 403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| task_id       | String | 标示当前异步请求的唯一task标识，之后调用任务状态查询接口时，使用当前值作为参数，如果发生错误，此字段不返回。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

## 返回值示例

 

 

 

## 本API特有的错误返回值

 

| HTTP状态代码 | 错误信息                 | 说明                          |
| ------------ | ------------------------ | ----------------------------- |
| 400          | INVALID_FACESET_TOKEN    | faceset_token无效             |
| 400          | INVALID_ OUTER_ID        | outer_id无效                  |
| 400          | INVALID_FACE_TOKENS_SIZE | face_tokens数组长度不符合要求 |
| 503          | TASK_QUEUE_IS_FULL       | 异步任务队列已满              |

 

## 通用的错误返回值

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
| 403           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

 

 

## 调用示例

 

 

 





# RemoveFaceAsync API

## 版本

 3.0

## 描述

移除一个FaceSet中的某些或者全部face_token

## 调用URL

 https://api-cn.faceplusplus.com/facepp/v3/faceset/async/removeface

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

 

## 请求参数

 

| 是否必选       | 参数名        | 类型                    | 参数说明                                                     |
| -------------- | ------------- | ----------------------- | ------------------------------------------------------------ |
| 必选           | api_key       | String                  | 调用此API的API Key                                           |
| 必选           | api_secret    | String                  | 调用此API的API Secret                                        |
| 必选（二选一） | faceset_token | String                  | FaceSet的标识                                                |
| outer_id       | String        | 用户自定义的FaceSet标识 |                                                              |
| 必选           | face_tokens   | String                  | 需要移除的人脸标识字符串，可以是一个或者多个face_token组成，用逗号分隔。最多不能超过1000个face_token注：face_tokens字符串传入“RemoveAllFaceTokens”则会移除FaceSet内所有的face_token |

 

 

## 返回值说明

 

| 字段          | 类型   | 说明                                                         |
| ------------- | ------ | ------------------------------------------------------------ |
| request_id    | String | 用于区分每一次请求的唯一的字符串。除非发生404（API_NOT_FOUND ) 或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| task_id       | String | 标示当前异步请求的唯一task标识，之后调用任务状态查询接口时，使用当前值作为参数，如果发生错误，此字段不返回。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。除非发生404（API_NOT_FOUND )或403 （AUTHORIZATION_ERROR）错误，此字段必定返回。 |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

 

 

 

 

## 返回值示例

 

 

 

## 本API特有的错误返回值

 

| HTTP状态代码 | 错误信息                 | 说明                          |
| ------------ | ------------------------ | ----------------------------- |
| 400          | INVALID_FACESET_TOKEN    | 所传的faceset_token不存在     |
| 400          | INVALID_OUTER_ID         | 所传的outer_id不存在          |
| 400          | INVALID_FACE_TOKENS_SIZE | face_tokens数组长度不符合要求 |
| 503          | TASK_QUEUE_IS_FULL       | 异步任务队列已满              |

 

## 通用的错误返回值

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

 

## 调用示例

 

 

 