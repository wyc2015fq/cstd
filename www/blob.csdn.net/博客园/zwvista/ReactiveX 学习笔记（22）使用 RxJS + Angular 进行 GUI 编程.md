# ReactiveX 学习笔记（22）使用 RxJS + Angular 进行 GUI 编程 - zwvista - 博客园

## [ReactiveX 学习笔记（22）使用 RxJS + Angular 进行 GUI 编程](https://www.cnblogs.com/zwvista/p/9574484.html)

### 课题
- 程序界面由3个文本编辑框和1个文本标签组成。
- 要求文本标签实时显示3个文本编辑框所输入的数字之和。
- 文本编辑框输入的不是合法数字时，将其值视为0。
- 3个文本编辑框的初值分别为1，2，3。

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

### CSS

打开 app.component.css 文件，将其改为

```
.number {
  width: 50px;
  text-align: right;
}
```

### 不使用 RxJS 的解决方案

app.module.ts 中添加 FormsModule 的引用。

```
import { FormsModule } from '@angular/forms';

@NgModule({
  imports: [
    BrowserModule,
    FormsModule,
  ],
})
```

打开 app.component.html 文件，将其改为

```
<div style="text-align:center">
  <h1>
    Welcome to {{ title }}!
  </h1>
</div>
<p>
  <input class="number" [(ngModel)]="number1" (ngModelChange)="onChangeNumber()"> +
  <input class="number" [(ngModel)]="number2" (ngModelChange)="onChangeNumber()"> +
  <input class="number" [(ngModel)]="number3" (ngModelChange)="onChangeNumber()"> =
  <label>{{result}}</label>
</p>
```

打开 app.component.ts 文件，将其改为

```
import { Component, AfterViewInit } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements AfterViewInit {

  number1 = '1';
  number2 = '2';
  number3 = '3';
  result: string;

  title = 'RxExample';
  constructor() { }

  ngAfterViewInit() {
    this.onChangeNumber();
  }

  onChangeNumber() {
    const g = s => Number(s) || 0;
    setTimeout(() => this.result = String(g(this.number1) + g(this.number2) + g(this.number3)));
  }
}
```

### 使用 RxJS 的解决方案

打开 app.component.html 文件，将其改为

```
<div style="text-align:center">
  <h1>
    Welcome to {{ title }}!
  </h1>
</div>
<p>
  <input #number1Ref class="number" value="1"> +
  <input #number2Ref class="number" value="2"> +
  <input #number3Ref class="number" value="3"> =
  <label>{{resultAsync | async}}</label>
</p>
```

打开 app.component.ts 文件，将其改为

```
import { Component, ElementRef, ViewChild, AfterViewInit } from '@angular/core';
import { combineLatest, fromEvent, Observable } from 'rxjs';
import { map, pluck, startWith } from 'rxjs/operators';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements AfterViewInit {

  @ViewChild('number1Ref') number1Ref: ElementRef;
  @ViewChild('number2Ref') number2Ref: ElementRef;
  @ViewChild('number3Ref') number3Ref: ElementRef;
  resultAsync: Observable<string>;

  title = 'RxExample';
  constructor() { }

  ngAfterViewInit() {
    const f = elemRef => fromEvent(elemRef.nativeElement, 'input')
      .pipe(pluck('target', 'value'), startWith((elemRef.nativeElement as HTMLInputElement).value));
    const g = s => Number(s) || 0;
    setTimeout(() => this.resultAsync = combineLatest(f(this.number1Ref), f(this.number2Ref), f(this.number3Ref))
      .pipe(map(results => String(g(results[0]) + g(results[1]) + g(results[2])))));
  }
}
```


