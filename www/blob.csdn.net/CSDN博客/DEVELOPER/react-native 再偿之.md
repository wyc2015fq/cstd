# react-native 再偿之 - DEVELOPER - CSDN博客





2018年05月22日 21:35:28[学术袁](https://me.csdn.net/u012827205)阅读数：192
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









![这里写图片描述](https://img-blog.csdn.net/20180522213751817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**该博文内容基于Windows系统，针对Android系统下的APP：**

介绍下使用webStorm创建ReactNative项目，当然首先你要配置好环境，环境是否正确很重要，关系到你是否有兴趣继续玩下去，提醒下根据[ReactNative](https://reactnative.cn/docs/0.51/getting-started.html#content)官网配置哦了，然后下载[WebStorm](https://www.jetbrains.com/webstorm/download/previous.html)开发工具安装运行。 

创建新项目，注意是**React Native** 那一栏，不要选择叉了！创建成功之后，和在`cmd-dos`页面一样，是在`WebStorm的终端(Terminal)执行命令:"react-native run-android"`，如果没有安装对应的**gradle**，则**WebStorm** 会自动去下载，之后会成功将app安装到您手机上。 

当然，你也可以通过使用CLI命令进行创建新的项目。比如：`react-native init ImoocChapterTwo --version 0.41.0` 通过这种方式，是按照你自己设定的react-native版本进行创建项目的。 
![这里写图片描述](https://img-blog.csdn.net/20180525095640770?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**有一点需要注意，这是我实践的结果：**比如对于你所使用的命令`react-native init ImoocChapterTwo --version 0.41.0`。`ImoocChapterTwo` 该单词就是你的`app`的名字，也是决定了你项目包名的，比如这样操作就会生成包名为`com.imoocchaptertwo` 项目工程 ，并且`ImoocChapterTwo`这个名字不能含数字（如：ImoocChapter2），不然在`dos页面中`使用`CLI`无法生成`project`。
### 进入正题

项目结构图

## ![这里写图片描述](https://img-blog.csdn.net/20180522223854170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 如何导出和导入使用一个ReactNative组件？

新建一个组件 HelloWordComponent.js

```
import React, {Component} from 'react';
import {
    Text,
} from 'react-native';

export default class HelloWordComponent extends Component<Props> {
    render() {
        return (
            <Text style={{fontSize: 40, backgroundColor: 'red'}}>
                 哈喽 帅哥 !
            </Text>
        );
    }
}
```

对于组件来说，**导出的规则就是：Class代码中以两个单词`export default` 作为前缀来实现的。**

然后我们切换到**App.js**代码页面看到

```
/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, {Component} from 'react';
import {
    Platform,
    StyleSheet,
    View
} from 'react-native';
import HelloWordComponent from './HelloWordComponent'

const instructions = Platform.select({
    ios: 'Press Cmd+R to reload,\n' +
    'Cmd+D or shake for dev menu',
    android: 'Double tap R on your keyboard to reload,\n' +
    'Shake or press menu button for dev menu',
});

type Props = {};
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    },
    welcome: {
        fontSize: 20,
        textAlign: 'center',
        margin: 10,
    },
    instructions: {
        textAlign: 'center',
        color: '#333333',
        marginBottom: 5,
    },
});
```

其中有一行代码是这样写的`import HelloWordComponent from './HelloWordComponent'`； 
**表示组件导入（可在此`app.js`中引用）的方式。**

如何使用？？，在`render()方法`中实现使用！
```
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
            </View>
        );
    }
}
```

## ![这里写图片描述](https://img-blog.csdn.net/20180522220947924?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 如何导出和导入使用一个变量或者常量？

新建一个组件EIComponent.js

```
import React, {Component} from 'react';
import {
    Text,
    View
} from 'react-native';

var name = '小明';
var age = '20';

export {name, age};

export default class EIComponent extends Component<Props> {
    constructor(props) {
        super(props);

        this.state = {count: 0,}
    }

    render() {
        return (
            <View>
                <Text style={{fontSize: 40, backgroundColor: 'red'}}
                      onPress={() => {
                          this.setState({
                              count: this.state.count + 1,
                          })
                      }}>

                    点击我累计吻{this.props.name}次
                </Text>

                <Text style={{fontSize: 50}}>一共吻了{this.state.count}次</Text>
            </View>
        );
    }
}
```

代码中可以很容易的发现

```
var name = '小明';
var age = '20';

export {name, age};
```

就是变量的定义和导出的方式！怎么引用呢？还是在app.js中

```
/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, {Component} from 'react';
import {
    Platform,
    StyleSheet,
    Text,
    View
} from 'react-native';
import HelloWordComponent from './HelloWordComponent'
import EIComponent, {name, age} from './EIComponent'

const instructions = Platform.select({
    ios: 'Press Cmd+R to reload,\n' +
    'Cmd+D or shake for dev menu',
    android: 'Double tap R on your keyboard to reload,\n' +
    'Shake or press menu button for dev menu',
});

type Props = {};
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
                <Text style={{fontSize: 30}}>姓名：{name}</Text>
                <Text style={{fontSize: 30}}>芳龄：{age}</Text>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    },
    welcome: {
        fontSize: 20,
        textAlign: 'center',
        margin: 10,
    },
    instructions: {
        textAlign: 'center',
        color: '#333333',
        marginBottom: 5,
    },
});
```

其中有一行代码是这样写的`import EIComponent, {name, age} from './EIComponent'`； 
**表示变量导入（可在此`app.js`中引用）的方式。**

如何使用？？，在 `render()` 方法中实现使用！
```
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
                <Text style={{fontSize: 30}}>姓名：{name}</Text>
                <Text style={{fontSize: 30}}>芳龄：{age}</Text>
            </View>
        );
    }
}
```

## ![这里写图片描述](https://img-blog.csdn.net/20180522222809837?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 如何导出和导入使用一个方法？

方法导出和导出变量一样简单，直接在加入代码

```java
export function sum(a = 3, b = 6) {
    return a + b;
}
```

即

```
import React, {Component} from 'react';
import {
    Text,
    View
} from 'react-native';

var name = '小明';
var age = '20';

export {name, age};

export function sum(a = 3, b = 6) {
    return a + b;
}

export default class EIComponent extends Component<Props> {
    constructor(props) {
        super(props);

        this.state = {count: 0,}
    }

    render() {
        return (
            <View>
                <Text style={{fontSize: 40, backgroundColor: 'red'}}
                      onPress={() => {
                          this.setState({
                              count: this.state.count + 1,
                          })
                      }}>

                    点击我累计吻{this.props.name}次
                </Text>

                <Text style={{fontSize: 50}}>一共吻了{this.state.count}次</Text>
            </View>
        );
    }
}
```

表示方法导入（可在此`app.js`中引用）的方式和变量的导入方式一样简单， 
`import EIComponent, {name, age, sum} from './EIComponent'`，只在`{}`中添加了`sum`的方法名就能实现！

```
/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, {Component} from 'react';
import {
    Platform,
    StyleSheet,
    Text,
    View
} from 'react-native';
import HelloWordComponent from './HelloWordComponent'
import EIComponent, {name, age, sum} from './EIComponent'

const instructions = Platform.select({
    ios: 'Press Cmd+R to reload,\n' +
    'Cmd+D or shake for dev menu',
    android: 'Double tap R on your keyboard to reload,\n' +
    'Shake or press menu button for dev menu',
});

type Props = {};
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
                <Text style={{fontSize: 30}}>姓名：{name}</Text>
                <Text style={{fontSize: 30}}>芳龄：{age}</Text>
                <Text style={{fontSize: 30}}>输入4，8计算结果={sum(4, 8)}</Text>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    },
    welcome: {
        fontSize: 20,
        textAlign: 'center',
        margin: 10,
    },
    instructions: {
        textAlign: 'center',
        color: '#333333',
        marginBottom: 5,
    },
});
```

如何使用？？，在 `render()` 方法中实现使用！

```
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
                <Text style={{fontSize: 30}}>姓名：{name}</Text>
                <Text style={{fontSize: 30}}>芳龄：{age}</Text>
                <Text style={{fontSize: 30}}>输入4，8计算结果={sum(4, 8)}</Text>
            </View>
        );
    }
}
```

![这里写图片描述](https://img-blog.csdn.net/2018052222341526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### ReactNative组件有生命周期吗？怎么调用的呢？

ReactNative组件是有自己的生命周期的，而且和我们的Android的四大组件之一的Activity是不是有些类似呢？!**她的生命周期主要分三大块：`装载、渲染和卸载`**，然后围绕这三大块进行生命周期的细分。 
![这里写图片描述](https://img-blog.csdn.net/2018052222461746?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后我呢，就新建一个组件LifeCycleComponent.js来打印下她的生命周期到底是怎么运作的！

```
import React, {Component} from 'react';
import {
    Text,
    View
} from 'react-native';


export default class LifeCycleComponent extends Component<Props> {
    constructor(props) {
        super(props);
        console.log('<<<<<<<<<-----constructor---->>>>>>>>>>>')

        this.state = {count: 0,}
    }

    componentWillMount() {
        console.log('<<<<<<<<<-----componentWillMount---->>>>>>>>>>>')
    }

    componentDidMount() {
        console.log('<<<<<<<<<-----componentDidMount---->>>>>>>>>>>')
    }

    componentWillReceiveProps(nextProps) {
        console.log('<<<<<<<<<-----componentWillReceiveProps---->>>>>>>>>>>')
    }

    shouldComponentUpdate(nextProps, nextState) {
        console.log('<<<<<<<<<-----shouldComponentUpdate---->>>>>>>>>>>')
        return true;
    }

    componentWillUpdate(nextProps, nextState) {
        console.log('<<<<<<<<<-----componentWillUpdate---->>>>>>>>>>>')
    }

    componentDidUpdate(preProps, preState) {
        console.log('<<<<<<<<<-----componentDidUpdate---->>>>>>>>>>>')
    }

    componentWillUnmount() {
        console.log('<<<<<<<<<-----componentWillUnmount---->>>>>>>>>>>')
    }

    render() {
        console.log('<<<<<<<<<-----render---->>>>>>>>>>>')
        return (
            <View>
                <Text style={{fontSize: 40, backgroundColor: 'blue'}}>这里在进行很严肃的ReactNative组件的生命周期测试</Text><Text style={{fontSize: 40, backgroundColor: 'blue'}}
                      onPress={() => {
                          this.setState({
                              count: this.state.count + 1,
                          })
                      }}
                    >这里在进行很严肃的ReactNative组件的生命周期测试</Text>
            </View>
        );
    }
}
```

然后导入到app.js中并使用

```
/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, {Component} from 'react';
import {
    Platform,
    StyleSheet,
    Text,
    View
} from 'react-native';
import HelloWordComponent from './HelloWordComponent'
import EIComponent, {name, age, sum} from './EIComponent'
import LifeCycleComponent from './LifeCycleComponent'

const instructions = Platform.select({
    ios: 'Press Cmd+R to reload,\n' +
    'Cmd+D or shake for dev menu',
    android: 'Double tap R on your keyboard to reload,\n' +
    'Shake or press menu button for dev menu',
});

type Props = {};
export default class App extends Component<Props> {
    render() {
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
                <Text style={{fontSize: 30}}>姓名：{name}</Text>
                <Text style={{fontSize: 30}}>芳龄：{age}</Text>
                <Text style={{fontSize: 30}}>输入4，8计算结果={sum(4, 8)}</Text>
                <LifeCycleComponent/>
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    },
    welcome: {
        fontSize: 20,
        textAlign: 'center',
        margin: 10,
    },
    instructions: {
        textAlign: 'center',
        color: '#333333',
        marginBottom: 5,
    },
});
```

这里先中断一下，叙述一下**ReactNative**的`远程调试`，看下图 
![这里写图片描述](https://img-blog.csdn.net/20180522225449867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
摇晃手机弹出对话框，红色框这个说明我已经开启了远程调试功能。如果么有开启，则点击开启就行，然后会在你的浏览器中打开一个页面，如下 
![这里写图片描述](https://img-blog.csdn.net/20180522230046198?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

根据上面提示**操作组合键**：按下`Ctrl+Shift+J`三键，正确打开`Debug页面`进行调试！ 

然后Reload查看控制台生命周期进行的log打印 
![这里写图片描述](https://img-blog.csdn.net/20180522230402601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后继续操作，点击**蓝色区域**触发监听。继续执行**生命周期**的方法（红色框区域） 
![这里写图片描述](https://img-blog.csdn.net/20180522230850242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后再次继续操作，**点击手机的返回键**退出该app。继续执行**生命周期**的方法（红色框区域） 
![这里写图片描述](https://img-blog.csdn.net/20180522231127772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在**未点击手机的返回键**退出该`app`情况下，如果我们继续操作，对`LifeCycleComponent`组件的使用进行控制。 

代码上稍作修改 
`app.js`
```
/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 * @flow
 */

import React, {Component} from 'react';
import {
    Platform,
    StyleSheet,
    Text,
    View
} from 'react-native';
import HelloWordComponent from './HelloWordComponent'
import LifeCycleComponent from './LifeCycleComponent'
import EIComponent, {name, age, sum} from './EIComponent'

const instructions = Platform.select({
    ios: 'Press Cmd+R to reload,\n' +
    'Cmd+D or shake for dev menu',
    android: 'Double tap R on your keyboard to reload,\n' +
    'Shake or press menu button for dev menu',
});

type Props = {};
export default class App extends Component<Props> {
    constructor(props) {
        super(props);
        this.state = ({remove: false})
    }

    render() {
        var viewComponent = this.state.remove ? null : <LifeCycleComponent/>;
        var text = this.state.remove ? '插入组件' : '拔出组件';
        return (
            <View style={styles.container}>
                <HelloWordComponent/>
                <Text style={{fontSize: 30}}>姓名：{name}</Text>
                <Text style={{fontSize: 30}}>芳龄：{age}</Text>
                <Text style={{fontSize: 30}}>输入4，8计算结果={sum(4, 8)}</Text>
                <Text style={{fontSize: 40}} onPress={() => {
                    this.setState({remove: !this.state.remove})
                }}>{text}</Text>
                {viewComponent}
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    },
    welcome: {
        fontSize: 20,
        textAlign: 'center',
        margin: 10,
    },
    instructions: {
        textAlign: 'center',
        color: '#333333',
        marginBottom: 5,
    },
});
```

`LifeCycleComponent.js`

```
import React, {Component} from 'react';
import {
    Text,
    View
} from 'react-native';


export default class LifeCycleComponent extends Component<Props> {
    constructor(props) {
        super(props);
        console.log('<<<<<<<<<-----constructor---->>>>>>>>>>>')
    }

    componentWillMount() {
        console.log('<<<<<<<<<-----componentWillMount---->>>>>>>>>>>')
    }

    componentDidMount() {
        console.log('<<<<<<<<<-----componentDidMount---->>>>>>>>>>>')
    }

    componentWillReceiveProps(nextProps) {
        console.log('<<<<<<<<<-----componentWillReceiveProps---->>>>>>>>>>>')
    }

    shouldComponentUpdate(nextProps, nextState) {
        console.log('<<<<<<<<<-----shouldComponentUpdate---->>>>>>>>>>>')
        return true;
    }

    componentWillUpdate(nextProps, nextState) {
        console.log('<<<<<<<<<-----componentWillUpdate---->>>>>>>>>>>')
    }

    componentDidUpdate(preProps, preState) {
        console.log('<<<<<<<<<-----componentDidUpdate---->>>>>>>>>>>')
    }

    componentWillUnmount() {
        console.log('<<<<<<<<<-----componentWillUnmount---->>>>>>>>>>>')
    }

    render() {
        console.log('<<<<<<<<<-----render---->>>>>>>>>>>')
        return (
            <View>
                <Text style={{fontSize: 40, backgroundColor: 'blue'}}
                >这里在进行很严肃的ReactNative组件的生命周期测试</Text>
            </View>
        );
    }
}
```

通过**点击**`拔出组件`操作，观察**生命周期**
![这里写图片描述](https://img-blog.csdn.net/2018052309265455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到周期方法执行顺序（红色框区域） 
![这里写图片描述](https://img-blog.csdn.net/20180523092818953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过**点击**`插入组件` 操作，观察**生命周期**
![这里写图片描述](https://img-blog.csdn.net/20180523092659419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到周期方法执行顺序（红色框区域） 
![这里写图片描述](https://img-blog.csdn.net/20180523092846772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[源码资源下载](https://download.csdn.net/download/u012827205/10431291)











