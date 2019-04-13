
# Jquery无刷新实时更新表格数据 - anlcy - 博客园






# [Jquery无刷新实时更新表格数据](https://www.cnblogs.com/camilla/p/7571585.html)
html代码：
<style>
.editbox
{
display:none
}
.editbox
{
font-size:14px;
width:70px;
background-color:\#ffffcc;
border:solid1px\#000;
padding:4px;
}
.edit_tr:hover
{
background:url(edit.png)right no-repeat\#80C8E5;
cursor:pointer;
}
</style><table width="600" align="center">
<tr class="head">
<th>First</th><th>Last</th>
</tr>
<?php
$sql=mysql_query("select * from add_delete_record");
$i=1;
while($row=mysql_fetch_array($sql))
{
$id=$row['id'];
$content=$row['content'];
$text=$row['text'];
if($i%2)
{
?>
<tr id="<?php echo $id; ?>" class="edit_tr">
<?php } else { ?>
<tr id="<?php echo $id; ?>" bgcolor="\#f2f2f2" class="edit_tr">
<?php } ?>
<td width="50%" class="edit_td">
<span id="first_<?php echo $id; ?>" class="text"><?php echo $content; ?></span>
<input type="text" value="<?php echo $content; ?>" class="editbox" id="first_input_<?php echo $id; ?>" />
</td>
<td width="50%" class="edit_td">
<span id="last_<?php echo $id; ?>" class="text"><?php echo $text; ?></span>
<input type="text" value="<?php echo $text; ?>"  class="editbox" id="last_input_<?php echo $id; ?>"/>
</td>
</tr>
<?php
$i++;
}
?>
</table>
<script type="text/javascript">
$(document).ready(function()
{
$(".edit_tr").click(function()
{
var ID=$(this).attr('id');
$("\#first_"+ID).hide();
$("\#last_"+ID).hide();
$("\#first_input_"+ID).show();
$("\#last_input_"+ID).show();
}).change(function()
{
var ID=$(this).attr('id');
var first=$("\#first_input_"+ID).val();
var last=$("\#last_input_"+ID).val();
var dataString = 'id='+ ID +'&content='+first+'&text='+last;
$("\#first_"+ID).html('<img src="load.gif" />');

if(first.length && last.length>0)
{
$.ajax({
async: true,
type: "POST",
url: "update.php",
data: dataString,
dataType: "json",
cache: false,
beforeSend: function () {
running = true;
},
success: function(html){
$("\#first_"+ID).html(html.first);
$("\#last_"+ID).html(html.last);
} ,
error: function (result) {
console.log("erroe" + result);
},
});
}
else
{
alert('不能为空.');
}
});
$(".editbox").mouseup(function()
{
return false
});
$(document).mouseup(function()
{
$(".editbox").hide();
$(".text").show();
});
});
</script>
PHP代码：
<?php
include_once('conn.php');
if($_POST['id'])
{
$id=mysql_escape_String($_POST['id']);
$content=mysql_escape_String($_POST['content']);
$text=mysql_escape_String($_POST['text']);
$sql = "update add_delete_record set content='$content',text='$text' where id='$id'";
mysql_query($sql);
$rst = array();
$rst['first'] = $content;
$rst['last'] = $text;
echo json_encode($rst);
exit;}
?>





