# hibernate联合主键生成机制（annotation方式） - z69183787的专栏 - CSDN博客
2013年07月19日 14:13:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1297
这里记录hibernate中组合主键的annotation方式。
hibernate-annotations说明文档中有说到组合主键使用annotation的方式实现，如下几种
下面是定义组合主键的几种语法:
1.  将组件类注解为@Embeddable,并将组件的属性注解为@Id
2.  将组件的属性注解为@EmbeddedId
3.  将类注解为@IdClass,并将该实体中所有属于主键的属性都注解为@Id
### 第一种方式实例：
1.主键类，如下
package com.seed.lee.model;
import javax.persistence.Column; 
import javax.persistence.Embeddable;
/** 
* 这个类作为Person类的（组合主键）主键类 
* 
* @author Administrator 
* 
*/ 
@Embeddable  //这里就是annotation文档中所说的，将组件类注解为@Embeddable
public class PeopleUionPKID implements java.io.Serializable {
    private String firstName;
    private String secondName;
    @Column(length=20) 
    public String getFirstName() { 
        return firstName; 
    }
    public void setFirstName(String firstName) { 
        this.firstName = firstName; 
    } 
    @Column(length=20) 
    public String getSecondName() { 
        return secondName; 
    }
    public void setSecondName(String secondName) { 
        this.secondName = secondName; 
    }
    @Override 
    public boolean equals(Object obj) { 
        if(obj instanceof PeopleUionPKID){ 
            PeopleUionPKID pk=(PeopleUionPKID)obj; 
            if(this.firstName.equals(pk.firstName)&&this.secondName.equals(pk.secondName)){ 
                return true; 
            } 
        } 
        return false; 
    }
    @Override 
    public int hashCode() { 
        return super.hashCode(); 
    }
}
2.与数据库表对应的类
package com.seed.lee.model;
import javax.persistence.Column; 
import javax.persistence.Entity; 
import javax.persistence.Id;
@Entity 
public class Peopel {
    PeopleUionPKID uionPKID = new PeopleUionPKID();
    private int age;
    private String sex;
    private String job;
    @Id   //这里就是文档中所述的，并将组件的属性注解为@Id 
    public PeopleUionPKID getUionPKID() { 
        return uionPKID; 
    }
    public void setUionPKID(PeopleUionPKID uionPKID) { 
        this.uionPKID = uionPKID; 
    }
    public int getAge() { 
        return age; 
    }
    public void setAge(int age) { 
        this.age = age; 
    }
    @Column(length=4) 
    public String getSex() { 
        return sex; 
    }
    public void setSex(String sex) { 
        this.sex = sex; 
    }
    @Column(length=20) 
    public String getJob() { 
        return job; 
    }
    public void setJob(String job) { 
        this.job = job; 
    }
}
说明：除了代码中标为粗体的与普通的单主键类不同之外，其他都大同小异.
#### 第二种方式比较简单
1.主键类，这个类不需要做任何的注解
package com.seed.lee.model;
import javax.persistence.Column;
/** 
* 这个类作为Person类的（组合主键）主键类 
* 
* @author Administrator 
* 
*/
public class PeopleUionPKID implements java.io.Serializable {
    private String firstName;
    private String secondName;
    @Column(length=20) 
    public String getFirstName() { 
        return firstName; 
    }
    public void setFirstName(String firstName) { 
        this.firstName = firstName; 
    } 
    @Column(length=20) 
    public String getSecondName() { 
        return secondName; 
    }
    public void setSecondName(String secondName) { 
        this.secondName = secondName; 
    }
    @Override 
    public boolean equals(Object obj) { 
        if(obj instanceof PeopleUionPKID){ 
            PeopleUionPKID pk=(PeopleUionPKID)obj; 
            if(this.firstName.equals(pk.firstName)&&this.secondName.equals(pk.secondName)){ 
                return true; 
            } 
        } 
        return false; 
    }
    @Override 
    public int hashCode() { 
        return super.hashCode(); 
    }
}
2.数据表映射的类(只需要在主键类的引用中添加@EmbeddedId就可以)
package com.seed.lee.model;
import javax.persistence.Column; 
import javax.persistence.EmbeddedId; 
import javax.persistence.Entity; 
import javax.persistence.Id;
@Entity 
public class Peopel {
    PeopleUionPKID uionPKID = new PeopleUionPKID();
    private int age;
    private String sex;
    private String job;
    @EmbeddedId 
    public PeopleUionPKID getUionPKID() { 
        return uionPKID; 
    }
    public void setUionPKID(PeopleUionPKID uionPKID) { 
        this.uionPKID = uionPKID; 
    }
    public int getAge() { 
        return age; 
    }
    public void setAge(int age) { 
        this.age = age; 
    }
    @Column(length=4) 
    public String getSex() { 
        return sex; 
    }
    public void setSex(String sex) { 
        this.sex = sex; 
    }
    @Column(length=20) 
    public String getJob() { 
        return job; 
    }
    public void setJob(String job) { 
        this.job = job; 
    }
}
#### 第三种方式,这种方式相对麻烦一点，据说这种是符合常人的编码规范的，所以有很多人用
1.  依然需要主键类，类中不需要注解任何的属性
package com.seed.lee.model;
import javax.persistence.Column;
/** 
* 这个类作为Person类的（组合主键）主键类 
* 
* @author Administrator 
* 
*/
public class PeopleUionPKID implements java.io.Serializable {
    private String firstName;
    private String secondName;
    @Column(length=20) 
    public String getFirstName() { 
        return firstName; 
    }
    public void setFirstName(String firstName) { 
        this.firstName = firstName; 
    } 
    @Column(length=20) 
    public String getSecondName() { 
        return secondName; 
    }
    public void setSecondName(String secondName) { 
        this.secondName = secondName; 
    }
    @Override 
    public boolean equals(Object obj) { 
        if(obj instanceof PeopleUionPKID){ 
            PeopleUionPKID pk=(PeopleUionPKID)obj; 
            if(this.firstName.equals(pk.firstName)&&this.secondName.equals(pk.secondName)){ 
                return true; 
            } 
        } 
        return false; 
    }
    @Override 
    public int hashCode() { 
        return super.hashCode(); 
    }
}
2.数据表对应的类（这个类中需要将表的组合主键也记录成属性，并在所有的主键字段的get方法上注解@id，最后就是在类名上方注解
@IdClass(value=PeopleUionPKID.class)，PeopleUionPKID就是主键类的类短名 
）
package com.seed.lee.model;
import javax.persistence.Column; 
import javax.persistence.Entity; 
import javax.persistence.Id; 
import javax.persistence.IdClass;
@Entity 
@IdClass(value=PeopleUionPKID.class) 
public class Peopel {
//    PeopleUionPKID uionPKID = new PeopleUionPKID(); 
    private String firstName; 
    private String secondName;
    private int age;
    private String sex;
    private String job;
//    @EmbeddedId 
//    public PeopleUionPKID getUionPKID() { 
//        return uionPKID; 
//    } 
// 
//    public void setUionPKID(PeopleUionPKID uionPKID) { 
//        this.uionPKID = uionPKID; 
//    }
    public int getAge() { 
        return age; 
    }
    @Id 
    public String getFirstName() { 
        return firstName; 
    }
    public void setFirstName(String firstName) { 
        this.firstName = firstName; 
    }
    @Id 
    public String getSecondName() { 
        return secondName; 
    }
    public void setSecondName(String secondName) { 
        this.secondName = secondName; 
    }
    public void setAge(int age) { 
        this.age = age; 
    }
    @Column(length=4) 
    public String getSex() { 
        return sex; 
    }
    public void setSex(String sex) { 
        this.sex = sex; 
    }
    @Column(length=20) 
    public String getJob() { 
        return job; 
    }
    public void setJob(String job) { 
        this.job = job; 
    }
}
具体查看hibernate annotation的说明文档。
