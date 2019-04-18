# Jax-ws 开发webService ，并使用spring注入service类中遇到 空指针 - z69183787的专栏 - CSDN博客
2013年03月26日 10:04:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10451
一般情况下，使用eclipse自带的 jax-ws 生成webservice 会自动生成2个类：
ContractConnector.java
```java
package com.wonders.webservice.contract;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import com.wonders.webservice.contract.entity.bo.ContractWs;
import com.wonders.webservice.contract.service.ContractWsService;
public class ContractConnector {
	
	public ContractConnector() {
		System.out.println("初始化connecter");
	}
	private static ContractWsService contractWsService;
	public void setContractWsService(ContractWsService contractWsService) {
		this.contractWsService = contractWsService;
	}
	public String saveContract(String contractXML,String secret){
		if(secret==null || !"abcd".equals(secret)){
			return "验证码错误！";
		}
		try {
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
			ContractWs contractWs = new ContractWs();		//存到临时表中去
			
			Document document = DocumentHelper.parseText(contractXML);
			Element root = document.getRootElement();
			
			/**必填区域**/
			//1.合同名称
			String contractName = root.elementText("contractName");
			if(!isEmpty(contractName)){
				contractWs.setContractName(contractName);
			}else{
				return "contractName(合同名称)不能为空";
			}
			//2.合同编号
			String contractNo = root.elementText("contractNo");
			if(!isEmpty(contractNo)){
				contractWs.setContractNo(contractNo);
			}else{
				return "contractNo(合同编号)不能为空";
			}
			//3.合同价
			String contractPrice = root.elementText("contractPrice");
			if(!isEmpty(contractPrice)){
				try {
					contractWs.setContractPrice(getFormattedMoney(contractPrice));
				} catch (Exception e) {
					return "contractPrice(合同价)只能输入数字";
				}
			}else{
				return "contractPrice(合同价)不能为空";
			}
			/** 项目要插id,是否根据projectName和projectNo查询后插入
			//4.项目名称
			String projectName = root.elementText("projectName");
			if(!isEmpty(projectName)){
				contract.setProjectName(projectName);
			}else{
				return "projectName(项目名称)不能为空";
			}
			//5.项目编号
			String projectNo = root.elementText("projectNo");
			if(!isEmpty(projectNo)){
				contract.setProjectNo(projectNo);
			}else{
				return "projectNo(项目编号)不能为空";
			}**/
			/**非必填**/
			//6.审定价(投资监理)(万元) 	
			String investSupervisorPrice = root.elementText("investSupervisorPrice");
			if(!isEmpty(investSupervisorPrice)){
				try {
					contractWs.setFinalPrice(getFormattedMoney(investSupervisorPrice));
				} catch (Exception e) {
					return "investSupervisorPrice(审定价(投资监理)只能输入数字)";
				}
			}
			//7.审定价(审价单位)
			String verifyPrice = root.elementText("verifyPrice");
			if(!isEmpty(verifyPrice)){
				try {
					contractWs.setVerifyPrice(getFormattedMoney(verifyPrice));
				} catch (Exception e) {
					return "verifyPrice(审定价(审价单位)只能输入数字)";
				}
			}
			//7.审定价(国家审计)
			String nationVerifyPrice = root.elementText("nationVerifyPrice");
			if(!isEmpty(verifyPrice)){
				try {
					contractWs.setNationVerifyPrice(getFormattedMoney(nationVerifyPrice));
				} catch (Exception e) {
					return "nationVerifyPrice(审定价(国家审计)只能输入数字)";
				}
			}
			
			//8.甲方出资
			//9.甲方执行
			//10.对方单位
			
			//11.支付方式
			String payType = root.elementText("payType");
			if(!isEmpty(payType)){
				contractWs.setPayType(payType);
			}
			//12.合同签约时间
			String contractSignedDate = root.elementText("contractSignedDate");
			if(!isEmpty(contractSignedDate)){
				try {
					sdf.parse(contractSignedDate);
					contractWs.setContractSignedDate(contractSignedDate);
				} catch (ParseException e) {
					return "contractSignedDate(合同签约时间)数据错误";
				}
			}
			//13.合同审批通过时间
			String contractPassedDate = root.elementText("contractPassedDate");
			if(!isEmpty(contractPassedDate)){
				try {
					sdf.parse(contractPassedDate);
					contractWs.setContractPassedDate(contractPassedDate);
				} catch (ParseException e) {
					return "contractPassedDate(合同审批通过时间)数据错误";
				}
			}
			
			//14.合同开始时间
			String contractStartDate = root.elementText("contractStartDate");
			if(!isEmpty(contractStartDate)){
				try {
					sdf.parse(contractStartDate);
					contractWs.setContractStartDate(contractStartDate);
				} catch (ParseException e) {
					return "contractStartDate(合同开始时间)数据错误";
				}
			}
			//15.合同结束时间
			String contractEndDate = root.elementText("contractStartDate");
			if(!isEmpty(contractEndDate)){
				try {
					sdf.parse(contractEndDate);
					contractWs.setContractEndDate(contractEndDate);
				} catch (ParseException e) {
					return "contractEndDate(合同结束时间)数据错误";
				}
			}
			//16.合同状态
			String contractStatus = root.elementText("contractStatus");
			if(!isEmpty(contractStatus)){
				if(contractStatus.equals("1") || contractStatus.equals("2") || contractStatus.equals("3")){
					contractWs.setContractStatus(contractStatus);
				}else{
					return "contractStatus(合同状态)数据错误";
				}
			}
			//17.合同类型
			String contractType = root.elementText("contractType");
			if(!isEmpty(contractType)){
				try {
					String types[] = contractType.split("-");
					if(types.length==2){
						int type1 = Integer.valueOf(types[0]);
						int type2 = Integer.valueOf(types[1]);
						if(type1 == 1){
							if(type2>=1 && type2<=8){
								contractWs.setContractType(contractType);
							}else{
								return "contractType(合同类型)数据错误";
							}
						}else if(type1==2){
							if(type2>=1 && type2<=3){
								contractWs.setContractType(contractType);
							}else{
								return "contractType(合同类型)数据错误";
							}
						}else if(type1==3){
							if(type2==1){
								contractWs.setContractType(contractType);
							}else{
								return "contractType(合同类型)数据错误";
							}
						}
					}else{
						return "contractType(合同类型)数据错误";
					}
				} catch (Exception e) {
					return "contractType(合同类型)数据错误";
				}
			}
			
			//18.采购方式
			String procurementType = root.elementText("procurementType");
			if(!isEmpty(procurementType)){
				try {
					int inviteBidType = Integer.valueOf(procurementType);
					if(inviteBidType>=1 && inviteBidType<=3){
						contractWs.setInviteBidType(procurementType);
					}else{
						return "procurementType(采购方式)数据错误";
					}
				} catch (NumberFormatException e) {
					return "procurementType(采购方式)数据错误";
				}
			}
			//19.合同附件
			//20.合同内容
			String contractContent = root.elementText("contractContent");
			if(!isEmpty(contractContent)){
				contractWs.setContractContent(contractContent);
			}
			//21.备注信息
			String remark = root.elementText("remark");
			if(!isEmpty(remark)){
				contractWs.setRemark("remark");
			}
			contractWs.setRemoved("0");
			contractWs.setCreateDate(sdf.format(new Date()));
			contractWsService.saveContractWs(contractWs);
			
		} catch (DocumentException e) {
			return "解析错误！";
		}
		return "保存成功";
	}
	
	public boolean isEmpty(String str){
		if(str ==null || "".equals(str)) return true;
		return false;
	}
	
	//得到保留6位小数后的字符串
	public String getFormattedMoney(String money){
		if(money==null || "".equals(money)){
			money = "0";
		}
		money = money.trim();
		Double result = 0d;
		try {
			result = Double.valueOf(money);
		} catch (NumberFormatException e) {
			result = 0d;
		}
		DecimalFormat df = new DecimalFormat("#0.000000");
		return df.format(result);
	}
}
```
ContractConnectorDelegate.java
```java
package com.wonders.webservice.contract;
@javax.jws.WebService(targetNamespace = "http://contract.webservice.wonders.com/", serviceName = "ContractConnectorService", portName = "ContractConnectorPort", wsdlLocation = "WEB-INF/wsdl/ContractConnectorService.wsdl")
public class ContractConnectorDelegate {
	com.wonders.webservice.contract.ContractConnector contractConnector = new com.wonders.webservice.contract.ContractConnector();
	public String saveContract(String contractXML, String secret) {
		return contractConnector.saveContract(contractXML, secret);
	}
}
```
ContractConnector中
ContractWsService该类中包含了存取数据及对hibernate 的使用，所以使用spring注入，但webservice调用时，一直抱空指针。
由于ContractConnectorDelegate类中 需要new ContractConnector类 ，所以无法注入service
故最后将service修改为static 后，程序启动就注入，这样new 了之后，该service 存在。
