
# JS-jQuery DropDownList 添加项 - 追求卓越,做到专业 - CSDN博客


2014年02月25日 11:19:19[Waldenz](https://me.csdn.net/enter89)阅读数：4901


js为DropDownList 添加项

```python
function addItem()
{
     var tOption = document.createElement("Option");
     tOption.text='text';
     tOption.value='value';
     document.getElementById("DDL1").add(tOption);
}
```
jQuery为DropDownList 添加项

```python
$('#mySelect').empty();//Clear items
//$('#mySelect').html("");//Also clear items
var myOptions = {
    val1 : 'text1',
    val2 : 'text2'
};
var mySelect = $('#mySelect');
$.each(myOptions, function(val, text) {
    mySelect.append($('<option></option>').val(val).html(text));
});
//var=dropdownlist value, text=dropdownlist text
```

