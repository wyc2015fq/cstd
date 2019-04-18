# serializeArray()与 serialize() - z69183787的专栏 - CSDN博客
2013年11月01日 16:22:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11832
serialize()序列表表格内容为字符串，用于 Ajax 请求。
serializeArray()序列化表格元素 (类似 '.serialize()' 方法) 返回 JSON 数据结构数据。
.serializeArray()
编码一个form元素集合为一个“name属性值/value属性值”的数组。json结构，如：
[
  {
    name: a
    value: 1
  },
  {
    name: b
    value: 2
  },
  {
    name: c
    value: 3
  },
  {
    name: d
    value: 4
  },
  {
    name: e
    value: 5
  }
]
.serialize()
编码一个form元素集合为有充的字符串。字符串为标准的url编码。如：
a=1&b=2&c=3&d=4&e=5
<form>
    <!-- 能够被json数组、url字符串据提取数据类型 -->
	<input type="text" name="a" value="1" id="a" />
	<input type="text" name="b" value="2" id="b" />
	<input type="hidden" name="c" value="3" id="c" />
	<textarea name="d" rows="8" cols="40">4</textarea>
	<select name="e">
            <option value="5" selected="selected">5</option>
            <option value="6">6</option>
            <option value="7">7</option>
  	</select>
    	<input type="checkbox" name="f" value="8" id="f1" checked="checked" />
        <input type="checkbox" name="f" value="9" id="f2" />
        <input type="radio" name="g" value="10" id="g1"/>
	<input type="radio" name="g" value="11" checked="checked" id="g2"/>
        <input type="password" name="h" maxlength="8" value="h" />
    <!-- 不能够被json、url字符串据提取数据类型 -->   
        <input type="submit" name="i" value="Submit" id="i" />
        <input type="button" name="j" value="Click me" onclick="msg()" />
        <input type="file" name="k" /><br />
        <input type="reset" value="Reset"  name="l" />
        <input type="image" src="/i/eg_submit.jpg" alt="Submit" />
</form>
```
<p><tt id="results"></tt></p>
```
``
```
<script>
    function showValues() {
      var fields = $(":input").serializeArray();
      $("#results").empty();
      jQuery.each(fields, function(i, field){
        $("#results").append(field.value + " ");
      });
    }
    $(":checkbox, :radio").click(showValues);
    $("select").change(showValues);
    showValues();
</script>
```
```
<script>
    function showValues() {
      var str = $("form").serialize();
      $("#results").text(str);
    }
    $(":checkbox, :radio").click(showValues);
    $("select").change(showValues);
    showValues();
</script>
```
