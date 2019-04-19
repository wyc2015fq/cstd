# Redux框架简介 - 零度的博客专栏 - CSDN博客
2017年05月24日 14:56:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：3427
React是个非常适合组件化开发的框架，可以说facebook重新定义了前端。但项目中实际使用下来，维护各组件的state是件比较头疼的事情。尤其页面交互比较复杂时，各种render，你都搞不清楚到底发生了什么，系统复杂到难以维护。还有一个问题就是React的非父子组件间的通信，如某些各组件都要用到的共享状态，没有一个统一的类似管理中心的地方，出了问题难以溯源到哪个组件改了这个state。Redux就是帮助你解决这些痛点，帮助你管理state。请牢记在心，学习一样新东西，了解它诞生的原因，能解决什么痛点，对你的学习帮助很大。
本篇不打算零基础介绍Redux，基础知识请自行参阅[英文官网](http://redux.js.org/)，[中文版](http://www.redux.org.cn/)，[阮老师的入门教程](http://www.ruanyifeng.com/blog/2016/09/redux_tutorial_part_one_basic_usages.html)
根据官网，Redux有三大原则：
1.单一数据源。整个应用的state都被存储在一个名为store的object树中。这个理念是正确的，但实际应用中，也不必太教条。例如一些仅限React组件内部使用的，纯展示用的state可以不必存储到store中。以我目前对Redux的理解，我倾向于只在store里存储各组件间有交互依赖关系的state。
2.Store里的state是只读的。只能通过分派Action通过Reducer来修改存在Store里的state。Action的本质是个描述已发生事件，能携带数据的plain object。作用是告知Reducer该更新Store里哪些state。
3.使用纯函数来执行修改state。即在Reducer里修改state。Reducer接收两个参数：旧的state和Action，返回一个新的state。它应该是个纯函数：只要有相同的参数，就一定能得到相同的返回值。这个理念是正确的，虽然你在Reducer里做一些不纯的操作也不会报错，但不建议这么做。
初始化Redux整体状态流转，如图：
![Redux](http://www.jackzxl.net/wp-content/uploads/2017/05/initRedux.jpg)
通过Redux的createStore方法来生成Store。createStore方法的参数是Reducer方法（可以用combinReducers方法将多个Reducer合并起来，将业务数据拆分的更清晰）。Reducer方法接收两个参数state和Action。生成完store对象后，需要用subscribe方法注册回调函数listener，当store内的state发生变化时会自动调用回调函数。回调函数内可以通过getState方法取得最新的state。
初始化完成后。用户要想修改Store里的state，只能通过dispatch方法，该方法接收Action作为参数。Reducer收到Action后，修改相应的state。进而触发初始化过程中注册的listener回调函数。如图：
![Redux](http://www.jackzxl.net/wp-content/uploads/2017/05/doRedux.jpg)
用Redux写了个教程，源代码已上传[Github](https://github.com/JackZhangXL/react-redux)。第一篇源代码请参照src/originRedux文件夹：
import React, { Component } from 'react';
import { render } from 'react-dom';
import { createStore } from 'redux';
import { Button } from 'antd';
import 'antd/dist/antd.css';
import './originRedux.pcss';
const reducer = (state, action) => {
    if (typeof state === 'undefined') {
        return 0;
    }
    switch (action.type) {
        case 'INCREMENT':
            return state + 1;
        case 'DECREMENT':
            return state - 1;
        case 'CLEAR_NUM':
            return 0;
        default:
            return state;
    }
};
const store = createStore(reducer);
const update = () => {
    const valueEl = document.getElementsByClassName('numValue');
    valueEl[0].innerHTML = store.getState().toString();
};
store.subscribe(update);
export default class Number extends Component {
    addNum = () => {
        store.dispatch({ type: 'INCREMENT' });
    };
    minusNum = () => {
        store.dispatch({ type: 'DECREMENT' });
    };
    clearNum = () => {
        store.dispatch({ type: 'CLEAR_NUM' });
    };
    render() {
        return (
            <div className="wrap">
                <h3>origin Redux</h3>
                Current Number: <span className="numValue">0</span>
                <div>
                    <Button size="large" className="numBtn" onClick={this.addNum}>+</Button>
                    <Button size="large" className="numBtn" onClick={this.minusNum}>-</Button>
                    <Button size="large" className="numBtn" onClick={this.clearNum}>clear</Button>
                </div>
            </div>
        );
    }
}
render(
    <Number />,
    document.getElementById('app'),
);
去除空行甚至不满50行，干脆全贴上来了，代码应该不难理解。Redux为了更好地和React结合，推出了react-redux包，后几篇会介绍。但这里第一个例子中使用React仅仅是为了方便，你完全可以将例子中React代码改写成jQuery。Redux并不是React专用框架~
后几篇会陆续介绍Action，Reducer，React-Redux，异步操作中间件，跟着[源代码](https://github.com/JackZhangXL/react-redux)启蒙Redux应该不在话下。
