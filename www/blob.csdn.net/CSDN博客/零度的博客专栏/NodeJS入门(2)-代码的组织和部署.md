# NodeJS入门(2)-代码的组织和部署 - 零度的博客专栏 - CSDN博客
2016年11月02日 11:44:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：969
1.模块路径解析规则：nodejs支持三种解析方式：/或C：开头的绝对路径；./开头的绝对路径；按照一定规则解析路径，直到找到模块位置。
       ◦内置模块：如果传递给require的是NodeJS内置模块名称，则不解析，直接返回内部模块导出对象。
       ◦node_modules目录：node_modules目录用于存放模块。如某个模块的绝对路径是/home/user/hello.js,在该模块中使用require('foo/bar')方式加载模块时，则寻找顺序如下：
       /home/user/node_modules/foo/bar
       /home/node_modules/foo/bar
       /node_modules/foo/bar
       如果设置了环境变量NODE_PATH=/home/user/lib:/home/lib,则寻找的顺序如下：
       /home/user/lib/foo/bar
       /home/lib/foo/bar
2.包：JS模块对应单个JS文件，放在一个目录内的多个子模块就形成了包。在组成一个包的所有子模块中，需要有一个入口模块，入口模块的导出对象被作为包的导出对象。示例如下：
- D:\test
    - cat/
        head.js
        body.js
        main.js
 cat目录定义了一个包，它包含了3个子模块，main.js为入口模块，内容如下：
var head = require('./head');
var body = require('./body');
exports.create = function (name) {
    return {
        name: name,
        head: head.create(),
        body: body.create()
    };
};
3.在其它模块使用时，需要使用require加载入口模块能达到目的。但是，我们可以让包看起来更像是个单个模块。如果模块的文件名是index.js,则加载模块时可以直接写到所在目录即可。也就是说，以下两条语句是等价的：var cat = require('d:/test/cat');和var cat = require('d:/test/cat/index');
4.如果想自定义入口模块的文件名和存放位置，就需要在包下新建一个package.json文件，并在文件中指定入口模块的路径。所以，上边的模块可以重构如下：
- D:\test
    - cat/
        +doc/
        - lib/
            head.js
            body.js
            main.js
        +tests/
        package.json
 然后在package.json中，注明模块的入口：
{
        "name": "cat",
        "main": "./lib/main.js"
 }
 5.使用NodeJS编写的程序，要么是包，要么就是一个命令行，包最终也是服务于命令行的。在Linux中，可通过shell脚本的#!注释来指定当前脚本使用的解析器。运行步骤如下：
     ◦在main.js中顶部增加#! /usr/bin/env node，表明当前脚本使用NodeJS解析。NodeJS执行时会自动忽略掉这行。
     ◦使用chmod +x main.js授于该文件执行权限。
     ◦在PATH环境变量中指定的某个目录下，例如：‘/usr/local/bin’下边创建一个软链文件，文件名与我们希望使用的终端命令同名，命令如下：sudo ln -s /home/user/bin/main.js /usr/local/bin/main
      这样，在任何目录下，我们在任何目录下都可以使用main命令了
      而且windows中，则靠cmd来解决问题。假如node-echo.js放在C:\user\bin目录，并且该目录已经添加到PATH环境变量中。则需要在该目录新建一个名为node-echo.cmd的文件，文件内容如下：@node "C:\User\user\bin\node-echo.js" %*，这样，我们就能在任何目录下使用node-echo命令了。
6.接下我们规划一下一个完整的工程目录结构，包含提供命令行模式和API模式两种使用方式，并且我们会借助三方包。所以，目录如下：
 - /home/user/workspace/node-echo/   # 工程目录
     - bin/                          # 存放命令行相关代码
         node-echo
     + doc/                          # 存放文档
     - lib/                          # 存放API相关代码
         echo.js
     - node_modules/                 # 存放三方包
         + argv/
     + tests/                        # 存放测试用例
     package.json                    # 元数据文件
     README.md                       # 说明文件
 部分代码如下：
 /* bin/node-echo */
 var argv = require('argv'),
     echo = require('../lib/echo');
 console.log(echo(argv.join(' ')));
 /* lib/echo.js */
 module.exports = function (message) {
     return message;
 };
 /* package.json */
 {
     "name": "node-echo",
     "main": "./lib/echo.js"
 }
 在上边这个例子中，通过node_moudles目录直接使用第三方包名加载模块。此外，定义了package.json之后，node-echo目录也可被当作一个包来使用。
7.NPM：npm是随同nodeJS一起安装的包管理工具，能解决NodeJS代码部署上的很多问题。常见应用场景如下： ◦允许用户从NPM服务器下载别人编写的三方包到本地使用。
◦允许用户从NPM服务器下载并安装别人编写的三方包到本地使用。
◦允许用户将自已编写的包或命令行程序上传到NPM服务器供别人使用。
8.NPM下载第三方包：在npmjs.org提代了搜索框可以根据包名来搜索，如果不知道名字，直接问度娘。确定了包名后，就可以在工程目录中打开终端，使用如下命令来开载第三方包：npm install argv(假如包名叫argv)，下载好的argv包就放在了工程目录下的node_modules目录下，所以在代码中直接require(\'argv\')的方式使用就行。
 如果下载指定版本，则在包名后加上@，例如通过以下命令可下载0.0.1版：npm install [argv@0.0.1](mailto:argv@0.0.1)
如果同时下载多个包，则可以通过package.json 中的字段配置实现。在该文件中申明第三方依赖包，然后在工程目录中使用npm install就可以批量安装第三方包了。更重要的是，如果依赖的包再次依赖了别的包，则会自动下载这些包。一个典型的package.json文件内容如下：
 - project/
     - node_modules/
         - node-echo/
             - node_modules/
                 + argv/
             ...
     ...
 9.安装命令行程序：该方法与安装第三方包相似。假如上边的例子中node-echo提供了命令行安装方式，那么只需要配置package.json ，就可以通过npm install node-echo -g命令来安装了。其中-g表示全局安装。NPM还会自动创建好Linux系统下需要的软链文件或Windows系统下需要的.cmd文件。它们在linux和windows安装的目录分别如下：
 - /usr/local/               # Linux系统下
     - lib/node_modules/
         + node-echo/
         ...
     - bin/
         node-echo
         ...
     ...
 - %APPDATA%\npm\            # Windows系统下
     - node_modules\
         + node-echo\
         ...
     node-echo.cmd
     ...
10.发布代码：第一次发布前需要注册帐号，在命令行下运行npm adduser，之后按照提示做即可。然后编辑package.json文件，加入npm必需要字段。接着上边node-echo的例子，packag.json里必要的字段如下： {
    "name": "node-echo",           # 包名，在NPM服务器上须要保持唯一
    "version": "1.0.0",            # 当前版本号
    "dependencies": {              # 三方包依赖，需要指定包名和版本号
        "argv": "0.0.2"
      },
    "main": "./lib/echo.js",       # 入口模块位置
    "bin" : {
        "node-echo": "./bin/node-echo"      # 命令行程序名和主模块位置
    }
}
之后，我们就可以在package.json所在目录下运行npm publish发布代码了。
11.版本号：NPM使用语义版本号来管理代码：X.Y.Z三位，分别代表主版本号、次版本号和补丁版本号。当代码变更时，版本号按以下原则更新：
+ 如果只是修复bug，需要更新Z位。
+ 如果是新增了功能，但是向下兼容，需要更新Y位。
+ 如果有大变动，向下不兼容，需要更新X位。
在申明三方包依赖时，除了可依赖于一个固定版本号外，还可依赖于某个范围的版本号。例如"argv": "0.0.x"表示依赖于0.0.x系列的最新版argv。NPM支持的所有版本号范围指定方式可以查看官方文档。
12.package.json中其它有用的字段，可以在这儿查看官方文档。
13.NPM常用命令：
NPM提供了很多命令，例如`install`和`publish`，使用`npm help`可查看所有命令。
使用`npm help <command>`可查看某条命令的详细帮助，例如`npm help install`。
在`package.json`所在目录下使用`npm install . -g`可先在本地安装当前命令行程序，可用于发布前的本地测试。
使用`npm update <package>`可以把当前目录下`node_modules`子目录里边的对应模块更新至最新版本。
使用`npm update <package> -g`可以把全局安装的对应命令行程序更新至最新版。
使用`npm cache clear`可以清空NPM本地缓存，用于对付使用相同版本号发布新版本代码的人。
使用`npm unpublish <package>@<version>`可以撤销发布自己发布过的某个版本代码。
