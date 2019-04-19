# 微信ios/Safari中的new Date()格式化坑 - 三少GG - CSDN博客
2016年11月12日 15:49:24[三少GG](https://me.csdn.net/scut1135)阅读数：3012

I suggest you use: 
```
new
 Date (year, month [, date [, hours [, minutes [, seconds [, ms ] ] ] ] ] )
```
To split the string you could try
```
var s = '2011-06-21T14:27:28.593Z';
var a = s.split(/[^0-9]/);
//for (i=0;i<a.length;i++) { alert(a[i]); }
var d=new Date (a[0],a[1]-1,a[2],a[3],a[4],a[5] );
alert(s+ " "+d);
```
To make your question easier your problem is with:
`new Date('2014-02-18 15:00:48')`
This work okay in chrome but not in safari. The mdn talks about [ECMAScript
 5 ISO-8601 format support](http://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/parse) says:
> 
Alternatively, the date/time string may be in ISO 8601 format. For example, "2011-10-10" (just date) or "2011-10-10T14:48:00" (date and time) can be passed and parsed.
If you include `T` it
 works:
`new Date('2014-02-18T15:00:48')`
You can use 
```
new
 Date('2014-02-18T15:00:48'.replace(/\s/, 'T'))
```
.
If you handle a lot of cases like this I will recommend using [moment](http://momentjs.com/) which
 seems to handle this case very well with or without T: [parsing from string](http://momentjs.com/docs/#/parsing/string/). Additionally your whole
 example is easier with momentjs:
`var minutes = moment().diff("2014-02-18 15:00:48", 'minutes');`
