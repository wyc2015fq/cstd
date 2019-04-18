# jQuery插件实战之fullcalendar（日历插件）Demo - z69183787的专栏 - CSDN博客
2014年09月16日 19:51:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2338
jQuery的插件非常多，应用的场景也非常丰富，今天我这里给大家介绍一款非常实用的日历页面开发插件 - **fullcalendar**，目前最新版本是1.5.1，使用这款插件能够快速帮助你快速编程实现基于web的日历查看功能，大家可能都使用过outlook的日历项功能，使用日历界面能更方便的查看待办事项或者约会。开发过程快速方便。插件首页的文档也非常全。当然目前只有英文文档。不错插件支持多语言，这个很不错。
在过去web程序开发中，我曾经使用这个插件开发了公司内部的会议室预定系统，这里我简单的介绍一下**fullcalendar**的相关开发，并且实例讲解一下如何使用这个插件开发日历，其中会使用到**fullcalendar**里相关的高级功能，例如，拖拽修改时间，生成个性化的会议室预定系统日历项内容。以上代码都是实际项目中使用的前台，大家如果需要可以直接修改使用。
![](http://hi.csdn.net/attachment/201110/11/0_1318330718p6C6.gif)
### 项目需求：
1. 需要在**fullcalendar**的原有界面上美化，这里我们使用jQueryUI来实现界面美化，因为**fullcalendar**可以很好的和jQueryUI无缝整合
2. 生成单独的日历项添加和查看功能，这里仍旧使用jQueryUI的dialog来实现
3. 因为是一个实际项目，需要数据验证，所以这里我们使用formVaildator插件实现
4. 添加一个 "转到某日"功能， 这个界面功能在**Fullcalendar**里没有，我们可以通过编程添加类似一个功能，其中调用了datepicker的一个addon，后面会介绍到
5. 周和日浏览可以自由的支持拖拽和移动，用来修改日期和时间
6. 浏览器支持： IE8和Firefox
### jQuery相关插件：
1. [**fullcalendar**](http://www.google.com.hk/url?sa=t&source=web&cd=1&ved=0CCEQFjAA&url=http%3A%2F%2Farshaw.com%2Ffullcalendar%2F&rct=j&q=fullcalendar&ei=vAEDToK7PIH-vQO_lfHvDQ&usg=AFQjCNFn7CjTrWq-63B_b7V1Bf7IsYgSBQ&cad=rja)
提供Calendar功能
2. [**formValidator**](http://www.position-absolute.com/articles/jquery-form-validator-because-form-validation-is-a-mess/)
提供输入验证功能
3. **[Timepicker Addon for jQuery UI Datepicker](http://blog.csdn.net/gulijiang2008/article/details/Timepicker%20Addon%20for%20jQuery%20UI%20Datepicker)**
提供datepicker时间选择功能
### 开发代码：
导入相关jQuery插件类库，如下：
**[html]**[view
 plain](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[copy](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[print](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[?](http://blog.csdn.net/gulijiang2008/article/details/6863755#)
- <scripttype="text/javascript"src="js/jquery-ui-1.8.6.custom.min.js"></script>
- 
- <scripttype="text/javascript"src="js/jquery-ui-timepicker-addon.js"></script>
- 
- <linkrel="stylesheet"href="css/redmond/jquery-ui-1.8.1.custom.css">
- 
- <!-- Jquery and Jquery UI -->
- 
- <scriptsrc="js/formValidator/js/jquery.validationEngine.js"type="text/javascript"></script>
- 
- <scriptsrc="js/formValidator/js/jquery.validationEngine-en.js"type="text/javascript"></script>
- 
- <linkrel="stylesheet"href="js/formValidator/css/validationEngine.jquery.css"type="text/css"media="screen"charset="utf-8"/>
- 
- <!-- FullCalender -->
- 
- <linkrel='stylesheet'type='text/css'href="js/fullcal/css/fullcalendar.css"/>
- <scripttype='text/javascript'src="js/fullcal/fullcalendar.js"></script>
生成日历主界面：
**[html]**[view
 plain](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[copy](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[print](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[?](http://blog.csdn.net/gulijiang2008/article/details/6863755#)
- $('#calendar').fullCalendar({  
-          header:{  
-            right: 'prev,next today',  
-            center: 'title',  
-            left: 'month,agendaWeek,agendaDay'  
-          },  
-          theme: true,  
-          editable: true,  
-          allDaySlot : false,  
-          events:  function(start, end , callback){  
-          var events = [];  
-  ...            
-          callback(events);  
-          },  
-  ...  
以上代码将在id=calendar的div里生成一个日历，其中theme定义使用jQueryUI来生成界面，events主要定义生成的会议室预定系统日历项目，这里我们使用如下代码生成日历项，在实际开发过程中，我们可以在这里调用后台程序，或者使用其它方法生成数据，这里为了简单演示，我们使用js来生成需要的日历项目，代码如下：
**[html]**[view
 plain](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[copy](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[print](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[?](http://blog.csdn.net/gulijiang2008/article/details/6863755#)
- var now = new Date();  
-         for(var i=-10;i<60;i++){  
-           var evtstart = new Date(now.getFullYear() , now.getMonth() , (now.getDate()-i), now.getHours()-5, now.getMinutes(), now.getSeconds(), now.getMilliseconds());  
- 
-           var evtend = new Date(now.getFullYear() , now.getMonth() , (now.getDate()-i), now.getHours(), now.getMinutes(), now.getSeconds(), now.getMilliseconds());               
- 
-           events.push({  
-             sid: 1,  
-             uid: 1,  
-             title: 'Daily Scrum meeting',  
-             start: evtstart,  
-             end: evtend,  
-             fullname: 'terry li',  
-             confname: 'Meeting 1',  
-             confshortname: 'M1',  
-             confcolor: '#ff3f3f',  
-             confid: 'test1',  
-             allDay: false,  
-             topic: 'Daily Scrum meeting',  
-             description : 'Daily Scrum meeting',  
-             id: 1,  
-           });         
-         }          
以上代码将生成一些日历项目，显示在日历中。
**[html]**[view
 plain](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[copy](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[print](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[?](http://blog.csdn.net/gulijiang2008/article/details/6863755#)
- $('#calendar').fullCalendar({    
-  ...  
-  ...   
-  dayClick: function(date, allDay, jsEvent, view) { // 定义了点击日历中日期格子的动作，这里将会调用jQueryUi的dialog生成创建新日历项的对话框  
-  ...  
- },  
- eventClick: function(event) {  // 定义了点击日历项的动作，这里将会调用jQueryUi的dialog显示日历项的内容  
- },  
-  ...   
接下来是fullcalendar的几个方法，用来设置会议室预定系统日历项的显示，分别是eventRender, eventAfterRender,这里几个方法可以用来生成日历项的内容，具体如下：
**[html]**[view
 plain](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[copy](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[print](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[?](http://blog.csdn.net/gulijiang2008/article/details/6863755#)
- eventRender: function(event, element) {  
-           var fstart  = $.fullCalendar.formatDate(event.start, "HH:mm");  
-         var fend  = $.fullCalendar.formatDate(event.end, "HH:mm");    
-         // Bug in IE8  
-         //element.html('<ahref=#>' + fstart + "-" +  fend + '<divstyle=color:#E5E5E5>' +  event.title + "</div></a>");  
-         },  
-         eventAfterRender : function(event, element, view) {  
-           var fstart  = $.fullCalendar.formatDate(event.start, "HH:mm");  
-         var fend  = $.fullCalendar.formatDate(event.end, "HH:mm");      
-         //element.html('<ahref=#><div>Time: ' + fstart + "-" +  fend + '</div><div>Room:' + event.confname + '</div><divstyle=color:#E5E5E5>Host:' +  event.fullname + "</div></a>");  
-         var confbg='';  
-         if(event.confid==1){  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }else if(event.confid==2){  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }else if(event.confid==3){  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }else if(event.confid==4){  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }else if(event.confid==5){  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }else if(event.confid==6){  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }else{  
- confbg = confbg + '<spanclass="fc-event-bg"></span>';  
-         }  
-         var titlebg =  '<span class="fc-event-conf" style="background:'+  event.confcolor +'">' + event.confshortname + '</span>';  
-         if(event.repweeks>0){  
- titlebg = titlebg + '<spanclass="fc-event-conf"style="background:#fff;top:0;right:15;color:#3974BC;font-weight:bold">R</span>';  
-         }  
-         if(view.name=="month"){  
-           var evtcontent = '<div class="fc-event-vert"><a>';  
- evtcontent = evtcontent + confbg;  
- evtcontent = evtcontent + '<spanclass="fc-event-titlebg">' +  fstart + " - " +  fend + titlebg + '</span>';  
- evtcontent = evtcontent + '<span>Room: ' +  event.confname + '</span>';  
- evtcontent = evtcontent + '<span>Host: ' +  event.fullname + '</span>';  
- evtcontent = evtcontent + '</a><divclass="ui-resizable-handle ui-resizable-e"></div></div>';  
-           element.html(evtcontent);  
-         }else if(view.name=="agendaWeek"){  
-           var evtcontent = '<a>';  
- evtcontent = evtcontent + confbg;  
- evtcontent = evtcontent + '<spanclass="fc-event-time">' +  fstart + "-" +  fend + titlebg + '</span>';  
- evtcontent = evtcontent + '<span>' +  event.confname + ' by ' + event.fullname + '</span>';  
-           //evtcontent = evtcontent + '<span>' +  event.fullname + '</span>';  
- evtcontent = evtcontent + '</a><spanclass="ui-icon ui-icon-arrowthick-2-n-s"><divclass="ui-resizable-handle ui-resizable-s"></div></span>';  
-           element.html(evtcontent);             
-         }else if(view.name=="agendaDay"){  
-           var evtcontent = '<a>';  
- evtcontent = evtcontent + confbg;  
- evtcontent = evtcontent + '<spanclass="fc-event-time">' +  fstart + " - " +  fend + titlebg + '</span>';  
- evtcontent = evtcontent + '<span>Room: ' +  event.confname + '</span>';  
- evtcontent = evtcontent + '<span>Host: ' +  event.fullname + '</span>';  
- evtcontent = evtcontent + '<span>Topic: ' +  event.topic + '</span>';  
- evtcontent = evtcontent + '</a><spanclass="ui-icon ui-icon-arrow-2-n-s"><divclass="ui-resizable-handle ui-resizable-s"></div></span>';  
-           element.html(evtcontent);                 
-         }  
-         },   
以上定义了相关会议室预定系统日历项显示方式， 下面介绍会议室预定系统日历项拖动和调整大小，代码片段如下：
**[html]**[view
 plain](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[copy](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[print](http://blog.csdn.net/gulijiang2008/article/details/6863755#)[?](http://blog.csdn.net/gulijiang2008/article/details/6863755#)
- eventDragStart: function( event, jsEvent, ui, view ) {  
-         ui.helper.draggable("option", "revert", true);  
-         },  
-         eventDragStop: function( event, jsEvent, ui, view ) {  
-         },  
-         eventDrop: function( event, dayDelta, minuteDelta, allDay, revertFunc, jsEvent, ui, view ) {   
-         if(1==1||2==event.uid){  
-           var schdata = {startdate:event.start, enddate:event.end, confid:event.confid, sid:event.sid};  
-           identityService.getToBeUpdatedConflictedScheduleDAO(schdata , {  
-             callback:function(data) {   
-               if(data.length== 0){                  
-                 var newschdata = {sid:event.sid, startdate:event.start, enddate:event.end};               
-                 identityService.updateScheduleByNewDateRange(newschdata, {  
-                   callback:function(data) {     
-                     alert("Thanks, reservation rescheduled successfully.");  
-                     //window.location.reload();  
-                   }                           
-                 });                         
-               }else{  
-                 revertFunc();  
-                 alert("Sorry, reservation cannot be rescheduled.");  
-               }               
-             }  
-           });  
-         }else{  
-           revertFunc();  
-         }  
-         },  
-         eventResizeStart:  function( event, jsEvent, ui, view ) {  
-         },  
-         eventResize: function(event,dayDelta,minuteDelta,revertFunc) {  
-         if(1==1||2==event.uid){  
-           var schdata = {startdate:event.start, enddate:event.end, confid:event.confid, sid:event.sid};  
-         }else{  
-           revertFunc();  
-         }  
-         }  
- 
-       });  
以上代码如果需要详细说明，请大家参考官方文档，代码细节这里不再说明。 大家可以使用下面的演示代码运行一下。Fullcalendar是一个非常实用的日历插件，使用得当可以开发非常复杂的功能。例如，我们这里开发的会议室预定系统。强烈推荐！
