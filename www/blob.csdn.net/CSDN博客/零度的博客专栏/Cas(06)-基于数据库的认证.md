# Cas(06)-基于数据库的认证 - 零度的博客专栏 - CSDN博客
2016年05月25日 14:04:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：785
Cas Server自身已经为我们实现了几种基于JDBC的AuthenticationHandler实现，但它们不包含在Cas Server的核心包里面，而是包含在cas-server-support-jdbc中，如果我们要使用Cas Server已经实现好的基于JDBC的AuthenticationHandler，我们必须先将cas-server-support-jdbc对应的jar包、相关数据库的驱动，以及所需要使用的数据源实现等jar包加入Cas
 Server的类路径中。如果是基于Maven的war覆盖机制来修改Cas Server的配置文件，则我们可以在自己的Maven项目的依赖中加入如下项（对应的驱动就没贴出来了）。
<dependency>
<groupId>org.jasig.cas</groupId>
<artifactId>cas-server-support-jdbc</artifactId>
<version>${cas.version}</version>
<scope>runtime</scope>
</dependency>
       Cas Server默认已经实现好的基于JDBC的AuthenticationHandler有三个，它们都继承自AbstractJdbcUsernamePasswordAuthenticationHandler，而且在认证过程中都需要一个DataSource。下面来对它们做一个简要的介绍。
## 1.1     BindModeSearchDatabaseAuthenticationHandler
       BindModeSearchDatabaseAuthenticationHandler将试图以传入的用户名和密码从配置的DataSource中建立一个连接，如果连接成功，则表示认证成功，否则就是认证失败。以下是BindModeSearchDatabaseAuthenticationHandler源码的一段主要代码，通过它我们可以明显的看清其逻辑：
**protected final boolean** authenticateUsernamePasswordInternal(
**final** UsernamePasswordCredentials credentials)
**throws** AuthenticationException {
**final** String username = credentials.getUsername();
**final** String password = credentials.getPassword();
**try** {
**final** Connection c =
**this**.getDataSource()
                .getConnection(username, password);
            DataSourceUtils.*releaseConnection*(c,
**this**.getDataSource());
**returntrue**;
        } **catch** (**final** SQLException e) {
**returnfalse**;
        }
    }
       当然，这种实现也需要你的DataSource支持getConnection(user,password)才行，否则将返回false。dbcp的BasicDataSource的不支持的，而c3p0的ComboPooledDataSource支持。
       以下是一个使用BindModeSearchDatabaseAuthenticationHandler的配置示例：
<beanid=*"authenticationManager"*
class=*"org.jasig.cas.authentication.AuthenticationManagerImpl"*>
      ...
<propertyname=*"authenticationHandlers"*>
<list>
            ...
<beanclass=*"org.jasig.cas.adaptors.jdbc.BindModeSearchDatabaseAuthenticationHandler"*>
<propertyname=*"dataSource"*ref=*"dataSource"*/>
</bean>
            ...
</list>
</property>
      ...
</bean>
## 1.2     QueryDatabaseAuthenticationHandler
       使用QueryDatabaseAuthenticationHandler需要我们指定一个SQL，该SQL将接收一个用户名作为查询条件，然后返回对应的密码。该SQL将被QueryDatabaseAuthenticationHandler用来通过传入的用户名查询对应的密码，如果存在则将查询的密码与查询出来的密码进行匹配，匹配结果将作为认证结果。如果对应的用户名不存在也将返回false。
       以下是QueryDatabaseAuthenticationHandler的一段主要代码：
**protected final boolean** authenticateUsernamePasswordInternal(**final**
 UsernamePasswordCredentials credentials) **throws** AuthenticationException {
**final** String username = getPrincipalNameTransformer().transform(credentials.getUsername());
**final** String password = credentials.getPassword();
**final** String encryptedPassword =
**this**.getPasswordEncoder().encode(
            password);
**try** {
**final** String dbPassword = getJdbcTemplate().queryForObject(**this**.sql, String.**class**,
 username);
**return** dbPassword.equals(encryptedPassword);
        } **catch** (**final** IncorrectResultSizeDataAccessException e) {
// this means the username was not found.
**returnfalse**;
        }
    }
       上面的逻辑非常明显。此外，如你所见，QueryDatabaseAuthenticationHandler使用的用户名会经过PrincipalNameTransformer进行转换，而密码会经过PasswordEncoder进行编码。Cas Server中基于JDBC的AuthenticationHandler实现中使用到的PrincipalNameTransformer默认是不进行任何转换的NoOpPrincipalNameTransformer，而默认使用的PasswordEncoder也是不会经过任何编码的PlainTextPasswordEncoder。当然了，cas-server-jdbc-support对它们也有另外两种支持，即PrefixSuffixPrincipalNameTransformer和DefaultPasswordEncoder。
### 1.2.1  PrefixSuffixPrincipalNameTransformer
       PrefixSuffixPrincipalNameTransformer的作用很明显，如其名称所描述的那样，其在转换时会将用户名加上指定的前缀和后缀。所以用户在使用的时候需要指定prefix和suffix两个属性，默认是空。
### 1.2.2  DefaultPasswordEncoder
       DefaultPasswordEncoder底层使用的是标准[Java](http://lib.csdn.net/base/17)类库中的MessageDigest进行加密的，其支持MD5、SHA等加密[算法](http://lib.csdn.net/base/31)。在使用时需要通过构造参数encodingAlgorithm来指定使用的加密算法，可以使用characterEncoding属性注入来指定获取字节时使用的编码，不指定则使用默认编码。以下是DefaultPasswordEncoder的源码，其展示了DefaultPasswordEncoder的加密逻辑。
**public final class** DefaultPasswordEncoder
**implements** PasswordEncoder {
**privatestaticfinalchar**[]
*HEX_DIGITS* = {'0',
'1', '2', 
'3', '4', '5',
'6', '7', 
'8', '9', 'a',
'b', 'c', 
'd', 'e', 'f'};
@NotNull
**privatefinal** String
encodingAlgorithm;
**private** String
characterEncoding;
**public** DefaultPasswordEncoder(**final** String encodingAlgorithm) {
**this**.encodingAlgorithm = encodingAlgorithm;
    }
**public** String encode(**final** String password) {
**if** (password ==
**null**) {
**returnnull**;
        }
**try** {
            MessageDigest messageDigest = MessageDigest
                .*getInstance*(**this**.encodingAlgorithm);
**if** (StringUtils.*hasText*(**this**.characterEncoding)) {
                messageDigest.update(password.getBytes(**this**.characterEncoding));
            } **else** {
                messageDigest.update(password.getBytes());
            }
**finalbyte**[] digest = messageDigest.digest();
**return** getFormattedText(digest);
        } **catch** (**final** NoSuchAlgorithmException e) {
**thrownew** SecurityException(e);
        } **catch** (**final** UnsupportedEncodingException e) {
**thrownew** RuntimeException(e);
        }
    }
/**
     * Takes the raw bytes from the digest and formats them correct.
     * 
     * **@param** bytes the raw bytes from the digest.
     * **@return** the formatted bytes.
     */
**private** String getFormattedText(**byte**[] bytes) {
**final** StringBuilder buf =
**new** StringBuilder(bytes.length * 2);
**for** (**int** j = 0; j < bytes.length; j++) {
            buf.append(*HEX_DIGITS*[(bytes[j] >> 4) & 0x0f]);
            buf.append(*HEX_DIGITS*[bytes[j] & 0x0f]);
        }
**return** buf.toString();
    }
**publicfinalvoid** setCharacterEncoding(**final** String characterEncoding)
 {
**this**.characterEncoding = characterEncoding;
    }
}
       如果在认证时需要使用DefaultPasswordEncoder，则需要确保数据库中保存的密码的加密方式和DefaultPasswordEncoder的加密算法及逻辑是一致的。如果这些都不能满足你的需求，则用户可以实现自己的PrincipalNameTransformer和PasswordEncoder。
       以下是一个配置使用QueryDatabaseAuthenticationHandler进行认证，且使用DefaultPasswordEncoder对密码进行MD5加密的示例：
<beanid=*"authenticationManager"*
class=*"org.jasig.cas.authentication.AuthenticationManagerImpl"*>
      ...
<propertyname=*"authenticationHandlers"*>
<list>
            ...
<beanclass=*"org.jasig.cas.adaptors.jdbc.QueryDatabaseAuthenticationHandler"*>
<propertyname=*"dataSource"*ref=*"dataSource"*/>
<propertyname=*"passwordEncoder"*ref=*"passwordEncoder"*/>
<propertyname=*"sql"*value=*"select password from t_user where username = ?"*/>
</bean>
            ...
</list>
</property>
      ...
</bean>
<beanid=*"passwordEncoder"*class=*"org.jasig.cas.authentication.handler.DefaultPasswordEncoder"*>
<constructor-argvalue=*"MD5"*/><!--
加密算法 -->
</bean>
## 1.3     SearchModeSearchDatabaseAuthenticationHandler
       SearchModeSearchDatabaseAuthenticationHandler的主要逻辑是将传入的用户名和密码作为条件从指定的表中进行查询，如果对应记录存在则表示认证通过。使用该AuthenticationHandler时需要我们指定查询时使用的表名（tableUsers）、用户名对应的字段名（fieldUser）和密码对应的字段名（fieldPassword）。此外，还可以选择性的使用PrincipalNameTransformer和PasswordEncoder。以下是SearchModeSearchDatabaseAuthenticationHandler源码中的一段主要代码：
**private static final** String
*SQL_PREFIX* = 
"Select count('x') from ";
@NotNull
**private** String
fieldUser;
@NotNull
**private** String
fieldPassword;
@NotNull
**private** String
tableUsers;
**private** String
sql;
**protectedfinalboolean** authenticateUsernamePasswordInternal(**final**
 UsernamePasswordCredentials credentials) **throws** AuthenticationException {
**final** String transformedUsername = getPrincipalNameTransformer().transform(credentials.getUsername());
**final** String encyptedPassword = getPasswordEncoder().encode(credentials.getPassword());
**finalint** count = getJdbcTemplate().queryForInt(**this**.sql,
           transformedUsername, encyptedPassword);
**return** count > 0;
    }
**publicvoid** afterPropertiesSet()
**throws** Exception {
**this**.sql =
*SQL_PREFIX* + **this**.tableUsers +
" Where " + **this**.fieldUser
        + " = ? And " +
**this**.fieldPassword +
" = ?"; 
    }
       以下是一个使用SearchModeSearchDatabaseAuthenticationHandler的配置示例：
<beanid=*"authenticationManager"*
class=*"org.jasig.cas.authentication.AuthenticationManagerImpl"*>
      ...
<propertyname=*"authenticationHandlers"*>
<list>
            ...
<beanclass=*"org.jasig.cas.adaptors.jdbc.SearchModeSearchDatabaseAuthenticationHandler"*>
<propertyname=*"dataSource"*ref=*"dataSource"*/>
<propertyname=*"passwordEncoder"*ref=*"passwordEncoder"*/>
<propertyname=*"tableUsers"*value=*"t_user"*/><!--
指定从哪个用户表查询用户信息 -->
<propertyname=*"fieldUser"*value=*"username"*/><!--
指定用户名在用户表对应的字段名 -->
<propertyname=*"fieldPassword"*value=*"password"*/><!--
指定密码在用户表对应的字段名 -->
</bean>
            ...
</list>
</property>
      ...
</bean>
       至此，cas-server-support-jdbc中支持JDBC的三个AuthenticationHandler就讲完了。如果用户觉得它们都不能满足你的要求，则还可以选择使用自己实现的AuthenticationHandler。至于其它认证方式，请参考官方文档。
（注：本文是基于cas 3.5.2所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2142616](http://haohaoxuexi.iteye.com/blog/2142616)）
