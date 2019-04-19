# easyUI里的checkbox编辑 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月13日 18:28:43[左直拳](https://me.csdn.net/leftfist)阅读数：13473
数据源如果有布尔值，那么在UI里，最合适的控件应该就是checkbox了。
easyUI的datagrid中，列的checkbox酱紫设置：
```java
{field:'status',title:'Status',width:50,align:'center',
	editor:{
		type:'checkbox',
		options:{
			on: "true",
			off: "false"
		}
	}
},
```
数据：
```java
{"total":28,"rows":[
	{"productid":"FI-SW-01","unitcost":10.00,"status":true,"listprice":16.50,"attr1":"Large","itemid":"EST-1"},
	{"productid":"K9-DL-01","unitcost":12.00,"status":true,"listprice":18.50,"attr1":"Spotted Adult Female","itemid":"EST-10"},
	{"productid":"RP-SN-01","unitcost":12.00,"status":true,"listprice":18.50,"attr1":"Venomless","itemid":"EST-11"}
]}
```
这样子就有个问题：
当datagrid里的行处于编辑状态时，checkbox并未能自动带上应有的值，比如本来对应是true，但当checkbox出现时，并没有自动勾选！造成很不好的后果：编辑一次，如果不手动对这个checkbox再打钩一次，那么原本是true，现在就变成了 false ！什么毛病。
对应办法是将数据里的布尔值变为字符串："status":true ==> "status":"true"
```java
{"total":28,"rows":[
	{"productid":"FI-SW-01","unitcost":10.00,"status":"true","listprice":16.50,"attr1":"Large","itemid":"EST-1"},
	{"productid":"K9-DL-01","unitcost":12.00,"status":"true","listprice":18.50,"attr1":"Spotted Adult Female","itemid":"EST-10"},
	{"productid":"RP-SN-01","unitcost":12.00,"status":"true","listprice":18.50,"attr1":"Venomless","itemid":"EST-11"}
]}
```
什么原因尚不清楚。可能是在checkbox中，值是字符型的。虽然它常被用于呈现布尔值，但它不认识什么布尔值。
