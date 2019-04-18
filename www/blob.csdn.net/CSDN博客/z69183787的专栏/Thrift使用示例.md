# Thrift使用示例 - z69183787的专栏 - CSDN博客
2017年06月13日 12:47:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：916
thfit sample ：[http://blog.csdn.net/z69183787/article/details/51333001](http://blog.csdn.net/z69183787/article/details/51333001)
 thrift idl 规范：[http://blog.csdn.net/z69183787/article/details/71515996](http://blog.csdn.net/z69183787/article/details/71515996)
pom：
```
<dependencies>
        <dependency>
            <groupId>org.apache.thrift</groupId>
            <artifactId>libfb303</artifactId>
            <version>0.8.0</version>
        </dependency>
        <dependency>
            <groupId>org.apache.thrift</groupId>
            <artifactId>libthrift</artifactId>
            <version>0.8.0</version>
        </dependency>
    </dependencies>
```
thrift文件：
```
namespace java com.xxxx.emidas.lottery.thrift
enum UserTypeEnum {
    XXXX_USER = 1,
    YYYY_USER = 2,
}
enum CouponIssueResultEnum {
    SUCCESS = 200,
    COUPON_NOT_EXIST = 301,
    COUPON_PLATFORM_ERROR = 302,
    COUPON_NO_STOCK = 303,
    COUPON_TIME_ERROR = 304,
    PARAMETER_ERROR = 401,
    USER_ERROR = 402,
    NO_MOBILE = 403,
    ERROR = 500,
    AUTHORITY_ERROR = 600,
    SECURITY_ERROR = 700,
    EXTERNAL_CHECK_ERROR = 800,
    EXTERNAL_INVOKE_ERROR = 900,
    FREQUENCY = 1000
}
struct UserInfo{
    1:required i32 userId;
    2:required UserTypeEnum userType;
}
struct CouponIssueRequest{
    1:string token;
    2:required UserInfo userInfo;
    3:required list<i32> couponIdList;
    4:required map<string,string> cxMap;
}
struct PrizeBean{
    1:required i32 prizeId;
    2:required string prizeName;
    3:required i32 prizeType;
    4:required i32 attendId;
    5:required i32 winningRecordId;
}
struct CouponIssueResult{
    1:required i32 couponId;
    2:required CouponIssueResultEnum code = CouponIssueResultEnum.SUCCESS;
    3:string externalFailReason;
    4:PrizeBean result;
}
struct CouponIssueResponse{
    1:required i32 successQuantity;
    2:required CouponIssueResultEnum code = CouponIssueResultEnum.SUCCESS;
    3:list<CouponIssueResult> result;
}
service CouponIssueThriftService
{
    CouponIssueResponse issueCoupon(1:CouponIssueRequest request)
}
```
thrift-0.8.0.exe -r -gen java ./xxxx.thrift
转换后的java文件：
![](https://img-blog.csdn.net/20170613124525346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
@Autowired
    private CouponIssueThriftService.Iface thriftService;
    @Test
    public void thriftTest() throws TException, VoucherException {
        Class<?>[] classes = CouponIssueThriftService.class.getClasses();
        for(Class<?> c : classes){
            System.out.println(c.getCanonicalName() + " - "+ c.getSimpleName());
        }
        CouponIssueRequest request = new CouponIssueRequest();
        request.setCxMap(new HashMap<String, String>());
        request.setCouponIdList(Arrays.asList(1, 2, 3, 4, 5));
        UserInfo userInfo = new UserInfo();
        userInfo.setUserId(444444);userInfo.setUserType(UserTypeEnum.XXXX_USER);
        request.setUserInfo(userInfo);
        CouponIssueResponse response = thriftService.issueCoupon(request);
        System.out.println("return -------------------------" + response);
    }
```
to be continued
