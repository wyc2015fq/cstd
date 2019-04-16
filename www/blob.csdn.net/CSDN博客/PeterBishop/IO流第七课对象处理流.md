# IO流第七课对象处理流 - PeterBishop - CSDN博客





2018年11月11日 13:43:34[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25








  这节课我们来讲对象处理流，首先来说说两个概念: 序列化和反序列化



以下内容摘自菜鸟教程:

Java 提供了一种对象序列化的机制，该机制中，一个对象可以被表示为一个字节序列，该字节序列包括该对象的数据、有关对象的类型的信息和存储在对象中数据的类型。



将序列化对象写入文件之后，可以从文件中读取出来，并且对它进行反序列化，也就是说，对象的类型信息、对象的数据，还有对象中的数据类型可以用来在内存中新建对象。



通俗来讲:

将对象写到文件里就叫序列化，从文件里读取出一个对象叫反序列化



注意:
- 不是所有对象都可以序列化，只有实现了java.io.Serializable接口的类的对象可以被序列化 ，除此之外数组也可以被序列化
- 不是对象里的所有属性都需要被写入到文件里，对于不需要被写入到文件里的属性可以在声明时加上transient关键字



接下来就是我们的主角了:

ObjectInputStream: 负责从文件中读取对象(反序列化)

ObjectOutputStream: 负责将对象写入文件(序列化)



构造器和常用方法自己去查API文档，我给个Demo大家看看就行:



Demo:
public class ObjectDisposeStream {



    public static void main(String[] args) throws IOException, ClassNotFoundException {

        Person person = new Person("xiaoye",20);

        String filepath = "F:\\code\\java\\object.txt";

        *sera*(filepath,person);

        Object obj = *read*(filepath);

        System.*out*.println(obj);

    }



    public static void sera(String filepath,Object obj) throws IOException {

        File file = new File(filepath);

        if (!file.exists())

            file.createNewFile();



        ObjectOutputStream objectOutputStream = new ObjectOutputStream(new FileOutputStream(file));

        objectOutputStream.writeObject(obj);





    }



    public static Object read(String filepath) throws IOException, ClassNotFoundException {

        ObjectInputStream objectInputStream = new ObjectInputStream(new FileInputStream(filepath));

        return objectInputStream.readObject();



    }

}



class Person implements Serializable {



    private String name;

    private transient int age;  //加上transient关键字表明在序列化时不需要将这个属性的值写入文件



    public Person(String name, int age) {

        this.name = name;

        this.age = age;

    }



    public Person(){}



    public String getName() {

        return name;

    }



    public void setName(String name) {

        this.name = name;

    }



    public int getAge() {

        return age;

    }



    public void setAge(int age) {

        this.age = age;

    }



    @Override

    public String toString() {

        return "Person{" +

                "name='" + name + '\'' +

                ", age=" + age +

                '}';

    }

}


