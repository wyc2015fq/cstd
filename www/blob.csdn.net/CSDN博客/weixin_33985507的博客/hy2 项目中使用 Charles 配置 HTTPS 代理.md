# hy2 项目中使用 Charles 配置 HTTPS 代理 - weixin_33985507的博客 - CSDN博客
2018年03月29日 13:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
- Charles 勾选 Proxy → macOS Proxy
- 
Proxy → SSL Proxying 开启所有的 443 端口
![2793567-238e9c3a97780494.png](https://upload-images.jianshu.io/upload_images/2793567-238e9c3a97780494.png)
- Tools → Map Remote Setting
- Enable Map Remote ☑️
- 
Add
![2793567-84d952ddfc887cd2.png](https://upload-images.jianshu.io/upload_images/2793567-84d952ddfc887cd2.png)

![2793567-3d04638e5419b191.png](https://upload-images.jianshu.io/upload_images/2793567-3d04638e5419b191.png)
- 
用 Yapi mock 数据，Charles 的 rewrite 规则的写法
![2793567-8da8c80499e41fd7](https://upload-images.jianshu.io/upload_images/2793567-8da8c80499e41fd7)
