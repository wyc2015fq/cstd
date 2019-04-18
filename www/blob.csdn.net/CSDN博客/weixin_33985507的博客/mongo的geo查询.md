# mongo的geo查询 - weixin_33985507的博客 - CSDN博客
2017年04月01日 06:40:32[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## maven
```
<dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-data-mongodb</artifactId>
        </dependency>
```
## domain
```
@Document(collection="coffeeShop")
public class CoffeeShop {
    @Id
    private String id;
    private String name;
    @GeoSpatialIndexed
    private double[] location;
    
    //....
    
}
```
## near查询
> spherical为true则距离单位为空间弧度，false则距离单位为水平单位度
### 度查询
> spherical为false，参数为公里数除以111
```
public GeoResults<CoffeeShop> near2(double[] poi){
        NearQuery near = NearQuery
                .near(new Point(poi[0],poi[1]))
                .spherical(false)
                .num(1);
        GeoResults<CoffeeShop> results = mongoTemplate.geoNear(near, CoffeeShop.class);
        return results;
    }
```
输出
`GeoResults: [averageDistance: 0.08294719588991498, results: GeoResult [content: com.codecraft.domain.CoffeeShop@747f6c5a, distance: 0.08294719588991498, ]]`
> 不指定spherical,默认为false，结果中的dis需要乘以111换算为km
```
public GeoResults<CoffeeShop> near2(double[] poi){
        NearQuery near = NearQuery
                .near(new Point(poi[0],poi[1]))
                .spherical(false)
                .distanceMultiplier(111)
                .num(1);
        GeoResults<CoffeeShop> results = mongoTemplate.geoNear(near, CoffeeShop.class);
        return results;
    }
```
输出
`GeoResults: [averageDistance: 9.207138743780563 org.springframework.data.geo.CustomMetric@28768e25, results: GeoResult [content: com.codecraft.domain.CoffeeShop@310d57b1, distance: 9.207138743780563 org.springframework.data.geo.CustomMetric@28768e25, ]]`
> 即北京阿里绿地中心距离三里屯星巴克距离9km
若要设置最大距离，则
```
public GeoResults<CoffeeShop> near2(double[] poi){
        NearQuery near = NearQuery
                .near(new Point(poi[0],poi[1]))
                .spherical(false)
                .maxDistance(5/111.0d)
                .distanceMultiplier(111)
                .num(1);
        GeoResults<CoffeeShop> results = mongoTemplate.geoNear(near, CoffeeShop.class);
        return results;
    }
```
> 结果为空
### 弧度查询
> 需要数据存储为(经度,纬度)，不然报错
```
org.springframework.dao.DataIntegrityViolationException: Write failed with error code 16755 and error message 'Can't extract geo keys: { _id: ObjectId('58df9c50b45cbc069f6ff548'), _class: "com.codecraft.domain.CoffeeShop", name: "深圳市南山区星巴克(海岸城店)", location: [ 22.52395, 113.943442 ] }  can't project geometry into spherical CRS: [ 22.52395, 113.943442 ]'; nested exception is com.mongodb.WriteConcernException: Write failed with error code 16755 and error message 'Can't extract geo keys: { _id: ObjectId('58df9c50b45cbc069f6ff548'), _class: "com.codecraft.domain.CoffeeShop", name: "深圳市南山区星巴克(海岸城店)", location: [ 22.52395, 113.943442 ] }  can't project geometry into spherical CRS: [ 22.52395, 113.943442 ]'
    at org.springframework.data.mongodb.core.MongoExceptionTranslator.translateExceptionIfPossible(MongoExceptionTranslator.java:85)
```
使用
```
public GeoResults<CoffeeShop> nearRadian(double[] poi){
        NearQuery near = NearQuery
                .near(new Point(poi[0],poi[1]))
                .spherical(true)
                .maxDistance(10,Metrics.KILOMETERS) //MILES以及KILOMETERS自动设置spherical(true)
                .distanceMultiplier(6371)
                .num(1);
        GeoResults<CoffeeShop> results = mongoTemplate.geoNear(near, CoffeeShop.class);
        return results;
    }
```
## test
```
@Test
    public void testInitGeo() {
        //http://map.yanue.net/toLatLng/
        CoffeeShop shop1 = new CoffeeShop("深圳市南山区星巴克(海岸城店)",new double[]{113.943442,22.52395});
        CoffeeShop shop2 = new CoffeeShop("广州市白云区星巴克(万达广场店)",new double[]{113.274643,23.180251});
        CoffeeShop shop3 = new CoffeeShop("北京市朝阳区星巴克(三里屯店)",new double[]{116.484385,39.923778});
        CoffeeShop shop4 = new CoffeeShop("上海市浦东新区星巴克(滨江店)",new double[]{121.638481,31.230895});
        CoffeeShop shop5 = new CoffeeShop("南京市鼓楼区星巴克(山西路店)",new double[]{118.788924,32.075343});
        CoffeeShop shop6 = new CoffeeShop("厦门市思明区星巴克(中华城店)",new double[]{118.089813,24.458157});
        CoffeeShop shop7 = new CoffeeShop("杭州市西湖区星巴克(杭州石函店)",new double[]{120.143005,30.280273});
        coffeeShopDao.save(Lists.newArrayList(shop1,shop2,shop3,shop4,shop5,shop6,shop7));
    }
    @Test
    public void testNear(){
        //经度\纬度
        double[] bjAli = new double[]{116.492644,40.006313};
        double[] szAli = new double[]{113.950723,22.558888};
        double[] shAli = new double[]{121.387616,31.213301};
        double[] hzAli = new double[]{120.033345,30.286398};
        Arrays.asList(bjAli,szAli,shAli,hzAli).stream().forEach(d -> {
            GeoResults<CoffeeShop> results = locationService.nearRadian(d);
            System.out.println(results);
        });
    }
```
## 小结
- 经度、纬度的坐标顺序很容易搞错，x轴是纬度，轴是经度，这也是Point定义的顺序。不过这样的顺序对于使用弧度spherical查询，很容易出错，即spherical查询要求顺序是(经度,纬度)，即数据和参数都是这样的顺序。
- 对于只需要取最近N个的场景，使用num即可；
- 要使用结果中的距离时，需要注意单位换算。
- 对于要指定maxDistance之类的入参时，使用非spherical要注意单位换算；对于使用spherical查询的时候，MILES以及KILOMETERS自动设置spherical(true)，无需关心入参单位转换。
> 另外，对于spherical与非spherical查询，貌似没啥区别，就是spherical在使用时入参无需关心单位换算，稍微方便点。
## doc
- [mongo.geospatial](http://docs.spring.io/spring-data/data-mongo/docs/1.10.0.M1/reference/html/#mongo.geospatial)
- [深入浅出Symfony2 - 结合MongoDB开发LBS应用](http://www.infoq.com/cn/articles/depth-study-of-Symfony2)
- [Units to use for maxdistance and MongoDB?](http://stackoverflow.com/questions/7837731/units-to-use-for-maxdistance-and-mongodb)
- [Spring Data – Part 4: Geospatial Queries with MongoDB](https://blog.codecentric.de/en/2012/02/spring-data-mongodb-geospatial-queries/)
