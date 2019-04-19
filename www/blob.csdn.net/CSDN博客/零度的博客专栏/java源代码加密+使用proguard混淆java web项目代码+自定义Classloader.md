# java源代码加密+使用proguard混淆java web项目代码+自定义Classloader - 零度的博客专栏 - CSDN博客
2015年08月06日 17:55:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：9116
如何保护我们的[源代码](https://www.baidu.com/s?wd=%E6%BA%90%E4%BB%A3%E7%A0%81&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3uHcYnvNbPWT4mhR3n1RL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6K1TL0qnfK1TL0z5HD0IgF_5y9YIZ0lQzqlpA-bmyt8mh7GuZR8mvqVQL7dugPYpyq8Q1DYn1DdnW6vP1fsPHRLnHDYnHb)，实际上，应该有几种方法可以使用：1、使用代码混淆器
 2、重载应用服务器的classloader
使用代码混淆器proguard进行代码混淆
1.首先下载proGuard.zip到本地: [proguard4.5beta4.tar.zip](http://files.cnblogs.com/zhouyalei/proguard4.5beta4.tar.zip)解压开,
2.新建文本文档,修改文件名为XXX.pro,然后复制下面内容到.pro
```
-injars 'Y:\src.jar' #需要混淆的jar
-outjars 'Y:\java\src.jar' #输出jar
-libraryjars 'C:\java\jre6\lib\rt.jar' #必须的系统类库
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\activation-1.1.jar' #项目需要的jar
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\ant.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\antlr-2.7.6.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\asm-attrs.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\asm.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\aspectjrt.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\aspectjweaver.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\axis-ant.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\axis.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\backport-util-concurrent.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\bsf-2.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\bsh-2.0b4.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\c3p0-0.9.1.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\cfgatewayadapter.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\cglib-2.1.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\ckeditor-java-core-3.5.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\CKFinder-2.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\CKFinderPlugin-FileEditor-2.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\CKFinderPlugin-ImageResize-2.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\cmpp3.0-java.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-attributes-api.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-attributes-compiler.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-beanutils-1.8.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-codec-1.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-collections-3.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-configuration-1.6.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-dbcp-1.2.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-discovery-0.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-fileupload-1.2.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-io-2.0.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-lang-2.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-logging-1.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-logging-api-1.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-pool.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\commons-validator-1.3.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\concurrent.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\dom4j-1.6.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\dwr.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\ehcache-1.5.0.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\ejb3-persistence.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\fckeditor-java-core-2.4.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\freemarker-2.3.15.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\generic-hession-api-0109.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\gson-1.7.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\hessian-4.0.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\hibernate-annotations.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\hibernate-commons-annotations.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\hibernate-entitymanager.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\hibernate-validator.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\hibernate3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\im4java-1.3.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jackson-core-asl-1.3.0.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jackson-mapper-asl-1.3.0.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jakarta-oro-2.0.8.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\Java2Word.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\javassist-3.4.GA.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jaxrpc.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jboss-archive-browsing.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jdbc2_0-stdext.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jdom.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jetty-6.1.24.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jetty-plus-6.1.24.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jetty-util-6.1.24.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jsf-api.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jsf-impl.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\json-lib-2.3-jdk15.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\json_simple-1.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jstl-1.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jstl-api-1.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jstl-impl-1.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jta.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\jtds-1.2.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\junit.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\log4j-1.2.15.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\mail-1.4.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\mysql-connector-java-5.1.22-bin.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\ognl-2.7.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\ojdbc5.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\p6spy.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\pager-taglib.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\poi-3.6-20091214.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\quartz-all-1.6.6.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\servlet-api-2.5-20081211.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\SimpleAPI.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\sitemesh-2.3.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\slf4j-api-1.5.8.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\slf4j-log4j12-1.5.8.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\spring.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\standard.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\struts2-core-2.1.8.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\struts2-dojo-plugin-2.1.8.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\struts2-json-plugin-2.3.1.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\struts2-sitemesh-plugin-2.1.8.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\struts2-spring-plugin-2.1.8.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\struts2-sunspoter-stream-1.0.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\Thumbnailator-0.3.10.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\urlrewrite-2.6.0.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\velocity-1.7.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\velocity-tools-2.0.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\wsdl4j-1.5.1.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\xalan.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\xstream-1.2.2.jar'
-libraryjars 'Y:\kf\WebRoot\WEB-INF\lib\xwork-core-2.1.6.jar'
-dontskipnonpubliclibraryclassmembers
-target 1.5
-dontshrink
-dontoptimize
-useuniqueclassmembernames
-keeppackagenames
-keepattributes Exceptions,InnerClasses,Signature,Deprecated,SourceFile,LineNumberTable,LocalVariable*Table,*Annotation*,Synthetic,EnclosingMethod
-keepparameternames
-dontpreverify
-dontwarn
-ignorewarnings
#不混淆lib下jar包里面的class文件
-keep class WebRoot.WEB-INFO.lib.*
#不混淆action包里面的类名,字段名,方法名
#在界面上会用到这些字段和getset方法
-keepnames class **.action.**{
    public private protected <fields>;
    public <methods>;
}
-keepnames class **.dao.**{
}
-keepnames class **.service.**{
}
-keep class * extends org.springframework.orm.hibernate3.support.HibernateDaoSupport {
    public <methods>;
}
#不混淆实体的字段和方法,jsp的表单里面会有这些属性的值,混淆之后,会出现无法提交表单情况
-keep class **.domain.** {
    public private protected <fields>;
    public <methods>;
}
# Keep names - Native method names. Keep all native class/method names.
-keepclasseswithmembers,allowshrinking class * {
    native <methods>;
}
# Keep names - _class method names. Keep all .class method names. This may be
# useful for libraries that will be obfuscated again with different obfuscators.
-keepclassmembers,allowshrinking class * {
    java.lang.Class class$(java.lang.String);
    java.lang.Class class$(java.lang.String,boolean);
}
# Remove - System method calls. Remove all invocations of System
# methods without side effects whose return values are not used.
-assumenosideeffects public class java.lang.System {
    public static long currentTimeMillis();
    static java.lang.Class getCallerClass();
    public static int identityHashCode(java.lang.Object);
    public static java.lang.SecurityManager getSecurityManager();
    public static java.util.Properties getProperties();
    public static java.lang.String getProperty(java.lang.String);
    public static java.lang.String getenv(java.lang.String);
    public static java.lang.String mapLibraryName(java.lang.String);
    public static java.lang.String getProperty(java.lang.String,java.lang.String);
}
# Remove - Math method calls. Remove all invocations of Math
# methods without side effects whose return values are not used.
-assumenosideeffects public class java.lang.Math { 
    public static double sin(double); 
    public static double cos(double); 
    public static double tan(double); 
    public static double asin(double); 
    public static double acos(double); 
    public static double atan(double); 
    public static double toRadians(double); 
    public static double toDegrees(double); 
    public static double exp(double); 
    public static double log(double); 
    public static double log10(double); 
    public static double sqrt(double);
    public static double cbrt(double);
    public static double IEEEremainder(double,double); 
    public static double ceil(double); 
    public static double floor(double); 
    public static double rint(double); 
    public static double atan2(double,double); 
    public static double pow(double,double); 
    public static int round(float); 
    public static long round(double); 
    public static double random(); 
    public static int abs(int); 
    public static long abs(long); 
    public static float abs(float); 
    public static double abs(double); 
    public static int max(int,int); 
    public static long max(long,long); 
    public static float max(float,float); 
    public static double max(double,double); 
    public static int min(int,int); 
    public static long min(long,long); 
    public static float min(float,float); 
    public static double min(double,double); 
    public static double ulp(double); 
    public static float ulp(float); 
    public static double signum(double); 
    public static float signum(float); 
    public static double sinh(double); 
    public static double cosh(double); 
    public static double tanh(double); 
    public static double hypot(double,double); 
    public static double expm1(double); 
    public static double log1p(double); 
}
# Remove - Number method calls. Remove all invocations of Number 
# methods without side effects whose return values are not used. 
-assumenosideeffects public class java.lang.* extends java.lang.Number { 
    public static java.lang.String toString(byte); 
    public static java.lang.Byte valueOf(byte); 
    public static byte parseByte(java.lang.String); 
    public static byte parseByte(java.lang.String,int); 
    public static java.lang.Byte valueOf(java.lang.String,int); 
    public static java.lang.Byte valueOf(java.lang.String); 
    public static java.lang.Byte decode(java.lang.String);
    public int compareTo(java.lang.Byte); 
    public static java.lang.String toString(short); 
    public static short parseShort(java.lang.String); 
    public static short parseShort(java.lang.String,int); 
    public static java.lang.Short valueOf(java.lang.String,int); 
    public static java.lang.Short valueOf(java.lang.String); 
    public static java.lang.Short valueOf(short); 
    public static java.lang.Short decode(java.lang.String); 
    public static short reverseBytes(short); 
    public int compareTo(java.lang.Short); 
    public static java.lang.String toString(int,int); 
    public static java.lang.String toHexString(int); 
    public static java.lang.String toOctalString(int); 
    public static java.lang.String toBinaryString(int); 
    public static java.lang.String toString(int); 
    public static int parseInt(java.lang.String,int); 
    public static int parseInt(java.lang.String);
    public static java.lang.Integer valueOf(java.lang.String,int); 
    public static java.lang.Integer valueOf(java.lang.String); 
    public static java.lang.Integer valueOf(int);
    public static java.lang.Integer getInteger(java.lang.String); 
    public static java.lang.Integer getInteger(java.lang.String,int);
    public static java.lang.Integer getInteger(java.lang.String,java.lang.Integer); 
    public static java.lang.Integer decode(java.lang.String);
    public static int highestOneBit(int); 
    public static int lowestOneBit(int);
    public static int numberOfLeadingZeros(int); 
    public static int numberOfTrailingZeros(int); 
    public static int bitCount(int); 
    public static int rotateLeft(int,int); 
    public static int rotateRight(int,int); 
    public static int reverse(int); 
    public static int signum(int); 
    public static int reverseBytes(int); 
    public int compareTo(java.lang.Integer); 
    public static java.lang.String toString(long,int); 
    public static java.lang.String toHexString(long); 
    public static java.lang.String toOctalString(long); 
    public static java.lang.String toBinaryString(long); 
    public static java.lang.String toString(long); 
    public static long parseLong(java.lang.String,int); 
    public static long parseLong(java.lang.String);
    public static java.lang.Long valueOf(java.lang.String,int); 
    public static java.lang.Long valueOf(java.lang.String);
    public static java.lang.Long valueOf(long);
    public static java.lang.Long decode(java.lang.String); 
    public static java.lang.Long getLong(java.lang.String); 
    public static java.lang.Long getLong(java.lang.String,long);
    public static java.lang.Long getLong(java.lang.String,java.lang.Long); 
    public static long highestOneBit(long); 
    public static long lowestOneBit(long); 
    public static int numberOfLeadingZeros(long); 
    public static int numberOfTrailingZeros(long); 
    public static int bitCount(long); 
    public static long rotateLeft(long,int); 
    public static long rotateRight(long,int); 
    public static long reverse(long); 
    public static int signum(long); 
    public static long reverseBytes(long); 
    public int compareTo(java.lang.Long); 
    public static java.lang.String toString(float); 
    public static java.lang.String toHexString(float); 
    public static java.lang.Float valueOf(java.lang.String); 
    public static java.lang.Float valueOf(float); 
    public static float parseFloat(java.lang.String); 
    public static boolean isNaN(float); 
    public static boolean isInfinite(float); 
    public static int floatToIntBits(float); 
    public static int floatToRawIntBits(float); 
    public static float intBitsToFloat(int); 
    public static int compare(float,float); 
    public boolean isNaN(); 
    public boolean isInfinite();
    public int compareTo(java.lang.Float); 
    public static java.lang.String toString(double); 
    public static java.lang.String toHexString(double); 
    public static java.lang.Double valueOf(java.lang.String); 
    public static java.lang.Double valueOf(double); 
    public static double parseDouble(java.lang.String); 
    public static boolean isNaN(double); 
    public static boolean isInfinite(double); 
    public static long doubleToLongBits(double); 
    public static long doubleToRawLongBits(double); 
    public static double longBitsToDouble(long); 
    public static int compare(double,double); 
    public boolean isNaN(); 
    public boolean isInfinite();
    public int compareTo(java.lang.Double);
    public <init>(byte); 
    public <init>(short); 
    public <init>(int); 
    public <init>(long); 
    public <init>(float); 
    public <init>(double); 
    public <init>(java.lang.String); 
    public byte byteValue(); 
    public short shortValue(); 
    public int intValue(); 
    public long longValue(); 
    public float floatValue(); 
    public double doubleValue();
    public int compareTo(java.lang.Object); 
    public boolean equals(java.lang.Object); 
    public int hashCode();
    public java.lang.String toString(); 
}
# Remove - String method calls. Remove all invocations of String # methods without side effects whose return values are not used. 
-assumenosideeffects public class java.lang.String { 
    public <init>(); 
    public <init>(byte[]); 
    public <init>(byte[],int); 
    public <init>(byte[],int,int); 
    public <init>(byte[],int,int,int);
    public <init>(byte[],int,int,java.lang.String); 
    public <init>(byte[],java.lang.String); 
    public <init>(char[]); 
    public <init>(char[],int,int); 
    public <init>(java.lang.String); 
    public <init>(java.lang.StringBuffer);
    public static java.lang.String copyValueOf(char[]); 
    public static java.lang.String copyValueOf(char[],int,int); 
    public static java.lang.String valueOf(boolean); 
    public static java.lang.String valueOf(char); 
    public static java.lang.String valueOf(char[]); 
    public static java.lang.String valueOf(char[],int,int); 
    public static java.lang.String valueOf(double); 
    public static java.lang.String valueOf(float); 
    public static java.lang.String valueOf(int);
    public static java.lang.String valueOf(java.lang.Object); 
    public static java.lang.String valueOf(long); 
    public boolean contentEquals(java.lang.StringBuffer); 
    public boolean endsWith(java.lang.String); 
    public boolean equalsIgnoreCase(java.lang.String); 
    public boolean equals(java.lang.Object); 
    public boolean matches(java.lang.String);
    public boolean regionMatches(boolean,int,java.lang.String,int,int); 
    public boolean regionMatches(int,java.lang.String,int,int); 
    public boolean startsWith(java.lang.String); 
    public boolean startsWith(java.lang.String,int); 
    public byte[] getBytes();
    public byte[] getBytes(java.lang.String); 
    public char charAt(int); 
    public char[] toCharArray();
    public int compareToIgnoreCase(java.lang.String); 
    public int compareTo(java.lang.Object); 
    public int compareTo(java.lang.String); 
    public int hashCode(); 
    public int indexOf(int); 
    public int indexOf(int,int); 
    public int indexOf(java.lang.String); 
    public int indexOf(java.lang.String,int); 
    public int lastIndexOf(int); 
    public int lastIndexOf(int,int); 
    public int lastIndexOf(java.lang.String); 
    public int lastIndexOf(java.lang.String,int); 
    public int length();
    public java.lang.CharSequence subSequence(int,int); 
    public java.lang.String concat(java.lang.String);
    public java.lang.String replaceAll(java.lang.String,java.lang.String); 
    public java.lang.String replace(char,char);
    public java.lang.String replaceFirst(java.lang.String,java.lang.String); 
    public java.lang.String[] split(java.lang.String); 
    public java.lang.String[] split(java.lang.String,int); 
    public java.lang.String substring(int); 
    public java.lang.String substring(int,int); 
    public java.lang.String toLowerCase();
    public java.lang.String toLowerCase(java.util.Locale); 
    public java.lang.String toString(); 
    public java.lang.String toUpperCase();
    public java.lang.String toUpperCase(java.util.Locale); 
    public java.lang.String trim();
}
# Remove - StringBuffer method calls. Remove all invocations of StringBuffer
# methods without side effects whose return values are not used. 
-assumenosideeffects public class java.lang.StringBuffer { 
    public <init>(); 
    public <init>(int);
    public <init>(java.lang.String); 
    public <init>(java.lang.CharSequence); 
    public java.lang.String toString(); 
    public char charAt(int); 
    public int capacity(); 
    public int codePointAt(int); 
    public int codePointBefore(int); 
    public int indexOf(java.lang.String,int); 
    public int lastIndexOf(java.lang.String); 
    public int lastIndexOf(java.lang.String,int); 
    public int length();
    public java.lang.String substring(int); 
    public java.lang.String substring(int,int); 
}
# Remove - StringBuilder method calls. Remove all invocations of StringBuilder # methods without side effects whose return values are not used. 
-assumenosideeffects public class java.lang.StringBuilder { 
    public <init>(); 
    public <init>(int);
    public <init>(java.lang.String); 
    public <init>(java.lang.CharSequence); 
    public java.lang.String toString(); 
    public char charAt(int); 
    public int capacity(); 
    public int codePointAt(int); 
    public int codePointBefore(int); 
    public int indexOf(java.lang.String,int); 
    public int lastIndexOf(java.lang.String); 
    public int lastIndexOf(java.lang.String,int); 
    public int length();
    public java.lang.String substring(int); 
    public java.lang.String substring(int,int); 
} 
[]()
```
 这段话是在网上找的,拼凑一下,能用...
修改上面的-injars,-outjars,-libraryjars路径等
3.开始->运行->输入"cmd"->输入"java -Xms256m -Xmx512m -jar D:\java\proguard4.5beta4\lib\proguard.jar @D:\java\proguard4.5beta4\kfwggl.pro"
不带引号,"D:\java\proguard4.5beta4\lib\proguard.jar"这个是proguard的jar路径,参照着填写自己的路径
"@D:\java\proguard4.5beta4\kfwggl.pro"这个是上面新建的文本文档的路径
然后回车,可能会有些错误,根据提示修改,成功例子:
![](http://images.cnitblog.com/blog/340816/201306/18171949-3ca76cf912c140e595e7665df7a0cc1b.png)
上图红线圈着的,表示混淆成功,输出jar在Y:\java\src.jar
上面完成了代码混淆，一般情况下，大多数的人都是用混编器（java obfuscator）来把开发出来的程序进行打乱，以想达到防止反编译的目的，但是，这种方法在网上也很容易找到相关的软件来重新整理，那么这个混编器工具也只能控制一些本来就不怎么会编程的人，而对于编程大牛没有多大意义。下面再介绍另一种方案。使用自定义的ClassLoader进行加密解密。我们在使用自定义的classloader的时候，装载自己的程序，那么就可以对装入的字节码进行一定的操作，比如解密。在调用自定义的装载器classloader的时候，首先是要装入被加密之后的文件，通常情况下仍旧已.class为扩展名，在调用defineClass之前对装入的数据解密。自定义ClassLoader在这里不详细展开，网上有大量的实例，下面给出一个可以自动生成密钥，使用该密钥进行加密解密的工具，读者可将该工具嵌入到自定义的ClassLoader之中实现，源代码的加密解密。该加密工具需要引入sunjce_provider.jar,读者可自行下载
package org.nercita.jar.security;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.Security;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
public class DESEncryptUtil {
//创建密钥
public static Key createKey() throws NoSuchAlgorithmException {//创建密钥
      Security.insertProviderAt(new com.sun.crypto.provider.SunJCE(), 1);
      KeyGenerator generator = KeyGenerator.getInstance("DES");
      generator.init(new SecureRandom());
      Key key = generator.generateKey();
      return key;
}
//
public static Key getKey(InputStream is) {
      try {
             ObjectInputStream ois = new ObjectInputStream(is);
             return (Key) ois.readObject();
        } catch (Exception e) {
          e.printStackTrace();
          throw new RuntimeException(e);
       }
}
//
private static byte[] doEncrypt(Key key, byte[] data) {//对数据进行加密?
      try {
           Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
           cipher.init(Cipher.ENCRYPT_MODE, key);
           byte[] raw = cipher.doFinal(data);
           return raw;
        } catch (Exception e) {
           e.printStackTrace();
           throw new RuntimeException(e);
       }
}
//
public static InputStream doDecrypt(Key key, InputStream in) {//对数据进行解密?
   try {
          Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
          cipher.init(Cipher.DECRYPT_MODE, key);
          ByteArrayOutputStream bout = new ByteArrayOutputStream();
          byte[] tmpbuf = new byte[1024];
          int count = 0;
          while ((count = in.read(tmpbuf)) != -1) {
              bout.write(tmpbuf, 0, count);
              tmpbuf = new byte[1024];
          }
          in.close();
          byte[] orgData = bout.toByteArray();
          byte[] raw = cipher.doFinal(orgData);
          ByteArrayInputStream bin = new ByteArrayInputStream(raw);
          return bin;
       } catch (Exception e){
          e.printStackTrace();
          throw new RuntimeException(e);
      }
}
//
public static void main(String[] args) throws Exception{
            System.out.println("===================");
        if(args.length == 2 && args[0].equals("key")){
            Key key = DESEncryptUtil.createKey();
            ObjectOutputStream oos = new ObjectOutputStream(
            new FileOutputStream(args[1]));
            oos.writeObject(key);
            oos.close();
            System.out.println("成功生成密钥文件");
       }else if (args.length == 3 && args[0].equals("encrypt")){
         File file = new File(args[1]);
         FileInputStream in = new FileInputStream(file);
         ByteArrayOutputStream bout = new ByteArrayOutputStream();
         byte[] tmpbuf = new byte[1024];
         int count = 0;
         while ((count = in.read(tmpbuf)) != -1){
           bout.write(tmpbuf, 0, count);
           tmpbuf = new byte[1024];
       }
         in.close();
         byte[] orgData = bout.toByteArray();
         Key key = getKey(new FileInputStream(args[2]));
         byte[] raw = DESEncryptUtil.doEncrypt(key, orgData);
         file = new File(file.getParent() + "\\en_" + file.getName());
         FileOutputStream out = new FileOutputStream(file);
         out.write(raw);
         out.close();
         System.out.println("成功加密，加密文件为:"+file.getAbsolutePath());
    } else if (args.length == 3 && args[0].equals("decrypt")){//对文件进行解密
        File file = new File(args[1]);
        FileInputStream fis = new FileInputStream(file);
        Key key = getKey(new FileInputStream(args[2]));
        InputStream raw = DESEncryptUtil.doDecrypt(key, fis);
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        byte[] tmpbuf = new byte[1024];
        int count = 0;
        while ((count = raw.read(tmpbuf)) != -1){
            bout.write(tmpbuf, 0, count);
            tmpbuf = new byte[1024];
        }
        raw.close();
        byte[] orgData = bout.toByteArray();
        file = new File(file.getParent() + "\\rs_" + file.getName());
        FileOutputStream fos = new FileOutputStream(file);
        fos.write(orgData);
        System.out.println("成功解密，解密文件位:"+file.getAbsolutePath());
    }else if(args.length==1 && args[0].equals("-h")) {
        System.out.println("\t文件加密解密\n");
        System.out.println("创建密钥文件：java -jar key.jar key E:/key.dat");
        System.err.println("其中key.jar为需要运行的Jar文件,key参数表示要创建加密文件 ,E:/key.dat表示加密文件的存放位置");
        System.out.println("\n");
        System.out.println("加密文件:java -jar key.jar encrypt E:/test.properties E:/key.dat ");
        System.err.println("其中key.jar为需要运行的Jar文件 encrypt 参数表示加密 E:/test.properties表示需要加密的文件 E:/key.dat表示加密密钥文件");
        System.out.println("\n");
        System.out.println("解密文件：java -jar key.jar decrypt E:/en_test.properties E:/key.dat ");
        System.err.println("其中key.jar为需要运行的Jar文件 decrypt参数表示解密 E:/en_test.properties表示需要解密的文件 E:/key.dat表示解密的密钥文件");
    }else {
        System.out.println("你需要运行参数-h");
    }
  }
}

