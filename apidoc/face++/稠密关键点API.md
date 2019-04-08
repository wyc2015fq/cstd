# 稠密关键点API

## 版本

1.0

## 描述

传入图片或是从 Detect API 检测出的人脸标识 face_token，获得人脸的1000点关键点信息，对五官和脸部轮廓进行精确定位。

一次调用支持一张人脸的关键点分析。如果图片中有多张人脸，只对人脸框面积最大的一个人脸进行分析。 （相同面积，随机选一张人脸）

本API是对人脸各部位的五官进行精确定位，推荐使用正脸高清大图获得最佳效果。

### 图片要求

图片格式：JPG(JPEG)，PNG
图片像素尺寸：最小 100*100 像素，最大 4096*4096 像素
图片文件大小：2 MB
最小人脸像素尺寸： 系统能够检测到的人脸框为一个正方形，正方形边长的最小值为图像短边长度的 24 分之一，最小值不低于 100 像素。 例如图片为 4096*3200 像素，则最小人脸像素尺寸为 132*132 像素。

## 调用URL

[https://api-cn.faceplusplus.com/facepp/v1/face/](https://api-cn.faceplusplus.com/facepp/v3/face/analyze)thousandlandmark

## 调用方法

POST

## 权限

所有 API Key 都可以调用本 API。

## 请求参数

| 是否必选       | 参数名          | 类型                                                         | 参数说明                                                     |
| :------------- | :-------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| 必选           | api_key         | String                                                       | 调用此 API 的 API Key                                        |
| 必选           | api_secret      | String                                                       | 调用此 API 的 API Secret                                     |
| 必选（四选一） | face_token      | String                                                       | 人脸标识 face_token，优先使用该参数                          |
| image_url      | String          | 图片的 URL                                                   |                                                              |
| image_file     | File            | 图片的二进制文件，需要用 post multipart/form-data 的方式上传。 |                                                              |
| image_base64   | String          | base64 编码的二进制图片数据如果同时传入了 image_url、image_file 和 image_base64参数，本 API 使用顺序为image_file 优先，image_url最低。 |                                                              |
| 必选           | return_landmark | String                                                       | 是否检测并返回人脸关键点。合法值为：all检测并返回所有部位的关键点。（只要参数中包括all，则返回所有部位关键点）left_eyebrowright_eyebrowleft_eyeright_eyenosemouseface 希望检测并返回的人脸部位。需要将需要的项目组成一个用逗号分隔的字符串，项目之间的顺序没有要求。关于各项的详细描述，参见下文“返回值”说明的 "landmarks" 部分。 |

## 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。                           |
| face          | Object | 经过分析的人脸注：如果没有检测出人脸则为空。                 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。                           |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### faces 数组中单个元素的结构

| 字段           | 类型   | 说明                                                         |
| :------------- | :----- | :----------------------------------------------------------- |
| face_rectangle | Object | 人脸矩形框的位置，包括以下属性。每个属性的值都是整数：top：矩形框左上角像素点的纵坐标left：矩形框左上角像素点的横坐标width：矩形框的宽度height：矩形框的高度 |
| landmark       | Object | 人脸五官及轮廓的关键点坐标数组。（详细关键点图示见下一段落） |

#### landmark 中包含的元素说明

| 字段          | 类型   | 说明                                                         |                                                              |
| :------------ | :----- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| face          | Object | 面部轮廓关键点集合。返回值为：face_hairline_0 ... face_hairline_144面部上半部分轮廓关键点，从右耳附近起始到左耳附近终，按逆时针顺序检测到的位置序列。face_contour_right_0... face_contour_right_63面部下半部分右边轮廓关键点。从下巴起始到右耳附近，按逆时针顺序检测到的位置序列。face_contour_right_0为下巴中心位置。 face_contour_left_0... face_contour_left_63面部下半部分左边轮廓关键点。从下巴起始到左耳附近，按顺时针顺序检测到的位置序列。面部有角度时，关键点会标出视觉区域内的面部轮廓关键点。如有遮挡目前是是标出预判的关键点位置。（计划今后可以对遮挡部位返回空值。） | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-face.jpg) |
| left_eyebrow  | Object | 左眉毛关键点集合。返回值为：left_eyebrow_0 ... left_eyebrow_63从左眉左端中心位置起始，按顺时针顺序检测到的左眉关键点位置序列。 | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-5.jpg) |
| right_eyebrow | Object | 右眉毛关键点集合。返回值为right_eyebrow_0 ... right_eyebrow_63从右眉右端中心位置起始，按逆时针顺序检测到的右眉关键点位置序列。 | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-3.jpg) |
| left_eye      | Object | 左眼关键点集合。返回值为left_eye_0 ... left_eye_62从左眼左端中心位置起始，按顺时针顺序检测到的左眼关键点位置序列。left_eye_pupil_radius左眼瞳孔半径 | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-4.jpg) |
| right_eye     | Object | 右眼关键点集合。返回值为right_eye_0 ... right_eye_62从右眼右端中心位置起始，按逆时针顺序检测到的右眼关键点位置序列。right_eye_pupil_center右眼瞳孔中心位置right_eye_pupil_radius右眼瞳孔半径 | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-2.jpg) |
| nose          | Object | 鼻子关键点集合。返回值为nose_left_0 ... nose_left_62从鼻子上部左端位置起始到鼻尖，顺序检测到的鼻子关键点位置序列。nose_right_0 ...nose_right_62从鼻子上部右端端位置起始到鼻尖，顺序检测到的鼻子关键点位置序列left_nostril左鼻孔位置（鼻孔上边缘中心）right_nostril右鼻孔位置（鼻孔上边缘中心）nose_midline_0...nose_midline_59从眉心中间到人中，从上到下顺序检测到的鼻子中线关键点位置序列 | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-1.jpg) |
| mouth         | Object | 嘴巴关键点集合。返回值为upper_lip_0 ... upper_lip_31上嘴唇的上边缘。从左边嘴角开始，从左到右检测到的上嘴唇上边缘关键点位置序列。upper_lip_32 ... upper_lip_63上嘴唇的下边缘。从右到左检测到的上嘴唇下边缘关键点位置序列。lower_lip_0 ... lower_lip_31下嘴唇的下边缘。从左边嘴角开始，从左到右检测到的下嘴唇下边缘关键点位置序列。lower_lip_32 ... lower_lip_63下嘴唇的上边缘。从右到左检测到的下嘴唇上边缘关键点位置序列。 | ![img](http://cdn.faceplusplus.com.cn/facepp/doc/landmark-7.jpg) |

#### Lines：

面部的五官区域和轮廓可以用将点连起来的方式来获得，可以生成一个polygonal chain，即多个点按顺序连起来形成的线，可以是闭环的也可以不是。这条线的表达方式就是点的集合。

![img](http://cdn.faceplusplus.com.cn/facepp/doc/denselandmark1.png)![img](http://cdn.faceplusplus.com.cn/facepp/doc/denselandmark2.png)

通过返回的关键点，可以生成对应的曲线。以下为一种示例（使用opencv的ploylines函数）展示如果通过点来生成曲线并进行后续操作。

**生成曲线示例：**

![img](http://cdn.faceplusplus.com.cn/facepp/doc/denselandmark3.png)

### 成功请求返回值示例：

```
{
	"time_used": 339,
	"request_id": "1548051575,97f2634e-cf29-4afa-8bc0-ff48f501828b",
	"face": {
		"landmark": {
			"left_eyebrow": {
				"left_eyebrow_54": {
					"y": 171,
					"x": 217
				},
				"left_eyebrow_55": {
					"y": 171,
					"x": 215
				},
				"left_eyebrow_52": {
					"y": 171,
					"x": 221
				},
				"left_eyebrow_53": {
					"y": 171,
					"x": 219
				},
				"left_eyebrow_50": {
					"y": 170,
					"x": 225
				},
				"left_eyebrow_51": {
					"y": 170,
					"x": 223
				},
				"left_eyebrow_56": {
					"y": 172,
					"x": 213
				},
				"left_eyebrow_57": {
					"y": 172,
					"x": 211
				},
				"left_eyebrow_18": {
					"y": 159,
					"x": 229
				},
				"left_eyebrow_19": {
					"y": 159,
					"x": 232
				},
				"left_eyebrow_16": {
					"y": 159,
					"x": 225
				},
				"left_eyebrow_17": {
					"y": 159,
					"x": 227
				},
				……"right_eyebrow_7": {
					"y": 178,
					"x": 358
				},
				"right_eyebrow_62": {
					"y": 186,
					"x": 360
				},
				"right_eyebrow_63": {
					"y": 187,
					"x": 362
				}
			}
		},
		"face_rectangle": {
			"width": 216,
			"top": 164,
			"height": 217,
			"left": 161
		}
	}
}
```

{ "time_used": 339, "request_id": "1548051575,97f2634e-cf29-4afa-8bc0-ff48f501828b", "face": {"landmark": { "left_eyebrow": { "left_eyebrow_54": { "y": 171, "x": 217 }, "left_eyebrow_55": { "y": 171, "x": 215 }, "left_eyebrow_52": { "y": 171, "x": 221 }, "left_eyebrow_53": { "y": 171, "x": 219 }, "left_eyebrow_50": { "y": 170, "x": 225 }, "left_eyebrow_51": { "y": 170, "x": 223 }, "left_eyebrow_56": { "y": 172, "x": 213 }, "left_eyebrow_57": { "y": 172, "x": 211 }, "left_eyebrow_18": { "y": 159, "x": 229 }, "left_eyebrow_19": { "y": 159, "x": 232 }, "left_eyebrow_16": { "y": 159, "x": 225 }, "left_eyebrow_17": { "y": 159, "x": 227 },……"right_eyebrow_7": { "y": 178, "x": 358 }, "right_eyebrow_62": { "y": 186, "x": 360 }, "right_eyebrow_63": { "y": 187, "x": 362 } } }, "face_rectangle": { "width": 216, "top": 164, "height": 217, "left": 161 }}}

### 失败请求返回值示例：

```json
{
    "time_used": 5,
    "error_message": "INVALID_FACE_TOKENS_SIZE",
    "request_id": "1469761051,ec285c20-8660-47d3-8b91-5dc2bffa0049"
}
```

## 当前 API 特有的 ERROR_MESSAGE

| HTTP状态代码 | 错误信息                         | 说明                                                   |
| :----------- | :------------------------------- | :----------------------------------------------------- |
| 400          | INVALID_FACE_TOKEN: <face_token> | 使用 face_token 作为参数时，所传的 face_token 不存在。 |

 

## 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                     | 说明                                                         |
| :------------ | :--------------------------- | :----------------------------------------------------------- |
| 401           | AUTHENTICATION_ERROR         | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR:<reason> | api_key没有调用本API的权限，具体原因为：用户自己禁止该api_key调用、管理员禁止该api_key调用、由于账户余额不足禁止调用。目前的<reason>有：Denied by Client（用户自己禁止该api_key调用）Denied by Admin（管理员禁止该api_key调用）Insufficient Account Balance（由于账户余额不足禁止调用） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED   | 并发数超过限制。注：这里的并发控制数超出限制，是指该API Key的QPS已经达到上限。如需要提高API Key的QPS配额请查看价格方案或者联系我们。 |
| 400           | MISSING_ARGUMENTS: <key>     | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS:<key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长等.） |
| 400           | COEXISTENCE_ARGUMENTS        | 同时传入了要求是二选一或多选一的参数。如有特殊说明则不返回此错误。 |
| 412           | IMAGE_DOWNLOAD_TIMEOUT       | 图片下载超时                                                 |
| 413           | Request Entity Too Large     | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404           | API_NOT_FOUND                | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR               | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系技术支持团队。 |

## 调用示例

```
`curl -X POST ``"https://api-cn.faceplusplus.com/facepp/v1/face/thousandlandmark"` `-F ``"api_key=<api_key>"` `\` `-F ``"api_secret=<api_secret>"` `\` `-F ``"image_file=@image_file.jpg"` `\` `-F ``"return_landmark=All"` 
```