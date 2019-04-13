
# JQuery removeClass和移除class的事件绑定 - 追求卓越,做到专业 - CSDN博客


2012年12月27日 14:03:59[Waldenz](https://me.csdn.net/enter89)阅读数：7010


今天遇到一个问题, 定义一个样式,且给样式绑定事件,让其只能输入数字. js函数如下:

```python
function NumbersOnlyBind() {
        $(".NumbersOnly").bind({
            keypress: function(event) {
                var key = event.which;
                if (key < 48 || key > 122 || key > 57) {
                    return false;
                }
            }
        });
    }
```
然后添加两个textbox, 第一个用于输入类型, 初始为NUM, 意思为只能输入数字模式, 第二个用于测试这种模式. 当第一个输入框为NUM时,点击按钮Apply, 则第二个输入框就只能输入数字, 第一个输入框为其他字符串时, 第二个则可以输入任何字符.
在实现时,思路是如果第一个输入框为NUM则给第二个输入框添加.NumbersOnly 样式, 否则移除该样式. 本以为移除样式后,第二个输入框中绑定的事件就可以移除, 但是事实证明是不可以移除的. 除非用unbind解除事件,因为虽然事件是通过.NumberOnly样式绑定的, 但是绑定后,这个事件就和这个输入框绑定上了,而和样式没关系了. 所以事件也不会随着样式的移除而移除..
页面源代码:

```python
<html>
<head>
<script type="text/javascript" src="./jquery/jquery-1.7.2.js"></script>
<script type="text/javascript">
$(document).ready(function() {
    function NumbersOnlyBind() {
        $(".NumbersOnly").bind({
            keypress: function(event) {
                var key = event.which;
                if (key < 48 || key > 122 || key > 57) {
                    return false;
                }
            }
        });
    }
    $("#txtType").val("NUM");
    $("#btnApply").click(function(event) {
        var type = $("#txtType").val();
        if (type == "NUM") {
            $("#txtTest").addClass("NumbersOnly");
            NumbersOnlyBind();
            // alert("numberonly");
        }
        else {
           // $("#txtTest").removeClass("NumbersOnly");
             $("#txtTest").unbind();
             //$(".NumbersOnly").unbind();//移除所有的事件
            // alert("Remove numberonly");
        }
        $("#txtTest").val("");
        $("#txtTest").attr("readonly", false);
        $("#txtTest").focus();
        event.preventDefault();
    });
});
</script>
</head>
<body>
	<input name="txtType" type="text" maxlength="10" id="txtType" >
	<input name="txtTest" type="text" maxlength="10" id="txtTest" >
	<input type="button"  name="btnApply" value="Apply" id="btnApply">
</body>
</html>
```



