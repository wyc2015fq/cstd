# mappingLocations、mappingDirectoryLocations - z69183787的专栏 - CSDN博客
2012年11月30日 16:46:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1424
由于spring对hibernate配置文件hibernate.cfg.xml的集成相当好， 
所以，在项目中我一直使用spring的org.springframework.orm.hibernate.LocalSessionFactoryBean来取代hibernate.cfg.xml文件的功能 
LocalSessionFactoryBean有好几个属性用来查找hibernate映射文件：mappingResources、mappingLocations、mappingDirectoryLocations与mappingJarLocations 
他们的区别： 
mappingResources：指定classpath下具体映射文件名 
<property name="mappingResources"> 
    <value>petclinic.hbm.xml </value> 
</property> 
mappingLocations：可以指定任何文件路径，并且可以指定前缀：classpath、file等 
<property name="mappingLocations"> 
    <value>/WEB-INF/petclinic.hbm.xml </value> 
</property> 
<property name="mappingLocations"> 
    <value>classpath:/com/company/domain/petclinic.hbm.xml </value> 
</property> 
也可以用通配符指定，'*'指定一个文件(路径)名，'**'指定多个文件(路径)名，例如： 
<property name="mappingLocations"> 
    <value>classpath:/com/company/domainmaps/*.hbm.xml </value> 
</property> 
上面的配置是在com/company/domain包下任何maps路径下的hbm.xml文件都被加载为映射文件 
mappingDirectoryLocations：指定映射的文件路径 
<property name="mappingDirectoryLocations">
  <list>
  <value>WEB-INF/HibernateMappings</value>
  </list>
</property>
也可以通过classpath来指出
<property name="mappingDirectoryLocations">
  <list>
  <value>classpath:/XXX/package/</value>
  </list>
</property>
mappingJarLocations：指定加载的映射文件在jar文件
