# android 热修复hotfix使用笔记 - weixin_33985507的博客 - CSDN博客
2017年02月06日 17:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
hotfix主要是解决紧急的错误，小改动的一个解决方案（阿里百川也在进一步的开发中，使限制越来越少，必须赞,期待2.0的发布)
[阿里百川HotFix接入口](https://link.jianshu.com?t=http://baichuan.taobao.com/docs/doc.htm?spm=a3c0d.7629140.0.0.QgGbM0&treeId=234&articleId=105461&docType=1)
## 一: Hotfix集成的步骤
1: 添加maven仓库地址
2: 添加gradle坐标版本依赖
3: 权限说明
4: 配置AndroidManifest文件,主要是appsecret，rsa字符串(阿里百川后台创建应用会生成)
5: 混淆配置
6: Application中初始化
上面具体了代码，请查看[集成配置](https://link.jianshu.com?t=http://baichuan.taobao.com/docs/doc.htm?spm=a3c0d.7629140.0.0.fUChRm&treeId=234&articleId=105457&docType=1)
## 二：hotfix使用过程中的问题
**1: 打包补丁时，路径是绝对路径**
看截图的命令行
![909565-285604f596a5adc5.png](https://upload-images.jianshu.io/upload_images/909565-285604f596a5adc5.png)
222.png
**2: 资源修改 ---- 暂时不支持  **
本来想修改XML布局文件中一个textview引用颜色值的，蓝色变成红色，但是当生成补丁包时，报错了,提示没有增加和修改类文件，对资源文件的修改忽略了，所以暂时不支持，可以放到代码中去修改
![909565-455cd49e200fbf3c.png](https://upload-images.jianshu.io/upload_images/909565-455cd49e200fbf3c.png)
111.png
**3: 发布补丁时，版本要注意**
**说明一：补丁是针对客户端具体某个版本的，patch和具体版本绑定**
eg. 应用当前版本号是1.1.0, 那么只能在后台查询到1.1.0版本对应发布的补丁, 而查询不到之前1.0.0旧版本发布的补丁.
**说明二：针对某个具体版本发布的新补丁, 必须包含所有的bugfix, 而不能依赖补丁递增修复的方式, 因为应用仅可能加载一个补丁**
eg. 针对1.0.0版本在后台发布了一个补丁版本号为1的补丁修复了bug1, 然后发现此时针对这个版本补丁1修复的不完全, 代码还有bug2, 在后台重新发布一个补丁版本号为2的补丁, 那么此时补丁2就必须同时包含bug1和bug2的修复才行, 而不是只包含bug2的修复(bug1就没被修复了)
FAQ。
**4：暂时不支持的点，期待2.0**
![909565-eff2eefed758d26f.png](https://upload-images.jianshu.io/upload_images/909565-eff2eefed758d26f.png)
不支持类型.png
最后自己测试成功的, 这里可以先用调试apk进行扫描二维码，进行本地测试一下，如果成功再发布：
![909565-6964c8b915615ec6.png](https://upload-images.jianshu.io/upload_images/909565-6964c8b915615ec6.png)
333.png
写的不对的地方，请指教，欢迎留言交流，期待2.0，
