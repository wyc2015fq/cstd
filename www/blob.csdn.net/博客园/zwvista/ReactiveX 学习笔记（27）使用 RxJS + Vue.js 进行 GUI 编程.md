# ReactiveX 学习笔记（27）使用 RxJS + Vue.js 进行 GUI 编程 - zwvista - 博客园

## [ReactiveX 学习笔记（27）使用 RxJS + Vue.js 进行 GUI 编程](https://www.cnblogs.com/zwvista/p/9976931.html)

### 课题
- 程序界面由3个文本编辑框和1个文本标签组成。
- 要求文本标签实时显示3个文本编辑框所输入的数字之和。
- 文本编辑框输入的不是合法数字时，将其值视为0。
- 3个文本编辑框的初值分别为1，2，3。

### 创建工程

```
# 安装 Vue CLI
$ npm install --global @vue/cli
# 创建新的应用程序 RxExample
$ vue create rx-example
# 选择 Manually select features
# 选择 Babel 和 TypeScript
$ cd rx-example
$ npm run serve
```

打开 Intellij IDEA, File / New / Project From Existing Sources...，然后选中工程所在文件夹

在向导的第1页选择 Create project from existing sources

完成向导后在点击 Finish 创建工程。

点击 Add Configurations, 点击 +npm

Name: Vue CLI Server

Scripts: serve

点击 OK 完成配置。

点击 Vue CLI Server 启动程序。

http://localhost:8080/ 可打开网页。

### 不使用 RxJS 的解决方案

删除 HelloWorld 组件，即 src/components/HelloWorld.vue 文件。

打开 App.vue，将其改为

```
<template>
  <p>
    <input class="number" v-model="number1" @input="onChangeNumber()"> +
    <input class="number" v-model="number2" @input="onChangeNumber()"> +
    <input class="number" v-model="number3" @input="onChangeNumber()"> =
    <label>{{result}}</label>
  </p>
</template>

<script lang="ts">
export default class App extends Vue {

  number1 = '1';
  number2 = '2';
  number3 = '3';
  result = '';

  mounted() {
    this.onChangeNumber();
  }

  onChangeNumber() {
    const g = (s: string) => Number(s) || 0;
    this.result = String(g(this.number1) + g(this.number2) + g(this.number3));
  }
}
</script>

<style>
  .number {
    width: 50px;
    text-align: right;
  }
</style>
```

### 使用 RxJS 的解决方案

删除 HelloWorld 组件，即 src/components/HelloWorld.vue 文件。

打开 App.vue，将其改为

```
<template>
  <p>
    <input id="number1" class="number" value="1"> +
    <input id="number2" class="number" value="2"> +
    <input id="number3" class="number" value="3"> =
    <label>{{result2}}</label>
  </p>
</template>

<script lang="ts">
import { combineLatest, fromEvent, Observable } from 'rxjs';
import { map, pluck, startWith } from 'rxjs/operators';

export default class App extends Vue {
  result2 = '';

  mounted() {
    const f = (id: string) => {
      const e = document.getElementById(id) as HTMLInputElement;
      return fromEvent(e, 'input').pipe<string, string>(pluck('target', 'value'), startWith(e.value)) as Observable<string>;
    };
    const g = (s: string) => Number(s) || 0;
    combineLatest(f('number1'), f('number2'), f('number3'))
      .pipe(map((results: string[]) => String(g(results[0]) + g(results[1]) + g(results[2]))))
      .subscribe(result => this.result2 = result);
  }
}
</script>

<style>
  .number {
    width: 50px;
    text-align: right;
  }
</style>
```


