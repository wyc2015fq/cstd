# Raw-GetRandomNumber

### 描述

此接口可获取基于视频活体验证过程中，所需要朗读的四位随机数字。

仅当appkey有使用网页验证API服务的视频验证权限时，才可以调用此API，否则返回错误码 403（AUTHORIZATION_ERROR:Denied）

### 调用URL

[https://api.megvii.com/faceid/lite/raw/get_random_number](https://api.megvii.com/faceid/lite/get_random_number)

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

### 调用方法

POST

### 参数

| 必选／可选 | 参数       | 类型   | 参数说明                                                     |
| :--------- | :--------- | :----- | :----------------------------------------------------------- |
| 必选       | api_key    | String | 调用此API的api_key；                                         |
| 必选       | api_secret | String | 调用此API的api_key的secret；                                 |
| 可选       | biz_no     | String | 用于标志一次验证流程，不超过128字符。如果要使用此参数，**强烈建议对一次验证流程中调用的API（比如Raw-GetRandomNumber、Raw-ValidateVideo、Raw-Verify）使用同一个biz_no，对不同的验证流程使用不同的biz_no**。 |

### 返回值说明

API返回的为一个JSON字符串。

| 字段                | 类型   | 说明                                                         |
| :------------------ | :----- | :----------------------------------------------------------- |
| request_id          | String | 用于区分每一次请求的唯一的字符串。除了发生错误404(API_NOT_FOUND)外，此字段必定返回。 |
| time_used           | Int    | 整个请求所花费的时间，单位为毫秒。除了发生错误404(API_NOT_FOUND)外，此字段必定返回。 |
| biz_no              | String | **此参数仅当调用时设置了biz_no参数才返回，但是如果设置了则即使API调用失败也返回，**值与传入的biz_no保持完全一致。 |
| token_random_number | String | **本字段仅调用成功时返回**，仅用于作为Raw-ValidateVideo接口的参数，本字段不超过1024字节。**注意：**Token有效期为24小时 |
| random_number       | String | 用于活体验证朗读时的四位随机数（为了保证唇语识别效果，建议使用我们的推荐数字），格式为"abcd"，其中abcd均为0～9的数字。 |
| error_message       | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不返回。 |

###  

### 成功示例 

```
{
  "time_used": 34,
  "biz_no": "3149525e-2c24-4862-8e9f-92040595f0a4",
  "random_number": "3167",
  "token_random_number": "6747484e7ed6e8e6d6f92d092cb72284",
  "request_id": "1490024799,225715ed-9871-478c-aabb-ec19542593cf"
}
```

 

### 失败示例

```
{
  "error_message" : "AUTHENTICATION_ERROR",
  "request_id" : "1458269099,6bf2627a-7827-410d-b646-41a34e1232e0"",
  "time_used" : 4
}
```

 

 

 

### 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| ------------- | ---------------------------- | ------------------------------------------------------------ |
| 403           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。                                             |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系FaceID客服或商务。 |

 

### 请求命令示例

 

 curl "[https://api.megvii.com/faceid/lite/raw/get_random_number](https://api.megvii.com/faceid/lite/get_random_number)"  -F api_key=<api_key>  -F api_secret=<api_secret>  -F biz_no=<biz_no>