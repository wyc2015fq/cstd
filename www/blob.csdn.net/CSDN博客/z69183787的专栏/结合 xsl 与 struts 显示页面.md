# 结合 xsl 与 struts 显示页面 - z69183787的专栏 - CSDN博客
2014年02月20日 16:40:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2494
个人分类：[模板引擎-Xsl](https://blog.csdn.net/z69183787/article/category/2278717)
后台：
```java
@Action(value="viewXML")
	public String viewXMLById(){
		String id =  request.getParameter("id");
		DwDataExchangeStore data = dataExchangeService.loadById(id);
		String xmlStr=data.getContent();
		String type=data.getDataType();
		Long valid=data.getValid();		
		xmlStr=xmlStr.replace("<root","<?xml-stylesheet type=\"text/xsl\" href=\"/portal/dataExchange/xsl/"+type+".xsl\"?>\n<root id=\""+id+"\" valid=\""+String.valueOf(valid)+"\"");
		
		Document doc=null;
		try {
			doc = DocumentHelper.parseText(xmlStr);
		} catch (DocumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Element deptIdEle=(Element)doc.selectSingleNode("root/Datas/BasicData/reportCompany");
		Element nameEle=(Element)doc.selectSingleNode("root/Datas/BasicData/reportPerson");
		if(deptIdEle!=null&&nameEle!=null){
			Attribute attr = deptIdEle.attribute("id");
			if(attr!=null){
				String deptId = attr.getText();
				if("10218".equals(deptId)){
					Attribute attr1 = nameEle.attribute("id");
					if(attr1!=null){
						attr1.setValue("G01009000200");
					}
					nameEle.setText("裘珏莹");
				}
				if("10215".equals(deptId)){
					Attribute attr1 = nameEle.attribute("id");
					if(attr1!=null){
						attr1.setValue("G01007000257");
					}
					nameEle.setText("魏宗浩");
				}
			}
		}
		xmlStr = doc.asXML();
		actionWriteXML(xmlStr);
		return null;
	}
```
```java
public void actionWriteXML(String str){
		if(response==null) return;
		Writer w = null;
		
		try {
			response.setContentType("text/xml");
			response.setCharacterEncoding("UTF-8");
			
			w = response.getWriter();
			
			w.write(str);
			
			w.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}finally{
			if(w!=null){
				try {
					w.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
```
xsl文件：
```
<?xml version="1.0" encoding="utf-8"?>
  <!-- Edited with XML Spy v2007 (http://www.altova.com) -->
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method='html' version='1.0' encoding='UTF-8' indent='yes' />
  <xsl:template match="/">
  <xsl:variable name="recValid" select="root/@valid"/>  
<html lang="en">
<head>
 <meta http-equiv="X-UA-Compatible" content="IE=edge" charset="utf-8"  />
<title>上海申通地铁集团有限公司项目立项申报表</title>
<link rel="stylesheet" href="css/formalize.css" />
<link rel="stylesheet" href="css/page.css" />
<link rel="stylesheet" href="css/default/imgs.css" />
<link rel="stylesheet" href="css/reset.css" />
    <link type="text/css" href="css/flick/jquery-ui-1.8.18.custom.css" rel="stylesheet" />
    <!--[if IE 6.0]>
           <script src="js/iepng.js" type="text/javascript"></script>
           <script type="text/javascript">
                EvPNG.fix('div, ul, ol, img, li, input, span, a, h1, h2, h3, h4, h5, h6, p, dl, dt');
           </script>
       <![endif]-->
<script src="/portal/js/html5.js"></script>
<script src="/portal/js/jquery-1.7.1.js"></script>
<script src="/portal/js/jquery-ui-1.8.18.custom.min.js"></script>
<script src="/portal/js/jquery.formalize.js"></script>  
<script type="text/javascript" src="js/eamProject.js"></script>  
</head>
<body class="Flow" onload="init()">
<div class="f_bg_fw">
    <div class="w850">
      <div class="logo_2"></div>
    </div>
      <div class="gray_bg">
          <!--Panel_6-->  
        <div class="Divab1">
          <!--1st-->
            <div class="panel_6">
              <div class="divT">
                <div class="mb10 icon icon_1"></div>
                <div class="more_4"><a href="#" title="更多">更多</a></div>
              </div>
              <div class="divH">
                <div class="divB">
                  <h5 class="clearfix">业务办理</h5>
                  <div class="con">
                    <ul class="button clearfix">                     
	                  	<xsl:if test="root[@valid='1']">
	                    	<li id="ywbl"><a href="javascript:eamProjectYwbl(1,'{$recValid}');" class="ywbl">保存草稿</a></li>
	                    </xsl:if>
					  <li><a href="#" target="blank" class="print">打印</a></li>
                      <li><a href="#" class="jk">业务监控</a></li>
                      <li><a href="#" class="tips">小提示</a></li>
                      <li><a href="#" class="imp">公文导入</a></li>
                      <li><a href="#" class="exp">公文导出</a></li>
                    </ul>
                  </div>
                </div>
                <div class="divF"></div>
              </div>
            </div>
            <!--1st End-->
            
        </div>
      <!--Divab1 End-->  
       <form action="http://10.1.48.60/workflow/PclProjectBasicInfo/addPclProjectBasicInfo.action" id="formAdd" name="formAdd" method="post" target="theOldUrl">  
			<input type="hidden" name="stepName" value="部门接受人工作分发"/>
            <input type="hidden" name="UserLoginName" value=""/>
            <input type="hidden" name="modelName" value="项目立项流程"/>
			<input type="hidden" name="DepLeaderUserLoginName" value=""/>
	  		<input type="hidden" name="UserLoginNames" value=""/>
		  	<input type="hidden" name="DepLeaderUserLoginNames" value=""/>
            <input type="hidden" id="reportPerson" name="reportPerson"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportPerson/@id"/></xsl:attribute></input>
            <input type="hidden" name="removed" value="0"/>
            <input type="hidden" id="projectAttachId" name="projectAttachId" value=""/>
            <input type="hidden" id="legislationInfoAttachId"  name="legislationInfoAttachId"/>
            <input type="hidden" id="excuteSolutionAttachId"  name="excuteSolutionAttachId"/>
            <input type="hidden" id="projectBudgetAttachId"  name="projectBudgetAttachId"/>
            <input type="hidden" id="projectPlanAttachId"  name="projectPlanAttachId"/>
            <input type="hidden" name="projectdevise"  id="projectdevise"   value=""/>
            <input type="hidden" name="extCode19"  id="extCode19"   value=""/>
            <input type="hidden" id="projectDeviseAttachId"  name="projectDeviseAttachId"/>
            <input type="hidden" id="extCode14" name="extCode14" value="0"/>
            <input type="hidden" id="extCode15" name="extCode15" value="0"/>
            <input type="hidden" id="extCode16" name="extCode16" value="0"/>
            <input type="hidden" id="extCode17" name="extCode17" value="0"/>
            <input type="hidden" id="extCode18" name="extCode18" value="0"/>
            <input type="hidden" name="loginName"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportPerson/@id"/></xsl:attribute></input>
            <input type="hidden" id="deptId" name="deptId"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportCompany/@id"/></xsl:attribute></input>
		  	<input type="hidden" id="deptName" name="deptName"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportCompany"/></xsl:attribute></input>
		  	<input type="hidden" id="userName" name="userName"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportPerson"/></xsl:attribute></input>
		  	<input name="infoMan" type="hidden"  id="infoMan" ><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportPerson"/></xsl:attribute></input>
       	  	<input name="infoDept" type="hidden" id="infoDept"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportCompany"/></xsl:attribute></input>
      	  	<!--input id="keywords" name="keywords" type="hidden"/-->  
      	  	<!--input name="extCode4" type="hidden" id="extCode4"/-->
         	<input name="ifSave" type="hidden" id="ifSave"/>        
             
          <div class="gray_bg2">
              <div class="w_bg">
                  <div>
                      <div class="Top_fw">
                          <h1 class="t_c">上海申通地铁集团有限公司<br></br>项目立项申报表</h1>
                             <div class="mb10 Step clearfix">
                             <ul class="clearfix">
                                  <li class="fst on w90">
                                      <dl>
                                          <dt></dt>
                                            <dd class="w90">立项</dd>
                                         </dl>
                                     </li>
                                  <li class="w90">
                                      <dl>
                                          <dt></dt>
                                            <dd class="w90">主管部门审核</dd>
                                         </dl>
                                    </li>
                                  <li class="w90">
                                      <dl>
                                          <dt></dt>
                                            <dd class="w90">部门会签</dd>
                                         </dl>
                                    </li>
                                  <li class="w90">
                                      <dl>
                                          <dt></dt>
                                            <dd class="w90">主管部门汇总</dd>
                                      </dl>
                                    </li>
                                  <li class="w90">
                                      <dl>
                                          <dt></dt>
                                            <dd class="w90">集团领导审核</dd>
                                         </dl>
                                    </li>                                    
                                  <li class="fin w90">
                                      <dl>
                                          <dt></dt>
                                            <dd class="w90">办结</dd>
                                         </dl>
                                    </li>
                                </ul>
                            </div>
                            <div class="mb10">
                               <table width="100%" border="0" cellspacing="0" cellpadding="0" class="table_4">
                                  <thead>
                                  <th colspan="4">
                                      <h5 class="fl">基本信息</h5>
                                    </th>
                                  </thead>
                                  <tr>
                                    <td class="lableTd" width="15%">项目名称</td>
                                    <td colspan="3"><input id="pName" name="pName" type="text" size="95"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/projectName"/></xsl:attribute></input></td>                                    
                                  </tr>
                                  <tr>
	                                  <td class="lableTd" width="15%">关键字</td>
	                                  <td colspan="3"><input id="keywords"  name="keywords" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/keywords"/></xsl:attribute></input></td>                                  
	                              </tr>
	                               <tr>
	                                  <td class="lableTd" width="15%">立项类型</td>
	                                  <td><input id="projectType"  name="projectType" type="text"  readonly="readonly" disabled="disabled" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/projectType"/></xsl:attribute></input></td>                                  
	                                  <td class="lableTd" width="15%">投资估算</td>
	                                  <td><input id="investCost" name="investCost" type="text" size="32"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/investCost"/></xsl:attribute></input>万元</td>
	                                </tr> 
	                               <tr>
	                                  <td class="lableTd" width="15%">计划开始时间</td>
	                                  <td><input id="planStartDate" name="planStartDate" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/planStartDate"/></xsl:attribute></input></td>                                  
	                                  <td class="lableTd" width="15%">计划完成时间</td>
	                                  <td><input id="planEndDate" name="planEndDate" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/planEndDate"/></xsl:attribute></input></td>
	                                </tr> 	                                                               
	                                <tr>
	                                  <td class="lableTd" width="15%">出资主体</td>
	                                  <td colspan='3'>
	                                  	<input type ="hidden" name="moneySource" id="moneySource"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/moneySource"/></xsl:attribute></input>                                  	
	                                    <input type ="checkbox" name="moneySource1" id="moneySource1" value="1">
											<xsl:if test="contains(root/Datas/BasicData/moneySource,'1')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                    </input>集团
	                                    <input type ="checkbox" name="moneySource5" id="moneySource5" value="5">
											<xsl:if test="contains(root/Datas/BasicData/moneySource,'5')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                    </input>上市公司
	                                    <input type ="checkbox" name="moneySource2" id="moneySource2" value="2">	                                    
											<xsl:if test="contains(root/Datas/BasicData/moneySource,'2')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                    </input>项目公司
	                                    <input type ="checkbox" name="moneySource3" id="moneySource3" value="3">
	                                    	<xsl:if test="contains(root/Datas/BasicData/moneySource,'3')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                    </input> 申报人自筹
	                      				<input type ="checkbox" name="moneySource4" id="moneySource4" value="4">
	                      					<xsl:if test="contains(root/Datas/BasicData/moneySource,'4')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                    </input>其他		                      			   	                      			
	                                  	<!-- input name="moneySource" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/moneySource"/></xsl:attribute></input-->
	                                  </td>
	                                  
                                	</tr>
                           			<tr>
	                                  <td class="lableTd" width="15%">出资构成</td>
	                                  <td colspan='3'><textarea id="moneySourceDetail" name="moneySourceDetail" rows="5" style="overflow:auto;border-width:1" ><xsl:value-of select="root/Datas/BasicData/moneySourceDetail"/></textarea></td>
	                                  
                                	</tr>   
                                <tr>
                                  <th colspan="4"><h5 class="fl">项目属性</h5></th>
                                </tr>
	                                <tr>
	                                  <td class="lableTd" width="15%">专业分类 </td>
	                                  <td colspan='3'>
										<input type ="checkbox" value="车辆" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'车辆')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
                                      	</input>车辆
	                                  	<input type ="checkbox" value="供电" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'供电')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
                                      	</input>供电
	                                  	<input type ="checkbox" value="通号" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'通号')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                      </input>通号
	                                  	<input type ="checkbox" value="工务" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'工务')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
	                                    </input>工务
	                                  	<input type ="checkbox" value="段场" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'段场')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
                                    	</input>段场
	                                  		<input type ="checkbox" value="车站机电" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'车站机电')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
                                      	</input>车站机电
	                                  	<input type ="checkbox" value="其他" id="majorShow">
	                                        <xsl:if test="contains(root/Datas/BasicData/major,'其他')">
	                                        <xsl:attribute name="checked">true</xsl:attribute>
	                                        </xsl:if>
                                      	</input>其他                        		                                  	                                     		                                      		                                      			                                      		                                 
	                            	    <input type="hidden" id="major" name="major"/>
	                                  	<!-- input name="major" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/major"/></xsl:attribute></input-->
	                                  </td>
	                                  
                                	</tr>  
                                	<tr>
	                                  <td class="lableTd" width="15%">项目分类</td>
	                                  <td colspan='3'>
										<select name="safe" id="safe" class="input_large">
	                                      <option value="">---请选择---</option>
	                                      <option value="大修类">
	                                          <xsl:if test="contains(root/Datas/BasicData/projectClass,'大修类')">
	                                          <xsl:attribute name="selected">selected</xsl:attribute>
	                                          </xsl:if>  大修类
	                                        </option>
	                                      <option value="更新改造类">
	                                          <xsl:if test="contains(root/Datas/BasicData/projectClass,'更新改造类')">
	                                          <xsl:attribute name="selected">selected</xsl:attribute>
	                                          </xsl:if>更新改造类
	                                        </option>
	                                      <option value="能源合同类">
	                                        <xsl:if test="contains(root/Datas/BasicData/projectClass,'能源合同类')">
	                                          <xsl:attribute name="selected">selected</xsl:attribute>
	                                          </xsl:if>能源合同类
	                                        </option>
	                                      <option value="其他类">
	                                          <xsl:if test="contains(root/Datas/BasicData/projectClass,'其他类')">
	                                          <xsl:attribute name="selected">selected</xsl:attribute>
	                                          </xsl:if>其他类
	                                        </option>
	                                    </select>
	                                  <!-- input name="projectClass" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/projectClass"/></xsl:attribute></input-->
	                                  </td>
	                                  
                                	</tr> 
								<tr>
                                 <td class="lableTd" width="15%">项目附件</td>
                                  <td colspan='3'>                                  
                                    <table width="100%" cellpadding="2" cellspacing="0" style="font-size:12px;border-top:#bbb 1px solid;border-right:#bbb 1px solid">
                                      <xsl:if test="root/Datas/AttachFileList/AttachFile/fileName ">
                                      <!--  附件属性名-->
                                      <tr>
                                      <td align='center' nowrap="nowrap" width='40%'>
                                        <font><b>文件名</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>大小</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='20%'>
                                        <font><b>上传时间</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>上传人</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='10%'>
                                        <font><b>版本</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='15%'>
                                        <font><b>备注</b> </font>
                                      </td>
                                      </tr>
                                      <xsl:for-each select="root/Datas/AttachFileList/AttachFile">
            
                                            <tr>
                                            <td style="line-height: 15px">
                                              <xsl:variable name="AttachFilePath"
                                              select="path" />
                                              <input type="hidden" name="filePath_projectAttachId" value="{$AttachFilePath}"/>
                         					  <input type="hidden" name="fileName_projectAttachId"><xsl:attribute name="value"><xsl:value-of select="fileName"/></xsl:attribute></input>
                                              <input type="hidden" name="fileExtName_projectAttachId" ><xsl:attribute name="value"><xsl:value-of select="fileExtName"/></xsl:attribute></input>                                              
                                              <input type="hidden" name="fileSize_projectAttachId"><xsl:attribute name="value"><xsl:value-of select="fileSize"/></xsl:attribute></input>
                                              <input type="hidden" name="version_projectAttachId" ><xsl:attribute name="value"><xsl:value-of select="version"/></xsl:attribute></input>
                                              <input type="hidden" name="memo_projectAttachId" ><xsl:attribute name="value"><xsl:value-of select="memo"/></xsl:attribute></input>
                                              <!--img src="/workflowNew/images/files/doc.gif" style="display: inline;" /-->
                                              <a href="{$AttachFilePath}" target="_blank"
                                                style="display: inline;"><xsl:value-of select="fileName"/>.<xsl:value-of select="fileExtName"/></a>                                              
                                            </td>
                                            <td style="line-height: 15px;">
                                              <xsl:value-of select="fileSize"/> K</td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploadDate"/> </td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploader"/> </td>                                                                                       
                                            <td style="line-height: 15px">v<xsl:value-of select="version"/></td>                                                                                       
                                            <td style="word-break: break-all"><xsl:value-of select="memo"/></td>
                                          </tr>
                                          
                                     </xsl:for-each>
                                     </xsl:if>
                                    </table>
                                  </td>
                                  
                                </tr>                                 
                                <tr>
                                  <th colspan="4"><h5 class="fl">人员信息</h5></th>
                                </tr>   
                               <tr>
                                  <td class="lableTd" width="15%">申报单位</td>
                                  <td><input name="infoDeptShow"  readonly="readonly" disabled="disabled" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportCompany"/></xsl:attribute></input></td>                                  
                                  <td class="lableTd" width="15%">负责人</td>
                                  <td>
                                   	<select name="mainPerson" id="mainPerson" class="input_large">
                                   		<option value="">---请选择---</option>
	                               	</select>
                                  <!-- input id="mainPerson" name="mainPerson" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportChargePerson"/></xsl:attribute></input-->
                                  </td>
                                </tr>   
                               <tr>
                                  <td class="lableTd" width="15%">申报人</td>
                                  <td><input name="infoManShow"  readonly="readonly" disabled="disabled" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportPerson"/></xsl:attribute></input></td>                                  
                                  <td class="lableTd" width="15%">联系电话</td>
                                  <td><input id="infoManTel" name="infoManTel" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/reportCompanyTel"/></xsl:attribute></input></td>
                                </tr>   
                                <tr>
                                  <td class="lableTd" width="15%">执行单位</td>
                                  <td><input id="mainCompany"  name="mainCompany" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/excuteCompany"/></xsl:attribute></input></td>                                  
                                  <td class="lableTd" width="15%">负责人</td>
                                  <td><input id="mainCompanyPerson" name="mainCompanyPerson" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/excuteChargePerson"/></xsl:attribute></input></td>
                                </tr> 
                                <tr>
                                  <td class="lableTd" width="15%">负责人联系电话</td>
                                  <td><input id="mainCompanyPersonTel" name="mainCompanyPersonTel" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/excuteChargePersonTel"/></xsl:attribute></input></td>                                  
                                  <td class="lableTd" width="15%">联系人</td>
                                  <td><input id="linkMan" name="linkMan" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/excuteLinkPerson"/></xsl:attribute></input></td>
                                </tr>  
                                <tr>
                                  <td class="lableTd" width="15%">联系人联系电话</td>
                                  <td><input id="linkManTel" name="linkManTel" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/excuteLinkPersonTel"/></xsl:attribute></input></td>                                  
                                  <td colspan="2"></td>                                  
                                </tr>                                                                    
                                <tr>
                                  <th colspan="4"><h5 class="fl">资料信息</h5></th>
                                </tr>                                   
                        		<tr>
                        			<td class="lableTd" colspan='4'>
										<h5 class="fl">立项依据</h5>
                                        <span class="fr clearfix">
                                          <h6 class="fl">相关附件:</h6><span class="fl mr8" id="fjcount1"><xsl:value-of select="root/Datas/BasicData/establishAccordingAttachNum"/></span>
                                        </span>  
                                    </td>                      		                                  
                               	</tr>  
                        			<tr height="100px">
                                  <td colspan='4'><textarea id="legislationInfo" name="legislationInfo" rows="7"  maxlength="500"><xsl:value-of select="root/Datas/BasicData/establishAccording"/></textarea></td>	                                  
                               	</tr>                                  	                                
								<tr>
                                  <td colspan='4'>                                  
                                    <table width="100%" cellpadding="2" cellspacing="0" style="font-size:12px;border-top:#bbb 1px solid;border-right:#bbb 1px solid">
                                      <xsl:if test="root/Datas/BasicData/establishAccordingAttachList/AttachFile/fileName ">
                                      <!--  附件属性名-->
                                      <tr>
                                      <td align='center' nowrap="nowrap" width='40%'>
                                        <font><b>文件名</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>大小</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='20%'>
                                        <font><b>上传时间</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>上传人</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='10%'>
                                        <font><b>版本</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='15%'>
                                        <font><b>备注</b> </font>
                                      </td>
                                      </tr>
                                      <xsl:for-each select="root/Datas/BasicData/establishAccordingAttachList/AttachFile">
            
                                            <tr>
                                            <td style="line-height: 15px">
                                              <xsl:variable name="AttachFilePath"
                                              select="path" />
                                              <input type="hidden" name="filePath_legislationInfoAttachId" value="{$AttachFilePath}"/>
                         					  <input type="hidden" name="fileName_legislationInfoAttachId"><xsl:attribute name="value"><xsl:value-of select="fileName"/></xsl:attribute></input>
                                              <input type="hidden" name="fileExtName_legislationInfoAttachId" ><xsl:attribute name="value"><xsl:value-of select="fileExtName"/></xsl:attribute></input>                                              
                                              <input type="hidden" name="fileSize_legislationInfoAttachId"><xsl:attribute name="value"><xsl:value-of select="fileSize"/></xsl:attribute></input>
                                              <input type="hidden" name="version_legislationInfoAttachId" ><xsl:attribute name="value"><xsl:value-of select="version"/></xsl:attribute></input>
                                              <input type="hidden" name="memo_legislationInfoAttachId" ><xsl:attribute name="value"><xsl:value-of select="memo"/></xsl:attribute></input>
                                              <!--img src="/workflowNew/images/files/doc.gif" style="display: inline;" /-->
                                              <a href="{$AttachFilePath}" target="_blank"
                                                style="display: inline;"><xsl:value-of select="fileName"/>.<xsl:value-of select="fileExtName"/></a>                                              
                                            </td>
                                            <td style="line-height: 15px;">
                                              <xsl:value-of select="fileSize"/> K</td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploadDate"/> </td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploader"/> </td>                                                                                       
                                            <td style="line-height: 15px">v<xsl:value-of select="version"/></td>                                                                                       
                                            <td style="word-break: break-all"><xsl:value-of select="memo"/></td>
                                          </tr>
                                          
                                     </xsl:for-each>
                                     </xsl:if>
                                    </table>
                                  </td>                                  
                                </tr>    
								<tr>
                        			<td class="lableTd" colspan='4'>
										<h5 class="fl">实施方案</h5>
                                        <span class="fr clearfix">
                                          <h6 class="fl">相关附件:</h6><span class="fl mr8" id="fjcount2"><xsl:value-of select="root/Datas/BasicData/excuteSolutionAttachNum"/></span>
                                        </span>  
                                    </td> 								
                               	</tr>  
                        			<tr height="100px">
                                  <td colspan='4'><textarea  id="excuteSolution" name="excuteSolution" rows="7"   maxlength="500" ><xsl:value-of select="root/Datas/BasicData/excuteSolution"/></textarea></td>	                                  
                               	</tr>                                  	                                
								<tr>
                                  <td colspan='4'>                                  
                                    <table width="100%" cellpadding="2" cellspacing="0" style="font-size:12px;border-top:#bbb 1px solid;border-right:#bbb 1px solid">
                                      <xsl:if test="root/Datas/BasicData/excuteSolutionAttachList/AttachFile/fileName ">
                                      <!--  附件属性名-->
                                      <tr>
                                      <td align='center' nowrap="nowrap" width='40%'>
                                        <font><b>文件名</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>大小</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='20%'>
                                        <font><b>上传时间</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>上传人</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='10%'>
                                        <font><b>版本</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='15%'>
                                        <font><b>备注</b> </font>
                                      </td>
                                      </tr>
                                      <xsl:for-each select="root/Datas/BasicData/excuteSolutionAttachList/AttachFile">
            
                                            <tr>
                                            <td style="line-height: 15px">
                                              <xsl:variable name="AttachFilePath"
                                              select="path" />
                                              <input type="hidden" name="filePath_excuteSolutionAttachId" value="{$AttachFilePath}"/>
                         					  <input type="hidden" name="fileName_excuteSolutionAttachId"><xsl:attribute name="value"><xsl:value-of select="fileName"/></xsl:attribute></input>
                                              <input type="hidden" name="fileExtName_excuteSolutionAttachId" ><xsl:attribute name="value"><xsl:value-of select="fileExtName"/></xsl:attribute></input>                                              
                                              <input type="hidden" name="fileSize_excuteSolutionAttachId"><xsl:attribute name="value"><xsl:value-of select="fileSize"/></xsl:attribute></input>
                                              <input type="hidden" name="version_excuteSolutionAttachId" ><xsl:attribute name="value"><xsl:value-of select="version"/></xsl:attribute></input>
                                              <input type="hidden" name="memo_excuteSolutionAttachId" ><xsl:attribute name="value"><xsl:value-of select="memo"/></xsl:attribute></input>
                                              <!--img src="/workflowNew/images/files/doc.gif" style="display: inline;" /-->
                                              <a href="{$AttachFilePath}" target="_blank"
                                                style="display: inline;"><xsl:value-of select="fileName"/>.<xsl:value-of select="fileExtName"/></a>                                              
                                            </td>
                                            <td style="line-height: 15px;">
                                              <xsl:value-of select="fileSize"/> K</td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploadDate"/> </td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploader"/> </td>                                                                                       
                                            <td style="line-height: 15px">v<xsl:value-of select="version"/></td>                                                                                       
                                            <td style="word-break: break-all"><xsl:value-of select="memo"/></td>
                                          </tr>
                                          
                                     </xsl:for-each>
                                     </xsl:if>
                                    </table>
                                  </td>                                  
                                </tr>
                                <tr>
                        			<td class="lableTd" colspan='4'>
										<h5 class="fl">资金估算</h5>
                                        <span class="fr clearfix">
                                          <h6 class="fl">相关附件:</h6><span class="fl mr8" id="fjcount3"><xsl:value-of select="root/Datas/BasicData/projectBudgetAttachNum"/></span>
                                        </span>  
                                    </td> 	                                
                               	</tr>  
                        			<tr height="100px">
                                  <td colspan='4'><textarea id="projectBudget" name="projectBudget" rows="7"   maxlength="500" ><xsl:value-of select="root/Datas/BasicData/projectBudget"/></textarea></td>	                                  
                               	</tr>                                  	                                
								<tr>
                                  <td colspan='4'>                                  
                                    <table width="100%" cellpadding="2" cellspacing="0" style="font-size:12px;border-top:#bbb 1px solid;border-right:#bbb 1px solid">
                                      <xsl:if test="root/Datas/BasicData/projectBudgetAttachList/AttachFile/fileName ">
                                      <!--  附件属性名-->
                                      <tr>
                                      <td align='center' nowrap="nowrap" width='40%'>
                                        <font><b>文件名</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>大小</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='20%'>
                                        <font><b>上传时间</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>上传人</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='10%'>
                                        <font><b>版本</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='15%'>
                                        <font><b>备注</b> </font>
                                      </td>
                                      </tr>
                                      <xsl:for-each select="root/Datas/BasicData/projectBudgetAttachList/AttachFile">
            
                                            <tr>
                                            <td style="line-height: 15px">
                                              <xsl:variable name="AttachFilePath"
                                              select="path" />
                                              <input type="hidden" name="filePath_projectBudgetAttachId" value="{$AttachFilePath}"/>
                         					  <input type="hidden" name="fileName_projectBudgetAttachId"><xsl:attribute name="value"><xsl:value-of select="fileName"/></xsl:attribute></input>
                                              <input type="hidden" name="fileExtName_projectBudgetAttachId" ><xsl:attribute name="value"><xsl:value-of select="fileExtName"/></xsl:attribute></input>                                              
                                              <input type="hidden" name="fileSize_projectBudgetAttachId"><xsl:attribute name="value"><xsl:value-of select="fileSize"/></xsl:attribute></input>
                                              <input type="hidden" name="version_projectBudgetAttachId" ><xsl:attribute name="value"><xsl:value-of select="version"/></xsl:attribute></input>
                                              <input type="hidden" name="memo_projectBudgetAttachId" ><xsl:attribute name="value"><xsl:value-of select="memo"/></xsl:attribute></input>
                                              <!--img src="/workflowNew/images/files/doc.gif" style="display: inline;" /-->
                                              <a href="{$AttachFilePath}" target="_blank"
                                                style="display: inline;"><xsl:value-of select="fileName"/>.<xsl:value-of select="fileExtName"/></a>                                              
                                            </td>
                                            <td style="line-height: 15px;">
                                              <xsl:value-of select="fileSize"/> K</td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploadDate"/> </td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploader"/> </td>                                                                                       
                                            <td style="line-height: 15px">v<xsl:value-of select="version"/></td>                                                                                       
                                            <td style="word-break: break-all"><xsl:value-of select="memo"/></td>
                                          </tr>
                                          
                                     </xsl:for-each>
                                     </xsl:if>
                                    </table>
                                  </td>                                  
                                </tr>   
                                <tr>
                        			<td class="lableTd" colspan='4'>
										<h5 class="fl">工程计划</h5>
                                        <span class="fr clearfix">
                                          <h6 class="fl">相关附件:</h6><span class="fl mr8" id="fjcount4"><xsl:value-of select="root/Datas/BasicData/projectPlanAttachNum"/></span>
                                        </span>  
                                    </td> 	                                
                               	</tr>  
                        			<tr height="100px">
                                  <td colspan='4'><textarea id="projectPlan"  name="projectPlan" rows="7"  maxlength="500" ><xsl:value-of select="root/Datas/BasicData/projectPlan"/></textarea></td>	                                  
                               	</tr>                                  	                                
								<tr>
                                  <td colspan='4'>                                  
                                    <table width="100%" cellpadding="2" cellspacing="0" style="font-size:12px;border-top:#bbb 1px solid;border-right:#bbb 1px solid">
                                      <xsl:if test="root/Datas/BasicData/projectPlanAttachList/AttachFile/fileName ">
                                      <!--  附件属性名-->
                                      <tr>
                                      <td align='center' nowrap="nowrap" width='40%'>
                                        <font><b>文件名</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>大小</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='20%'>
                                        <font><b>上传时间</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>上传人</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='10%'>
                                        <font><b>版本</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='15%'>
                                        <font><b>备注</b> </font>
                                      </td>
                                      </tr>
                                      <xsl:for-each select="root/Datas/BasicData/projectPlanAttachList/AttachFile">
            
                                            <tr>
                                            <td style="line-height: 15px">
                                              <xsl:variable name="AttachFilePath"
                                              select="path" />
                                              <input type="hidden" name="filePath_projectPlanAttachId" value="{$AttachFilePath}"/>
                         					  <input type="hidden" name="fileName_projectPlanAttachId"><xsl:attribute name="value"><xsl:value-of select="fileName"/></xsl:attribute></input>
                                              <input type="hidden" name="fileExtName_projectPlanAttachId" ><xsl:attribute name="value"><xsl:value-of select="fileExtName"/></xsl:attribute></input>                                              
                                              <input type="hidden" name="fileSize_projectPlanAttachId"><xsl:attribute name="value"><xsl:value-of select="fileSize"/></xsl:attribute></input>
                                              <input type="hidden" name="version_projectPlanAttachId" ><xsl:attribute name="value"><xsl:value-of select="version"/></xsl:attribute></input>
                                              <input type="hidden" name="memo_projectPlanAttachId" ><xsl:attribute name="value"><xsl:value-of select="memo"/></xsl:attribute></input>
                                              <!--img src="/workflowNew/images/files/doc.gif" style="display: inline;" /-->
                                              <a href="{$AttachFilePath}" target="_blank"
                                                style="display: inline;"><xsl:value-of select="fileName"/>.<xsl:value-of select="fileExtName"/></a>                                              
                                            </td>
                                            <td style="line-height: 15px;">
                                              <xsl:value-of select="fileSize"/> K</td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploadDate"/> </td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploader"/> </td>                                                                                       
                                            <td style="line-height: 15px">v<xsl:value-of select="version"/></td>                                                                                       
                                            <td style="word-break: break-all"><xsl:value-of select="memo"/></td>
                                          </tr>
                                          
                                     </xsl:for-each>
                                     </xsl:if>
                                    </table>
                                  </td>                                  
                                </tr> 
                                <tr>
                        			<td class="lableTd" colspan='4'>
										<h5 class="fl">项目设计</h5>
										<xsl:if test="root/Datas/BasicData/projectDesignAttachNum">
                                        <span class="fr clearfix" id="upload">
                                          <h6 class="fl">相关附件:</h6><span class="fl mr8" id="fjcount5"><xsl:value-of select="root/Datas/BasicData/projectDesignAttachNum"/></span>
                                        </span> 
                                        </xsl:if> 
                                    </td> 	                                
                               	</tr>  
                        		<tr>
                                  <td colspan='4'>
					               <input type="radio" value="1" name="projectdevise1" id="projectdevise1" onclick="checkdevise('1')">
                                        <xsl:if test="contains(root/Datas/BasicData/projectDesign,'1')">
                                        <xsl:attribute name="checked">true</xsl:attribute>
                                        </xsl:if>
                                      </input> 无需设计
                                   <input type="radio" value="2" name="projectdevise1" id="projectdevise1" onclick="checkdevise('2')">
                                      <xsl:if test="contains(root/Datas/BasicData/projectDesign,'2')">
                                        <xsl:attribute name="checked">true</xsl:attribute>
                                        </xsl:if>
                                    </input>建议委外设计
                                   <input type="radio" value="3" name="projectdevise1" id="projectdevise1" onclick="checkdevise('3')">
                                      <xsl:if test="contains(root/Datas/BasicData/projectDesign,'3')">
                                        <xsl:attribute name="checked">true</xsl:attribute>
                                        </xsl:if>
                                    </input>提交设计文件
                                                                         
                                  <!-- input name="projectDesign" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/projectDesign"/></xsl:attribute></input-->
                                  </td>	                                  
                               	</tr>                                  	                                
								<tr id="attachshow">
                                  <td colspan='4'>                                  
                                    <table width="100%" cellpadding="2" cellspacing="0" style="font-size:12px;border-top:#bbb 1px solid;border-right:#bbb 1px solid">
                                      <xsl:if test="root/Datas/BasicData/projectDesignAttachList/AttachFile/fileName ">
                                      <!--  附件属性名-->
                                      <tr>
                                      <td align='center' nowrap="nowrap" width='40%'>
                                        <font><b>文件名</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>大小</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='20%'>
                                        <font><b>上传时间</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='5%'>
                                        <font><b>上传人</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='10%'>
                                        <font><b>版本</b> </font>
                                      </td>
                                      <td align='center' nowrap="nowrap" width='15%'>
                                        <font><b>备注</b> </font>
                                      </td>
                                      </tr>
                                      <xsl:for-each select="root/Datas/BasicData/projectDesignAttachList/AttachFile">
            
                                            <tr>
                                            <td style="line-height: 15px">
                                              <xsl:variable name="AttachFilePath"
                                              select="path" />
                                              <input type="hidden" name="filePath_projectDeviseAttachId" value="{$AttachFilePath}"/>
                         					  <input type="hidden" name="fileName_projectDeviseAttachId"><xsl:attribute name="value"><xsl:value-of select="fileName"/></xsl:attribute></input>
                                              <input type="hidden" name="fileExtName_projectDeviseAttachId" ><xsl:attribute name="value"><xsl:value-of select="fileExtName"/></xsl:attribute></input>                                              
                                              <input type="hidden" name="fileSize_projectDeviseAttachId"><xsl:attribute name="value"><xsl:value-of select="fileSize"/></xsl:attribute></input>
                                              <input type="hidden" name="version_projectDeviseAttachId" ><xsl:attribute name="value"><xsl:value-of select="version"/></xsl:attribute></input>
                                              <input type="hidden" name="memo_projectDeviseAttachId" ><xsl:attribute name="value"><xsl:value-of select="memo"/></xsl:attribute></input>
                                              <!--img src="/workflowNew/images/files/doc.gif" style="display: inline;" /-->
                                              <a href="{$AttachFilePath}" target="_blank"
                                                style="display: inline;"><xsl:value-of select="fileName"/>.<xsl:value-of select="fileExtName"/></a>                                              
                                            </td>
                                            <td style="line-height: 15px;">
                                              <xsl:value-of select="fileSize"/> K</td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploadDate"/> </td>
                                            <td style="line-height: 15px"><xsl:value-of select="uploader"/> </td>                                                                                       
                                            <td style="line-height: 15px">v<xsl:value-of select="version"/></td>                                                                                       
                                            <td style="word-break: break-all"><xsl:value-of select="memo"/></td>
                                          </tr>
                                          
                                     </xsl:for-each>
                                     </xsl:if>
                                    </table>
                                  </td>                                  
                                </tr> 
								<tr>
                        			<td class="lableTd" colspan='4'>
										<h5 class="fl">是否需要监理</h5>                                       
                                    </td> 	                                
                               	</tr>  
                        		<tr>
                                  <td colspan='4'>
									<input type="radio" value="1" name="monitor" id="monitor">
                                       <xsl:if test="contains(root/Datas/BasicData/monitor,'1')">
                                        <xsl:attribute name="checked">true</xsl:attribute>
                                        </xsl:if>
                                    </input>需要监理
                                 	<input type="radio" value="2" name="monitor" id="monitor">
                                     <xsl:if test="contains(root/Datas/BasicData/monitor,'2')">
                                        <xsl:attribute name="checked">true</xsl:attribute>
                                        </xsl:if>
                                    </input>无需监理
                                                                                         	         
                                  <!-- input name="monitor" type="text" size="36"><xsl:attribute name="value"><xsl:value-of select="root/Datas/BasicData/monitor"/></xsl:attribute></input-->
                                  </td>	                                  
                               	</tr>                                                                                                     
                              </table>
                          </div>
                          <div class="mb10 t_c">
                          <!-- input type="submit" value="初审通过" />
                                 
                          <input type="button" value="返 回" />
                          <input type="reset" value="取 消" / -->
                          </div>
                            <div class="footer"></div>
                        </div>
                    </div>
                </div>
            </div>
       </form>
        <form action="confirmValid.action" id="recUpdate" name="recUpdate" method="post" target="theUpdateUrl">  
        <input type="hidden" name="id"><xsl:attribute name="value"><xsl:value-of select="root/@id"/></xsl:attribute></input> 
        </form>
       <iframe name="theOldUrl" style="display: none;"></iframe>
        <iframe name="theUpdateUrl" style="display: none;"></iframe>
      </div>       
    </div>
</body>
</html>
     <!--html结束 -->
</xsl:template>
</xsl:stylesheet>
```
