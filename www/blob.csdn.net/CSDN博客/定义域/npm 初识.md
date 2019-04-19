# npm 初识 - 定义域 - CSDN博客
2016年02月26日 11:13:16[定义域](https://me.csdn.net/u010278882)阅读数：587
npm 初识
# 什么是 nmp
- 表示 Node.js 的开放式模块登记和管理系统: npmjs.org
- 表示 Node.js 默认的模块管理器, 可用于下载、安装和管理 Node 模块
注: npm 随 node 安装, 不需要额外处理
# 基础命令
```
// 升级、获取帮助、查看配置
npm install npm@latest -g // npm 升级到最新版
npm help // 查看 npm 命令列表
npm help <command> // 查看特定命令的离线帮助
npm config list // 查看 nmp 配置
// 设置环境变量(设置 npm init 默认使用的值)
npm set init-author-name 'name'
npm set init-author-email 'email'
npm set init-author-url 'url'
npm set init-author-license 'mit'
// 创建模块
npm init // 生成 package.json 文件
// 查看模块信息
npm info <module>
npm info <module> <member> // 查看模块中的成员信息
// 查看安装的所有模块
npm list // 当前项目安装的模块及其依赖
npm list -global // 全局安装的
// 安装和删除模块
npm install // 没有参数，安装 package.json 中指定的全部依赖到本项目中
npm install <module> // 项目中使用
npm install -g <module>  // 全局使用
npm install <url> // 从网络安装
npm install <module> --force // 无论是否安装都强制安装
rm - rf <module> // 删除模块（普通的删除文件操作）
// 指定安装版本
npm install <module>@<version>
<version> 
1. latest 最新版
2. 0.1.1 版本号
3. 版本范围 '>1.0 < 2.0'
```
# 参考
[npm 模块管理器](http://javascript.ruanyifeng.com/nodejs/npm.html)
