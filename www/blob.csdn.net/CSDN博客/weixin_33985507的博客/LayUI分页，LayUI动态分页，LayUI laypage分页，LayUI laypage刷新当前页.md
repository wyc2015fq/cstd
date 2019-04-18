# LayUI分页，LayUI动态分页，LayUI laypage分页，LayUI laypage刷新当前页 - weixin_33985507的博客 - CSDN博客
2017年08月01日 17:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：196
**LayUI分页，LayUI动态分页，LayUI laypage分页，LayUI ****laypage****刷新当前页**
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
©Copyright 蕃薯耀 2017年8月1日
http://www.cnblogs.com/fanshuyao/
效果图：
![](https://images2017.cnblogs.com/blog/1081330/201708/1081330-20170801175701677-638412650.png)
**一、引用js依赖**
主要是jquery-1.11.3.min.js 和 layui.all.js ， json2.js用来做json对象转换的
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- <script type="text/javascript" src="${pageContext.request.contextPath}/js/jquery-1.11.3.min.js"></script>  
- <script type="text/javascript" src="${pageContext.request.contextPath}/plugin/layui/lay/dest/layui.all.js"></script>  
- <script type="text/javascript" src="${pageContext.request.contextPath}/js/json2.js"></script>  
**二、js分页方法封装（分页使用模板laytpl）**
1、模板渲染
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- /** 
-  * 分页模板的渲染方法 
-  * @param templateId 分页需要渲染的模板的id 
-  * @param resultContentId 模板渲染后显示在页面的内容的容器id 
-  * @param data 服务器返回的json对象 
-  */  
- function renderTemplate(templateId, resultContentId, data){  
-     layui.use(['form','laytpl'], function(){  
-         var laytpl = layui.laytpl;  
-         laytpl($("#"+templateId).html()).render(data, function(html){  
-             $("#"+resultContentId).html(html);  
-         });  
-     });  
-     layui.form().render();// 渲染  
- };  
2、layui.laypage 分页封装
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- /** 
-  * layui.laypage 分页封装 
-  * @param laypageDivId 分页控件Div层的id 
-  * @param pageParams 分页的参数 
-  * @param templateId 分页需要渲染的模板的id 
-  * @param resultContentId 模板渲染后显示在页面的内容的容器id 
-  * @param url 向服务器请求分页的url链接地址 
-  */  
- function renderPageData(laypageDivId, pageParams, templateId, resultContentId, url){  
- if(isNull(pageParams)){  
-         pageParams = {  
-             pageIndex : 1,  
-             pageSize : 10  
-         }  
-     }  
-     $.ajax({  
-         url : url,//basePath + '/sysMenu/pageSysMenu',  
-         method : 'post',  
-         data : pageParams,//JSON.stringify(datasub)  
-         async : true,  
-         complete : function (XHR, TS){},  
-         error : function(XMLHttpRequest, textStatus, errorThrown) {  
- if("error"==textStatus){  
-                 error("服务器未响应，请稍候再试");  
-             }else{  
-                 error("操作失败，textStatus="+textStatus);  
-             }  
-         },  
-         success : function(data) {  
-             var jsonObj;  
- if('object' == typeof data){  
-                 jsonObj = data;  
-             }else{  
-                 jsonObj = JSON.parse(data);  
-             }  
-             renderTemplate(templateId, resultContentId, jsonObj);  
- 
- //重新初始化分页插件  
-             layui.use(['form','laypage'], function(){  
-                 laypage = layui.laypage;  
-                 laypage({  
-                     cont : laypageDivId,  
-                     curr : jsonObj.pager.pageIndex,  
-                     pages : jsonObj.pager.totalPage,  
-                     skip : true,  
-                     jump: function(obj, first){//obj是一个object类型。包括了分页的所有配置信息。first一个Boolean类，检测页面是否初始加载。非常有用，可避免无限刷新。  
-                         pageParams.pageIndex = obj.curr;  
-                         pageParams.pageSize = jsonObj.pager.pageSize;  
- if(!first){  
-                             renderPageData(laypageDivId, pageParams, templateId, resultContentId, url);  
-                         }  
-                     }  
-                 });  
-             });  
-         }  
-     });  
- };  
3、刷新当前分页的方法，可省略
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- /** 
-  * 分页插件刷新当前页的数据，必须有跳转的确定按钮，因为根据按钮点击事件刷新 
-  */  
- function reloadCurrentPage(){  
-     $(".layui-laypage-btn").click();  
- };  
**三、页面代码**
1、分页表格及分页控件
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- <!-- 分页表格 -->  
- <div class="layui-form">  
-   <table class="layui-table">  
-     <thead>  
-       <tr>  
-         <th class="w20"><input type="checkbox" name="checkBoxAll" lay-skin="primary" lay-filter="allChoose"></th>  
-      <th class="w200">许可名称</th>  
-      <th class="w200">许可编码</th>  
-      <th class="w200">菜单名称</th>  
-      <th>许可链接</th>  
-     </tr>   
-      </thead>  
-       <tbody id="page_template_body_id">  
-     </tbody>  
-   </table>  
- </div>  
- <!-- 分页控件div -->        
- <div id="imovie-page-div"></div>  
2、分页模板
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- <script id="page_template_id" type="text/html">  
- {{#  layui.each(d.list, function(index, item){ }}  
- <tr>  
-     <td><input type="checkbox" name="permissionId" lay-skin="primary" value="{{item.permissionId}}"></td>  
-     <td>{{item.permissionName || ''}}</td>  
-     <td>{{item.permissionCode || ''}}</td>  
-     <td>{{item.menuName || ''}}</td>  
-     <td>{{item.permissionUrl || ''}}</td>  
- </tr>  
- {{#  }); }}  
- </script>  
3、分页执行代码：
分页参数：
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- function getPageParams(){  
-     var pageParams = {  
-     pageIndex : 1,  
-     pageSize : 2  
-     };  
-     pageParams.permissionName = $("input[name='permissionName']").val();  
-     pageParams.permissionCode = $("input[name='permissionCode']").val();  
-     pageParams.menuName = $("input[name='menuName']").val();  
- return pageParams;  
- };  
分页执行方法：
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- function initPage(){  
-     renderPageData("imovie-page-div", getPageParams(), "page_template_id",   
- "page_template_body_id", basePath + '/sysPermission/pageSysPermission');  
- };  
页面加载初始化分页：
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- $(function(){  
-     initPage();  
- });  
如果包括上面效果图的查询，如下：
Html页面代码
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- <div>  
-             <form class="layui-form layui-form-pane">  
-                 <div class="layui-form-item">  
-                     <div class="layui-inline">  
-                         <label class="layui-form-label">许可名称</label>  
-                         <div class="layui-input-inline">  
-                             <input type="text" name="permissionName"   
-                                 autocomplete="off" class="layui-input" placeholder="请输入许可名称" >  
-                         </div>  
-                     </div>  
-                     <div class="layui-inline">  
-                         <label class="layui-form-label">许可编码</label>  
-                         <div class="layui-input-inline">  
-                             <input type="text" name="permissionCode"   
-                                 autocomplete="off" placeholder="请输入许可编码" class="layui-input">  
-                         </div>  
-                     </div>  
-                     <div class="layui-inline">  
-                         <label class="layui-form-label">菜单名称</label>  
-                         <div class="layui-input-inline layui-input-inline-0">  
-                             <input type="text" name="menuName"   
-                                 autocomplete="off" placeholder="请选择菜单名称" class="layui-input">  
- 
-                         </div>  
-                     </div>  
-                     <div class="layui-inline">  
-                         <button id="btnSubmit" class="layui-btn" lay-submit="" lay-filter="formFilter">查询</button>  
-                     </div>  
-                 </div>  
-             </form>  
-         </div>  
查询语句：
Java代码  ![收藏代码](http://fanshuyao.iteye.com/images/icon_star.png)
- $(function(){  
-     initPage();  
- 
-     layui.use(['form'], function(){  
-         var form = layui.form();  
- //监听提交  
-         form.on('submit(formFilter)', function(data){  
-             initPage();  
- return false;  
-         });  
- 
- 
-     });  
- });  
**四、懂 jquery 插件封装的大神可以将其封装成独立的分页插件，这样更加容易使用。我表示不太懂，^_^**
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
©Copyright 蕃薯耀 2017年8月1日
http://www.cnblogs.com/fanshuyao/
