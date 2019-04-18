# redis+lua与秒杀--lua入门 - z69183787的专栏 - CSDN博客
2018年08月08日 22:01:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：70
个人分类：[Redis-基础																[算法-抽奖概率/秒杀](https://blog.csdn.net/z69183787/article/category/5636095)](https://blog.csdn.net/z69183787/article/category/2923783)
## 秒杀场景
用户秒杀，后端服务接收到请求之后的操作步骤：
- 校验请求参数
- 解析用户信息
- 解析所秒商品信息，及秒商品数量
- 校验用户在限定的时间内是否秒过
- 校验库存是否充足
- 扣库存
- 记录用户秒单记录
- 异步写订单记录
- 返回请求
## 问题
在上面所描述的步骤中 校验库存与扣库存，存在先后顺序，但是并没有原子性。在关系数据库中，可以通过事务来解决这个问题，但是关系数据库性能有瓶颈。当然在请求量可以控制的情况下，使用关系数据库的乐观锁，也是可以的。就像现在我在公司所使用的方案为秒杀单接口进行限流+数据库的乐观锁就完全解决这个问题。但是这个通过限流来解决了请求的峰值。 
如果我们将校验库存，扣库存这段逻辑放在redis上执行，也存在原子性问题。redis帮我们也提供了事务功能，但是这个事务功能太差强人意了。所以在本文中，我们希望通过redis+lua的方式解决这个问题.
## 解决方案
使用redis执行lua的原子性，将上面的步骤在redis中执行,前提条件即秒杀商品及库存数据全部都要推到redis中。准备工作：
- 所需要秒杀商品 及库存数据推到redis;
业务代码逻辑：
- 校验请求参数
- 解析用户信息
- 解析所秒商品信息，及所秒商品数量
- 使用lua编写相应的脚本，放到redis去执行，返回0表示已秒过，返回2库存不足，返回1扣库存成功；
- 异步写入订单记录
- 返回请求
lua脚本需要完成的业务逻辑：
- 用户是否忆秒订单判断
- 校验库存
- 扣库存
- 设置抢单成功用户标识
- 返回结果
lua脚本内容
```
--判断该用户是否秒杀过，如果已秒则不允许再秒
     local hasSecKill=redis.call('sismember',ARGV[1],KEYS[1])       
    if hasSecKill ~=0 then
        return 0;
    end
     --设置抢单标识
    --redis.call('set',KEYS[1],1);
    --设置过期时间
    --redis.call('expire',KEYS[1],30000);
    --check库存
    for goodsNum=2,#KEYS  do
        local goodsStock=redis.call('get',KEYS[goodsNum]);
        if goodsStock< ARGV[goodsNum] then
            return 2;
        end
    end
    --扣库存
    for goodsNum=2,#KEYS do
        redis.call('DECRBY',KEYS[goodsNum],ARGV[goodsNum]);
    end
    -- 所有抢单成功的用户
    redis.call('sadd',ARGV[#ARGV],KEYS[1]);
    return 1;
```
当然你可以将这段lua放在java代码中：
```
public boolean orderSecKill(SecKillPara para, Jedis jedis) {
    /* 0,已抢过单
     * 2.库存不足
     * 1.抢单成功
     */
    Long customerId = para.getCustomerId();
    String script =
            " local ismeber=redis.call('sismember',ARGV[1], KEYS[1]) "
            +" if ismeber ~= 0 then " // --判断该用户是否秒杀过，如果已秒则不允许再秒
            + " return 0 " 
            + " end "
          //  + " redis.call('set',KEYS[1],1) " // --设置抢单标识
           // + " redis.call('expire',KEYS[1],2000) " // --设置过期时间
            // --check库存
            + " for goodsNum=2,#KEYS do " 
            + "     local goodsStock=redis.call('get',KEYS[goodsNum]) "
            + "     if goodsStock < ARGV[goodsNum] then " 
            + "        return 2;" 
            + "      end " 
            + " end    "
            // --扣库存
            + " for goodsNum=2,#KEYS do " 
            + "   redis.call('DECRBY',KEYS[goodsNum],ARGV[goodsNum]) " 
            + " end "
            // -- 所有抢单成功的用户
            + " redis.call('sadd',ARGV[1],KEYS[1])" 
            + " return 1;";
    List<String> keys = new ArrayList<String>();
    List<String> args = new ArrayList<String>();
    keys.add(customerId.toString());
    args.add("all_order_user");// 所有抢单的用户
    for (Map.Entry<Long, Integer> goods : para.getGoodsWithAmount().entrySet()) {
        keys.add(goods.getKey().toString());
        args.add(goods.getValue().toString());
    }
    Object o = jedis.eval(script, keys, args);
     Long result=Long.valueOf(o.toString());
    if(result==1){
        return true;
    }
    return false;
}
     public static final class SecKillPara{
    private final Long customerId;//抢单人
    private final Map<Long/*所抢skuId*/,Integer /*所抢sku个数*/> goodsWithAmount;
    public SecKillPara(Long customerId, Map<Long, Integer> goodsWithAmount) {
        super();
        this.customerId = customerId;
        this.goodsWithAmount = goodsWithAmount;
    }
    public Long getCustomerId() {
        return customerId;
    }
    public Map<Long, Integer> getGoodsWithAmount() {
        return goodsWithAmount;
    }  
}
这个只是我作的一个小demo，并没有进行强测试，如果需要使用的同学则需要好好的测试哦。
```
