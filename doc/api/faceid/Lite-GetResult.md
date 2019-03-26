# Lite-GetResult

## URL

 GET [https://api.megvii.com/faceid/lite/get_result](https://api.megvii.com/faceid/lite/)

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

## 说明

此接口提供活体结果反查功能，可以以biz_id为索引对 FaceID Lite 验证结果进行反查。

注：该接口的数据存储有效期为三个月，建议在每笔业务结束之后及时的取回数据。

## 参数

| 必选／可选 | 参数         | 类型   | 参数说明                                                     |
| :--------- | :----------- | :----- | :----------------------------------------------------------- |
| 必选       | api_key      | String | 调用此API的api_key；                                         |
| 必选       | api_secret   | String | 调用此API的api_key的secret；                                 |
| 必选       | biz_id       | String | 通过 get_token, notify_url 或者 return_url 返回的活体业务编号。 |
| 可选       | return_image | String | 此参数为可选参数，可在下面三种参数中选择，决定了是否返回活体图像数据：0（默认）：不需要图像1：需要返回最佳活体质量图(image_best，仅当procedure_type为video时有效）2：需要返回身份证人像面图像3：需要返回身份证国徽面图像4：需要返回所有图像5：需要返回正脸自拍照片（仅当procedure_type为selfie时有效）6：需要返回侧脸自拍照片（仅当procedure_type为selfie时有效） |

## 返回结果

### 返回值说明

返回值说明见“FaceID Lite 返回值说明”表格。

### 成功

 

```
{
    "request_id": "1462259763,e2d2f8d6-204b-4c43-92ea-1d62b071f83c",
    "status": "OK",
    "biz_info": {
        "biz_extra_data": "...",
        "biz_id": "1462259748,52b13fb5-8dfb-4537-a62b-a641d5e929f1",
        "biz_no": "cc47190f-5502-44a2-ab74-ea4f0f649f61"
    },
    "idcard_info": {
        "idcard_mode": "1",
        "idcard_uneditable_feild": "idcard_number,idcard_valid_date",
        "idcard_number": "xxxxxx19910602xxxx",
        "idcard_name": "陈AB",
        "idcard_issued_by": "北京市公安局",
        "front_side": {
            "ocr_result": {
                "address": "北京市海淀区XXXX",
                "birthday": {
                    "day": "2",
                    "month": "6",
                    "year": "1991"
                },
                "gender": "男",
                "id_card_number": "xxxxxx19910602xxxx",
                "name": "陈XX",
                "race": "汉",
                "legality": {
                    "ID Photo": 0.855,
                    "Temporary ID Photo ": 0,
                    "Photocopy": 0.049,
                    "Screen": 0.096,
                    "Edited": 0
                }
            },
            "upload_times": 1
        },
        "back_side": {
            "ocr_result": {
                "issued_by": "北京市公安局海淀分局",
                "valid_date": "2010.11.13-2020.11.13",
                "legality": {
                    "ID Photo": 0.855,
                    "Temporary ID Photo ": 0,
                    "Photocopy": 0.049,
                    "Screen": 0.096,
                    "Edited": 0
                }
            },
            "upload_times": 2
        }
    },
    "liveness_result": {
        "procedure_type": "video",
        "result": "PASS/FAIL/TIMEOUT",
        "details": {
            "UPLOAD_TIMES": 5,
            "FACE_NOT_FOUND": 0,
            "LOW_FACE_QUALITY": 0,
            "INVALID_VIDEO_DURATION": 1,
            "SR_ERROR": 2,
            "NOT_SYNCHRONIZED": 1,
            "VIDOE_FORMAT_UNSUPPORTED": 0,
            "NO_AUDIO": 0
        },
        "face_genuineness": {
            "synthetic_face_confidence": 0.88,
            "synthetic_face_threshold": 0.5,
            "mask_confidence": 0.32,
            "mask_threshold": 0.5,
            "screen_replay_confidence": 0,
            "screen_replay_threshold": 0.5
        }
    },
    "verify_result": {
        "result_faceid": {
            "confidence": 68.918,
            "thresholds": {
                "1e-3": 64,
                "1e-4": 69,
                "1e-5": 74,
                "1e-6": 79.9
            }
        },
        "result_ref1": {
            "confidence": 68.918,
            "thresholds": {
                "1e-3": 64,
                "1e-4": 69,
                "1e-5": 74,
                "1e-6": 79.9
            }
        },
        "result_idcard_photo": {
            "confidence": 68.918,
            "thresholds": {
                "1e-3": 64,
                "1e-4": 69,
                "1e-5": 74,
                "1e-6": 79.9
            }
        },
        "result_idcard_datasource": {
            "confidence": 68.918,
            "thresholds": {
                "1e-3": 64,
                "1e-4": 69,
                "1e-5": 74,
                "1e-6": 79.9
            }
        },
        "id_exceptions": {
            "id_attacked": 0,
            "id_photo_monochrome": 0
        }
    },
    "images": {
        "image_idcard_back": "data: image/jpeg;base64,...",
        "image_idcard_front": "data: image/jpeg;base64,...",
        "image_best": "data: image/jpeg;base64,..."
    },
    "time_used": 93
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
| 403           | AUTHORIZATION_ERROR:<reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有：Denied. （没有权限调用当前API） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。                                             |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系FaceID客服或商务。 |

# Return_url 和 Notify_url 回调

## 说明

 客户在GetToken方法中传入的 return_url 和 notify_url 会在完成 FaceID Lite 验证后被系统回调。其中

1. return_url：会通过Post方法回调，直接通过页面跳转实现；
2. notify_url：会通过Post方法回调，有FaceID后台发起调用。出于安全性考虑，FaceID服务对服务器端回调端口有白名单要求，支持的端口有：443，5000，16003，8883，8028；（如果回调地址不是HTTPS的，则推荐进行签名校验）

回调的内容为一个Json数据，具体的内容如下表所示。

## 参数

| 字段 | 类型   | 说明                                                         | 示例                                     |
| :--- | :----- | :----------------------------------------------------------- | :--------------------------------------- |
| data | String | 实名验证返回值，具体见“”FaceID Lite 返回值说明               |                                          |
| sign | String | 数据段的签名，签名方法为 sign = sha1(API_SECRET + json_data)，其中sha1返回的字符均为小写。 | ac041f49940c5afb2640a251633a8029ae69c1d5 |

 

 

------

 

# 附：FaceID Lite 返回值说明

| 参数            | 类型   | 说明                                                         | 示例                                                         |
| :-------------- | :----- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| request_id      | String | API 调用的流水号                                             | "1462259763,e2d2f8d6-204b-4c43-92ea-1d62b071f83c"            |
| status          | String | 表示目前 FaceID Lite 的使用状态：NOT_STARTED：get_token 之后，并没有调用过 do 方法，还没有开始验证；PROCESSING：正在进行 FaceID Lite 验证；OK：完成了 FaceID Lite 验证（OK并不表示通过了实名验证，是流程正常结束）；FAILED：验证流程未完成或出现异常；CANCELLED：用户主动取消了验证流程；TIMEOUT：流程超时。(当处于“NOT_STARTED”或者“PROCESSING”状态时，idcard_info / liveness_result /verify_result字段均不会返回。 | "OK"                                                         |
| biz_info        | Json   | 包含：biz_id, biz_no, biz_extra_databiz_id：业务流串号，可以用于反查比对结果；biz_no：客户业务流水号，会在notify和return时原封不动的返回给客户；biz_extra_data：在调用 notify_url 和 return_url 时会返回的额外数据，用户可以用此接口来传递一些额外信息。 | `{``    ``"biz_extra_data"``: ``"..."``,``    ``"biz_id"``: ``"1462259748,52b13fb5-8dfb-4537-a62b-a641d5e929f1"``,``    ``"biz_no"``: ``"cc47190f-5502-44a2-ab74-ea4f0f649f61"``}` |
| time_used       | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           | 100                                                          |
| idcard_info     | Json   | 身份证识别的结果，此字段在 idcard_mode = 0 时不返回；如果用户中途中断了活体流程，则此字段也不返回。内容包括：idcard_mode：表示 get_token 时选择的 idcard_mode；idcard_uneditable_feild：表示 get_token 是选择的 idcard_uneditable_feild；idcard_mode_user：该字段仅在idcard_mode=4时返回，表示用户选择的身份证信息录入模式0：选择拍摄身份证人像面；1：选择手输身份证信息；idcard_number：表示最终经过用户确认的身份证号 （根据idcard_uneditable_feild可能被修改过）；idcard_name：表示最终经过用户确认的姓名 （根据idcard_uneditable_feild可能被修改过）；idcard_valid_date：表示最终经过用户确认的身份证有效期 （根据idcard_uneditable_field可能被修改过，如果不用拍摄身份证国徽面则不返回此字段）；  idcard_issued_by：表示最终经过用户确认的身份证签发机关 （根据idcard_uneditable_field可能被修改过，如果不用拍摄身份证国徽面则不返回此字段）；front_side：身份证人像面的识别结果（如果不用拍摄身份证人像面则不返回此字段）；ocr_result：文字识别结果，详见【FaceID Ocridcard API】；upload_times：在FaceID Lite验证流程中身份证人像面上传的次数；back_side：身份证国徽面的识别结果（如果不用拍摄身份证国徽面则不返回此字段）；ocr_result：文字识别结果，详见【FaceID Ocridcard API】；upload_times：在FaceID Lite验证流程中身份证国徽面上传的次数；ocr_front_quality：身份证人像面各字段质量判断及逻辑判断结果；（该字段仅在浅色主题下进行返回）ocr_back_quality：身份证国徽面各字段质量判断及逻辑判断结果；（该字段仅在浅色主题下进行返回） | {     "idcard_mode": "1",     "idcard_uneditable_field": "idcard_number,idcard_valid_date",     "idcard_number": "xxxxxx19910602xxxx",     "idcard_name": "陈AB",     "idcard_valid_date": "2010.11.13-2020.11.13",     "idcard_issued_by": "北京市公安局",     "front_side": {         "ocr_result": {             "address": "北京市海淀区XXXX",             "birthday": {                 "day": "2",                 "month": "6",                 "year": "1991"             },             "gender": "男",             "id_card_number": "xxxxxx19910602xxxx",             "name": "陈XX",             "race": "汉",             "legality": {                 "ID Photo": 0.855,                 "Temporary ID Photo ": 0,                 "Photocopy": 0.049,                 "Screen": 0.096,                 "Edited": 0             }         },         "upload_times": 1     },     "back_side": {         "ocr_result": {             "issued_by": "北京市公安局海淀分局",             "valid_date": "2010.11.13-2020.11.13",             "legality": {                 "ID Photo": 0.855,                 "Temporary ID Photo ": 0,                 "Photocopy": 0.049,                 "Screen": 0.096,                 "Edited": 0             }         },         "upload_times": 2     },     "ocr_front_quality": {         "gender": {             "quality": 0.925,             "logic": 0,             "result": "男"         },         "address": {             "quality": 0.958,             "logic": 0,             "result": "xxx"         },         "idcard_number": {             "quality": 0.995,             "logic": 0,             "result": "xxxx"         },         "name": {             "quality": 0.996,             "logic": 0,             "result": "张三"         },         "birth_month": {             "quality": 0.971,             "logic": 0,             "result": "8"         },         "birth_day": {             "quality": 0.975,             "logic": 0,             "result": "31"         },         "nationality": {             "quality": 0.959,             "logic": 0,             "result": "汉"         },         "birth_year": {             "quality": 0.942,             "logic": 0,             "result": "1996"         }     },     "ocr_back_quality": {         "issued_by": {             "quality": 0.994,             "logic": 0,             "result": "xxxxx"         },         "valid_date_end": {             "quality": 0.995,             "logic": 0,             "result": "xxxx"         },         "valid_date_start": {             "quality": 0.995,             "logic": 0,             "result": "20140515"         }     } } |
| liveness_result | Json   | 活体检测结果；如果用户中途中断了活体流程，则此字段不返回。result：活体检测的结果，返回值分为两类：PASS：活体检测通过；FAIL：活体检测失败；details：活体检测结果的细节： **> 当验证流程的procedure_type为"video"时：**UPLOAD_TIMES：<活体视频上传次数>；FACE_NOT_FOUND：<视频中没有检测到人脸的次数>；LOW_FACE_QUALITY：<视频中人脸质量太差的次数>；INVALID_VIDEO_DURATION：<视频时长不对的次数>；SR_ERROR：<语音识别结果有误的次数>；NOT_SYNCHRONIZED：<视频唇语识别错误的次数>；NO_AUDIO：<视频无声音的次数>；VIDEO_FORMAT_UNSUPPORTED：<视频格式错误的次数>；**> 当验证流程的procedure_type取值为"selfie"时：**face_front：FACE_UPLOAD_TIMES：<照片上传次数>；FACE_NOT_FOUND：<照片中没有检测到人脸的次数>；FACE_BLURRED：<人脸太模糊的次数>;FACE_MULTIPLE_FACES：<出现多张人脸的次数>;FACE_POOR_LIGHT：<人脸区过亮或过暗的次数>;FACE_NOT_FRONT：<非正脸照的次数>;FACE_NOT_PROPER_POSITION：<人脸位置不合适的次数>;UNSUPPORTED_FORMAT：<照片格式错误的次数>face_side：FACE_UPLOAD_TIMES：<照片上传次数>FACE_NOT_FOUND：<照片中没有检测到人脸的次数>FACE_BLURRED：<人脸太模糊的次数>FACE_MULTIPLE_FACES：<出现多张人脸的次数>FACE_POOR_LIGHT：<人脸区过亮或过暗的次数>FACE_NOT_SIDE：<非侧脸照的次数>FACE_NOT_PROPER_POSITION：<人脸位置不合适的次数>UNSUPPORTED_FORMAT：<照片格式错误的次数>face_genuineness：表示对假脸攻击的判定，它包含四组置信度和阈值，均为实数取值［0，1］区间。如果一个置信度大于其对应的阈值，则可以认为存在对应类型的假脸攻击。synthetic_face_confidence：<合成脸的置信度>synthetic_face_threshold：<合成脸的阈值>mask_confidence：<面具的置信度>mask_threshold：<面具的阈值>screen_replay_confidence：<屏幕翻拍的置信度>screen_replay_threshold：<屏幕翻拍的阈值>selfie_inconsistent：仅取值"0"或"1"。其中"0"表示两张自拍照片图像内容一致，"1"表示两张自拍照片图像内容不一致。本字段仅procedure_type为selfie时才会出现selfie_metadata_inconsistent：仅取值"0", "1", "2"。其中"0"表示两张照片通过元数据判断，是一致的；"1"表示两张照片元数据不一致；"2"表示照片非jpg格式（这种情况一般只出现用户从相册选择照片上传的情况，这种情况意味照片本来就不是自拍照，无法验证是本人）。本字段仅procedure_type为selfie时且liveness_preferences不为selfie_no_metadata_check时才会出现 | 当procedure_type为"video":  `{``    ``"result"``: ``"PASS/FAIL"``,``    ``"details"``: {``        ``"UPLOAD_TIMES"``: ``5``,``        ``"FACE_NOT_FOUND"``: ``0``,``        ``"LOW_FACE_QUALITY"``: ``0``,``        ``"INVALID_VIDEO_DURATION"``: ``1``,``        ``"SR_ERROR"``: ``2``,``        ``"NOT_SYNCHRONIZED"``: ``1``,``        ``"VIDOE_FORMAT_UNSUPPORTED"``: ``0``,``        ``"NO_AUDIO"``: ``0``    ``},``    ``"face_genuineness"``: {``        ``"synthetic_face_confidence"``: ``0.88``,``        ``"synthetic_face_threshold"``: ``0.5``,``        ``"mask_confidence"``: ``0.32``,``        ``"mask_threshold"``: ``0.5``,``        ``"screen_replay_confidence"``: ``0``,``        ``"screen_replay_threshold"``: ``0.5``    ``}``}`  当procedure_type为"selfie": `{    "result": "PASS/FAIL",    "details": {        "face_front": {            "FACE_NOT_FOUND": 1,            "FACE_BLURRED": 0,            "FACE_MULTIPLE_FACES": 0,            "FACE_POOR_LIGHT": 1,            "FACE_NOT_FRONT": 2,            "FACE_NOT_PROPER_POSITION": 1,            "UNSUPPORTED_FORMAT": 0        },        "face_side": {            "FACE_NOT_FOUND": 1,            "FACE_BLURRED": 0,            "FACE_MULTIPLE_FACES": 0,            "FACE_POOR_LIGHT": 1,            "FACE_NOT_SIDE": 2,            "FACE_NOT_PROPER_POSITION": 1,            "UNSUPPORTED_FORMAT": 0        }    },    "face_genuineness": {        "synthetic_face_confidence": 0.88,        "synthetic_face_threshold": 0.5,        "mask_confidence": 0.32,        "mask_threshold": 0.5,        "screen_replay_confidence": 0,        "screen_replay_threshold": 0.5,        "selfie_inconsistent": 0,        "selfie_metadata_inconsistent": 0    }}`  `` |
| verify_result   | Json   | 人脸比对结果；如果用户中途中断了活体流程，则此字段不返回。error_message：在做人脸验证的时候出现错误null：表示没有出现错误**NO_SUCH_ID_NUMBER**：有源比对时，数据源中没有此身份证号码的记录。**此错误会产生计费**。**ID_NUMBER_NAME_NOT_MATCH**：有源比对时，数据源中存在此身份证 号码，但与提供的姓名不匹配。**此错误会产生计费**。**IMAGE_ERROR_UNSUPPORTED_FORMAT: data_source**：有源比对时，姓名和身份证号已经通过核查。但底图无法解析、或者没有底图。**此错误会产生计费**。DATA_SOURCE_ERROR：有源比对时调用数据源发生错误，一般来说是数 据源出错。出现此错误时建议停止业务，并立即联系FaceID客服或商务，待 确定数据源正常后再开启业务。INTERNAL_ERROR：服务器内部错误，当此类错误发生时请再次请求，如 果持续出现此类错误，请及时联系FaceID客服或商务。result_faceid：活体采集人像与数据源的比对结果；"**confidence**"： 比对结果的置信度，Float类型，取值［0，100］， 数字越大表示两张照片越可能是同一个人。“**thresholds**”：一组用于参考的置信度阈值，Object类型， 包含四个字段，均为Float类型、取值［0，100］：“**1e-3**”：误识率为千分之一的置信度阈值；“**1e-4**”：误识率为万分之一的置信度阈值；“**1e-5**”：误识率为十万分之一的置信度阈值; “**1e-6**”：误识率为百万分之一的置信度阈值。result_ref[x]：活体采集人像与上传的image_ref[x]的比对结果；（同result_faceid）result_idcard_photo：活体采集人像与身份证照片上的人脸比对的结果；（同result_faceid）result_idcard_datasource：身份证照片上的人脸和数据源照片比对的结果；（同result_faceid）id_exceptions：返回身份相关的异常情况，如身份证号码是否曾被冒用来攻击FaceID活体检测、数据源人像照片是否存在质量不佳等问题。调用者可通过此对象增进对比对结果的解读。**本对象仅在有源比对时（comparison_type == 1)返回，**返回包含如下字段：**"id_attacked"：**Int类型，判别身份证号码是否曾被冒用来攻击FaceID活体检测， **取值1表示曾被攻击、取值0表示未被攻击**。攻击形态包括但不限于戴面具、换人 攻击、软件3D合成人脸等手段。若被判别为“是”，则有可能这个身份证号码目前 存在被利用的风险。注意：判别为“是”不意味身份持有者本人参与攻击，有可能 其身份被他人盗用而本人无感知。**"id_photo_monochrome"：**Int类型，数据源人像照片的色彩判断，**取值1表示** **是黑白照片、取值0表示是彩色照片**。数据源存在一部分人像照片是黑白的现象， 黑白的照片会影响比对质量，一般来说将降低比对分数。本字段表达这样的异常。 | `{    "result_faceid": {        "confidence": 68.918,        "thresholds": {            "1e-3": 64,            "1e-4": 69,            "1e-5": 74,            "1e-6": 79.9        }    },    "result_ref1": {        "confidence": 68.918,        "thresholds": {            "1e-3": 64,            "1e-4": 69,            "1e-5": 74,            "1e-6": 79.9        }    },    "result_idcard_photo": {        "confidence": 68.918,        "thresholds": {            "1e-3": 64,            "1e-4": 69,            "1e-5": 74,            "1e-6": 79.9        }    },    "result_idcard_datasource": {        "confidence": 68.918,        "thresholds": {            "1e-3": 64,            "1e-4": 69,            "1e-5": 74,            "1e-6": 79.9        }    },    "id_exceptions": {        "id_attacked": 0,        "id_photo_monochrome": 0    }}` |
| images          | Json   | 活体检测得到的图像，调用时通过 return_image 来选择，或以jpg编码并用base64字符串返回，或返回为null。只有当 status 返回字段为 "OK"  时，图像才会返回。image_best：质量最佳的活体图像；image_idcard_back：最后一次上传的身份证国徽面；image_idcard_front：最后一次上传的身份证人像面。**注意：此字段仅在get_result接口调用时才会返回有图像信息，在notify_url和return_url不返回图片数据信息** | `{``    ``"image_idcard_back"``:``"data:image/jpeg;base64,..."``,``    ``"image_idcard_front"``:``"data:image/jpeg;base64,..."``,``    ``"image_best"``:``"data:image/jpeg;base64,..."``}` |

 注：红色部分为新增字段 

 

 

 

 

 

 

 

 

 