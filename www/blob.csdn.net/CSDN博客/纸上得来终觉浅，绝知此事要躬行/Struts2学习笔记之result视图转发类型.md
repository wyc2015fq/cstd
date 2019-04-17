# Struts2学习笔记之result视图转发类型 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月18日 23:22:14[boonya](https://me.csdn.net/boonya)阅读数：1516标签：[struts																[action																[class																[redirect																[jsp](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记


第四记：result视图转发类型

（1）、普通的页面转发

            <action name="example"  class="com.marker.controller.ExampleAction">

                      <result  name="index">/index.jsp</result>


                       <result  name="list">/WEB-INF/pages/list.jsp</result>

                      <result  name="success">/WEB-INF/pages/success.jsp</result>


           </action>


（2）、redirect重定向

         重定向到JSP页面：


          <action name="example"  class="com.marker.controller.ExampleAction">

                      <result  name="index" type="redirect">/help/index.jsp</result>


           </action>


           注：重定向时，在WEB-INF下的文件不能够重定向找到。


（3）、redirect重定向传递参数

        3-1：直接用类似EL表达式传递参数

           <action name="example"  class="com.marker.controller.ExampleAction">

                      <result  name="index" type="redirect">/help/index.jsp？name=${name}</result>


           </action>

        3-2:处理中文参数

           如果不转码：在jsp页面获取的方式为:${param.name},中文显示为乱码。


          编码： String name=URLEncoder("中华帝国"，“UTF-8”);

          解码：<%= new String(request.getParameter("name").getBytes("ISO8859-1","UTF-8"),"UTF-8") %>


（4）、redirectAction重定向

             2-2:重定向到Action：

          <action name="example0"  class="com.marker.controller.ExampleFAction">

                      <result  name="index" type="redirect">/help/index.jsp</result>

                      <result >/default.jsp</result>


           </action>

            <action name="example"  class="com.marker.controller.ExampleAction">

                      <result   type="redirectAction">

                            <param name="actionName">example0</param>

                            <param name="namespace">/</param>


                      </result>


            </action>

（5）、plainText视图代码输出

           <action name="example"  class="com.marker.controller.ExampleAction">

                      <result   type="plainText">

                            <param name="location">/test/home.jsp</param> <-!指定视图路径  -->


                            <param name="charSet">UTF-8</param>    //   <-!解决输出页面中文乱码问题  -->


                      </result>


            </action>

（6）、在某个包下创建一个公共视图

        6-1:同一个包下访问


          <package name="hers" namespace="/hers/">

                    <globle-results>

                              <result name="message">/messages.jsp</result>


                    <globle-results>

         </package>

        访问路径：/hers/message.action


        6-2：不同包下的访问，全局视图

             <package name="his" namespace="/his/"  extends="hers">

                      <action name="example"  class="com.marker.controller.ExampleAction">

                                  <result  name="index" >/index.jsp</result>


                      </action>

            </package>  


       访问路径：/his/message.action](https://so.csdn.net/so/search/s.do?q=redirect&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




