# RandomAccess接口理解 - z69183787的专栏 - CSDN博客
2018年04月08日 11:45:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：965
[https://blog.csdn.net/stick2it/article/details/53469910](https://blog.csdn.net/stick2it/article/details/53469910)
根据javadoc上面的的解释是：
RandomAccess 是一个标记接口，用于标明实现该接口的List支持快速随机访问，主要目的是使算法能够在随机和顺序访问的list中表现的更加高效。
我们可以简单的看下Collections下的binarySearch方法的源码:
[java][view
 plain](https://blog.csdn.net/stick2it/article/details/53469910#)[copy](https://blog.csdn.net/stick2it/article/details/53469910#)
- publicstatic <T>  
- int binarySearch(List<? extends Comparable<? super T>> list, T key) {  
- if (list instanceof RandomAccess || list.size()<BINARYSEARCH_THRESHOLD)  
- return Collections.indexedBinarySearch(list, key);  
- else
- return Collections.iteratorBinarySearch(list, key);  
-     }  
从源码中我们可以看到，在进行二分查找的时候，list会先判断是否是RandomAccess也即是否实现了RandomAccess接口，接着在调用想用的二分查找算法来进行，（其中: BINARYSEARCH_THRESHOLD Collections的一个常量（5000），它是二分查找的阀值。）如果实现了RandomAccess接口的List，执行indexedBinarySearch方法，否则执行 iteratorBinarySearch方法。
分别看下这两个方法的实现:
indexedBinarySearch 方法:
[java][view
 plain](https://blog.csdn.net/stick2it/article/details/53469910#)[copy](https://blog.csdn.net/stick2it/article/details/53469910#)
- privatestatic <T>  
- int indexedBinarySearch(List<? extends Comparable<? super T>> list, T key) {  
- int low = 0;  
- int high = list.size()-1;  
- 
- while (low <= high) {  
- int mid = (low + high) >>> 1;  
-             Comparable<? super T> midVal = list.get(mid);  
- int cmp = midVal.compareTo(key);  
- 
- if (cmp < 0)  
-                 low = mid + 1;  
- elseif (cmp > 0)  
-                 high = mid - 1;  
- else
- return mid; // key found
-         }  
- return -(low + 1);  // key not found
-     }  
indexedBinarySearch 方法是直接通过get来访问元素
iteratorBinarySearch方法:
[java][view
 plain](https://blog.csdn.net/stick2it/article/details/53469910#)[copy](https://blog.csdn.net/stick2it/article/details/53469910#)
- privatestatic <T>  
- int iteratorBinarySearch(List<? extends Comparable<? super T>> list, T key)  
-     {  
- int low = 0;  
- int high = list.size()-1;  
-         ListIterator<? extends Comparable<? super T>> i = list.listIterator();  
- 
- while (low <= high) {  
- int mid = (low + high) >>> 1;  
-             Comparable<? super T> midVal = get(i, mid);  
- int cmp = midVal.compareTo(key);  
- 
- if (cmp < 0)  
-                 low = mid + 1;  
- elseif (cmp > 0)  
-                 high = mid - 1;  
- else
- return mid; // key found
-         }  
- return -(low + 1);  // key not found
-     }  
iteratorBinarySearch中ListIterator来查找相应的元素
javadoc中特别指出:
It is recognized that the distinction between random and sequential access is often fuzzy. For example, some List implementations
 provide asymptotically linear access times if they get huge, but constant access times in practice. Such a Listimplementation should generally implement this interface. As a rule
 of thumb, a List implementation should implement this interface if, for typical instances of the class, this loop:
     for (int i=0, n=list.size(); i < n; i++)
         list.get(i);
 runs faster than this loop:     for (Iterator i=list.iterator(); i.hasNext(); )
         i.next();
总结：实现RandomAccess接口的的List可以通过简单的for循环来访问数据比使用iterator访问来的高效快速。
参考文档:https://docs.oracle.com/javase/7/docs/api/java/util/RandomAccess.html
