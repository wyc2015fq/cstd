# 通过做一个基于Node的微服务器来学习Docker - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [小谢](http://www.jobbole.com/members/xiezhigang) 翻译。未经许可，禁止转载！
英文出处：[dwmkerr](http://www.dwmkerr.com/learn-docker-by-building-a-microservice/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
如果你正准备着手学习 Docker，别再观望，动起手来吧！
在这篇文章中，我将告诉你 Docker 是如何工作的？使用中会遇到什么问题？如何通过 Docker 完成一个基本的开发任务——构建一个微服务器。
我们将以一台配有 Node.js 服务和 MySQL 后台的服务器为例，从在本地运行代码开始，完成一个运行着微服务和数据库的容器。
![](http://www.dwmkerr.com/content/images/2016/04/Article.png)
## 什么是 Docker ？
从本质上来说，Docker 是一种软件，让用户创建镜像文件（就像虚拟机中的模板），然后在容器中运行这个镜像的实例。
Docker 维护着有着大量镜像的存储库，名字叫 [Docker Hub](http://hub.docker.com) ，你可以将它作为尝试镜像的起始点，或者用来免费存储你的镜像。你可以安装 Docker ，选择你喜欢的镜像，然后在容器中运行它的实例。
本文我们将介绍创建镜像、从镜像创建容器等一系列内容。
## 安装 Docker
如果你想跟上本文的节奏，那么你需要安装 Docker 。
点击 [docs.docker.com/engine/installation](https://docs.docker.com/engine/installation/) 这个链接，在上面查看适合你的系统的安装向导。
如果你是 Mac 或者 Windows 操作系统，那么你需要使用虚拟机。我在 Mac OS X 上使用 Parallels 安装 Ubuntu 虚拟机来应付大多数的开发任务。因为它支持快照功能，当你做实验的时候，他可以方便的将破坏了的环境恢复回去。
### 试试看
输入以下命令：

Shell
```
docker run -it ubuntu
```
一段时间后，你将会看到如下提示：

Shell
```
root@719059da250d:/#
```
``试试如下的命令，然后退出容器：

Shell
```
root@719059da250d:/# lsb_release -a  
No LSB modules are available.  
Distributor ID:    Ubuntu  
Description:    Ubuntu 14.04.4 LTS  
Release:    14.04  
Codename:    trusty  
root@719059da250d:/# exit
```
``这看起来没什么，但是其实在后台发生了很多事情。
你看到的是在你的机器上运行着的 Ubuntu 的隔离容器环境里的 bash shell。这个环境完全归你所有——可以在上面安装软件，运行软件，可以做任何你想做的事情。
下图表明了刚刚发生了什么（图来自于《[理解 Docker 架构](https://docs.docker.com/v1.8/introduction/understanding-docker/) 》一文）：
![Docker Run Flow](http://www.dwmkerr.com/content/images/2016/04/Flow.png)
1. 列出如下的 Docker 指令：
- docker ： 运行 docker 客户端
- run ： 运行一个新的容器
- -it ：让容器带有“交互终端”的一个参数
- ubuntu ： 容器所依赖的基础镜像
2. 在主机（我们的机器）上运行的 docker 服务检查本地是否有所请求的镜像拷贝——这里发现没有。
3. docker 服务检查公有存储库（the docker hub），看是否有可用的名为 `ubuntu` 的镜像——这里发现有。
4. docker 服务下载镜像，将其存储到本地缓存里（为了下一次直接使用）。
5. docker 服务基于 `ubuntu 镜像创建新的容器。`
Try any of these:
试试下面这些命令：

Shell
```
docker run -it haskell  
docker run -it java  
docker run -it python
```
我们没准备使用 [Haskell](https://xkcd.com/1312/) ，但是你可以看到，搭建一个环境是多么容易。
构建自己的镜像也很轻松，可以在这上面安装应用程序或者服务，可以是数据库，或者是其他你需要的。随后就可以在任意安装了 Docker 的机器上运行它们——要保证镜像是相同的、可预测的方式在每台机器上运行。我们可以将软件及其运行所需的环境整体构建成代码，并且轻松部署。
让我们以一个简单微服务器为例。
## 概述
我们将要用 Node.js 和 MySQL 创建一个让我们管理邮件地址到电话号码目录的微服务。
## 开始
要完成本地开发，需要安装MySQL，并且创建一个测试数据库…
…摇头。
创建本地数据库，并且上面运行脚本，这很容易，但是可能会带来一些问题。很多不受控制的事情开始了。它可能工作，我们甚至可以通过提交进代码库的 shell 脚本来控制这些步骤，但是如果其他开发人员已经安装了 MySQL 了呢？如果他们的数据库已经使用了我们想要创建的名称  ‘users’  了呢？
### 第一步：在 Docker 中创建一个数据库测试服务器
这是很好的 Docker 应用场景。我们可能不想在 Docker 里运行生产环境数据库（比如可能会使用 Amazon RDS），但是可以使用 Docker 容器创建一个干净的 MySQL 数据库做开发——让我们的开发及其保持干净，并且保证所有东西都在控制中，并且可重复使用。
运行下面的命令：

Shell
```
docker run --name db -d -e MYSQL_ROOT_PASSWORD=123 -p 3306:3306 mysql:latest
```
该命令启动一个运行着的 MySQL 实例，通过 3306 端口访问，root 密码为 123 。
- ` docker run` 告诉引擎，用户想要运行一个镜像（在最后传入的是镜像，[mysql:latest](https://hub.docker.com/_/mysql/) ）
-  –name db 将整个容器命名为 db 。
-  -d detach，在后台运行容器。
-  -e MYSQL_ROOT_PASSWORD=123（或者是 –env）环境变量 – 参数告诉 docker 所提供的环境变量。这之后跟着的变量正是 MySQL 镜像检查且用来设置的默认 root 密码。
-  -p` 3306:3306（或者 --publish）` 告诉引擎用户想要将容器内的3306端口映射到外部的3306端口上。
最后一部分很重要——即使这是 MySQL 的默认端口，如果用户不显式告诉 docker 想要映射的端口，docker 就会阻塞该端口的访问（因为容器默认是隔离的，直到用户告诉 docker 想要访问它们）。
该命令返回值是容器 id，这是容器的指针，用户可以用它来停止容器，向容器发送命令等等。让我们看看正在运行的是哪些容器：

Shell
```
$ docker ps
CONTAINER ID  IMAGE         ...  NAMES  
36e68b966fd0  mysql:latest  ...  db
```
关键的信息是容器 ID，镜像和名称。连接到这个镜像看看里面有什么：

Shell
```
$ docker exec -it db /bin/bash
root@36e68b966fd0:/# mysql -uroot -p123  
mysql> show databases;  
+--------------------+
| Database           |
+--------------------+
| information_schema |
+--------------------+
1 rows in set (0.01 sec)
mysql> exit  
Bye  
root@36e68b966fd0:/# exit
```
下面这么做也很有意思：
1. `docker exec -it db` ：告诉 docker 用户想要在名为 `db` 的容器里执行一个命令（我们也可以使用 id，或者 id 的前几个字母）。 `-it` 确保用户有交互型终端。
2. `mysql -uroot -p123` ：我们实际在容器里作为进程运行的命令，这里是 mysql 客户端。
我们可以创建数据库，表，用户，其他你需要的等等。
### 打包测试数据库
在容器内运行 MySQL 需要一些 Docker 技巧，但是让我们先打住，看看服务。现在，使用脚本创建一个 `test-database` 目录来启动数据库，停止数据库以及搭建测试数据：

Shell
```
test-databasesetup.sql  
test-databasestart.sh  
test-databasestop.sh
```
启动脚本很简单：

Shell
```
#!/bin/sh
# Run the MySQL container, with a database named 'users' and credentials
# for a users-service user which can access it.
echo "Starting DB..."  
docker run --name db -d   
  -e MYSQL_ROOT_PASSWORD=123 
  -e MYSQL_DATABASE=users -e MYSQL_USER=users_service -e MYSQL_PASSWORD=123 
  -p 3306:3306 
  mysql:latest
# Wait for the database service to start up.
echo "Waiting for DB to start up..."  
docker exec db mysqladmin --silent --wait=30 -uusers_service -p123 ping || exit 1
# Run the setup script.
echo "Setting up initial data..."  
docker exec -i db mysql -uusers_service -p123 users < setup.sql
```
该脚本在一个分离容器钟运行数据库镜像（比如，在后台运行），创建了一个用户来访问 `users` 数据库，然后等待数据库服务器启动，随后运行 `setup.sql` 脚本来设置初始数据。
setup.sql 的内容是：

MySQL
```
create table directory (user_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, email TEXT, phone_number TEXT);  
insert into directory (email, phone_number) values ('homer@thesimpsons.com', '+1 888 123 1111');  
insert into directory (email, phone_number) values ('marge@thesimpsons.com', '+1 888 123 1112');  
insert into directory (email, phone_number) values ('maggie@thesimpsons.com', '+1 888 123 1113');  
insert into directory (email, phone_number) values ('lisa@thesimpsons.com', '+1 888 123 1114');  
insert into directory (email, phone_number) values ('bart@thesimpsons.com', '+1 888 123 1115');
```
`stop.sh` 脚本会停止容器并且删除容器（docker 默认会保留容器，这样能够快速重启，本示例中并不需要这样）：

Shell
```
#!/bin/sh
# Stop the db and remove the container.
docker stop db && docker rm db
```
之后会进一步简化这个过程，让它更加顺畅。在 repo 里的 [step1](https://github.com/dwmkerr/node-docker-microservice/tree/step1) 分支里查看这一阶段的代码。
### 第二步：用 Node.js 创建一个微服务
本文的主题是 Docker 的学习，因此并不会花太多篇幅讲解 Node.js 的微服务。只是强调一些重点。

Shell
```
test-database/          # contains the code seen in Step 1  
users-service/          # root of our node.js microservice  
- package.json          # dependencies, metadata
- index.js              # main entrypoint of the app
- api/                  # our apis and api tests
- config/               # config for the app
- repository/           # abstraction over our db
- server/               # server setup code
```
让我们仔细看看这一部分。首先看看这个代码库。最好将你的数据库访问封装和抽象成一些类，允许模拟它来实现测试目的：

JavaScript
```
//  repository.js
//
//  Exposes a single function - 'connect', which returns
//  a connected repository. Call 'disconnect' on this object when you're done.
'use strict';
var mysql = require('mysql');
//  Class which holds an open connection to a repository
//  and exposes some simple functions for accessing data.
class Repository {  
  constructor(connection) {
    this.connection = connection;
  }
  getUsers() {
    return new Promise((resolve, reject) => {
      this.connection.query('SELECT email, phone_number FROM directory', (err, results) => {
        if(err) {
          return reject(new Error("An error occured getting the users: " + err));
        }
        resolve((results || []).map((user) => {
          return {
            email: user.email,
            phone_number: user.phone_number
          };
        }));
      });
    });
  }
  getUserByEmail(email) {
    return new Promise((resolve, reject) => {
      //  Fetch the customer.
      this.connection.query('SELECT email, phone_number FROM directory WHERE email = ?', [email], (err, results) => {
        if(err) {
          return reject(new Error("An error occured getting the user: " + err));
        }
        if(results.length === 0) {
          resolve(undefined);
        } else {
          resolve({
            email: results[0].email,
            phone_number: results[0].phone_number
          });
        }
      });
    });
  }
  disconnect() {
    this.connection.end();
  }
}
//  One and only exported function, returns a connected repo.
module.exports.connect = (connectionSettings) => {  
  return new Promise((resolve, reject) => {
    if(!connectionSettings.host) throw new Error("A host must be specified.");
    if(!connectionSettings.user) throw new Error("A user must be specified.");
    if(!connectionSettings.password) throw new Error("A password must be specified.");
    if(!connectionSettings.port) throw new Error("A port must be specified.");
    resolve(new Repository(mysql.createConnection(connectionSettings)));
  });
};
```
其实有很多种其他实现方式！但是我们可以像下面这样创建 `Repository `对象：

JavaScript
```
repository.connect({  
  host: "127.0.0.1",
  database: "users",
  user: "users_service",
  password: "123",
  port: 3306
}).then((repo) => {
  repo.getUsers().then(users) => {
    console.log(users);
  });
  repo.getUserByEmail('homer@thesimpsons.com').then((user) => {
    console.log(user);
  })
  //  ...when you are done...
  repo.disconnect();
});
```
在 `repository/repository.spec.js `文件里也有一系列的单元测试。得到 repo 后，就可以创建服务器了。`server/server.js `如下：

JavaScript
```
//  server.js
var express = require('express');  
var morgan = require('morgan');
module.exports.start = (options) => {
  return new Promise((resolve, reject) => {
    //  Make sure we have a repository and port provided.
    if(!options.repository) throw new Error("A server must be started with a connected repository.");
    if(!options.port) throw new Error("A server must be started with a port.");
    //  Create the app, add some logging.
    var app = express();
    app.use(morgan('dev'));
    //  Add the APIs to the app.
    require('../api/users')(app, options);
    //  Start the app, creating a running server which we return.
    var server = app.listen(options.port, () => {
      resolve(server);
    });
  });
};
```
该模块暴露了一个 `start `函数，可以像下面这样使用：

JavaScript
```
var server = require('./server/server);  
server.start({port: 8080, repo: repository}).then((svr) => {  
  // we've got a running http server :)
});
```
注意到 `server.js 中`使用了 `api/users/js 吧`？代码如下：

JavaScript
```
//  users.js
//
//  Defines the users api. Add to a server by calling:
//  require('./users')
'use strict';
//  Only export - adds the API to the app with the given options.
module.exports = (app, options) => {
  app.get('/users', (req, res, next) => {
    options.repository.getUsers().then((users) => {
      res.status(200).send(users.map((user) => { return {
          email: user.email,
          phoneNumber: user.phone_number
        };
      }));
    })
    .catch(next);
  });
  app.get('/search', (req, res) => {
    //  Get the email.
    var email = req.query.email;
    if (!email) {
      throw new Error("When searching for a user, the email must be specified, e.g: '/search?email=homer@thesimpsons.com'.");
    }
    //  Get the user from the repo.
    options.repository.getUserByEmail(email).then((user) => {
      if(!user) { 
        res.status(404).send('User not found.');
      } else {
        res.status(200).send({
          email: user.email,
          phoneNumber: user.phone_number
        });
      }
    })
    .catch(next);
  });
};
```
这些文件都有和源码匹配的单元测试。
我们还需要配置。与其使用特定的库函数，不如使用一个简单的文件 – `config/config.js ：`

JavaScript
```
//  config.js
//
//  Simple application configuration. Extend as needed.
module.exports = {  
    port: process.env.PORT || 8123,
  db: {
    host: process.env.DATABASE_HOST || '127.0.0.1',
    database: 'users',
    user: 'users_service',
    password: '123',
    port: 3306
  }
};
```
我们可以按需进行配置。目前，大部分配置是硬编码的，但是从`端口的配置中`可以看出，我们可以很容易的通过添加环境变量的方式来改变它。
最后一步 – 将它和包含所有东西的 `index.js `文件连接到一起：

JavaScript
```
//    index.js
//
//  Entrypoint to the application. Opens a repository to the MySQL
//  server and starts the server.
var server = require('./server/server');  
var repository = require('./repository/repository');  
var config = require('./config/config');
//  Lots of verbose logging when we're starting up...
console.log("--- Customer Service---");  
console.log("Connecting to customer repository...");
//  Log unhandled exceptions.
process.on('uncaughtException', function(err) {  
  console.error('Unhandled Exception', err);
});
process.on('unhandledRejection', function(err, promise){  
  console.error('Unhandled Rejection', err);
});
repository.connect({  
  host: config.db.host,
  database: config.db.database,
  user: config.db.user,
  password: config.db.password,
  port: config.db.port
}).then((repo) => {
  console.log("Connected. Starting server...");
  return server.start({
    port: config.port,
    repository: repo
  });
}).then((app) => {
  console.log("Server started successfully, running on port " + config.port + ".");
  app.on('close', () => {
    repository.disconnect();
  });
});
```
我们做了一点错误处理，在此之上仅仅加载了配置，创建了 repo 并且启动了服务器。
这就是微服务，它让用户能够得到所有用户，或者搜索某个用户：

Shell
```
HTTP GET /users                              # gets all users  
HTTP GET /search?email=homer@thesimpons.com  # searches by email
```
如果下载了相关代码，可以发现有一些可用的命令：

Shell
```
cd ./users-service  
npm install         # setup everything  
npm test            # unit test - no need for a test database running  
npm start           # run the server - you must have a test database running  
npm run debug       # run the server in debug mode, opens a browser with the inspector  
npm run lint        # check to see if the code is beautiful
```
除了代码之外，我们完成了：
1. 用于调试的 Node 面板
2. 用于单元测试的 Mocha/shoud/supertest
3. 用于 linting 的 ESlint
大功告成！
使用如下命令运行数据库测试：

Shell
```
cd test-database/  
./start.sh
```
然后启动服务：

Shell
```
cd ../users-service/  
npm start
```
可以用浏览器打开 [localhost:8123/users](http://localhost:8123/users)，就可以看到数据库已经可以使用了。如果使用的是 Docker Machine（假设，在 Mac 或者 Windows 上），那么 `localhost `无法访问，你需要使用 docker 的 IP。可以通过 `docker-machine ip `得到 IP 地址。
可以看到，我们很快完成了服务的创建。继续下一步之前如果想查看代码，见 [step2](https://github.com/dwmkerr/node-docker-microservice/tree/step2) 分支。
# 第三步：微服务 Docker 化
现在开始变得有趣啦！
我们已经有了一个可以运行在开发环境里的微服务，只要它和安装的 Node.js 版本兼容即可。这一步想做的是搭建起我们的服务，这样可以从其中创建一个 Docker Image，从而可以将服务部署到任何支持 docker 的地方。
要达到这一目的，需要创建一个 Dockerfile。Dockerfile 告诉 Docker 引擎如何构建镜像。我们会在 `users-service `目录下创建一个简单的 Dockerfile，并且研究如何通过修改它来适应需求。
## 创建 Dockerfile
在 `users-service/ `目录下创建名为 `Dockerfile `的文本文件，内容如下：

Shell
```
# Use Node v4 as the base image.
FROM node:4
# Run node 
CMD ["node"]
```
运行如下命令行构建镜像，并在镜像上运行容器：

Shell
```
docker build -t node4 .    # Builds a new image  
docker run -it node4       # Run a container with this image, interactive
```
首先看看构建命令。
1. `docker build `告诉引擎用户需要创建一个新的镜像
2. `-t node4 `使用标签 `node4 `标记该镜像。之后就可以使用这个标签来指代该镜像。
3. `. `在当前目录里查找 `Dockerfile`.
控制台打出一些输出之后，就可以看到新的镜像创建好了。使用 `docker images `命令可以在系统里看到所有镜像。下面的命令和之前的很类似：
1. `docker run `从某个镜像里运行新容器
2. `-it `使用交互式终端
3. `node4 `是想要在容器里使用的镜像的标签。
当运行该镜像时，会得到 node repl，运行如下命令检查当前版本：

Shell
```
> process.version
'v4.4.0'  
> process.exit(0)
```
这很可能和你当前机器上的 node 版本不同。
## 检查 Dockerfile
从 Dockerfile 里可以很容易看出发生了什么：
1. `FROM node:4 `在 Dockerfile 里指定的第一件事就是基础镜像。[docker hub上的 Node 官方页面](https://hub.docker.com/_/node/)可以搜索列出所有可用镜像。这里用的是安装了 node 的 ubuntu。
2. `CMD ["node"] `里的  `CMD `告诉 docker 该镜像需要运行 node 程序。当 node 程序终止时，容器会关闭。
使用额外的几个命令，可以更新 Dockerfile，从而运行服务：

Shell
```
# Use Node v4 as the base image.
FROM node:4
# Add everything in the current directory to our image, in the 'app' folder.
ADD . /app
# Install dependencies
RUN cd /app;   
    npm install --production
# Expose our server port.
EXPOSE 8123
# Run our app.
CMD ["node", "/app/index.js"]
```
唯一的改变是使用了 `ADD `命令将当前目录下的所有东西拷贝到名为 `app/ `的容器目录里。随后使用 `RUN `在镜像里运行命令，该命令安装了模块。最后，`EXPOSE `了服务器端口，告诉 docker 想要支持 `8123 `端口的连接，然后运行服务器代码。
确保 test-database 服务已经运行着，然后再次构建并且运行镜像：

Shell
```
docker build -t users-service .  
docker run -it -p 8123:8123 users-service
```
如果在浏览器里查看 [`localhost:8123/users`](8123/users)，会看到一个错误，检查控制台，提示容器报告了一些问题：

Shell
```
--- Customer Service---
Connecting to customer repository...  
Connected. Starting server...  
Server started successfully, running on port 8123.  
GET /users 500 23.958 ms - 582  
Error: An error occured getting the users: Error: connect ECONNREFUSED 127.0.0.1:3306  
    at Query._callback (/app/repository/repository.js:21:25)
    at Query.Sequence.end (/app/node_modules/mysql/lib/protocol/sequences/Sequence.js:96:24)
    at /app/node_modules/mysql/lib/protocol/Protocol.js:399:18
    at Array.forEach (native)
    at /app/node_modules/mysql/lib/protocol/Protocol.js:398:13
    at nextTickCallbackWith0Args (node.js:420:9)
    at process._tickCallback (node.js:349:13)
```
我勒个去！从 `users-service `容器到 `test-database `容器的连接被拒绝了。运行 `docker ps `查看所有运行着的容器：

Shell
```
CONTAINER ID  IMAGE          PORTS                   NAMES  
a97958850c66  users-service  0.0.0.0:8123->8123/tcp  kickass_perlman  
47f91343db01  mysql:latest   0.0.0.0:3306->3306/tcp  db
```
这两个容器都运行着呢，到底怎么回事呢？
## 连接容器
我们看到的问题实际上是可以预期的。Docker 容器应该是互相隔离的，因此如果不显式地允许容器间连接的话就容器间就无法互联。
是的，用户可以从自己的机器（宿主机）连接到容器里，因为我们为这样的连接开启了端口（比如，使用了`-p 8123:8123`）。如果以同样的方式允许容器间互联，那么运行在同一台机器上的两个容器之间就应该能够通信，即使开发人员不想这么做。并且这是灾难性的，尤其是我们在集群的机器上利用容器运行不同的应用程序的时候。
如果想要从某个容器连接到另一个容器，需要连接这两个容器，告诉 docker，用户显式想要允许这两个容器间通信。有两种方式可以完成这一目标，第一种是“不流行的旧方式”但是非常简单，第二种之后会介绍。
### 使用 link 参数连接容器
当运行容器时，可以使用 `link `参数告诉 docker 我们想要连接到另外的容器上。本文示例中，可以通过如下命令正确运行服务：

Shell
```
docker run -it -p 8123:8123 --link db:db -e DATABASE_HOST=DB users-service
```
1. `docker run -it `在容器里运行 docker 镜像，使用交互式终端。
2. `-p 8123:8123 `将宿主机的 8123 端口映射到容器的 8123 端口上
3. `link db:db `连接到名为 `db`的 容器上，并且用 `db `指代该容器
4. `-e DATABASE_HOST=db `将环境变量 `DATABASE_HOST `设置为 `db`
5. `users-service `是在容器内运行的镜像名称
现在当我们访问 `[localhost:8123/users](8123/users) `时一切工作正常。
#### 它是如何工作的呢？
还记得服务的配置文件么？它让用户能够使用环境变量指定数据库的主机名：

JavaScript
```
//  config.js
//
//  Simple application configuration. Extend as needed.
module.exports = {  
    port: process.env.PORT || 8123,
  db: {
    host: process.env.DATABASE_HOST || '127.0.0.1',
    database: 'users',
    user: 'users_service',
    password: '123',
    port: 3306
  }
};
```
运行容器时，将环境变量设置为 `DB`，这意味着要连接到一个名字为 `DB `的主机上。当连接到容器上时，docker 引擎会自动为我们设置好一切。
尝试运行 `docker ps `列出所有运行着的容器。查询运行 `users-service `的容器名称，这是个随机名称，例如 t`rusting_jang`:

Shell
```
docker ps  
CONTAINER ID  IMAGE          ...   NAMES  
ac9449d3d552  users-service  ...   trusting_jang  
47f91343db01  mysql:latest   ...   db
```
现在可以看到容器可用的主机：

Shell
```
docker exec trusting_jang cat /etc/hosts  
127.0.0.1    localhost  
::1    localhost ip6-localhost ip6-loopback
fe00::0    ip6-localnet  
ff00::0    ip6-mcastprefix  
ff02::1    ip6-allnodes  
ff02::2    ip6-allrouters  
172.17.0.2    db 47f91343db01    # linking magic!!  
172.17.0.3    ac9449d3d552
```
还记得 `docker exec `是怎么工作的吗？选择一个容器名，之后跟着想在容器上执行的命令，在本例中是 `cat /etc/hosts`。
好了，主机文件之前可没有 `# linking magic!! `注释，可以看到 -docker 将 `db `添加到了主机文件里，因此可以通过主机名连接到容器上。这是连接信息：

Shell
```
docker exec trusting_jang printenv | grep DB  
DB_PORT=tcp://172.17.0.2:3306  
DB_PORT_3306_TCP=tcp://172.17.0.2:3306  
DB_PORT_3306_TCP_ADDR=172.17.0.2  
DB_PORT_3306_TCP_PORT=3306  
DB_PORT_3306_TCP_PROTO=tcp  
DB_NAME=/trusting_jang/db
```
从该命令还可以看到当 docker 连接容器时，它还提供了一系列包含有用信息的环境变量，比如，主机名，tcp 端口和容器名。
第3步完成了 —— MySQL 数据库正常运行在容器里，还可以在本地或者在容器里运行 node.js 微服务，并且已经知道了如何连接这两者。
如果你想了解更多，可以在 [step3](https://github.com/dwmkerr/node-docker-microservice/tree/step3) 的分支里查看这一阶段的代码。
# 第4步：环境的集成测试
现在可以编写集成测试，调用实际服务器，作为 docker 容器运行，调用容器化的测试数据库。
可以用任何语言，或者在任何平台上完成集成测试，但是为了保持简洁，这里使用的是 Node.js，因为项目里已经使用了Mocha 和 Supertest。
在名为 `integration-tests `的新目录下，创建一个 `index.js`:

JavaScript
```
var supertest = require('supertest');  
var should = require('should');
describe('users-service', () => {
  var api = supertest('http://localhost:8123');
  it('returns a 200 for a known user', (done) => {
    api.get('/search?email=homer@thesimpsons.com')
      .expect(200, done);
  });
});
```
它会检查 API 调用，并且显示测试结果。
只要 `users-services `和 `test-database `正在运行，测试就能够通过。但是，这时候服务开始变得有点难处理：
1. 需要使用 shell 脚本来启动和停止数据库
2. 需要记住一系列命令来基于数据库启动用户服务
3. 需要使用 node 直接运行集成测试
既然我们已经很熟悉 Docker 了，应该能够解决这些问题。
### 简化 Test 数据库
目前测试数据库有如下文件：

Shell
```
/test-database/start.sh
/test-database/stop.sh
/test-database/setup.sql
```
既然已经很熟悉 Docker 了，让我们尝试改进它们。
在 Docker Hub 上查看 [mysql 镜像文档](https://hub.docker.com/_/mysql/)，有一处注释告诉用户任何添加到镜像的 `/docker-entrypoint-initdb.d `目录的 `.sql `或者 `.sh `文件会在搭建 DB 的时候执行。
这意味着可以使用 `Dockerfile `代替 `start.sh `和 `stop.sh `。

Shell
```
FROM mysql:5
ENV MYSQL_ROOT_PASSWORD 123  
ENV MYSQL_DATABASE users  
ENV MYSQL_USER users_service  
ENV MYSQL_PASSWORD 123
ADD setup.sql /docker-entrypoint-initdb.d
```
现在运行测试数据库只需要：

Shell
```
docker build -t test-database .  
docker run --name db test-database
```
### 组合
构建并且运行每个容器仍然有些费时。可以使用 [Docker Compose](https://docs.docker.com/compose/) 工具进一步简化。
Docker Composer 允许用户创建一个文件，在其中定义系统里的每个容器，容器间的关系，并且构建或者运行它们。
首先，[安装 Docker Compose](https://docs.docker.com/compose/install/)。在项目根目录下创建一个新文件，称为 `docker-compose.yml`:

YAML
```
version: '2'  
services:  
  users-service:
    build: ./users-service
    ports:
     - "8123:8123"
    depends_on:
     - db
    environment:
     - DATABASE_HOST=db
  db:
    build: ./test-database
```
现在就可以试一下啦：

Shell
```
docker-compose build  
docker-compose up
```
Docker Compose 会构建出应用程序所需要的所有镜像，从其上创建出容器，并且以正确顺序运行容器，从而启动整个应用程序！
`docker-compose build `命令构建 `docker-compose.yml `文件里列出的每个镜像：

YAML
```
version: '2'  
services:  
  users-service:
    build: ./users-service
    ports:
     - "8123:8123"
    depends_on:
     - db
    environment:
     - DATABASE_HOST=db
  db:
    build: ./test-database
```
每个服务的 `build `值告诉 docker 到哪里找到 `Dockerfile `。当用户运行 `docker-compose up `时，docker 会启动所有服务。注意在 `Dockerfile `里，用户可以指定端口和依赖关系。实际上，在这个文件里，用户可以更改所有配置。
在另一个终端里，运行 `docker compose down`，可以正常关闭容器。
# 总结
本文里介绍了很多 docker 知识，不过这里还有一些。我希望你能够从本文找到一些有趣有用的东西，能够帮助你在工作中使用 docker。
和平常一样，欢迎提问和建议！同时强烈推荐文档《[理解 Docker](https://docs.docker.com/engine/understanding-docker/) 》，可以帮助大家更深入地理解 docker 的工作机制。
在 [github.com/dwmkerr/node-docker-microservice](https://github.com/dwmkerr/node-docker-microservice) 处可以看到本文所构建的项目的最终源码。
# 注意
1. 将所有东西完全拷贝不是什么好主意，因为那样也会拷贝 node_modules 文件夹。通常来说显式指定想要拷贝的文件或文件夹会更好，或者使用 .dockerignore 文件，它和 .gitignore 文件类似。
2. 如果服务器不正常工作，显示一个很讨厌的 exception，这是由一个bug造成的，详见 [github.com/visionmedia/supertest/issues/314](https://github.com/visionmedia/supertest/issues/314)。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/78805a221a988e79ef3f42d7c5bfd418-1.jpeg)![](http://www.jobbole.com/wp-content/uploads/2016/04/78805a221a988e79ef3f42d7c5bfd418-3.jpeg)
