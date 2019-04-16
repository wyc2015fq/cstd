# Jquery操作table - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Jquery操作table](https://www.cnblogs.com/xdp-gacl/p/3473861.html)



```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


  1  $(function () {
  2             SetControlDisabled(0, "", true); //控制grid的第一行的所有控件不可用             
  3             SetControlDisabled("", 1, true); //控制grid的第二列的所有控件不可用       
  4             SetControlDisabled("", "标题3", true); //控制grid的标题是‘标题3’的那一列的所有控件不可用
  5             SetControlDisabled(2, "标题5", true); //控制grid的第三行并且标题是‘标题5’的那一列的所有控件不可用
  6             //SetControlDisabled(true);控制grid里面的所有控件不可用     
  7 
  8             var controls = findControl(1);//查找第二行的所有控件
  9             for (var control in controls) {
 10                 alert(controls[control].attr("id"));
 11             }
 12 
 13             var targetControls = findControl(0, 1); //查找第一行，第2列的所有控件
 14             for (var control in targetControls) {
 15                 alert(targetControls[control].attr("id"));
 16             }
 17 
 18             String.prototype.trim = function () {
 19                 return this.replace(/^(\s|\u3000)+|(\s|\u3000)+$/g, "");
 20             }
 21 
 22             //根据表头标题找到表头所在的列索引
 23             function GetTdIndexByTdText(strTdText) {
 24                 var targetTdIndex = 0;
 25                 $("#grid thead tr").find("th").each(function (index, item) {
 26                     if ($(item).text().trim() == strTdText) {
 27                         targetTdIndex = index;
 28                         return false;
 29                     }
 30                 });
 31                 return targetTdIndex;
 32             }
 33             //查找grid里面的控件
 34             function findControl() {
 35                 var controls = [];
 36                 var controlId = null;
 37                 if (arguments.length == 1) {
 38                     var targetTr = $("#grid tbody tr").eq(arguments[0]);
 39                     targetTr.find("td").each(function (index, item) {
 40                         $(item).find("input").each(function (i_index, i_item) {
 41                             controlId = $(i_item).attr("id")
 42                             if (controlId === "") {
 43                                 controlId = newGuid();
 44                                 $(i_item).attr("id", controlId);
 45                             }
 46                             controls[controlId] = $(i_item);
 47                         });
 48                         $(item).find("select").each(function (i_index, i_item) {
 49                             controlId = $(i_item).attr("id")
 50                             if (controlId === "") {
 51                                 controlId = newGuid();
 52                                 $(i_item).attr("id", controlId);
 53                             }
 54                             controls[controlId] = $(i_item);
 55                         });
 56                         $(item).find("textarea").each(function (i_index, i_item) {
 57                             controlId = $(i_item).attr("id")
 58                             if (controlId === "") {
 59                                 controlId = newGuid();
 60                                 $(i_item).attr("id", controlId);
 61                             }
 62                             controls[controlId] = $(i_item);
 63                         });
 64                     });
 65                 } else if (arguments.length == 2) {
 66                     var targetTr = $("#grid tbody tr").eq(arguments[0]);
 67                     var targetTd = targetTr.find("td").eq(arguments[1]);
 68                     targetTd.find("input").each(function (index, item) {
 69                         controlId = $(item).attr("id")
 70                         if (controlId === "") {
 71                             controlId = newGuid();
 72                             $(item).attr("id", controlId);
 73                         }
 74                         controls[controlId] = $(item);
 75 
 76                     });
 77                     targetTd.find("select").each(function (index, item) {
 78                         controlId = $(item).attr("id")
 79                         if (controlId === "") {
 80                             controlId = newGuid();
 81                             $(item).attr("id", controlId);
 82                         }
 83                         controls[controlId] = $(item);
 84 
 85                     });
 86                     targetTd.find("textarea").each(function (index, item) {
 87                         controlId = $(item).attr("id")
 88                         if (controlId === "") {
 89                             controlId = newGuid();
 90                             $(item).attr("id", controlId);
 91                         }
 92                         controls[controlId] = $(item);
 93 
 94                     });
 95                 }
 96                 return controls;
 97             }
 98 
 99             //控制grid里面的控件是否可用
100             function SetControlDisabled(rowIndex, colIndex, bolFlag) {
101                 //SetGridControlEnabeld(rowIndex,"",bolFlag)             
102                 if (rowIndex >= 0 && colIndex == "") {
103                     var targetTr = $("#grid tbody tr").eq(rowIndex);
104                     targetTr.find("td").each(function (index, item) {
105                         SetDisabled(item, bolFlag);
106                     });
107                 }
108                 //SetGridControlEnabeld("",colIndex,bolFlag)               
109                 else if (rowIndex == "") {
110                     $("#grid tbody tr").each(function (index, item) {
111                         var td = null;
112                         if (typeof (colIndex) == "string") {
113                             td = $(item).find("td").eq(GetTdIndexByTdText(colIndex))
114                         } else {
115                             td = $(item).find("td").eq(colIndex)
116                         }
117 
118                         SetDisabled(td, bolFlag);
119                     });
120                 } else if (rowIndex >= 0 && typeof (rowIndex) == "number") {
121                     var targetTr = $("#grid tbody tr").eq(rowIndex);
122                     var targetTd = null;
123                     if (typeof (colIndex) == "string") {
124                         targetTd = targetTr.find("td").eq(GetTdIndexByTdText(colIndex))
125                     } else {
126                         targetTd = targetTr.find("td").eq(colIndex)
127                     }
128                     SetDisabled(targetTd, bolFlag);
129 
130                 } else if (arguments.length == 1) {
131                     var bolFlag = arguments[0];
132                     $("#grid tbody tr").each(function (trIndex, trItem) {
133                         $(trItem).find("td").each(function (tdIndex, tdItem) {
134                             SetDisabled(tdItem, bolFlag);
135                         });
136                     });
137                 }
138             }
139 
140             //生成 GUID 
141             function newGuid() {
142                 var guid = "";
143                 for (var i = 1; i <= 32; i++) {
144                     var n = Math.floor(Math.random() * 16.0).toString(16);
145                     guid += n;
146                     if ((i == 8) || (i == 12) || (i == 16) || (i == 20))
147                         guid += "-";
148                 }
149                 return guid;
150             }
151 
152             //设置某个容器里面的控件是否可用
153             function SetDisabled(container, bolFlag) {
154                 $(container).find("input").each(function (index, item) {
155                     $(item).attr('disabled', bolFlag);
156                 });
157                 $(container).find("select").each(function (index, item) {
158                     $(item).attr('disabled', bolFlag);
159                 });
160                 $(container).find("textarea").each(function (index, item) {
161                     $(item).attr('disabled', bolFlag);
162                 });
163             }
164         }); 


View Code
```

HTML测试代码：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1  <div id="grid">
 2         <table border="1px">
 3             <thead>
 4                 <tr>
 5                     <th title="标题1">
 6                         标题1
 7                     </th>
 8                     <th title="标题2">
 9                         标题2
10                     </th>
11                     <th title="标题3">
12                         标题3
13                     </th>
14                     <th title="标题4">
15                         标题4
16                     </th>
17                     <th title="标题5">
18                         标题5
19                     </th>
20                      <th title="标题6">
21                         标题6
22                     </th>
23                 </tr>
24             </thead>
25             <tbody>
26                 <tr>
27                     <td>
28                         <input id="Text1" type="text" />
29                     </td>
30                     <td>
31                         <select id="Select1">
32                             <option>111111111111</option>
33                         </select>
34                     </td>
35                     <td>
36                         <input id="Checkbox1" type="checkbox" />
37                     </td>
38                     <td>
39                         <input id="Radio1" type="radio" />
40                     </td>
41                     <td>
42                         <textarea cols="20" rows="2"></textarea>
43                     </td>
44                     <td>
45                         <input type="button" value="button" />
46                     </td>
47                 </tr>
48                 <tr>
49                     <td>
50                         <input id="Text2" type="text" />
51                     </td>
52                     <td>
53                         <select id="Select2">
54                             <option>111111111111</option>
55                         </select>
56                     </td>
57                     <td>
58                         <input id="Checkbox2" type="checkbox" />
59                     </td>
60                     <td>
61                         <input id="Radio2" type="radio" />
62                     </td>
63                     <td>
64                         <textarea cols="20" rows="2"></textarea>
65                     </td>
66                     <td>
67                         <input type="button" value="button" />
68                     </td>
69                 </tr>
70                 <tr>
71                     <td>
72                         <input id="Text3" type="text" />
73                     </td>
74                     <td>
75                         <select id="Select3">
76                             <option>111111111111</option>
77                         </select>
78                     </td>
79                     <td>
80                         <input id="Checkbox3" type="checkbox" />
81                     </td>
82                     <td>
83                         <input id="Radio3" type="radio" />
84                     </td>
85                     <td>
86                         <textarea id="TextArea3" cols="20" rows="2"></textarea>
87                     </td>
88                     <td>
89                         <input id="Button3" type="button" value="button" />
90                     </td>
91                 </tr>
92             </tbody>
93         </table>
94     </div>


View Code
```











