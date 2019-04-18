# Uncaught Error: _registerComponent(...): Target container is not a DOM element - weixin_33985507的博客 - CSDN博客
2017年12月11日 13:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Uncaught Error: _registerComponent(...): Target container is not a DOM element
```java
<script type="text/babel">
    var SessionPage = React.createClass({
        getInitialState: function () {
            var context = {
                context: {}
            };
            return context;
        },
        componentDidMount: function () {
            this.serverRequest = $.ajax({
                url: this.props.url,
                data: {},
                type: 'GET',
                dataType: 'json',
                success: function (data) {
                    this.setState({
                        context: data
                    })
                }.bind(this), // 修改bind()前的函数内部this变量指向。if 不bind的话，方法内部的this 就是 $.ajax({这个对象}), bind传入的this应该是组件。 可以console 输出一下看看。
                error: function (msg) {
                    console.log("error:" + msg);
                }.bind(this)
            })
        },
        componentWillUnmount: function () {
            this.serverRequest.abort()
        },
        render: function () {
            var creatItem = function (it) {
                return (<code>JSON.stringify(it)</code>)
            };
            return (
                    <div>{creatItem(this.state.context)}</div>
            );
        }
    });
    ReactDOM.render(<SessionPage url="/api/session"/>);
</script>
```
就是一个找不到DOM节点的问题。
改成：
ReactDOM.render(<SessionPage url="/api/session"/>, document.getElementById("App"));
