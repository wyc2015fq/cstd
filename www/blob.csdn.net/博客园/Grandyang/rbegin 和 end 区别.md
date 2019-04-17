# rbegin 和 end 区别 - Grandyang - 博客园







# [rbegin 和 end 区别](https://www.cnblogs.com/grandyang/p/4537277.html)







在使用C++的STL时，经常会用到迭代器，那么不得不搞清楚里面的一些指针 begin(), end(), rbegin(), rend()之间的区别与联系，以及它们分别都指向哪个元素。首先要明白的一点是它们都是指针，其中begin()指向第一个元素，我们容易误认为end()是指向最后一个元素，其实不是的，end()是指向最后一个元素的下一个。这就是为什么我们在使用map或是unordered_map来查找某个值时都要和end()比较呢，因为如果等于end()了，则说明所有的元素都遍历过一遍了，也包括最后一个元素。rbegin()指向最后一个元素，rend()指向第一个元素，和begin()相等。














