# React Native - LC900730的博客 - CSDN博客
2017年09月10日 19:48:12[lc900730](https://me.csdn.net/LC900730)阅读数：101标签：[ReactNativ](https://so.csdn.net/so/search/s.do?q=ReactNativ&t=blog)
个人分类：[react-native](https://blog.csdn.net/LC900730/article/category/7161938)
### 基本
render()：初始化方法
```
class AHelloWorld extends Component{
    //返回具体组件内容
    render(){
        //返回
        return(
            <View style={styles.container}>
              <Text style={styles.container}>
                 你好，世界！//必须放在text组件里面
              </Text>
            </View>
            );
    }
//var style也可以
const style=StyleSheet.create({
    container:{
        flex:1,
        justifyContent：'center'
        alignItems:'center'
        backgroundColor:"F5FCFF",
    },
    weclome:{
        fontsize:20
        textAlign:'center'
        margin:10
    },
    instructions:{
        textAlign:'center',
        color:'#333333',
        marginBottom:5,
    },
});
```
### JSX
利用熟悉的HTML语法创建虚拟DOM
```xml
var root=(
    <div className="box">
        <div className="pic">
            <img src="images/1.jpg">
       <div>
     </div>
)
```
在实际开发中，JSX在产品打包阶段都已经编译成纯JavaScript，JSX的语法不会带来任何性能影响。因此，JSX本身并不是高深技术，只是一个比较高深但是很直观的语法糖。
### View组件中常见属性
React Native组件View，其作用是等同于ios中的UIView，Android中的android.view,或是`<div>` 标签，它是所有标签的父控件。 
Flexbox：弹性布局 
Transforms：动画属性 
backfaceVisibility enum(‘visible’,’hidden’):定义界面翻转的时候是否可见 
backgroundColor    color 
borderStyle.enum(‘solod’,’dotted’,’dashed’) 
opacity number:设置透明度，取值从0-1 
overflow enum(‘visible’,’hidden’)：设置内容超出容器部分是显示还是隐藏
### View组件运用
下图是View基本运用：
```
var JHViewTest=React.createClass({
    render(){
        return{
            <View style={{flex:1,padding:30,
            backgroundColor:'red'}}>
                <View style={{width:300,height:50,
            backgroundColor:'red'}}>
            </View>
            }
    }
})
```
render函数中，我们返回了一个顶层的View，然后View中包含着另一个子层的View。 
在顶层的View中的style属性里面设置了其占满父控件，内边距为30，背景颜色为黄色；对应子层中View的style属性中设置了宽度300，高度350，这是CSS写法。
### 多个view
```xml
//只能有一个根view
<View style={styles.container}>
    <Text>其实我是存在的</Text>
    <View style={styles.innerViewStyle}>
        <Text>我是里面的View</Text>
    </View>
</View>
```
### flexbox弹性盒布局
