# 关于Gson解析时候特殊符号，被转义的问题，如’单引号 - z69183787的专栏 - CSDN博客
2013年12月24日 19:25:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11930
```
//Creating the JSON object, and getting as String:
JsonObject json = new JsonObject();
JsonObject inner = new JsonObject();
inner.addProperty("value", "xpath('hello')");
json.add("root", inner);
System.out.println(json.toString());
//Trying to pretify JSON String:
Gson gson = new GsonBuilder().setPrettyPrinting().create();
JsonParser parser = new JsonParser();
JsonElement je = parser.parse(json.toString());
System.out.println(gson.toJson(je));
输出
```
```
{"root":{"value":"xpath('hello')"}}
{
  "root": {
    "value": "xpath(\u0027hello\u0027)"
  }
}
解决办法
```
```
Gson gs = new GsonBuilder()
    .setPrettyPrinting()
    .disableHtmlEscaping()
    .create();
```
