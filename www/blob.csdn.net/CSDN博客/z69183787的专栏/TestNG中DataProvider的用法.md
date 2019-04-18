# TestNG中DataProvider的用法 - z69183787的专栏 - CSDN博客
2017年01月23日 16:50:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1005
 提供数据的一个测试方法。注解的方法必须返回一个Object[] []，其中每个对象[]的测试方法的参数列表中可以分配。该@Test方法，希望从这个DataProvider的接收数据，需要使用一个dataProvider名称等于这个注解的名字。
**第一种用法 ：**返回Object[][]****
@DataProvider(name="user")
**public**Object[][]user(){
**return****new**Object[][]{
                              {"","","账号不能为空"},
                              {"admin","
 ","密码不能为空"},
                              {" ","a123456","账号不能为空"},
                              {"ad ","123456","账号“ad”不存在"},
                              {"admin","12345","密码错误"},
                    };
           }
     DataProvider返回的是一个Object的二维数组，二维数组中的每个一维数组都会传递给调用函数，作为参数使用。运行的时候，会发现， @Test标识的test method被执行的次数和object[][]包含的一维数组的个数是一致的，而@Test标识的函数的参数个数，也和object内一维数组内的元素数是一致的。
@Test(dataProvider="user")
**public****void**testEcafeLogin(StringuserName, 
 StringpassWord, StringexpectText)
 {
                   loginPageClassloginpage=**new**loginPageClass();
loginpage.load();
loginpage.login(userName,passWord);
                    Assert.*assertEquals*(loginpage.getString(),expectText);
loginpage.logout();
          }    
