# Java EE 6核心特征：Bean Validation特性概述(1) - aisoo的专栏 - CSDN博客
2012年12月05日 11:38:26[aisoo](https://me.csdn.net/aisoo)阅读数：309
Java EE 6 提出了 Bean Validation 规范，使用注解的方式对 Java Bean 进行约束验证，不局限于某一层次或者某一编程模型，灵活易用。下边将向您系统的介绍该规范的各种特性。
概述 Bean Validation 规范
Bean 是 Java Bean 的缩写，在 Java 分层架构的实际应用中，从表示层到持久化层，每一层都需要对 Java Bean 进行业务符合性验证，如图 1 所示。然而对于同一个 Java Bean 的对象，在每一层都需要实现同样的验证逻辑时，这将是一项耗时且容易诱发错误的做法。Bean Validation 规范的目标就是避免多层验证的重复性。事实上，开发者更倾向于将验证规则直接放到 Java Bean 本身，使用注解的方式进行验证规则的设计。
![Java 分层验证结构示意图](http://images.51cto.com/files/uploadimg/20110402/1559310.gif)
图 1. Java 分层验证结构示意图
JSR303 规范(Bean Validation 规范)提供了对 Java EE 和 Java SE 中的 Java Bean 进行验证的方式。该规范主要使用注解的方式来实现对 Java Bean 的验证功能，并且这种方式会覆盖使用 XML 形式的验证描述符，从而使验证逻辑从业务代码中分离出来，如图 2 所示。
![Java Bean 验证模型示意图](http://images.51cto.com/files/uploadimg/20110402/1559311.gif)
图 2. Java Bean 验证模型示意图
JSR303 规范提供的 API 是 Java Bean 对象模型的一般扩展，它并不局限于某一层或者某一编程模型，在服务器端和客户端都可使用，其最大的特点就是易用而且灵活。
Hibernate Validator4.0 是 JSR303 规范的参考实现之一，本文所有示例代码均使用该参考实现。
下面给出一个 Bean Validation 的简单示例(清单 1)：
清单 1：
- publicclass Employee {   
- @NotNull(message = "The id of employee can not be null")   
- private Integer id;   
- 
- @NotNull(message = "The name of employee can not be null")   
- @Size(min = 1,max = 10,message="The size of employee's name must between 1 and 10")   
- private String name;   
- 
- publicint getId() {   
- return id;   
-  }   
- publicvoid setId(int id) {   
- this.id = id;   
-  }   
- public String getName() {   
- return name;   
-  }   
- publicvoid setName(String name) {   
- this.name = name;   
-  }   
- publicstaticvoid main(String[] args) {   
-  Employee employee = new Employee();   
-  employee.setName("Zhang Guan Nan");   
-  ValidatorFactory vf = Validation.buildDefaultValidatorFactory();   
-  Validator validator = vf.getValidator();   
-  Set<constraintviolation> set = validator.validate(employee);   
- for (ConstraintViolation constraintViolation : set) {   
-  System.out.println(constraintViolation.getMessage());   
-  }   
-  }   
-  }  
运行该示例的输出结果为：
- The size of employee's name must between 1 and 10
- The id of employee can not be null
从示例中可以看出，Bean Validation 使用注解(@NotNull 和 @Size)的方式对字段 id 和 name 进行了约束声明，当该 Java Bean 被实际使用时，相关的验证器就会对该类的实例进行验证确保其符合该约束声明。完成 Java Bean 的验证通常可分为如下四个步骤：
1. 约束注解的定义
2. 约束验证规则(约束验证器)
3. 约束注解的声明
4. 约束验证流程
本文第二大部分将详细介绍约束注解的定义和约束验证规则;第三大部分将详细介绍约束注解的声明和约束验证流程;第四大部分将介绍 JSR303 规范提供的 API。
