# TypeScript 的声明文件的使用与编写 - =朝晖= - 博客园
# [TypeScript 的声明文件的使用与编写](https://www.cnblogs.com/dhcn/p/7722248.html)
https://fenying.net/2016/09/19/typings-for-typescript/
TypeScript 是 JavaScript 的超集，相比 JavaScript，其最关键的功能是静态类型检查 (Type Guard)。然而 JavaScript 本身是没有静态类型检查功能的，TypeScript 编译器也仅提供了 ECMAScript 标准里的标准库类型声明，只能识别 TypeScript 代码里的类型。
那么 TypeScript 中如何引用一个 JavaScript 文件呢？例如使用 lodash，async 等著名的 JavaScript 第三方库。答案是通过**声明文件（Declaration Files）**。
## 什么是声明文件？
和 C/C++ 的 `*.h` 头文件（Header files）非常相似：当你在 C/C++ 程序中引用了一个第三方库（.lib/.dll/.so/.a/.la）时，C/C++ 编译器无法自动地识别库内导出名称和函数类型签名等，这就需要使用头文件进行接口声明了。
同理地，TypeScript 的声明文件是一个以 `.d.ts` 为后缀的 TypeScript 代码文件，但它的作用是描述一个 JavaScript 模块（广义上的）内所有导出接口的类型信息。
> 
为了简洁，下面把 **声明文件** 简称为 `Definition`。
### 网页上引用非模块化的 JavaScript 文件里的名称
```
// <script src="sample-00.js"></script>
// 通过 script 标签引入名称到 JS 的全局命名空间中。
var name = "Mick";
 
function test(inStr) {
 
return inStr.substr(0, 4);
}
```
在另一个 TypeScript 文件里引用里面的名称，不可用
```
// File: test-01.ts
console.log(name); // 编译报错，name 不存在。
console.log(test("hello")); // 编译报错，test 不存在。
```
因为 TypeScript 不能从纯 JavaScript 文件里摘取类型信息，所以 TypeScript 的编译器根本不知道变量 `name` 的存在。这一点和 C/C++ 非常相似，而解决方法也几乎一致：使用一个 Definition，把这个变量声明写进去，让其它需要使用这个变量的文件引用。
```
// File sample-00.d.ts
declare let name: string;
declare let test: (inStr: string) => string;
```
在 TypeScript 文件里使用 **三斜线指令** 引用这个文件：
```
// File: test-01.ts
/// <reference path="./sample-00.d.ts">
console.log(name); // 编译通过。
console.log(test("hello")); // 编译通过。
```
### 使用第三方库
第三方库 async 也是纯 JavaScript 库，没有类型信息。要在 TypeScript 中使用，可以到 DefinitelyTyped 组织的 GitHub 仓库里面下载一份 [`async.d.ts`](https://github.com/DefinitelyTyped/DefinitelyTyped/blob/master/async/async.d.ts) 文件，将之引用进来。
```
// File: test-02.ts
/// <reference path="./async.d.ts">
import async = require("async");
 
async.series([
 
function(next: ErrorCallback): void {
 
console.log(1);
next();
},
 
function(next: ErrorCallback): void {
 
console.log(2);
next();
},
 
function(next: ErrorCallback): void {
 
console.log(3);
next();
}
 
], function(err?: Error): void {
 
if (err) {
 
console.log(err);
 
return;
}
 
console.log("Done");
 
});
```
但是一个个库都去下载对应的 Definition ，实在太麻烦了，也不方便管理，所以我们可以使用 DefinitelyTyped 组织提供的声明管理器——typings。
## 使用 typings 声明管理器
### 安装与基本使用
typings 是一个用 Node.js 编写的工具，托管在 NPM 仓库里，通过下面的命令可以安装
```
npm install typings -g
```
现在我们要安装 async 库的 Definition 就简单了，直接一句命令行
```
typings install dt~async --global
```
> 
提示：`install` 命令可以缩写为 `i`，且可以一次安装多个 Definition 。
参数 `--global` 意义请参考我另一篇文章《TypeScript 的两种声明文件写法的区别和根本意义》。
`--global` 可简写为 `-G`。
这样， async 库的 Definition 就会被安装到 `./typings/globals/async/index.d.ts` 。可以自由地使用 async 库的 Definition 了。
如果你觉得这个路径太长了，可以使用 `./typings/index.d.ts` 这个文件。这是一个统一索引文件，**使用 typings 工具安装的所有 Definition 都会被引用添加到这个文件里**，所以通过引用这个文件，就可以轻松引用所有安装过的 Definition ！
### Definition 的源
还有，安装 Definition 命令里的 `dt~async` 是什么东西？`async` 当然是一个库的名称。那 `dt` 呢？其实 `dt` 是指**源**，表示这个 Definition 的来源。目前绝大多数的库 Definition 都是托管在 DefinitelyTyped 项目的 GitHub 仓库里面的，所以使用 `dt~库名称` 可以找到绝大部分库的 Definition 。
如果你不确定某个库 Definition 的源，可以使用下面的命令查找
```
typings search --name 库准确名称
```
一个输出例子是：
```
$ typings search --name jquery
Viewing 1 of 1
 
NAME   SOURCE HOMEPAGE           DESCRIPTION VERSIONS UPDATED
jquery dt     http://jquery.com/             1        2016-09-08T20:32:39.000Z
```
可以看出，jquery 库 Definition 信息是存在的，对应的 源（SOURCE） 是 `dt`。
### 安装某个库特定版本的 Definition
2016 年 9 月初，很多人发现通过 typings 安装的 `env~node` 在 TS 编译输出为 ES5 标准的情况下不可用了，编译报错。原因是 DefinitelyTyped 库将 `env~node` 的最新版本更新为 6.0 版本，只支持 ES6 标准了。这导致很多编译目标为 ES5 甚至 ES3 的项目都因为无法识别里面的 ES6 标准元素而出错。
解决方案是安装特定的兼容分支即可，如何安装特定版本的 Definition 呢？首先，通过 typings 工具的 info 命令查看某个库声明的分支信息。例如：
```
$ typings info env~node --versions
TAG                   VERSION LOCATION                                                            UPDATED
6.0.0+20160902022231  6.0.0   github:types/env-node/6#30804787ed04e4d475046ef0335bef502f492da0    2016-09-02T02:22:31.000Z
4.0.0+20160902022231  4.0.0   github:types/env-node/4#30804787ed04e4d475046ef0335bef502f492da0    2016-09-02T02:22:31.000Z
0.12.0+20160902022231 0.12.0  github:types/env-node/0.12#30804787ed04e4d475046ef0335bef502f492da0 2016-09-02T02:22:31.000Z
0.11.0+20160902022231 0.11.0  github:types/env-node/0.11#30804787ed04e4d475046ef0335bef502f492da0 2016-09-02T02:22:31.000Z
0.10.0+20160902022231 0.10.0  github:types/env-node/0.10#30804787ed04e4d475046ef0335bef502f492da0 2016-09-02T02:22:31.000Z
0.8.0+20160902022231  0.8.0   github:types/env-node/0.8#30804787ed04e4d475046ef0335bef502f492da0  2016-09-02T02:22:31.000Z
```
可以看到 `env~node` 有 6 个分支(Tag)，对应 Node.js 的不同版本。
> 
这些分支对 Node.js 是版本号，但对于 typings 它们都是分支，而不是版本！
然后通过
```
typings i env~node#4.0.0+20160902022231 --global
```
就安装好了。
### 从 GitHub 仓库安装 Definition
可以使用 typings 从指定的 GitHub 仓库里下载安装 Definition
命令格式有两种：
```
# 文件式
typings i github:用户名/项目名称/文件路径 --global
```
或
```
# 仓库式
typings i github:用户名/项目名称 --global
```
#### 直接安装仓库里的某个文件作为 Definition
```
# 文件式
# 安装这个文件的最新 commit 版本
typings i github:DefinitelyTyped/DefinitelyTyped/express/express.d.ts --global
```
#### 使用特定 commit 版本作为 Definition
```
# 文件式
# 安装这个文件的 commit=5fd6d6b4eaabda87d19ad13b7d6709443617ddd8 的版本
typings i github:DefinitelyTyped/DefinitelyTyped/express/express.d.ts#5fd6d6b4eaabda87d19ad13b7d6709443617ddd8 --global
```
#### 使用专用的 GitHub 仓库
假设我为一个叫 ABCDEFG 的库写了一个 Definition，现在我要把它发布到 GitHub 上作为 typings 源。那么先建立一个 GitHub 项目，名字随意，这里假设是 `https://github.com/sample/abcdefg-typings`。
把 Definition 取名为 `index.d.ts`，再添加一个文件 `typings.json`，内容如下：
```
{
  "name": "abcdefg",
  "main": "index.d.ts",
  "version": "0.1.0-demo"
}
```
将 `index.d.ts` 和 `typings.json` 两个文件提交到 GitHub 的 sample/abcdefg-typings 仓库。现在，我们可以通过下面的命令安装了。
```
# 仓库式
# 安装这个仓库的最新 commit 版本
typings i github:sample/abcdefg-typings --global
```
安装成功后可以看到控制台提示
```
typings WARN badlocation "github:sample/abcdefg-typings" is mutable and may change, consider specifying a commit hash
abcdefg-typings@0.1.0-demo
`-- (No dependencies)
```
那句警告的意思是建议使用一个 commit ID，这个就随意了。
### 使用 typings.json 管理 Definition
看了上面的用法，为了更方便的管理一个项目依赖的 Definition （比如更新版本），typings 需要使用一个名为 `typings.json`文件来记录我们安装过的 Definition 。
先初始化它，
```
typings init
```
这个命令初始化了 typings.json 文件，内容是一个空的 Definition 依赖记录表：
```
{
  "dependencies": {}
}
```
现在我们来安装 Definition ，并记录到表中：
```
typings i env~node dt~async --global --save
```
后面的 `--save`（可简写为 `-S`） 会将 Definition 信息添加到 Definition 依赖记录表，比如现在的 typings.json 文件内容如下：
```
{
  "dependencies": {},
  "globalDependencies": {
    "async": "registry:dt/async#2.0.1+20160804113311",
    "node": "registry:dt/node#6.0.0+20160915134512"
  }
}
```
这样，发布项目时或者上传代码到 GitHub 的时候，typings 目录就可有可无了，需要的时候直接一句 `typings i` 就完成了 Definition 的安装。需要注意的是，typings 默认安装最新版本的 Definition，如果你不想每次都安装最新的，可以通过 **2.4. 从 GitHub 仓库安装 Definition** 的方法解决。
## 编写 Definition
前面讲了很多关于如何使用 Definition 的内容，那都是“用”，下面来讲讲如何自己写一个 Definition。
###1 Node.js 与 NPM 模块
NPM 在某个项目内本地安装的模块都在项目的 `./node_modules` 目录下，一个模块一个目录，以模块名称为目录名。
对于一个 NPM 模块，通过里面的 package.json 文件的 `main` 字段可以指定其默认的入口文件。在 Node.js 里通过 `require("模块名称")` 引用的就是这个默认的入口文件。如果未指定 package.json 文件的 `main` 字段，但是存在 index.js 文件，那么 index.js 也会被当成默认的入口文件。
除此之外，在 Node.js 里面还可以单独引用 NPM 模块的其中一个文件，而不只是通过默认入口文件引用模块。例如：
```
var sample = require("sample");
var lib1 = require("sample/lib1");
var lib2 = require("sample/lib2");
```
现在假设这三个文件的代码如下，我们将在后面以这三个文件为基础编写 Definition：
```
// File: ./node_modules/sample/index.js
var abc = 321;
exports.setABC = function(abcValue) {
    abc = abcValue;
};
exports.getABC = function() {
    return abc;
};
 
exports.defaultABC = abc;
```
```
// File: ./node_modules/sample/lib1.js
var Hello = (function () {
    function Hello(a) {
        this.valueA = a;
    }
    Object.defineProperty(Hello.prototype, "a", {
        get: function () {
            return this.valueA;
        },
        enumerable: true,
        configurable: true
    });
    Hello.initClass = function () {
        Hello.instCount = 0;
    };
    /**
     * 假设这是一个重载函数，支持多种调用方式
     */
    Hello.prototype.setup = function (x, b) {
        if (b === void 0) { b = null; }
        return false;
    };
    return Hello;
}());
exports.Hello = Hello;
```
```
// File: ./node_modules/sample/lib2.js
 
var randStrSeed = "abcdefghijklmnopqrstuvwxyz0123456789";
 
function randomString(length) {
 
    var ret = "";
 
    while (length-- > 0) {
 
        ret += randStrSeed[Math.floor(Math.random() * randStrSeed.length)];
    }
 
    return ret;
}
 
module.exports = randomString;
```
这是三个典型的模块类型，第一个导出了变量和函数，第二个导出了一个类，第三个则将一个函数作为一个模块导出。
现在我们以这三个文件为例，分别以**模块导出声明 (External Module Definition)** 和 **全局类型声明(Global Type Definition)** 两种写法编写 Definition。
### 全局类型声明写法
假如上面的3个文件同属一个模块 **sample**，但是它并不是我们自己在 NPM 上发布的，即是说我们无权给它添加内建 Definition。所以我们用全局类型声明写法。
这是一个不是很复杂的模块，那么我们用一个 `.d.ts` 文件就可以了。
第一个文件是模块的入口文件，可以直接当成模块 sample。定义如下：
```
declare module "sample" {
 
    // 导出函数 setABC
    export function setABC(abcValue: number): void;
 
    // 导出函数 getABC
    export function getABC(): number;
 
    // 导出变量 defaultABC
    export let defaultABC: number;
}
```
第二个文件是导出了两个类，可以当成模块 “sample/lib1”。下面来看看如何导出类。
这个类里面有构造函数，有静态方法，有普通方法，有属性，也有静态属性，还有 getter。
类有两种声明编写方式：`标准式`和`分离式`。
`标准式`很直接，就是像 C/C++ 的头文件里声明类一样只写类声明不写实现：
```
declare module "sample/lib1" {
 
    export class Hello {
 
        private valueA;
 
        b: number;
 
        static instCount: number;
 
        a: number;
 
        constructor(a: number);
 
        static initClass(): void;
 
        /**
        * 假设这是一个重载函数，支持多种调用方式
        */
        setup(name: string): boolean;
 
        setup(name: string, age: number): boolean;
    }
}
```
但是这种写法也有不便的地方，比如扩展类不方便——JavaScript允许你随时扩展一个类的原型对象实现对类的扩展，或者随时给类添加静态成员。`标准式`写法很难实现扩展，因为你无法重复声明一个类。
所以下面我们来看看所谓的`分离式`声明。在这之前我们要理解，JS 的类是用函数实现的，即是说 JS 的类本质上就是一个构造函数 + Prototype。Prototype 的成员就是类的成员；而类的静态方法就是这个构造函数对象本身的成员方法。
因此我们可以分开写这两者的声明：
```
declare module "sample/lib1" {
 
    /**
     * 在分离式写法里面，一个类的 Prototype 的声明是一个直接以类名称为名的
     * interface。我们把成员函数和变量/getter/setter 都行写在 prototype
     * 的接口里面。
     *
     * 注意：类原型的 interface 取名与类名一致。 
     */
    export interface Hello {
 
        /**
         * 接口里面只写类的 public 属性
         */
        "b": number;
 
        /**
         * Getter/Setter 直接成属性即可。
         */
        "a": number;
 
        /**
         * 重载函数的声明写法
         */ 
        setup(name: string): boolean;
        setup(name: string, age: number): boolean;
    }
 
    /**
     * 在分离式写法里面，一个类的构造函数对象也是一个 interface ，但是对
     * 其命名无具体要求，合理即可。
     * 
     * 把类的静态方法和属性都写在这里面。
     */
    export interface HelloConstructor {
 
        /**
         * 静态属性
         */
        "instCount": number;
 
        /**
         * 静态方法
         */
        initClass(): void;
 
        /**
         * 构造函数！
         * 使用 new 代替 constructor，并声明其返回值类型是该类的Prototype。
         */
        new(a: number): Hello;
    }
 
    /**
     * 将 Hello 覆盖声明为 HelloConstructor。
     * 
     * 这样，在需要作为类使用的时候它就是 HelloConstructor，
     * 需要作为接口使用的时候就是 Hello（原型接口）。
     */
    export let Hello: HelloConstructor;
}
```
如上，就是导出类的两种姿势~
接着看第三个文件，直接将一个函数作为模块导出，也是很简单的。
```
declare module "sample/lib2" {
 
    let randomString: (length: number) => string;
 
    export = randomString;
}
```
最后把 3 个模块的声明合并成一个文件 sample.d.ts，在文件里用三斜线指令引用即可。
### 模块导出声明写法
模块导出声明写法里面不用注明是哪个模块，一般给每个导出的文件都配备一个以 `.d.ts` 为后缀的 Definition。
- 
文件 ./node_modules/sample/index.d.ts
```
// File: ./node_modules/sample/index.d.ts
// 导出函数 setABC
export declare function setABC(abcValue: number): void;
 
// 导出函数 getABC
export declare function getABC(): number;
 
// 导出变量 defaultABC
export declare let defaultABC: number;
```
- 
文件 ./node_modules/sample/lib1.d.ts
```
// File: ./node_modules/sample/lib1.d.ts
 
export class Hello {
 
    private valueA;
 
    b: number;
 
    static instCount: number;
 
    a: number;
 
    constructor(a: number);
 
    static initClass(): void;
 
    /**
    * 假设这是一个重载函数，支持多种调用方式
    */
    setup(name: string): boolean;
 
    setup(name: string, age: number): boolean;
}
```
- 
文件 ./node_modules/sample/lib2.d.ts
```
// File: ./node_modules/sample/lib2.d.ts
 
let randomString: (length: number) => string;
 
export = randomString;
```
### 如何确定现有类的声明接口名称？
以确认 `String` 类的声明接口名称为例。
在 TypeScript 源码的 lib.d.ts 里面可以找到：
```
declare var String: StringConstructor;
```
这就是 String 类的构造函数了，即是说 `StringConstructor` 定义了 `String` 的静态方法。
> 
使用如 Visual Studio Code 的编辑器，在里面随意打开一个 `*.ts` 文件，然后输入比如 `String` ，鼠标移动上去，可以看到类型定义。
然后查看 `StringConstructor` 的定义：
```
/*
 * 全局类/对象的声明都是在 lib.d.ts 里面定义的，即是说 TypeScript 通常会
 * 默认引用一个 lib.d.ts 文件，所以这里面的内容无需引用声明即可使用。
 *
 * 也正因此 StringConstructor 不需要 declare 和 export。
 *
 */
interface StringConstructor {
    new (value?: any): String;
    (value?: any): string;
    prototype: String;
    fromCharCode(...codes: number[]): string;
}
```
这里可以看出，String 类的构造函数的声明是接口 `StringConstructor`，而其 `String.prototype` 的声明是接口 `String`，显然用了`分离式`写法。
```
interface String {
 
    //...
}
```
### 扩展 JavaScript 全局类/对象
前面我们实现了一个模块的声明文件。
以 langext 的代码为例，试图为 JS 原生的 `String` 类添加一个 `random` 静态方法。
如果直接写：
```
String.random = function(len: number): string {
 
    return '...';
};
```
是无法通过编译的，因为 TS 的类型检查，根据既有的 `String` 类定义，发现 `random` 不是 `String` 类的静态成员。
解决方法是使用一个声明文件，在里面写：
```
interface StringConstructor {
 
    random(length: number): string;
}
```
然后引用这个定义文件即可。
这是利用了 TS 的 interface 可分离定义特性，同名的 interface，只要字段定义不冲突就可以分开定义。【参考 4.2 节】
## 编写 Definition 的注意事项
### 不要使用内层 declare
只能在 Definition 的顶层使用 `declare`，比如下面的写法都是**错误**的：
```
declare module "sample" {
 
    // 此处应当使用 export
    declare let a: string;
}
 
declare namespace Sample {
 
    // 此处应当使用 export
    declare let a: string;
}
```
### 避免全局污染
虽然全局声明写法允许你引入名称到全局命名空间中，但这也意味着，引入的顶层名称都是全局的。所以应该将所有的模块内导出的元素都放进模块或者命名空间内：
```
declare module "sample" {
 
    /**
     * 仅可通过 import { Person } from "sample" 访问。
     */
    export interface Person {
 
        name: string;
    }
}
 
declare namespace Sample {
 
    export interface Animal {
 
        type: string;
    }
}
```
**而不是**
```
/**
 * 无需 import 即可使用，即全局的
 */
interface Person {
 
    name: string;
}
```
不过以下情况例外：
- 
当在扩展全局对象/类的时候，允许这么写
```
interface StringConstructor {
 
    random(length: number): string;
}
```
- 
当确实引入了新的全局名称时，比如 script 里的全局变量
```
declare let globalName: string;
```
### 注意声明冲突
module 和 namespace 都是可以重复声明的——但是里面的元素不能冲突。具体如下：
```
declare module "sample" {
 
    export let name: string;
 
    export interface ABC {
 
        value: string;
    }
}
 
declare module "sample" {
 
    // 冲突，因为 sample 模块里已经有了导出变量 name
    export let name: string;
 
    // 不冲突，因为两个内容不重复的重名 interface 可以合并。
    export interface ABC {
 
 
        name: string;
    }
}
 
declare module "sample" {
 
    // 冲突，因为前面的 sample.ABC 里面已经定义了 value 字段。
    export interface ABC {
 
 
        value: string;
    }
}
```
### 模块名称要区分大小写！
这一点对于 Windows 上的 Node.js 开发人员很致命！因为在 Windows 下文件名不区分大小写，所以你不区分大小写都可以成功引用模块，**但是，Node.js 并不认为仅仅名称大小写不一致的两个文件是同一个模块！**
这将导致一个严重的后果——同一个模块被初始化为不同名称（大小写不一致）的多个实例，导致各处引用的不是同一个实例，从而造成数据不同步。

