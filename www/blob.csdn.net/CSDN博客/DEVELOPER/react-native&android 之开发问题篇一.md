# react-native&android 之开发问题篇一 - DEVELOPER - CSDN博客





2018年07月05日 15:14:52[学术袁](https://me.csdn.net/u012827205)阅读数：491
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









在`react-native` 开发过程中引入别人的**react-native-x库**，环境上会有某种不通过。 

着实很忧愁，下面就以我遇到的问题排下名词；

# `MEET 壹`

```
Error:Execution failed for task ':app:processDebugManifest'.
> Manifest merger failed : Attribute meta-data#android.support.VERSION@value value=(26.1.0) from [com.android.support:exifinterface:26.1.0] AndroidManifest.xml:25:13-35
    is also present at [com.android.support:appcompat-v7:26.0.2] AndroidManifest.xml:28:13-35 value=(26.0.2).
    Suggestion: add 'tools:replace="android:value"' to <meta-data> element at AndroidManifest.xml:23:9-25:38 to override.
```

**该问题出现原因是，**`AndroidManifest.xml`合并过程中，由于`com.android.support:appcompat`版本不同导致无法成功合并； 
**`solution:`**一股脑的把所依赖的**react-native-x库**等影响因素全部搞一致，按照我的经验，也并不一定把所有的依赖库的都搞一致。但是更重要的是你要判断出上面报错针对的是哪一个依赖库的。**注意：按照上面日志的版本要求做。**因为在一些`react-native`库中在实现功能上，要求的版本较高；

如下

```bash
compileSdkVersion 26
buildToolsVersion "26.0.2"
compile 'com.android.support:appcompat-v7:26.1.0'
```

# `MEET 贰`

![这里写图片描述](https://img-blog.csdn.net/2018070515083580?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
Error:Could not find common.jar (android.arch.core:common:1.0.0).
Searched in the following locations:
    https://jcenter.bintray.com/android/arch/core/common/1.0.0/common-1.0.0.jar
```

**该问题出现原因是，** jcenter已经删掉了android.arch.core库，所以需要从 maven { url “[https://maven.google.com](https://maven.google.com)” }中获取android.arch.core:common-1.1.0.jar 
**`solution:`**在project工程根目录下add代码指令`maven { url "https://maven.google.com" }`

如下：
```
allprojects {
    repositories {
        mavenLocal()
        maven {
            // All of React Native (JS, Obj-C sources, Android binaries) is installed from npm
            url "$rootDir/../node_modules/react-native/android"
        }
        //解决问题：Error:Failed to resolve: android.arch.core:common:1.1.0
        maven { url "https://maven.google.com" }
        jcenter()
        maven { url 'https://jitpack.io' }
    }
}
```

# `MEET 叁`

![这里写图片描述](https://img-blog.csdn.net/20180706162626564?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
`undefined is not an object (evaluating 'this.props.navigation.goBack')`


这里出现的问题是，我从`react-native`首页面`HomeScreen.js`中点击某个按钮。通过

```python
import {StackNavigator,NavigationActions} from 'react-navigation';
```

的 `this.props.navigation.navigate("GoodsListScreen")`命令进入到一个新的列表，在新的Component页面中引入了一个自定义的**TitleBar**，就是这样的

```
export default class GoodsListScreen extends Component {
...
render() {
        return (<View style={styles.container}>
            <TitleBar
                /** {...this.props} 加上这一行就对了*/ 
                title = "育婴"
                headerBar = {{backgroundColor:Colors.black}}
                statusBar = {{backgroundColor:Colors.black}}
            />
            ...
        </View>);
    }
}
```

点击**返回**按钮，想要返回到上一个页面，就**报错了**。 
**该问题出现原因是，**`undefined is not an object`。结合上面图片，说明`this.props.navigation`是null。也就是说，页面组件属性this.props没有传到子组件中。 
**`solution:`** TitleBar 标签中添加属性 {…this.props}，可在子组件解构赋值。
# `MEET 肆`

![这里写图片描述](https://img-blog.csdn.net/20180707102102162?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
`Warning: Can only update a mounted or mounting component. This usually means you called setState, replaceState, or forceUpdate on an unmounted component. This is a no-op.`

  大概意思：只能更新已装载或装载过的组件。侧面说明你在这样未装载的组件上调用setState、replaceState或forceUpdate是不行滴。


我是怎么编出这样的代码逻辑的：写了一个demo例子，在`react-native`版本`0.51.0`下。对组件**FlatList**实现下拉刷新和上拉加载更多。但是在快速滑动页面，直接就加载了n多个item数据，并且中间一大段是空白，然后下边很快提示了上面的警告！ 
**然后也看到官网上也有这方面的简单提示：**

> 
为了优化内存占用同时保持滑动的流畅，列表内容会在屏幕外异步绘制。这意味着如果用户滑动的速度超过渲染的速度，则会先看到空白的内容。这是为了优化不得不作出的妥协，而我们也在设法持续改进。


**`solution:`** 对组件**FlatList**添加属性

```
onEndReachedThreshold={0.1}
                initialNumToRender={3}
                getItemLayout={(data, index) => ({
                    length: 250, offset: (250 + 10) * index, index
                })}
```

onEndReachedThreshold滑动到距离底部距离**0.1*item**高度开始加载更多； 

initialNumToRender首次进入渲染**3个**item； 

getItemLayout避免动态测量内容尺寸的开销； 

解决了问题。[源码地址分支blog0706](https://github.com/Y-JH/csdn-blog/tree/blog0706)
# `MEET 伍`

![这里写图片描述](https://img-blog.csdn.net/20180707112434265?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
`Cannot add  child that doesn't have a YogaNode to a parent without a measure function.`


```
//正确代码
render() {
        return (<View style={{flex: 1, backgroundColor: Colors.bg}}>
            <FlatList
                ref={(flatlist) => this.flatlist = flatlist}
                ListHeaderComponent={this._header}
                renderItem={this._renderItem}
                ItemSeparatorComponent={this.props.itemSeparator}
                data={this.state.dataSource}
                keyExtractor={this._keyExtractor}
                onRefresh={() => this.onRefresh()}
                refreshing={this.state.isRefresh}
                onEndReachedThreshold={10}
                onEndReached={
                    () => this.onLoadMore()
                }
            />
            <Toast ref='toast'/>
        </View>);
    }
```

**`solution:`**我的问题出现的就是`<Toast ref='toast'/>`直接放在了上面代码块中，它所在行的上一行的`'/>'`的后面导致。这样竟然也能搞出错误，当然，有的博客上说在render方法中一些注释也会导致这种问题！

# `MEET 陆`

![这里写图片描述](https://img-blog.csdn.net/20180710100135312?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
`undefined is not an object (evaluating '_react2.PropTypes.string')`


我的代码是这样的，然出现上面的错误

```
/**
 * 功能：自定义的样式Button按钮
 * Created by YJH on 2018/7/10.
 */
import React, { Component,PropTypes } from 'react';
import { StyleSheet, Text, TouchableOpacity } from 'react-native';

export default class Button extends Component {
    static propTypes = {
        text: PropTypes.string.isRequired,
        onClick: PropTypes.func.isRequired,
    };

    render() {
        const { text, onClick } = this.props;
        return (
            <TouchableOpacity style={styles.Button}  onPress={onClick}>
                <Text>{text}
                </Text>
            </TouchableOpacity>
        )
    }
}

const styles = StyleSheet.create({
    Button: {
       ...
    }
});
```

**该问题出现原因是，**在`ReactNative的Framework`升级到**0.50.3**后，使用上面的引入方式就不能使用了。 

依照我的项目，正确的引入方式是，`import PropTypes from '../../../../../node_modules/prop-types/';`
**`solution:``PropTypes`这个属性移动到了**依赖的node_modules中**，再正确引入即可；






