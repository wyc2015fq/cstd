# Spring Validation Example – Spring MVC Form Validator - 零度的博客专栏 - CSDN博客
2017年04月06日 10:42:30[零度anngle](https://me.csdn.net/zmx729618)阅读数：903
                
When we accept user inputs in any web application, it become necessary to validate them. We can validate the user input at client side using JavaScript but it’s also necessary to validate them at server side to make sure we are processing valid data incase
 user has javascript disabled.
## Spring Validation
![spring mvc validation example, spring validator, hibernate validator](http://cdn.journaldev.com/wp-content/uploads/2014/04/spring-validation-mvc-validator-hibernate-validator.jpg)
[Spring MVC Framework](http://www.journaldev.com/2433/spring-mvc-tutorial) supports JSR-303 specs by
 default and all we need is to add JSR-303 and it’s implementation dependencies in Spring MVC application. Spring also provides `@Validator` annotation and `BindingResult` class
 through which we can get the errors raised by Validator implementation in the controller request handler method.
We can create our custom validator implementations by two ways – first one is to create an annotation that confirms to the JSR-303 specs and implement it’s Validator class. Second approach is to implement the `org.springframework.validation.Validator` interface
 and add set it as validator in the Controller class using `@InitBinder` annotation.
Let’s create a simple Spring MVC project in Spring Tool Suite where we will use JSR-303 specs with it’s implementation artifact hibernate-validator. We will use annotation based form validation and create our own custom
 validator based on JSR-303 specs standards. We will also create our own custom validator class by implementing `Validator` interface and use it in one of the controller
 handler methods. Our final project looks like below image.
![Spring Validation Example](http://cdn.journaldev.com/wp-content/uploads/2014/04/Spring-MVC-Form-Validation-Project-266x450.png)
Let’s look at each of the components one by one.
## Spring MVC Form Validator
Our final pom.xml file looks like below. Apart from standard Spring MVC artifacts, we have validation-api and hibernate-validator dependencies in the project.
<?xml version="1.0" encoding="UTF-8"?><projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd"><modelVersion>4.0.0</modelVersion><groupId>com.journaldev</groupId><artifactId>spring</artifactId><name>SpringFormValidation</name><packaging>war</packaging><version>1.0.0-BUILD-SNAPSHOT</version><properties><java-version>1.7</java-version><org.springframework-version>4.0.2.RELEASE</org.springframework-version><org.aspectj-version>1.7.4</org.aspectj-version><org.slf4j-version>1.7.5</org.slf4j-version></properties><dependencies><!-- Form Validation using Annotations --><dependency><groupId>javax.validation</groupId><artifactId>validation-api</artifactId><version>1.1.0.Final</version></dependency><dependency><groupId>org.hibernate</groupId><artifactId>hibernate-validator</artifactId><version>4.1.0.Final</version></dependency><!-- Spring --><dependency><groupId>org.springframework</groupId><artifactId>spring-context</artifactId><version>${org.springframework-version}</version><exclusions><!-- Exclude Commons Logging in favor of SLF4j --><exclusion><groupId>commons-logging</groupId><artifactId>commons-logging</artifactId></exclusion></exclusions></dependency><dependency><groupId>org.springframework</groupId><artifactId>spring-webmvc</artifactId><version>${org.springframework-version}</version></dependency><!-- AspectJ --><dependency><groupId>org.aspectj</groupId><artifactId>aspectjrt</artifactId><version>${org.aspectj-version}</version></dependency><!-- Logging --><dependency><groupId>org.slf4j</groupId><artifactId>slf4j-api</artifactId><version>${org.slf4j-version}</version></dependency><dependency><groupId>org.slf4j</groupId><artifactId>jcl-over-slf4j</artifactId><version>${org.slf4j-version}</version><scope>runtime</scope></dependency><dependency><groupId>org.slf4j</groupId><artifactId>slf4j-log4j12</artifactId><version>${org.slf4j-version}</version><scope>runtime</scope></dependency><dependency><groupId>log4j</groupId><artifactId>log4j</artifactId><version>1.2.15</version><exclusions><exclusion><groupId>javax.mail</groupId><artifactId>mail</artifactId></exclusion><exclusion><groupId>javax.jms</groupId><artifactId>jms</artifactId></exclusion><exclusion><groupId>com.sun.jdmk</groupId><artifactId>jmxtools</artifactId></exclusion><exclusion><groupId>com.sun.jmx</groupId><artifactId>jmxri</artifactId></exclusion></exclusions><scope>runtime</scope></dependency><!-- @Inject --><dependency><groupId>javax.inject</groupId><artifactId>javax.inject</artifactId><version>1</version></dependency><!-- Servlet --><dependency><groupId>javax.servlet</groupId><artifactId>servlet-api</artifactId><version>2.5</version><scope>provided</scope></dependency><dependency><groupId>javax.servlet.jsp</groupId><artifactId>jsp-api</artifactId><version>2.1</version><scope>provided</scope></dependency><dependency><groupId>javax.servlet</groupId><artifactId>jstl</artifactId><version>1.2</version></dependency><!-- Test --><dependency><groupId>junit</groupId><artifactId>junit</artifactId><version>4.7</version><scope>test</scope></dependency></dependencies><build><plugins><plugin><artifactId>maven-eclipse-plugin</artifactId><version>2.9</version><configuration><additionalProjectnatures><projectnature>org.springframework.ide.eclipse.core.springnature</projectnature></additionalProjectnatures><additionalBuildcommands><buildcommand>org.springframework.ide.eclipse.core.springbuilder</buildcommand></additionalBuildcommands><downloadSources>true</downloadSources><downloadJavadocs>true</downloadJavadocs></configuration></plugin><plugin><groupId>org.apache.maven.plugins</groupId><artifactId>maven-compiler-plugin</artifactId><version>2.5.1</version><configuration><source>1.6</source><target>1.6</target><compilerArgument>-Xlint:all</compilerArgument><showWarnings>true</showWarnings><showDeprecation>true</showDeprecation></configuration></plugin><plugin><groupId>org.codehaus.mojo</groupId><artifactId>exec-maven-plugin</artifactId><version>1.2.1</version><configuration><mainClass>org.test.int1.Main</mainClass></configuration></plugin></plugins></build></project>
![](http://static.blog.csdn.net/images/save_snippets.png)
### Deployment Descriptor
When you create Spring MVC project from STS, it creates two context configuration files. I have cleaned it up a bit and have only one spring bean configuration file. My final web.xml file looks like below.
<?xml version="1.0" encoding="UTF-8"?><web-appversion="2.5"xmlns="http://java.sun.com/xml/ns/javaee"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"><!-- Processes application requests --><servlet><servlet-name>appServlet</servlet-name><servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class><init-param><param-name>contextConfigLocation</param-name><param-value>/WEB-INF/spring/spring.xml</param-value></init-param><load-on-startup>1</load-on-startup></servlet><servlet-mapping><servlet-name>appServlet</servlet-name><url-pattern>/</url-pattern></servlet-mapping></web-app>
![](http://static.blog.csdn.net/images/save_snippets.png)
### Spring Bean Configuration File
Usually we look into spring wirings at the last, but this time we don’t have much configurations in the spring bean configuration file. Our final spring.xml file looks like below.
<?xml version="1.0" encoding="UTF-8"?><beans:beansxmlns="http://www.springframework.org/schema/mvc"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:beans="http://www.springframework.org/schema/beans"xmlns:context="http://www.springframework.org/schema/context"xsi:schemaLocation="http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd
		http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
		http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd"><!-- DispatcherServlet Context: defines this servlet's request-processing infrastructure --><!-- Enables the Spring MVC @Controller programming model --><annotation-driven/><!-- Handles HTTP GET requests for /resources/** by efficiently serving up static resources in the ${webappRoot}/resources directory --><resourcesmapping="/resources/**"location="/resources/"/><!-- Resolves views selected for rendering by @Controllers to .jsp resources in the /WEB-INF/views directory --><beans:beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver"><beans:propertyname="prefix"value="/WEB-INF/views/"/><beans:propertyname="suffix"value=".jsp"/></beans:bean><beans:beanid="employeeValidator"class="com.journaldev.spring.form.validator.EmployeeFormValidator"/><beans:beanid="messageSource"class="org.springframework.context.support.ReloadableResourceBundleMessageSource"><beans:propertyname="basename"value="classpath:message"/><beans:propertyname="defaultEncoding"value="UTF-8"/></beans:bean><context:component-scanbase-package="com.journaldev.spring"/></beans:beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
The only important point to note are `employeeValidator` bean that we will inject into one of the controller and `messageSource` bean
 to read the localized data from resource bundles. Rest of the part is to support annotations, view resolvers and providing package to scan for Controller classes and other components.
### Model Classes
We have two model classes in this project – first one where we will use JSR-303 annotation and our custom annotation based validator and second one where we will use only our Validator implementation.
Customer.java code:
package com.journaldev.spring.form.model;import java.util.Date;import javax.validation.constraints.Max;import javax.validation.constraints.Min;import javax.validation.constraints.NotNull;import javax.validation.constraints.Past;import javax.validation.constraints.Size;import org.hibernate.validator.constraints.Email;import org.hibernate.validator.constraints.NotEmpty;import org.springframework.format.annotation.DateTimeFormat;import com.journaldev.spring.form.validator.Phone;publicclassCustomer{@Size(min=2, max=30)privateString name;@NotEmpty@EmailprivateString email;@NotNull@Min(18)@Max(100)privateInteger age;@NotNullprivateGender gender;@DateTimeFormat(pattern="MM/dd/yyyy")@NotNull@PastprivateDate birthday;@PhoneprivateString phone;publicenumGender{
		MALE, FEMALE
	}publicString getName(){return name;}publicvoid setName(String name){this.name = name;}publicString getEmail(){return email;}publicvoid setEmail(String email){this.email = email;}publicInteger getAge(){return age;}publicvoid setAge(Integer age){this.age = age;}publicGender getGender(){return gender;}publicvoid setGender(Gender gender){this.gender = gender;}publicDate getBirthday(){return birthday;}publicvoid setBirthday(Date birthday){this.birthday = birthday;}publicString getPhone(){return phone;}publicvoid setPhone(String phone){this.phone = phone;}}
![](http://static.blog.csdn.net/images/save_snippets.png)
Notice that we are using @Email, @NotEmpty and @DateTimeFormat annotations that are additional to JSR-303 and provided by hibernate validator implementation. Some of the JSR-303 annotations that we are using are @Size, @NotNull etc.
@Phone annotation used is our custom implementation based on JSR-303 specs, we will look into it in next section.
Employee.java code:
package com.journaldev.spring.form.model;publicclassEmployee{privateint id;privateString name;privateString role;publicint getId(){return id;}publicvoid setId(int id){this.id = id;}publicString getName(){return name;}publicvoid setName(String name){this.name = name;}publicString getRole(){return role;}publicvoid setRole(String role){this.role = role;}}
![](http://static.blog.csdn.net/images/save_snippets.png)
Employee is a standard java bean and we will use our custom Validator implementation to validate the form with Employee bean.
### Custom Validator Implementations
Phone.java code:
package com.journaldev.spring.form.validator;import java.lang.annotation.Documented;import java.lang.annotation.Retention;import java.lang.annotation.Target;import java.lang.annotation.ElementType;import java.lang.annotation.RetentionPolicy;import javax.validation.Constraint;import javax.validation.Payload;@Documented@Constraint(validatedBy =PhoneValidator.class)@Target({ElementType.METHOD,ElementType.FIELD })@Retention(RetentionPolicy.RUNTIME)public@interfacePhone{String message()default"{Phone}";Class<?>[] groups()default{};Class<?extendsPayload>[] payload()default{};}
![](http://static.blog.csdn.net/images/save_snippets.png)
Most of the part is boiler-plate code to confirm with JSR-303 specs. The most important part is @Constraint annotation where we provide the class that will be used for validation i.e `PhoneValidator`.
PhoneValidator.java code:
package com.journaldev.spring.form.validator;import javax.validation.ConstraintValidator;import javax.validation.ConstraintValidatorContext;publicclassPhoneValidatorimplementsConstraintValidator<Phone,String>{@Overridepublicvoid initialize(Phone paramA){}@Overridepublicboolean isValid(String phoneNo,ConstraintValidatorContext ctx){if(phoneNo ==null){returnfalse;}//validate phone numbers of format "1234567890"if(phoneNo.matches("\\d{10}"))returntrue;//validating phone number with -, . or spaceselseif(phoneNo.matches("\\d{3}[-\\.\\s]\\d{3}[-\\.\\s]\\d{4}"))returntrue;//validating phone number with extension length from 3 to 5elseif(phoneNo.matches("\\d{3}-\\d{3}-\\d{4}\\s(x|(ext))\\d{3,5}"))returntrue;//validating phone number where area code is in braces ()elseif(phoneNo.matches("\\(\\d{3}\\)-\\d{3}-\\d{4}"))returntrue;//return false if nothing matches the inputelsereturnfalse;}}
![](http://static.blog.csdn.net/images/save_snippets.png)
Our JSR-303 specs validator implementation should implement `javax.validation.ConstraintValidator`interface. If we are using some resource such as DataSource, we
 can initialize them in the `initialize()`method. The validation method is `isValid` and
 it returns true if data is valid else it should return false.
If you are new to regular expressions, you can read more about it at [Java
 Regular Expressions Tutorial](http://www.journaldev.com/634/regular-expression-in-java-regex-example).
EmployeeFormValidator.java class code:
package com.journaldev.spring.form.validator;import org.springframework.validation.Errors;import org.springframework.validation.ValidationUtils;import org.springframework.validation.Validator;import com.journaldev.spring.form.model.Employee;publicclassEmployeeFormValidatorimplementsValidator{//which objects can be validated by this validator@Overridepublicboolean supports(Class<?> paramClass){returnEmployee.class.equals(paramClass);}@Overridepublicvoid validate(Object obj,Errors errors){ValidationUtils.rejectIfEmptyOrWhitespace(errors,"id","id.required");Employee emp =(Employee) obj;if(emp.getId()<=0){
			errors.rejectValue("id","negativeValue",newObject[]{"'id'"},"id can't be negative");}ValidationUtils.rejectIfEmptyOrWhitespace(errors,"name","name.required");ValidationUtils.rejectIfEmptyOrWhitespace(errors,"role","role.required");}}
![](http://static.blog.csdn.net/images/save_snippets.png)
EmployeeFormValidator is the validator implementation that is specific to Spring Framework. `supports()`method implementation by Spring Framework to know objects
 on which this validation can be used.
We implement `validate()` method and add errors if any field validation fails. Spring provides `org.springframework.validation.ValidationUtils` utility
 class for basic validations such as null or empty. Once this method returns, spring framework binds the Errors object to the BindingResult object that we use in our controller handler method.
Notice that `ValidationUtils.rejectIfEmptyOrWhitespace()` last argument takes the key name for message resources. This way we can provide localized error messages
 to the user. For more information about i18n in Spring, read [Spring
 i18n Example](http://www.journaldev.com/2610/spring-mvc-internationalization-i18n-and-localization-l10n-example).
### Controller Classes
We have two controller classes, one for annotation based form validation and another for our custom validator.
CustomerController.java class code:
package com.journaldev.spring.form.controllers;import java.util.HashMap;import java.util.Map;import javax.validation.Valid;import org.slf4j.Logger;import org.slf4j.LoggerFactory;import org.springframework.stereotype.Controller;import org.springframework.ui.Model;import org.springframework.validation.BindingResult;import org.springframework.web.bind.annotation.RequestMapping;import org.springframework.web.bind.annotation.RequestMethod;import com.journaldev.spring.form.model.Customer;@ControllerpublicclassCustomerController{privatestaticfinalLogger logger =LoggerFactory.getLogger(CustomerController.class);privateMap<String,Customer> customers =null;publicCustomerController(){
		customers =newHashMap<String,Customer>();}@RequestMapping(value ="/cust/save", method =RequestMethod.GET)publicString saveCustomerPage(Model model){
		logger.info("Returning custSave.jsp page");
		model.addAttribute("customer",newCustomer());return"custSave";}@RequestMapping(value ="/cust/save.do", method =RequestMethod.POST)publicString saveCustomerAction(@ValidCustomer customer,BindingResult bindingResult,Model model){if(bindingResult.hasErrors()){
			logger.info("Returning custSave.jsp page");return"custSave";}
		logger.info("Returning custSaveSuccess.jsp page");
		model.addAttribute("customer", customer);
		customers.put(customer.getEmail(), customer);return"custSaveSuccess";}}
![](http://static.blog.csdn.net/images/save_snippets.png)
When we use annotation based form validation, we just need to make little changes in our controller handler method implementation to get it working.
First we need to annotate model object that we want to validate with `@Valid` annotation. Then we need to have BindingResult argument in the method, spring takes
 care of populating it with error messages. The handler method logic is very simple, if there are any errors we are responding with the same page or else we are redirecting user to the success page.
Another important point to note is that we are adding "customer" attribute to the model, this is necessary to let Spring framework know which model object to use in the form page. If we won't do it, object binding to form data will not take place and our form
 validation will not work.
EmployeeController.java class code:
package com.journaldev.spring.form.controllers;import java.util.HashMap;import java.util.Map;import org.slf4j.Logger;import org.slf4j.LoggerFactory;import org.springframework.beans.factory.annotation.Autowired;import org.springframework.beans.factory.annotation.Qualifier;import org.springframework.stereotype.Controller;import org.springframework.ui.Model;import org.springframework.validation.BindingResult;import org.springframework.validation.Validator;import org.springframework.validation.annotation.Validated;import org.springframework.web.bind.WebDataBinder;import org.springframework.web.bind.annotation.InitBinder;import org.springframework.web.bind.annotation.ModelAttribute;import org.springframework.web.bind.annotation.RequestMapping;import org.springframework.web.bind.annotation.RequestMethod;import com.journaldev.spring.form.model.Employee;@ControllerpublicclassEmployeeController{privatestaticfinalLogger logger =LoggerFactory.getLogger(EmployeeController.class);privateMap<Integer,Employee> emps =null;@Autowired@Qualifier("employeeValidator")privateValidator validator;@InitBinderprivatevoid initBinder(WebDataBinder binder){
		binder.setValidator(validator);}publicEmployeeController(){
		emps =newHashMap<Integer,Employee>();}@ModelAttribute("employee")publicEmployee createEmployeeModel(){// ModelAttribute value should be same as used in the empSave.jspreturnnewEmployee();}@RequestMapping(value ="/emp/save", method =RequestMethod.GET)publicString saveEmployeePage(Model model){
		logger.info("Returning empSave.jsp page");return"empSave";}@RequestMapping(value ="/emp/save.do", method =RequestMethod.POST)publicString saveEmployeeAction(@ModelAttribute("employee")@ValidatedEmployee employee,BindingResult bindingResult,Model model){if(bindingResult.hasErrors()){
			logger.info("Returning empSave.jsp page");return"empSave";}
		logger.info("Returning empSaveSuccess.jsp page");
		model.addAttribute("emp", employee);
		emps.put(employee.getId(), employee);return"empSaveSuccess";}}
![](http://static.blog.csdn.net/images/save_snippets.png)
For using custom validator, first we need to inject it in the controller class. We are using [spring
 bean auto wiring](http://www.journaldev.com/2623/spring-autowired-annotation) to achieve this using `@Autowired` and `@Qualifier` annotations.
Next we need to have a method that will take WebDataBinder as argument and we set our custom validator to be used. This method should be annotated with `@InitBinder` annotation.
Using `@ModelAttribute` is another way to add our bean object to the Model. Rest of the code is similar to customer controller implementation.
### Form Validation Error Messages Resource Bundle
It's time to look at our resource bundle where we have different types of messages to be used for validation errors.
message_en.properties file:
#application defined error messsages
id.required=Employee ID is required
name.required=EmployeeNameis required
role.required=EmployeeRoleis required
negativeValue={0} can't be negative or zero
#Spring framework error messages to be used when conversion from form data to bean fails
typeMismatch.int={0} Value must be an integer
typeMismatch.java.lang.Integer={0} must be an integer
typeMismatch={0} is of invalid format
#application messages for annotations, {ValidationClass}.{modelObjectName}.{field}
#the {0} is field name, other fields are in alphabatical order, max and then min  
Size.customer.name=Customer {0} should be between {2} and {1} characters long
NotEmpty.customer.email=Email is a required field
NotNull.customer.age=Customer {0} should be in years
#Generic annotation class messages
Email=Email address is not valid
NotNull=This is a required field
NotEmpty=This is a required field
Past=Date should be Past
#Custom validation annotation
Phone=Invalid format, valid formats are 1234567890, 123-456-7890 x1234
![](http://static.blog.csdn.net/images/save_snippets.png)
I have provided message key details in the comment itself, so I will skip them here. The only important point to note here is the way messages will be looked up, first key name {ValidationClass}.{modelObjectName}.{field} is looked up and if that is not found
 then {ValidationClass}.{modelObjectName} is looked up. If that is missing, then finally {ValidationClass} key is looked up. If nothing is found then the default message provided will be returned.
Read more about resource messages at [Spring
 Localization Example](http://www.journaldev.com/2610/spring-mvc-internationalization-i18n-and-localization-l10n-example).
### View Pages with Form and Errors
Since we are using Spring framework validation implementation, we will have to use Spring Form tags to get the errors and set the form bean and variable names.
Our custSave.jsp file code is given below.
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"><%@ taglib uri="http://www.springframework.org/tags/form"
	prefix="springForm"%>
<html><head><metahttp-equiv="Content-Type"content="text/html; charset=UTF-8"><title>Customer Save Page</title><style>.error {
	color:#ff0000;
	font-style: italic;
	font-weight: bold;}</style></head><body><springForm:formmethod="POST"commandName="customer"action="save.do"><table><tr><td>Name:</td><td><springForm:inputpath="name"/></td><td><springForm:errorspath="name"cssClass="error"/></td></tr><tr><td>Email:</td><td><springForm:inputpath="email"/></td><td><springForm:errorspath="email"cssClass="error"/></td></tr><tr><td>Age:</td><td><springForm:inputpath="age"/></td><td><springForm:errorspath="age"cssClass="error"/></td></tr><tr><td>Gender:</td><td><springForm:selectpath="gender"><springForm:optionvalue=""label="Select Gender"/><springForm:optionvalue="MALE"label="Male"/><springForm:optionvalue="FEMALE"label="Female"/></springForm:select></td><td><springForm:errorspath="gender"cssClass="error"/></td></tr><tr><td>Birthday:</td><td><springForm:inputpath="birthday"placeholder="MM/dd/yyyy"/></td><td><springForm:errorspath="birthday"cssClass="error"/></td></tr><tr><td>Phone:</td><td><springForm:inputpath="phone"/></td><td><springForm:errorspath="phone"cssClass="error"/></td></tr><tr><tdcolspan="3"><inputtype="submit"value="Save Customer"></td></tr></table></springForm:form></body></html>
![](http://static.blog.csdn.net/images/save_snippets.png)
`commandName="customer"` is used to set the name of the model attribute under which form object is exposed. It's default value is "command" by default, hence we
 should set it to the model attribute name we are using in our controller classes.
`springForm:errors` is used to render the errors, if any, found when the page is rendered. `path` attribute
 is used to define the object property to be used for data binding. Rest of the code is standard HTML with some CSS for error messages styling.
Our custSaveSuccess.jsp file is given below.
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %>
<%@ page session="false" %>
<html><head><title>Customer Saved Successfully</title></head><body><h3>
	Customer Saved Successfully.
</h3><strong>Customer Name:${customer.name}</strong><br><strong>Customer Email:${customer.email}</strong><br><strong>Customer Age:${customer.age}</strong><br><strong>Customer Gender:${customer.gender}</strong><br><strong>Customer Birthday:<fmt:formatDatevalue="${customer.birthday}"type="date"/></strong><br></body></html>
![](http://static.blog.csdn.net/images/save_snippets.png)
Simple JSP page showing the customer values if there are no validation errors and this page is returned as response. It's name is empSave.jsp.
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd"><%@ taglib uri="http://www.springframework.org/tags/form"
	prefix="springForm"%>
<html><head><metahttp-equiv="Content-Type"content="text/html; charset=UTF-8"><title>Employee Save Page</title><style>.error {
	color:#ff0000;
	font-style: italic;
	font-weight: bold;}</style></head><body><springForm:formmethod="POST"commandName="employee"action="save.do"><table><tr><td>Employee ID:</td><td><springForm:inputpath="id"/></td><td><springForm:errorspath="id"cssClass="error"/></td></tr><tr><td>Employee Name:</td><td><springForm:inputpath="name"/></td><td><springForm:errorspath="name"cssClass="error"/></td></tr><tr><td>Employee Role:</td><td><springForm:selectpath="role"><springForm:optionvalue=""label="Select Role"/><springForm:optionvalue="ceo"label="CEO"/><springForm:optionvalue="developer"label="Developer"/><springForm:optionvalue="manager"label="Manager"/></springForm:select></td><td><springForm:errorspath="role"cssClass="error"/></td></tr><tr><tdcolspan="3"><inputtype="submit"value="Save"></td></tr></table></springForm:form></body></html>
![](http://static.blog.csdn.net/images/save_snippets.png)
empSaveSuccess.jsp file:
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page session="false" %>
<html><head><title>Employee Saved Successfully</title></head><body><h3>
	Employee Saved Successfully.
</h3><strong>Employee ID:${emp.id}</strong><br><strong>Employee Name:${emp.name}</strong><br><strong>Employee Role:${emp.role}</strong><br></body></html>
![](http://static.blog.csdn.net/images/save_snippets.png)
### Test the Spring MVC Form Validation Application
Our application is ready to deploy and run some tests, deploy it in your favorite servlet container. I am using Apache Tomcat 7 and below images show some of the pages with validation error messages. Based on your input data, you might get different error messages
 too.
![Spring Validator Example](http://cdn.journaldev.com/wp-content/uploads/2014/04/Spring-MVC-Form-Validation-Annotation-1-450x215.png)
![Spring Validation Example](http://cdn.journaldev.com/wp-content/uploads/2014/04/Spring-MVC-Form-Validation-Annotation-2-450x207.png)
![Spring Form Validation example](http://cdn.journaldev.com/wp-content/uploads/2014/04/Spring-MVC-Form-Validation-450x198.png)
That's all for Spring MVC Form validation with different ways and using resource bundles for localized error messages. You can download the sample project from below link and play around with it to learn more.
