# nodejs总结

2019年02月25日 15:30:17

 

运动人生

 

阅读数：13

 

标签： [nodejs](http://so.csdn.net/so/search/s.do?q=nodejs&t=blog)[webpack](http://so.csdn.net/so/search/s.do?q=webpack&t=blog)[nodejs项目结构](http://so.csdn.net/so/search/s.do?q=nodejs%E9%A1%B9%E7%9B%AE%E7%BB%93%E6%9E%84&t=blog) 更多

个人分类： [nodejs](https://blog.csdn.net/qq_38019655/article/category/8697758)



# nodejs总结

**一、安装配置：**

```
   npm config set prefix "F:\software\work\nodejs"  --> 指定安装的module放在哪个路径
　　npm config set cache "F:\software\work\nodejs\node_cache" --> 指定缓存配置的路径
　　npm config set registry=http://registry.npm.taobao.org --> 指定下载镜像的路径
　　npm install -g cnpm
　　npm install webpack --> 下载打包软件
　　npm install webpack-cli -g --> 高版本后下载webpack-cli要加上-g参数
　　npm cache clean --force --> 清除缓存
1234567
```

**二、运行参数：**

```
npm rum build  --> 启动一个项目，自动打开浏览器http://localhost:8080/#/
启动别的项目，1.CD 到该项目名目录，2：npm install 3：npm run dev 4：打开浏览器测试
cnpm install express --> 安装某个模块
cnpm update express --> 更新某个模块
cnpm uninstall express--> 卸载某个模块
-g代表全局,默认是当前路径下
cnpm publish express--> 发布某个模块
cnpm search express-->搜索某个模块
node express.js  -->执行某个js文件
123456789
```

**三、模块的引入：**

```
注意：在代码中，本地安装可以直接通过require()的方式引入；var moduleName = require('moduleName');
全局的安装是供命令行(command line)使用的，比如grunt，全局安装的方式是没有办法用require调用包的；
在项目的目录下进行模块的安装，比如:cnpm install mysql --save 参数save代表会在package.json中增加依赖备注
123
```

**四、webpack：**

```
1、创建一个项目的目录，用terminal进入该目录下，执行cnpm init命令，就会去初始化，生成的package.json文件。
2、执行cnpm install webpack --save-dev下载webpack相关的依赖
3、对css进行打包压缩 npm install css-loader style-loader --save-dev 
4、webpack --mode development 或者 production
1234
```

**五、项目结构：**

```
1、public 目录是静态文件的存储目录，包括css，javascript，图片等
2、routes.js url路由表
3、views 目录是模板引擎
4、app.js 这是网站的初始化文件，或者讲是入口文件
5、package.json 包管理文件，又或者模块管理文件
6、lib/ 跟业务无关的公共模块
7、modules 自己封装好的module
8、node_modules：存放所有的项目依赖库，配置package.json之后执行npm install后自动创建的文件夹
9、dispatch.js 主进程文件
10、worker.js 工作进程
11、common/ 跟业务相关的公共模块
12、proxy/ 数据访问代理层
123456789101112
```

**六、问题总结：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190228151209165.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4MDE5NjU1,size_16,color_FFFFFF,t_70)
1、用webpack打包时重复出现缺少@webassemblyjs相关的jar包时,先将node_modules和package-lock.json，再cnpm install,就可以。
2、代码中依赖的module下载时放正式环境，打包相关的或者插件放开发环境。
3、webpack打包时回去中webpack.config.js文件，在里面写打包相关的配置，并指定entry–入口