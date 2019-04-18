# qq网站里动态加载脚本的实现 - weixin_33985507的博客 - CSDN博客
2011年12月11日 16:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
function JsLoad(sUrl ,options){
    options = options || {};
    if(!options.sId) options.sId = 'jsload_'+Math.random();
    if(!options.varname) options.varname = 'datav4';
    var sId = 'jsload_'+options.sId;
    if(AppTui.$(sId)!='undefined'&&AppTui.$(sId)!=null){
        AppTui.$(sId).parentNode.removeChild(AppTui.$(sId));
    }
    var _script = document.createElement("script");
    _script.setAttribute("id", sId);
    _script.setAttribute("type", "text/javascript");
    _script.setAttribute("src", sUrl);
    document.getElementsByTagName("head")[0].appendChild(_script);
    if (!!document.all) {// IE
        _script.onreadystatechange = function(){
            var responseText = null;
            if (this.readyState == "loaded" || this.readyState == "complete") {
                if(options.completeListener) {
                    try{
                        responseText = eval('('+options.varname+')');
                        if(typeof responseText != null){
                            options.completeListener(responseText);
                        }else{
                            if(options.errorListener) {
                                options.errorListener();
                            }
                        }
                    }
                    catch(e)
                    {
                        if(options.errorListener) 
                        {
                            options.errorListener();
                        }
                    }
                }
                _data=null;
                if(AppTui.$(sId)!='undefined'&&AppTui.$(sId)!=null){
                    AppTui.$(sId).parentNode.removeChild(AppTui.$(sId));
                }
            }
        };
    }
    else {// FF
        _script.onload = function(){
            var responseText = null;
            if(options.completeListener) {
                try{
                    responseText = eval('('+options.varname+')'); 
                    if(typeof responseText!=null){
                        options.completeListener(responseText);
                    }else{
                        if(options.errorListener) {
                            options.errorListener();
                        }    
                    }
                }
                catch(e)
                {
                  if(options.errorListener) {
                        options.errorListener();
                    }
                }
            }
            _data=null;
            //alert( sId );
            if(AppTui.$(sId)!='undefined'&&AppTui.$(sId)!=null){
                AppTui.$(sId).parentNode.removeChild(AppTui.$(sId));
            }
        };
    }
};
其中AppTui的定义如下：
if (typeof AppTui == "undefined")
{
    var AppTui = new Object();
}
AppTui.$ = function(s)
{
    return (typeof s == "object") ? s: document.getElementById(s);
};
