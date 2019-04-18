# Gson解析null替换为空字符串 - z69183787的专栏 - CSDN博客
2016年08月03日 17:36:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1544
json解析有很多工具，这里说的是最常用也是解析速度最快的Gson，Gson是google家出的，有一个缺点就是无法设置null替换，
我们只能手动的批量替换服务器返回的null了，正常的接口定义的时候是绝对不允许服务器返回null的，后台结果却总会出现null！
如果搜索的话有一个常见的答案，
`Gson gson = new GsonBuilder().serializeNulls().create();`
但是这个却无法解决反序列问题,怎么解决呢？我在stackoverflow上找到了这个问题，亲测有效
[http://stackoverflow.com/questions/9483348/gson-treat-null-as-empty-string/24252578#24252578](http://stackoverflow.com/questions/9483348/gson-treat-null-as-empty-string/24252578#24252578)
解决办法如下：
```
Gson gson  = new GsonBuilder().registerTypeAdapterFactory(new NullStringToEmptyAdapterFactory()).create();
//然后用上面一行写的gson来序列化和反序列化实体类type
gson.fromJson(json, type);
gson.toJson(type);
```
//NullStringToEmptyAdapterFactory的代码
```
public class NullStringToEmptyAdapterFactory<T> implements TypeAdapterFactory {
    @SuppressWarnings("unchecked")
    public <T> TypeAdapter<T> create(Gson gson, TypeToken<T> type) {
        Class<T> rawType = (Class<T>) type.getRawType();
        if (rawType != String.class) {
            return null;
        }
        return (TypeAdapter<T>) new StringNullAdapter();
    }
}
```
// StringNullAdapter代码
```
public class StringNullAdapter extends TypeAdapter<String> {
    @Override
    public String read(JsonReader reader) throws IOException {
        // TODO Auto-generated method stub
        if (reader.peek() == JsonToken.NULL) {
            reader.nextNull();
            return "";
        }
        return reader.nextString();
    }
    @Override
    public void write(JsonWriter writer, String value) throws IOException {
        // TODO Auto-generated method stub
        if (value == null) {
            writer.nullValue();
            return;
        }
        writer.value(value);
    }
}
```
文／hloong（简书作者）
原文链接：http://www.jianshu.com/p/c6dd9a6b10ee
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
