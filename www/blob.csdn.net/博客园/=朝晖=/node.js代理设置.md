# node.js代理设置 - =朝晖= - 博客园
# [node.js代理设置](https://www.cnblogs.com/dhcn/p/7115402.html)
1.设置代理
npm config set proxy=http://proxy.tencent.com:8080
设置代理服务器，比如：npm config set proxy=http://127.0.0.1
```
npm config set proxy http://proxy.company.com:8080  
npm config set https-proxy http://proxy.company.com:8080
```
我将http://127.0.0.1设置为代理服务器，但http://127.0.0.1没有实际的代理功能，最后只好把所有安装包所需资源下载到127.0.0.1根目录下，
最后安装成功，express所需的资源较多。
且在windows下，没有后缀名的文件和文件夹命名冲突，需要修改JSON文件指向其它的目录，
express所需资源及整合下载：
[http://115.com/file/ani71dpa#www.zip](http://115.com/file/ani71dpa#www.zip)
如果下载不了，可以评论里面留下邮箱，我会一一发送给大家。
下载完后，放至域名根目录，并将域名设为代理服务器，这个确定可以安装成功。
2.设置镜像
默认是https://registry.npmjs.org
npm config set registry http://registry.npmjs.vitecho.com（失效了） 
npm config set registry http://npm.tuna.tsinghua.edu.cn/registry (清华大学Node Packaged Modules镜像)
“http://registry.npmjs.vitecho.com”是国内的镜像网站。不限定这个网址，你可以使用任何资源镜像网站。
你也可以通过其它方式访问http://registry.npmjs.vitecho.com/express，下载下来，express文件为一个json文件。
将express放在自己确认可访问的HTTP服务器下，比如我在本机配置了HTTP服务器，我将express放在http://127.0.0.1/express，
那么执行：npm config set registry http://127.0.0.1
转自：http://www.2fz1.com/?tag=windows
3.清除http请求缓存
npm cache clean
4.删除配置文件
npm config delete **key(配置项)**
转自：http://www.cnblogs.com/terrylin/archive/2013/01/24/2874743.html
http://jjasonclark.com/how-to-setup-node-behind-web-proxy

