# JQuery EasyUI combobox动态添加option - BlueSky - CSDN博客
2016年04月24日 15:54:54[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：2241
```
<input class="easyui-combobox" id="rwlb" name="rwlb" style="width:435px" data-options="valueField:'id', textField:'text', panelHeight:'auto'" >
```
方法一：
```
1 var data, json;
2 json = '[{"id":"年计划","text":"年计划","selected":true}]';
3 data = $.parseJSON(json);
4 $("#rwlb").combobox("loadData", data);
```
方法二：
```
1 var data,json;
2 data = [];
3 data.push({ "text": "测试", "id": 100 });
4 $("#rwlb").combobox("loadData", data);
```
