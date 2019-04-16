# JQuery Kendo UI使用技巧总结 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年08月30日 23:57:46[gloomyfish](https://me.csdn.net/jia20003)阅读数：26981








**Kendo UI开发总结 By Gloomyfish on 2013-04-25**

**在Grid中支持分页刷新:**

scrollable: {virtual : **true**},

**在Grid的DataSource中添加分页支持：**

serverPaging: **true**,

serverSorting: **true**,

serverFiltering: **true**,

pageSize: 50,



在grid中显示列(显示/隐藏菜单)与过滤支持菜单

filterable: **true**,

columnMenu: **true**,



在Grid中隐藏某个指定的列，需要在columns中指定column下面添加:

hidden:**true**,



使用模板来格式化显示grid中的列数据，给对应列添加模板的代码如下:

         template : "#=(value==-1) ? '-' :formatValue(value)#"



对齐显示grid中列文本的代码如下：

attributes:{ style: 
"text-align: right"}



绑定一个Kendo datasource到kendo.observable对象：

**var** formVM = kendo.observable({

     sources: formDS,

     source: **null**

});



绑定指定的formVM到某个Div元素：

kendo.bind($("#form_div"),formVM);



在页面数据改变时，更新绑定的数据源(DataSource):

formDS.bind("change",
**function**() {

//binds the view-model to the first entryin the
datasource

   formVM.set("source",
**this**.view()[0]);

});



强制更新form_div中的值：

formVM.set("source.userName",
'gloomyfish');对应的HTML为

<inputid=*"uname"*name=*"uname"*data-bind=*"value: source.userName"*/>



启动一个新的浏览器新窗口代码如下：

**var** left = (screen.width/2)-(800/2);

**var** top = 0;//(screen.height/2)-(800/2);

// force to open new widows in
chrome,IE, FF

window.open("personInfo.html","_blank","resizable=yes, scrollbars=yes,titlebar=yes, width=800, height=800, top="+
 top +", left="+ left);



下载grid中选中的所有文件：

**var** grid = $("#file_grid").data("kendoGrid");

**var** tr = grid.select();
//gets selected <tr> element

**if**(tr.length >= 1) {       


**for**(**var** i=0; i<tr.length;i++) {

**var** item =grid.dataItem(tr[i]);
//gets the dataSourceitem

**var** fid = item.id;

            makeFrame("file/"+ fid +"/fileContent.htm");


        }

}

**function** makeFrame( url )

    {

**var** ifrm = document.createElement(
"IFRAME"); 

        ifrm.setAttribute( 
"style", "display:none;") ;

        ifrm.setAttribute( 
"src", url ) ; 

        ifrm.style.width = 0+"px";


        ifrm.style.height = 0+"px";


        document.body.appendChild( ifrm ) ;


    }



通过window.location或者window.location.href打开指定URL



同步Grid数据更新，使用如下代码：

grid.dataSource.sync() 

或者

myGrid.dataSource.read();

myGrid.refresh();



一个Ajax请求跳转页面的例子：

   $.ajax({

        url : 
"openMyInfo",

        type : 
"POST",

        data : {userName: name, userId:id },        

        success : **function**(jqXHR, textStatus) {

**if** (jqXHR.jsonResponse.success) {              


**var** url = jqXHR.jsonResponse.message;

                window.location=url;           


            } **else** {

               alert(jqXHR.jsonResponse.message);

            }

        },

        error : **function**(jqXHR, textStatus, errorThrown) {

            alert (errorThrown);

        }

});



Kendo UI dropdown list级联菜单刷新：

在父dropdown list上绑定change事件函数：change : onItemChange

在函数中刷新更新子dropdow list的数据源(data source)

**var** subDDList = $('#subListDiv').data("kendoDropDownList");

subDDList.setDataSource(buildSubList(selectParentId));



上传文件对话框使用

   $("#selectedFiles").kendoUpload({

        async: {

            saveUrl: 
"myDomain/fileUpload.htm",

            autoUpload: **true**

        },

        multiple:**true**, //
支持多个文件上传,

        complete: uploadFileComplete, //上传结束以后处理,

        error: onError,

        cancel: onCancel,

        select: onSelect,

        success: onSuccess

});

调用代码 $("# selectedFiles ").click();//模拟鼠标双击事件调用,

页面上selectedFileshtml元素为隐藏对象。



禁用IE缓存，在JQuery1.2以上版本支持：

$.ajax({ 

    type:"GET",

    url:"static/cache.js",


    dataType:"text",

    cache:**false**,
// disable cache(禁用IE缓存)

    ifModified:**true**

});



在HTML文件中加上：

<metahttp-equiv=*"Expires"*content=*"0"*/>

<metahttp-equiv=*"Cache-Control"*content=*"no-cache"*/>

<metahttp-equiv=*"Pragma"*content=*"no-cache"*/>



**提交数据之后打开在新窗口：**

<formaction=*"getMyInfo.htm"*method=*"post"*target=*"_blank"*>



获取Servlet的真是路径：

request.getSession().getServletContext().getRealPath("/");



实现路径重定向：

((HttpServletResponse)response).sendRedirect(redirectURL);



JS中替换反斜线正则表达式：

**var**myString = content.replace(/\//g, 
"\\\\");

或者：**var** value = value.replace(/\\/g,
"whatever");



JQuery中报UncaughtSyntaxError: Unexpected identifier

原因多数是你在前一行少加了分号，或者使用了不恰当的关键字标识，比如

setTimeout(**newfunction**(){alert(“Helloworld”);}, 200);

其中new是多余关键字，导致错误。



隐藏页面上DIV内容：

$(my_div_id).css("display","none");   


显示它：

$(my_div_id).css("display","");



纯JavaScript方式实现，永远有效的隐藏一个DIV内容的方法：

document.getElementById('myDivID').style.display =
'none';

显示它：

document.getElementById('myDivID').style.display =
'';





