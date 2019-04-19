# Hibernate 多对多之拆分中间表增加有多个关系字段 - 零度的博客专栏 - CSDN博客
2016年08月04日 11:19:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：3678
**Hibernate多对多关系中间表有其他属性的配置方式**
一、需求原因
        在我做系统架构时遇到情况是这样：资源菜单实体类(Resource)和角色实体类(Role)是多对多关系，需要各个角色可以个性化自己的资源菜单顺序。
二、设计理念
多对多的关系拆分为两个一对多（以下为关系映射图）
![](https://img-my.csdn.net/uploads/201207/11/1341986545_2711.png)
三、具体配置
方式一：XML方式
Role实体
public class Role implements Serializable {
         /*ID*/
         private Long id;
         /*名称*/
         private String name;
         /*与RoleResource的一对多关系*/
         Private Set<RoleResource> roleResources= new HashSet<RoleResource>();
         //get set
}
Resource实体
public class Resource implements Serializable {
         /*ID*/
         private Long id;
         /*名称*/
         private String name;
         /*与RoleResource的一对多关系*/
         private Set<RoleResource> roleResources = new HashSet<RoleResource>();
         // getset
}
RoleResource辅助实体
public class RoleResource implements Serializable{
         /*ID*/
         private Long id;
         /*与Role的多对一关系*/
         private Role role;
         /*与Resource的多对一关系*/
         private Resource resource;
         /*排序字段*/
         private Integer sort;
         // getset
}
Role.hbm.xml
<hibernate-mappingpackage="com.glw.domain">
         <class name="Role" table="glw_role">
                   <id name="id" column="id">
                            <generator class="native" />
                   </id>
                   <property name="name" type="string" not-null="true"unique="true" length="50"/>
                   <!--roleResource,与RoleResource的一对多关系-->
                   <set name="roleResources" order-by="id ASC"inverse="true" lazy="false">
                            <key column="roleId"/>
                            <one-to-many class="RoleResource" />
                   </set>
         </class>
</hibernate-mapping>
Resource.hbm.xml
<hibernate-mappingpackage="com.glw.domain">
         <class name="Resource" table="glw_resource">
                   <id name="id" column="id">
                            <generator class="native" />
                   </id>
                   <property name="name" type="string" not-null="true"length="50"/>
                   <!--roleResources,与RoleResource的一对多关系-->
                   <set name="roleResources" order-by="id ASC"inverse="true" lazy="false">
                            <key column="resourceId"/>
                            <one-to-many class="RoleResource"/>
                   </set>
         </class>
</hibernate-mapping>
RoleResource.hbm.xml
<hibernate-mappingpackage="com.glw.domain">
         <class name="RoleResource" table="glw_role_resource">
                   <id name="id" column="id">
                            <generator class="native" />
                   </id>
                   <property name="sort" type="integer" not-null="true" />
                   <!--role,与Role的多对一关系-->
                   <many-to-one name="role" class="Role" column="roleId" />
                   <!--resource,与Resource的多对一关系-->
                   <many-to-one name="resource" class="Resource"column="resourceId"/>
         </class>
</hibernate-mapping>
Hibernate.cfg.xml中配置
<mapping resource="com/glw/domain/Role.hbm.xml"/>
<mapping resource="com/glw/domain/Resource.hbm.xml" />
<mapping resource="com/glw/domain/RoleResource.hbm.xml" />
方式二：Annotation方式
Role实体
@Entity
@Table(name="glw_role")
public class Role {
         @Id
         @GeneratedValue(strategy=GenerationType.TABLE)
         private Long id;
         @Column(length=50)
         private String name;
         @OneToMany(mappedBy="role",cascade=CascadeType.ALL)
         private Set<RoleResource> roleResources = new HashSet<RoleResource>();
         //get set
}
Resource实体
@Entity
@Table(name="glw_resource")
public class Resource {
         @Id
         @GeneratedValue(strategy=GenerationType.TABLE)
         private Long id;
         @Column(length=50)
         private String name;
         @OneToMany(mappedBy="resource",cascade=CascadeType.ALL)
         private Set<RoleResource> roleResources = new HashSet<RoleResource>();
         // getset
}
RoleResource辅助实体
@Entity
@Table(name="glw_role_resource")
public class RoleResource {
         @Id
         @GeneratedValue(strategy=GenerationType.TABLE)
         private Long id;
         @Column
         private Integer sort;
         @ManyToOne(cascade=CascadeType.ALL)
         @JoinColumn(name="roleId",nullable=true)
         private Role role;
         @ManyToOne(cascade=CascadeType.ALL)
         @JoinColumn(name="resourceId",nullable=true)
         private Resource resource;
         // getset
}
Hibernate.cfg.xml中配置
<mapping class="com.glw.domain.Role"/>
<mapping class="com.glw.domain.Resource"/>
<mapping class="com.glw.domain.RoleResource"/>
四、完毕
Xml和Annotation方式可任意选取一种，以上本人均测试通过。[](mailto:%E6%AC%A2%E8%BF%8E%E5%8F%91%E9%82%AE%E4%BB%B6%E5%88%B0linwu_gao@163.com)
