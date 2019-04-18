# Angular2学习笔记 - zwvista - 博客园

## [Angular2学习笔记](https://www.cnblogs.com/zwvista/p/8989047.html)

### Angular2

这里 Angular2 是指采用 TypeScript 语言的 Angular 2.0及以上版本。与采用 JavaScript 语言的 AngularJS 相比，Angular2 不仅底层语言有变化，框架结构也完全不同。

### Angular CLI

Angular CLI 是用来实现诸如创建 Angular 工程，添加 Angular 组件，测试打包发布 Angular 应用程序等一系列功能的一个命令行工具。

CLI 是 Command Line Interface（命令行界面）的缩写。

安装 Angular CLI 需要预先安装 Node 和 NPM。

```
# 安装 Angular CLI
$ npm install -g @angular/cli
# 创建新的应用程序 my-app
$ ng new my-app
# 进入程序所在路径
$ cd my-app
# 安装依赖
$ npm install
# 编译并运行
$ ng serve --open
# http://localhost:4200 打开主页
# 添加 heroes 组件
$ ng generate component heroes
# 添加 hero 服务
$ ng generate service hero
```

### Components（组件）

组件是构成 Angular 应用程序的基本元素。一个组件由四个文件构成。

```
app 组件（工程创建时已经包含在内）包含以下文件：
app.component.css
app.component.html
app.component.spec.ts
app.component.ts
```

通过使用命令 ng generate component xxx 可以添加 xxx 组件。

### Services（服务）

服务是构成 Angular 应用程序的重要元素。一个服务由两个文件构成。

```
message 服务包含以下文件：
message.service.spec.ts
message.service.ts
```

通过使用命令 ng generate service xxx 可以添加 xxx 服务。

### Ecosystem（生态周边）

```
# ng-bootstrap
$ npm install --save @ng-bootstrap/ng-bootstrap
# PrimeNG
$ npm install primeng --save
# Angular Material
$ npm install --save @angular/material @angular/cdk
```

### 参考链接

官方网站： [https://angular.io/guide/quickstart](https://angular.io/guide/quickstart)

参考书：[https://angular-2-training-book.rangle.io](https://angular-2-training-book.rangle.io)
[Using Bootstrap with Angular](https://medium.com/codingthesmartway-com-blog/using-bootstrap-with-angular-c83c3cee3f4a)
[PrimeNG](https://www.primefaces.org/primeng/#/)
[How to use JavaScript libraries in Angular 2+ apps?](https://hackernoon.com/how-to-use-javascript-libraries-in-angular-2-apps-ff274ba601af)


