# ReactiveX 学习笔记（28）使用 RxJS + React.js 进行 GUI 编程 - zwvista - 博客园
## [ReactiveX 学习笔记（28）使用 RxJS + React.js 进行 GUI 编程](https://www.cnblogs.com/zwvista/p/9976942.html)
### 课题
- 程序界面由3个文本编辑框和1个文本标签组成。
- 要求文本标签实时显示3个文本编辑框所输入的数字之和。
- 文本编辑框输入的不是合法数字时，将其值视为0。
- 3个文本编辑框的初值分别为1，2，3。
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
[http://localhost:3000/](http://localhost:3000/) 可打开网页。
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
### CSS
打开 App.css，添加CSS
```
.number {
    width: 50px;
    text-align: right;
}
```
### 不使用 RxJS 的解决方案
打开 App.tsx，将其改为
```
import './App.css';
class App extends React.Component {
  state = {
    number1: '1',
    number2: '2',
    number3: '3',
    result: '',
  };
  componentDidMount() {
    this.onChangeNumber(null);
  }
 
  render() {
    return (
      <div className="App">
        <p>
          <input name="number1" className="number" value={this.state.number1} onChange={this.onChangeNumber} /> +
          <input name="number2" className="number" value={this.state.number2} onChange={this.onChangeNumber} /> +
          <input name="number3" className="number" value={this.state.number3} onChange={this.onChangeNumber} /> =
          <label>{this.state.result}</label>
        </p>
      </div>
    );
  }
 
  onChangeNumber = (e: any) => {
    const f = () => {
      const g = (s: string) => Number(s) || 0;
      this.setState({
        result: String(g(this.state.number1) + g(this.state.number2) + g(this.state.number3))
      });
    };
    if (e != null) {
      const elem = e.nativeEvent.target as HTMLInputElement;
      this.setState({[elem.name]: elem.value}, () => f());
    } else {
      f();
    }
  };
}
 
export default App;
```
### 使用 RxJS 的解决方案
打开 App.tsx，将其改为
```
import './App.css';
import { combineLatest, fromEvent, Observable } from 'rxjs';
import { map, pluck, startWith } from 'rxjs/operators';
class App extends React.Component {
  state = {
    result2: '',
  };
  componentDidMount() {
    const f = (id: string) => {
      const e = document.getElementById(id) as HTMLInputElement;
      return fromEvent(e, 'input').pipe<string, string>(pluck('target', 'value'), startWith(e.value)) as Observable<string>;
    };
    const g = (s: string) => Number(s) || 0;
    combineLatest(f('number1'), f('number2'), f('number3'))
      .pipe(map((results: string[]) => String(g(results[0]) + g(results[1]) + g(results[2]))))
      .subscribe(result2 => this.setState({result2}));
  }
 
  render() {
    return (
      <div className="App">
        <p>
          <input id="number1" className="number" defaultValue="1" /> +
          <input id="number2" className="number" defaultValue="2" /> +
          <input id="number3" className="number" defaultValue="3" /> =
          <label>{this.state.result2}</label>
        </p>
      </div>
    );
  }
}
 
export default App;
```
