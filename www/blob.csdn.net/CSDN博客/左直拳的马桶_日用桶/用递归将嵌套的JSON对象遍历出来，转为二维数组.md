# 用递归将嵌套的JSON对象遍历出来，转为二维数组 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月30日 15:20:57[左直拳](https://me.csdn.net/leftfist)阅读数：7699
如题所示，代码如下：
```java
var arJsonNesting = [{id:1,name:"zhang3"
                        ,children:[{id:2,name:"zhang33"},{id:3,name:"zhang44"}]}
                    ,{id:4,name:"li4"}];//注意有的JSON对象有子对象children
var arJson = new Array();
function refining(arJsonNesting,arJson) {//将嵌套的JSON对象遍历出来，转为二维数组
    for (var i = 0,j = arJson.length; i < arJsonNesting.length; i++) {
        arJson[j] = {
            Id:arJsonNesting[i].id
            ,Name:arJsonNesting[i].name
        };
        j++;
        if(arJsonNesting[i].children){
            refining(arJsonNesting[i].children,arJson);
        }
    }
}
```
