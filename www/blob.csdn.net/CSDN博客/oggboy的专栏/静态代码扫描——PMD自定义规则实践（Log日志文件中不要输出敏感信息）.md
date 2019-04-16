# 静态代码扫描——PMD自定义规则实践（Log日志文件中不要输出敏感信息） - oggboy的专栏 - CSDN博客





2016年05月16日 14:53:48[oggboy](https://me.csdn.net/oggboy)阅读数：1816








> 
[上篇文章](http://blog.csdn.net/oggboy/article/details/51354948)用一个简单的规则举例，帮助大家快速了解如何自定义规则。接下来我们开始使用比较复杂的例子进行剖析撰写规则的过程。


我还是以自定义规则的思路逐步讲解，但是不会和上篇一样解释的非常详细，很多细节希望大家能够进行思考后获得恍然大悟的感觉：

## 1. 明确想要自定义的规则。

需要自定义的规则大多来自开发的需求。这次举例的规则是： 

Log日志文件中不要输出敏感信息。根据过往的代码样本总结出可定义为敏感信息的字段包括 

    - pid 

    - uid 

    - imei 

    - classname 

    - getLocalClassName 

    - getPackageCodePath 

    - getPackagePath 

    - android.os.Process.myPid 

    - android.os.Process.myUid 

    - android.os.Process.getUidForName 

其中有些是字符串，有些是获取特定字段的方法，所涉及的主要是手机相关信息、类名、代码路径等。
## 2. 列举会触犯这种规则的所有不同的写法。

错误样例的代码：

```java
public class TestLog{
    static Logger Log = Logger.getLogger("log");
    static boolean DEBUG = true;
    static boolean DEBUG1 = false;
     public static void main(String []args){
        Context cont = activity.getApplicationContext();    
        String classname = activity.getLocalClassName();
        String pcodeName = cont.getPackageCodePath();
        int id= android.os.Process.myPid();
        String pid =String.valueOf(id);     
        int uicd= android.os.Process.myUid();
        String uid = String.valueOf(uicd);
        int idname= android.os.Process.getUidForName("pay");
        String imei = ((TelephonyManager)getSystemService(TELEPHONY_SERVICE)).getDeviceId();
        int bbq=activity.getLocalClassName();

        Log.i("classname", classname);//触发规则
        Log.i("pcodeName", pcodeName);//触发规则
        Log.i("pid", pid);//触发规则 
        Log.i("uid", uid);//触发规则
        Log.i("imei", imei); //触发规则
        Log.i("imei", imei.length);
        Log.i("imei", imei.size());
        Log.i("imei:", activity.getLocalClassName());//触发规则
        Log.i("imei:", MYUUID);
        Log.i("imei:", imei.toString());//触发规则
        Log.i("imei:", ab.imei.toString());//触发规则
        Log.i("imei:", bbq);//触发规则
        Log.i("imei:", idname);//触发规则
        Log.i("imei:", id);//触发规则
        Log.i("imei:", uicd);//触发规则
        Log.i("imei:", pcodeName);//触发规则
        Log.i("imei:", 101);       

        if (DEBUG) {
            Log.i("imei", imei);//触发规则
        }      
        if (DEBUG1) {
            Log.i("imei", imei);
        }      
     }  
}
```

我在代码中标记出了`触发规则`的代码，大家可以根据代码前后文理解这些代码语句输出了什么敏感信息。

## 3. 使用designer.bat分析所有写法的抽象语法树的特点。

在语法树分析阶段，就得把这些触发错误的各种代码进行归类。 

a. 在日志中直接输出敏感信息字符串。 

b. 日志中直接输出敏感信息字符串做了处理。 

c. 日志中输出的变量在前文中被赋值敏感信息。 

d. 日志被日志开关包裹，开关为true时，才会输出敏感信息。
## 4. 编写规则代码捕捉这种特点。

```java
package net.sourceforge.pmd.lang.java.rule.androidredline;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import org.jaxen.JaxenException;

import net.sourceforge.pmd.RuleContext;
import net.sourceforge.pmd.lang.ast.Node;
import net.sourceforge.pmd.lang.java.ast.ASTBlockStatement;
import net.sourceforge.pmd.lang.java.ast.ASTBooleanLiteral;
import net.sourceforge.pmd.lang.java.ast.ASTCompilationUnit;
import net.sourceforge.pmd.lang.java.ast.ASTExpression;
import net.sourceforge.pmd.lang.java.ast.ASTFormalParameter;
import net.sourceforge.pmd.lang.java.ast.ASTIfStatement;
import net.sourceforge.pmd.lang.java.ast.ASTName;
import net.sourceforge.pmd.lang.java.ast.ASTPrimaryPrefix;
import net.sourceforge.pmd.lang.java.ast.ASTStatementExpression;
import net.sourceforge.pmd.lang.java.ast.ASTType;
import net.sourceforge.pmd.lang.java.ast.ASTVariableDeclarator;
import net.sourceforge.pmd.lang.java.ast.ASTVariableDeclaratorId;
import net.sourceforge.pmd.lang.java.ast.ASTVariableInitializer;
import net.sourceforge.pmd.lang.java.rule.AbstractJavaRule;

/** * @author yuanwei  * @category log中敏感信息泄露检测 */
public class LogBlockRule extends AbstractJavaRule {
    private static Set<String> SensitiveStrings = new HashSet<String>();
    private List<ASTName> astNamewithLog =(List<ASTName>)new ArrayList<ASTName>();
    private List<ASTName> SASTNames =(List<ASTName>)new ArrayList<ASTName>();
    private List<ASTVariableDeclaratorId> SensitiveVariables =(List<ASTVariableDeclaratorId>)new ArrayList<ASTVariableDeclaratorId>();
    private List<String> BooleanStrings=new ArrayList<String>();

    static {
        SensitiveStrings.add("classname");
        SensitiveStrings.add("pid");
        SensitiveStrings.add("uid");
        SensitiveStrings.add("imei");
        SensitiveStrings.add("getLocalClassName");
        SensitiveStrings.add("getPackageCodePath");
        SensitiveStrings.add("getPackagePath");
        SensitiveStrings.add("android.os.Process.myPid");
        SensitiveStrings.add("android.os.Process.myUid");
        SensitiveStrings.add("android.os.Process.getUidForName");
    }


    @Override
    public Object visit(ASTCompilationUnit node, Object data) {     
        checkLogRule(node,data);            
        return super.visit(node, data);
    }

    @SuppressWarnings("unchecked")
    private void checkLogRule(Node node,Object data) {
        //long start = System.currentTimeMillis();
        String xpathBoolean = ".//FieldDeclaration/VariableDeclarator/VariableInitializer/Expression/PrimaryExpression"
                +"/PrimaryPrefix/Literal/BooleanLiteral[@True='true']";

        pickUpLogMethods(node); //遍历找出源代码中的log.*代码

        if(!this.astNamewithLog.isEmpty()){
            try {
                List<ASTBooleanLiteral> xpathBooleanStringNames = (List<ASTBooleanLiteral>) node.findChildNodesWithXPath(xpathBoolean); 
                if(xpathBooleanStringNames.size()>0){
                    for(ASTBooleanLiteral booleanLiteral:xpathBooleanStringNames){
                        ASTVariableDeclarator variableDeclarator = booleanLiteral.getFirstParentOfType(ASTVariableDeclarator.class);
                        ASTVariableDeclaratorId variableDeclaratorId = variableDeclarator.getFirstChildOfType(ASTVariableDeclaratorId.class);
                        this.BooleanStrings.add(variableDeclaratorId.getImage());
                    }
                }
                List<ASTName> xpathLogNames = this.astNamewithLog; 
                if(xpathLogNames.size()>0){ 
                    for(ASTName name:xpathLogNames){                        
                        String imageString = name.getImage();
                        if(imageString!=null && imageString.contains("Log.")){
                            ASTIfStatement ifStatement=name.getFirstParentOfType(ASTIfStatement.class);
                            ASTBlockStatement blockStatement=name.getFirstParentOfType(ASTBlockStatement.class);
                            List<ASTName> names2=(List<ASTName>) blockStatement.findDescendantsOfType(ASTName.class);

                            if(names2.size()>0){
                                for(ASTName name2:names2){                                  
                                    if(name2!=null){
                                        String imageString2=name2.getImage();

                                        boolean sflag=CheckIsSensitiveString(imageString2);                                     
                                        //当前没发现包含敏感信息，把该ASTName节点存储后续解析
                                        if (!sflag) {
                                            this.SASTNames.add(name2);
                                        }
                                        //当前发现包含敏感信息，确认是否被if包围
                                        if (sflag) {
                                            //被if判断包围，确认判断条件是否为true
                                            if(ifStatement!=null){
                                                ASTExpression astExpression=ifStatement.getFirstDescendantOfType(ASTExpression.class);
                                                ASTName astName =astExpression.getFirstDescendantOfType(ASTName.class);
                                                if(astName!=null){
                                                    String astNameString=astName.getImage();
                                                    //判断条件的值为true
                                                    if(this.BooleanStrings.size()>0 && this.BooleanStrings.contains(astNameString)){                                                    
                                                        addViolation(data, name2);                                              
                                                    }
                                                }                                       
                                            }else {
                                                //没有被if判断包围，触发规则
                                                addViolation(data, name2);
                                            }
                                        }                                       
                                    }   
                                }
                            }                       
                        }
                    }
                }

                //第二层敏感信息监测
                List<ASTVariableDeclaratorId> variableDeclaratorIds = node.findDescendantsOfType(ASTVariableDeclaratorId.class);
                //找出定义的所有变量
                if (variableDeclaratorIds.size()>0) {
                    for(ASTVariableDeclaratorId variableDeclaratorId:variableDeclaratorIds){    
                        ASTType type = variableDeclaratorId.getTypeNode();
                        if(!(type.jjtGetParent() instanceof ASTFormalParameter)){
                            ASTName astName=variableDeclaratorId.getFirstParentOfType(ASTVariableDeclarator.class).getFirstDescendantOfType(ASTName.class);
                            if(astName!=null){
                                if (CheckIsSensitiveString(astName.getImage())) {
                                    this.SensitiveVariables.add(variableDeclaratorId);                                  
                                }                               
                            }
                        }
                    }

                    if(this.SensitiveVariables.size()>0){                       
                        for(ASTVariableDeclaratorId SensitiveVariable:this.SensitiveVariables){
                            for(ASTName SecondastName:this.SASTNames){
                                String astNameimage=SecondastName.getImage();
                                if(!(hasNullInitializer(SensitiveVariable)) && astNameimage!=null&&SensitiveVariable.getImage().equalsIgnoreCase(astNameimage)){
                                    //被if判断包围
                                    ASTIfStatement ifStatement=SecondastName.getFirstParentOfType(ASTIfStatement.class);
                                    if(ifStatement!=null){
                                        ASTExpression astExpression=ifStatement.getFirstDescendantOfType(ASTExpression.class);
                                        ASTName astName3 =astExpression.getFirstDescendantOfType(ASTName.class);
                                        if(astName3!=null){
                                            String astNameString=astName3.getImage();
                                            //判断条件的值为true
                                            if(this.BooleanStrings.size()>0 && this.BooleanStrings.contains(astNameString)){
                                                addViolation(data, SecondastName);                                              
                                            }
                                        }
                                    //没有被if判断包围，触发规则
                                    }else{
                                        addViolation(data, SecondastName);
                                    }
                                }                               
                            }
                        }
                    }   
                }               
            } catch (JaxenException e) {
                e.printStackTrace();
            }finally{
                this.astNamewithLog.clear();
                this.SASTNames.clear();
                this.BooleanStrings.clear();
                this.SensitiveVariables.clear();
            }
         }
    }

    //判断是否包含敏感信息
    private boolean CheckIsSensitiveString(String imageString2) {   
        for(String SensitiveString:SensitiveStrings){
            if(imageString2.equalsIgnoreCase(SensitiveString)){
                return true;
            }
            if (imageString2!=null&&imageString2.contains(".")) {
                String[] partStrings=imageString2.split("\\.");
                int LastIndex=partStrings.length-1;
                if (partStrings[LastIndex].equals("length")||partStrings[LastIndex].equals("size")) {
                    return false;
                }else {
                    for (int i = 0; i < partStrings.length; i++) {
                        String partString = partStrings[i];
                        if (partString.equalsIgnoreCase(SensitiveString)) {
                            return true;                            
                        }
                    }
                }               
            }           
        }   
        return false;
    }

    //判断变量是否为null，如果初始化为null则剔除
    private boolean hasNullInitializer(ASTVariableDeclaratorId var) {
        ASTVariableInitializer init = var.getFirstDescendantOfType(ASTVariableInitializer.class);
        if (init != null) {
            try {
                List<?> nulls = init.findChildNodesWithXPath("Expression/PrimaryExpression/PrimaryPrefix/Literal/NullLiteral");
                return !nulls.isEmpty();
            } catch (JaxenException e) {
                return false;
            }
        }
        return false;
    }

    private void pickUpLogMethods(Node node){
        //遍历找出源代码中的log.*代码
        List<ASTStatementExpression> pexs =  node.findDescendantsOfType(ASTStatementExpression.class);

        for(ASTStatementExpression ast : pexs){
            ASTPrimaryPrefix primaryPrefix = ast.jjtGetChild(0).getFirstDescendantOfType(ASTPrimaryPrefix.class);
            if (primaryPrefix != null) {
                ASTName name = primaryPrefix.getFirstChildOfType(ASTName.class);
                if (name != null) {
                    String imageString = name.getImage();
                    if(imageString.startsWith("Log.")){
                        astNamewithLog.add(name);
                    }
                }
            }
        }       
    }
}
```

为了方便大家的理解，代码加了注释。

## 5. 创建自己的xml规则文件，内容包括规则的相关信息。

在上一篇文章中已经详细的介绍过，不再赘述。

## 6. 运行PMD扫描错误代码，验证是否能触发自定义规则。

特别需要注意的一点，规则完成后，使用样例代码验证只是确认自己撰写的逻辑是否正确。后续一定要经过大量实际代码扫描的验证，逐渐的提高规则的准确性，降低误报率。





