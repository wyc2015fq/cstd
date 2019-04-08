# Recognize Text API (V1)

# 版本

1.0

## 描述

调用者提供图片文件或者图片URL，进行图片分析，找出图片中出现的文字信息。

### 图片要求：

图片格式：JPG(JPEG)，PNG

图片像素尺寸：最小48*48像素，最大800*800像素

图片文件大小：2MB

### 更新日志

2017年3月28日：支持base64编码的图片。

## 调用URL

https://api-cn.faceplusplus.com/imagepp/v1/recognizetext

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名     | 类型                                                         | 参数说明              |
| :------------- | :--------- | :----------------------------------------------------------- | :-------------------- |
| 必选           | api_key    | String                                                       | 调用此API的API Key    |
| 必选           | api_secret | String                                                       | 调用此API的API Secret |
| 必选（三选一） | image_url  | String                                                       | 图片的URL             |
| image_file     | File       | 一个图片，二进制文件，需要用post multipart/form-data的方式上传。 |                       |
| image_base64   | String     | base64编码的二进制图片数据如果同时传入了image_url、image_file和image_base64参数，本API使用顺序为image_file优先，image_url最低。 |                       |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| result        | Object | 被检测出的文字信息，由一个或多个data对象组成。注：如果没有检测出文字则为空 |
| image_id      | String | 被检测的图片在系统中的标识                                   |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### data对象的元素结构

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| type          | String | data对象的类型，包括：document：文档paragraph：段落textline：一行文字word：词character: 字符 |
| value         | String | 识别出的文字，以UTF-8格式编码                                |
| position      | Array  | 文字在图片中的坐标信息，是一个数组，包含文字的多个坐标点信息，每一个坐标点有以下两个参数：x：Int值，坐标点的横坐标y：Int值，坐标点的纵坐标 |
| child-objects | Array  | data对象的子集对象，也是一个data对象组成的数组               |

注：识别出的文字信息存在层级关系，words为最小层级，document为最高层级。高层及对象的子集不仅限于低一个层级的对象，即一个document的子集对象可能同时包括paragraph，textline和words。

## 返回值示例

### 成功请求返回值示例：

```
{
	"image_id": "dq5nxyTmSYbENv+pfnIpIQ==",
	"result": [{
		"child-objects": [{
			"child-objects": [],
			"position": [{
				"y": 203,
				"x": 12
			}, {
				"y": 203,
				"x": 27
			}, {
				"y": 219,
				"x": 27
			}, {
				"y": 219,
				"x": 12
			}],
			"type": "character",
			"value": "低"
		}, {
			"child-objects": [],
			"position": [{
				"y": 204,
				"x": 26
			}, {
				"y": 204,
				"x": 40
			}, {
				"y": 218,
				"x": 41
			}, {
				"y": 219,
				"x": 27
			}],
			"type": "character",
			"value": "头"
		}, {
			"child-objects": [],
			"position": [{
				"y": 204,
				"x": 41
			}, {
				"y": 204,
				"x": 54
			}, {
				"y": 219,
				"x": 54
			}, {
				"y": 219,
				"x": 41
			}],
			"type": "character",
			"value": "思"
		}, {
			"child-objects": [],
			"position": [{
				"y": 203,
				"x": 54
			}, {
				"y": 204,
				"x": 69
			}, {
				"y": 219,
				"x": 69
			}, {
				"y": 219,
				"x": 54
			}],
			"type": "character",
			"value": "故"
		}, {
			"child-objects": [],
			"position": [{
				"y": 203,
				"x": 70
			}, {
				"y": 205,
				"x": 84
			}, {
				"y": 220,
				"x": 82
			}, {
				"y": 218,
				"x": 68
			}],
			"type": "character",
			"value": "乡"
		}],
		"position": [],
		"type": "textline",
		"value": "低头思故乡"
	}, {
		"child-objects": [{
			"child-objects": [],
			"position": [{
				"y": 48,
				"x": 13
			}, {
				"y": 48,
				"x": 27
			}, {
				"y": 62,
				"x": 27
			}, {
				"y": 62,
				"x": 13
			}],
			"type": "character",
			"value": "唐"
		}, {
			"child-objects": [],
			"position": [{
				"y": 48,
				"x": 26
			}, {
				"y": 48,
				"x": 41
			}, {
				"y": 62,
				"x": 41
			}, {
				"y": 63,
				"x": 27
			}],
			"type": "character",
			"value": "朝"
		}, {
			"child-objects": [],
			"position": [{
				"y": 48,
				"x": 39
			}, {
				"y": 47,
				"x": 54
			}, {
				"y": 62,
				"x": 55
			}, {
				"y": 63,
				"x": 40
			}],
			"type": "character",
			"value": "诗"
		}, {
			"child-objects": [],
			"position": [{
				"y": 47,
				"x": 56
			}, {
				"y": 47,
				"x": 69
			}, {
				"y": 62,
				"x": 70
			}, {
				"y": 62,
				"x": 55
			}],
			"type": "character",
			"value": "人"
		}, {
			"child-objects": [],
			"position": [{
				"y": 47,
				"x": 73
			}, {
				"y": 48,
				"x": 88
			}, {
				"y": 62,
				"x": 87
			}, {
				"y": 62,
				"x": 72
			}],
			"type": "character",
			"value": "李"
		}, {
			"child-objects": [],
			"position": [{
				"y": 48,
				"x": 86
			}, {
				"y": 48,
				"x": 100
			}, {
				"y": 62,
				"x": 100
			}, {
				"y": 63,
				"x": 86
			}],
			"type": "character",
			"value": "白"
		}, {
			"child-objects": [],
			"position": [{
				"y": 47,
				"x": 104
			}, {
				"y": 48,
				"x": 119
			}, {
				"y": 63,
				"x": 118
			}, {
				"y": 62,
				"x": 103
			}],
			"type": "character",
			"value": "字"
		}, {
			"child-objects": [],
			"position": [{
				"y": 47,
				"x": 118
			}, {
				"y": 47,
				"x": 133
			}, {
				"y": 62,
				"x": 133
			}, {
				"y": 63,
				"x": 118
			}],
			"type": "character",
			"value": "太"
		}, {
			"child-objects": [],
			"position": [{
				"y": 48,
				"x": 132
			}, {
				"y": 47,
				"x": 147
			}, {
				"y": 62,
				"x": 147
			}, {
				"y": 63,
				"x": 132
			}],
			"type": "character",
			"value": "白"
		}],
		"position": [],
		"type": "textline",
		"value": "唐朝诗人李白字太白"
	}, {
		"child-objects": [{
			"child-objects": [],
			"position": [{
				"y": 9,
				"x": 12
			}, {
				"y": 9,
				"x": 26
			}, {
				"y": 23,
				"x": 26
			}, {
				"y": 24,
				"x": 12
			}],
			"type": "character",
			"value": "静"
		}, {
			"child-objects": [],
			"position": [{
				"y": 8,
				"x": 26
			}, {
				"y": 9,
				"x": 41
			}, {
				"y": 24,
				"x": 41
			}, {
				"y": 24,
				"x": 26
			}],
			"type": "character",
			"value": "夜"
		}, {
			"child-objects": [],
			"position": [{
				"y": 9,
				"x": 40
			}, {
				"y": 9,
				"x": 54
			}, {
				"y": 23,
				"x": 55
			}, {
				"y": 24,
				"x": 40
			}],
			"type": "character",
			"value": "思"
		}],
		"position": [],
		"type": "textline",
		"value": "静夜思"
	}, {
		"child-objects": [{
			"child-objects": [],
			"position": [{
				"y": 164,
				"x": 13
			}, {
				"y": 165,
				"x": 27
			}, {
				"y": 179,
				"x": 26
			}, {
				"y": 179,
				"x": 12
			}],
			"type": "character",
			"value": "举"
		}, {
			"child-objects": [],
			"position": [{
				"y": 165,
				"x": 26
			}, {
				"y": 165,
				"x": 40
			}, {
				"y": 180,
				"x": 40
			}, {
				"y": 180,
				"x": 26
			}],
			"type": "character",
			"value": "头"
		}, {
			"child-objects": [],
			"position": [{
				"y": 164,
				"x": 40
			}, {
				"y": 164,
				"x": 54
			}, {
				"y": 179,
				"x": 54
			}, {
				"y": 180,
				"x": 40
			}],
			"type": "character",
			"value": "望"
		}, {
			"child-objects": [],
			"position": [{
				"y": 165,
				"x": 55
			}, {
				"y": 165,
				"x": 69
			}, {
				"y": 180,
				"x": 69
			}, {
				"y": 180,
				"x": 54
			}],
			"type": "character",
			"value": "明"
		}, {
			"child-objects": [],
			"position": [{
				"y": 164,
				"x": 67
			}, {
				"y": 165,
				"x": 82
			}, {
				"y": 180,
				"x": 82
			}, {
				"y": 180,
				"x": 67
			}],
			"type": "character",
			"value": "月"
		}],
		"position": [],
		"type": "textline",
		"value": "举头望明月"
	}, {
		"child-objects": [{
			"child-objects": [],
			"position": [{
				"y": 86,
				"x": 12
			}, {
				"y": 87,
				"x": 26
			}, {
				"y": 101,
				"x": 26
			}, {
				"y": 101,
				"x": 12
			}],
			"type": "character",
			"value": "床"
		}, {
			"child-objects": [],
			"position": [{
				"y": 86,
				"x": 26
			}, {
				"y": 87,
				"x": 41
			}, {
				"y": 101,
				"x": 41
			}, {
				"y": 102,
				"x": 26
			}],
			"type": "character",
			"value": "前"
		}, {
			"child-objects": [],
			"position": [{
				"y": 86,
				"x": 41
			}, {
				"y": 87,
				"x": 56
			}, {
				"y": 102,
				"x": 55
			}, {
				"y": 102,
				"x": 40
			}],
			"type": "character",
			"value": "明"
		}, {
			"child-objects": [],
			"position": [{
				"y": 87,
				"x": 54
			}, {
				"y": 87,
				"x": 69
			}, {
				"y": 102,
				"x": 69
			}, {
				"y": 102,
				"x": 54
			}],
			"type": "character",
			"value": "月"
		}, {
			"child-objects": [],
			"position": [{
				"y": 86,
				"x": 68
			}, {
				"y": 87,
				"x": 83
			}, {
				"y": 102,
				"x": 83
			}, {
				"y": 102,
				"x": 68
			}],
			"type": "character",
			"value": "光"
		}],
		"position": [],
		"type": "textline",
		"value": "床前明月光"
	}, {
		"child-objects": [{
			"child-objects": [],
			"position": [{
				"y": 125,
				"x": 12
			}, {
				"y": 126,
				"x": 27
			}, {
				"y": 140,
				"x": 27
			}, {
				"y": 141,
				"x": 12
			}],
			"type": "character",
			"value": "疑"
		}, {
			"child-objects": [],
			"position": [{
				"y": 126,
				"x": 26
			}, {
				"y": 126,
				"x": 40
			}, {
				"y": 140,
				"x": 40
			}, {
				"y": 140,
				"x": 26
			}],
			"type": "character",
			"value": "是"
		}, {
			"child-objects": [],
			"position": [{
				"y": 126,
				"x": 40
			}, {
				"y": 126,
				"x": 54
			}, {
				"y": 139,
				"x": 55
			}, {
				"y": 140,
				"x": 41
			}],
			"type": "character",
			"value": "地"
		}, {
			"child-objects": [],
			"position": [{
				"y": 126,
				"x": 55
			}, {
				"y": 126,
				"x": 69
			}, {
				"y": 141,
				"x": 69
			}, {
				"y": 141,
				"x": 54
			}],
			"type": "character",
			"value": "上"
		}, {
			"child-objects": [],
			"position": [{
				"y": 126,
				"x": 68
			}, {
				"y": 126,
				"x": 83
			}, {
				"y": 141,
				"x": 83
			}, {
				"y": 141,
				"x": 68
			}],
			"type": "character",
			"value": "霜"
		}],
		"position": [],
		"type": "textline",
		"value": "疑是地上霜"
	}],
	"request_id": "1473766181,9ef2c635-87f2-40b2-88ea-ee77a8dd67c7",
	"time_used": 1356
}
```

### 请求失败返回示例:

```
{
	"time_used": 3,
	"error_message": "MISSING_ARGUMENTS: image_url, image_file, image_base64",
	"request_id": "1470378968,c6f50ec6-49bd-4838-9923-11db04c40f8d"
}
```

## 当前API特有的ERROR_MESSAGE

| HTTP状态代码 | 错误信息                               | 说明                                                         |
| :----------- | :------------------------------------- | :----------------------------------------------------------- |
| 400          | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。 |
| 400          | INVALID_IMAGE_SIZE:<param>             | 客户上传的图像像素尺寸太大或太小，图片要求请参照本API描述。<param>对应图像太大的那个参数的名称 |
| 400          | IMAGE_FILE_TOO_LARGE                   | 客户上传的图像文件太大。本 API 要求图片文件大小不超过 2 MB   |
| 412          | IMAGE_DOWNLOAD_TIMEOUT                 | 下载图片超时                                                 |
| 400          | INVALID_IMAGE_URL                      | 无法从指定的image_url下载图片，图片URL错误或者无效           |

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key和api_secret不匹配。                                  |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指此API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
curl -X POST "https://api-cn.faceplusplus.com/imagepp/v1/recognizetext" \
-F "api_key=<api_key>" \
-F "api_secret=<api_secret>" \
-F "image_file=@image_file.jpg"
```

