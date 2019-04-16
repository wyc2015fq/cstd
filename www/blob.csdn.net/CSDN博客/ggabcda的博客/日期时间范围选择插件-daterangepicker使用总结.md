# 日期时间范围选择插件-daterangepicker使用总结 - ggabcda的博客 - CSDN博客





2017年09月14日 15:50:18[ggabcda](https://me.csdn.net/ggabcda)阅读数：10386








分享说明:

　　项目中要使用日期时间范围选择对数据进行筛选;精确到年月日 时分秒;起初,使用了layui的时间日期选择插件;但是在IIE8第一次点击会报设置格式错误;研究了很久没解决,但能确定不是layui的问题;因为自己写的demo可以在IE8运行;只是在我的项目环境下某些代码冲突了;所以换用了bootstrap插件daterangepicker;看了很多资料;结合官网了文档;基本算是搞定了;把我的总结代码分享给大家;希望对使用daterangepicker插件的初学者有帮助.

　　总结分为四个部分;**日期范围选择实现**,**日期时间选择**,**使用两个单日历实现范围选择**,**使用div代替input实现日期时间选择**;下面是代码

css 代码

```
<style type="text/css">
    body,
    ul,
    p,
    h3,
    img,
    input {
        margin: 0;
        padding: 0;
    }

    .box {
        display: block;
        text-align: center;
        margin: 20px auto;
    }

    input {
        width: 400px;
        height: 40px;
    }

    label {
        display: inline-block;
        width: 90px;
        line-height: 40px;
        height: 40px;
        margin: 0;
        font-weight: normal;
        font-family: "宋体";
        background-color: #ddd;
    }
    .divDateSelect{
        width: 185px;
        height: 50px;
        line-height: 50px;
        margin:10px auto;
        border:2px solid #ddd;
        border-radius: 5px;
    }
    </style>
```

html代码:

```
1     <!-- 日期时间范围选择代码 -->
 2     <div class="box">
 3         <label for="datePicker">双日历</label>
 4         <input type="text" name="datePicker" class="datePicker" id="datePicker">
 5     </div>
 6     <!-- 日期时间选择代码 -->
 7     <div class="box">
 8         <label for="singledatePicker">单日历</label>
 9         <input type="text" name="singledatePicker" class="singledatePicker" id="singledatePicker">
10     </div>
11     <!-- 两个单日历实现日期时间范围选择代码 -->
12     <div class="box">
13         <label for="from">从</label>
14         <input type="text" name="from" class="from" id="from">
15         <label for="to">到</label>
16         <input type="text" name="to" class="to" id="to">
17     </div>
18     <!-- 不使用input,用div实现代码 -->
19     <div class="divDateSelect" id="divDateSelect">
20         &nbsp;<i class="glyphicon glyphicon-calendar fa fa-calendar"></i>
21         <span></span> <b class="caret"></b>
22     </div>
```

js 代码;按照上下顺序对应html四部分

```
1     $('input[name="datePicker"]').daterangepicker({
 2         timePicker: true, //显示时间
 3         timePicker24Hour: true, //时间制
 4         timePickerSeconds: true, //时间显示到秒
 5         startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
 6         endDate: moment(new Date()), //设置结束器日期
 7         maxDate: moment(new Date()), //设置最大日期
 8         "opens": "center",
 9         ranges: {
10             // '今天': [moment(), moment()],
11             '昨天': [moment().subtract(1, 'days'), moment().subtract(1, 'days')],
12             '上周': [moment().subtract(6, 'days'), moment()],
13             '前30天': [moment().subtract(29, 'days'), moment()],
14             '本月': [moment().startOf('month'), moment().endOf('month')],
15             '上月': [moment().subtract(1, 'month').startOf('month'), moment().subtract(1, 'month').endOf('month')]
16         },
17         showWeekNumbers: true,
18         locale: {
19             format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
20             applyLabel: '确定', //确定按钮文本
21             cancelLabel: '取消', //取消按钮文本
22             customRangeLabel: '自定义',
23             daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
24             monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
25                 '七月', '八月', '九月', '十月', '十一月', '十二月'
26             ],
27             firstDay: 1
28         },
29     }, function(start, end, label) {
30         timeRangeChange = [start.format('YYYY-MM-DD HH:mm:ss'), end.format('YYYY-MM-DD HH:mm:ss')];
31         console.log(timeRangeChange);
32     });
```

```
1     $('input[name="singledatePicker"]').daterangepicker({
 2         "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
 3         singleDatePicker: true, //单日历
 4         showDropdowns: true, //年月份下拉框
 5         timePicker: true, //显示时间
 6         timePicker24Hour: true, //时间制
 7         timePickerSeconds: true, //时间显示到秒
 8         startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
 9         maxDate: moment(new Date()), //设置最大日期
10         "opens": "center",
11         showWeekNumbers: true,
12         locale: {
13             format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
14             applyLabel: '确定', //确定按钮文本
15             cancelLabel: '取消', //取消按钮文本
16             daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
17             monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
18                 '七月', '八月', '九月', '十月', '十一月', '十二月'
19             ],
20             firstDay: 1
21         },
22     }, function(start) {
23         console.log(start.format('YYYY-MM-DD HH:mm:ss'));
24     });
```

```
1     var minDate = null;
 2     var max = null;
 3     function fromDate(maxDate) {
 4         if(!maxDate){
 5             max = moment(new Date())
 6         }else{
 7             max = maxDate;
 8         }
 9         $('input[name="from"]').daterangepicker({
10             "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
11             singleDatePicker: true, //单日历
12             showDropdowns: true, //年月份下拉框
13             timePicker: true, //显示时间
14             timePicker24Hour: true, //时间制
15             timePickerSeconds: true, //时间显示到秒
16             // startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
17             maxDate: max , //设置最大日期
18             "opens": "center",
19             showWeekNumbers: true,
20             locale: {
21                 format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
22                 applyLabel: '确定', //确定按钮文本
23                 cancelLabel: '取消', //取消按钮文本
24                 daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
25                 monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
26                     '七月', '八月', '九月', '十月', '十一月', '十二月'
27                 ],
28                 firstDay: 1
29             },
30         }, function(s) {
31             toDate(s);
32         });
33     }
34     fromDate()
35     function toDate(minDate) {
36         $('input[name="to"]').daterangepicker({
37             "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
38             singleDatePicker: true, //单日历
39             showDropdowns: true, //年月份下拉框
40             timePicker: true, //显示时间
41             timePicker24Hour: true, //时间制
42             timePickerSeconds: true, //时间显示到秒
43             // startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
44             maxDate: moment(new Date()), //设置最大日期
45             minDate: minDate,
46             "opens": "center",
47             showWeekNumbers: true,
48             locale: {
49                 format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
50                 applyLabel: '确定', //确定按钮文本
51                 cancelLabel: '取消', //取消按钮文本
52                 daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
53                 monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
54                     '七月', '八月', '九月', '十月', '十一月', '十二月'
55                 ],
56                 firstDay: 1
57             },
58         }, function(s) {
59             fromDate(s)
60         });
61     }
62     toDate();
```

```
1     var start = moment(new Date());
 2     function cb(start) {
 3         $('#divDateSelect span').html(start.format('YYYY-MM-DD HH:mm:ss'));
 4     }
 5     $('#divDateSelect').daterangepicker({
 6         "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
 7         singleDatePicker: true, //单日历
 8         showDropdowns: true, //年月份下拉框
 9         // timePicker: true, //显示时间
10         timePicker24Hour: true, //时间制
11         timePickerSeconds: true, //时间显示到秒
12         startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
13         maxDate: moment(new Date()), //设置最大日期
14         "opens": "center",
15         showWeekNumbers: true,
16         locale: {
17             format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
18             applyLabel: '确定', //确定按钮文本
19             cancelLabel: '取消', //取消按钮文本
20             daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
21             monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
22                 '七月', '八月', '九月', '十月', '十一月', '十二月'
23             ],
24             firstDay: 1
25         },
26     }, cb);
27     cb(start);
```

效果图:

第一部分:

![](http://images2017.cnblogs.com/blog/1209403/201709/1209403-20170913152146032-2009410984.png)

第二部分:

![](http://images2017.cnblogs.com/blog/1209403/201709/1209403-20170913152209953-422301472.png)

第三部分就是两个第二部分组实现第一部分的效果;原理为在确定好开始日期后;设置选择结束日期日历的最小选择日期;在结束日期选中后;设置开始日期的最大选择日期;

第四部分:

![](http://images2017.cnblogs.com/blog/1209403/201709/1209403-20170913152606078-252914967.png)

关键选项的含义已经在代码中注释了;引入文件css包括bootstrap的css文件;daterangepicker的css文件;js包括jquery的js;bootstrap的js;daterangepicker的js以及moment.js;

　　备注:

　　1 moment.js使用了数组的indexOf()方法;但IE8不支持;需要引入兼容代码;代码地址https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf　　在polyfill下;

　　2 在IE8下;双日历的范围选择出现连个日历竖直排列问题;解决方法为给存放两个日历的盒子设置固定的宽度,足以放下两个日历的div;再把两个日历的div设置float:left即可;

　　3 官网地址;选项设置: http://www.daterangepicker.com/#options　　例子: http://www.daterangepicker.com/#examples

　　4 希望本文能够帮助初识daterangepicker的朋友.
`* *`
---恢复内容结束---

### 分享说明:

　　项目中要使用日期时间范围选择对数据进行筛选;精确到年月日 时分秒;起初,使用了layui的时间日期选择插件;但是在IIE8第一次点击会报设置格式错误;研究了很久没解决,但能确定不是layui的问题;因为自己写的demo可以在IE8运行;只是在我的项目环境下某些代码冲突了;所以换用了bootstrap插件daterangepicker;看了很多资料;结合官网了文档;基本算是搞定了;把我的总结代码分享给大家;希望对使用daterangepicker插件的初学者有帮助.

　　总结分为四个部分;**日期范围选择实现**,**日期时间选择**,**使用两个单日历实现范围选择**,**使用div代替input实现日期时间选择**;下面是代码

css 代码

```
<style type="text/css">
    body,
    ul,
    p,
    h3,
    img,
    input {
        margin: 0;
        padding: 0;
    }

    .box {
        display: block;
        text-align: center;
        margin: 20px auto;
    }

    input {
        width: 400px;
        height: 40px;
    }

    label {
        display: inline-block;
        width: 90px;
        line-height: 40px;
        height: 40px;
        margin: 0;
        font-weight: normal;
        font-family: "宋体";
        background-color: #ddd;
    }
    .divDateSelect{
        width: 185px;
        height: 50px;
        line-height: 50px;
        margin:10px auto;
        border:2px solid #ddd;
        border-radius: 5px;
    }
    </style>
```

html代码:

```
1     <!-- 日期时间范围选择代码 -->
 2     <div class="box">
 3         <label for="datePicker">双日历</label>
 4         <input type="text" name="datePicker" class="datePicker" id="datePicker">
 5     </div>
 6     <!-- 日期时间选择代码 -->
 7     <div class="box">
 8         <label for="singledatePicker">单日历</label>
 9         <input type="text" name="singledatePicker" class="singledatePicker" id="singledatePicker">
10     </div>
11     <!-- 两个单日历实现日期时间范围选择代码 -->
12     <div class="box">
13         <label for="from">从</label>
14         <input type="text" name="from" class="from" id="from">
15         <label for="to">到</label>
16         <input type="text" name="to" class="to" id="to">
17     </div>
18     <!-- 不使用input,用div实现代码 -->
19     <div class="divDateSelect" id="divDateSelect">
20         &nbsp;<i class="glyphicon glyphicon-calendar fa fa-calendar"></i>
21         <span></span> <b class="caret"></b>
22     </div>
```

js 代码;按照上下顺序对应html四部分

```
1     $('input[name="datePicker"]').daterangepicker({
 2         timePicker: true, //显示时间
 3         timePicker24Hour: true, //时间制
 4         timePickerSeconds: true, //时间显示到秒
 5         startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
 6         endDate: moment(new Date()), //设置结束器日期
 7         maxDate: moment(new Date()), //设置最大日期
 8         "opens": "center",
 9         ranges: {
10             // '今天': [moment(), moment()],
11             '昨天': [moment().subtract(1, 'days'), moment().subtract(1, 'days')],
12             '上周': [moment().subtract(6, 'days'), moment()],
13             '前30天': [moment().subtract(29, 'days'), moment()],
14             '本月': [moment().startOf('month'), moment().endOf('month')],
15             '上月': [moment().subtract(1, 'month').startOf('month'), moment().subtract(1, 'month').endOf('month')]
16         },
17         showWeekNumbers: true,
18         locale: {
19             format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
20             applyLabel: '确定', //确定按钮文本
21             cancelLabel: '取消', //取消按钮文本
22             customRangeLabel: '自定义',
23             daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
24             monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
25                 '七月', '八月', '九月', '十月', '十一月', '十二月'
26             ],
27             firstDay: 1
28         },
29     }, function(start, end, label) {
30         timeRangeChange = [start.format('YYYY-MM-DD HH:mm:ss'), end.format('YYYY-MM-DD HH:mm:ss')];
31         console.log(timeRangeChange);
32     });
```

```
1     $('input[name="singledatePicker"]').daterangepicker({
 2         "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
 3         singleDatePicker: true, //单日历
 4         showDropdowns: true, //年月份下拉框
 5         timePicker: true, //显示时间
 6         timePicker24Hour: true, //时间制
 7         timePickerSeconds: true, //时间显示到秒
 8         startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
 9         maxDate: moment(new Date()), //设置最大日期
10         "opens": "center",
11         showWeekNumbers: true,
12         locale: {
13             format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
14             applyLabel: '确定', //确定按钮文本
15             cancelLabel: '取消', //取消按钮文本
16             daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
17             monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
18                 '七月', '八月', '九月', '十月', '十一月', '十二月'
19             ],
20             firstDay: 1
21         },
22     }, function(start) {
23         console.log(start.format('YYYY-MM-DD HH:mm:ss'));
24     });
```

```
1     var minDate = null;
 2     var max = null;
 3     function fromDate(maxDate) {
 4         if(!maxDate){
 5             max = moment(new Date())
 6         }else{
 7             max = maxDate;
 8         }
 9         $('input[name="from"]').daterangepicker({
10             "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
11             singleDatePicker: true, //单日历
12             showDropdowns: true, //年月份下拉框
13             timePicker: true, //显示时间
14             timePicker24Hour: true, //时间制
15             timePickerSeconds: true, //时间显示到秒
16             // startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
17             maxDate: max , //设置最大日期
18             "opens": "center",
19             showWeekNumbers: true,
20             locale: {
21                 format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
22                 applyLabel: '确定', //确定按钮文本
23                 cancelLabel: '取消', //取消按钮文本
24                 daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
25                 monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
26                     '七月', '八月', '九月', '十月', '十一月', '十二月'
27                 ],
28                 firstDay: 1
29             },
30         }, function(s) {
31             toDate(s);
32         });
33     }
34     fromDate()
35     function toDate(minDate) {
36         $('input[name="to"]').daterangepicker({
37             "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
38             singleDatePicker: true, //单日历
39             showDropdowns: true, //年月份下拉框
40             timePicker: true, //显示时间
41             timePicker24Hour: true, //时间制
42             timePickerSeconds: true, //时间显示到秒
43             // startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
44             maxDate: moment(new Date()), //设置最大日期
45             minDate: minDate,
46             "opens": "center",
47             showWeekNumbers: true,
48             locale: {
49                 format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
50                 applyLabel: '确定', //确定按钮文本
51                 cancelLabel: '取消', //取消按钮文本
52                 daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
53                 monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
54                     '七月', '八月', '九月', '十月', '十一月', '十二月'
55                 ],
56                 firstDay: 1
57             },
58         }, function(s) {
59             fromDate(s)
60         });
61     }
62     toDate();
```

```
1     var start = moment(new Date());
 2     function cb(start) {
 3         $('#divDateSelect span').html(start.format('YYYY-MM-DD HH:mm:ss'));
 4     }
 5     $('#divDateSelect').daterangepicker({
 6         "autoApply": true, //选择日期后自动提交;只有在不显示时间的时候起作用timePicker:false
 7         singleDatePicker: true, //单日历
 8         showDropdowns: true, //年月份下拉框
 9         // timePicker: true, //显示时间
10         timePicker24Hour: true, //时间制
11         timePickerSeconds: true, //时间显示到秒
12         startDate: moment().hours(0).minutes(0).seconds(0), //设置开始日期
13         maxDate: moment(new Date()), //设置最大日期
14         "opens": "center",
15         showWeekNumbers: true,
16         locale: {
17             format: "YYYY-MM-DD HH:mm:ss", //设置显示格式
18             applyLabel: '确定', //确定按钮文本
19             cancelLabel: '取消', //取消按钮文本
20             daysOfWeek: ['日', '一', '二', '三', '四', '五', '六'],
21             monthNames: ['一月', '二月', '三月', '四月', '五月', '六月',
22                 '七月', '八月', '九月', '十月', '十一月', '十二月'
23             ],
24             firstDay: 1
25         },
26     }, cb);
27     cb(start);
```

效果图:

第一部分:

![](http://images2017.cnblogs.com/blog/1209403/201709/1209403-20170913152146032-2009410984.png)

第二部分:

![](http://images2017.cnblogs.com/blog/1209403/201709/1209403-20170913152209953-422301472.png)

第三部分就是两个第二部分组实现第一部分的效果;原理为在确定好开始日期后;设置选择结束日期日历的最小选择日期;在结束日期选中后;设置开始日期的最大选择日期;

第四部分:

![](http://images2017.cnblogs.com/blog/1209403/201709/1209403-20170913152606078-252914967.png)

关键选项的含义已经在代码中注释了;引入文件css包括bootstrap的css文件;daterangepicker的css文件;js包括jquery的js;bootstrap的js;daterangepicker的js以及moment.js;

　　备注:

　　1 moment.js使用了数组的indexOf()方法;但IE8不支持;需要引入兼容代码;代码地址https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/Array/indexOf　　在polyfill下;

　　2 在IE8下;双日历的范围选择出现连个日历竖直排列问题;解决方法为给存放两个日历的盒子设置固定的宽度,足以放下两个日历的div;再把两个日历的div设置float:left即可;

　　3 官网地址;选项设置: http://www.daterangepicker.com/#options　　例子: http://www.daterangepicker.com/#examples

　　4 希望本文能够帮助初识daterangepicker的朋友.如有错误,希望大家指出
`* *`


