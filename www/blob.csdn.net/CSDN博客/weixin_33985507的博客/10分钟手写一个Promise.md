# 10分钟手写一个Promise - weixin_33985507的博客 - CSDN博客
2018年08月15日 18:05:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
不要被then需要异步执行吓到了，then传入的是个函数，什么时候调用，应该由第一次回调决定
静态的all、race等方法，后面再补充
```
function promise(func){
    this.state = 'pending'; //状态
    this.resolveArg; //回调参数，下同
    this.rejectArg;
    this.thenQueue = []; //调用队列
    this.thenable = true;
    this.resolveFunc = _resolveFunc.bind(this);
    this.rejectFunc  = _rejectFunc.bind(this);
    this.catchFunc   = _catchFunc.bind(this);
    //将 res，rej回调传入作用域内并执行主函数
    try {
        func(this.resolveFunc,this.rejectFunc)
    } catch (e) {
        this.catchFunc(e);
    }
}
promise.prototype = {
    constructor : promise,//绑定构造器
    /* 调用then，只需要把传入的回调压进then队列里即可，不需要timeout操作*/
    then : function(res,rej) {
        if (!this.thenable) {
            return this;
        }
        //
        this.thenQueue.push({
            res : res,
            rej : rej
        })
        return this;
    },
    catch: function(func) {
        this.catchFunc = func;
        return this
    },
    finally: function (func) {
        this.finallyFunc = func;
        this.thenable = false;
        return this;
    }
}
/* rej、res回调会将then队列里的函数依次执行，并且上一个then的返回值作为下一个then的参数 */
function _rejectFunc (args) {
    this.rejectArg = args;
    this.state     = 'rejected'
    if(!this.thenQueue.length){return;}
    while (this.thenQueue.length) {
        rejCallback = this.thenQueue.shift().rej;
        try {
            var newReturns = rejCallback(this.rejectArg);
            this.rejectArg = newReturns;
        } catch (e) {
            this.catchFunc(e)
        }
    }
    this.finallyFunc && this.finallyFunc();
    this.thenable = false;
}
function _resolveFunc (args) {
    this.resolveArg = args;
    this.state      = 'fullfilled'
    if(!this.thenQueue.length){return;}
    while (this.thenQueue.length) {
        resCallback = this.thenQueue.shift().res;
        try {
            var newReturns  = resCallback(this.resolveArg)
            this.resolveArg = newReturns;
        } catch(e) {
            this.catchFunc(e)
        }
    }
    this.finallyFunc && this.finallyFunc();
    this.thenable = false;
}
function _catchFunc (e){
    throw Error('Uncaught promise error:' + e)
}
new promise(res=>{
    setTimeout(()=>{
        res([123])
    },1000)
}).then(res=>{
    console.log('res',res)
    return 'bbb'
}).then(res=>{
    console.log(res)
})
```
## 存在的问题
Promise.then是在下一个MicroTask调用的，而上述的代码是同步执行
```
new promise(res=>{
  setTimeout(()=>{
    res()
  })
  console.log('抢先一步')
}).then()
```
设置了setTimeout会使then在MacroTask之后调用
Node.js中可以使用setImmediate，就能使其真正的在下一个MicroTask执行
