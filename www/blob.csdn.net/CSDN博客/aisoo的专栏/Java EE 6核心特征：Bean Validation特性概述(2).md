# Java EE 6核心特征：Bean Validation特性概述(2) - aisoo的专栏 - CSDN博客
2012年12月05日 11:40:49[aisoo](https://me.csdn.net/aisoo)阅读数：489
约束的定义
约束注解
Bean Validation 规范对约束的定义包括两部分，一是约束注解，清单 1 中的 @NotNull 就是约束注解;二是约束验证器，每一个约束注解都存在对应的约束验证器，约束验证器用来验证具体的 Java Bean 是否满足该约束注解声明的条件。
在 Java Bean 中，对某一方法、字段、属性或其组合形式等进行约束的注解，即为约束注解，如清单 2 所示：
清单 2：
- @NotNull(message = "The id of employee can not be null")   
- private Integer id;  
清单 2 的含义为：对于字段 id，在 Java Bean 的实例中值不能为空。对于每一个约束注解，在实际使用前必须有相关定义。JSR303 规范默认提供了几种约束注解的定义(见表 1)，我们也可以扩展规范提供的 API，实现符合自身业务需求的约束注解。
表 1. Bean Validation 规范内嵌的约束注解定义
|**约束注解名称**|**约束注解说明**|
|----|----|
|@Null|验证对象是否为空|
|@NotNull|验证对象是否为非空|
|@AssertTrue|验证 Boolean 对象是否为 true|
|@AssertFalse|验证 Boolean 对象是否为 false|
|@Min|验证 Number 和 String 对象是否大等于指定的值|
|@Max|验证 Number 和 String 对象是否小等于指定的值|
|@DecimalMin|验证 Number 和 String 对象是否大等于指定的值，小数存在精度|
|@DecimalMax|验证 Number 和 String 对象是否小等于指定的值，小数存在精度|
|@Size|验证对象（Array,Collection,Map,String）长度是否在给定的范围之内|
|@Digits|验证 Number 和 String 的构成是否合法|
|@Past|验证 Date 和 Calendar 对象是否在当前时间之前|
|@Future|验证 Date 和 Calendar 对象是否在当前时间之后|
|@Pattern|验证 String 对象是否符合正则表达式的规则|
约束注解和普通的注解一样，一个典型的约束注解的定义应该至少包括如下内容(清单 3)：
清单 3：
- @Target({ })   // 约束注解应用的目标元素类型 
- @Retention()   // 约束注解应用的时机 
- @Constraint(validatedBy ={})  // 与约束注解关联的验证器 
- public@interface ConstraintName{   
- String message() default" ";   // 约束注解验证时的输出消息 
- Class[] groups() default { };  // 约束注解在验证时所属的组别 
- Classextends Payload>[] payload() default { }; // 约束注解的有效负载 
- }  
约束注解应用的目标元素类型包括 METHOD, FIELD, TYPE, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER。METHOD 约束相关的 getter 方法;FIELD 约束相关的属性;TYPE 约束具体的 Java Bean;ANNOTATION_TYPE 用在组合约束中;该规范同样也支持对参数(PARAMETER)和构造器(CONSTRUCTOR)的约束。
验证时的组别属性将在本文第三大部分中组与组序列中详细介绍。
有效负载通常用来将一些元数据信息与该约束注解相关联，常用的一种情况是用负载表示验证结果的严重程度。
清单 4 给出一个验证字符串非空的约束注解的定义：
清单 4：
- @Target({ METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER })   
- @Retention(RUNTIME)   
- @Documented
- @Constraint(validatedBy = {NotEmptyValidator.class})   
- public@interface NotEmpty {   
- String message() default"this string may be empty";   
- Class[] groups() default { };   
- Classextends Payload>[] payload() default {};   
- } 
约束注解定义完成后，需要同时实现与该约束注解关联的验证器。约束验证器的实现需要扩展 JSR303 规范提供的接口 javax.validation.ConstraintValidator。清单 5 给出该接口。
清单 5：
- publicinterface ConstraintValidator<a < span="">extends Annotation, T> {   
- void initialize(A constraintAnnotation);   
- boolean isValid(T value, ConstraintValidatorContext context);   
- }  
该接口有两个方法，方法 initialize 对验证器进行实例化，它必须在验证器的实例在使用之前被调用，并保证正确初始化验证器，它的参数是约束注解;方法 isValid 是进行约束验证的主体方法，其中 value 参数代表需要验证的实例，context 参数代表约束执行的上下文环境。
对于清单 4 定义的约束注解，清单 6 给出了与该注解对应的验证器的实现。
清单 6：
- publicclass NotEmptyValidator implements ConstraintValidator{   
- publicvoid initialize(NotEmpty parameters) {   
- }   
- publicboolean isValid(String string,   
-    ConstraintValidatorContext constraintValidatorContext) {   
- if (string == null) returnfalse;   
- elseif(string.length()<1) returnfalse;   
- elsereturntrue;   
- }   
- }   
至此，一个可以声明并使用的约束注解已经定义完毕，清单 7 将给出该约束注解在实际程序中的使用。为节省篇幅，这里只给出针对清单 1 的增加和修改内容，未给出全部的示例代码，您可以在本文的附录中获得全部的代码。
清单 7：
首先在清单 1 中的类 Employee 中加入字段 company 和相应的 getter 和 setter 方法：
- @NotEmpty
- private String company; 
然后在 main 函数中加入如下代码清单：
- String company = new String();  
- employee.setCompany(company);  
再次运行该程序，输出结果为：
- The id of employee can not be null
- this string may be empty  
- The size of employee's name must between 1 and 10
多值约束
下面介绍 Bean Validation 规范的一个特性，多值约束(Multiple Constraints)：对于同一个目标元素，在进行约束注解声明时可以同时使用不同的属性达到对该目标元素进行多值验证的目的。如清单 8 所示：
清单 8：
- public@interface ConstraintName{   
- String message() default" ";   
- Class[] groups() default { };   
- Classextends Payload>[] payload() default { };   
- @Target({ METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER })   
- @Retention(RUNTIME)   
- @Documented
- @interface List {   
- ConstraintName[] value();   
- }   
- }   
实现多值约束只需要在定义约束注解的同时定义一个 List(@interface List{})。使用该约束注解时，Bean Validation 将 value 数组里面的每一个元素都处理为一个普通的约束注解，并对其进行验证，所有约束条件均符合时才会验证通过。
清单 9 定义了一个约束注解，它用来验证某一字符串是否包含指定的内容。
清单 9：
- @Target({ METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER })   
- @Retention(RUNTIME)   
- @Documented
- @Constraint(validatedBy = PatternOfStringValidator.class)   
- public@interface PatternOfString {   
- String mustContainLetter();   
- String message() default"this pattern may not be right";   
- Class[] groups() default { };   
- Classextends Payload>[] payload() default {};   
- 
- @Target({ METHOD, FIELD, ANNOTATION_TYPE})   
- @Retention(RUNTIME)   
- @interface List {   
- PatternOfString[] value();   
- }   
- }   
该约束注解对应的验证器如清单 10 所示：
清单 10：
- publicclass PatternOfStringValidator implements ConstraintValidator  
-  {   
- private String letterIn;   
- publicvoid initialize(PatternOfString parameters) {   
- this.letterIn=parameters.mustContainLetter();   
- }   
- publicboolean isValid(String string,   
- ConstraintValidatorContext constraintValidatorContext) {   
- if (string.contains(letterIn))   
- returntrue;   
- returnfalse;   
- }   
- }   
如果想验证某一字符串是否同时包含两个子串，那么多值约束就显得比较重要了，清单 11 将详细给出多值约束的使用。
清单 11：
在清单 1 中的类 Employee 中增加如下字段 place 以及相应的 getter 和 setter 方法：
- @PatternOfString.List({  
- @PatternOfString(mustContainLetter = "CH",  
- message = "It does not belong to China"),  
- @PatternOfString(mustContainLetter="MainLand",  
- message="It does not belong to MainLand")})  
- private String place; 
然后在 main 函数中加入如下代码清单：
- String place = "C";  
- employee.setPlace(place);  
再次运行该程序，输出结果为：
- It does not belong to MainLand  
- It does not belong to China  
- this string may be empty  
- The id of employee can not be null
- The size of employee's name must between 1 and 10
如果将 place 赋值为 String place = "CHINA"，则输出结果为：
- this string may be empty  
- The id of employee can not be null
- It does not belong to MainLand  
- The size of employee's name must between 1 and 10
可见，该约束会对声明的两个约束注解分别进行验证，只要存在不符合约束验证规则的 Java Bean 实例，就将产生相应的验证失败信息。约束注解声明的时候可以根据不同的约束值使用 message 参数给出不同的输出信息。
组合约束
下面介绍 Bean Validation 规范中另一个重要的特性：组合约束。Bean Validation 规范允许将不同的约束进行组合来创建级别较高且功能较多的约束，从而避免原子级别约束的重复使用。如清单 4 定义的约束注解 @NotEmpty，是用来判断一个字符串在非空的基础上长度至少为 1，其实际意义等同于 @NotNull 和 @Size(min=1)的组合形式，因此可以将 @NotEmpty 约束定义为组合约束 NotEmpty2，如清单 12 所示：
清单 12：
- @NotNull
- @Size(min = 1)   
- @Target({ METHOD, FIELD, ANNOTATION_TYPE, CONSTRUCTOR, PARAMETER })   
- @Retention(RUNTIME)   
- @Documented
- @Constraint(validatedBy = {NotEmptyValidator2.class})   
- public@interface NotEmpty2 {   
- String message() default"this string may be empty";   
- Class[] groups() default { };   
- Classextends Payload>[] payload() default {};   
- 
- @Target({ METHOD, FIELD, ANNOTATION_TYPE})   
- @Retention(RUNTIME)   
- @interface List {   
- NotEmpty2[] value();   
- }   
- }   
实际使用中 @NotEmpty2 约束注解可以得到与 @NotEmpty 约束注解同样的验证结果。
