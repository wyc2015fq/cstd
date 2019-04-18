# react-jsx - weixin_33985507的博客 - CSDN博客
2017年09月07日 13:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
> 
使用React，不一定非要使用JSX语法，可以使用原生的JS进行开发。但是JSX在定义类似HTML这种树形结构时，十分的简单明了。简明的代码结构更利于开发和维护。
从本质上讲，JSX 只是为 React.createElement(component, props, ...children) 函数提供的语法糖。
- jsx代码
```
<MyButton color="blue" shadowSize={2}>
  Click Me
</MyButton>
```
- 编译为
```
React.createElement(
  MyButton,
  {color: 'blue', shadowSize: 2},
  'Click Me'
)
```
*这很像vue里面的render函数*
- 如果不存在子节点，你可以使用自闭合（self-closing）格式的标签。例如:
```
<div className="sidebar" />
```
- 编译为：
```
React.createElement(
  'div',
  {className: 'sidebar'},
  null
)
```
- jsx代码
```
React.render(
    <div>
        <div>
            <div>content</div>
        </div>
    </div>,
    document.getElementById('example')
);
```
- 编译为
```
React.render(
    React.createElement('div', null,
        React.createElement('div', null,
            React.createElement('div', null, 'content')
        )
    ),
    document.getElementById('example')
);
```
### HTML属性
> 
注意：标签的属性class和for，需要写成className和htmlFor。因为两个属性是JavaScript的保留字和关键字。无论你是否使用JSX。
```
#使用JSX
React.render(
    <label className="xxx" htmlFor="input">content</label>,
    document.getElementById('example')
);
#不使用JSX
React.render(
    React.createElement('label', {className: 'xxx', htmlFor: 'input'}, 'content'),
    document.getElementById('example')
);
```
### 转化
- 
##### JS表达式
表达式用{}包起来，不要加引号，加引号就会被当成字符串。
JSX是HTML和JavaScript混写的语法，当遇到<，JSX就当HTML解析，遇到{就当JavaScript解析。
- 
##### 解析变量
```
var str="hello"
React.render(
    <div>{str}</div>,
    document.body
);
//甚至
var arr=[1,2,3,4]
React.render(
    <ul>
          {arr.map(function(item){
                return <li>{item}</li>
          })}
    </ul>,
    document.body
);
```
- 
##### 属性表达式
```
React.render(
    <div className={2 > 1 ? 'class-a' : 'class-b'}>content</div>,
    document.body
);
```
- 
##### 子表达式
```
var Nav = React.createClass({
    render: function () {
        return <div>nav</div>
    }
});
React.render(
    <div>
        {2 > 1 ? <Nav/> : <div>div</div>}
    </div>,
    document.body
);
```
### 特别注意
- 
##### style属性
在React中写行内样式时，要这样写，不能采用引号的书写方式
```
React.render(
    <div style={{color:'red'}}>
        xxxxx
    </div>,
    document.body
);
```
- 
##### HTML转义
比如我们有一些内容是用户输入的富文本，从后台取到数据后展示在页面上，希望展示相应的样式
```
var content='<strong>content</strong>';
React.render(
    <div>{content}</div>,
    document.body
);
```
结果页面直接输出内容了：
![7687940-581a33198ec69db2.png](https://upload-images.jianshu.io/upload_images/7687940-581a33198ec69db2.png)
A.png
React默认会进行HTML的转义，避免XSS攻击，如果要不转义，可以这么写：
```
var content='<strong>content</strong>';    
React.render(
    <div dangerouslySetInnerHTML={{__html: content}}></div>,
    document.body
);
```
- 
##### 自定义HTML属性
如果在编写React过程中使用了自定义属性，React不会渲染的
```
React.render(
    <div dd='xxx'>content</div>,
    document.body
);
```
![7687940-d89a0d37fc47792c.png](https://upload-images.jianshu.io/upload_images/7687940-d89a0d37fc47792c.png)
B.png
要想使用得这么写：
```
React.render(
    <div data-dd='xxx' aria-dd='xxx'>content</div>,
    document.body
);
```
> 
jsx基本使用先到这里，希望可以帮到同学们
