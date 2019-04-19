# Selenium基本使用(十二)测试报告生成 - _天枢 - 博客园
## [Selenium基本使用(十二)测试报告生成](https://www.cnblogs.com/yhleng/p/7568303.html)
2017-09-21 14:06 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7568303)
**报告目录结构**
**![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170921135730056-388815505.png)**
Reporter根目录
      ｜
      ｜－－－－－－－－－－－－－LOGS 
      ｜                                                ｜
      ｜                                         2017－09－21_12-34-39   每次按日期时间动态生成报告目录 
      ｜                                                ｜
      ｜                                             Image   用于存储每个功能点，验证截图，这个会显示在报告上    
      ｜
      ｜－－－ LOG.XSLT  #报告样式及格式
**1、使用xslt，来设定样式**
```
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
 <html>
 <Head>
        <style type="text/css">
          body {
          background:#fff;
          margin:0;
          padding:40px 20px;
          font-family: "Arial", Arial, Sans-serif;
          font-size: 16px;
          color:#000;
          }
          table {
          margin:5px 5px 0;
          border:0px solid #222;
          font-size: 0.8em;
          }
          td {
          margin:5px 5px 0;
          padding:10px 10px 10px 10px;
          vertical-align:text-top;
          border:1px solid #222;
          border-width:1px 1px 1px 1px;
          }
          td.light {
          border:0px solid #222;
          }
          td.number {
          text-align:right;
          }
          td.status {
          text-align:right;
          vertical-align:text-bottom;
          }
        </style>
 </Head>
 <body>
   <!--OVER RESULT -->
   <xsl:variable name="OVER_STATUS" select="REPORT/OVER_STATUS"/>
   <xsl:variable name="STATUS" select="REPORT/LOG_ENTRY/STATUS"/>
   <h2>自动化测试执行报告</h2>
   <!--table -->
   <table border="1">
     <!--all result value-->
     <tr bgcolor="white" height = "35">
        <td>Overall Test Result</td>
        <td><xsl:value-of select="$OVER_STATUS"/></td>
        <td colspan="8"> </td>
     </tr>
     <tr bgcolor="#D8BFD9" height = "35">
       <th>Step</th>
       <th>Execute Time</th>
       <th>Step Result</th>
       <th>Description</th>
       <th>Step Description</th>
       <th>Action</th>
       <th>Index</th>
       <th>Element</th>
       <th>Value</th>
       <th>Expected Results </th>
     </tr>
       <!--select font color -->
       <xsl:variable name="fontColor">
           <xsl:choose>
               <xsl:when test="$STATUS = FAILED or $STATUS = PASSED">
                   <xsl:text>white</xsl:text>
               </xsl:when>
               <xsl:otherwise>black</xsl:otherwise>
           </xsl:choose>
       </xsl:variable>
          <!-- Compute the background color according to the overall test result -->
       <xsl:variable name="StatusBackgroundColor">
            <xsl:choose>
              <xsl:when test="$STATUS = 'FAILED'">
                <!--<xsl:text>rgb(158, 48, 57)</xsl:text> -->
                <xsl:text>rgb(255, 40, 38)</xsl:text>
              </xsl:when>
              <xsl:when test="$STATUS = 'WARNING'">
                <xsl:text>rgb(132, 76, 84)</xsl:text>
              </xsl:when>
              <xsl:when test="$STATUS = 'PASSED'">
                <xsl:text>rgb(121, 180, 112)</xsl:text>
              </xsl:when>
              <xsl:when test="$STATUS = 'DONE'">
                <xsl:text>white</xsl:text>
              </xsl:when>
              <xsl:when test="$STATUS = 'INFO'">
                <xsl:text>white</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>white</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
       </xsl:variable>
       <!--log entry-->
       <xsl:for-each select = "REPORT/LOG_ENTRY">
           <tr>
               <td><xsl:value-of select="STEP"/></td>
               <td><xsl:value-of select="EXECUTION_TIME"/></td>
               <td bgcolor="{$StatusBackgroundColor}"><xsl:value-of select="STEP_RESULT"/></td>
               <td><xsl:value-of select="DESCRIPTION"/></td>
               <td><xsl:value-of select="STEP_DESCRIPTION"/></td>
               <td><xsl:value-of select="ACTION"/></td>
               <td><xsl:value-of select="INDEX"/></td>
               <td><xsl:value-of select="ELEMENT"/></td>
               <td><xsl:value-of select="VALUE"/></td>
               <td><xsl:value-of select="EXPECTED_RESULTS"/></td>
           </tr>
           <!--    Store image path    -->
           <xsl:variable name="Image">
                <xsl:value-of select="IMAGE_PATH"/>
           </xsl:variable>
           <!--  If XML contains an image path then display it according to the following -->
           <xsl:choose>
               <xsl:when test="normalize-space($Image)!=''">
                  <!-- Comment the Next line to Stop showing embedded image in log  and change the width number to make image size change-->
                  <tr>
                     <td colspan="10">
                       <a href="{$Image}" target="_blank" >
                         <img src="{$Image}" width="300"></img>
                       </a>
                     </td>
                  </tr>
               </xsl:when>
           </xsl:choose>
       </xsl:for-each>
       <tr bgcolor="white">
           <font color="{$fontColor}">
               <td colspan="10">Overall Test Result:  <xsl:value-of select="$OVER_STATUS"/></td>
           </font>
       </tr>
   </table>
 </body>
 </html>
</xsl:template>
</xsl:stylesheet>
```
**2、生成xml报告(python代码会自动生成)**
```
<?xml version="1.0" encoding="utf-8"?>
    <?xml-stylesheet href="../../LOG.XSLT" type="text/xsl"?>
    <REPORT>
        <OVER_STATUS>PASSED</OVER_STATUS>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>2</STEP>
            <EXECUTION_TIME>2017-09-21_12-34-47</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION>普惠家登录</DESCRIPTION>
            <STEP_DESCRIPTION>输入'用户名'</STEP_DESCRIPTION>
            <ACTION>input</ACTION>
            <INDEX>By.XPATH</INDEX>
            <ELEMENT>//Input[@id='userName']</ELEMENT>
            <VALUE>msh195</VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>3</STEP>
            <EXECUTION_TIME>2017-09-21_12-34-48</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>输入'密码'</STEP_DESCRIPTION>
            <ACTION>input</ACTION>
            <INDEX>By.XPATH</INDEX>
            <ELEMENT>//Input[@id='password']</ELEMENT>
            <VALUE>112233</VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>4</STEP>
            <EXECUTION_TIME>2017-09-21_12-34-56</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>单击'立即登录'</STEP_DESCRIPTION>
            <ACTION>click</ACTION>
            <INDEX>By.LINK_TEXT</INDEX>
            <ELEMENT>立即登录</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>5</STEP>
            <EXECUTION_TIME>2017-09-21_12-34-56</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>‘我的帐户’是否存在</STEP_DESCRIPTION>
            <ACTION>verify</ACTION>
            <INDEX>By.LINK_TEXT</INDEX>
            <ELEMENT>我的账户</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS>登录成功，显示登录后页面</EXPECTED_RESULTS>
            <IMAGE_PATH>D:\PHJ\Reporter\LOGS\2017-09-21_12-34-39\Image\image_2017-09-21_12-34-56.png</IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>6</STEP>
            <EXECUTION_TIME>2017-09-21_12-34-59</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION>正常充值</DESCRIPTION>
            <STEP_DESCRIPTION>单击'充值'</STEP_DESCRIPTION>
            <ACTION>click</ACTION>
            <INDEX>By.XPATH</INDEX>
            <ELEMENT>//div[@class='account_center_tips_div_4 fl']/input</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>7</STEP>
            <EXECUTION_TIME>2017-09-21_12-34-59</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>‘充值记录’是否存在</STEP_DESCRIPTION>
            <ACTION>verify</ACTION>
            <INDEX>By.LINK_TEXT</INDEX>
            <ELEMENT>充值记录</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS>进入'充值页面'，存在充值记录链接</EXPECTED_RESULTS>
            <IMAGE_PATH>D:\PHJ\Reporter\LOGS\2017-09-21_12-34-39\Image\image_2017-09-21_12-34-59.png</IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>8</STEP>
            <EXECUTION_TIME>2017-09-21_12-35-00</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>选择‘充值方式'绑定支付</STEP_DESCRIPTION>
            <ACTION>click</ACTION>
            <INDEX>By.ID</INDEX>
            <ELEMENT>rapid</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>9</STEP>
            <EXECUTION_TIME>2017-09-21_12-35-02</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>输入‘金额’</STEP_DESCRIPTION>
            <ACTION>input</ACTION>
            <INDEX>By.ID</INDEX>
            <ELEMENT>recharge_money</ELEMENT>
            <VALUE>100</VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>10</STEP>
            <EXECUTION_TIME>2017-09-21_12-35-03</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>单击‘确认充值’</STEP_DESCRIPTION>
            <ACTION>click</ACTION>
            <INDEX>By.LINK_TEXT</INDEX>
            <ELEMENT>确认充值</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS></EXPECTED_RESULTS>
            <IMAGE_PATH></IMAGE_PATH>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>PASSED</STATUS>
            <STEP>11</STEP>
            <EXECUTION_TIME>2017-09-21_12-35-05</EXECUTION_TIME>
            <STEP_RESULT>PASSED</STEP_RESULT>
            <DESCRIPTION></DESCRIPTION>
            <STEP_DESCRIPTION>充值结果'对话框是否存在</STEP_DESCRIPTION>
            <ACTION>verify</ACTION>
            <INDEX>By.LINK_TEXT</INDEX>
            <ELEMENT>充值完成</ELEMENT>
            <VALUE></VALUE>
            <EXPECTED_RESULTS>弹出'充值结果'对话框，存在充值完成链接</EXPECTED_RESULTS>
            <IMAGE_PATH>D:\PHJ\Reporter\LOGS\2017-09-21_12-34-39\Image\image_2017-09-21_12-35-05.png</IMAGE_PATH>
        </LOG_ENTRY>
    </REPORT>
```
**3、python生成报告代码**
```
#*_*coding:utf-8*_*
import xml.dom.minidom as xmlDoc
import os
import gl
import sys
import time
class cREPORTXML(object):
    def __init__(self):
        self.__struct = self.createReportNode()
    #创建report节点
    def createReportNode(self):
        try:
            xmlD = xmlDoc.Document()
            #xml样式
            xlstNode = xmlD.createProcessingInstruction("xml-stylesheet","href=\"../../LOG.XSLT\" type=\"text/xsl\"")
            xmlD.appendChild(xlstNode)
            report = xmlD.createElement('REPORT')
            xmlD.appendChild(report)
            overStatus = xmlD.createElement('OVER_STATUS')
            overStatus.appendChild(xmlD.createTextNode('PASSED'))
            report.appendChild(overStatus)
            returnResult = []
            returnResult.append(xmlD)
            returnResult.append(report)
            returnResult.append(overStatus)
        except Exception,ex:
            return ex.message
        return returnResult
    def writeOverStatus(self,overStatus = 'PASSED'):
        self.__struct[2]._get_childNodes().item(0).nodeValue = overStatus
    def writeReport(self,dict,xmlPath):
        #reportNodeList = self.createReportNode()
        entry = self.createLogEntry(self.__struct[0],dict)
        self.__struct[1].appendChild(entry)
        self.writeXml(self.__struct[0],xmlPath + r'\reportLog.xml')
        #self.writeXml(self.__struct[0],gl.reporterPath+'reportxml_%s.xml'%(gl.curTimeStr))
 #-------------创建xml格式-有多个相同的节点，并且该节点下有4个名称相同的子节点----------------
        #createLogEntry(self,docObj,executeTime,stepResult,description,stepDiscription,action,index,element,value,expectResult):
    def createLogEntry(self,docObj,dict):
        entry = docObj.createElement("LOG_ENTRY")
        status = docObj.createElement("STATUS")
        nodeStep = docObj.createElement("STEP")
        nodeExecuteTime = docObj.createElement("EXECUTION_TIME")
        nodeStepResult = docObj.createElement("STEP_RESULT")
        nodeComponentName = docObj.createElement("DESCRIPTION")
        nodeStepDiscription = docObj.createElement("STEP_DESCRIPTION")
        nodeActin = docObj.createElement("ACTION")
        nodeIndex = docObj.createElement("INDEX")
        nodeElement = docObj.createElement("ELEMENT")
        nodeValue = docObj.createElement("VALUE")
        nodeExpectResult = docObj.createElement("EXPECTED_RESULTS")
        nodeimagePath = docObj.createElement('IMAGE_PATH')
        status.appendChild(docObj.createTextNode(dict['stepResult']))
        nodeStep.appendChild(docObj.createTextNode(str(dict['Step'])))
        nodeExecuteTime.appendChild(docObj.createTextNode(dict['executeTime']))
        nodeStepResult.appendChild(docObj.createTextNode(dict['stepResult']))
        nodeComponentName.appendChild(docObj.createTextNode(dict['description']))
        nodeStepDiscription.appendChild(docObj.createTextNode(dict['stepDiscription']))
        nodeActin.appendChild(docObj.createTextNode(dict['action']))
        nodeIndex.appendChild(docObj.createTextNode(dict['index']))
        nodeElement.appendChild(docObj.createTextNode(dict['element']))
        nodeValue.appendChild(docObj.createTextNode(dict['value']))
        nodeExpectResult.appendChild(docObj.createTextNode(dict['expectResult']))
        nodeimagePath.appendChild(docObj.createTextNode(dict['ImagePath']))
        entry.appendChild(status)
        entry.appendChild(nodeStep)
        entry.appendChild(nodeExecuteTime)
        entry.appendChild(nodeStepResult)
        entry.appendChild(nodeComponentName)
        entry.appendChild(nodeStepDiscription)
        entry.appendChild(nodeActin)
        entry.appendChild(nodeIndex)
        entry.appendChild(nodeElement)
        entry.appendChild(nodeValue)
        entry.appendChild(nodeExpectResult)
        entry.appendChild(nodeimagePath)
        return entry
    #参数，xml对象，准备存储xml文件路径，文件模式：读 and 写 (r and w)
    def writeXml(self,xmlDoc,xmlPath):
        f = open(xmlPath,"w")
        xmlDoc.writexml(f,indent='\t', addindent='\t', newl='\n', encoding="utf-8")
        f.close()
```
** 4、生成效果（报告内显示图片，点击会显示大图）**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170921140520525-17502145.png)
