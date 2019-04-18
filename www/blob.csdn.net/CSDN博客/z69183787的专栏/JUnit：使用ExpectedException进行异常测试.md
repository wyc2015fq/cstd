# JUnit：使用ExpectedException进行异常测试 - z69183787的专栏 - CSDN博客
2015年12月31日 16:25:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6110
Junit中有好几种测试异常的方法。就像我在 [前面几篇文章](http://it.deepinmind.com/) 中写道的那样，我比较喜欢使用org.junit.rules.ExpectedException规则。一般来说，ExpectedException规则是org.junit.Before,
 org.junit.After, org.junit.BeforeClass，或者org.junit.AfterClass注解的一种替代方式 ，但是它们的功能更为强大，也更容易在多个工程间或者不同类中进行共享。本文中我会介绍下org.junit.rules.ExpectedException规则的一些高级用法。 
验证异常信息
标准的JUnit的org.junit.Test注解提供了一个expected属性，你可以用它来指定一个Throwble类型，如果方法调用中抛出了这个异常，这条测试用例就算通过了。很多情况下有它就足够了，不过如果你想验证下异常的信息——你就得另寻出路了。使用ExpectedException来实现这个非常简单： 
```java
public class ExpectedExceptionsTest {
  @Rule
  public ExpectedException thrown = ExpectedException.none();
  @Test
  public void verifiesTypeAndMessage() {
    thrown.expect(RuntimeException.class);
    thrown.expectMessage("Runtime exception occurred");
    throw new RuntimeException("Runtime exception occurred");
  }
}
```
在这段代码中，我们期望抛出的异常中包含指定的信息。和只匹配类型相比，这样做更安全。为什么？我们假设我们有这么个ExceptionThrower： 
```java
class ExceptionsThrower {
    void throwRuntimeException(int i) {
        if (i <= 0) {
            throw new RuntimeException("Illegal argument: i must be <= 0");
        }
        throw new RuntimeException("Runtime exception occurred");
    }
}
```
可以看到，抛出的两个异常都是RuntimeException，因此如果我们不检查异常信息的话，我们无法百分百确定方法抛出的异常到底是哪个。那么下面这个测试用例就会通过了： 
@Test
publicvoid runtimeExceptionOccurs() {
thrown.expect(RuntimeException.class);
// opposite to expectedexceptionsThrower.throwRuntimeException(0);
}
@Test
publicvoid illegalArgumentExceptionOccurs() {
thrown.expect(RuntimeException.class);
// opposite to expectedexceptionsThrower.throwRuntimeException(1);
}而检查异常信息的话就会解决这个问题，能确保是你想要的那个异常。单就这点来说，异常规则就要秒杀@Test注解里面的expected属性了。 
不过如果你想验证的异常信息非常复杂呢？ExpectedException允许你传一个Hamcrest匹配器（matcher)给expectMessage方法（而不是一个字符串）。来看下这个例子： 
@Test
publicvoid verifiesMessageStartsWith() {
    thrown.expect(RuntimeException.class);
    thrown.expectMessage(startsWith("Illegal argument:"));
    thrownew RuntimeException("Illegal argument: i must be <= 0");
}当然了，你还可以指定你自己的匹配器来进行消息验证。再看下这个例子。 
```java
@Test
public void verifiesMessageMatchesPattern() {
  thrown.expect(RuntimeException.class);
  thrown.expectMessage(new MatchesPattern("[Ii]llegal .*"));
  throw new RuntimeException("Illegal argument: i must be <= 0");
}
class MatchesPattern extends TypeSafeMatcher<String> {
  private String pattern;
  public MatchesPattern(String pattern) {
    this.pattern = pattern;
  }
  @Override
  protected boolean matchesSafely(String item) {
    return item.matches(pattern);
  }
  @Override
  public void describeTo(Description description) {
    description.appendText("matches pattern ")
      .appendValue(pattern);
  }
  @Override
  protected void describeMismatchSafely(String item, Description mismatchDescription) {
    mismatchDescription.appendText("does not match");
  }
}
```
校验异常对象
在有些场景下光匹配异常信息还不够。可能你的异常中有自定义的一些方法，你也想验证一下它们。完全没问题。给ExpectedException的expect方法指定一个matcher就搞定了。 
```java
@Test
public void verifiesCustomException() {
  thrown.expect(RuntimeException.class);
  thrown.expect(new ExceptionCodeMatches(1));
  throw new CustomException(1);
}
class CustomException extends RuntimeException {
  private final int code;
  public CustomException(int code) {
    this.code = code;
  }
  public int getCode() {
    return code;
  }
}
class ExceptionCodeMatches extends TypeSafeMatcher<CustomException> {
  private int code;
  public ExceptionCodeMatches(int code) {
    this.code = code;
  }
  @Override
  protected boolean matchesSafely(CustomException item) {
    return item.getCode() == code;
  }
  @Override
  public void describeTo(Description description) {
    description.appendText("expects code ")
        .appendValue(code);
  }
  @Override
  protected void describeMismatchSafely(CustomException item, Description mismatchDescription) {
    mismatchDescription.appendText("was ")
        .appendValue(item.getCode());
  }
}
```
注意了，这里同时实现了describeTo和describeMismatchSafely 两个方法。因为我希望如果测试失败的话输出的错误信息能看起来好一些
 。看下它的输出： 
java.lang.AssertionError:
Expected: (an instance of java.lang.RuntimeException and expects code <1>)
     but: expects code <1> was <2>检查异常原因
ExpectedException还有一个作用就是用来检查异常原因。这也可以通过自定义的匹配器来完成： 
```java
@Test
public void verifiesCauseTypeAndAMessage() {
  thrown.expect(RuntimeException.class);
  thrown.expectCause(new CauseMatcher(IllegalStateException.class, "Illegal state"));
  throw new RuntimeException("Runtime exception occurred",
      new IllegalStateException("Illegal state"));
}
private static class CauseMatcher extends TypeSafeMatcher<Throwable> {
  private final Class<? extends Throwable> type;
  private final String expectedMessage;
  public CauseMatcher(Class<? extends Throwable> type, String expectedMessage) {
    this.type = type;
    this.expectedMessage = expectedMessage;
  }
  @Override
  protected boolean matchesSafely(Throwable item) {
    return item.getClass().isAssignableFrom(type)
        && item.getMessage().contains(expectedMessage);
  }
  @Override
  public void describeTo(Description description) {
    description.appendText("expects type ")
        .appendValue(type)
        .appendText(" and a message ")
        .appendValue(expectedMessage);
  }
}<2>
```
总结
ExpectedException规则是JUnit一个强大的特性。再加上Hamcrest匹配器，你的异常测试用例将变得更加健壮，并且可重复使用。
