# 用NODEJS处理EXCEL文件导入导出 - z69183787的专栏 - CSDN博客
2016年05月09日 17:21:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16305
参考文章
[http://librajt.github.io/2013/08/04/handle-excel-file-with-nodejs/](http://librajt.github.io/2013/08/04/handle-excel-file-with-nodejs/)
[](http://librajt.github.io/2013/08/04/handle-excel-file-with-nodejs/)
对比了 ExcelJS ,[https://github.com/guyonroche/exceljs#create-a-workbook](http://mgcrea.github.com/node-xlsx)
[node-xlsx](http://mgcrea.github.com/node-xlsx) ，[https://github.com/mgcrea/node-xlsx](https://github.com/mgcrea/node-xlsx)
等 nodejs 等现有组件，决定使用node-xlsx。
node-xlsx 基于现有前端强大组件 js-xlsx，  [https://github.com/SheetJS/js-xlsx](https://github.com/SheetJS/js-xlsx)
使用例子：
var express = require('express');
var router = express.Router();
var xlsx = require('node-xlsx');
var fs = require('fs');
/* GET import excel test. */
router.get('/importExcel', function(req, res, next) {
 var filename='./public/test.xlsx';
 console.error(filename);
 // read from a file
var obj = xlsx.parse(filename);
console.log(JSON.stringify(obj));
 
res.send('import successfully!');
});
/* GET export excel test. */
router.get('/exportExcel', function(req, res, next) {
// write
var data = [[1,2,3],[true, false, null, 'sheetjs'],['foo','bar',new Date('2014-02-19T14:30Z'), '0.3'], ['baz', null, 'qux']];
var buffer = xlsx.build([{name: "mySheetName", data: data}]);
fs.writeFileSync('b.xlsx', buffer, 'binary');
res.send('export successfully!');
});
补充：
文件上传操作可以选择以下两种
fs                           [https://github.com/jprichardson/node-fs-extra](https://github.com/jprichardson/node-fs-extra)
formidable   [https://github.com/felixge/node-formidable](https://github.com/felixge/node-formidable)
