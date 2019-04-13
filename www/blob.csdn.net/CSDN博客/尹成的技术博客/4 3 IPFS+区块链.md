
# 4.3 IPFS+区块链 - 尹成的技术博客 - CSDN博客

2018年11月09日 14:34:47[尹成](https://me.csdn.net/yincheng01)阅读数：119个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



js-ipfs-api如何使用js-ipfs-api
具体实现安装create-react-app:sudo npm install -g create-react-app
创建React项目：create-react-app ipfs-http-demo
运行：cd ipfs-http-demo && npm start
安装 ipfs-api：npm install –save ipfs-api
完成前端逻辑
导入ipfsconst ipfsApi = require(“ipfs-api”); //导入IPFS
const ipfs = ipfsApi({host:’localhost’, port:’5001’, protocol:’http’}); // 配置网络
上传字符串到IPFS中的Promise函数
测试
跨域资源共享的CORS配置ipfs config –json API.HTTPHeaders.Access-Control-Allow-Methods ‘[“PUT”,”GET”,”POST”,”OPTIONS”]’
ipfs config –json API.HTTPHeaders.Access-Control-Allow-Origin ‘[“*”]’
ipfs config –json API.HTTPHeaders.Access-Control-Allow-Credentials ‘[“true”]’
ipfs config –json API.HTTPHeaders.Access-Control-Allow-Headers ‘[“Authorization”]’
ipfs config –json API.HTTPHeaders.Access-Control-Expose-Headers ‘[“Location”]’
用正确的端口运行daemonipfs config Addresses.API
ipfs config Addresses.API /ip4/127.0.0.1/tcp/5001
ipfs daemon
测试
从IPFS读取数据

IPFS图片上传与下载创建React项目：create-react-app ipfs-http-pic
cd ipfs-http-pic && npm start
安装ipfs-api:npm install –save ipfs-api
App.js导入IPFS
实现上传图片到IPFS的Promise函数
6 上传图片到IPFS
IPFS与Ethereumtruffle unbox react
安装ipfs-api
修改前端逻辑
修改合约
truffle develop
ipfs daemon
npm start

学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

