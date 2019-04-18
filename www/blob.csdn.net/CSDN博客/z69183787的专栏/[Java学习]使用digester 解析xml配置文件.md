# [Java学习]使用digester 解析xml配置文件 - z69183787的专栏 - CSDN博客
2015年07月19日 12:46:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1311
Java离不开xml配置文件，而Apache的Digester是解析xml的一个很好的工具。
Digester的运行依赖下列包：
一个遵循Jaxp(1.1版本及以后)的XML解析器
Jakarta commons beanutils包(1.5版本及以后)
Jakarta commons collections包(2.1版本及以后)
Jakarta commons logging包(1.0.2版本及以后)
（请到 http://www.apache.org 下载运行环境）
下面，是我的一个用digester解析xml配置文件的例子.
//--webconfig.xml--
<?xml version="1.0" encoding="UTF-8"?>
<web>
<root name = "root_name1">
<bar id="0" title="The Zero Child"/>
<bar id="123" title="The First Child"/>
<bar id="456" title="The Second Child"/>
</root>
<root name = "root_name2">
<bar id="789" title="Another Child"/>
</root>
</web>
分析该文件，发现如下的层次结构web-> root -> bar
因此，我们先建立三个映射的JavaBean：WebConfig.java ,Root.java, Bar.java
//--Bar--.java
package com.stru.config;
public class Bar {
private int id;
private String title;
public int getId() {
return id;
}
public void setId(int id) {
this.id = id;
}
public String getTitle() {
return title;
}
public void setTitle(String title) {
this.title = title;
}
}
//--Root--.java
package com.stru.config;
import java.util.HashMap ;
public class Root {
private String name;
private HashMap barsMap = new HashMap();
public String getName() {
return name;
}
public void setName(String name) {
this.name = name;
}
public void addBar(Bar bar){
barsMap.put(new Integer(bar.getId()), bar) ;
}
public Bar findBar(int id){
return (Bar)barsMap.get(new Integer(id)) ; 
}
public Bar[] getBars(){
Object objs[] = barsMap.values().toArray() ;
Bar[] results = new Bar[objs.length];
for (int i = 0; i < objs.length-1; i++) {
for (int j = 0; j < objs.length - 1 - i; j++) {
int x1 = ((Bar)objs[j]).getId() ;
int x2 = ((Bar)objs[j+1]).getId() ;
if (x1>x2){
Object temp = objs[j] ;
objs[j] = objs[j+1] ;
objs[j+1] = temp ;
}
}
}
for (int i = 0; i < objs.length; i++) {
results[i] = (Bar) objs[i];
}
return results ;
}
public int getSize(){
return barsMap.size() ;
}
}
//--WebConfig.java--
package com.stru.config;
import java.util.HashMap ;
public class WebConfig {
private HashMap rootsMap = new HashMap() ;
public void addRoot(Root root){
rootsMap.put(root.getName(), root) ;
}
public Root findRoot (String name){
return (Root)rootsMap.get(name) ;
}
public Root[] getRoots(){
Object objs[] = rootsMap.values().toArray() ;
Root[] results = new Root[objs.length];
for (int i = 0; i < objs.length; i++) {
results[i] = (Root) objs[i];
}
return results ;
}
public int getSize(){
return rootsMap.size() ;
}
}
映射的JavaBean 建立之后，需要建立对应规则，Digester的对应规则如下：　|Digest||
|----|----|
|Digester digester = new Digester();|新建一个类|
|digester.setValidating(false);|是否进行XML与相应的DTD的合法性验证（此处为false）|
|digester.addObjectCreate("root", "Root");|当遇到<root>时创建一个Root对象，并将其放在栈顶|
|digester.addSetProperties("root");|根据<root>元素的属性(attribute)，对刚创建的Root对象的属性(property)进行设置|
|digester.addObjectCreate("root/bar", "Bar");|当遇到<root>的子元素<bar>时创建一个Bar对象，并将其放在栈顶。|
|digester.addSetProperties("root/bar");|根据<bar>元素的属性(attribute)，对刚创建的Bar对象的属性(property)进行设置|
|digester.addSetNext("root/bar", "addBar", "Bar");|当再次遇到<root>的子元素<bar>时创建一个Bar对象，并将其放在栈顶，同时调用第二栈顶元素(Root对象)的addBar方法。|
|digester.parse();|进行解析，得到的是Object|
//--ConfigRuleSet.java--
package com.stru.config;
import org.apache.commons.digester.*;
public class ConfigRuleSet extends RuleSetBase{        //注意，需要继承RuleSetBase
public void addRuleInstances(Digester digester){
digester.setValidating( false );
digester.addObjectCreate("web", WebConfig.class) ;
digester.addSetProperties("web");
digester.addObjectCreate( "web/root", Root.class );
digester.addSetProperties("web/root");
digester.addSetNext( "web/root", "addRoot" );
digester.addObjectCreate( "web/root/bar", Bar.class );
digester.addSetProperties("web/root/bar");
digester.addSetNext( "web/root/bar", "addBar" );
} 
}
这样就可以，简单吧？最后，我们建立一个测试类来进行测试：
//ConfigTest.java
package com.stru.config;
import java.io.File;
import java.io.IOException;
import org.apache.commons.digester.*;
import org.xml.sax.SAXException;
public class ConfigTest {
public static void main(String[] args) {
final String fileName = "D://ecampus//tom-stru//WEB-INF//webconfig.xml" ;
WebConfig config = new WebConfig() ;
Digester digester = new Digester() ;
ConfigRuleSet rule = new ConfigRuleSet();
rule.addRuleInstances(digester) ;
try {
config = (WebConfig)digester.parse(new File(fileName)) ;
System.out.println("roots size :" + config.getSize()) ;
Root[] roots = config.getRoots() ;
for (int i = 0; i < roots.length; i++) {
System.out.println("root name:" + roots[i].getName()) ;
Bar[] bars = roots[i].getBars() ;
for (int j = 0; j < bars.length; j++) {
if(j>0) System.out.println("/t------------------");
System.out.println("/tbar id:" + bars[j].getId()) ;
System.out.println("/tbar title:" + bars[j].getTitle()) ;
}
}
} catch (IOException e) {
e.printStackTrace();
} catch (SAXException e) {
e.printStackTrace();
} 
}
}
运行结果：
roots size :2
root name:root_name2
bar id:789
bar title:Another Child
root name:root_name1
bar id:0
bar title:The Zero Child
------------------
bar id:123
bar title:The First Child
------------------
bar id:456
bar title:The Second Child
此程序在Eclipse3.0 下测试通过.
