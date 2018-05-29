# 人脸识别魔镜接口说明


## 版本
 0.1

## 描述
传入一张或多张图片进行人脸检测和人脸分析。
如果没有识别过则返回新生成的Id号。
如果曾经识别过则返回当时的生成的Id号。

## 图片要求
图片格式: jpg(jpeg), png
图片像素尺寸：最小 48*48 像素，最大 4096*4096 像素
最小人脸像素尺寸: 人脸尺寸不能小于160*160

## 更新日志
2017 年 10 月 18 日：完成基本功能。

## 调用URL
http://...

## 调用方法
POST
注意: Content-Type 必须是 multipart/form-data

## 请求参数
无

## 返回值说明
|字段|类型|说明|
|:--|:--|:--|
|result|String|返回值|
|isnew|bool|是新的人脸|
|id|int|内部id, -1表示识别失败, 大于等于0表示自动生成的id号|
|role_id|int|角色id[1,20]|
|role_name|String|角色名字|
|rec_count|uint|识别次数|

|result 值|说明|
|:--|:--|
|OK|识别成功|
|ARGS_ERR|参数错误|
|CAN_NOT_DETECT_FACE|没检测到人脸|
|FACE_IS_NOT_FRONT|不是正脸|


## 返回值示例
### 请求成功返回示例
	{"result":"OK","id":"3","isnew":"false","role_id":"4","username":"","rec_count":"2" }

### 请求失败返回示例
	{"result":"FACE_IS_NOT_FRONT","id":-1}

## 当前API特有的ERROR_MESSAGE
无

## 通用的ERROR_MESSAGE
无
## 调用示例
	curl -X POST "http://localhost" -F "image_file=@0.jpg" -F "image_file=@1.jpg"