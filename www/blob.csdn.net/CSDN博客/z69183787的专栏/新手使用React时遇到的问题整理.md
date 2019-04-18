# 新手使用React时遇到的问题整理 - z69183787的专栏 - CSDN博客
2017年02月16日 11:54:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2133
本人新手一枚，以前没写过博客，但是总感觉做技术的不整理自己学的东西，是对自己的不负责任。有加上最近刚开始学习新技术，所以就想借这个机会开始学习写写博客，记录一下自己的学习历程。
由于是刚开始学习[React](https://facebook.github.io/react/docs/getting-started.html),以及一些相关的框架如[React](http://lib.csdn.net/base/react)-redux
 和 React-Router等，很多东西都是自学的，所以很多东西都不是很明白，但是我写博客就是为了记录自己的经历的，中间有什么说的不对的大家可以给我指导指导，小弟再次先跪谢了
首先要学习React,建议大家还是先去看看官方文档吧，这个东西很重的
在实习使用中需要注意的问题：
**1.作用域问题（这个相当重要）**
特别是在使用**this**时需要特别注意this的作用域问题。
在map()循环里如果需要全局作用域时，需要把全局作用域当做变量传递。这样使用的时候才不会出错。（小弟在这经常犯错，所以记录一下）
在调用当前组件的方法时，需要加this
如果在子组件中需要用到父组件的数据和方法的时候，在创建子组件的时候，需要把这些数据和方法传递到子组件里面，然后在子组件中就可以通过this,props.property或者this.props.function来进行使用了。
例如： 
**[html]**[view
 plain](http://blog.csdn.net/u013323872/article/details/51446157#)[copy](http://blog.csdn.net/u013323872/article/details/51446157#)
- <Main>
- <SubMain
- name = {this.props.name}  
- speech={this.personSpeech.bind(this)}  
- ></SubMain>
- </Main>
在子组件SubMain中就可以通过this.props.name 来获取组件Main中传递过来的name的值，同理也可以在组件SubMain中执行this.props.speech()方法
**2.在Redux使用的时候， Action文件里尽量保持Action文件的纯净**，及传入什么数据返回就是什么数据，最好把请求的数据和action方法分离开，以保持action的纯净
**3.在Reducer 文件里，renturn的结果(新的state)时候，必须要使用Object.assign()来返回新的state,**如果不使用，则页面不会跟着数据刷新（本人在这就犯了大错数据改变了，但是页面死活不刷新，各种找原因，最后才发现是这里的问题。。。）官方有解释说state中的数据不能被修改，只能通过更新state,返回新的state
**4.在给变量取名的时候，一定要注意尽量避免时候关键字**，虽然小弟已经很小心了，但是还是中招了, 就他  ---> key  尼玛这都是关键字么？ 我不是很清楚但是我确实是使用它后出错了，有那位大哥知道的为解释一下
**5.需要注意在Reducer文件里的所有reducer是一个大的switch(action.type)结构**，多个界面可以使用同一个Reducer，前提是传递的action.type正确。
**6.组件的生命周期相当重要**
**目前就用到了这几个：**
**[html]**[view
 plain](http://blog.csdn.net/u013323872/article/details/51446157#)[copy](http://blog.csdn.net/u013323872/article/details/51446157#)
- //在初始化渲染执行之前立刻调用  
- componentWillMount()  
- //在初始化渲染之后立刻被调用，这个位置可以操作DOM  
- componentDidMount()  
- //在组件的更新已同步到DOM中之后被调用，该方法不会在初始化渲染的时候调用。  
- //该方法可以在组件更新之后操作DOM元素  
- componentDidUpdate()  
7.在组件的return（）中是可以使用方法的，但是必须使用{ }括起来，好像是只能执行方法，不能直接写判断条件
例如：
**[html]**[view
 plain](http://blog.csdn.net/u013323872/article/details/51446157#)[copy](http://blog.csdn.net/u013323872/article/details/51446157#)
- <tbody>
-     {data.map(function(item){  
-         return (<trkey={item.key}>
- <td>{item.name}</td>
- <td>{item.userName}</td>
- <td>{item.password}</td>
- </tr>
-         )}  
-     )}  
- </tbody>
