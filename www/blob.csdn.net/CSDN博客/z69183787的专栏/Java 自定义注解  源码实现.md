# Java 自定义注解   源码实现 - z69183787的专栏 - CSDN博客
2014年10月23日 15:05:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1222
@Retention 指定注释的生存时期
 * CLASS:注释记录在类文件中，但在运行时 VM 不需要保留注释（默认）。
 * RUNTIME:注释记录在类文件中，在运行时 VM 将保留注释，因此可以使用反射机制读取注释内容。
 * SOURCE:编译器要丢弃的注释。
 @Target 
 * 指示注释类型所适用的程序元素的种类，如果注释类型声明中不存在 Target 元注释，
 * 则声明的类型可以用在任一程序元素上。
 * ElementType.ANNOTATION_TYPE：注释类型声明
 * ElementType.CONSTRUCTOR：构造方法声明
 * ElementType.FILED：字段声明
 * ElementType.LOCAL_VARIABLE：局部变量声明
 * ElementType.METHOD：方法声明
 * ElementType.PACKAGE：包声明
 * ElementType.PARAMETER：参数声明
 * ElementType.TYPE：类、借口或枚举声明
● 定义注解
@Target({ElementType.*FIELD*})
@Retention(RetentionPolicy.*RUNTIME*)
**public @interface **NotNull {
          String message() **default **"Element is not null";
}
@Target({ElementType.*FIELD*})
@Retention(RetentionPolicy.*RUNTIME*)
**public @interface **Null {
        String message() **default **"Element must be null";
}
@Target({ElementType.*FIELD*})
@Retention(RetentionPolicy.*RUNTIME*)
**public @interface **Min {
**long **value() **default **0;
}
@Target({ElementType.*FIELD*})
@Retention(RetentionPolicy.*RUNTIME*)
**public @interface **Max {
**long **value();
}
@Target({ElementType.*FIELD*})
@Retention(RetentionPolicy.*RUNTIME*)
**public @interface **Size {
**int **min();
**int **max();
}
● 实现类（利用反射机制）
**public abstract class **Validator {
**public **<T, E> **void **validate(T t, E e) {
**if** (e **instanceof **Field) {
                            Field f = (Field) e;
                            validateField(t, f);
                    } **else if **(e **instanceof **Method) {
                           Method m = (Method) e;
                           validateMethod(t, m);
                    }
          }
**public **<T> **void **validateField(T t, Field f) {
//**TODO **implements in subclass
          }
**public **<T> **void **validateMethod(T t, Method m) {
//**TODO **implements in subclass
          }
}
**public class **NotNullValidator **extends **Validator {
@Override
**public **<T> **void **validateField(T t, Field f) {
**if **(f.isAnnotationPresent(NotNull.**class**)) {
**try **{
                               f.setAccessible(**true**);
**if **(f.get(t) == **null**) {
**throw new **ValidatorException(f.getAnnotation(NotNull.**class**).message());
                               }
                          } **catch **(Exception e) {
                                 e.printStackTrace();
                          }
                 }
        }
}
**public class **NullValidator **extends **Validator {
@Override
**public **<T> **void **validateField(T t, Field f) {
**if **(f.isAnnotationPresent(Null.**class**)) {
**try **{
                               f.setAccessible(**true**);
**if **(f.get(t) ！= **null**) {
**throw new **ValidatorException(f.getAnnotation(Null.**class**).message());
                               }
                          } **catch **(Exception e) {
                                 e.printStackTrace();
                          }
                 }
        }
}
**public class **MinValidator **extends **Validator {
@Override
**public **<T> **void **validateField(T t, Field f) {
** if **(f.isAnnotationPresent(Min.**class**)) {
**try **{
                              f.setAccessible(**true**);
                              Object value = f.get(t);
**if**( value **instanceof **Short || value **instanceof **Integer || value **instanceof **Long)
 {
**long **val = f.getAnnotation(Min.**class**).value();
**if**(Long.valueOf(value.toString()) < val) {
**throw new **ValidatorException(f.getName()+"'value is not samller than " + val);
                                    }
                              } **else **{
**throw new **ValidatorException(f.getName()+" is not number.");
                              }  
                         } **catch **(Exception e) {
                                   e.printStackTrace();
                         }
                  }
         }
}
**public class **MaxValidator **extends **Validator {
@Override
**public **<T> **void **validateField(T t, Field f) {
** if **(f.isAnnotationPresent(Max.**class**)) {
**try **{
                              f.setAccessible(**true**);
                              Object value = f.get(t);
**if**( value **instanceof **Short || value **instanceof **Integer || value **instanceof **Long)
 {
**long **val = f.getAnnotation(Max.**class**).value();
**if**(Long.valueOf(value.toString()) > val) {
**throw new **ValidatorException(f.getName()+"'value is not greater than " + val);
                                    }
                              } **else **{
**throw new **ValidatorException(f.getName()+" is not number.");
                              }  
                         } **catch **(Exception e) {
                                   e.printStackTrace();
                         }
                  }
         }
}
**public class **SizeValidator **extends **Validator {
  @Override
**public **<T> **void **validateField(T t, Field f) {
**if **(f.isAnnotationPresent(Size.**class**)) {
**try **{
                               f.setAccessible(**true**);
                               Object value = f.get(t);
**if**( value **instanceof **Short || value **instanceof **Integer || value **instanceof **Long)
 {
**long **max = f.getAnnotation(Size.**class**).max();
**long **min = f.getAnnotation(Size.**class**).min();
**long **val = Long.valueOf(value.toString());
**if**(val < min || val > max) {
**throw new **ValidatorException(f.getName()+"'value is in the " + min + " to " + max + " range");
                                        }
                                } **else **{
**throw new **ValidatorException(f.getName()+" is not number.");
                                }  
                        } **catch **(Exception e) {
                                  e.printStackTrace();
                        }
                 }
        }
}
● 定义验证异常
**public class **ValidatorException **extends **Exception{
**public **ValidatorException() {
**super**();
          }
**public **ValidatorException(String message) {
**super**(message);
         }
**public **ValidatorException(String message, Throwable throwable) {
**super**(message, throwable);
         }
**public **ValidatorException(Throwable throwable) {
**super**(throwable);
         } 
}
**public enum **ValidatorType {
*Null*,*NotNull*,*Min*,*Max*,*Size*;
}
**public class** ValidatorFactory {
**public static **Validator createValidator(ValidatorType type) {
**switch**(type) {
**case ***Null *: **return new **NullValidator();
**case ***NotNull *: **return new **NotNullValidator();
**case ***Min *: **return new **MinValidator();
**case ***Max *: **return new **MaxValidator();
**case ***Size *: **return new **SizeValidator();
**default **: **return null**;
                    }
         }
}
**public class **Validation {
**public static **<T> **void **validate(T t) {
                     Field[] fields = t.getClass().getDeclaredFields();
**for**(Field f : fields) {
                           Annotation[] as = f.getAnnotations();
**for**(Annotation a : as) {
                                 String typeName = a.annotationType().getSimpleName();
                                 Validator validator = ValidatorFactory.*createValidator*(ValidatorType.*valueOf*(typeName));
                                 validator.validate(t, f);
                           }
                     }
          }
}
● 测试
**public class** TestObject {
@NotNull(message="name can not null.")
**private **String name;
@NotNull(message="age can not null.")
@Max(value = 32)
@Size(max=100, min=10)
**private **Integer age;
}
**public class **ClientTest {
**public void** test() {
                  TestObject obj = **new **TestObject();
                  obj.setName("JOE");
                  obj.setAge(10);
                   Validation.*validate*(obj);
         }
}
