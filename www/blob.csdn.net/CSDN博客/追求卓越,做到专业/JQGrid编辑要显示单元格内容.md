
# JQGrid编辑要显示单元格内容 - 追求卓越,做到专业 - CSDN博客


2012年12月10日 10:06:00[Waldenz](https://me.csdn.net/enter89)阅读数：2902


makeURL方法中可以编辑要显示的单元格内容
```python
colModel:  { name: "DATESOLD", index: "DATESOLD", width: "250", formatter: makeURL }, 
    function makeURL(cellvalue, options, rowObject) {
                 alert(cellvalue);//单元格值
                 alert(rowObject["test"]); //单元格行数据类
                 alert(options["rowId"]); //Grid类中的属性
                 alert(options["colModel"]["name"]);
        return "<a href='#' style='color: blue' >" + cellvalue + "</a>";
    }
```


