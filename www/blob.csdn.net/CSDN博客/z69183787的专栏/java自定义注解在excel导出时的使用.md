# java自定义注解在excel导出时的使用 - z69183787的专栏 - CSDN博客
2016年07月12日 10:46:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1983
自定义注解：
```java
@Documented
@Target({ElementType.METHOD,ElementType.FIELD})
@Retention(RetentionPolicy.RUNTIME)
public @interface ExcelColumn {
    String name() default "zhoushun";
}
```
注解类：
```java
public class ActivityDataDetailBean implements Serializable {
    private static final long serialVersionUID = 7689111063501975678L;
    @ExcelColumn(name="活动ID")
    private int activityId;
    @ExcelColumn(name = "日期")
    private String date;
    @ExcelColumn(name = "城市")
    private String cityName;
    @ExcelColumn(name = "投放位置")
    private String slot;
    @ExcelColumn(name="pageId")
    private String topicId;
    @ExcelColumn(name = "页面PV")
    private Long pv;
    @ExcelColumn(name = "页面UV")
    private Long uv;
    @ExcelColumn(name = "页面点击数")
    private Long click;
    @ExcelColumn(name="页面到达率")
    private String reachRate;
    @ExcelColumn(name = "页面关闭率")
    private String closeRate;
    @ExcelColumn(name = "分享量")
    private Long shareCount;
```
导出时获取注解内容：
```java
private static void getReflectInfo(Class<?> c,List<String> head,List<Method> getMethod) throws IntrospectionException {
        Field[] fields = c.getDeclaredFields();
        for (int i = 0; i < fields.length; i++) {
            if (!fields[i].getName().equalsIgnoreCase("serialVersionUID") && !fields[i].getName().equalsIgnoreCase("$jacocoData")) {
                ExcelColumn excelColumns = fields[i].getAnnotation(ExcelColumn.class);
                if (excelColumns != null) {
                    head.add(excelColumns.name());
                } else {
                    head.add(fields[i].getName());
                }
                getMethod.add(new PropertyDescriptor(fields[i].getName(), c).getReadMethod());
            }
        }
    }
```
