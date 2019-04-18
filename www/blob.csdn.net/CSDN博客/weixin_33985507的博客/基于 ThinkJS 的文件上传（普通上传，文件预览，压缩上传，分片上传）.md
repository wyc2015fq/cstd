# 基于 ThinkJS 的文件上传（普通上传，文件预览，压缩上传，分片上传） - weixin_33985507的博客 - CSDN博客
2018年01月24日 04:43:38[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
具体实现参见：[demo](https://github.com/zhengqingxin/file-upload-demo)
## 实现原理简介
上传：
FormData + ajax
## 文件预览
canvas 读取生成 image
## 压缩图片上传：
### 原理
canvas.toDataURL实现压缩
### 实现
FileReader读取文件内容转为img --> canvas读取img --> 输出压缩后的img --> 转为 Blob 对象上传
## 分片上传：
### 原理
利用 file.slice 将文件分片
### 实现
- 客户端：文件分片 --> 将分片上传 --> 完成所有分片上传 --> 通知服务端合并文件
- 服务端：接收分片 --> 存储同组分片 --> 接收客户端合并请求 --> 合并并存储文件 --> 删除分片
- 
注意点：
- 分片分组：这里我在前端将同一组的分片的 file.name 设置成同一个uuid，这样后端将同一组的分片 存到同一个临时文件夹中
- 分片顺序：前端会带一个 order 参数，后端会生成文件名为 ${order}-${uuid} 名称的分片，这样合并文件的时候会直接按顺序合并
