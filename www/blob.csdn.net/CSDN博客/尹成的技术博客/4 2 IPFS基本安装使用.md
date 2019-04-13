
# 4.2 IPFS基本安装使用 - 尹成的技术博客 - CSDN博客

2018年11月09日 14:33:47[尹成](https://me.csdn.net/yincheng01)阅读数：26个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



安装IPFS获取https://docs.ipfs.io/introduction/install/
安装sudo tar -zxvf go-ipfs_v0.4.17_freebsd-amd64.tar.gz -C /usr/local/bin/
sudo ln -s /usr/local/bin/go-ipfs/ipfs /usr/bin/ipfs

项目配置创建ipfs节点 : ipfs init
查看进入指定ipfs目录：cd ~/.ipfs
设置EDITOR环境变量：export EDITOR=/usr/bin/vim
查看配置：ipfs config edit
4 查看节点id：ipfs id
启动节点服务：ipfs daemon
添加跨域资源共享配置ipfs config –json API.HTTPHeaders.Access-Control-Allow-Methods ‘[“PUT”,”GET”,”POST”,”OPTIONS”]’
ipfs config –json API.HTTPHeaders.Access-Control-Allow-Origin ‘[“*”]’
验证启动服务器 ipfs daemon
执行命令(新建终端) ： ipfs cat /ipfs/QmS4ustL54uo8FzR9455qaxZwuMiUhyvMcX9Ba8nUH4uVv/readme
进入浏览器 输入http://localhost:5001/webui

在IPFS中新增文件
a. 新建文件 test.txt
b. 写入内容
c. ipfs add test.txt
d. 同步到网络：ipfs daemon
e. 通过ipfs数据浏览器查看 ：http://ipfs.io/ipfs/hash…
通过IPFS创建目录存储文件
a. 创建目录：ipfs files mkdir /troytan
b. 拷贝文件：ipfs files cp /ipfs/hash… /troytan/test.txt
c. 查看目录内容：ipfs files ls /troytan/
d. 查看拷贝的文件内容：ipfs files read /troytan/test.txt
在IPFS上新增目录
a. 上传整个目录：ipfs add -r ipfsdir/
b. 通过路径访问上传目录中指定文件的数据
i. 方法一：cat QmTh1dT71hbPQoPZSbEMGUjyxVfDPJwqF9F4XMq4NV3ihS(指定文件的散列)
ii. 方法二：cat /ipfs/QmTh1dT71hbPQoPZSbEMGUjyxVfDPJwqF9F4XMq4NV3ihS(指定文件的散列)
iii. 方法三：ipfs cat /ipfs/QmaLHrL8kG5LbWRTLisVxpirhCDiUhpaGUGZuicvyoJifE/new.txt(此处的散列是目录的散列)
通过Hash查看IPFS网络数据
a. 通过目录访问文件：https://ipfs.io/ipfs/QmaLHrL8kG5LbWRTLisVxpirhCDiUhpaGUGZuicvyoJifE/new.txt(此处是目录的散列)
b. 通过hash直接访问文件：https://ipfs.io/ipfs/QmTh1dT71hbPQoPZSbEMGUjyxVfDPJwqF9F4XMq4NV3ihS
创建简易网页发布到IPFS新建html与css
添加到ipfs：ipfs add -r site/
网络同步：ipfs daemon
访问数据：http://ipfs.io/ipfs/hash…(此处的哈希是目录的哈希)
发布到IPNS：在我们修改网站内容并重新添加到ipfs时，hash会发生变化，当我们的网站更新时，我们可以将网站发布到IPNS，在IPNS中，允许我们节点域名空间中引用一个IPFS HASH，也就是说我们可以通过节点ID对项目的根目录的IPFS HASH进行绑定，后面我们访问网站时直接通过节点ID访问即可，当我们更新博客时，重新发币到IPNS即可。发布方式：ipfs name publish hash，此处返回节点ID
验证：检查返回的节点ID和我们自己查找的这个节点ID是否一致
访问IPNS：https://ipfs.io/ipns/QmcmnKk8Ydzrwh6KxQzXdNgbGdpeGZs4htakaYvMDn7FgU(节点ID)

发布个人博客
1. 搭建静态博客(Hugo)
1. 安装hugo:sudo apt-get install hugo
2. 查检安装结果:hugo version
3. 新建：hugo new site troytan
4. 添加主题：
1. git init
2. git submodule addhttps://github.com/themefisher/navigator-hugo.git
3. echo ‘theme = “navigator-hugo”’ >> config.toml
5. 添加内容
1. hugo new posts/my-first-post.md
2. hugo server -D
2. 替换节点ID(如果有)
3. 重复步骤七
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

