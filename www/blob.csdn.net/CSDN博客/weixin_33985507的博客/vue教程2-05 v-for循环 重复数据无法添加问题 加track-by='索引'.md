# vue教程2-05 v-for循环 重复数据无法添加问题  加track-by='索引' - weixin_33985507的博客 - CSDN博客
2017年04月27日 11:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
vue教程2-05 v-for循环 重复数据无法添加问题  加track-by='索引'
![](https://images2015.cnblogs.com/blog/1118847/201704/1118847-20170427113019897-1674918320.png)
 解决问题的代码示例：
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title></title>
    <style>
    </style>
    <script src="vue.js"></script>
    <script>
    </script>
</head>
<body>
    <div id="box">
        <input type="button" value="添加" @click="add">
        <ul>
            <li v-for="val in arr" track-by="$index">
                {{val}}
            </li>
        </ul>
    </div>
    <script>
        var vm=new Vue({
            data:{
                arr:['apple','pear','orange']
            },
            methods:{
                add:function(){
                    this.arr.push('tomato');
                }
            }
        }).$mount('#box');
    </script>
</body>
</html>
```
