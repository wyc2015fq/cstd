# CSV数据导出（分隔符等符号需转义） ，解决Excel卡死的问题 - z69183787的专栏 - CSDN博客
2018年12月10日 16:37:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：199
```java
public static void main(String... args) throws IOException {
        List<String> list = new ArrayList<>();
        list.add("quick");
        list.add("over,the,lazy,dog");
        list.add("Venture \"Extended Edition, Very Large\"");
        String re = list.stream()
                .map(s -> convertToCsvFormat(s))
                .collect(Collectors.joining(","));
        System.out.println(re);
        FileUtils.writeStringToFile(new File("D:\\dddd.csv"),re);
    }
    private static String convertToCsvFormat(String input) {
        if (input.contains("\"")) {
            input = input.replaceAll("\"", "\"\"");
        }
        if (input.contains(",")) {
            input = String.format("\"%s\"", input);
        }
        return input;
    }
```
![](https://img-blog.csdnimg.cn/20181210163726821.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
