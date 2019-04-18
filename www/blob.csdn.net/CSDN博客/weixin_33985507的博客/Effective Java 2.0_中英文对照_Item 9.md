# Effective Java 2.0_中英文对照_Item 9 - weixin_33985507的博客 - CSDN博客
2016年12月02日 23:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
文章作者：Tyan
博客：[noahsnail.com](https://link.jianshu.com?t=http://noahsnail.com)  |  [CSDN](https://link.jianshu.com?t=http://blog.csdn.net/quincuntial)  |  [简书](https://www.jianshu.com/users/7731e83f3a4e/latest_articles)
## Item 9: Always override hashCode when you override equals
A common source of bugs is the failure to override the `hashCode` method. **You must override `hashCode` in every class that overrides `equals`.** Failure to do so will result in a violation of the general contract for `Object.hashCode`, which will prevent your class from functioning properly in conjunction with all hash-based collections, including `HashMap`, `HashSet`, and `Hashtable`.
一个常见的错误来源是没有重写`hashCode`方。**在每个重写`equals`方法的类中，你必须重写`hashCode`方法。**不这样做会违反`Object.hashCode`的通用约定，这会使你的类不能在功能上与所有基于哈希的集合进行恰当的结合，包括`HashMap`，`HashSet`和`Hashtable`。
Here is the contract, copied from the `Object` specification [JavaSE6]:
下面是这些约定，从`Object`规范中拷贝的[JavaSE6]：
- 
Whenever it is invoked on the same object more than once during an execution of an application, the `hashCode` method must consistently return the same integer, provided no information used in `equals` comparisons on the object is modified. This integer need not remain consistent from one execution of an application to another execution of the same application.
- 
假设同一个对象在进行`equals`比较时没有修改信息，那么在一个应用执行期间，无论什么时候对同一个对象调用多次`hashCode`方法，它的`hashCode`方法都必须返回一个一致的整数。这个整数在应用多次执行期间不必保持一致。
- 
If two objects are equal according to the `equals`(`Object`) method, then calling the `hashCode` method on each of the two objects must produce the same integer result.
- 
如果两个对象根据`equals`(`Object`)方法是相等的，那么调用每一个对象的`hashCode`方法必须产生同样的整数结果。
- 
It is not required that if two objects are unequal according to the `equals`(`Object`) method, then calling the `hashCode` method on each of the two objects must produce distinct integer results. However, the programmer should be aware that producing distinct integer results for unequal objects may improve the performance of hash tables.
- 
如果两个对象根据`equals`(`Object`)方法不相等，不要求调用每一个对象的`hashCode`方法必须产生同样的整数结果。然而，程序员应该意识到对于不等的对象产生不同的整数结果可能改善哈希表的性能。
**The key provision that is violated when you fail to override `hashCode` is the second one: equal objects must have equal hash codes.** Two distinct instances may be logically equal according to a class’s `equals` method, but to `Object`’s `hashCode` method, they’re just two objects with nothing much in common. Therefore `Object`’s `hashCode` method returns two seemingly random numbers instead of two equal numbers as required by the contract.
**当不重写`hashCode`时，违反的第二条是关键约定：相等对象必须具有相等的哈希值。**两个不同的对象根据类的`equals`方法可能在逻辑上是相等的，但对于`Object`的`hashCode`方法，它们是两个对象，没有共同的东西，因此`Object`的`hashCode`方法返回两个看似随机的数字来代替约定要求的相等数字。
For example, consider the following simplistic `PhoneNumber` class, whose `equals` method is constructed according to the recipe in Item 8:
例如，考虑下面简化的`PhoneNumber`类，它的`equals`方法是根据Item 8的流程构建的：
```
public final class PhoneNumber {
    private final short areaCode;
    private final short prefix;
    private final short lineNumber;
    public PhoneNumber(int areaCode, int prefix, int lineNumber) {
        rangeCheck(areaCode, 999, "area code");
        rangeCheck(prefix, 999, "prefix");
        rangeCheck(lineNumber, 9999, "line number");
        this.areaCode = (short) areaCode;
        this.prefix = (short) prefix;
        this.lineNumber = (short) lineNumber;
    }
    private static void rangeCheck(int arg, int max, String name) {
        if (arg < 0 || arg > max)
            throw new IllegalArgumentException(name + ": " + arg);
    }
    @Override
    public boolean equals(Object o) {
        if (o == this)
            return true;
        if (!(o instanceof PhoneNumber))
            return false;
        PhoneNumber pn = (PhoneNumber) o;
        return pn.lineNumber == lineNumber && pn.prefix == prefix
                && pn.areaCode == areaCode;
    }
    // Broken - no hashCode method!
    ... // Remainder omitted
}
```
Suppose you attempt to use this class with a `HashMap`:
假设你试图在`HashMap`中使用这个类：
```
Map<PhoneNumber, String> m = new HashMap<PhoneNumber, String>();
m.put(new PhoneNumber(707, 867, 5309), "Jenny");
```
At this point, you might expect `m.get(new PhoneNumber(707, 867, 5309))` to return "Jenny", but it returns null. Notice that two `PhoneNumber` instances are involved: one is used for insertion into the `HashMap`, and a second, equal, instance is used for (attempted) retrieval. The `PhoneNumber` class’s failure to override `hashCode` causes the two equal instances to have unequal hash codes, in violation of the `hashCode` contract. Therefore the `get` method is likely to look for the phone number in a different hash bucket from the one in which it was stored by the `put` method. Even if the two instances happen to hash to the same bucket, the `get` method will almost certainly return null, as `HashMap` has an optimization that caches the hash code associated with each entry and doesn’t bother checking for object equality if the hash codes don’t match.
这时候，你可能期待`m.get(new PhoneNumber(707, 867, 5309))`返回`Jenny`，但它返回空。注意涉及到两个`PhoneNumber`实例：一个用来插入到`HashMap`，第二个相等的实例用来（试图）检索。`PhoneNumber`类没有重写`hashCode`方法引起两个相等的实例有不等的哈希值，违反了`hashCode`约定。因此`get`方法可能在一个与`put`方法储存的哈希桶不同的哈希桶中查找电话号码。即使两个实例碰到哈希到同一个桶中，`get`几乎必定返回空，因为`HashMap`缓存了每个输入相关的哈希吗，如果哈希码不匹配，不会检查对象的相等性。
Fixing this problem is as simple as providing a proper `hashCode` method for the `PhoneNumber` class. So what should a `hashCode` method look like? It’s trivial to write one that is legal but not good. This one, for example, is always legal but should never be used:
修正这个问题很简单，为`PhoneNumber`类提供一个合适的`hashCode`方法。因此`hashCode`方法应该看起来是什么样的？编写一个合法但不好的方法是没意义的。例如，下面的方法合法但从未被用到：
```
// The worst possible legal hash function - never use!
@Override 
public int hashCode() { 
    return 42; 
}
```
It’s legal because it ensures that equal objects have the same hash code. It’s atrocious because it ensures that every object has the same hash code. Therefore, every object hashes to the same bucket, and hash tables degenerate to linked lists. Programs that should run in linear time instead run in quadratic time. For large hash tables, this is the difference between working and not working.
它是合法的因为它保证了相等的对象有同样的哈希值。它是极差的因为它保证了每个对象都有同样的哈希值。因此，每个对象哈希到相同的桶中，哈希表退化成链表。程序从应该运行在线性时间内变成运行在平方时间内。对于打的哈希表，这是工作和不工作的区别。
A good hash function tends to produce unequal hash codes for unequal objects. This is exactly what is meant by the third provision of the `hashCode` contract. Ideally, a hash function should distribute any reasonable collection of unequal instances uniformly across all possible hash values. Achieving this ideal can be difficult. Luckily it’s not too difficult to achieve a fair approximation. Here is a simple recipe:
一个好的哈希函数对于不等的对象趋向于产生不等的哈希值。这与`hashCode`约定中的第三条是一个意思。理想情况下，一个哈希函数应该将任何合理的不等的实例集合，统一散列在所有可能的哈希值上。要取得这样的目标是非常困难的。幸运的是不难取得一个公平的近似。下面是简单的流程：
- 
Store some constant nonzero value, say, 17, in an `int` variable called `result`.
- 
For each significant field `f` in your object (each field taken into account by the `equals` method, that is), do the following:
a. Compute an `int` hash code `c` for the field:
i. If the field is a `boolean`, compute `(f ? 1 : 0)`.
ii. If the field is a `byte`, `char`, `short`, or `int`, compute `(int) f`.
iii. If the field is a `long`, compute `(int)(f^(f>>>32))`.
iv. If the field is a `float`, compute `Float.floatToIntBits(f)`.
v. If the field is a `double`, compute `Double.doubleToLongBits(f)`, and then hash the resulting `long` as in step 2.a.iii.
vi. If the field is an object reference and this class’s `equals` method compares the field by recursively invoking `equals`, recursively invoke `hashCode` on the field. If a more complex comparison is required, compute a “canonical representation” for this field and invoke `hashCode` on the canonical representation. If the value of the field is `null`, return `0` (or some other constant, but `0` is traditional).
vii. If the field is an array, treat it as if each element were a separate field. That is, compute a hash code for each significant element by applying these rules recursively, and combine these values per step 2.b. If every element in an array field is significant, you can use one of the `Arrays.hashCode` methods added in release 1.5.
b. Combine the hash code `c` computed in step 2.a into result as follows: `result = 31 * result + c`;
- 
Return result.
- 
When you are finished writing the `hashCode` method, ask yourself whether equal instances have equal hash codes. Write unit tests to verify your intuition! If equal instances have unequal hash codes, figure out why and fix the problem.
- 
存储一些非零常量值，例如17，存储在变量名为`result`的`int`变量中。
- 
对于对象中每一个有意义的字段`f`（每一个`equals`方法考虑的字段），按以下做法去做：
a. 为这个字段计算一个`int`型的哈希码`c`：
i. 如果这个字段是一个`boolean`，计算`(f ? 1 : 0)`。
ii. 如果这个字段是一个`byte`，`char`，`short`或`int`，计算`(int) f`。
iii. 如果这个字段是一个`long`，计算`(int)(f^(f>>>32))`。
iv. 如果这个字段是一个`float`，计算`Float.floatToIntBits(f)`。
v. 如果这个字段是一个`double`，计算`Double.doubleToLongBits(f)`，然后对结果`long`进行2.a.iii处理。
vi. 如果这个字段是一个对象引用并且这个类的`equals`方法通过递归调用`equals`方法来比较这个字段，那么对这个字段递归的调用`hashCode`方法。如果需要更复杂的比较，为这个字段计算一个“标准表示”然后在标准表示上调用`hashCode`方法。如果字段值为`null`，返回`0`(或一些其它常量，但`0`是传统表示).
vii. 如果字段是一个数组，将它每一个元素看做是一个单独的字段。也就是说，通过递归的应用这些规则为每一个有效元素计算一个哈希值，并结合这些值对每一个用步骤2.b处理。如果数组的每个元素都是有意义的，你可以用JDK 1.5中的`Arrays.hashCode`方法。
b. 结合步骤2.a计算的哈希码`c`得到结果如下：`result = 31 * result + c`；
- 
返回结果。
- 
当你完成了`hashCode`方法的编写后，问一下自己相等的对象是否有相同的哈希码。写单元测试来验证你的直觉！如果相等的实例有不等的哈希码弄明白为什么并修正这个问题。
You may exclude redundant fields from the hash code computation. In other words, you may ignore any field whose value can be computed from fields included in the computation. You must exclude any fields that are not used in `equals` comparisons, or you risk violating the second provision of the `hashCode` contract.
你可以从哈希码计算中排除冗余字段。换句话说，你可以忽略那些可以从根据计算中的字段计算出值的字段。你必须排除那些`equals`比较没有使用的字段，或者你冒险违反`hashCode`约定中的第二条。
A nonzero initial value is used in step 1 so the hash value will be affected by initial fields whose hash value, as computed in step 2.a, is zero. If zero were used as the initial value in step 1, the overall hash value would be unaffected by any such initial fields, which could increase collisions. The value 17 is arbitrary.
步骤1中使用了一个非零初始值，因此哈希值会受到哈希值为0的最初字段的影响，最初字段的哈希值是在步骤2.a中计算的。如果0作为初始值在步骤1中使用，全部的哈希值将不受任何这样的最初字段的影响，这将会增加哈希碰撞。
The multiplication in step 2.b makes the result depend on the order of the fields, yielding a much better hash function if the class has multiple similar fields. For example, if the multiplication were omitted from a `String` hash function, all anagrams would have identical hash codes. The value 31 was chosen because it is an odd prime. If it were even and the multiplication overflowed, information would be lost, as multiplication by 2 is equivalent to shifting. The advantage of using a prime is less clear, but it is traditional. A nice property of 31 is that the multiplication can be replaced by a shift and a subtraction for better performance: `31 * i == (i << 5) - i`. Modern VMs do this sort of optimization automatically.
Let’s apply the above recipe to the `PhoneNumber` class. There are three significant fields, all of type short:
步骤2.b中的乘积使结果依赖于字段的顺序，如果这个类有多个相似的字段会取得一个更好的哈希函数。例如，`String`哈希函数忽略了乘积，所有的字母顺序将有相同的哈希码。选择值31是因为它是一个奇素数。如果它是偶数并且乘积溢出，会损失信息，因为与2想乘等价于位移运算。使用一个素数的优势不是那么明显，但习惯上都使用素数。31的一个很好的特性是乘积可以用位移和减法运算替换从而取得更好的性能：`31 * i == (i << 5) - i`。现代的虚拟机能自动进行排序的优化。让我们对`PhoneNumber`类应用上面的步骤。这儿有三个字段，所有的类型缩写：
```
@Override public int hashCode() {
    int result = 17;
    result = 31 * result + areaCode;
    result = 31 * result + prefix;
    result = 31 * result + lineNumber;
    return result;
}
```
Because this method returns the result of a simple deterministic computation whose only inputs are the three significant fields in a `PhoneNumber`instance, it is clear that equal `PhoneNumber` instances have equal hash codes. This method is, in fact, a perfectly good `hashCode` implementation for `PhoneNumber`, on a par with those in the Java platform libraries. It is simple, reasonably fast, and does a reasonable job of dispersing unequal phone numbers into different hash buckets.
因为这个方法返回一个简单的确定性运算的结果，唯一的输入是`PhoneNumber`实例中的三个有效字段，很明显相等的`PhoneNumber`有相等的哈希值。事实上，这个方法对于`PhoneNumber`来说是一个完美的很好的`hashCode`实现，与Java平台库的实现是等价的。它是简单的，相当的快，做者合理的工作——将不等的电话号码分散到不同的哈希桶里。
If a class is immutable and the cost of computing the hash code is significant, you might consider caching the hash code in the object rather than recalculating it each time it is requested. If you believe that most objects of this type will be used as hash keys, then you should calculate the hash code when the instance is created. Otherwise, you might choose to lazily initialize it the first time `hashCode` is invoked (Item 71). It is not clear that our `PhoneNumber` class merits this treatment, but just to show you how it’s done:
如果一个类是不可变的，计算哈希码的代价是很明显的，你可能想缓存对象中的哈希码而不是每次请求时重新计算它。如果你认为这种类型的大多数对象将作为哈希键使用，那当实例创建时你应该计算哈希码。此外，当第一次调用`hashCode`时(Item 71)，你可以选择延迟初始化。我们的`PhoneNumber`类进行这样处理的优点不是很明显，但可以显示一下它是怎么做的：
```
// Lazily initialized, cached hashCode
    private volatile int hashCode;  // (See Item 71)
    @Override 
    public int hashCode() {
        int result = hashCode;
        if (result == 0) {
            result = 17;
            result = 31 * result + areaCode;
            result = 31 * result + prefix;
            result = 31 * result + lineNumber;
            hashCode = result;
        }
        return result;
    }
```
While the recipe in this item yields reasonably good hash functions, it does not yield state-of-the-art hash functions, nor do the Java platform libraries provide such hash functions as of release 1.6. Writing such hash functions is a research topic, best left to mathematicians and theoretical computer scientists. Perhaps a later release of the platform will provide state-of-the-art hash functions for its classes and utility methods to allow average programmers to construct such hash functions. In the meantime, the techniques described in this item should be adequate for most applications.
虽然在本条目中这些步骤取得了合理的好的哈希函数，但它不是最新的哈希函数，也不是Java 1.6平台库提供的哈希函数。写这样一个哈希函数是一个研究课题，最好留给数学家和理论科学家。也许Java平台后面的版本会为它的类和工具方法提供最新的哈希函数来允许普通的程序员构建这样的哈希函数。同时，本条目描述的技术应该足够满足大部分应用了。
**Do not be tempted to exclude significant parts of an object from the hash code computation to improve performance.** While the resulting hash function may run faster, its poor quality may degrade hash tables’ performance to the point where they become unusably slow. In particular, the hash function may, in practice, be confronted with a large collection of instances that differ largely in the regions that you’ve chosen to ignore. If this happens, the hash function will map all the instances to a very few hash codes, and hash-based collections will display quadratic performance. This is not just a theoretical problem. The `String` hash function implemented in all releases prior to 1.2 examined at most sixteen characters, evenly spaced throughout the string, starting with the first character. For large collections of hierarchical names, such as URLs, this hash function displayed exactly the pathological behavior noted here.
**不要试图将对象的有效部分排除在哈希码计算之外来提高性能。**虽然最终结果的哈希函数可能运行更快，但它的质量很差可能会降低哈希表的性能，使哈希表变成慢的不可用的状态。尤其是在实践中，哈希函数可能面临在你选择忽略的区域中存在很大不同的实例集合。如果这种情况发生了，哈希函数会映射所有的实例到一个非常小的哈希码上，基于哈希的集合的性能将会变成平方级的。这不仅仅是一个理论问题。`String`哈希函数在1.2之前的实现中，最多检查16个字符，整个字符串等间距，从第一个字符开始。对于名字分层的大集合，例如URLs，哈希函数正好展现了这里提到的病态行为。
Many classes in the Java platform libraries, such as `String`, `Integer`, and `Date`, include in their specifications the exact value returned by their `hashCode` method as a function of the instance value. This is generally not a good idea, as it severely limits your ability to improve the hash function in future releases. If you leave the details of a hash function unspecified and a flaw is found or a better hash function discovered, you can change the hash function in a subsequent release, confident that no clients depend on the exact values returned by the hash function.
Java平台库中的许多类，例如`String`，`Integer`和`Date`，包含了类规范中它们的`hashCode`方法返回的确定值。这通常不是一个好注意，因为它严重限制了你在将来版本中改进哈希函数的能力。如果没有指定哈希函数的细节，当发现有缺陷或一个更好的哈希函数时，你可以在接下来的版本中改变哈希函数，确信没有用户依赖哈希函数返回的确定值。
