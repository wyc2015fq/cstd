
# sonarqube基础：扫描规则：1: Sonar way之Java版 - 知行合一 止于至善 - CSDN博客

2018年11月03日 06:21:43[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2853所属专栏：[DevOps系列之：持续构建与集成](https://blog.csdn.net/column/details/27057.html)



SonarQube内嵌了Sonar way的扫描规则，不同语言具有不同版本，比如C\#/Java/Javascript等，缺省从三个维度对规则进行划分，这篇文章以Sonarqube 5.6.5版本为例，将Java部分的相关的规则进行简单整理。
# 规则导出
使用下图的back up功能可将相关的规则导出成xml文件格式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181102164827959.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==,size_16,color_FFFFFF,t_70)
# 规则状况
此版本的Java的Sonar way共有规则254条，相关的数量如下：
|总类|规则数量|
|---|---|
|缺陷（Bug）|75|
|安全（Vulnerability）|20|
|改善（Code Smell）|159|
# 缺陷
|缺陷相关的规则|
|---|
|“.equals()” should not be used to test the values of “Atomic” classes|
|“@NonNull” values should not be set to null|
|“BigDecimal(double)” should not be used|
|“Calendars” and “DateFormats” should not be static|
|“Cloneables” should implement “clone”|
|“compareTo” should not return “Integer.MIN_VALUE”|
|“Double.longBitsToDouble” should not be used for “int”|
|“equals(Object obj)” and “hashCode()” should be overridden in pairs|
|“equals(Object obj)” should be overridden along with the “compareTo(T obj)” method|
|“equals(Object obj)” should test argument type|
|“Externalizable” classes should have a no-arguments constructor|
|“hashCode” and “toString” should not be called on array instances|
|“instanceof” operators that always return “true” or “false” should be removed|
|“InterruptedException” should not be ignored|
|“Iterator.hasNext()” should not call “Iterator.next()”|
|“Object.wait(…)” and “Condition.await(…)” should be called inside a “while” loop|
|“Object.wait(…)” should never be called on objects that implement “java.util.concurrent.locks.Condition”|
|“PreparedStatement” and “ResultSet” methods should be called with valid indices|
|“read” and “readLine” return values should be used|
|“return” statements should not occur in “finally” blocks|
|“runFinalizersOnExit” should not be called|
|“ScheduledThreadPoolExecutor” should not have 0 core threads|
|“Serializable” inner classes of non-serializable classes should be “static”|
|“SingleConnectionFactory” instances should be set to “reconnectOnException”|
|“toString()” and “clone()” methods should not return null|
|“wait(…)”, “notify()” and “notifyAll()” methods should only be called when a lock is obviously held on an object|
|A “for” loop update clause should move the counter in the right direction|
|Assertions should be complete|
|Assignments should not be made from within sub-expressions|
|Classes should not be compared by name|
|Collections should not be passed as arguments to their own methods|
|Conditions should not unconditionally evaluate to “TRUE” or to “FALSE”|
|Custom serialization method signatures should meet requirements|
|Default EJB interceptors should be declared in “ejb-jar.xml”|
|Dissimilar primitive wrappers should not be used with the ternary operator without explicit casting|
|Fields in a “Serializable” class should either be transient or serializable|
|Floating point numbers should not be tested for equality|
|Identical expressions should not be used on both sides of a binary operator|
|IllegalMonitorStateException should not be caught|
|Inappropriate “Collection” calls should not be made|
|Inappropriate regular expressions should not be used|
|Instance methods should not write to “static” fields|
|Ints and longs should not be shifted by more than their number of bits-1|
|Invalid “Date” values should not be used|
|JUnit assertions should not be used in “run” methods|
|JUnit framework methods should be declared properly|
|JUnit test cases should call super methods|
|Locks should be released|
|Loop conditions should be true at least once|
|Loops should not be infinite|
|Math operands should be cast before assignment|
|Methods “wait(…)”, “notify()” and “notifyAll()” should never be called on Thread instances|
|Methods should not be named “hashcode” or “equal”|
|Multiline blocks should be enclosed in curly braces|
|Neither “Math.abs” nor negation should be used on numbers that could be “MIN_VALUE”|
|Non-public methods should not be “@Transactional”|
|Non-serializable classes should not be written|
|Non-serializable objects should not be stored in “HttpSessions”|
|Null pointers should not be dereferenced|
|Objects should not be created to be dropped immediately without being used|
|Printf-style format strings should not lead to unexpected behavior at runtime|
|Raw byte values should not be used in bitwise operations in combination with shifts|
|Reflection should not be used to check non-runtime annotations|
|Related “if/else if” statements should not have the same condition|
|Relational operators should be used in “for” loop termination conditions|
|Resources should be closed|
|Servlets should not have mutable instance fields|
|Short-circuit logic should be used in boolean contexts|
|Silly equality checks should not be made|
|super.finalize() should be called at the end of Object.finalize() implementations|
|Synchronization should not be based on Strings or boxed primitives|
|The non-serializable super class of a “Serializable” class should have a no-argument constructor|
|The value returned from a stream read should be checked|
|Thread.run() and Runnable.run() should not be called directly|
|Values should not be uselessly incremented|
# 安全
|脆弱性相关的规则|
|---|
|“ConcurrentLinkedQueue.size()” should not be used|
|“enum” fields should not be publicly mutable|
|“File.createTempFile” should not be used to create a directory|
|“HttpServletRequest.getRequestedSessionId()” should not be used|
|“public static” fields should be constant|
|“static final” arrays should be “private”|
|“wait(…)” should be used instead of “Thread.sleep(…)” when a lock is held|
|Cookies should be “secure”|
|Credentials should not be hard-coded|
|Cryptographic RSA algorithms should always incorporate OAEP (Optimal Asymmetric Encryption Padding)|
|Exceptions should not be thrown from servlet methods|
|Generic exceptions should never be thrown|
|IP addresses should not be hardcoded|
|Mutable fields should not be “public static”|
|The Object.finalize() method should not be called|
|Throwable and Error should not be caught|
|Throwable.printStackTrace(…) should not be called|
|Values passed to SQL commands should be sanitized|
|Web applications should not have a “main” method|
|Web applications should use validation filters|
# 改善
|改善相关的规则|
|---|
|“@Override” annotation should be used on any method overriding (since Java 5) or implementing (since Java 6) another one|
|“assert” should only be used with boolean variables|
|“catch” clauses should do more than rethrow|
|“clone” should not be overridden|
|“compareTo” results should not be checked for specific values|
|“DateUtils.truncate” from Apache Commons Lang library should not be used|
|“entrySet()” should be iterated when both the key and value are needed|
|“finalize” should not set fields to “null”|
|“FIXME” tags should be handled|
|“for” loop incrementers should modify the variable being tested in the loop’s stop condition|
|“for” loop stop conditions should be invariant|
|“indexOf” checks should not be for positive numbers|
|“indexOf” checks should use a start position|
|“Iterator.next()” methods should throw “NoSuchElementException”|
|“Lock” objects should not be “synchronized”|
|“Object.finalize()” should remain protected (versus public) when overriding|
|“private” methods called only by inner classes should be moved to those classes|
|“readObject” should not be “synchronized”|
|“readResolve” methods should be inheritable|
|“ResultSet.isLast()” should not be used|
|“static” members should be accessed statically|
|“StringBuilder” and “StringBuffer” should not be instantiated with a character|
|“switch case” clauses should not have too many lines|
|“switch” statements should end with a “default” clause|
|“switch” statements should have at least 3 “case” clauses|
|“switch” statements should not contain non-case labels|
|“switch” statements should not have too many “case” clauses|
|“Thread.sleep” should not be used in tests|
|“Threads” should not be used where “Runnables” are expected|
|“TODO” tags should be handled|
|“toString()” should never be called on a String object|
|“URL.hashCode” and “URL.equals” should be avoided|
|@FunctionalInterface annotation should be used to flag Single Abstract Method interfaces|
|A “while” loop should be used instead of a “for” loop|
|A field should not duplicate the name of its containing class|
|Abstract classes without fields should be converted to interfaces|
|Abstract methods should not be redundant|
|Annotation repetitions should not be wrapped|
|Anonymous inner classes containing only one method should become lambdas|
|Array designators “[]” should be located after the type in method signatures|
|Array designators “[]” should be on the type, not the variable|
|Boxing and unboxing should not be immediately reversed|
|Branches should have sufficient coverage by unit tests|
|Case insensitive string comparisons should be made without intermediate upper or lower casing|
|Catches should be combined|
|Child class members should not shadow parent class members|
|Child class methods named for parent class methods should be overrides|
|Class names should comply with a naming convention|
|Class names should not shadow interfaces or superclasses|
|Class variable fields should not have public accessibility|
|Classes extending java.lang.Thread should override the “run” method|
|Classes from “sun.*” packages should not be used|
|Classes named like “Exception” should extend “Exception” or a subclass|
|Classes should not be empty|
|Classes that override “clone” should be “Cloneable” and call “super.clone()”|
|Classes with only “static” methods should not be instantiated|
|Collapsible “if” statements should be merged|
|Collection.isEmpty() should be used to test for emptiness|
|Collections.emptyList(), emptyMap() and emptySet() should be used instead of Collections.EMPTY_LIST, EMPTY_MAP and EMPTY_SET|
|Constant names should comply with a naming convention|
|Constants should not be defined in interfaces|
|Constructor injection should be used instead of field injection|
|Control flow statements “if”, “for”, “while”, “switch” and “try” should not be nested too deeply|
|Dead stores should be removed|
|Declarations should use Java collection interfaces such as “List” rather than specific implementation classes such as “LinkedList”|
|Dependencies should not have “system” scope|
|Deprecated “${pom}” properties should not be used|
|Deprecated code should be removed eventually|
|Deprecated elements should have both the annotation and the Javadoc tag|
|Empty arrays and collections should be returned instead of null|
|Enumeration should not be implemented|
|Exception classes should be immutable|
|Exception handlers should preserve the original exception|
|Exception types should not be tested using “instanceof” in catch blocks|
|Exceptions should not be thrown in finally blocks|
|Execution of the Garbage Collector should be triggered only by the JVM|
|Exit methods should not be called|
|Expressions should not be too complex|
|Field names should comply with a naming convention|
|Fields in non-serializable classes should not be “transient”|
|Future keywords should not be used as names|
|Generic wildcard types should not be used in return parameters|
|Inheritance tree of classes should not be too deep|
|Inner class calls to super class methods should be unambiguous|
|Interface names should comply with a naming convention|
|JUnit rules should be used|
|Labels should not be used|
|Lambdas and anonymous classes should not have too many lines|
|Lambdas should be replaced with method references|
|Lamdbas containing only one statement should not nest this statement in a block|
|Literal boolean values should not be used in condition expressions|
|Local variable and method parameter names should comply with a naming convention|
|Local Variables should not be declared and then immediately returned or thrown|
|Local variables should not shadow class fields|
|Long suffix “L” should be upper case|
|Loops should not contain more than a single “break” or “continue” statement|
|Maps with keys that are enum values should be replaced with EnumMap|
|Method names should comply with a naming convention|
|Method overrides should not change contracts|
|Method parameters, caught exceptions and foreach variables should not be reassigned|
|Methods and field names should not be the same or differ only by capitalization|
|Methods named “equals” should override Object.equals(Object)|
|Methods of “Random” that return floating point values should not be used in random integer generation|
|Methods should not be empty|
|Methods should not be too complex|
|Methods should not have too many parameters|
|Modifiers should be declared in the correct order|
|Multiple variables should not be declared on the same line|
|Nested "enum"s should not be declared static|
|Nested blocks of code should not be left empty|
|Nested code blocks should not be used|
|Non-constructor methods should not have the same name as the enclosing class|
|Null should not be returned from a “Boolean” method|
|Objects should not be created only to “getClass”|
|Only static class initializers should be used|
|Overriding methods should do more than simply call the same method in the super class|
|Package declaration should match source file directory|
|Package names should comply with a naming convention|
|Parentheses should be removed from a single lambda input parameter when its type is inferred|
|Parsing should be used to convert “Strings” to primitives|
|Primitive wrappers should not be instantiated only for “toString” or “compareTo” calls|
|Primitives should not be boxed just for “String” conversion|
|Public constants and fields initialized at declaration should be “static final” rather than merely “final”|
|Public methods should throw at most one checked exception|
|Redundant casts should not be used|
|Return of boolean expressions should not be wrapped into an “if-then-else” statement|
|Sections of code should not be “commented out”|
|Silly bit operations should not be performed|
|Silly math should not be performed|
|Source files should not have any duplicated blocks|
|Standard outputs should not be used directly to log anything|
|Statements should be on separate lines|
|Static non-final field names should comply with a naming convention|
|String function use should be optimized for single characters|
|String literals should not be duplicated|
|String.valueOf() should not be appended to a String|
|Strings literals should be placed on the left side when checking for equality|
|Subclasses that add fields should override “equals”|
|Switch cases should end with an unconditional “break” statement|
|Synchronized classes Vector, Hashtable, Stack and StringBuffer should not be used|
|TestCases should contain tests|
|The default unnamed package should not be used|
|The diamond operator ("<>") should be used|
|The members of an interface declaration or class should appear in a pre-defined order|
|The Object.finalize() method should not be overriden|
|The signature of “finalize()” should match that of “Object.finalize()”|
|Throws declarations should not be superfluous|
|Try-catch blocks should not be nested|
|Try-with-resources should be used|
|Type parameter names should comply with a naming convention|
|Unused labels should be removed|
|Unused local variables should be removed|
|Unused method parameters should be removed|
|Unused private fields should be removed|
|Unused private method should be removed|
|Unused type parameters should be removed|
|Useless imports should be removed|
|Useless parentheses around expressions should be removed to prevent any misunderstanding|
|Utility classes should not have public constructors|
# 总结
Sonar缺省从Bug/Vulnerability/Code Smell维度进行划分，同时不同维度的扫描规则的等级也有所不同，比如Major或者Minor，在实际使用的时候可以根据具体情况进行调整，在接下来的文章中将会介绍如何定制生成sonar的profile。

