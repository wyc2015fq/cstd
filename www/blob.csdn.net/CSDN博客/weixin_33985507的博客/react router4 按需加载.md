# react router4 按需加载 - weixin_33985507的博客 - CSDN博客
2017年07月01日 14:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
众所周知，使用react，基本上都是react + router + redux的单页面应用的套路，好用，但是应用越来越庞大时，首页的渲染时间就会越来越长， 这个时候就需要用到按需加载。
router3及其之前用的是webpack带的require.ensure配合router的getComponent，随着router4的到来，去掉了getComponent，这个时候应该怎么写呢，官方提供了一个新的套路。
- 首先使用官方提供的bundle (这个组件和你用webpack打包的bundle没啥关系) 组件
```
import React, {Component} from 'react'
export default class Bundle extends Component {
    constructor(props) {
        super(props)
    }
    componentWillMount() {
        this.load(this.props)
    }
    componentWillReceiveProps(nextProps) {
        if (nextProps.load !== this.props.load) {
            this.load(nextProps)
        }
    }
    load = (props) => {
        this.setState({
            mod:null
        })
        props.load((mod) => {
            this.setState({
                mod:mod.default?mod.default:mod
            })
        })
    }
    render() {
        return this.props.children(this.state.mod)
    }
}
```
- 
需要用到bundle-loader
npm i bundle-loader --save-dev
- 
引入container的方式也改变了
```
import BlogPage from 'bundle-loader?lazy!./BlogPage' //在组件前面加bundle-loader?lazy!
```
- 用bundle组件包裹container
```
import Bundle from '../bundle.js';
export const Blog = () => (
    <Bundle load={BlogPage}>
        {(List) => <List/>}
    </Bundle>
)
```
- 最后的route按正常配置即可
```
<Route  path='/blog' component={ Blog }/>
```
- 碰到无法传递prop的问题，用withRouter可解决
```
import { withRouter } from 'react-router'
class Test extends Component {
    ...
    render(){
        const { match, location, history } = this.props
        ...
    }
}
export default withRouter(Test)
//export default withRouter(connect(...)(Test))//with redux
```
了解更多信息可关注的本人的 [github](https://link.jianshu.com?t=https://github.com/ShiChenCong)
