# Nginx 对同一应用做分流后Session超时问题解决 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月15日 12:05:29[boonya](https://me.csdn.net/boonya)阅读数：119
补充说明：session问题在80端口下的应用静态代理系统图片就可以解决，也可以实现两个项目不要项目名称，比如80代理8080和8090一个后台一个移动端。请参看使用静态代理处理跨域问题。
**目录**
[Nginx服务拆分架构](#Nginx%E6%9C%8D%E5%8A%A1%E6%8B%86%E5%88%86%E6%9E%B6%E6%9E%84)
[接口同步应用共享数据](#%E6%8E%A5%E5%8F%A3%E5%90%8C%E6%AD%A5%E5%BA%94%E7%94%A8%E5%85%B1%E4%BA%AB%E6%95%B0%E6%8D%AE)
[Redis缓存应用共享数据](#Redis%E7%BC%93%E5%AD%98%E5%BA%94%E7%94%A8%E5%85%B1%E4%BA%AB%E6%95%B0%E6%8D%AE)
[Nginx解决方案](#Nginx%E8%A7%A3%E5%86%B3%E6%96%B9%E6%A1%88)
[修改Tomcat server.xml配置](#%E4%BF%AE%E6%94%B9Tomcat%20server.xml%E9%85%8D%E7%BD%AE)
[修改Nginx配置](#%E4%BF%AE%E6%94%B9Nginx%E9%85%8D%E7%BD%AE)
[使用静态代理处理跨域问题](#%E4%BD%BF%E7%94%A8%E9%9D%99%E6%80%81%E4%BB%A3%E7%90%86%E5%A4%84%E7%90%86%E8%B7%A8%E5%9F%9F%E9%97%AE%E9%A2%98)
[代理系统静态访问](#%E4%BB%A3%E7%90%86%E7%B3%BB%E7%BB%9F%E9%9D%99%E6%80%81%E8%AE%BF%E9%97%AE)
[代理服务器访问](#%E4%BB%A3%E7%90%86%E6%9C%8D%E5%8A%A1%E5%99%A8%E8%AE%BF%E9%97%AE)
并发问题：同一个应用实现了后端功能和移动端接口，因为并发卡在了移动端图片上传导致后端服务受影响。
提出方案：通过Nginx代理两个Tomcat实现后端和移动端应用分离，主服务地址唯一，通过转发实现唯一路径访问。
方案问题：因为应用部署需要去掉项目路径，这里会遇到转发的路径如果不设置项目名称会造成主服务器session失效。
## Nginx服务拆分架构
### 接口同步应用共享数据
目前Redis没做严格测试暂时通过接口来实现两个Tomcat之间的数据交互
![](https://img-blog.csdnimg.cn/20190315113935466.png)
因为接口安全要么做登录验证要么做弱验证，这里面应用之间安全问题可以通过配置secureCode校验。
### Redis缓存应用共享数据
上Redis之后的架构：
![](https://img-blog.csdnimg.cn/20190315114331547.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
## Nginx解决方案
修改转发的路径不采用/ ，避免应用路径与Nginx的 location /{} 产生session重建。
### 修改Tomcat server.xml配置
![](https://img-blog.csdnimg.cn/20190315115040855.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
转发服务不要使用path="/"。
### 修改Nginx配置
唯一地址及后台服务代理
![](https://img-blog.csdnimg.cn/20190315120445343.png)
转发Tomcat代理
![](https://img-blog.csdnimg.cn/20190315120416234.png)
解决了什么问题：
1、附件上传在移动端超时问题（后台访问移动应用上传图片地址）
2、共享数据访问在移动端超时问题（后台访问移动端缓存接口）
## 使用静态代理处理跨域问题
### 代理系统静态访问
![](https://img-blog.csdnimg.cn/20190327135133305.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
### 代理服务器访问
![](https://img-blog.csdnimg.cn/2019032713521515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
