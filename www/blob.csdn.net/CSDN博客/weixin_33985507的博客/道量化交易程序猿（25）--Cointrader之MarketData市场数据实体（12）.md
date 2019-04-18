# 道量化交易程序猿（25）--Cointrader之MarketData市场数据实体（12） - weixin_33985507的博客 - CSDN博客
2015年09月20日 20:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

转载注明出处：[http://blog.csdn.net/minimicall](http://blog.csdn.net/minimicall)。[http://cloudtrade.top/](http://cloudtrade.top/)
前面一节我们说到了远端事件。当中。市场数据就属于远端事件。市场数据有什么？我们通过代码来回答这个问题：
```java
package org.cryptocoinpartners.schema;
import javax.annotation.Nullable;
import javax.persistence.Entity;
import javax.persistence.ManyToOne;
import org.joda.time.Instant;
/**
 * @author Tim Olson
 */
@Entity //实体，在数据库中会创建表格
public abstract class MarketData extends RemoteEvent {
    protected MarketData(Instant time, @Nullable String remoteKey, Market market) {
        this(time, Instant.now(), remoteKey, market);
    }
    protected MarketData(Instant time, Instant timeReceived, String remoteKey, Market market) {
        super(time, timeReceived, remoteKey);
        this.market = market;
    }
    @ManyToOne(optional = false)
    public Market getMarket() {
        return market;
    }
    // JPA
    protected MarketData() {
        super();
    }
    protected void setMarket(Market market) {
        this.market = market;
    }
    private Market market;//市场
}
```
市场数据里面仅仅有一个成员。市场。
也即是说市场数据是某一时刻的市场。
我们接下来看看市场又有什么数据，Market。
我们通过凝视代码来学习。
```java
package org.cryptocoinpartners.schema;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import javax.persistence.Basic;
import javax.persistence.Cacheable;
import javax.persistence.Entity;
import javax.persistence.Index;
import javax.persistence.ManyToOne;
import javax.persistence.NamedQuery;
import javax.persistence.NoResultException;
import javax.persistence.PostPersist;
import javax.persistence.Table;
import javax.persistence.Transient;
import org.cryptocoinpartners.enumeration.FeeMethod;
import org.cryptocoinpartners.util.PersistUtil;
import org.cryptocoinpartners.util.RemainderHandler;
/**
 * Represents the possibility to trade one Asset for another at a specific Exchange.
 *
 * @author Tim Olson
 */
@Entity //表格
@Cacheable // 缓存
@NamedQuery(name = "Market.findByMarket", query = "select m from Market m where exchange=?1 and listing=?2") // 命名查询
@Table(indexes = { @Index(columnList = "exchange"), @Index(columnList = "listing"), @Index(columnList = "active") }) //编织索引
public class Market extends EntityBase {
    public static Collection<Market> findAll() {//查询全部市场数据
        return PersistUtil.queryList(Market.class, "select m from Market m");
    }
    /** adds the Market to the database if it does not already exist */
    public static Market findOrCreate(Exchange exchange, Listing listing) {
        return findOrCreate(exchange, listing, listing.getPriceBasis(), listing.getVolumeBasis());
    }
    @PostPersist
    private void postPersist() {
        //PersistUtil.detach(this);
    }
//查询或创建市场数据
    public static Market findOrCreate(Exchange exchange, Listing listing, double quoteBasis, double volumeBasis) {
        // final String queryStr = "select m from Market m where exchange=?1 and listing=?
```
2";
        try {
            return PersistUtil.namedQueryOne(Market.class, "Market.findByMarket", exchange, listing);
        } catch (NoResultException e) {
            final Market ml = new Market(exchange, listing, quoteBasis, volumeBasis);
            PersistUtil.insert(ml);
            return ml;
        }
    }
    /**
     @return active Markets for the given exchange
     */
    public static Collection<Market> find(Exchange exchange) {
        return PersistUtil.queryList(Market.class, "select s from Market s where exchange=?
1 and active=?2", exchange, true);
    }
    /**
     @return active Markets for the given listing
     */
    public static Collection<Market> find(Listing listing) {
        return PersistUtil.queryList(Market.class, "select s from Market s where listing=?1 and active=?2", listing, true);
    }
    @ManyToOne(optional = false)
    public Exchange getExchange() {
        return exchange;
    }
    @ManyToOne(optional = false)
    public Listing getListing() {
        return listing;
    }
    @Basic(optional = false)
    public double getPriceBasis() {
        return listing.getPriceBasis() == 0 ?
 priceBasis : listing.getPriceBasis();
    }
    @Transient
    public int getScale() {
        int length = (int) (Math.log10(getPriceBasis()));
        return length;
    }
    @Basic(optional = false)
    public double getVolumeBasis() {
        return listing.getVolumeBasis() == 0 ? volumeBasis : listing.getVolumeBasis();
    }
    /** @return true iff the Listing is currently traded at the Exchange.  The Market could have been retired. */
    public boolean isActive() {
        return active;
    }
    @Transient
    public Asset getBase() {
        return listing.getBase();
    }
    @Transient
    public Asset getQuote() {
        return listing.getQuote();
    }
    @Transient
    public int getMargin() {
        return listing.getMargin() == 0 ? exchange.getMargin() : listing.getMargin();
    }
    @Transient
    public double getFeeRate() {
        return listing.getFeeRate() == 0 ? exchange.getFeeRate() : listing.getFeeRate();
    }
    @Transient
    public FeeMethod getMarginFeeMethod() {
        return listing.getMarginFeeMethod() == null ? exchange.getMarginFeeMethod() : listing.getMarginFeeMethod();
    }
    @Transient
    public FeeMethod getFeeMethod() {
        return listing.getFeeMethod() == null ?
 exchange.getFeeMethod() : listing.getFeeMethod();
    }
    @Transient
    public double getMultiplier() {
        return listing.getMultiplier();
    }
    @Transient
    public double getTickValue() {
        return listing.getTickValue();
    }
    @Transient
    public double getContractSize() {
        return listing.getContractSize();
    }
    @Transient
    public double getTickSize() {
        return listing.getTickSize();
    }
    @Transient
    public Asset getTradedCurrency() {
        return listing.getTradedCurrency();
    }
    @Transient
    public String getSymbol() {
        return exchange.toString() + ':' + listing.toString();
    }
    @Override
    public String toString() {
        return getSymbol();
    }
    public static Market forSymbol(String marketSymbol) {
        for (Market market : findAll()) {
            if (market.getSymbol().equalsIgnoreCase(marketSymbol))
                return market;
        }
        return null;
    }
    public static List<String> allSymbols() {
        List<String> result = new ArrayList<>();
        List<Market> markets = PersistUtil.queryList(Market.class, "select m from Market m");
        for (Market market : markets)
            result.add((market.getSymbol()));
        return result;
    }
    public static class MarketAmountBuilder {
        public DiscreteAmount fromPriceCount(long count) {
            return priceBuilder.fromCount(count);
        }
        public DiscreteAmount fromVolumeCount(long count) {
            return volumeBuilder.fromCount(count);
        }
        public DiscreteAmount fromPrice(BigDecimal amount, RemainderHandler remainderHandler) {
            return priceBuilder.fromValue(amount, remainderHandler);
        }
        public DiscreteAmount fromVolume(BigDecimal amount, RemainderHandler remainderHandler) {
            return volumeBuilder.fromValue(amount, remainderHandler);
        }
        public MarketAmountBuilder(double priceBasis, double volumeBasis) {
            this.priceBuilder = DiscreteAmount.withBasis(priceBasis);
            this.volumeBuilder = DiscreteAmount.withBasis(volumeBasis);
        }
        private final DiscreteAmount.DiscreteAmountBuilder priceBuilder;
        private final DiscreteAmount.DiscreteAmountBuilder volumeBuilder;
    }
    public MarketAmountBuilder buildAmount() {
        if (marketAmountBuilder == null)
            marketAmountBuilder = new MarketAmountBuilder(getPriceBasis(), getVolumeBasis());
        return marketAmountBuilder;
    }
    // JPA
    protected Market() {
    }
    protected void setExchange(Exchange exchange) {
        this.exchange = exchange;
    }
    protected void setListing(Listing listing) {
        this.listing = listing;
    }
    protected void setActive(boolean active) {
        this.active = active;
    }
    protected void setPriceBasis(double quoteBasis) {
        this.priceBasis = quoteBasis;
    }
    protected void setVolumeBasis(double volumeBasis) {
        this.volumeBasis = volumeBasis;
    }
    private Market(Exchange exchange, Listing listing, double priceBasis, double volumeBasis) {
        this.exchange = exchange;
        this.listing = listing;
        this.priceBasis = priceBasis;
        this.volumeBasis = volumeBasis;
        this.active = true;
    }
    private Exchange exchange;//交易所
    private Listing listing;//挂牌清单
    private double priceBasis;//基准价格
    private double volumeBasis;//基准量
    private boolean active;//是否活跃
    private MarketAmountBuilder marketAmountBuilder;
}

版权声明：本文博主原创文章。博客，未经同意不得转载。
