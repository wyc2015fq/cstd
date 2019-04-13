
# 编写一个简单的 jQuery 插件（模板） - 追求卓越,做到专业 - CSDN博客


2012年10月22日 16:16:31[Waldenz](https://me.csdn.net/enter89)阅读数：884标签：[jquery																](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)[function																](https://so.csdn.net/so/search/s.do?q=function&t=blog)[conflict																](https://so.csdn.net/so/search/s.do?q=conflict&t=blog)[javascript																](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=conflict&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=function&t=blog)个人分类：[JS/jQuery																](https://blog.csdn.net/enter89/article/category/1257949)
[
																								](https://so.csdn.net/so/search/s.do?q=function&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)

//You need an anonymous function to wrap around your function to avoid conflict
(function($){
    //Attach this new method to jQuery
    $.fn.extend({
        //This is where you write your plugin's name
        pluginname: function() {
            //options
            var defaults = {
                option1: "default_value"
            }
            var options = $.extend(defaults, options);
            //a public method
            this.methodName: function () {
                //call this method via $.pluginname().methodName();
            }
            //Iterate over the current set of matched elements
            return this.each(function() {
                var o = options;
                //code to be inserted here
            });
        }
    });
//pass jQuery to the function,
//So that we will able to use any valid Javascript variable name
//to replace "$" SIGN. But, we'll stick to $ (I like dollar sign: ) )      
})(jQuery);

