# jackson2 json转换至对象，多个属性报错 - z69183787的专栏 - CSDN博客
2016年08月24日 16:04:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4251
```java
@org.junit.Test
    public void jsonTest() throws IOException {
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.configure(SerializationFeature.FAIL_ON_EMPTY_BEANS, false);
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        String json = "{\"age\":11111,\"name\":\"bbbbbbb \"}";
        N n = objectMapper.readValue(json, N.class);
        System.out.flush();
    }
    public static class N{
        int age;
        public int getAge() {
            return age;
        }
        public void setAge(int age) {
            this.age = age;
        }
    }
```
默认情况下 json字符串中如果比对象属性多的话 会报错（GSON 没有这个问题）
com.fasterxml.jackson.databind.exc.UnrecognizedPropertyException: Unrecognized field "name" (class Test$N), not marked as ignorable (one known property: "age"])
加上这句配置 objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
就会成功转换了
