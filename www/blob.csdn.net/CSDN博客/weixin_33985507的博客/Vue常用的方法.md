# Vue常用的方法 - weixin_33985507的博客 - CSDN博客
2019年02月28日 11:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
### v-if
逻辑判断
### v-for
用于for循环
```
v-for=“item in dataArray”
```
```
v-for=“(item,index)in dataArray”
```
### v-bind
用于绑定数据
例如v-bind:src:"`model.url`",其中`model.url`是data里面的数据
### v-on:click
用于事件响应
例1：有参数传递
```
<el-button type="danger" circle v-on:click="deleteAction(index)">
  <i class="fa fa-trash-o fa-lg"></i>
</el-button>
```
```
deleteAction: function(index) {
  //打印出传递的参数，比如删除，编辑操作等需要id
  console.log(index);
}
```
例2：无参数传递
```
<el-button type="danger" circle v-on:click="doneAction">
  <i class="fa fa-trash-o fa-lg"></i>
</el-button>
```
```
doneAction: function(e) {
  //当前按钮的属性
  console.log(e);
}
```
