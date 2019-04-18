# Builder设计模式结合lombok减少过多传参 - z69183787的专栏 - CSDN博客
2019年01月04日 19:48:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：44
个人分类：[工具-Lombok																[设计模式](https://blog.csdn.net/z69183787/article/category/2176787)](https://blog.csdn.net/z69183787/article/category/5834225)
[https://blog.csdn.net/j16421881/article/details/79368004](https://blog.csdn.net/j16421881/article/details/79368004)
建造者模式将复杂对象的创建过程简化，用来传参也很合适。lombok的主要作用是通过一些注解，消除样板式代码，更多详见 lombok官网。该框架已经内置了这种模式。
  过多的传参，可读性差，尤其是参数类型一致顺序还容易写颠倒。下面以一则不友好的传参为例展示一下，如何通过lombok迅速重构。
```java
public class Mytest {
  public static void main(String[] args) {
    pay("2018-02-25", "小明", "2", "3");
  }
  public static void pay(String date, String name, String term, String amount) {
    System.out.println(date + "日，借款人" + name + "在第" + term + "期还款" + amount);
  }
}
```
首先将参数封装。 
Builder跟Date分别是两个聚合注解，省去了手写getter、setter跟builder方法。
```java
import lombok.Builder;
import lombok.Data;
@Data
@Builder
public class BidBuilder {
  private String date;
  private String name;
  private String term;
  private String amount;
}
```
加上该注解后，通过反编译发现lombok生成的实际代码如下
```java
public class BidBuilder
{
  private String date;
  private String name;
  private String term;
  private String amount;
  public void setDate(String date)
  {
    this.date = date;
  }
  public void setName(String name)
  {
    this.name = name;
  }
  public void setTerm(String term)
  {
    this.term = term;
  }
  public void setAmount(String amount)
  {
    this.amount = amount;
  }
  public boolean equals(Object o)
  {
    if (o == this) {
      return true;
    }
    if (!(o instanceof BidBuilder)) {
      return false;
    }
    BidBuilder other = (BidBuilder)o;
    if (!other.canEqual(this)) {
      return false;
    }
    Object this$date = getDate();Object other$date = other.getDate();
    if (this$date == null ? other$date != null : !this$date.equals(other$date)) {
      return false;
    }
    Object this$name = getName();Object other$name = other.getName();
    if (this$name == null ? other$name != null : !this$name.equals(other$name)) {
      return false;
    }
    Object this$term = getTerm();Object other$term = other.getTerm();
    if (this$term == null ? other$term != null : !this$term.equals(other$term)) {
      return false;
    }
    Object this$amount = getAmount();Object other$amount = other.getAmount();return this$amount == null ? other$amount == null : this$amount.equals(other$amount);
  }
  protected boolean canEqual(Object other)
  {
    return other instanceof BidBuilder;
  }
  public int hashCode()
  {
    int PRIME = 59;int result = 1;Object $date = getDate();result = result * 59 + ($date == null ? 43 : $date.hashCode());Object $name = getName();result = result * 59 + ($name == null ? 43 : $name.hashCode());Object $term = getTerm();result = result * 59 + ($term == null ? 43 : $term.hashCode());Object $amount = getAmount();result = result * 59 + ($amount == null ? 43 : $amount.hashCode());return result;
  }
  public String toString()
  {
    return "BidBuilder(date=" + getDate() + ", name=" + getName() + ", term=" + getTerm() + ", amount=" + getAmount() + ")";
  }
  public static class BidBuilderBuilder
  {
    private String date;
    private String name;
    private String term;
    private String amount;
    public String toString()
    {
      return "BidBuilder.BidBuilderBuilder(date=" + this.date + ", name=" + this.name + ", term=" + this.term + ", amount=" + this.amount + ")";
    }
    public BidBuilder build()
    {
      return new BidBuilder(this.date, this.name, this.term, this.amount);
    }
    public BidBuilderBuilder amount(String amount)
    {
      this.amount = amount;return this;
    }
    public BidBuilderBuilder term(String term)
    {
      this.term = term;return this;
    }
    public BidBuilderBuilder name(String name)
    {
      this.name = name;return this;
    }
    public BidBuilderBuilder date(String date)
    {
      this.date = date;return this;
    }
  }
  BidBuilder(String date, String name, String term, String amount)
  {
    this.date = date;this.name = name;this.term = term;this.amount = amount;
  }
  public static BidBuilder.BidBuilderBuilder builder()
  {
    return new BidBuilder.BidBuilderBuilder();
  }
  public String getDate()
  {
    return this.date;
  }
  public String getName()
  {
    return this.name;
  }
  public String getTerm()
  {
    return this.term;
  }
  public String getAmount()
  {
    return this.amount;
  }
}
```
通过lombok几行代码就实现了大量需要手动完成的模版代码。重构后如下
```java
public class Mytest2 {
  public static void main(String[] args) {
    BidBuilder bidBuilder =
        BidBuilder.builder().date("2018-02-25").name("小明").term("2").amount("3").build();
    pay(bidBuilder);
  }
  public static void pay(BidBuilder bidBuilder) {
    System.out.println(
        bidBuilder.getDate()
            + "日，借款人"
            + bidBuilder.getName()
            + "在第"
            + bidBuilder.getTerm()
            + "期还款"
            + bidBuilder.getAmount());
  }
}
```
后续若需要添加新的参数，只需更改BidBuilder，比如添加下次还款时间，只需要添加一行.
```java
@Data
@Builder
public class BidBuilder {
  // 篇幅限制，省略其他参数
  // 下次还款时间 
  private String nextDate;
}
 // 然后动态生成新的Builder
 BidBuilder bidBuilder =
        BidBuilder.builder()
            .date("2018-02-25")
            .name("小明")
            .term("2")
            .amount("100")
            .nextDate("2018-03-25")
            .build();
```
