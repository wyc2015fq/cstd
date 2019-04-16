# JavaScript学习总结(八)——JavaScript数组 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaScript学习总结(八)——JavaScript数组](https://www.cnblogs.com/xdp-gacl/p/3679579.html)



　　JavaScript中的Array对象就是数组，首先是一个动态数组，无需预先制定大小，而且是一个像Java中数组、ArrayList、Hashtable等的超强综合体。

## 一、数组的声明

　　常规方式声明：

**1、****var arrName = new Array();//创建一个数组**

**　　　　2、var **arrName **= new Array([size]);　//创建一个数组并指定长度，注意不是上限，是长度**

**　　　　3、var ****arrName ****=`new Array("孤傲苍狼","白虎神皇","灭世魔尊");**//创建一个数组**，并初始化数组的内容`**

**　　注意：虽然**var **arrName **= new Array([size]);**指定了长度，但实际上所有情况下数组都是变长的，也就是说即使指定了长度为2，仍然可以将元素存储在规定长度以外的，注意：这时长度会随之改变。**

　　Array的简化声明

**　1、普通数组初始化：var arr = [3, 5, 6, 8, 9];**

范例1：

```
1 <script type="text/javascript">
 2   //JavaScript声明数组的四种方式
 3     var arr1 = new Array();//创建一个空数组
 4     arr1[0]="xdp";
 5     arr1[1]="gacl";
 6     var arr2 = new Array(2);//创建一个数组并指定长度为2
 7     arr2["name0"]="xdp";//arr2第一个元素
 8     arr2["name1"]="gacl";//arr2第二个元素
 9     arr2["name2"]="xtxd";//arr2第三个元素，arr2虽然在声明时指明了长度为2，但是还是可以添加超过其指明长度的元素
10     var arr3 = new Array("孤傲苍狼","白虎神皇","灭世魔尊");//创建一个数组并初始化数组中的元素
11     var arr4 = [1,true,"String"];//Array的简化声明
12 
13     document.write("遍历arr1中的元素：<br/>");
14     for(var i in arr1) {
15         document.write(arr1[i]+"<br/>");
16     }
17     document.write("-----------------------------------------------------------------------------<br/>");
18     document.write("遍历arr2中的元素：<br/>");
19     for(var i in arr2) {
20         document.write("arr2[\""+i+"\"]="+arr2[i]+"<br/>");
21     }
22     document.write("-----------------------------------------------------------------------------<br/>");
23     document.write("遍历arr3中的元素：<br/>");
24     for(var i in arr3) {
25         document.write(arr3[i]+"<br/>");
26     }
27     document.write("-----------------------------------------------------------------------------<br/>");
28     document.write("遍历arr4中的元素：<br/>");
29     for(var i in arr4) {
30         document.write(arr4[i]+"<br/>");
31     }
32   </script>
```

运行结果：

![](data:image/png;base64,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)

范例2：

```
1 <script type="text/javascript">
 2         var names = new Array();//普通方式声明数组，不需要指明数组的长度
 3         names[0] = "孤傲苍狼";
 4         names[1] = "白虎神皇";
 5         names[2] = "灭世魔尊";
 6         for (var i = 0; i < names.length; i++) {
 7             document.write("names["+i+"] = "+names[i]);
 8             document.write("<br/>");
 9         }
10 
11         var pinyins = new Array();
12         pinyins["人"] = "ren";
13         pinyins["口"] = "kou";
14         pinyins["手"] = "shou";
15         document.write("pinyins[\"人\"] = "+pinyins["人"]);
16         document.write("<br/>");
17         document.write("pinyins.手 = "+pinyins.手);//像Hashtable、Dictionary那样用，而且像它们一样效率高。
18         document.write("<br/>");
19         //Array的简化声明
20         var arr1 = [3, 5];//普通数组初始化
21          for (var i = 0; i < arr1.length; i++) {
22             document.write("arr1["+i+"] = "+arr1[i]);
23             document.write("<br/>");
24         }
25 </script>
```

运行结果：

![](data:image/png;base64,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)

## 二、数组练习

[Ferris](http://js.fgm.cc/learn/)写过一个数组的案例，以下就是他的案例代码，挺全的，思路也挺好！

```
1 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
  2 <html xmlns="http://www.w3.org/1999/xhtml"><head>
  3 <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
  4 <title>数组练习：各种数组方法的使用</title>
  5 <style>
  6 div{color:green;padding:10px 15px;margin:12px 0;background:#f0f0f0;border:1px dotted #333;font:12px/1.5 Courier New;word-wrap:break-word;}
  7 </style>
  8 <script type="text/javascript">
  9 window.onload = function ()
 10 {
 11     var aDiv = document.getElementsByTagName("div");
 12     var aInput = document.getElementsByTagName("input");
 13     var i = 0;
 14     var bS1 = bS2 = true;
 15     var aTmp = [];
 16     
 17     //删除/添加第一项
 18     aInput[0].onclick = function ()
 19     {
 20         aTmp = getArray(aDiv[0].innerHTML);
 21         bS1 ?
 22         //删除第一项， shift()方法
 23         (aTmp.shift(), this.value = this.value.replace("删除","添加"), bS1 = false) :
 24         //添加第一项, unshift()方法
 25         (aTmp.unshift("January(1)"), this.value = this.value.replace("添加","删除"), bS1 = true);
 26         //输出
 27         aDiv[0].innerHTML = aTmp.join()
 28     };
 29     
 30     
 31     //删除/添加最后一项
 32     aInput[1].onclick = function ()
 33     {
 34         aTmp = getArray(aDiv[0].innerHTML);
 35         bS2 ?
 36         //删除最后一项, pop()方法
 37         (aTmp.pop(), this.value = this.value.replace("删除","添加"), bS2 = false) :
 38         //添加最后一项, push()方法
 39         (aTmp.push("December(12)"), this.value = this.value.replace("添加","删除"), bS2 = true);
 40         //输出
 41         aDiv[0].innerHTML = aTmp.join()
 42     };
 43     
 44     
 45     //复制, concat()方法
 46     aInput[2].onclick = function ()
 47     {
 48         aTmp = getArray(aDiv[1].innerHTML);
 49         //输出
 50         aDiv[1].innerHTML = aTmp.concat(aTmp).toString().replace(/\s/g,"")
 51     };
 52     
 53     
 54     //还原, 利用数组的 length 特点
 55     aInput[3].onclick = function ()
 56     {
 57         aTmp = getArray(aDiv[1].innerHTML);
 58         //设置数组长度
 59         aTmp.length = 10;
 60         //输出
 61         aDiv[1].innerHTML = aTmp.join()
 62     };
 63     
 64     
 65     //第三组数据还原
 66     aInput[4].onclick = function ()
 67     {
 68         aTmp = ["red","green","blue","white","yellow","black","brown"];
 69         //输出
 70         aDiv[2].innerHTML = aTmp.join()
 71     };
 72     
 73     
 74     //删除前三项
 75     aInput[5].onclick = function ()
 76     {
 77         aTmp = getArray(aDiv[2].innerHTML);
 78         //删除, 0开始, 删除3个
 79         aTmp.splice(0, 3);    
 80         //输出
 81         aDiv[2].innerHTML = aTmp.join()
 82     };
 83     
 84     
 85     //删除第二至三项
 86     aInput[6].onclick = function ()
 87     {
 88         aTmp = getArray(aDiv[2].innerHTML);
 89         //删除, 2开始, 删除2个
 90         aTmp.splice(1, 2);    
 91         //输出
 92         aDiv[2].innerHTML = aTmp.join()
 93     };
 94     
 95     
 96     //在第二顶后插入"orange", "purple"
 97     aInput[7].onclick = function ()
 98     {
 99         aTmp = getArray(aDiv[2].innerHTML);
100         //插入, 2开始, 插入"orange", "purple"
101         aTmp.splice(1, 0, "orange", "purple");    
102         //输出
103         aDiv[2].innerHTML = aTmp.join()
104     };
105     
106     
107     //替换第二项和第三项
108     aInput[8].onclick = function ()
109     {
110         aTmp = getArray(aDiv[2].innerHTML);
111         //插入, 2开始替换
112         aTmp.splice(1, 2, "#009900", "#0000ff");    
113         //输出
114         aDiv[2].innerHTML = aTmp.join()
115     };
116     
117     //将div中的内容转为数组
118     //str    div对象
119     function getArray(str)
120     {
121         aTmp.length = 0;
122         str = str.split(",");
123         for (var i in str)aTmp.push(str[i]);
124         return aTmp
125     }
126 }
127 </script>
128 </head>
129 <body>
130 <div>January(1),February(2),March(3),April(4),May(5),June(6),July(7),Aguest(8),September(9),October(10),November(11),December(12)</div>
131 <input value="删除January(1)" type="button">
132 <input value="删除December(12)" type="button">
133 <div>0,1,2,3,4,5,6,7,8,9</div>
134 <input value="复制" type="button">
135 <input value="还原" type="button">
136 <div>red,green,blue,white,yellow,black,brown</div>
137 <input value="还原" type="button">
138 <input value="删除前三项" type="button">
139 <input value="删除第二至三项" type="button">
140 <input value="在第二项后插入(orange, purple)" type="button">
141 <input value="替换第二项和第三项" type="button">
142 
143 
144 </body></html>
```









