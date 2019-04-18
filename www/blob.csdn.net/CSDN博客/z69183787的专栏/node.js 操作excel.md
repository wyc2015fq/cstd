# node.js 操作excel - z69183787的专栏 - CSDN博客
2016年05月09日 17:22:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1088
首先安装依赖库node-xlsx
npm install node-xlsx
在操作文件中直接引用
var xlsx = require("node-xlsx");
读取excel
var list = xlsx.parse("./excel/" + excelName);
//读出后是数组，包含每个sheet
[
    { name: 'sheet1',data: [ [Object], [Object], [Object], [Object], [Object] ] },
    { name: 'sheet2', data: [ [Object] ] }
]
name=sheet名称
data=每个sheet的数据，
剩下的就灵活操作咯......
