# Angular集成admin-lte框架 - =朝晖= - 博客园
# [Angular集成admin-lte框架](https://www.cnblogs.com/dhcn/p/7778245.html)
     其实上一篇里面提到的集成datatables.net就是admin-lte里面的一个子插件，不过这个子插件，他是自带types定义文件的，admin-lte这个东西在DefinitelyTyped里面目前没有人给它写过types定义,不过倒也不难，他也是基于jQuery的扩展，同类典型是Bootstrap，自己仿照Bootstrap的types定义，给出一个片段，其他的后续自己补上：
```
/// <reference types="jquery"/>
interface JQuery {
    layout(fixDes: string): JQuery;
}
interface JQuerySupport {
    transition: boolean | TransitionEventNames;
}
declare module 'admin-lte' {
}
```
      这个搞定以后，在tsconfig.app.json配置文件里面把admin-lte模块加进'types'配置项目列表，根据上篇集成datatables.net的说明，之后就可以直接调用接口定义好的方法声明了。

