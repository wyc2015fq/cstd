# Java: Best way of converting List to List - z69183787的专栏 - CSDN博客
2017年08月28日 11:40:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：387
|60down voteaccepted|As far as I know, iterate and instantiate is the only way to do this. Something like (for others potential help, since I'm sure you know how to do this):```java```List<Integer> oldList = .../* Specify the size of the list up front to prevent resizing. */List<String> newList = new ArrayList<String>(oldList.size()) for (Integer myInt : oldList) {   newList.add(String.valueOf(myInt)); }````````````|
Using [Google
 Collections](http://code.google.com/p/google-collections/), you could use the `transform` method
 in the [Lists](http://google-collections.googlecode.com/svn/trunk/javadoc/com/google/common/collect/Lists.html) class
```java
```
import com.google.common.collect.Lists;
import com.google.common.base.Functions
List<Integer> integers = Arrays.asList(1, 2, 3, 4);
List<String> strings = Lists.transform(integers, Functions.toStringFunction());
```
```
The `List` returned
 by `transform` is
 a view on the backing list - the transformation will be applied on each
 access to the transformed list.
Be aware that `Functions.toStringFunction()` will
 throw a `NullPointerException` when
 applied to null, so only use it if you are sure your list will not contain null.
