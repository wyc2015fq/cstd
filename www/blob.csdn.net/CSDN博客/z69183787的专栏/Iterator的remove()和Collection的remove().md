# Iterator的remove()和Collection的remove() - z69183787的专栏 - CSDN博客
2017年12月13日 16:54:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：291
一、遍历集合的方式有很多，这里就以List 为例
如果是单线程的我们一般使用： int  len= list.size()
[java][view
 plain](http://blog.csdn.net/qh_java/article/details/50154405#)[copy](http://blog.csdn.net/qh_java/article/details/50154405#)
- for (int i=0;i<len;i++){  
- 
- }  
如果是多线程的程序，我们就用Iterator 迭代器来遍历：
[java][view
 plain](http://blog.csdn.net/qh_java/article/details/50154405#)[copy](http://blog.csdn.net/qh_java/article/details/50154405#)
- Iterator<T> it= list.iterator()  
- while(it.hasNext()){  
- T t= it.next();   //获取集合中的值
- // 这里我们要注意如果直接使用list的remove方法就会报异常
- // 而如果使用Iterator类型的实例的remove方法来删除则不会出现问题
- }  
两种方式，在使用Iterator遍历中如果我们直接使用集合的remove方法来删除则会报异常而如果我们使用Iterator 实例的remove方法来删除的话则正常，这个大家都知道，但为什么？
1、使用Iterator 的remove方法来删除：ArrayList中Iterator 的源码：
[java][view
 plain](http://blog.csdn.net/qh_java/article/details/50154405#)[copy](http://blog.csdn.net/qh_java/article/details/50154405#)
- 
- /**
-      * Returns an iterator over the elements in this list in proper sequence.
-      *
-      * <p>The returned iterator is <a href="#fail-fast"><i>fail-fast</i></a>.
-      *
-      * @return an iterator over the elements in this list in proper sequence
-      */
- // 获取集合的Iterator 实例
- public Iterator<E> iterator() {  
- returnnew Itr();  
-     }   
- 
- /**
-      * An optimized version of AbstractList.Itr
-      */
- privateclass Itr implements Iterator<E> {  
- // 表示下一个要访问的元素的索引，从next()方法的具体实现就可看出
- int cursor;       // index of next element to return
- // 表示上一个访问的元素的索引
- int lastRet = -1; // index of last element returned; -1 if no such
- // 表示对ArrayList修改次数的期望值，它的初始值为modCount。
- int expectedModCount = modCount;  
- 
- // 判断这个集合有没有下一个元素的时候，也就是在判断当前值和集合的大小
- publicboolean hasNext() {  
- return cursor != size;  
-         }  
- // 获取下一个元素
- @SuppressWarnings("unchecked")  
- public E next() {  
-             checkForComodification();  
- // 表示下一个要访问的元素的索引
- int i = cursor;  
- // 如果当前位置大于等于集合的大小，则获取下一个值则报异常
- if (i >= size)  
- thrownew NoSuchElementException();  
- // 将集合的数据复制给新的数组对象
-             Object[] elementData = ArrayList.this.elementData;  
- // arraylist集合底层就是一个数组，如果下一个值得检索值大于 集合的值则 报异常
- if (i >= elementData.length)  
- thrownew ConcurrentModificationException();  
- //索引值加1 
-             cursor = i + 1;  
- // 返回之前的当前值的集合的值
- return (E) elementData[lastRet = i];  
-         }  
- // 删除，使用this 调用类的方法而不是内部类的方法
- publicvoid remove() {  
- // 如果不满足则抛出异常
- if (lastRet < 0)  
- thrownew IllegalStateException();  
- // 这个函数的作用就是检测  expectedModCount == modCount是不是相等，如果相等则不会抛出异常否则抛出异常 
-             checkForComodification();  
- try {  
- //删除集合中的内容，删除的是之前的不是现在的
-                 ArrayList.this.remove(lastRet);  
-                 cursor = lastRet;  
-                 lastRet = -1;  
- // 使用iterator的方法进行删除的时候会修改expectedModCount的值为modCount 所以这样就不会出现异常
-                 expectedModCount = modCount;  
-             } catch (IndexOutOfBoundsException ex) {  
- thrownew ConcurrentModificationException();  
-             }  
-         }  
- // 这个方法用来检查两个变量的值是否相等
- finalvoid checkForComodification() {  
- if (modCount != expectedModCount)  
- thrownew ConcurrentModificationException();  
-         }  
-     }  
2、直接调用集合的remove方法：
[java][view
 plain](http://blog.csdn.net/qh_java/article/details/50154405#)[copy](http://blog.csdn.net/qh_java/article/details/50154405#)
- /**
-      * Removes the first occurrence of the specified element from this list,
-      * if it is present.  If the list does not contain the element, it is
-      * unchanged.  More formally, removes the element with the lowest index
-      * <tt>i</tt> such that
-      * <tt>(o==null ? get(i)==null : o.equals(get(i)))</tt>
-      * (if such an element exists).  Returns <tt>true</tt> if this list
-      * contained the specified element (or equivalently, if this list
-      * changed as a result of the call).
-      *
-      * @param o element to be removed from this list, if present
-      * @return <tt>true</tt> if this list contained the specified element
-      */
- publicboolean remove(Object o) {  
- // 找到要删除的对象，调用fastRemove() 方法来删除
- if (o == null) {  
- for (int index = 0; index < size; index++)  
- if (elementData[index] == null) {  
-                     fastRemove(index);  
- returntrue;  
-                 }  
-         } else {  
- for (int index = 0; index < size; index++)  
- if (o.equals(elementData[index])) {  
-                     fastRemove(index);  
- returntrue;  
-                 }  
-         }  
- returnfalse;  
-     }  
[java][view
 plain](http://blog.csdn.net/qh_java/article/details/50154405#)[copy](http://blog.csdn.net/qh_java/article/details/50154405#)
- /*
-     * Private remove method that skips bounds checking and does not
-     * return the value removed.
-     */
- privatevoid fastRemove(int index) {  
- // modCount的值改变了
-        modCount++;  
- int numMoved = size - index - 1;  
- if (numMoved > 0)  
-            System.arraycopy(elementData, index+1, elementData, index,  
-                             numMoved);  
- // 删除元素
- elementData[--size] = null; // Let gc do its work
-    }  
上面的实例中是Iterator的remove方法和Collection的remove方法，而如果是add方法则也是同样的原理。
通过这两段代码我们发现出现异常的原因：
当集合使用Iterator进行迭代的时候，实际是new Itr()创建一个内部对象，初始化包含对象个数，可以理解为在独立线程中操作的。Iterator创建之后引用指向原来的集合对象。当原来的对象数量发生变化时，这个内部对象索引表内容其实是不会同步的。所以，当索引指针往后移动的时候就找不到要迭代的对象了。内部对象操作时为了避免这种情况都会通过checkForComodification方法检测是否一致，不一致提前抛出异常ConcurrentModifiedException。
异常解决办法：
Iterator 支持从源集合中安全地删除对象，只需在 Iterator 上调用 remove() 即可。这样做的好处是可以避免 ConcurrentModifiedException ，这个异常顾名思意：当打开 Iterator 迭代集合时，同时又在对集合进行修改。有些集合不允许在迭代时删除或添加元素，但是调用 Iterator 的 remove() 方法是个安全的做法。
调用Iterator 的remove方法时不仅修改了集合的个数也修改了内部对象的个数，这样再调用checkForComodification（） 方法进行检测的时候发现原来集合对象的个数和内部对象的个数相同这样自然就没异常了
总结：
如果调用Iterator 的remove（） 方法来删除的话，则iterator的内部对象个数和原来集合中对象的个数会保持同步，而直接调用集合的remove方法来删除的话，集合中对象的个数会变化而Iterator 内部对象的个数不会变化，当调用Iterator 的next 遍历的时候发现集合中的对象的个数和Iterator 内部对象的个数不同，这样指针往后移动的时候就找不到要迭代的对象。这是报异常的主要原因，但
 内部对象操作时为了避免这种情况都会通过checkForComodification方法检测是否一致，不一致提前抛出异常ConcurrentModifiedException。
