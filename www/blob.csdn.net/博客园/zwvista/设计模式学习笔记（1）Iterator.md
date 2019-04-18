# 设计模式学习笔记（1）Iterator - zwvista - 博客园

## [设计模式学习笔记（1）Iterator](https://www.cnblogs.com/zwvista/p/8040444.html)

### Iterator 模式

![Iterator Pattern](https://www.tutorialspoint.com/design_pattern/images/iterator_pattern_uml_diagram.jpg)

```
public interface Iterator {
   public boolean hasNext();
   public Object next();
}
```

```
public interface Container {
   public Iterator getIterator();
}
```

```
public class NameRepository implements Container {
   public String names[] = {"Robert" , "John" ,"Julie" , "Lora"};

   @Override
   public Iterator getIterator() {
      return new NameIterator();
   }

   private class NameIterator implements Iterator {

      int index;

      @Override
      public boolean hasNext() {
        return index < names.length;
      }

      @Override
      public Object next() {
        return hasNext() ? names[index++] : null;
      }     
   }
}
```

```
public class IteratorPatternDemo {
    
   public static void main(String[] args) {
      NameRepository namesRepository = new NameRepository();

      for(Iterator iter = namesRepository.getIterator(); iter.hasNext();){
         String name = (String)iter.next();
         System.out.println("Name : " + name);
      }     
   }
}
```

```
Name : Robert
Name : John
Name : Julie
Name : Lora
```


