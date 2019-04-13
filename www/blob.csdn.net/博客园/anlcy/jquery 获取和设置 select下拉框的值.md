
# jquery 获取和设置 select下拉框的值 - anlcy - 博客园






# [jquery 获取和设置 select下拉框的值](https://www.cnblogs.com/camilla/p/6823558.html)
获取Select ：
获取select 选中的 text :
$("\#ddlRegType").find("option:selected").text();
获取select选中的 value:
$("\#ddlRegType ").val();
获取select选中的索引:
$("\#ddlRegType ").get(0).selectedIndex;
设置select:
设置select 选中的索引：
$("\#ddlRegType ").get(0).selectedIndex=index;//index为索引值
设置select 选中的value：
$("\#ddlRegType ").attr("value","Normal“);
$("\#ddlRegType ").val("Normal");
$("\#ddlRegType ").get(0).value = value;
设置select 选中的text:
var count=$("\#ddlRegType option").length;
for(var i=0;i<count;i++)
{          
if($("\#ddlRegType ").get(0).options[i].text == text)
{
$("\#ddlRegType ").get(0).options[i].selected = true;
break;
}
}
$("\#select_id
option[text='jQuery']").attr("selected", true);
设置select option项:
$("\#select_id").append("<option
value='Value'>Text</option>");  //添加一项option
$("\#select_id").prepend("<option value='0'>请选择</option>"); //在前面插入一项option
$("\#select_id option:last").remove(); //删除索引值最大的Option
$("\#select_id option[index='0']").remove();//删除索引值为0的Option
$("\#select_id
option[value='3']").remove(); //删除值为3的Option
$("\#select_id option[text='4']").remove(); //删除TEXT值为4的Option
清空 Select:
$("\#ddlRegType ").empty();





