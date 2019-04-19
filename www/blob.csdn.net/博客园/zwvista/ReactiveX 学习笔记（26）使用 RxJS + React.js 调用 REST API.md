# ReactiveX 学习笔记（26）使用 RxJS + React.js 调用 REST API - zwvista - 博客园
## [ReactiveX 学习笔记（26）使用 RxJS + React.js 调用 REST API](https://www.cnblogs.com/zwvista/p/9976061.html)
### JSON : Placeholder
[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。
以下使用 RxJS6 + React.js 调用该网站的 REST API，获取字符串以及 JSON 数据。
- GET /posts/1
- GET /posts
- POST /posts
- PUT /posts/1
- DELETE /posts/1
所有 GET API 都返回JSON数据，格式（JSON-Schema）如下：
```
{
  "type":"object",
  "properties": {
    "userId": {"type" : "integer"},
    "id": {"type" : "integer"},
    "title": {"type" : "string"},
    "body": {"type" : "string"}
  }
}
```
### 创建工程
```
# 安装 CLI
$ npm install -g create-react-app
# 创建新的应用程序 RxExample
$ create-react-app rx-example --scripts-version=react-scripts-ts
$ cd rx-example
$ npm start
```
打开 Intellij IDEA, File / New / Project From Existing Sources...，然后选中工程所在文件夹
在向导的第1页选择 Create project from existing sources
完成向导后在点击 Finish 创建工程。
点击 Add Configurations, 点击 +npm
Name: React CLI Server
Scripts: start
点击 OK 完成配置。
点击 React CLI Server 启动程序。
http://localhost:3000/ 可打开网页。
### TSLint
打开 tslint.json，将其改为
```
{
  "extends": ["tslint:recommended", "tslint-react", "tslint-config-prettier"],
  "linterOptions": {
    "exclude": [
      "config/**/*.js",
      "node_modules/**/*.ts",
      "coverage/lcov-report/*.js"
    ]
  },
  "rules": {
    "interface-name": false,
    "ordered-imports": false,
    "no-console": false,
    "object-literal-sort-keys": false,
    "member-access": false,
    "variable-name": false,
    "member-ordering": false,
    "class-name": false
  }
}
```
### Post
在 src 文件夹下添加 post.ts，内容如下
```
export class Post {
  userId!: number;
  id!: number;
  title!: string;
  body!: string;
  toString(): string {
    return `Post {userId = ${this.userId}, id = ${this.id}, title = "${this.title}", body = "${this.body.replace(/\n/g, '\\n')}"}`;
  }
}
```
### 安装 react.di
react.di 是一个在 React.js 中实现 DI（依赖注入） 的包。
使用这个包需要安装 react.di 和 reflect-metadata。
`$ npm install react.di@next reflect-metadata --save`
打开 tsconfig.json 在 compilerOptions 中
添加关于 emitDecoratorMetadata 的设定。
```
"emitDecoratorMetadata": true,
    "experimentalDecorators": true,
```
### 安装 Rxios
访问 API 采用将 RxJS 和 axios 合为一体的 Rxios 组件，但是该组件尚未升级到 RxJS6。
这里我们只安装 RxJS 和 axios，然后直接以源码形式引入 Rxios，并将其升级到 RxJS6。
`$ npm install axios rxjs`
在 src 文件夹下添加 rxios.ts，内容如下
```
// https://github.com/davguij/rxios/blob/master/src/index.ts
// ported to rxjs6
import axios, { AxiosInstance, AxiosRequestConfig, AxiosPromise } from 'axios';
// import { Observable } from 'rxjs/Observable';
import { Observable } from 'rxjs';
export interface rxiosConfig extends AxiosRequestConfig {
  localCache?: boolean;
}
class rxios {
  private _httpClient: AxiosInstance;
  constructor(options: rxiosConfig = {}) {
    this._httpClient = axios.create(options);
  }
  private _makeRequest<T>(method: string, url: string, queryParams?: object, body?: object) {
    let request: AxiosPromise<T>;
    switch (method) {
      case 'GET':
        request = this._httpClient.get<T>(url, {params: queryParams});
        break;
      case 'POST':
        request = this._httpClient.post<T>(url, body, {params: queryParams});
        break;
      case 'PUT':
        request = this._httpClient.put<T>(url, body, {params: queryParams});
        break;
      case 'PATCH':
        request = this._httpClient.patch<T>(url, body, {params: queryParams});
        break;
      case 'DELETE':
        request = this._httpClient.delete(url, {params: queryParams});
        break;
      default:
        throw new Error('Method not supported');
    }
    return new Observable<T>(subscriber => {
      request.then(response => {
        subscriber.next(response.data);
        subscriber.complete();
      }).catch((err: Error) => {
        subscriber.error(err);
        subscriber.complete();
      });
    });
  }
  public get<T>(url: string, queryParams?: object) {
    return this._makeRequest<T>('GET', url, queryParams);
  }
  public post<T>(url: string, body: object, queryParams?: object) {
    return this._makeRequest<T>('POST', url, queryParams, body);
  }
  public put<T>(url: string, body: object, queryParams?: object) {
    return this._makeRequest<T>('PUT', url, queryParams, body);
  }
  public patch<T>(url: string, body: object, queryParams?: object) {
    return this._makeRequest<T>('PATCH', url, queryParams, body);
  }
  public delete(url: string, queryParams?: object) {
    return this._makeRequest('DELETE', url, queryParams);
  }
}
export {rxios, rxios as Rxios};
```
### post 服务
在 src 文件夹下添加 post.service.ts，内容如下
```
import { Injectable } from 'react.di';
import { Observable, from } from 'rxjs';
import { map, mergeAll, take, tap } from 'rxjs/operators';
import { Post } from './post';
import { Rxios } from './rxios';
@Injectable
export class PostService {
  private readonly http = new Rxios();
  private readonly baseUrl = 'http://jsonplaceholder.typicode.com/';
  constructor() {
    this.getPostAsString().subscribe();
    this.getPostAsJson().subscribe();
    this.getPosts(2).subscribe();
    this.createPost().subscribe();
    this.updatePost().subscribe();
    this.deletePost().subscribe();
  }
  private getPostAsString(): Observable<string> {
    const url = `${this.baseUrl}posts/1`;
    return new Rxios({transformResponse: undefined}).get<string>(url)
      .pipe(
        tap((result: any) => console.log(result)),
      );
  }
  private getPostAsJson(): Observable<Post> {
    const url = `${this.baseUrl}posts/1`;
    return this.http.get<Post>(url)
      .pipe(
        map((result: any) => Object.assign(new Post(), result)),
        tap((result: any) => console.log('' + result)),
      );
  }
  private getPosts(n: number): Observable<Post> {
    const url = `${this.baseUrl}posts`;
    return from(this.http.get<Post[]>(url))
      .pipe(
        mergeAll(),
        map((result: any) => Object.assign(new Post(), result)),
        take(n),
        tap((result: any) => console.log('' + result)),
      );
  }
  private createPost(): Observable<string> {
    const url = `${this.baseUrl}posts`;
    return this.http.post(url, {
      params: {
        userId: 101,
        title: 'test title',
        body: 'test body',
      }
    })
      .pipe(
        map((result: any) => JSON.stringify(result)),
        tap((result: any) => console.log(result)),
      );
  }
  private updatePost(): Observable<string> {
    const url = `${this.baseUrl}posts/1`;
    return this.http.put(url, {
      params: {
        userId: 101,
        title: 'test title',
        body: 'test body',
      }
    })
      .pipe(
        map((result: any) => JSON.stringify(result)),
        tap((result: any) => console.log(result)),
      );
  }
  private deletePost(): Observable<string> {
    const url = `${this.baseUrl}posts/1`;
    return this.http.delete(url)
      .pipe(
        map((result: any) => JSON.stringify(result)),
        tap((result: any) => console.log(result)),
      );
  }
}
```
- getPostAsString 方法取出第1个Post，返回字符串
- getPostAsJson 方法取出第1个Post，返回Post对象
- getPosts 方法取出前n个Post，返回n个Post对象
- createPost 方法创建1个Post，返回字符串
- updatePost 方法更新第1个Post，返回字符串
- deletePost 方法删除第1个Post，返回字符串
打开 App.tsx，将其改为
```
import * as React from 'react';
import './App.css';
import logo from './logo.svg';
import { PostService } from './post.service';
import { Inject, Module } from 'react.di';
@Module({
  providers: [
    PostService,
  ],
})
class App extends React.Component {
  @Inject postService!: PostService;
  componentDidMount() {
    console.log(this.postService);
  }
  public render() {
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h1 className="App-title">Welcome to React</h1>
        </header>
        <p className="App-intro">
          To get started, edit <code>src/App.tsx</code> and save to reload.
        </p>
      </div>
    );
  }
}
 
export default App;
```
### 输出结果
```
{
  "userId": 1,
  "id": 1,
  "title": "sunt aut facere repellat provident occaecati excepturi optio reprehenderit",
  "body": "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"
}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
{"params":{"userId":101,"title":"test title","body":"test body"},"id":101}
{"params":{"userId":101,"title":"test title","body":"test body"},"id":1}
{}
```
