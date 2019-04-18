# ReactiveX 学习笔记（18）使用 RxJS + Angular 调用 REST API - zwvista - 博客园

## [ReactiveX 学习笔记（18）使用 RxJS + Angular 调用 REST API](https://www.cnblogs.com/zwvista/p/9523127.html)

### JSON : Placeholder

[JSON : Placeholder (https://jsonplaceholder.typicode.com/)](https://jsonplaceholder.typicode.com/) 是一个用于测试的 REST API 网站。

以下使用 RxJS6 + Angular 6 调用该网站的 REST API，获取字符串以及 JSON 数据。
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
# 安装 Angular CLI
$ npm install -g @angular/cli
# 创建新的应用程序 RxExample
$ ng new RxExample
$ cd RxExample
$ npm audit fix --force
```

打开 Intellij IDEA, File / New / Project From Existing Sources...，然后选中工程所在文件夹

在向导的第1页选择 Create project from existing sources

完成向导后在点击 Finish 创建工程。

点击 Add Configurations, 点击 +npm

Name: Angular CLI Server

Scripts: start

点击 OK 完成配置。

点击 Angular CLI Server 启动程序。

http://localhost:4200/ 可打开网页。

### Post

在 app 文件夹下添加 post.ts，内容如下

```
export class Post {
  userId: number;
  id: number;
  title: string;
  body: string;
  toString(): string {
    return `Post {userId = ${this.userId}, id = ${this.id}, title = "${this.title}", body = "${this.body.replace(/\n/g, '\\n')}"}`;
  }
}
```

### post 服务

```
# 添加 post 服务
$ ng generate service post
```

将生成的 post.service.ts 改为

```
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, from } from 'rxjs';
import { map, mergeAll, take, tap } from 'rxjs/operators';
import { Post } from './post';

@Injectable()
export class PostService {
  private baseUrl = 'http://jsonplaceholder.typicode.com/';

  constructor(private http: HttpClient) { }

  restExample() {
    this.getPostAsString().subscribe();
    this.getPostAsJson().subscribe();
    this.getPosts(2).subscribe();
    this.createPost().subscribe();
    this.updatePost().subscribe();
    this.deletePost().subscribe();
  }

  private getPostAsString(): Observable<string> {
    const url = `${this.baseUrl}posts/1`;
    return this.http.get(url, {responseType: 'text'})
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

app.module.ts 中添加 PostService 和 HttpClientModule 的引用。

```
import { PostService } from './post.service';
import { HttpClientModule } from '@angular/common/http';

@NgModule({
  imports: [
    BrowserModule,
    HttpClientModule,
  ],
  providers: [
    PostService,
  ],
})
```

在 app.component.ts 中添加 post 服务的引用，将其改为

```
import { Component } from '@angular/core';
import {PostService} from './post.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'RxExample';
  constructor(private postService: PostService) { }
}
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
Post {userId = 1, id = 2, title = "qui est esse", body = "est rerum tempore vitae\nsequi sint nihil reprehenderit dolor beatae ea dolores neque\nfugiat blanditiis voluptate porro vel nihil molestiae ut reiciendis\nqui aperiam non debitis possimus qui neque nisi nulla"}
Post {userId = 1, id = 1, title = "sunt aut facere repellat provident occaecati excepturi optio reprehenderit", body = "quia et suscipit\nsuscipit recusandae consequuntur expedita et cum\nreprehenderit molestiae ut ut quas totam\nnostrum rerum est autem sunt rem eveniet architecto"}
{"params":{"userId":101,"title":"test title","body":"test body"},"id":1}
{"params":{"userId":101,"title":"test title","body":"test body"},"id":101}
{}
```


