# NodeJs的包 - LC900730的博客 - CSDN博客
2017年09月10日 23:13:03[lc900730](https://me.csdn.net/LC900730)阅读数：199
## 1.创建以及加载模块
### 1.创建模块
在Node.js中，创建一个模块非常简单，因为一个文件就是一个模块。
```
//module.js
var name;
exports.setName=function(thyName){
    name=thyName;
};
exports.setHello=function(){
    console.log('Hello '+name);
};
```
```java
//getmodule.js
    var myModule=require('./module')
    myModule.setName("BYVoid");
    myModule.sayHello();
```
module.js通过exports对象把setName和sayHello作为模块的访问接口，在getmodule.js中通过require(‘./module’)加载这个模块，然后就可以访问module.js中exports对象的成员函数
### 单次加载
require不会重复加载模块 
无论调用多少次require，获得的模块都是同一个
```java
var hello1=require('./module');
hello1.setName('BYVoid');
var hello2=require('./module');
hello2.setName('BYVoid 2');
hello1.sayHello();
```
运行结果是Hello BYVoid2，因为hello1和hello2指向的是同一个实例。
### 覆盖exports
有时候我们希望把一个对象封装到模块中
```java
//singleobject.js
function Hello(){
    var name;
    this.setName=function(thyName){
        name=thyName;
    };
    this.setHello=function(){
        console.log('Hello'+name);
    };
}
exports.Hello=Hello;
```
我们需要在其他文件中require(‘./singleobject’.Hello)来获取Hello对象。 
如果是module.exports=Hello;就可以直接获得这个对象。
```java
//gethello.js
var Hello=require('./hello');
hello =new Hello();
hello.setName('BYVoid');
hello.sayHello();
```
唯一变化是使用module.exports=Hello代替了exports.Hello=Hello。在外部引用该模块时，其接口对象就是要输出Hello对象本身，而不是exports。
## 2.创建包
包是在模块基础上更深一步的抽象。Node.js的包类似于C/C++的函数库或者Java/.Net的类库。它将某个独立功能封装起来，用于发布、更新、依赖管理和版本控制。
CommonJs规范
- package.json位于顶层目录下
- 二进制文件在bin目录下
- Js代码在lib目录下
- 文档在doc目录下
- 单元测试在test目录下
### 2.1作为文件夹的模块
模块与文件是一一对应的。文件不仅可以是Js代码或者二进制代码，还可以是一个文件夹。最简单的包就是一个作为文件夹的模块。 
如
```java
//somepackage/index.js
exports.hello=function () {
    console.log('Hello');
}
```
```java
//getpackage.js
    var someoackage=require('./somepackage');
    someoackage.hello();
```
node getpackage.js //Hello 
把文件夹封装为一个模块，即所谓的包。包通常是一些模块的集合，在模块基础上提供了更高层的抽象，相当于提供了一些固定接口的函数库。通过定制package.json我们可以创建更复杂、更完善、更符合规范的包用于发布。
### package.json
node.js在调用某个包的时候，会检查包中package.json文件的main字段，将其作为包的接口模块，如果package.json或main不存在，则以index.js或者index.node作为包的接口。 
- name
- description
- version
- keywords
- maintainers
- contributors
- bugs
- licenses
- repositories
- dependencies
```bash
如“dependencies”:{
    "webkit":"1.2"
    "ssl":{
        "gnutls":["1.0","2.0"],
        "openssl":"0.9.8"
    }
}
```
### 本地模式和全局模式
npm默认在[http://npmjs.org](http://npmjs.org)搜索或者下载包，安装到当前目录的node_modules子目录下。
本地模式下不会注册PATH环境变量，如安装supervisor是为了在命令行中直接使用它。npm本地模式仅仅是把包安装到node_modules下，其中bin目录没有包含在PATH环境变量中。全局安装下，package.json文件中bin字段包含的 文件会被链接到/usr/local/bin
### 创建全局链接
全局安装的包不能直接通过require使用，但是通过npm link命令可以打破这一限制。 
如我们npm install -g express 全局安装了express，这时候在工程的目录下运行命令： 
npm link express 
会发现./node_module/express->/usr/local/lib/node_modules/express
