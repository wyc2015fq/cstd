# [Hibernate] - Query Select - weixin_33985507的博客 - CSDN博客
2014年09月03日 16:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
测试了常用的一些HQL查询方法，具体HQL的强大可以参考：
[http://docs.jboss.org/hibernate/orm/3.5/reference/zh-CN/html/queryhql.html#queryhql-casesensitivity](http://docs.jboss.org/hibernate/orm/3.5/reference/zh-CN/html/queryhql.html#queryhql-casesensitivity)
Java Beans:
```
package com.my.bean;
import java.util.Date;
import java.util.Set;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToMany;
import javax.persistence.Table;
@Entity
@Table(name="user")
public class User {
    @Id @GeneratedValue @Column(name="user_id", nullable=false)
    private long userID;
    
    @Column(name="user_name", length=100, nullable=false)
    private String userName;
    
    @Column(name="create_time", nullable=false)
    private Date createTime;
    
    @OneToMany(mappedBy="user", cascade=CascadeType.ALL)
    private Set<UserCard> cards;
    public long getUserID() {
        return userID;
    }
    public void setUserID(long userID) {
        this.userID = userID;
    }
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
    public Date getCreateTime() {
        return createTime;
    }
    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }
    public Set<UserCard> getCards() {
        return cards;
    }
    public void setCards(Set<UserCard> cards) {
        this.cards = cards;
    }
}
```
```
package com.my.bean;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
@Entity
@Table(name="user_card")
public class UserCard {
    @Id
    @GeneratedValue(strategy=GenerationType.IDENTITY)
    @Column(name="card_id")
    private long cardID;
    
    @Column(name="card_name")
    private String cardName;
    
    @ManyToOne(cascade=CascadeType.ALL)
    @JoinColumn(name="user_id")
    private User user;
    
    public long getCardID() {
        return cardID;
    }
    public void setCardID(long cardID) {
        this.cardID = cardID;
    }
    public String getCardName() {
        return cardName;
    }
    public void setCardName(String cardName) {
        this.cardName = cardName;
    }
    public User getUser() {
        return user;
    }
    public void setUser(User user) {
        this.user = user;
    }
}
```
一些DTO的Java Beans:
```
package com.my.bean.dto;
public class GroupByTemp {
    private long sumUser;
    private long userID;
    
    public long getSumUser() {
        return sumUser;
    }
    public void setSumUser(long sumUser) {
        this.sumUser = sumUser;
    }
    public long getUserID() {
        return userID;
    }
    public void setUserID(long userID) {
        this.userID = userID;
    }
    
    /**
     * @param sumUser
     * @param userID
     */
    public GroupByTemp(long sumUser, long userID) {
        this.sumUser = sumUser;
        this.userID = userID;
    }
}
```
```
package com.my.bean.dto;
import java.math.BigInteger;
import java.util.Date;
public class UserSQLTemp {
    private BigInteger userID;
    private Date createTime;
    private String userName;
    public BigInteger getUserID() {
        return userID;
    }
    public void setUserID(BigInteger userID) {
        this.userID = userID;
    }
    public Date getCreateTime() {
        return createTime;
    }
    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
}
```
```
package com.my.bean.dto;
public class UserTemp {
    private long userID;
    private String userName;
    private String cardName;
    
    public long getUserID() {
        return userID;
    }
    public void setUserID(long userID) {
        this.userID = userID;
    }
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
    public String getCardName() {
        return cardName;
    }
    public void setCardName(String cardName) {
        this.cardName = cardName;
    }
    
    public UserTemp(long userID, String userName, String cardName) {
        super();
        this.userID = userID;
        this.userName = userName;
        this.cardName = cardName;
    }
}
```
测试：
```
package com.my.init;
import java.util.List;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.Transaction;
import org.hibernate.transform.Transformers;
import com.my.bean.User;
import com.my.bean.UserCard;
import com.my.bean.dto.GroupByTemp;
import com.my.bean.dto.UserSQLTemp;
import com.my.bean.dto.UserTemp;
import com.my.dao.util.HibernateUtil;
public class Test {
    @SuppressWarnings({ "unchecked" })
    public static void main(String[] args) {
        Session session = HibernateUtil.getSessionFactory().openSession();
        Transaction tx = session.beginTransaction();
        try {
            // ----------------------------------------------------
            // Select all data
            // ----------------------------------------------------
            String hql = "FROM User AS U ORDER BY U.createTime DESC";
            Query query = session.createQuery(hql);
            List<User> users = query.list();
            for (User user : users) {
                System.out.println("User:" + user.getUserName());
                System.out.println("Card size:" + user.getCards().size());
                for (UserCard c : user.getCards()) {
                    System.out.println("Card:" + c.getCardName());
                }
            }
            // ----------------------------------------------------
            // Select all user name
            // ----------------------------------------------------
            hql = "SELECT U.userName FROM User AS U";
            query = session.createQuery(hql);
            List<String> userNames = query.list();
            for (String item : userNames) {
                System.out.println("Name:" + item);
            }
            // ----------------------------------------------------
            // Where
            // ----------------------------------------------------
            hql = "FROM User AS U WHERE U.userID=:userID";
            query = session.createQuery(hql);
            query.setParameter("userID", (long) 1);
            users = query.list();
            for (User user : users) {
                System.out.println("User:" + user.getUserName());
            }
            // ----------------------------------------------------
            // Other fields, DTO
            // ----------------------------------------------------
            hql = "SELECT new com.my.bean.dto.UserTemp(U.userID, U.userName, C.cardName)"
                    + " FROM User AS U, UserCard AS C"
                    + " WHERE U.userID=:userID";
            query = session.createQuery(hql);
            query.setParameter("userID", (long) 2);
            List<UserTemp> temps = query.list();
            for (UserTemp item : temps) {
                System.out.println("User name:" + item.getUserName()
                        + "\tCard name:" + item.getCardName());
            }
            // ----------------------------------------------------
            // Group by
            // ----------------------------------------------------
            hql = "SELECT new com.my.bean.dto.GroupByTemp(SUM(C.user.userID), C.user.userID) FROM UserCard AS C GROUP BY C.cardName";
            query = session.createQuery(hql);
            List<GroupByTemp> results = query.list();
            for (GroupByTemp item : results) {
                System.out.println("UserID:" + item.getUserID() + "\tSum:"
                        + item.getSumUser());
            }
            // ----------------------------------------------------
            // Count
            // ----------------------------------------------------
            hql = "SELECT COUNT(*) FROM User";
            query = session.createQuery(hql);
            long count = (long) query.uniqueResult();
            System.out.println("Total:" + count);
            // ----------------------------------------------------
            // SQL
            // ----------------------------------------------------
            String sql = "SELECT user_id AS userID, user_name AS userName, create_time AS createTime FROM user";
            query = session.createSQLQuery(sql).setResultTransformer(
                    Transformers.aliasToBean(UserSQLTemp.class));
            List<UserSQLTemp> usersTemp = query.list();
            for (UserSQLTemp item : usersTemp) {
                System.out.println("UserID:" + item.getUserID()
                        + "\tUser name:" + item.getUserName());
            }
            // ----------------------------------------------------
            // Left join
            // ----------------------------------------------------
            hql = "SELECT U FROM User U LEFT JOIN U.cards C WITH C.cardID=:cardID WHERE U.userName=:userName";
            query = session.createQuery(hql);
            query.setLong("cardID", 1);
            query.setString("userName", "Robin");
            users = query.list();
            for (User user : users) {
                System.out.println("User ID:" + user.getUserID()
                        + "\tUser name:" + user.getUserName());
            }
            // ----------------------------------------------------
            // Top N records
            // ----------------------------------------------------
            hql = "SELECT U FROM User AS U";
            query = session.createQuery(hql).setMaxResults(1);
            users = query.list();
            for (User user : users) {
                System.out.println("User name:" + user.getUserName());
            }
            // ----------------------------------------------------
            // Like
            // ----------------------------------------------------
            hql = "SELECT U FROM User AS U WHERE U.userName LIKE :userName";
            query = session.createQuery(hql);
            query.setString("userName", "%m%");
            users = query.list();
            for (User user : users) {
                System.out.println("User name:" + user.getUserName());
            }
            // ----------------------------------------------------
            // Paging, set First result and Max result
            // ----------------------------------------------------
            hql = "SELECT U FROM User AS U";
            query = session.createQuery(hql).setFirstResult(1).setMaxResults(1);
            users = query.list();
            for (User user : users) {
                System.out.println("User name:" + user.getUserName());
            }
            // ----------------------------------------------------
            // IN
            // ----------------------------------------------------
            hql = "SELECT U FROM User AS U WHERE U.userID IN (:userID)";
            query = session.createQuery(hql);
            Long[] userIDs = new Long[] { (long) 1, (long) 2 };
            query.setParameterList("userID", userIDs);
            users = query.list();
            for (User user : users) {
                System.out.println("User name:" + user.getUserName());
            }
            // ----------------------------------------------------
            // Sub search
            // ----------------------------------------------------
            hql = "SELECT U FROM User AS U WHERE U IN (SELECT C.user FROM UserCard AS C WHERE C.cardID=:cardID)";
            query = session.createQuery(hql);
            query.setLong("cardID", 1);
            users = query.list();
            for (User user : users) {
                System.out.println("User name:" + user.getUserName());
            }
            // ----------------------------------------------------
            // Sub case
            // ----------------------------------------------------
            hql = "SELECT new com.my.bean.dto.UserTemp("
                    + "U.userID, U.userName, (SELECT C.cardName FROM UserCard C WHERE C.cardID=:cardID)"
                    + ") FROM User U";
            query = session.createQuery(hql);
            query.setLong("cardID", 1);
            temps = query.list();
            for (UserTemp item : temps) {
                System.out.println("UserID:" + item.getUserID()
                        + "\tUser name:" + item.getUserName() + "\tCard name:"
                        + item.getCardName());
            }
            tx.commit();
        } catch (Exception e) {
            tx.rollback();
            e.printStackTrace();
        }
        session.close();
    }
}
```
HQL中也可以使用SQL，见测试例子：SQL
（但这种方法不推荐使用，因为这样就破坏了OO设计，除非在逼不得已的情况下，比如那些最初没设计好数据库然后又要出复杂到恶心的报表时）
