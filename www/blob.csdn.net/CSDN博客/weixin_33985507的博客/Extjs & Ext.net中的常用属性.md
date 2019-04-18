# Extjs & Ext.net中的常用属性 - weixin_33985507的博客 - CSDN博客
2013年05月07日 18:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
Extjs & Ext.Net 弹出整个浏览器对话框的方法
top.Ext.Msg.alert("值");
top.Ext.Msg.confirm("值");
top.Ext.Msg.notify("值");
Ext.getCmp("控件ID").setText("值");  //设置控件的Text值
Ext.getCmp("控件ID").setValue("值");  //设置控件的Value值
Ext.getCmp("控件ID").getText("值");  //获取控件的Text值
Ext.getCmp("控件ID").getValue("值");  //获取控件的Value值
grid中获取表格的焦点
Ext.getCmp(‘entryItemGrid’).startEditing(行，列);  ‘entryItemGrid’是对应grid的ID
获取当前文本框的焦点
Ext.getCmp(’selectRow’).focus(true,10);   ’selectRow’是对应文本框的ID，true是否选中。10表示延迟时间
store数据的绝对匹配
findDs.findExact(‘lgid’,gid);   这个是绝对匹配的函数，第一个参数是要匹配的字段。。第二个是要匹配的指
获取grid行的焦点
Ext.getCmp(‘list_Good_Grid’).getSelectionModel().selectRow(0);
list_Good_Grid是对应grid的ID。getSelectionModel函数是得到区域模型。selectRow得到一行
下拉框取值问题
ComboBox中有个,hiddenName : ”属性，里面写的什么字段。getValue()就得到的是什么字段
但是如果你getValue（）给的是id的值，但是又想得到文本框的值，可以用getRawValue（）
求stort里面某个字段的值
首先得到这个stort用getStrot（）。Ext.getCmp(‘entryItemGrid’).getStore();
然后用get(‘id’)，来得到某个字段的值。要求那个字段就将这个字段放入括号
移除strot里面的值
Ext.getCmp(‘entryItemGrid’).getStore().removeAll();
向一个store里面加值
getGoodStore.loadData(items,true);  getGoodStore是一个store  
**loadData** ( `Object data, ``Boolean add `)
**combobox 获得焦点即下拉的问题总结**
以前我使用这个方式来做： listeners:{'focus':{fn:function(e){e.onTriggerClick();},buffer:200}},
这个方法有一个问题就是用户在点击下拉箭头时，会同时触发focus和expand事件，而onTriggerClick中包含expand事件，故会两次触发expand事件，造成冲突。
解决方法如下：listeners:{'focus':{fn:function(e){e.expand();this.doQuery(this.allQuery, true);},buffer:200}},
Extjs设置控件的状态不可用。   Ext.getCmp("控件ID").setDisabled(true);
设置Grid的某行某列的背景颜色     Grid控件ID.getView().getCell(行数, 列数).style.backgroundColor = 'red';
 draggable:false   禁止extjs window的拖动
 tabPanel.getActiveTab()     获取当前选中的tabpanel对象
grid.getView().getRow(girdcount).style.height='80px';   行高
