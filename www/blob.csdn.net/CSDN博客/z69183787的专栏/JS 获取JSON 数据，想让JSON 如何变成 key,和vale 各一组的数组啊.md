# JS 获取JSON 数据，想让JSON 如何变成 key,和vale 各一组的数组啊 - z69183787的专栏 - CSDN博客
2013年12月11日 09:59:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3310
``
`var``arr=``new``Array();`
`    ``var``arr1=``new``Array();`
`    ``var``j=0;`
`    ``for``(``var``i ``in``formdata){`
`        ``//alert(i);`
`        ``arr[j]=i;`
`        ``arr1[j]=formdata[i];`
`        ``j++;`
`    ``}`
`    ``alert(arr);`
`    ``alert(arr1);`
```
这样即可
```
```
```
