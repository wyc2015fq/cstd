# easyUI的下拉框combobox与树tree联动 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月15日 09:45:05[左直拳](https://me.csdn.net/leftfist)阅读数：9459
参与联动的有 2 个combobox 和 1 个tree：
```
<input id="combobox1" class="easyui-combobox" name="combobox1" style=" width:120px;" />
<input id="combobox2" class="easyui-combobox" name="combobox2" style=" width:120px;" />
<div id="tree1" class="easyui-tree"></div>
```
联动流程：
combobox1 控制 combobox2，combobox2 控制 tree1
联动代码：
```java
<script type="text/javascript">
    //先写被控制者，再写控制者。因为控制者要调用被控制者。当然，这个似乎无关紧要
    $("#combobox2").combobox({
        url:"",
        method: "get",
        valueField: 'Id',
        textField: 'Name',
        onSelect: function (record) {//控制树 <----------------------
            var url = "/api/customview/Tree?Id=" + record.Id;
            $('#tree1').tree({
                animate: true,
                checkbox: true,
                lines: true,
                url:url,
                method:"get",
                onLoadSuccess: function (row, data) {
                    $(this).tree("collapseAll");//树节点全部闭合
                },
                toolbar: [{
                    text: 'Add Group',
                    iconCls: 'icon-folder-add',
                    handler: function () {
                        alert('Add Group');
                    }
                }]
            });
        }
        , onLoadSuccess: function () { //加载完成后,设置选中第一项
            var val = $(this).combobox("getData");
            for (var item in val[0]) {
                if (item == "Id") {
                    $(this).combobox("select", val[0][item]);
                }
            }
        }
    });
    $("#combobox1").combobox({
        url: "/api/customview/Item1",
        method: "get",
        valueField: 'Id',
        textField: 'Name',
        onSelect: function (record) {//控制combobox2 <----------------------
            var url = "/api/customview/Item2?Id=" + record.Id;
            $("#combobox2").combobox('reload', url);
        }
        , onLoadSuccess: function () { //加载完成后,设置选中第一项
            var val = $(this).combobox("getData");
            for (var item in val[0]) {
                if (item == "Id") {
                    $(this).combobox("select", val[0][item]);
                }
            }
        }
    });
</script>
```
