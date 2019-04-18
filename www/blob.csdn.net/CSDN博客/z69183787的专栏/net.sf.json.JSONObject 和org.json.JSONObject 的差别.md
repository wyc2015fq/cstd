# net.sf.json.JSONObject 和org.json.JSONObject 的差别 - z69183787的专栏 - CSDN博客
2015年07月16日 17:10:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12409
net.sf.json.JSONObject 和org.json.JSONObject  的差别。
一、创建json对象
String str = "{\"code\":\"0000\", \"msg\":{\"availableBalance\":31503079.02}}
org.json.JSONObject：
JSONObject json = new JSONObject(str);
net.sf.json.JSONObject：
JSONObject json = JSONObject.fromObject(str);  net.sf.json.jsonobject 没有 new JSONObject(String)的构造方法
二、解析json
第一种直接用json对象.getXXX()；方法获取
net.sf.json.JSONObject： 没有严格要求获取字段的类型跟getXXX()的类型一样
org.json.JSONObject：获取的字段类型必须跟getXXX()的类型一样
e.g.
JSONObject msgObj = json.getJSONObject("msg");
String availableBalance = msgObj.getString("availableBalance");
如果在org.json.JSONObject 就会报错，可以msgObj.getDouble("availableBalance");也不会丢精度；而net.sf.json.JSONObject正确，但是精度会丢失，如果String str = "{\"code\":\"0000\",
 \"msg\":{\"availableBalance\":\"31503079.02\"}}";
就不会丢失精度。
第二中json对象直接转变实体对象
public class BalanceDto {
private String availableBalance;
public String getAvailableBalance() {
return availableBalance;
}
public void setAvailableBalance(String availableBalance) {
this.availableBalance = availableBalance;
}
public String toString(){
return "availableBalance   "+availableBalance;
}
}
org.json.JSONObject：
BalanceDto alanceDto  = (BalanceDto) JSONObject.stringToValue(msgObj);
这个句话编译通过，但是运行会报错，原因是BalanceDto 类中availableBalance 的类型跟json中的“availableBalance ”类型不同意
net.sf.json.JSONObject：
String msg = json.getString("msg");
BalanceDto  alanceDto = (BalanceDto) JSONObject.toBean(
msg, new BalanceDto().getClass());
三、从json中获取数组
JSONArray subArray = json.getJSONArray("msg");
net.sf.json.JSONObject:
int leng = subArray.size();
org.json.JSONObject：
int leng = subArray.length();
