# Axis2 webservice客户端（wsdd）复杂对象 - z69183787的专栏 - CSDN博客
2014年07月14日 09:33:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5193
**使用org.apache.axis.description.OperationDesc oper;org.apache.axis.description.ParameterDesc param;**
**组织对应的bo对象，并设置调用方法名，方法返回值及传递值。**
import javax.xml.namespace.QName;
import org.apache.axis.client.Call;
import org.apache.axis.client.Service;
import com.bizenit.ssows.accessservice.AccessServiceImplProxy;
import com.bizenit.ssows.accessservice.GetUserNameRequest;
import com.bizenit.ssows.accessservice.GetUserNameResponse;
import com.bizenit.ssows.accessservice.LoggedInRequest;
import com.bizenit.ssows.accessservice.LoggedInResponse;
import com.bizenit.ssows.accessservice.TargetAcctInRequest;
import com.bizenit.ssows.accessservice.TargetAcctInResponse;
```java
try{  
			org.apache.axis.description.OperationDesc oper;
			org.apache.axis.description.ParameterDesc param;
	        
			oper = new org.apache.axis.description.OperationDesc();
	        oper.setName("TargetAcctIn");
	        param = new org.apache.axis.description.ParameterDesc(
	        		new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", 
	        				"TargetAcctInRequest"), 
	        				org.apache.axis.description.ParameterDesc.IN, 
	        				new javax.xml.namespace.QName(
	        						"http://ssows.bizenit.com/accessservice", ">TargetAcctInRequest"), 
	        						com.bizenit.ssows.accessservice.TargetAcctInRequest.class, false, false);
	        oper.addParameter(param);
	        oper.setReturnType(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", ">TargetAcctInResponse"));
	        oper.setReturnClass(TargetAcctInResponse.class);
	        oper.setReturnQName(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", "TargetAcctInResponse"));
	        oper.setStyle(org.apache.axis.constants.Style.DOCUMENT);
	        oper.setUse(org.apache.axis.constants.Use.LITERAL);
	        //oper.setReturnQName(new javax.xml.namespace.QName(
	          //      "http://xml.apache.org/xml-soap", "Element"));
	        //oper.setReturnClass(org.w3c.dom.Element.class);
	        // 指定访问web服务的位置  
			String url = "http://10.1.48.162:14100/shmetro-ssows/sumwebservice/accessServiceImpl";  
			// 创建服务  
			Service service = new Service(); 
			service.setTypeMappingVersion("1.2");
			// 创建调用  
			Call call = (Call)service.createCall();  
			// 设置调用服务来源  
			
			call.setOperation(oper);
			call.setTargetEndpointAddress(url);  
			call.setUseSOAPAction(true);
	        call.setSOAPActionURI("");
	        call.setEncodingStyle(null);
	        call.setProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
	        call.setProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
	        call.setSOAPVersion(org.apache.axis.soap.SOAPConstants.SOAP11_CONSTANTS);
			// 设置调用方法名  
			call.setOperationName(new QName("", "TargetAcctIn"));  
			// 调用  
			//call.setPortName(new QName("http://ssows.bizenit.com/accessservice","AccessServiceImplSoap11"));
			TargetAcctInRequest r = new TargetAcctInRequest("greata","G01013800003","XieTong");
			LoggedInRequest rr = new LoggedInRequest("XieTong", "CE09B81E92AF1BF5", "G00100000226", "XieTong");
			
			TargetAcctInResponse result = (TargetAcctInResponse)call.invoke(new Object[]{r});  
			System.out.print("result:  " + result.getTargetacct() );  
			}catch(Exception e){  
			e.printStackTrace();  
			}
```
```java
/**
 * LoggedInRequest.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */
package com.bizenit.ssows.accessservice;
public class TargetAcctInRequest  implements java.io.Serializable {
    private java.lang.String targetmark;
    private java.lang.String appacct;
    private java.lang.String appmark;
    public TargetAcctInRequest() {
    }
    public TargetAcctInRequest(
           java.lang.String targetmark,
           java.lang.String appacct,
           java.lang.String appmark) {
           this.targetmark = targetmark;
           this.appacct = appacct;
           this.appmark = appmark;
    }
    public java.lang.String getTargetmark() {
		return targetmark;
	}
	public void setTargetmark(java.lang.String targetmark) {
		this.targetmark = targetmark;
	}
	/**
     * Gets the appacct value for this LoggedInRequest.
     * 
     * @return appacct
     */
    public java.lang.String getAppacct() {
        return appacct;
    }
    /**
     * Sets the appacct value for this LoggedInRequest.
     * 
     * @param appacct
     */
    public void setAppacct(java.lang.String appacct) {
        this.appacct = appacct;
    }
    /**
     * Gets the appmark value for this LoggedInRequest.
     * 
     * @return appmark
     */
    public java.lang.String getAppmark() {
        return appmark;
    }
    /**
     * Sets the appmark value for this LoggedInRequest.
     * 
     * @param appmark
     */
    public void setAppmark(java.lang.String appmark) {
        this.appmark = appmark;
    }
    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof TargetAcctInRequest)) return false;
        TargetAcctInRequest other = (TargetAcctInRequest) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((this.targetmark==null && other.getTargetmark()==null) || 
             (this.targetmark!=null &&
              this.targetmark.equals(other.getTargetmark()))) &&
            ((this.appacct==null && other.getAppacct()==null) || 
             (this.appacct!=null &&
              this.appacct.equals(other.getAppacct()))) &&
            ((this.appmark==null && other.getAppmark()==null) || 
             (this.appmark!=null &&
              this.appmark.equals(other.getAppmark())));
        __equalsCalc = null;
        return _equals;
    }
    private boolean __hashCodeCalc = false;
    public synchronized int hashCode() {
        if (__hashCodeCalc) {
            return 0;
        }
        __hashCodeCalc = true;
        int _hashCode = 1;
        if (getTargetmark() != null) {
            _hashCode += getTargetmark().hashCode();
        }
       if (getAppacct() != null) {
            _hashCode += getAppacct().hashCode();
        }
        if (getAppmark() != null) {
            _hashCode += getAppmark().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }
    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(TargetAcctInRequest.class, true);
    static {
        typeDesc.setXmlType(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", ">TargetAcctInRequest"));
        org.apache.axis.description.ElementDesc elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("targetmark");
        elemField.setXmlName(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", "targetmark"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("appacct");
        elemField.setXmlName(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", "appacct"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
        elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("appmark");
        elemField.setXmlName(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", "appmark"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
    }
    /**
     * Return type metadata object
     */
    public static org.apache.axis.description.TypeDesc getTypeDesc() {
        return typeDesc;
    }
    /**
     * Get Custom Serializer
     */
    public static org.apache.axis.encoding.Serializer getSerializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanSerializer(
            _javaType, _xmlType, typeDesc);
    }
    /**
     * Get Custom Deserializer
     */
    public static org.apache.axis.encoding.Deserializer getDeserializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanDeserializer(
            _javaType, _xmlType, typeDesc);
    }
}
```
```java
/**
 * TargetAcctInResponse.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis 1.4 Apr 22, 2006 (06:55:48 PDT) WSDL2Java emitter.
 */
package com.bizenit.ssows.accessservice;
public class TargetAcctInResponse  implements java.io.Serializable {
    private java.lang.String targetacct;
    public TargetAcctInResponse() {
    }
    public TargetAcctInResponse(
           java.lang.String targetacct) {
           this.targetacct = targetacct;
    }
    /**
     * Gets the targetacct value for this TargetAcctInResponse.
     * 
     * @return targetacct
     */
    public java.lang.String getTargetacct() {
        return targetacct;
    }
    /**
     * Sets the targetacct value for this TargetAcctInResponse.
     * 
     * @param targetacct
     */
    public void setTargetacct(java.lang.String targetacct) {
        this.targetacct = targetacct;
    }
    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof TargetAcctInResponse)) return false;
        TargetAcctInResponse other = (TargetAcctInResponse) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((this.targetacct==null && other.getTargetacct()==null) || 
             (this.targetacct!=null &&
              this.targetacct.equals(other.getTargetacct())));
        __equalsCalc = null;
        return _equals;
    }
    private boolean __hashCodeCalc = false;
    public synchronized int hashCode() {
        if (__hashCodeCalc) {
            return 0;
        }
        __hashCodeCalc = true;
        int _hashCode = 1;
        if (getTargetacct() != null) {
            _hashCode += getTargetacct().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }
    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(TargetAcctInResponse.class, true);
    static {
        typeDesc.setXmlType(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", ">TargetAcctInResponse"));
        org.apache.axis.description.ElementDesc elemField = new org.apache.axis.description.ElementDesc();
        elemField.setFieldName("targetacct");
        elemField.setXmlName(new javax.xml.namespace.QName("http://ssows.bizenit.com/accessservice", "targetacct"));
        elemField.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        elemField.setNillable(false);
        typeDesc.addFieldDesc(elemField);
    }
    /**
     * Return type metadata object
     */
    public static org.apache.axis.description.TypeDesc getTypeDesc() {
        return typeDesc;
    }
    /**
     * Get Custom Serializer
     */
    public static org.apache.axis.encoding.Serializer getSerializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanSerializer(
            _javaType, _xmlType, typeDesc);
    }
    /**
     * Get Custom Deserializer
     */
    public static org.apache.axis.encoding.Deserializer getDeserializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanDeserializer(
            _javaType, _xmlType, typeDesc);
    }
}
```
