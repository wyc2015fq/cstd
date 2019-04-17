# JavaScript对象之正则表达式-RegExp - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月24日 10:58:02[boonya](https://me.csdn.net/boonya)阅读数：859
所属专栏：[JavaScript学习提升](https://blog.csdn.net/column/details/jsmanlan.html)









正则表达式在很多地方都会用到，应用最多的就是JS的表达验证，下面来学习怎么使用正则表达式。在JavaScript中用RegExp 对象表示正则表达式，它是对字符串执行模式匹配的强大工具。

# RegExp正则表达式语法



```java
new RegExp(pattern, attributes);
```



### 参数

1.参数*pattern* 是一个字符串，指定了正则表达式的模式或其他正则表达式。



2.参数 *attributes* 是一个可选的字符串，包含属性 "g"、"i" 和 "m"，分别用于指定全局匹配、区分大小写的匹配和多行匹配。ECMAScript 标准化之前，不支持 m 属性。如果*pattern* 是正则表达式，而不是字符串，则必须省略该参数。



### 返回值



一个新的 RegExp 对象，具有指定的模式和标志。如果参数 *pattern* 是正则表达式而不是字符串，那么 RegExp() 构造函数将用与指定的 RegExp 相同的模式和标志创建一个新的 RegExp 对象。

如果不用 new 运算符，而将 RegExp() 作为函数调用，那么它的行为与用 new 运算符调用时一样，只是当 *pattern* 是正则表达式时，它只返回*pattern*，而不再创建一个新的 RegExp 对象。

### 异常抛出

注：异常是指程序出错，JS在出错的情况下不能继续往下执行，键盘按“F12”查看控制台，浏览器控制台会抛出错误信息。


SyntaxError - 如果 *pattern* 不是合法的正则表达式，或 *attributes* 含有 "g"、"i" 和 "m" 之外的字符，抛出该异常。

TypeError - 如果 *pattern* 是 RegExp 对象，但没有省略 *attributes* 参数，抛出该异常。

![](https://img-blog.csdn.net/20160824102707495)

![](https://img-blog.csdn.net/20160824102714625)

![](https://img-blog.csdn.net/20160824102721370)

![](https://img-blog.csdn.net/20160824102728651)


# RegExp常用示例

```java
/* ---------------------------------RegExp  校验模式-----------------------------*/
//语法:RegExpObject.test(string)
//说明:test() 方法检索字符串中的指定值。返回值是 true 或 false
var re1 = new RegExp("^\\s+$","g");
var msg = re1.test("he llo");
alert(msg);
 
//语法:RegExpObject.exec(string)
//说明:exec() 方法检索字符串中的指定值。返回值是被找到的值。如果没有发现匹配，则返回 null
var a;
var re2 = new RegExp("e", "g");
do {
    a = re2.exec("hello world hi!");
    alert(a);
}
while (a != null);

//语法:stringObject.search(regexp)
//说明:search() 从字符串的开始进行检索，总是返回stringObject的第一个匹配的位置 

/* ---------------------------------String 校验模式-----------------------------*/

//校验是否全由数字组成
function isDigit(s)
{
var patrn=/^[0-9]{1,20}$/;
if (!patrn.exec(s)) return false
return true
}

//校验登录名：只能输入5-20个以字母开头、可带数字、“_”、“.”的字串
function isRegisterUserName(s)
{
var patrn=/^[a-zA-Z]{1}([a-zA-Z0-9]|[._]){4,19}$/;
if (!patrn.exec(s)) return false
return true
}

//校验用户姓名：只能输入1-30个以字母开头的字串
function isTrueName(s)
{
var patrn=/^[a-zA-Z]{1,30}$/;
if (!patrn.exec(s)) return false
return true
}

//校验密码：只能输入6-20个字母、数字、下划线
function isPasswd(s)
{
var patrn=/^(\w){6,20}$/;
if (!patrn.exec(s)) return false
return true
}

//校验普通电话、传真号码：可以“+”开头，除数字外，可含有“-”
function isTel(s)
{
//var patrn=/^[+]{0,1}(\d){1,3}[ ]?([-]?(\d){1,12})+$/;
var patrn=/^[+]{0,1}(\d){1,3}[ ]?([-]?((\d)|[ ]){1,12})+$/;
if (!patrn.exec(s)) return false
return true
}

//校验手机号码：必须以数字开头，除数字外，可含有“-”
function isMobil(s)
{
var patrn=/^[+]{0,1}(\d){1,3}[ ]?([-]?((\d)|[ ]){1,12})+$/;
if (!patrn.exec(s)) return false
return true
}

//校验邮政编码
function isPostalCode(s)
{
//var patrn=/^[a-zA-Z0-9]{3,12}$/;
var patrn=/^[a-zA-Z0-9 ]{3,12}$/;
if (!patrn.exec(s)) return false
return true
}

//校验搜索关键字
function isSearch(s)
{
var patrn=/^[^`~!@#$%^&*()+=|\\\][\]\{\}:;'\,.<>/?]{1}[^`~!@$%^&()+=|\\\][\]\{\}:;'\,.<>?]{0,19}$/;
if (!patrn.exec(s)) return false
return true
}
function isIP(s) //by zergling
{
var patrn=/^[0-9.]{1,20}$/;
if (!patrn.exec(s)) return false
return true
}

/*----------------------------------JQuery 正则表达式验证器-------------------------*/
// 使用方法-input标签验证类型，如： validType="PLAIN"
$.extend($.fn.validatebox.defaults.rules, {
    CHS: {
        validator: function (value, param) {
            return /^[\u0391-\uFFE5]+$/.test(value);
        },
        message: '请输入汉字'
    },
    PLAINLENGTH: {
        validator: function (value, param) {
            return /^[A-Za-z0-9\u4e00-\u9fa5]{0,20}$/.test(value);
        },
        message: '请输入20个以内的中英文以及数字'
    },
    GoodsLevel: {
        validator: function (value, param) {
            return /^0[0-6]{1}$/.test(value);
        },
        message: '请输入00到06之间的'
    },
    STANDLENGTH: {
        validator: function (value, param) {
            return /^[A-Za-z%\.0-9\u4e00-\u9fa5]{0,20}$/.test(value);
        },
        message: '请输入20个以内的中英文数字以及百分号'
    },
    DECIMAL: {
        validator: function (value, param) {
            return /^[123456789]\d{0,7}(\.\d{1,2})?$/.test(value);
        },
        message: '请输入正确的数据,只保留两位小数'
    },
    PERCENT: {
        validator: function (value, param) {
            return /^\d+\.?\%$/.test(value);
        },
        message: '请输入正确的比例(50%)'
    },
    SCORE: {
        validator: function (value, param) {
            return /^\d{2}:\d{2}$/.test(value);
        },
        message: '请输入正确的比例(00:00)'
    },
    ENFUN: {
        validator: function (value, param) {
            return /^[A-Za-z_\-\\(\\)]+$/.test(value);
        },
        message: '请输入正确的函数名 例:test()'
    },
   LETTER: {
        validator: function (value, param) {
            return /^[A-Za-z]+$/.test(value);
        },
        message: '请输入英文字母'
    },
     LOWER_LETTER: {
        validator: function (value, param) {
            return /^[a-z]+$/.test(value);
        },
        message: '请输入小写英文字母'
    },
    EN: {
        validator: function (value, param) {
            return /^[A-Za-z\-]+$/.test(value);
        },
        message: '请输入英文字母,可以包含(-)'
    },
    PLAIN: {
        validator: function (value, param) {
            return /^[A-Za-z0-9\u4e00-\u9fa5]+$/.test(value);
        },
        message: '请输入中英文以及数字'
    },
    IMAGE: {
        validator: function (value, param) {
            return /^\S+\.(jpg|gif|jpeg|png|bmp)$/.test(value);
        },
        message: '请输入正确的图片类型'
    },
    URL: {
        validator: function (value, param) {
            return /^\S+\.html$/.test(value);
        },
        message: '请输入正确的url地址'
    },
    LINK: {
        validator: function (value, param) {
            return /(?:https|http|ftp|rtsp|mms):\/\/.+/.test(value);
        },
        message: '请输入正确的url地址'
    },
    ZIP: {
        validator: function (value, param) {
            return /^[1-9]\d{5}$/.test(value);
        },
        message: '邮政编码不存在'
    },
    QQ: {
        validator: function (value, param) {
            return /^[1-9]\d{4,10}$/.test(value);
        },
        message: 'QQ号码不正确'
    },
    mobile: {
        validator: function (value, param) {
            return /^[1][3458][0-9]{9}$/.test(value);
        },
        message: '手机号码不正确'
    },
    notEmpty: {
        validator: function (value, param) {
            return /^.*$/.test(value);
        },
        message: '该项不能为空'
    },
    loginName: {
        validator: function (value, param) {
            //return /^[\u0391-\uFFE5\w]+$/.test(value);
            return /^\w{3,20}$/.test(value);
        },
        //message: '请输入中英文，数字以及下划线'
         message: '请输入3-20位数字、字母、下划线'
    },
    safepass: {
        validator: function (value, param) {
            return safePassword(value);
        },
        message: '密码由字母和数字组成，6-20位'
    },
    equalTo: {
        validator: function (value, param) {
            return value == $(param[0]).val();
        },
        message: '两次输入的密码不一致'
    },
    number: {
        validator: function (value, param) {
            return /^\d+$/.test(value);
        },
        message: '请输入数字'
    },
    idcard: {
        validator: function (value, param) {
            return idCard(value);
        },
        message:'请输入正确的身份证号码'
    },
    //座机
    tel: {
        validator: function (value, param) {
            var phone = /^0\d{2,3}-?\d{7,8}$/;
            return phone.test(value);
        },
        message:'请输入正确的电话号码'
    },
    selectValueRequired: {
        validator: function (value, param) {
            return $(param[0]).combobox('getValue') !='';
        },
        message: '请选择下拉数据'
    },
    //传真
    fax: {
        validator: function (value, param) {
            var phone = /^0\d{2,3}-?\d{7,8}$/;
            return phone.test(value);
        },
        message:'请输入正确的传真号码'
    },
    //电话验证（是否是手机或座机）
    mobileOrTel: {
        validator: function (value, param) {
            var mobile = /^[1][23456789][0-9]{9}$/ , phone =  /^(\d{3,4}-)?\d{7,8}$/; var freePhone = /^[48]00/;
            return mobile.test(value) || phone.test(value) || freePhone.test(value);
        },
        message:'请输入正确的电话号码'
    },
    positive:{
        validator: function (value, param) {
            var pos = /^[0-9]*[1-9][0-9]*$/;
            return pos.test(value);
        },
        message:'请输入正整数'
    },
    warehousecode:{
        validator: function (value, param) {
            return /^[A-Za-z0-9]{5}$/.test(value);
        },
        message:'请输入五位数字或英文'
    },
    UNLIMITDECIMAL: {
        validator: function (value, param) {
            return /^\d{1,10}(\.\d{1,10})?$/.test(value);
        },
        message: '请输入正确的数据,只保留两位小数'
    },
    ENONE:{
        validator: function (value, param) {
            return /^[A-Za-z]{1}$/.test(value);
        },
        message: '请输入一位英文字母'
    },
    LOCATIONCODE:{
        validator: function (value, param) {
            return /^[A-Za-z0-9\-]{9}$/.test(value);
        },
        message: '请输入9位库位编号'
    },
    LONGITUDE:{
        validator: function (value, param) {
            return value>=-180.000000&&value<=180.000000;
        },
        message: '请输入有效经度'
    },
    LATITUDE:{
        validator: function (value, param) {
            return value>=-90.000000&&value<=90.000000;
        },
        message: '请输入有效纬度'
    }
});
```





