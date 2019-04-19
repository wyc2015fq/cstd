# Java EE 6核心特征：Bean Validation特性概述(3) - aisoo的专栏 - CSDN博客
2012年12月05日 11:37:13[aisoo](https://me.csdn.net/aisoo)阅读数：498
约束的声明和验证流程
本文第二大部分介绍了如何定义约束注解和验证器，本章主要介绍如何在 Java Bean 中应用存在定义的约束注解，主要包括两部分：一是约束的声明;二是约束的验证流程。
在需要进行约束的目标元素前面用注解的方式即可声明约束，这意味着该目标元素必须满足该约束的验证条件。如清单 13 即在字段 id 上声明了约束 @NotNull：
清单 13：
- @NotNull(message = "The id of employee can not be null")   
- private Integer id; 
该目标元素在具体实例中被赋值后，Bean Validation 就会调用相关的流程进行验证。具体使用方式可以参见清单 14 所示，其中所涉及的接口将在本文第四大部分详细介绍。
　清单 14：
- ValidatorFactory vf = Validation.buildDefaultValidatorFactory();   
- Validator validator = vf.getValidator();   
- Set<constraintviolation> set = validator.validate(JavaBeanInstance);   
- for (ConstraintViolation constraintViolation : set) {   
- System.out.println(constraintViolation.getMessage());   
- } 
Bean Validation 规范对 Java Bean 的验证流程如下：在实际使用中调用 Validator.validate(JavaBeanInstance) 方法后，Bean Validation 会查找在 JavaBeanInstance上所有的约束声明，对每一个约束调用对应的约束验证器进行验证，最后的结果由约束验证器的 isValid 方法产生，如果该方法返回 true，则约束验证成功，否则验证失败。验证失败的约束将产生约束违规对象(ConstraintViolation 的实例)并放到约束违规列表中。验证完成后所有的验证失败信息均能在该列表中查找并输出。
前提条件
Bean Validation 规范规定在对 Java Bean 进行约束验证前，目标元素必须满足以下条件：
- 
如果验证的是属性(getter 方法)，那么必须遵从 Java Bean 的命名习惯(JavaBeans 规范);
- 
静态的字段和方法不能进行约束验证;
- 
约束适用于接口和基类;
- 
约束注解定义的目标元素可以是字段、属性或者类型等;
- 
可以在类或者接口上使用约束验证，它将对该类或实现该接口的实例进行状态验证;
- 
字段和属性均可以使用约束验证，但是不能将相同的约束重复声明在字段和相关属性(字段的 getter 方法)上。
Object Graph 验证
除了支持 Java Bean 的实例验证外，Bean Validation 规范同样支持 Object Graph 的验证。Object Graph 即为对象的拓扑结构，如对象之间的引用关系。如果类 A 引用类 B，则在对类 A 的实例进行约束验证时也需要对类 B 的实例进行约束验证，这就是验证的级联性。当对 Java 语言中的集合、数组等类型进行验证时也需要对该类型的每一个元素进行验证。
完成级联验证的方式就是使用 @Valid 注解，如清单 15 所示：
清单 15：
- publicclass Person {   
- @NotEmpty
- private String name;   
- public String getName() {   
- return name;   
- }   
- publicvoid setName(String name) {   
- this.name = name;   
- }   
- }   
- publicclass Order {   
- @Valid
- private Person person;   
- public Person getPerson() {   
- return person;   
- }   
- publicvoid setPerson(Person person) {   
- this.person = person;   
- }   
- }   
在对 Order 的实例进行验证时，只有当在 Order 引用的对象 Person 前面声明了注解 @Valid，才对 Person 中 name 字段的 @NotEmpty 注解进行验证，否则将不予验证。
组
Bean Validation 规范中一个重要的概念，就是组和组序列。组定义了约束的子集。对于一个给定的 Object Graph 结构，有了组的概念，则无需对该 Object Graph 中所有的约束进行验证，只需要对该组定义的一个子集进行验证即可。完成组别验证需要在约束声明时进行组别的声明，否则使用默认的组 Default.class.
组使用接口的方式进行定义，清单 16 给出了如何定义组并使用组进行约束验证。
清单 16：
- publicinterface GroupA {}   
- publicclass User {   
- @NotEmpty (message = "firstname may be empty")   
- private String firstname;   
- @NotEmpty(message = "middlename may be empty", groups = Default.class)   
- private String middlename;   
- @NotEmpty(message = "lastname may be empty",groups = GroupA.class)   
- private String lastname;   
- }   
- publicstaticvoid main(String[] args){   
- User user = new User();   
- ValidatorFactory vf = Validation.buildDefaultValidatorFactory();   
- Validator validator = vf.getValidator();   
- Set<constraintviolation> set = validator.validate(user,GroupA.class);   
- for (ConstraintViolation constraintViolation : set) {   
- System.out.println(constraintViolation.getMessage());   
- }   
- }  
在类 User 中需要验证的字段上声明验证时所属的组别属性，如(groups=GroupA.class), 然后在 main 函数中调用 validator.validate(user,GroupA.class)) 方法，在此必须指定需要验证的组别。如果不显示指明，则是默认的组别。
如清单 16，验证器只会验证类 User 的 lastname 字段，如果使用 validator.validate(user))，则会使用 Default.class 组别，从而验证 firstname 和 middlename 字段。
需要注意的是：组也有继承的属性。对某一组别进行约束验证的时候，也会对其所继承的基类进行验证。
组可以进行隐式定义，其好处是可以不必在约束声明的时候显式声明组别属性，如清单 16 中的(groups=GroupA.class)。清单 17 给出了一个隐式定义的组接口(Animal)，其中包含对相应属性(getter 方法)的约束声明。相应的 Java Bean(Dog)实现了该接口。
清单 17：
- publicinterface Animal {   
- @NotEmpty String getName();   
- @NotEmpty String getOwnerName();   
- }   
- publicclass Dog implements Animal {   
- private String name;   
- private String ownername;   
- private String type;   
- publicvoid setType(String type) {   
- this.type = type;   
- }   
- public String getName() {   
- returnnull;   
- }   
- public String getOwnerName() {   
- returnnull;   
- }   
- @NotEmpty(message = "type of the dog may be empty")   
- public String getType() {   
- return type;   
- }   
- } 
这样在对类 Dog 的实例进行验证的时候，如果使用默认的组别(Default.class)，则 name，ownername 和 type 都将进行验证;如果使用 Animal 的组别，如清单 18 所示，则只会对 name 和 ownername 属性进行验证。
清单 18：
- publicstaticvoid main(String[] args) {   
- Dog dog = new Dog();   
- ValidatorFactory vf = Validation.buildDefaultValidatorFactory();   
- Validator validator = vf.getValidator();   
- Set<constraintviolation> set = validator.validate(dog,Animal.class);   
- for (ConstraintViolation constraintViolation : set) {   
- System.out.println(constraintViolation.getMessage());   
- }   
- }  
输出结果为：
- this string may be empty  
- this string may be empty 
组序列
默认情况下，不同组别的约束验证是无序的，然而在某些情况下，约束验证的顺序却很重要，如下面两个例子：(1)第二个组中的约束验证依赖于一个稳定状态来运行，而这个稳定状态是由第一个组来进行验证的。(2)某个组的验证比较耗时，CPU 和内存的使用率相对比较大，最优的选择是将其放在最后进行验证。因此，在进行组验证的时候尚需提供一种有序的验证方式，这就提出了组序列的概念。
一个组可以定义为其他组的序列，使用它进行验证的时候必须符合该序列规定的顺序。在使用组序列验证的时候，如果序列前边的组验证失败，则后面的组将不再给予验证。
清单 19 声明了组 GroupA.class，GroupB.class 和 Group.class，其中 default，GroupA，GroupB 均为 Group 的序列。
清单 19：
- publicinterface GroupA {   
- }   
- publicinterface GroupB {   
- }   
- @GroupSequence({Default.class, GroupA.class, GroupB.class})   
- publicinterface Group {   
- }   
- publicclass User {   
- @NotEmpty (message = "firstname may be empty")   
- private String firstname;   
- @NotEmpty(message = "middlename may be empty", groups = Default.class)   
- private String middlename;   
- @NotEmpty(message = "lastname may be empty",groups = GroupA.class)   
- private String lastname;   
- @NotEmpty(message = "country may be empty",groups = GroupB.class)   
- private String country;   
- }   
- publicstaticvoid main(String[] args){   
- User user = new User();   
- ValidatorFactory vf = Validation.buildDefaultValidatorFactory();  
- Validator validator = vf.getValidator();   
- Set<constraintviolation> set = validator.validate(user,Group.class);   
- for (ConstraintViolation constraintViolation : set) {   
- System.out.println(constraintViolation.getMessage());   
- }   
- }  
清单 19 中 main 函数的输出结果为：
- middlename may be empty  
- firstname may be empty 
从输出结果可以看出，该验证将不再为属于 GroupA 和 GroupB 的约束进行验证，因为属于组序列(Group.class)中前面位置的 Default 组验证失败。只有当在 main 函数加入如下代码片段使属于 Default 组别的验证通过后，方可进行后续组别(GroupA，GroupB)的验证。
- user.setFirstname("firstname");  
- user.setMiddlename("midlename"); 
穿透验证器(TrversableProperty)
穿透验证器主要适用于 JPA 规范，JPA 规范提供一种惰性连接属性，允许实体对象的某些字段被延迟加载，这些被延迟加载的字段需要 JPA 从底层数据库中获取。Bean Validation 规范通过 TraversableResolver 接口来控制这类字段的存取性。在实际使用中需要先调用该接口中的 isReachable() 方法，如果返回 true，则证明该属性是可存取的，方可进行属性的约束验证。同样，在进行级联验证时，也需要首先确定所引用的字段或者属性的可存取性方可进行约束的级联验证。
