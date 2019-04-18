# 创建自定义JSR303的验证约束(Creating custom constraints) - z69183787的专栏 - CSDN博客
2014年10月22日 15:36:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1439

     由于输入验证在软件开发中是必须的一件事情，特别是与用户交互的软件产品，验证用户的潜在输入错误是必不可少的一件事情，然而各种开源的验证框架也很多，为了一统标准，jsr303规范横空出世了，它定义了一些标准的验证约束，标准毕竟是标准，它不可能定义到所有的验证约束，它只是提供了一些基本的常用的约束，不过它提供了一个可拓展的自定义验证约束。下面就来说说怎么样自定义一个约束.
      为了创建一个自定义约束，以下三个步骤是必须的。
• Create a constraint annotation (首先定义一个约束注解)
• Implement a validator(第二步是实现这个验证器)
• Define a default error message(最后添加一条默认的错误消息即可)
    假定有这么一个要求，要验证用户的两次输入密码必须是相同的，非常常见的一个要求。下面就基于这个要求来自定义一个约束。
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- package org.leochen.samples;  
- 
- import javax.validation.Constraint;  
- import javax.validation.Payload;  
- import java.lang.annotation.*;  
- 
- /**
-  * User: leochen
-  * Date: 11-12-8
-  * Time: 下午11:31
-  */
- 
- @Target({ElementType.TYPE,ElementType.ANNOTATION_TYPE})  
- @Retention(RetentionPolicy.RUNTIME)  
- @Constraint(validatedBy = MatchesValidator.class)  
- @Documented
- public@interface Matches {  
-     String message() default"{constraint.not.matches}";  
-     Class<?>[] groups() default {};  
-     Class<? extends Payload>[] payload() default {};  
- 
-     String field();  
-     String verifyField();  
- }  
 从上到下来说吧，@Target表示注解可出现在哪些地方，比如可以出现在class上,field,method,又或者是在另外一个annotation上，这里限制只能出现在类和另外一个注解上，@Retention表示该注解的保存范围是哪里,RUNTIME表示在源码(source)、编译好的.class文件中保留信息，在执行的时候会把这一些信息加载到JVM中去的．@Constraint比较重要，表示哪个验证器提供验证。@interface表明这是一个注解,和class一样都是关键字,message(),groups()和payload()这三个方法是一个标准的约束所具备的，其中message()是必须的，{constraint.not.matches}表示该消息是要插值计算的，也就是说是要到资源文件中寻找这个key的，如果不加{}就表示是一个普通的消息，直接文本显示,如果消息中有需要用到{或}符号的，需要进行转义,用\{和\}来表示。groups()表示该约束属于哪个验证组,在验证某个bean部分属性是特别有用(也说不清了，具体可以查看Hibernate
 Validator的文档细看) default必须是一个类型为Class<?>[]的空数组,attribute payload that can be used by clients of the Bean Validation API to assign custom payload objects to a constraint. This attribute is not used by the API itself.下面连个字段是我们添加进去的，表示要验证字段的名称，比如password和confirmPassword.
    下面就来实现这个约束。
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- package org.leochen.samples;  
- 
- import org.apache.commons.beanutils.BeanUtils;  
- 
- import javax.validation.ConstraintValidator;  
- import javax.validation.ConstraintValidatorContext;  
- import java.lang.reflect.InvocationTargetException;  
- 
- /**
-  * User: leochen
-  * Date: 11-12-8
-  * Time: 下午11:39
-  */
- publicclass MatchesValidator implements ConstraintValidator<Matches,Object>{  
- private String field;  
- private String verifyField;  
- 
- publicvoid initialize(Matches matches) {  
- this.field = matches.field();  
- this.verifyField = matches.verifyField();  
-     }  
- 
- publicboolean isValid(Object value, ConstraintValidatorContext context) {  
- try {  
-             String fieldValue= BeanUtils.getProperty(value,field);  
-             String verifyFieldValue = BeanUtils.getProperty(value,verifyField);  
- boolean valid = (fieldValue == null) && (verifyFieldValue == null);  
- if(valid){  
- returntrue;  
-             }  
- 
- boolean match = (fieldValue!=null) && fieldValue.equals(verifyFieldValue);  
- if(!match){  
-                 String messageTemplate = context.getDefaultConstraintMessageTemplate();  
-                 context.disableDefaultConstraintViolation();  
-                 context.buildConstraintViolationWithTemplate(messageTemplate)  
-                         .addNode(verifyField)  
-                         .addConstraintViolation();  
-             }  
- return match;  
-         } catch (IllegalAccessException e) {  
-             e.printStackTrace();  
-         } catch (InvocationTargetException e) {  
-             e.printStackTrace();  
-         } catch (NoSuchMethodException e) {  
-             e.printStackTrace();  
-         }  
- returntrue;  
-     }  
- }  
 我们必须要实现ConstraintValidator这个接口,下面就来具体看看这个接口是怎么定义的吧：
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- package javax.validation;  
- 
- import java.lang.annotation.Annotation;  
- 
- publicinterface ConstraintValidator<A extends Annotation, T> {  
- /**
-      * Initialize the validator in preparation for isValid calls.
-      * The constraint annotation for a given constraint declaration
-      * is passed.
-      * <p/>
-      * This method is guaranteed to be called before any use of this instance for
-      * validation.
-      *
-      * @param constraintAnnotation annotation instance for a given constraint declaration
-      */
- void initialize(A constraintAnnotation);  
- 
- /**
-      * Implement the validation logic.
-      * The state of <code>value</code> must not be altered.
-      *
-      * This method can be accessed concurrently, thread-safety must be ensured
-      * by the implementation.
-      *
-      * @param value object to validate
-      * @param context context in which the constraint is evaluated
-      *
-      * @return false if <code>value</code> does not pass the constraint
-      */
- boolean isValid(T value, ConstraintValidatorContext context);  
- }  
 A 表示边界范围为java.lang.annotation.Annotation即可,这个T参数必须满足下面两个限制条件：
<!-- Generated by javadoc (build 1.6.0_20) on Fri Jun 04 05:41:40 PDT 2010 -->
<noscript></noscript>
- T must resolve to a non parameterized type (T 必须能被解析为非参数化的类型,通俗讲就是要能解析成具体类型，比如Object,Dog,Cat之类的，不能是一个占位符)
- or generic parameters of T must be unbounded wildcard types(或者也可以是一个无边界范围含有通配符的泛型类型)
我们在`**[initialize](http://clongjava.iteye.com/javax/validation/ConstraintValidator.html#initialize%28A%29)**([A](http://clongjava.iteye.com/javax/validation/ConstraintValidator.html) constraintAnnotation)` 方法中获取到要验证的两个字段的名称，在isValid方法中编写验证规则。
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- String fieldValue= BeanUtils.getProperty(value,field);  
- String verifyFieldValue = BeanUtils.getProperty(value,verifyField);  
 通过反射获取验证字段的值，由于我们要实现的是一个密码和确认密码一致的问题，而这两个字段类型都是java.lang.String类型，所以我们直接通过BeanUtils来获取他们各自的值。
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- String messageTemplate = context.getDefaultConstraintMessageTemplate();  
-                 context.disableDefaultConstraintViolation();  
-                 context.buildConstraintViolationWithTemplate(messageTemplate)  
-                         .addNode(verifyField)  
-                         .addConstraintViolation();  
 以上是我们把验证出错的消息放在哪个字段上显示，一般我们是在确认密码上显示密码不一致的消息。
好了这样我们的自定义约束就完成了，下面来使用并测试吧。
假如我们要验证这么一个formbean
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- package org.leochen.samples;  
- 
- /**
-  * User: leochen
-  * Date: 11-12-20
-  * Time: 下午4:04
-  */
- @Matches(field = "password", verifyField = "confirmPassword",  
-                  message = "{constraint.confirmNewPassword.not.match.newPassword}")  
- publicclass TwoPasswords {  
- private String password;  
- private String confirmPassword;  
- 
- public String getPassword() {  
- return password;  
-     }  
- 
- publicvoid setPassword(String password) {  
- this.password = password;  
-     }  
- 
- public String getConfirmPassword() {  
- return confirmPassword;  
-     }  
- 
- publicvoid setConfirmPassword(String confirmPassword) {  
- this.confirmPassword = confirmPassword;  
-     }  
- }    
 在路径下放入我们的资源文件：ValidationMessages.properties（名字必须叫这个，不然你就费好大一番劲，何苦呢是不是，基于约定来）
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- javax.validation.constraints.AssertFalse.message = must be false
- javax.validation.constraints.AssertTrue.message  = must be true
- javax.validation.constraints.DecimalMax.message  = must be less than or equal to {value}  
- javax.validation.constraints.DecimalMin.message  = must be greater than or equal to {value}  
- javax.validation.constraints.Digits.message      = numeric value out of bounds (<{integer} digits>.<{fraction} digits> expected)  
- javax.validation.constraints.Future.message      = must be in the future  
- javax.validation.constraints.Max.message         = must be less than or equal to {value}  
- javax.validation.constraints.Min.message         = must be greater than or equal to {value}  
- javax.validation.constraints.NotNull.message     = may not be null
- javax.validation.constraints.Null.message        = must be null
- javax.validation.constraints.Past.message        = must be in the past  
- javax.validation.constraints.Pattern.message     = must match "{regexp}"
- javax.validation.constraints.Size.message        = size must be between {min} and {max}  
- 
- org.hibernate.validator.constraints.CreditCardNumber.message = invalid credit card number  
- org.hibernate.validator.constraints.Email.message            = not a well-formed email address  
- org.hibernate.validator.constraints.Length.message           = length must be between {min} and {max}  
- org.hibernate.validator.constraints.NotBlank.message         = may not be empty  
- org.hibernate.validator.constraints.NotEmpty.message         = may not be empty  
- org.hibernate.validator.constraints.Range.message            = must be between {min} and {max}  
- org.hibernate.validator.constraints.SafeHtml.message         = may have unsafe html content  
- org.hibernate.validator.constraints.ScriptAssert.message     = script expression "{script}" didn't evaluate to true
- org.hibernate.validator.constraints.URL.message              = must be a valid URL  
- 
- 
- 
- ## custom constraints  
- 
- constraint.not.matches=two fields not matches  
- constraint.confirmNewPassword.not.match.newPassword=two password not the same  
 单元测试如下：
Java代码  ![收藏代码](http://clongjava.iteye.com/images/icon_star.png)
- package org.leochen.samples;  
- 
- import org.junit.BeforeClass;  
- import org.junit.Test;  
- 
- import javax.validation.ConstraintViolation;  
- import javax.validation.Validation;  
- import javax.validation.Validator;  
- import javax.validation.ValidatorFactory;  
- 
- import java.util.Set;  
- 
- importstatic junit.framework.Assert.assertEquals;  
- importstatic junit.framework.Assert.assertNotNull;  
- 
- /**
-  * User: leochen
-  * Date: 11-12-20
-  * Time: 下午4:06
-  */
- publicclass TwoPasswordsTest {  
- privatestatic Validator validator;  
- 
- @BeforeClass
- publicstaticvoid setUp() {  
-         ValidatorFactory factory = Validation.buildDefaultValidatorFactory();  
-         validator = factory.getValidator();  
-     }  
- 
- 
- @Test
- publicvoid testBuildDefaultValidatorFactory() {  
-         ValidatorFactory factory = Validation.buildDefaultValidatorFactory();  
-         Validator validator = factory.getValidator();  
- 
-         assertNotNull(validator);  
-     }  
- 
- @Test
- publicvoid testPasswordEqualsConfirmPassword() {  
-         TwoPasswords bean = new TwoPasswords();  
-         bean.setPassword("110");  
-         bean.setConfirmPassword("110");  
- 
-         Set<ConstraintViolation<TwoPasswords>> constraintViolations = validator.validate(bean);  
- for (ConstraintViolation<TwoPasswords> constraintViolation : constraintViolations) {  
-             System.out.println(constraintViolation.getMessage());  
-         }  
- 
-         assertEquals("newPassword and confirmNewPassword should be the same.", 0, constraintViolations.size());  
-     }  
- 
- @Test
- publicvoid testPasswordNotEqualsConfirmPassword() {  
-         TwoPasswords bean = new TwoPasswords();  
-         bean.setPassword("110");  
-         bean.setConfirmPassword("111");  
- 
-         Set<ConstraintViolation<TwoPasswords>> constraintViolations = validator.validate(bean);  
- 
-         assertEquals(1, constraintViolations.size());  
-         assertEquals("two password not the same", constraintViolations.iterator().next().getMessage());  
-     }  
- 
- @Test
- publicvoid testIfTwoPasswordWereNullShouldPast() {  
-         TwoPasswords bean = new TwoPasswords();  
-         bean.setPassword(null);  
-         bean.setConfirmPassword(null);  
- 
-         Set<ConstraintViolation<TwoPasswords>> constraintViolations = validator.validate(bean);  
- 
-         assertEquals(0, constraintViolations.size());  
-     }  
- 
- @Test
- publicvoid testIfOneIsNullAndOtherIsNotShouldNotPast() {  
-         TwoPasswords bean = new TwoPasswords();  
-         bean.setPassword(null);  
-         bean.setConfirmPassword("110");  
- 
-         Set<ConstraintViolation<TwoPasswords>> constraintViolations = validator.validate(bean);  
- 
-         assertEquals(1, constraintViolations.size());  
-         assertEquals("two password not the same", constraintViolations.iterator().next().getMessage());  
-     }  
- }  
 测试全部通过的
- [CustomConstraintTutorial.7z](http://dl.iteye.com/topics/download/079f9854-7306-32a7-8b48-4878d703710d) (9.4 KB)
- 下载次数: 65
