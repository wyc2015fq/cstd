# PC-GetResult

## URL

GET <https://api.megvii.com/faceid/liveness/v2/get_result> 

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障。

## 说明

此接口提供活体结果反查功能，可以以biz_id为索引对网页端活体检测及比对结果进行反查。

## 参数

| 必选／可选 | 参数           | 类型   | 参数说明                                                     |
| :--------- | :------------- | :----- | :----------------------------------------------------------- |
| 必选       | api_key        | String | 调用此API的api_key；                                         |
| 必选       | api_secret     | String | 调用此API的api_key的secret；                                 |
| 必选       | biz_id         | String | 通过 notify_url 或者 return_url 返回的活体业务编号。         |
| 可选       | get_image_type | Int    | 此参数为可选参数，可在下面三种参数中选择，决定了是否返回活体图像数据：0（默认）：不需要图像1：需要返回最佳活体质量图(image_best)2：需要返回所有活体图像(image_best+image_action) |

## 返回结果

### 返回值说明

返回值说明见“网页端活体检测及比对返回值说明”表格。

### 成功

 

```
{
    "biz_extra_data": "...",
    "biz_id": "1462259748,52b13fb5-8dfb-4537-a62b-a641d5e929f1",
    "biz_no": "cc47190f-5502-44a2-ab74-ea4f0f649f61",
    "images": {
        "image_action1": "data:image/jpeg;base64,...",
        "image_action2": "data:image/jpeg;base64,...",
        "image_action3": "data:image/jpeg;base64,...",
        "image_best": "data:image/jpeg;base64,..."
    },
    "liveness_result": {
        "failure_reason": null,
        "log": "1462259749\n0:A\n2596:M\n3862:E\n8148:P\n9446:O\n",
        "result": "success",
        "version": "MegLive 2.4.0L"
    },
    "request_id": "1462259763,e2d2f8d6-204b-4c43-92ea-1d62b071f83c",
    "time_used": 93,
    "verify_result": {
        "request_id": "1461740007,71eeb124-08f0-47b3-8fc8-ac048cfa1372",
        "result_faceid": {
            "confidence": 68.91876,
            "thresholds": {
                "1e-3": 64,
                "1e-4": 69,
                "1e-5": 74
            }
        },
        "result_ref1": {
            "confidence": 68.91876,
            "thresholds": {
                "1e-5": 74,
                "1e-4": 69,
                "1e-3": 64
            }
        },
        "face_genuineness": {
            "synthetic_face_confidence": 0.88,
            "synthetic_face_threshold": 0.5,
            "mask_confidence": 0.32,
            "mask_threshold": 0.5,
            "screen_replay_confidence": 0.33,
            "screen_replay_threshold": 0.5
        },
        "time_used": 1020
    }
}
 
```

### 失败

 

```
{
    "error_message": "RESULT_NOT_FOUND",
    "request_id": "1462259901,fa79992d-ca61-48de-aa50-ea337c6aad42",
    "time_used": 4
}
```

 

 

### 错误码列表

#### GetResult 特有的 ERROR_MESSAGE

 

| HTTP 状态代码 | 错误信息         | 说明                               |
| :------------ | :--------------- | :--------------------------------- |
| 400           | RESULT_NOT_FOUND | 此错误类型表示传入的业务编号错误。 |

#### 通用的ERROR_MESSAGE

 

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 403           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API）No data source permission. （仅为FaceID Verify API存在，表示使用“有源比对”的方式调用FaceID Verify API，但是没有“有源比对”的权限） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。                                             |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系FaceID客服或商务。 |

 

------

 

# Return_url和Notify_url回调

## 说明

 客户在GetToken方法中传入的 return_url 和 notify_url 会在完成网页端活体检测和比对后，被FaceID回调。其中

1. return_url：会通过Post方法回调，直接通过页面跳转实现；
2. notify_url：会通过Post方法回调，有FaceID后台发起调用；（如果回调地址不是HTTPS的，则推荐进行签名校验）

回调的内容为一个Json数据，具体的内容如下表所示。

## 参数

| 字段 | 类型   | 说明                                                         | 示例                                     |
| :--- | :----- | :----------------------------------------------------------- | :--------------------------------------- |
| data | String | 网页端活体检测及比对返回值，具体见“网页端活体检测及比对返回值说明” |                                          |
| sign | String | 数据段的签名，签名方法为 sign = sha1(API_SECRET + json_data)，其中sha1返回的字符均为小写。 | ac041f49940c5afb2640a251633a8029ae69c1d5 |

 

------

 

# 附：网页端活体检测及比对返回值说明

| return_url | notify_url | get_result | 参数            | 类型   | 说明                                                         | 示例                                                         |
| :--------- | :--------- | :--------- | :-------------- | :----- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| √          | √          | √          | request_id      | String | 每一次网页端活体检测和比对请求的流水号                       | "1462259763,e2d2f8d6-204b-4c43-92ea-1d62b071f83c"            |
| √          | √          | √          | biz_id          | String | 业务流串号，可以用于反查比对结果                             | "1462259748,52b13fb5-8dfb-4537-a62b-a641d5e929f1"            |
| √          | √          | √          | biz_no          | String | 客户业务流水号，会在notify和return时原封不动的返回给客户。   | "cc47190f-5502-44a2-ab74-ea4f0f649f61"格式为客户自定义       |
| √          | √          | √          | biz_extra_data  | String | 在调用notify_url和return_url时会返回的额外数据，用户可以用此接口来传递一些额外信息。 | "..."                                                        |
| √          | √          | √          | liveness_result | Json   | 活体检测结果：log：活体日志；version：活体版本；result：成功或失败；failure_reason：活体失败的原因，如果成功则为nullaction_mixed： 做了错误的动作not_video：检测到活体攻击（也有可能是网络不通畅或动作过快造成的）timeout：活体动作超时quality_check_timeout：照镜子流程超时（120秒）no_input_frame：视频流输入中断（可能是网络中断造成的）interrupt_in_mirro_state：用户在照镜子流程中断了网页端活体检测interrupt_in_action_state：用户在活体做动作时中断了网页端活体检测 | `"liveness_result"``: {``    ``"failure_reason"``: ``null``,``    ``"log"``:``"1462259749\n0:A\n2596:M\n3862:E\n8148:P\n9446:O\n"``,``    ``"result"``: ``"success"``,``    ``"version"``: ``"MegLive 2.4.0L"``}` |
| √          | √          | √          | verify_result   | Json   | 人脸比对结果（返回结果同[Verify接口](http://wiki.megvii-inc.com/display/CS/2.0.1)的返回结果），如果用户选择的 comparison_type 为 -1，或者活体检测没有通过时，该返回为 null。 此处范围值为 verify 的结果。 如果verify请求出现的无响应的情况，则返回如下格式 `{``    ``"error"``: ``"RAW ERROR MESSAGE"``}`` ` `//example``{``    ``"error"``: ``"VERIFY_API_FAILED"``}`具体的错误类型有："VERIFY_API_FAILED"表示调用Verify请求时发生错误，哪些错误是否**计费**请参照Verify错误返回表。"INTERNAL_ERROR"表示服务器发生了内部错误 | "verify_result": {         "request_id": "1461740007,71eeb124-08f0-47b3-8fc8-ac048cfa1372",         "result_faceid": {             "confidence": 68.91876,             "thresholds": {                 "1e-3": 64,                 "1e-4": 69,                 "1e-5": 74             }         },         "result_ref1": {             "confidence": 68.91876,             "thresholds": {                 "1e-5": 74,                 "1e-4": 69,                 "1e-3": 64             }         },         "face_genuineness": {             "synthetic_face_confidence": 0.88,             "synthetic_face_threshold": 0.5,             "mask_confidence": 0.32,             "mask_threshold": 0.5,             "screen_replay_confidence": 0.33,             "screen_replay_threshold": 0.5         },         "id_exceptions": {             "id_attacked": 0,             "id_photo_monochrome": 0         },         "time_used": 1020     } |
|            |            | √          | images          | Json   | 活体检测得到的图像，调用时通过get_image_type来选择，或以jpg编码并用base64字符串返回，或返回为null。image_best：质量最佳的活体图像；image_action[x]：恰好通过活体动作时采集的图像。 | `"images"``: {``    ``"image_action1"``: ``"data:image/jpeg;base64,..."``,``    ``"image_action2"``: ``"data:image/jpeg;base64,..."``,``    ``"image_action3"``: ``"data:image/jpeg;base64,..."``,``    ``"image_best"``: ``"data:image/jpeg;base64,..."``}` |
| √          | √          | √          | time_used       | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           | 100                                                          |

注：以上参数名标红表示为本次更新新增字段

 