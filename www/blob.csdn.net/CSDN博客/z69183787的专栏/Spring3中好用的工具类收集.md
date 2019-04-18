# Spring3中好用的工具类收集 - z69183787的专栏 - CSDN博客
2014年08月13日 12:06:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1143
**1) 请求工具类 org.springframework.web.bind.ServletRequestUtils**
//取请求参数的整数值：
public static Integer getIntParameter(ServletRequest request, String name)
public static int getIntParameter(ServletRequest request, String name, int defaultVal) -->**单个值**
public static int[] getIntParameters(ServletRequest request, String name) -->**数组**
还有譬如**long、float、double、boolean、String**的相关处理方法。
**2) 字符串工具类 org.springframework.util.StringUtils**
首字母大写： public static String capitalize(String str)
首字母小写：public static String uncapitalize(String str)
判断字符串是否为null或empty： public static boolean hasLength(String str)
判断字符串是否为非空白字符串(即至少包含一个非空格的字符串)：public static boolean hasText(String str)
获取文件名：public static String getFilename(String path) 如e.g. "mypath/myfile.txt" -> "myfile.txt"
获取文件扩展名：public static String getFilenameExtension(String path) 如"mypath/myfile.txt" -> "txt"
还有譬如数组转集合、集合转数组、路径处理、字符串分离成数组、数组或集合合并为字符串、数组合并、向数组添加元素等。
**3) 对象序列化与反序列化 org.springframework.util.SerializationUtils**
public static byte[] serialize(Object object)
public static Object deserialize(byte[] bytes)
**4) 数字处理 org.springframework.util.NumberUtils**
字符串转换为Number并格式化，包括具体的Number实现类，如Long、Integer、Double，字符串支持16进制字符串，并且会自动去除字符串中的空格：
    public static <T extends Number> T parseNumber(String text, Class<T> targetClass)
    public static <T extends Number> T parseNumber(String text, Class<T> targetClass, **NumberFormat**numberFormat)
各种Number中的转换，如Long专为Integer，自动处理数字溢出（抛出异常）：
public static <T extends Number> T convertNumberToTargetClass(Number number, Class<T> targetClass)
**5) 文件复制 org.springframework.util.FileCopyUtils**
流与流之间、流到字符串、字节数组到流等的复制
**6) 目录复制 org.springframework.util.FileSystemUtils**
递归复制、删除一个目录
**7) MD5加密 org.springframework.util.DigestUtils**
字节数组的MD5加密 public static String md5DigestAsHex(byte[] bytes)
