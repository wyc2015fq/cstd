# ExtJs grid合并单元格 - 左直拳的马桶_日用桶 - CSDN博客
2015年04月29日 21:56:14[左直拳](https://me.csdn.net/leftfist)阅读数：5192
extjs中，如果要输出一些有合并单元格的表格，要怎么做呢？如图所示：
![](https://img-blog.csdn.net/20150429212955430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从网上找了个例子，其主要思想是表格的store装载完毕后，随即对这个grid的td进行一个个的控制，用的方法也是原始的javascript方法。
我针对目前在做的项目，做了一些修改。
```java
//grid的store
var gridStore = new Ext.data.Store({
    autoDestroy: true,
    pageSize: 25,
    model: 'myGridModel',
    proxy: {
        type: 'ajax',
        url: 'report.aspx?action=GetList',
        reader: {
            root: 'topics',
            totalProperty: 'totalCount'
        },
        simpleSortMode: true
    },
    remoteSort: true
    , listeners: {
        load: function () {//该store装载完毕即执行以下方法：
            afterListStoreLoad();
        }
    }
});
```
```java
function afterListStoreLoad() {
    mergeCells(Ext.getCmp("grid1"), [0, 1, 2, 3], 1, function (cellText, sumText, i, col) {
//匿名函数，对应function mergeCells(grid,cols,groupcol,setText)的参数setText	
//在调用方设置一个匿名函数，主要是因为需要合并单元格的列有各种情况，比如有的需要合计，有的属分组，有的是rownumber，不同的情况，对应不同的处理。
//如果要通过参数来一一指定类型，好让函数来处理的话，会非常麻烦。但通过匿名函数， 将处理留给调用方自行决定，不仅代码节省，逻辑也更清晰。
        var text;
        if (0 == col) {//行号
            text = i;
        } else if (2 == col || 3 == col) {
            if (cellText.length == 0) {
                cellText = 0;
            }
            if (sumText.length == 0) {
                sumText = 0;
            }
            text = sumText * 1 + cellText * 1;
        } else {
            text = cellText;
        }
        return text;
    });
}
/** 
* 合并单元格 
* @param {} grid  要合并单元格的grid对象 
* @param {} cols  要合并哪几列 [1,2,4] ,列号由左到右，从rownumber列算起，起始号码为0
* @param {} groupcol 分组列。为简单起见，目前只支持一个列。以后可以扩充到多列
*/
function mergeCells(grid,cols,groupcol,setText) {
    var arrayTr = document.getElementById(grid.getId() + "-body").firstChild.firstChild
            .getElementsByTagName('tr');
    var trCount = arrayTr.length;
    var rowspanObj = {}; //要进行跨列操作的td对象{ tr: 1,td: 2,rowspan: 5,text: 0 }  
    var removeObjs = []; //要进行删除的td对象[{tr:2,td:2},{tr:3,td:2}]
    var rowNumber = 1;
    var merge = function () { //定义合并函数  
        if (rowspanObj.rowspan != 1) {
            var arrayTd = arrayTr[rowspanObj.tr].getElementsByTagName("td"); //合并行  
            var td = arrayTd[rowspanObj.td];
            td.rowSpan = rowspanObj.rowspan;
            td.innerText = rowspanObj.text;
            td.vAlign = "middle";
            td.align = "center";
            Ext.each(removeObjs, function (obj) { //隐身被合并的单元格  
                arrayTd = arrayTr[obj.tr].getElementsByTagName("td");
                arrayTd[obj.td].style.display = 'none';
            });
        }
    };
    var addf = function (cellText, i, col) {//添加一条待合并行
        rowspanObj["rowspan"] = rowspanObj["rowspan"] + 1;
        rowspanObj["text"] = setText(cellText, rowspanObj["text"], rowNumber, col);
        removeObjs.push({
            tr: i,
            td: col
        });
        if (i == trCount - 1)
            merge();
    };
    var mergef = function (cellText,i,col) {//合并上一个分组，并开启下一个分组
        merge();
        rowspanObj = {
            tr: i,
            td: col,
            text: cellText,
            rowspan: 1
        };
        removeObjs = [];
        rowNumber++;
    };
    Ext.each(cols, function (colIndex) { //逐列去操作tr  
        var col = colIndex;
        var groupText;
        var arrayTd;
        if (trCount > 0) {
            arrayTd = arrayTr[0].getElementsByTagName("td");
            groupText = arrayTd[groupcol].innerText;
            rowspanObj = {
                tr: 0,
                td: col,
                text: arrayTd[col].innerText,
                rowspan: 1
            }
            for (var i = 1; i < trCount; i++) {
                arrayTd = arrayTr[i].getElementsByTagName("td");
                var cellText = arrayTd[col].innerText;
                if (arrayTd[groupcol].innerText == groupText) {
                    addf(cellText, i, col);
                } else {//合并列
                    mergef(cellText, i, col);
                    groupText = arrayTd[groupcol].innerText;
                }
            }
        }
    });
};
```

参考文章：
[点击打开链接](http://www.cnblogs.com/small-bai/p/ExtJs-grid%E5%90%88%E5%B9%B6%E5%8D%95%E5%85%83%E6%A0%BC-%E8%B7%A8%E8%A1%8C%E8%B7%A8%E5%88%97.html)
