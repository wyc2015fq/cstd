
# Java - ArrayList vs. LinkedList - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 12:04:05[Chimomo](https://me.csdn.net/chimomo)阅读数：147


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Similarities
(1) Both ArrayList and LinkedList are implementation of List interface.
(2) They both maintain the elements insertion order which means while displaying ArrayList and LinkedList elements the result set would be having the same order in which the elements got inserted into the List.
(3) Both these classes are non-synchronized and can be made synchronized explicitly by using`Collections.synchronizedList`method.
(4) The iterator and listIterator returned by these classes are fail-fast (if list is structurally modified at any time after the iterator is created, in any way except through the iterator’s own remove or add methods, the iterator will throw a[ConcurrentModificationException](http://docs.oracle.com/javase/6/docs/api/java/util/ConcurrentModificationException.html)).
# Differences
(1)**Search**: ArrayList search operation is pretty fast compared to the LinkedList search operation.`get(int index)`in ArrayList gives the performance of`O(1)`while LinkedList performance is`O(n)`.
Reason: ArrayList maintains dynamic array for its elements as it uses array data structure implicitly which makes it faster for searching an element in the list. On the other side LinkedList implements**doubly linked list**which requires the traversal through all the elements for searching an element.
(2)**Deletion**: LinkedList remove operation gives`O(1)`performance while ArrayList gives variable performance:`O(n)`in worst case (while removing first element) and`O(1)`in best case (While removing last element).
Conclusion: LinkedList element deletion is faster compared to ArrayList.
Reason: LinkedList’s each element maintains two pointers (addresses) which points to the both neighbor elements in the list. Hence removal only requires change in the pointer location in the two neighbor nodes (elements) of the node which is going to be removed. While In ArrayList all the elements need to be shifted to fill out the space created by removed element.
(3)**Inserts Performance**: LinkedList add method gives`O(1)`performance while ArrayList gives`O(n)`in worst case. Reason is same as explained for remove.
(4)**Memory Overhead**: ArrayList maintains indexes and element data while LinkedList maintains element data and two pointers for neighbor nodes hence the memory consumption is high in LinkedList comparatively.
(5) ArrayList class can**act as a list**only because it implements List only. LinkedList class can**act as a list and queue**both because it implements List and Deque interfaces.
# When to use LinkedList and when to use ArrayList?
(1) As explained above the insert and remove operations give good performance (`O(1)`) in LinkedList compared to ArrayList(`O(n)`). Hence if there is a requirement of frequent addition and deletion in application then LinkedList is a best choice.
(2) Search (get method) operations are fast in Arraylist (`O(1)`) but not in LinkedList (`O(n)`) so If there are less add and remove operations and more search operations requirement, ArrayList would be your best bet.
(3) If you have large lists, keep in mind that memory usage is also different. Each element of a`LinkedList`has more overhead since pointers to the next and previous elements are also stored.`ArrayLists`don't have this overhead. However,`ArrayLists`take up as much memory as is allocated for the capacity, regardless of whether elements have actually been added. The default initial capacity of an`ArrayList`is pretty small (10 from Java 1.4 - 1.8). But since the underlying implementation is an array, the array must be resized if you add a lot of elements. To avoid the high cost of resizing when you know you're going to add a lot of elements, construct the`ArrayList`with a higher initial capacity.

