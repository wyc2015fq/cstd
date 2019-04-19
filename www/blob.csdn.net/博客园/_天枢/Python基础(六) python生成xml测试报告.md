# Python基础(六) python生成xml测试报告 - _天枢 - 博客园
## [Python基础(六) python生成xml测试报告](https://www.cnblogs.com/yhleng/p/7068484.html)
2017-06-23 09:58 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7068484)
思路:
1.使用xslt样式,这样可以很好的和xml结合,做出漂亮的报告
2.生成xml结构
xslt样式是个很有意思,也很强大的,现在用的很多,很方便就能做出一个漂亮的报告,可以百度一下,语法相当简单,跟写html差不多的.
在这里可以定制好,我们要生成报告,是什么样子的,然后在从xml获取数据.
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
        <td colspan="2"> </td>
     </tr>
     <tr bgcolor="#D8BFD8" height = "35">
       <th>执行时间</th>
       <th>单步结果</th>
       <th>响应CODE</th>
       <th>Response信息</th>
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
       <!--background color-->
       <xsl:variable name="backgroundColor">
           <xsl:choose>
               <xsl:when test="$STATUS = 'FAILED'">
                   <xsl:text>rgb(255,0,0)</xsl:text>
               </xsl:when>
               <xsl:when test="$STATUS = 'PASSED'">
                   <xsl:text>rgb(60,179,113)</xsl:text>
               </xsl:when>
               <xsl:otherwise>
                   <xsl:text>white</xsl:text>
               </xsl:otherwise>
           </xsl:choose>
       </xsl:variable>
       <!--log entry-->
       <xsl:for-each select = "REPORT/LOG_ENTRY">
           <tr>
               <td><xsl:value-of select="EXECUTION_TIME"/></td>
               <td bgcolor="{$backgroundColor}"><xsl:value-of select="STEP_RESULT"/></td>
               <td><xsl:value-of select="COMPONENT_NAME"/></td>
               <td><xsl:value-of select="STEP_DESCRIPTION"/></td>
           </tr>
       </xsl:for-each>
       <tr bgcolor="white">
           <font color="{$fontColor}">
               <td colspan="4">Overall Test Result:<xsl:value-of select="$OVER_STATUS"/></td>
           </font>
       </tr>
   </table>
 </body>
 </html>
</xsl:template>
</xsl:stylesheet>
```
xml是要按照,xslt定制的结构进行生成,或者换句话说,xslt样式要按照xml结构去做
xml结构
REPORT/LOG_ENTRY
REPORT下OVER_STATUS节点,这个是整个报告的结果,只有当所有条目为passed时才会为passed
每个LOG_ENTRY节点,代表一行数据
包括:
STATUS单行数据执行状态
EXECUTION_TIME执行时间
STEP_RESULT单步执行结果
COMPONENT_NAME组件名称,
STEP_DESCRIPTION步骤描述
当然这些都可以自已定义,可以自己增加或减少,但是xslt表中也要相对应的增加或减少
```
<?xml version="1.0" encoding="utf-8"?>
    <?xml-stylesheet href="LOG.XSLT" type="text/xsl"?>
    <REPORT>
        <OVER_STATUS>PASSED</OVER_STATUS>
        <LOG_ENTRY>
            <STATUS>FAILED</STATUS>
            <EXECUTION_TIME>2017.06.15 15:57:16</EXECUTION_TIME>
            <STEP_RESULT>FAILED</STEP_RESULT>
            <COMPONENT_NAME>704</COMPONENT_NAME>
            <STEP_DESCRIPTION>{u'nextUrl': u'http://www.elong.com', u'message': u'\u9a8c\u8bc1\u7801\u9519\u8bef', u'code': u'704', u'success': False, u'isShowVerifyCode': True}</STEP_DESCRIPTION>
        </LOG_ENTRY>
        <LOG_ENTRY>
            <STATUS>FAILED</STATUS>
            <EXECUTION_TIME>2017.06.15 15:57:16</EXECUTION_TIME>
            <STEP_RESULT>FAILED</STEP_RESULT>
            <COMPONENT_NAME>704</COMPONENT_NAME>
            <STEP_DESCRIPTION>{u'nextUrl': u'http://www.elong.com', u'message': u'\u9a8c\u8bc1\u7801\u9519\u8bef', u'code': u'704', u'success': False, u'isShowVerifyCode': True}</STEP_DESCRIPTION>
        </LOG_ENTRY>
    </REPORT>
```
下面上,生成xml的python代码,这个创建xml结构就很简单了.
使用xml.dom.minidom就可以了,掌握几个要点很容易就创建一个xml
1,创建一个xml文档
```
import xml.dom.minidom as xmlDoc  #xmlDoc起的别名
xmldoc = xmlDoc.Document
```
2.因为我们要使用xslt样式,所以呢.
创建的xml第一行是xml头    <?xml version="1.0" encoding="utf-8"?> 这样python默认已经有了,就不用我们创建了,那么
第二行,我们要写样式头 <?xml-stylesheet href="LOG.XSLT" type="text/xsl"?>   头中写了引用相同目录下的LOG.XSLT ,类型TEXT/XSL
要注意一点href属性不要写成这样D:\demo\LOG.XSLT 这样写会有问题,只要把这个文件和xml放在同一目录就好了.
```
#xml样式
            xlstNode = xmlD.createProcessingInstruction("xml-stylesheet","href=\"LOG.XSLT\" type=\"text/xsl\"")
            xmlD.appendChild(xlstNode)
```
3.创建一个REPORT根节点,你也可以起名ROOT,叫什么名字看你自己了
创建节点用,createElement,之后用appendChild增加节点到xml对象
```
report = xmlD.createElement('REPORT')
            xmlD.appendChild(report)
```
4.之后创建其它节点,一样用createElement,但是如果节点下要增加内容要用xmlDoc.createTextNode('passed')
以下代码意思是:创建一个over_status的节点,节点文本为passed,然后,将此节点增加到根节点REPORT下
```
overStatus = xmlD.createElement('OVER_STATUS')
            overStatus.appendChild(xmlD.createTextNode('PASSED'))
            report.appendChild(overStatus)
```
![](https://images2015.cnblogs.com/blog/1149221/201706/1149221-20170623094823273-1820222032.png)
有以上4点,基本创建一个xml没有问题了.
 生成xml具体python代码:
以下代码创建根节点做为了一个单独的函数,之所以这么做因为要生成的报告,只创建一个根节点,和over_status 结果状态
其它累加的行放在了节点LOG_ENTRY下,一个根节点下可以有多个LOG_ENTRY节点........一个LOG_ENTRY节点代码一行数据执行结果
```
#*_*coding:utf-8*_*
import xml.dom.minidom as xmlDoc
import os
import gl
import sys
class cREPORTXML(object):
    def __init__(self):
        self.__struct = self.createReportNode()
    #创建report节点
    def createReportNode(self):
        try:
            xmlD = xmlDoc.Document()
            #xml样式
            xlstNode = xmlD.createProcessingInstruction("xml-stylesheet","href=\"LOG.XSLT\" type=\"text/xsl\"")
            xmlD.appendChild(xlstNode)
            report = xmlD.createElement('REPORT')
            xmlD.appendChild(report)
            overStatus = xmlD.createElement('OVER_STATUS')
            overStatus.appendChild(xmlD.createTextNode('PASSED'))
            report.appendChild(overStatus)
            returnResult = []
            returnResult.append(xmlD)
            returnResult.append(report)
        except Exception,ex:
            return ex.message
        return returnResult
    def writeReport(self,execTime,stepResult,comName,stepDisc):
        #reportNodeList = self.createReportNode()
        entry = self.createLogEntry(self.__struct[0],execTime,stepResult,comName,stepDisc)
        self.__struct[1].appendChild(entry)
        self.writeXml(self.__struct[0],gl.reporterPath+'reportxml.xml')
        #self.writeXml(self.__struct[0],gl.reporterPath+'reportxml_%s.xml'%(gl.curTimeStr))
 #-------------创建xml格式-有多个相同的节点，并且该节点下有4个名称相同的子节点----------------
    def createLogEntry(self,docObj,executeTime,stepResult,componentName,stepDiscription):
        entry = docObj.createElement("LOG_ENTRY")
        status = docObj.createElement("STATUS")
        nodeExecuteTime = docObj.createElement("EXECUTION_TIME")
        nodeStepResult = docObj.createElement("STEP_RESULT")
        nodeComponentName = docObj.createElement("COMPONENT_NAME")
        nodeStepDiscription = docObj.createElement("STEP_DESCRIPTION")
        status.appendChild(docObj.createTextNode(stepResult))
        nodeExecuteTime.appendChild(docObj.createTextNode(executeTime))
        nodeStepResult.appendChild(docObj.createTextNode(stepResult))
        nodeComponentName.appendChild(docObj.createTextNode(componentName))
        nodeStepDiscription.appendChild(docObj.createTextNode(stepDiscription))
        entry.appendChild(status)
        entry.appendChild(nodeExecuteTime)
        entry.appendChild(nodeStepResult)
        entry.appendChild(nodeComponentName)
        entry.appendChild(nodeStepDiscription)
        return entry
    #参数，xml对象，准备存储xml文件路径，文件模式：读 and 写 (r and w)
    def writeXml(self,xmlDoc,xmlPath):
        f = open(xmlPath,"w")
        xmlDoc.writexml(f,indent='\t', addindent='\t', newl='\n', encoding="utf-8") #中间的加了一些格式符,这样生成的xml自动对齐格式
        f.close()
if __name__=='__main__':
    reportx =cREPORTXML()
    print  reportx.writeReport('20170602','PASSED','1-SETTEXT','AUTOMATION TEST')
    print  reportx.writeReport('20170606','FIELD','2-SETTEXT','AUTOMATION TEST')
```
 用ie打开xml报告,当然可以看出总结果显示有点问题,这个不影响报告展示,代码中处理一下就好.
简单的报告就完成了,追求完美的可以在细化一下,增加一些其它信息,调整一下颜色.
![](https://images2015.cnblogs.com/blog/1149221/201706/1149221-20170623095513179-1608295165.png)
