# Html.DropDownListFor的选项值为字符型问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年04月05日 17:40:44[左直拳](https://me.csdn.net/leftfist)阅读数：793
我快要疯了。[asp.net](http://asp.net) mvc的这个DropDownListFor，无论在服务器端如何设置，设置哪个值被选中，结果到了页面输出，选中值根本没有被选中，没有任何一个值被选中，下拉框只冷冰冰地显示一个“–请选择–”。
强调一下，如果对于 value 是数值型，而text是字符型来说，不存在以上问题；但如果value 和 text ，都是字符型，问题就像开头描述的那样。
我靠，太玄乎了吧。
花了好长时间，累到差点虚脱，我都怀疑自己是不是太老，不再适合搞开发了。连这种简单的问题都搞不定，说不过去啊。
后来发现：
1、服务器端设置哪个值被选中，或者默认值是啥，对于 DropDownListFor 不起作用，它会根据绑定的属性自动匹配；对 DropDownList 有作用；
比如：
```
public enum EYJStatus
    {
        [EnumTitle("解除")]
        解除,
        [EnumTitle("蓝色")]
        蓝色,
        [EnumTitle("黄色")]
        黄色,
        [EnumTitle("橙色")]
        橙色,
        [EnumTitle("红色")]
        红色
    }
无论是在服务器端指定默认值
var yjStatus = new SelectList(EnumHelper.GetItemKeyList<EYJStatus>().Select(s => s.ToString()).ToList(),"蓝色");
还是不指定
var yjStatus = new SelectList(EnumHelper.GetItemKeyList<EYJStatus>().Select(s => s.ToString()).ToList());
然后
ViewData.Add("STATE", yjStatus);
在前端，都没有影响：
@Html.DropDownListFor(m => m.STATE, null, "--请选择--", new { })
```
2、对于DropDownListFor，其参数 IEnumerable selectList，似乎只能设置成null，然后由系统自动对应到ViewData[“属性名”]，通过这种方式才能起作用。比如：
```
var yjStatus = new SelectList(EnumHelper.GetItemKeyList<EYJStatus>().Select(s => s.ToString()).ToList());
ViewData.Add("STATE", yjStatus);
//有效，且能解决本文所说的问题
@Html.DropDownListFor(m => m.STATE, null, "--请选择--", new { })
//无效
@Html.DropDownListFor(m => m.STATE, ViewData["STATE"] as IEnumerable<SelectListItem>, "--请选择--", new { })
```
3、DropDownListFor 会设置 IEnumerable selectList 的值
比如：
```
//服务器端
var yjStatus = EnumHelper.GetItemKeyList<EYJStatus>().Select(s => new SelectListItem()
{
    Text = s.ToString(),
    Value = s.ToString(),
    Selected = (s.ToString() == "蓝色") ? true : false
}).ToList();
ViewData.Add("STATE", yjStatus);
//前端
@Html.DropDownListFor(m => m.STATE, null, "--请选择--", new { })
@Html.DropDownList("STATE2", ViewData["STATE"] as IEnumerable<SelectListItem>, "--请选择--", new { })
```
结果第二个下拉框的默认值不再是“蓝色”，有可能是其他值；假如将两个下拉框调换一下位置：
```
@Html.DropDownList("STATE2", ViewData["STATE"] as IEnumerable<SelectListItem>, "--请选择--", new { })
@Html.DropDownListFor(m => m.STATE, null, "--请选择--", new { })
```
则处于前面的下拉框永远显示“蓝色”。
[如果能读一下asp.net](http://xn--asp-p18d1by31i9ixbn0cvxu.net) mvc的相关源代码，应该可以明了个中原因吧。不过，要读懂并非易事，即使读懂，也改变了什么。
把时间花在这种应用型的处理办法上，真窝囊。但又不能不处理。再有，参数 IEnumerable selectList 一定要设置成null？这明显不符合常理，期望以后找到答案，或者有谁能告诉我一下。
这个问题今天依旧困扰我，很垃圾的一个东西。我索性不用它的这个DropDownList了，不就是一个下拉框吗？我直接用循环
```
@{ 
    var sels = (ViewData["CATEGORY"] as IEnumerable<SelectListItem>).ToList();
    sels.Insert(0, new SelectListItem()
    {
        Text = "--请选择--",
        Value = "-1",
    });
}
<select id="CATEGORY" name="CATEGORY">
    @foreach (var item in sels)
    {
        if (item.Selected)
        {
            <option value="@item.Value" selected>@item.Text</option>
        }
        else
        {
            <option value="@item.Value">@item.Text</option>
        }
    }
</select>
```
