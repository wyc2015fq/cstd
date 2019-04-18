# 初学React，setState后获取到的thisstate没变，还是初始state？ - z69183787的专栏 - CSDN博客
2016年08月22日 15:19:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15898
问题：(javascript)初学React，setState后获取到的thisstate没变，还是初始state？
描述:
```
getInitialState(){
            return {data:[]};
          },
          componentDidMount(){
            var data = [ { author: "Pete Hunt", text: "This is one comment" },
                         { author: "Jordan Walke", text: "This is *another* comment" } ];
            (function(data){
              this.setState({data:data});
              console.log(this.state.data);
            }).call(this,data);
          },
```
为什么log里打出来的data是`[]`呢？
解决方案1:
react 的 setState 是异步执行的，所以你后面立即 console 是还没改变状态， setState 函数接受两个参数，一个是一个对象，就是设置的状态，还有一个是一个回调函数，就是设置状态成功之后执行的，你可以这样：
```
(function(data){
   var that = this;
   this.setState({data:data}, function () {
       console.log(that.state.data);
   });
}).call(this,data);
```
解决方案2:
不能立即 console.log(this.state.data);的，你可以在 render方法里取
解决方案3:
善用官方文档：
```
void setState(
  function|object nextState,
  [function callback]
)
```
The second (optional) parameter is a callback function that will be executed once setState is completed and the component is re-rendered.
所以正确做法是
```
this.setState(
    Object.assign({}, { data }),
    () => console.log(this.state)
)
```
```
```
