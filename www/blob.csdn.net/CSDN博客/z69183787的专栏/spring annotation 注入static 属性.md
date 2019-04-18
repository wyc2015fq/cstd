# spring annotation 注入static 属性 - z69183787的专栏 - CSDN博客
2014年05月09日 09:18:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13277
第一种方法:
@Component
public class UserUtils
{
  private static UserAccessor userAccessor;
  /**
   * Sets the user DAO. This method should never be called except by Spring
   * @param userAccessor The user accessor to set
   */
  @Autowired(required = true)
  public void setUserAccessor(userAccessor UserAccessor) {
    UserUtils.userAccessor = userAccessor;
  }
}
第二种方法:
@Component
public class UserUtils
{
    private static UserAccessor userAccessor;
    @Autowired(required = true)
    public UserUtils(@Qualifier(“userAccessorImpl”) userAccessor UserAccessor) {
         UserUtils.userAccessor = userAccessor;
    }
}
