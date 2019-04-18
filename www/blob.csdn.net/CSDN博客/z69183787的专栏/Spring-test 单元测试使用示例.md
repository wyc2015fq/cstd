# Spring-test 单元测试使用示例 - z69183787的专栏 - CSDN博客
置顶2017年06月14日 11:02:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1954
pom：
<spring.version>4.1.8.RELEASE</spring.version>
<slf4j.version>1.7.10</slf4j.version>
<log4j2.version>2.3</log4j2.version>
 <disruptor.version>3.3.4</disruptor.version>
```
<dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
            <scope>test</scope>
            <exclusions>
                <exclusion>
                    <groupId>junit</groupId>
                    <artifactId>junit</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>org.testng</groupId>
            <artifactId>testng</artifactId>
            <version>6.7</version>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.10</version>
            <scope>test</scope>
        </dependency>
        <!-- log4j2-->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jcl-over-slf4j</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>jul-to-slf4j</artifactId>
            <version>${slf4j.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-api</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-core</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-slf4j-impl</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.logging.log4j</groupId>
            <artifactId>log4j-1.2-api</artifactId>
            <version>${log4j2.version}</version>
        </dependency>
        <dependency>
            <groupId>com.lmax</groupId>
            <artifactId>disruptor</artifactId>
            <version>${disruptor.version}</version>
        </dependency>
        <!-- log4j2-->
```
基类：
```java
/**
 * 测试基类
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(
        locations = {"classpath*:config/spring/common/appcontext-*.xml",
                "classpath*:config/spring/local/appcontext-*.xml",
                "classpath*:config/spring/local/emidas-coupon-*.xml"})
public abstract class AbstractTest {
}
```
测试类：
```java
public class Test extends AbstractTest{
    private static final Logger logger = LogUtil.get(Test.class);
    @Autowired
    private PrizePackageService prizePackageService;
    @Autowired
    private PrizeShopMapper prizeShopMapper;
    @Autowired
    private PrizePoolMapper prizePoolMapper;
    @Autowired
    private PrizePackageMapper prizePackageMapper;
    @Autowired
    private PrizeRemoteService prizeRemoteService;
    @Autowired
    private CacheClient cacheClient;
    @Autowired
    private BonusEventService bonusEventService;
    @Autowired
    private MqService mqService;
    @Autowired
    private PrizeMapper prizeMapper;
    @Autowired
    private Main main;
    @Autowired
    @Qualifier("oldJdbcTemplate")
    private JdbcTemplate jdbcTemplate;
    @Autowired
    private LotteryRemoteService lotteryRemoteService;
    private AtomicReference<Thread> sign =new AtomicReference<Thread>();
    @Autowired
    private HelpUtil helpUtil;
    @Autowired
    private ShopService shopService;
    @org.junit.Test
    public void randomDraw(){
        PrizePackageDTO prizePackageDTO = prizeRemoteService.loadPrizePackage(16302);
        prizePackageDTO.setDrawMethod(DrawMethodEnum.RANDOM);
        Response<PrizePackageDTO> response = prizeRemoteService.updatePrizePackage(prizePackageDTO);
        System.out.println(response);
        //17963 17974 17976
        RandomConfigDTO randomConfigDTO = new RandomConfigDTO();
        randomConfigDTO.setPackageId(16302);
        randomConfigDTO.setPrizeLevel(PrizeLevelEnum.BIG_PRIZE);
        List<PrizeRandomDTO> randoms = new ArrayList<PrizeRandomDTO>();
        PrizeRandomDTO randomDTO = new PrizeRandomDTO();
        randomDTO.setPrizeId(17963);
        randomDTO.setPercent(new BigDecimal("10"));
        randoms.add(randomDTO);
        randomDTO = new PrizeRandomDTO();
        randomDTO.setPrizeId(17974);
        randomDTO.setPercent(new BigDecimal("20"));
        randoms.add(randomDTO);
        randomDTO = new PrizeRandomDTO();
        randomDTO.setPrizeId(17976);
        randomDTO.setPercent(new BigDecimal("30"));
        randoms.add(randomDTO);
        randomDTO = new PrizeRandomDTO();
        randomDTO.setPrizeId(0);
        randomDTO.setPercent(new BigDecimal("40"));
        randoms.add(randomDTO);
        randomConfigDTO.setPrizeRandomDTOs(randoms);
        Response<Boolean> randomResponse = prizeRemoteService.configPrizeRandom(randomConfigDTO);
        System.out.println(randomConfigDTO);
        System.out.println();
    }
```
