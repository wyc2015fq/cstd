# react-native 之"Cannot update during an exitsting state transition" 与 函数方法bind()/箭头函数有关？ - DEVELOPER - CSDN博客





2018年06月08日 15:17:57[学术袁](https://me.csdn.net/u012827205)阅读数：641
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









**react-native `Warning:setState(...):Cannot update during an exitsting state transition...`**

这个问题出现的确实让我措手不及，结果是这样的 
![这里写图片描述](https://img-blog.csdn.net/20180608144628987?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
业务逻辑：在页面`HomePage`中点击`setting-item`按钮弹出页面`ThemePage`，并选择自己喜欢的样式进行样式的切换。 

导致错误操作：点击了js页面的一个方法`onPress={this.onSelectThemeItem(key)}`所导致的。即第14行代码中的方法`onPress` 的开始触发了问题！

**而代码是这样的，下面的代码是会出现上面错误的代码**

在`extends Component`组件的`js`页面中的部分代码展示

```
export default class xxComponent extends Component {

   ...
   ..

   onSelectThemeItem(key) {
       this.props.onModalClose();
       this.themeDao.saveTheme(ThemeFlags[key]);
       //发送通知，通知修改主题
       DeviceEventEmitter.emit('ACTION_BASE', ACTION_NOTIFY.changeTheme, ThemeFactory.createTheme(ThemeFlags[key]))
   }

   getThemeItem(key) {
       let themeTitle = <TouchableOpacity style={{flex: 1}} onPress={this.onSelectThemeItem(key)}>
           <View style={[{backgroundColor: ThemeFlags[key]}, styles.themeItem]}>
               <Text style={{color: 'white', fontWeight: '300'}}>{key}</Text>
           </View>
       </TouchableOpacity>;

       return themeTitle;
   }

    ...
    ..
}
```

而这里出现的问题：view绘制中，再次更改props，导致view会再次去render，报错！就在14行的代码..

`这种问题最可能出现的位置，就是在你所触发的方法中需要创建一个匿名函数（箭头函数），而你确没能满足。`
**为什么？下文解决！**

解决问题方法： 

将代码 
![这里写图片描述](https://img-blog.csdn.net/20180608151718446?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

换作 
![这里写图片描述](https://img-blog.csdn.net/20180608151732469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**分析”`Cannot update during an exitsting state transition`” 与 函数方法`bind()`、`箭头函数`的关系**
![从只能说她们有点关系！](https://img-blog.csdn.net/20180613151417987?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

从上面犯错的代码种截取一段代码来看，逻辑是在一个页面中通过点击页面中的一个组件而触发一个事件。 
**从这行代码上看，事实确是在刚执行到该页面js代码，进行渲染的时候就会去执行花括号中的方法`this.onSelectThemeItem(key)` ，导致在view渲染中又请求渲染的操作。从而报以上的错误！**
首先了解下平时我们写**bind()方法**和**箭头函数**的几种形式 

第一种

```
...
xxFunction(){..}
...
<XXView xxxx={this.xxFunction.bind(this)} />
```

第二种

```
constructor(props) {
    super(props);
    this.xxFunction= this.xxFunction.bind(this);
}

...
xxFunction(){..}
...
```

第三种

```
...
xxFunction= ()=>{..};
...
<XXView xxxx={this.xxFunction} />
```

第四种

```
...
xxFunction(){..}
...
<XXView xxxx={()=>this.xxFunction()} />
```

**bind函数**和**箭头函数**区别： 

```
在ES5下，React.createClass会把所有的方法都bind一遍，这样可以提交到任意的地方作为回调函数，而this不会变化。 

在ES6下，你需要通过bind来绑定this引用，或者使用箭头函数（它会绑定当前scope的this引用）来调用。
```


由此能够得出=>>>**bind()方法**和**箭头函数**在使用是**等价**的。且在其形式上**2和3是一致**的、**1和4是一致**的。
通过 [**bind() 函数**](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Function/bind)会创建一个新函数（称为绑定函数），该新函数是由指定的this值和初始化参数改造的原函数拷贝。

好的，让我们一同看下代码

```
/**
 * Created by YJH on 2018/6/13.
 */
import React, {Component} from 'react';
import {
    BackAndroid,
} from 'react-native';

export default class Backforward{
    constructor(props){
        this.backpress = this.onHardwareBackforward.bind(this);
        this.props=props;//由于不是component组件，所以后面使用props时候，需要将props通过this.props存储起来
    }

    componentDidMount(){
        if(this.props.backforward){
            BackAndroid.addEventListener("backforward", this.backpress);
        }
    }

    componentWillUnmount(){
        if(this.props.backforward){
            BackAndroid.removeEventListener("backforward", this.backpress);
        }
    }
    onHardwareBackforward(e){

        return this.props.backforward(e);
    }

}
```

通过bind绑定函数`onHardwareBackforward`返回一个原始函数拷贝的新函数`this.backpress`；并在17行和23行代码上使用`react-native`内的**组件BackAndroid**添加了安卓手机返回按钮的监听，和移除监听。当用户点击手机返回按钮则触发26行代码`onHardwareBackforward`方法执行，并调用暴露在js页面中的方法执行关闭当前的js页面。

就是下面代码

```
export default class xxDetailPage extends Component {

    constructor(props) {
        super(props);
        //实现对android手机返回按钮的监听返回功能
        this.backforward = new Backforward({backforward: (e) => this.onBackforward(e)});
        ...
    }

    componentDidMount(){
        this.backforward.componentDidMount();
    }
    componentWillUnmount(){
        this.backforward.componentWillUnmount();
    }
    onBackforward(e) {
        this.goBack();
        return true;
    }

    goBack() {
        if (this.state.isBack) {
            this.webView.goBack();
        } else {
            this.props.navigator.pop();
        }
    }
    ...
    ..
}
```

即`Backforward.js`第28行代码调用`xxDetailPage.js`第16行代码，完成了回调监听！当然，这种方式使用箭头函数也是可以实现的！

参考内容： 
[https://blog.csdn.net/sinat](https://blog.csdn.net/sinat_34693148/article/details/72631956)
[https://developer.mozilla.org](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Function/bind)
[http://bbs.reactnative.cn](http://bbs.reactnative.cn/topic/15/react-react-native-%E7%9A%84es5-es6%E5%86%99%E6%B3%95%E5%AF%B9%E7%85%A7%E8%A1%A8/2)







