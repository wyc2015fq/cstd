# 自己实现ArrayList - PeterBishop - CSDN博客





2018年10月17日 18:16:03[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：13
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









我们已经学过了数组了，但我们知道一个事，数组是定长的，定长的数组显然是满足不了我们的需求的，因为我们不知道自己需要多长的数组，所以只能定义的尽可能大，这样就会造成极大的空间浪费。



那么我们可不可以想个办法去实现一个不固定长度的容器呢？


这节课我就来带大家实现一个不固定长度的容器。



Code:





/**

 * 我的数组表

 * 用数组来存储数据

 */

public class MyArrayList<E> {


    //存储数据

    private Object[] values;


    //MyArrayList中存储的元素个数

    private int size = 0;


    //接下来写构造器

    public MyArrayList()

    {

        values = new Object[10];    //我们默认初始化数组长度为10

    }


    //当然你我们也应该允许自定义容器长度

    public MyArrayList(int capital)

    {

        values = new Object[capital];

    }


    //定义一个获取MyArrayList内存储元素个数的方法

    public int size()

    {

        return size;

    }


    //核心方法，添加元素

    public void add(E e)

    {

        //再添加元素之前需要做判断

        if (size == values.length)

        {

            expandCapital();

        }

        //这个其实不难，就是往数组里添加元素

        values[size] = e;

        size++;

    }


    //扩容

    private void expandCapital()

    {

        if (size == values.length)

        {

            Object[] newValues = new Object[size * 2];  //我们将数组长度扩大一倍

            //然后拷贝数据

            for (int i = 0; i < size; i++) {

                newValues[i] = values[i];

            }

            //让values指向扩容后的数组

            values = newValues;

        }

    }


    //接下来我们需要有一个获取元素的方法

    public E get(int index)

    {

        //这里还有点小问题，没有防止数组越界,加个判断

        if (index < 0 || index >= size)

            return null;

        return (E)values[index];

    }


    //有添加就要有删除


    /**

     * 删除方法，删掉index位置的元素，并返回被删掉的元素

     * @return

     */

    public E delete(int index)

    {

        //第一步，找到要被删除的元素

        E e  = get(index);

        if (e != null)

        {

            //开始删除，想法就是让a[i] = a[i+1],然后让最后一个元素指向null，最后size--

            for (int i = index; i < size - 1; i++)

                values[index] = values[index + 1];

            values[size - 1] = null;

            size--;

        }

        //如果e为null

        return null;

    }


    //再来写一个修改

    public void set(int index, E e)

    {

        if (get(index) != null)

        {

            values[index] = e;

        }

    }


    //最后写一个insert，在任意位置插入

    public void insert(int index, E e)

    {

        //首先做判断，插入位置是否合法

        if (index < 0 || index > size())

            return;

        if (index == size)

            add(e);

        else

        {

            //和删除相反

            add(null);

            for (int i = size; i > index; i--)

            {

                values[i] = values[i-1];

            }

            values[index] = e;

            return;

        }


    }


    public boolean contains(E e)

    {

        //如果e为null就检查一下表里有没有null

        if (e == null)

        {

            for (int i = 0; i < size; i++) {

                if (values[i] == null)

                    return true;

            }

        }

        else

        {

            //如果e不为null就调用equals方法判断表里是否有元素和e相等

            for (int i = 0; i < size; i++) {

                if (e.equals(values[i]))

                    return true;

            }

        }

        return false;


    }


    //注意，这里必须new一个新的数组返回，因为我们不希望返回的数组是values引用的数组对象

    public Object[] toArray()

    {

        Object[] newValues = new Object[this.size()];

        for (int i = 0; i < size; i++) {

            newValues[i] = values[i];

        }

        return newValues;

    }


    //将数组复制到arr中,只不过这个传入的数组长度必须要足够长

    public void toArray(E[] arr)

    {

        if (arr.length < size)

        {

            System.out.println("这个数组长度小于" + this.size() + ", 请传入一个长度比" + this.size() + "更长的数组");

            return;

        }

        for (int i = 0; i < size; i++) {

            arr[i] = (E)values[i];

        }

    }


    public static void main(String[] args) {

        MyArrayList<Integer> list = new MyArrayList<>();

        list.add(1);

        list.add(2);

        System.out.println(list.size());    //2

        //可以看到我们可以动态的添加数据，但有一个问题

        for (int i = 0; i < 100; i++)

        {

            list.add(i);

        }

        //数组下标越界了，但是我们希望我们可以添加任意个元素，怎么办呢？

        //一个想法是数组扩容

        System.out.println("添加100个元素后的表的长度:" + list.size());

        //测试一下

        System.out.println("第50个元素为:" + list.get(50));

        System.out.println("第51个元素为:" + list.get(51));


        System.out.println("我要删除第50个元素啦");

        list.delete(50);

        System.out.println("现在第50个元素为:" + list.get(50));

        System.out.println("现在表的长度为:" + list.size());

        //可以很明显的看到元素被删除了


        //试试可不可以

        System.out.println("现在我要在表的第50位置插入48");

        list.insert(50,48);

        System.out.println("现在表的第50位置的元素为:" + list.get(50));

        System.out.println("现在表的长度为:" + list.size());


        //这就是动态数组表的增删改查

        //最后再加点其他功能

        System.out.println(list.contains(60));


        Object[] objects = list.toArray();

        System.out.println(objects.length);


        Integer[] arr = new Integer[30];

        list.toArray(arr);  //这样就长度不够


        Integer[] arr2 = new Integer[200];

        list.toArray(arr2);

        System.out.println(arr2[0]);    //这个就正常了

    }


}



