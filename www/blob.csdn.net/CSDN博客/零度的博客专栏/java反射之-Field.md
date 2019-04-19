# java反射之-Field - 零度的博客专栏 - CSDN博客
2016年04月11日 15:28:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：963
package zmx.reflect.test;
import static java.lang.System.out;
import java.lang.reflect.Field;
import javassist.Modifier;
public class FieldTest {
 enum Color {
  Blue,
  Red
 }
 class Inner {
 }
 public static void main(String args[]) throws  Exception {
  Role role = new Role();
  role.setId("role1");
  role.setUserId("user1");
  role.setRoleName("Role1");
  //Field idField = getDeclaredField(role.getClass(), "id");
  Field idField = role.getClass().getField("id");
  Field childrenField = getDeclaredField(role.getClass(), "children");
  Field roleTypeField = getDeclaredField(role.getClass(), "roleType");
  Field userField = getDeclaredField(role.getClass(), "user");
  // 获取属性声明时类型对象（返回class对象）
  System.out.println(idField.getType());
  // 返回属性声的Type类型
  System.out.println(idField.getGenericType());
  // 如果属性是一个泛型，从getType只能得到这个属性的接口类型
  System.out.println(childrenField.getType());
  // 如果属性是一个参数化类型，从getGenericType还能得到这个泛型的参数类型
  System.out.println(childrenField.getGenericType());
  // 获取属性声明时名字
  System.out.println(idField.getName());
  // 获得这个属性上所有的注释
  System.out.println(idField.getAnnotations().length);
  // 获取属性的修饰
  System.out.println(Modifier.toString(idField.getModifiers()));
  // 判断这个属性是否是枚举类
  System.out.println(roleTypeField.isEnumConstant());
  // 判断这个属性是否是复合类
  System.out.println(userField.isSynthetic());
  // FieldTest$Color是Color枚举类编译后的名字。
  isSyntheticOrEnumConstant("zmx.reflect.test.FieldTest$Color");
  // FieldTest$Inner是Inner类编译后的名字。
  isSyntheticOrEnumConstant("zmx.reflect.test.FieldTest$Inner");
  try
  {
   // 取得对象这个Field上的值
   System.out.println(idField.get(role));
   // 向对象的这个Field重新设置值
   idField.set(role, "role2");
   System.out.println(idField.get(role));
  } catch(IllegalArgumentException e) {
   e.printStackTrace();
  } catch
  (IllegalAccessException e) {
   e.printStackTrace();
  }
 }
 public static Field getDeclaredField(final Class cla, final String fieldName) {
  for(Class superClass = cla; superClass != null; superClass = superClass.getSuperclass()) {
   try{
    return superClass.getDeclaredField(fieldName);
   } catch (NoSuchFieldException e) {
    e.printStackTrace();
   }
  }
  return null;
 }
 public static void isSyntheticOrEnumConstant(String completePackageName) {
  try
  {
   Class<?> c = Class.forName(completePackageName);
   Field[] flds = c.getDeclaredFields();
   for
   (Field f : flds) {
    out.format("%-8s[ synthetic=%-5b enum_constant=%-5b ]%n",
    c.getName() + ":"
    + f.getName(), f.isSynthetic(), f.isEnumConstant());
   }
  } catch (ClassNotFoundException e) {
   e.printStackTrace();
  }
 }
}
package zmx.reflect.test;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;
@SuppressWarnings("serial")
@MappedSuperclass
public class AbsEntity {
  @Id
  @GeneratedValue
  public String id;
  public String getId() {
   return id;
  }
  public void setId(String id) {
   this.id = id;
  }
}
package zmx.reflect.test;
public interface IEntity {
 public String getDisplayString();
}
package zmx.reflect.test;
public enum RoleType {
  Manager, Employee;
}
package zmx.reflect.test;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import org.hibernate.annotations.AccessType;
import org.hibernate.annotations.GenericGenerator;
@Entity
@Table(name = "users", schema = "public")
public class User {
 @Id
 @GeneratedValue(generator = "system-uuid")
 @GenericGenerator(name = "system-uuid", strategy = "uuid.hex")
 @Column(length = 40)
 @AccessType("property")
 private String id;
 private String loginName;
 private String password;
 private String address;
 @ManyToOne
 private Role role;
 public String getId() {
  return
  id;
 }
 protected void setId(String id) {
  this.id = id;
 }
 public String getLoginName() {
  return
  loginName;
 }
 public void setLoginName(String loginName) {
  this.loginName = loginName;
 }
 public String getPassword() {
  return
  password;
 }
 public void setPassword(String password) {
  this.password = password;
 }
 public String getAddress() {
  return
  address;
 }
 public void setAddress(String address) {
  this.address = address;
 }
 public Role getRole() {
  return
  role;
 }
 public void setRole(Role role) {
  this.role = role;
 }
}
package zmx.reflect.test;
import java.util.List;
import javax.persistence.Entity;
import javax.persistence.Table;
@Entity
@Table(schema = "public")
public class Role extends AbsEntity implements IEntity{
 public String roleName;
 public String userId;
 public List<User> children;
 private RoleType roleType = RoleType.Manager;
 private User user;
 public Role() {}
 private Role(String roleName) {
  this.roleName = roleName;
 }
 public String getRoleName() {
  return
  roleName;
 }
 public void setRoleName(String roleName) {
  this.roleName = roleName;
 }
 public String getUserId() {
  return
  userId;
 }
 public void setUserId(String userId) {
  this.userId = userId;
 }
 public String getDisplayString() {
  System.out.println("I am a Role");
  return
  "Role";
 }
}
输出结果：
class java.lang.String
class java.lang.String
interface java.util.List
java.util.List<zmx.reflect.test.User>
id
2
public
false
false
zmx.reflect.test.FieldTest$Color:Blue[ synthetic=false enum_constant=true  ]
zmx.reflect.test.FieldTest$Color:Red[ synthetic=false enum_constant=true  ]
zmx.reflect.test.FieldTest$Color:ENUM$VALUES[ synthetic=true  enum_constant=false ]
zmx.reflect.test.FieldTest$Inner:this$0[ synthetic=true  enum_constant=false ]
role1
role2
