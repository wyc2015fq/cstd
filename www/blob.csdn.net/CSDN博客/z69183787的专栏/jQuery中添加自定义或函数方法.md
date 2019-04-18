# jQuery中添加自定义或函数方法 - z69183787的专栏 - CSDN博客
2014年01月26日 21:34:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1046
方法一：
   jQuery.fn.setApDiv=function () {
        //apDiv浮动层显示位置居中控制
        var wheight=$(window).height();
        var wwidth=$(window).width();
        var apHeight=wheight-$("#apDiv").height();
        var apWidth=wwidth-$("#apDiv").width();
        $("#apDiv").css("top",apHeight/2);
        $("#apDiv").css("left",apWidth/2);
    }
调用方法：$("#apDiv").setApDiv();
--------------------------------------------------------------------------------
方法二：
      //jQuery 应用扩展
      jQuery.extend({
              // 设置 apDiv
            setApDiv:function () {
            //apDiv浮动层显示位置居中控制
            var wheight=$(window).height();
            var wwidth=$(window).width();
            var apHeight=wheight-$("#apDiv").height();
            var apWidth=wwidth-$("#apDiv").width();
            $("#apDiv").css("top",apHeight/2);
            $("#apDiv").css("left",apWidth/2);
            }
      });
调用方法：$.setApDiv();
总结 一种如$.extend({'aa':function(){}}),这种调用时就是这样$.aa(),另一种如$.fn.extend({'aa':function(){}}),这种调用时就得这样,$(this).aa()
--------------------------------------------------------------------------------
方法三：
     $.postJSON = function(url, data, callback) {
  $.post(url, data, callback, "json");
 };
调用方法：$.postJSON('/post/getsecurejsonpost',{}, function(data) {});
