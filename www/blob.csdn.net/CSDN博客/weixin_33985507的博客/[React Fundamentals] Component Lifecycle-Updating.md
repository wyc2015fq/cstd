# [React Fundamentals] Component Lifecycle - Updating - weixin_33985507的博客 - CSDN博客
2016年08月16日 03:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

The React component lifecycle will allow you to update your components at runtime. This lesson will explore how to do that.
```
import React from 'react';
import ReactDOM from 'react-dom';
export default class App extends React.Component {
    constructor(){
        super();
        this.state = {
            increasing: false
        }
    }
    update(){
        ReactDOM.render(<App val={this.props.val+1} /> , document.getElementById('app'));
    }
    componentWillReceiveProps(nextProps){
        //Invoked when a component is receiving new props. This method is not called for the initial render.
        this.setState({
            increasing: nextProps.val > this.props.val
        })
    }
    shouldComponentUpdate(nextProps, nextState){
        // Control whether the component should re-render
        return nextProps.val % 5 === 0; // update dom every 5 clicks
    }
    render() {
        console.log(this.state.increasing);
        return (
            <div>
                <button onClick={this.update.bind(this)}>{this.props.val}</button>
            </div>
        )
    }
    componentDidUpdate(prevProps, prevState){
        //After DOM update
        console.log("prevProps", prevProps);
    }
}
App.defaultProps = {
    val: 0
}
```
