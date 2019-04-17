# Java中级第三课：ArrayList核心代码注释汉化 - PeterBishop - CSDN博客





2019年01月22日 22:22:36[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22
个人分类：[Java中级](https://blog.csdn.net/qq_40061421/article/category/8626230)








public class ArrayList<E> extends AbstractList<E>

        implements List<E>, RandomAccess, Cloneable, java.io.Serializable {

    private static final long *serialVersionUID *= 8683452581122892189L;



    */**     * **默认初始化容量.     */    *private static final int *DEFAULT_CAPACITY *= 10;



    */**     * **一个空数组对象，用于空表实例共享，初始容量为0的空表实例用这个     */    *private static final Object[] *EMPTY_ELEMENTDATA *= {};



    */**     * **共享空数组实例，用于默认大小的空实例。我们     * 将其与空元素数据区分开来，以了解何时填充     * 添加第一个元素。     */    *private static final Object[] *DEFAULTCAPACITY_EMPTY_ELEMENTDATA *= {};



    */**     * **存储arraylist元素的数组缓冲区。     * arraylist的容量是这个数组缓冲区的长度。     * 任何带有elementdata==defaultcapacity_empty_elementdata的空arraylist     * 将在添加第一个元素时扩展为默认容量。     */    *transient Object[] elementData; //非私有以简化嵌套类访问(我也不知道啥意思，百度翻译的)



    */**     * Arraylist**的大小(ArrayList实例里包含的元素的个数)     *     * **@serial     ***/    *private int size;



    */**     * **用一个给定的初始化容量来构造一个空表     *     * **@param ****initialCapacity **这个表的初始化容量     * **@throws **IllegalArgumentException 如果这个initialCapacity是不合法的     */    *public ArrayList(int initialCapacity) {

        if (initialCapacity > 0) {

            this.elementData = new Object[initialCapacity];

        } else if (initialCapacity == 0) {

            this.elementData = *EMPTY_ELEMENTDATA*;

        } else {

            throw new IllegalArgumentException("Illegal Capacity: " +

                    initialCapacity);

        }

    }



    */**     * **用默认初始化长度(10)来构造一个空表     */    *public ArrayList() {

        this.elementData = *DEFAULTCAPACITY_EMPTY_ELEMENTDATA*;

    }



    */**     * **构造一个表包含给定集合对象里的所有顺序，顺序为这个集合对象的迭代器迭代的顺序(也就是原有顺序)     *     * **@param ****c **给定的集合对象     * **@throws **NullPointerException 如果c为null     */    *public ArrayList(Collection<? extends E> c) {

        elementData = c.toArray();

        if ((size = elementData.length) != 0) {

            // c.toArray might (incorrectly) not return Object[] (see 6260652)

            if (elementData.getClass() != Object[].class)

                elementData = Arrays.*copyOf*(elementData, size, Object[].class);

        } else {

            // replace with empty array.

            this.elementData = *EMPTY_ELEMENTDATA*;

        }

    }



    */**     * **将这个ArrayList对象的容量调整至size大小，应用程序使用此操作可以最小化存储一个ArrayList对象     */    *public void trimToSize() {

        modCount++;

        if (size < elementData.length) {

            elementData = (size == 0)

                    ? *EMPTY_ELEMENTDATA                    *: Arrays.*copyOf*(elementData, size);

        }

    }



    */**     * **增加此ArrayList实例的容量，     * 如果有必要,确保它至少能容纳元素的数量由最小容量参数指定。     *     * **@param ****minCapacity **设计好的最小容量     */    *public void ensureCapacity(int minCapacity) {

        int minExpand = (elementData != *DEFAULTCAPACITY_EMPTY_ELEMENTDATA*)

                // any size if not default element table

                ? 0

                // larger than default for default empty table. It's already

                // supposed to be at default size.

                : *DEFAULT_CAPACITY*;



        if (minCapacity > minExpand) {

            ensureExplicitCapacity(minCapacity);

        }

    }



    */**     * **计算容量，如果elementData是DEFAULTCAPACITY_EMPTY_ELEMENTDATA,则返回默认容量与minCapacity之间的最大值     * 否则直接返回minCapacity     *     * **@param ****elementData **元素数据     * **@param ****minCapacity **最小容量     * **@return     ***/    *private static int calculateCapacity(Object[] elementData, int minCapacity) {

        if (elementData == *DEFAULTCAPACITY_EMPTY_ELEMENTDATA*) {

            return Math.*max*(*DEFAULT_CAPACITY*, minCapacity);

        }

        return minCapacity;

    }



    */**     * **确保容量足够     *     * **@param ****minCapacity **最小容量     */    *private void ensureCapacityInternal(int minCapacity) {



        //计算最小容量

        minCapacity = *calculateCapacity*(elementData, minCapacity);

        //扩容

        ensureExplicitCapacity(minCapacity);

    }



    */**     * **确保扩展容量     *     * **@param ****minCapacity **最小容量     */    *private void ensureExplicitCapacity(int minCapacity) {

        modCount++;



        //如果最小容量大于elementData的长度就执行扩容方法grow(minCapacity)

        if (minCapacity - elementData.length > 0)

            grow(minCapacity);

    }



    */**     * **要分配的数组的最大大小。     * 一些虚拟机在数组中保留一些头字。     * 尝试分配较大的数组可能会导致OutOfMemoryError:请求的数组大小超过了VM限制     */    *private static final int *MAX_ARRAY_SIZE *= Integer.*MAX_VALUE *- 8;



    */**     * **增加容量以确保它至少能容纳最小容量参数指定的元素数。     *     * **@param ****minCapacity **设计好的最小容量     */    *private void grow(int minCapacity) {

        // 旧容量 = elementData的长度

        int oldCapacity = elementData.length;



        // 新容量 = 旧容量 + 旧容量位右1位(新容量是旧容量的1.5倍)

        int newCapacity = oldCapacity + (oldCapacity >> 1);

        //如果新容量小于最小容量，把最小容量赋值给新容量

        if (newCapacity - minCapacity < 0)

            newCapacity = minCapacity;

        //如果新容量大于MAX_ARRA_SIZE,则执行hugeCapacity确保按新容量扩容不会导致OutOfMemoryError

        if (newCapacity - *MAX_ARRAY_SIZE *> 0)

            newCapacity = *hugeCapacity*(minCapacity);

        // 重新构造一个长度为newCapacity的数组，并将原来的数据复制过去

        // 然后让elementData指向扩容后的数组

        elementData = Arrays.*copyOf*(elementData, newCapacity);

    }



    */**     * **如果minCapacity小于0，抛出OutOfMemoryError     * 如果min大于MAX_ARRAY_SIZE，返回Integer.MAX_VALUE     * 否则,返回MAX_ARRAY_SIZE     *     * **@param ****minCapacity **最小容量     * **@return     ***/    *private static int hugeCapacity(int minCapacity) {

        if (minCapacity < 0) // overflow

            throw new OutOfMemoryError();

        return (minCapacity > *MAX_ARRAY_SIZE*) ?

                Integer.*MAX_VALUE *:

                *MAX_ARRAY_SIZE*;

    }

}




