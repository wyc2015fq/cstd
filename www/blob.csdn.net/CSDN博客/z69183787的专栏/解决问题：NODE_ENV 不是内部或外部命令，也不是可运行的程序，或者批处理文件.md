# 解决问题：NODE_ENV 不是内部或外部命令，也不是可运行的程序，或者批处理文件 - z69183787的专栏 - CSDN博客
2018年06月23日 22:16:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：631
[https://blog.csdn.net/huangpin815/article/details/75669525](https://blog.csdn.net/huangpin815/article/details/75669525)
解决windows 执行 前端node 项目的兼容性问题。。。
### 1 环境
```
win10+powershell
```
- 1
- 2
### 2 问题
我要执行的npm script如下： 
(脚本的内容不需要关注，需要关注的是它的写法。）
```bash
"scripts": {
        "build": "NODE_ENV=production webpack",
        "dev": "NODE_ENV=development node dev-server.js"
 }
```
- 1
- 2
- 3
- 4
两个脚本都报同样的错误：
```
NODE_ENV 不是内部或外部命令，也不是可运行的程序，或者批处理文件
```
- 1
- 2
事实上，以上两条脚本都合并两条命令（这种操作在powershell中不被支持，在cmd中也不被支持，这是Mac中bash或Linux的shell中的独特操作），拆分两条脚本如下：
```bash
"scripts": {
        "build": "set NODE_ENV=production && webpack",
        "dev": "set NODE_ENV=development && node dev-server.js"
    },
```
- 1
- 2
- 3
- 4
运行脚本，没有报错
以上。
