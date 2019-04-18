# 使用Jackson转换带下划线的属性为驼峰属性 - z69183787的专栏 - CSDN博客
2016年04月29日 10:03:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14941
使用jackson时，如果json的属性使用下划线，而实体的属性使用驼峰式，怎么办？
很简单，直接贴代码
```java
public class JacksonTest {
    public static void main(String[] args) throws JsonParseException, JsonMappingException, IOException {
        ObjectMapper mapper = new ObjectMapper();
        String json = "{\"user_name\":\"bflee\",\"id_number\":\"123456\"}";
        mapper.setPropertyNamingStrategy(PropertyNamingStrategy.CAMEL_CASE_TO_LOWER_CASE_WITH_UNDERSCORES);
        O o = (O) mapper.readValue(json, O.class);
        System.out.println(o.getIdNumber());
        }
    }
    class O implements Serializable{
        private static final long serialVersionUID = -3004824622398622080L;
        private String userName ;
        private String idNumber ;
        public String getUserName() {
            return userName;
        }
        public void setUserName(String userName) {
            this.userName = userName;
        }
        public String getIdNumber() {
            return idNumber;
        }
        public void setIdNumber(String idNumber) {
            this.idNumber = idNumber;
     }
}
```
