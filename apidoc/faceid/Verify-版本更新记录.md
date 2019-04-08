# Verify-版本更新记录

\---------------------------------
2017-06-07， V2.0.6
\---------------------------------

**API功能和能力升级：**

- 1) 新增对炫彩活体SDK的比对支持

\---------------------------------
2017-02-22， V2.0.5
\---------------------------------

**API功能和能力升级：**

- 1) 新增对image参数和image_ref[x]参数图片的旋转检测能力，详情见新增的multi_oriented_detection参数描述。
- 2) 新增第三方数据源更新调度策略的设置，详情见新增的data_source_image_update_status参数描述。
- 3) 新增换脸攻击检测能力，增强移动端活体SDK的防攻击能力。需要配合移动端MegLive活体SDK 2.4.3及以上版本使用。详情见face_genuineness返回段中的face_replaced。
- 4) 新增Delta使用的校验，用于防止攻击者使用历史数据进行人脸比对攻击，详情见新增的check_delta参数描述。

\---------------------------------
V2.0.1
\---------------------------------

相比之前的FaceID Verify方法（v1版本)，该版本的改进主要有：

- \- 使用最新的比对算法，性能提升；
- \- 加入了云端比对照的假脸检测功能；
- \- 比对照可以为未经过detect方法检测的照片了，可以将人脸检测和人脸比对打包在一起做，并且可以返回检测出的人脸。
- \- 修复了v1版本中“一个api_key只能有源比对和无源比对中二选一”的bug；
- \- 与MegLive配合使用时，image_action不再必须上传；
- \- 增加了对曾经有活体攻击历史的身份证号码的判定。
- \- 增进了对黑白的第三方身份信息数据源照片的判定结果返回。
- \- 优化了多个参数、返回值、错误码的设计；
- \- 优化了文档。 

\---------------------------------
V1.0.0
\---------------------------------

人脸比对API的第一个发行版本，文档下载地址：[Verify 1.0.0 （旧版本）文档](http://bj-faceid-prod-asset.oss-cn-beijing.aliyuncs.com/WIKI_SYNC_PICS/DOCS/Verify_1.0.0.pdf)

 

------

### 附：Verify 2.0.1 与 Verify 1.0.0 的错误码的差别

FaceID Verify API 从1.0升级到2.0是软件的主版本升级，除功能有大幅升级外，错误码返回也做了相应的完善，未与Verify 1.0的错误码一一对应，不保证向前兼容性。开发者选择从Verify 1.0升级到2.0，须仔细阅读Verify 2.0文档。本文给出的是两个版本API（最新版2.0.1 与 1.0.0） 的错误码返回的差别，方便FaceID开发者参考。

在下面的表格里，黑色文字行表示两个版本的API的返回码一致、或仅有装饰性的说明文字优化；而红色文字行存在实质性变化，具体为下列情况之一：

- －两个版本API的返回码错误信息不一致，但表示相同的错误（例如：NO_SUCH_ID_NUMBER  vs. NO_SUCH_PERSON)
- －两个版本的的返回码错误信息一致，但内容有变化（例如：AUTHORIZATION_ERROR:<reason>  vs. AUTHORIZATION_ERROR)
- －两个版本的返回码错误信息不一致，但新版本的错误信息包含并扩展了旧版本错误信息的内容（例如 IMAGE_ERROR_UNSUPPORTED_FORMAT:datasource vs. IDCARD_NO_PHOTO)
- －新版本新增的错误码（例如：400（LOW_QUALITY）)

 

**返回****JSON****里错误码字段名的变更：**

| **Verify 2.0.1** | **Verify 1.0** |
| ---------------- | -------------- |
| “error_message”  | “error”        |

 

**返回****JSON****里错误码值的变更：**

| **Verify 2.0.1** | **Verify 1.0**                         |               |                                |                                                              |
| :--------------- | :------------------------------------- | :------------ | :----------------------------- | :----------------------------------------------------------- |
| HTTP 状态代码    | 错误信息                               | HTTP 状态代码 | 错误信息                       | 说明                                                         |
| 400              | NO_FACE_FOUND:<param>                  | 400           | NO_FACE_FOUND                  | **1.0:**此错误只会出现于result_idcard及result_faceid，表示身份证图像中没有找到人脸。 **2.0.1:**从**第三方身份信息数据源（以下简称“数据源”）**获得的身份证图像、或是用户传递的image_ref图像中没有找到人脸。当有源比对时，如果数据源没有指定人的图像、或者图像里没有检测到人脸，参数<param>值为data_source；当image_ref1、image_ref2、image_ref3中没有人脸时，参数<param>分别返回各自的参数名。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400              | MISSING_ARGUMENTS:<key>                | 400           | MISSING_ARGUMENTS:<key>        | 缺少某个必要参数。                                           |
| 400              | INVALID_FACE_TOKEN:<token>             | 400           | INVALID_FACE_TOKEN:<token>     | **1.0:**所传的face_token不存在。 **2.0.1:**使用face token作为待验证图时，所传的face_token不存在。 |
| 400              | NO_SUCH_ID_NUMBER                      | 400           | NO_SUCH_PERSON                 | **1.0:**身份证号无法对应一个自然人（存在身份证号真实存在，但公安网中并没有录入此信息的情况）。 **2.0.1:**有源比对时，数据源中没有此身份证号码的记录。 |
| 400              | INVALID_NAME_FORMAT                    | 400           | INVALID_NAME_FORMAT            | **1.0:**错误的姓名编码，姓名要求以UTF-8编码给出。 **2.0.1:**有源比对时，idcard_name参数字符数过多（多于32字符）、或者使用错误的编码（姓名要求以UTF-8编码）。 |
| 400              | INVALID_IDCARD_NUMBER                  | 400           | INVALID_IDCARD_NUMBER          | **1.0：**错误的身份证号码，身份证号码必定为18位数字，且最后一位可以是X（大小写均可）。 **2.0.1:**有源比对时，idcard_number参数不是正确的身份证号码格式。身份证号码必定为18位数字，且最后一位可以是X（大小写均可）。 |
| 400              | DATA_VALIDATION_ERROR                  | 400           | DATA_VAILDATION_ERROR          | **1.0:**校验数据与上传的图像无法一一对应，或者图像上传不完整。 **2.0.1:**配合MegLive SDK使用时，delta 参数的校验数据与上传的图像无法一一对应，或者图像上传不完整。 |
| 400              | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 400           | IMAGE_ERROR_UNSUPPORTED_FORMAT | **1.0:**客户上传的image_idcard图像无法正确解析，其可能不是一个图像文件。 **2.0.1:**参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。<param>为 data_source、 facetoken、 image_ref1、 image_ref2、 image_ref3、 image中的一个。当<param>取值为data_source时，表示数据源的图片无法解析、或者数据源没有照片。**注意：这对应Verify 1.0 API里的错误码 400（IDCARD_NO_PHOTO）**当<param>取其他值时对应相应的参数。 |
| 400              | INVALID_IMAGE_SIZE:<param>             | 400           | INVALID_IMAGE_SIZE:<param>     | **1.0:**客户上传的图像像素尺寸的长或宽超过4096像素。<param>对应图像太大的那个参数的名称。 **2.0.1:**客户上传的图像太大。具体是指图像存储尺寸超过2MB、或者像素尺寸的长或宽超过4096像素。<param>对应图像太大的那个参数的名称，为image_ref1、 image_ref2、 image_ref3、 image中的一个。请注意：如果图片存储尺寸大于2MB，会触发错误413(Request Entity Too Large)。 |
| 400              | DATA_SOURCE_ERROR                      | 400           | DATA_SOURCE_ERROR              | 当数据源发生错误时会出现此错误，出现此错误时建议停止业务，并联系客服，待数据源回复正常后再开启业务。 |
| 400              | ID_NUMBER_NAME_NOT_MATCH               | 400           | IDCARD_NO_MATCH                | **1.0：**客户上传的身份证号有对应的人，但身份证号与一同上传的姓名不匹配。 **2.0.1:**有源比对时，数据源中存在此身份证号码，但与提供的姓名不匹配。 |
| 400              | IMAGE_ERROR_UNSUPPORTED_FORMAT:<param> | 400           | IDCARD_NO_PHOTO                | **1.0:**身份证号和姓名可被查询到并确认一致，但公安网中没有此人的照片故无法比对。 **2.0.1:****(对应IDCARD_NO_PHOTO时，<param>取值 datasource）**参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。<param>为 data_source、 facetoken、 image_ref1、 image_ref2、 image_ref3、 image中的一个。当<param>取值为data_source时，表示数据源的图片无法解析、或者数据源没有照片。**注意：这对应Verify 1.0 API里的错误码 400（IDCARD_NO_PHOTO）.**当<param>取其他值时对应相应的参数。 |
| 403              | AUTHORIZATION_ERROR<:reason>           | 403           | AUTHORIZATION_ERROR            | **1.0:**客户身份信息验证不通过。 **2.0.1:**api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。<reason>有：Limit reached. （这个api_key对当前API的调用量达到上限。仅当api_key为测试key时才有可能发生） Denied. （没有权限调用当前API）No data source permission. （仅为FaceID Verify API存在，表示使用“有源比对”的方式调用FaceID Verify API，但是没有“有源比对”的权限） |
| 403              | CONCURRENCY_LIMIT_EXCEEDED             | 403           | CONCURRENCY_LIMIT_EXCEEDED     | 并发数超过限制。                                             |
| 413              | Request Entity Too Large               | 413           | Request Entity Too Large       | 客户发送的请求大小超过了2MB限制。该错误的返回格式为纯文本，不是json格式。 |
| 404              | API_NOT_FOUND                          | 404           | API_NOT_FOUND                  | 所调用的API不存在。                                          |
| 500              | INTERNAL_ERROR                         | 500           | INTERNAL_ERROR                 | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系FaceID客服或商务。 |
| 400              | BAD_ARGUMENTS:<key>                    | （无）        | （无）                         | **1.0:**（无） **2.0.1：**某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长，etc.） |
| 400              | LOW_QUALITY                            | （无）        | （无）                         | **1.0:**（无）  **2.0.1：**参数image对应的图像中的人脸图像质量低于阈值face_quality_threshold。请注意：image参数对应的图像中可能存在多张脸，这里指的“人脸”是指其中的面积最大的脸。image发现多张脸可能导致400（MULTIPLE_FACES）的错误，请参考该错误码的说明。 |
| 400              | MULTIPLE_FACES:<param>                 | （无）        | （无）                         | **1.0:**（无） **2.0.1：**参数<param>对应的图像中发现有多张脸。<param>为image_ref1、image_ref2、image_ref3、image中的一个。仅当fail_when_multiple_faces取值为1时，<param>才可能取值为image。请注意：<param>只会有一项，即第一个满足条件的名称。 |

 

 

 