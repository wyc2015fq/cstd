# Java设计模式（命令模式） - DEVELOPER - CSDN博客





2017年04月15日 11:41:48[学术袁](https://me.csdn.net/u012827205)阅读数：589
所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)









今天介绍一下命令模式！昨天晚上给老板怼了一架，这么虐待我们“改变世界的人”，对我可以但是怼我的兄弟就不行！当场提出离职，所以为了庆祝一下。今天就介绍介绍这种模式。

最近我想我们都在看一部电视剧《人民的名义》，没看过的赶紧去脑补一下！这么经典的电视剧，就是抛下女朋友不管也得看一看。特别式里面的人物，我的偶像，李达康书记还有小鲜肉侯亮平反贪局长！来看一张图，就这眼神这演技，满满的一百分~

![](https://img-blog.csdn.net/20170415112453321)


书归正传，谈咱们的java设计模式（命令模式）：

什么式命令模式麽？将一个命令请求封装成一个对象，从而使我们可用不同的命令请求对客户进行参数化;对请求排队或者记录请求日志，以及支持可撤销的操作。

命令模式的优点是啥？命令模式的优点是实现了命令的发起对象和命令的具体实现对象———命令接收者的完全解耦，这样很方便扩展新的命令，只要实现新的命令对象，然后在装配的时候，把具体实现对象设置到命令中，就可以使用这个命令对象了，不用改变已有的实现。

假如有这样一个情景：李达康书记发现高晓琴贪污犯罪，这时候就命令侯亮平局长立刻前往山水庄园进行逮捕！好，我们使用代码实现一下：

```java
//李达康 京州市市长 （命令的发起人）
public class LiDakang {

    private ICommand iCommand;
    public LiDakang(ICommand iCommand){
        this.iCommand = iCommand;
    }

    //李达康书记，发起命令:赶紧的~逮捕那个贪官！
    public void sendCommand(){
        System.out.println("命令：立刻去她家里进行逮捕~");
        iCommand.executeCommand();
    }
}
```


```java
//命令接口
public interface ICommand {
    //执行命令方法（并非实际执行操作）
    void executeCommand();
}
```





```java
//命令的实现类（真正收到达康书记命令的类，并要求亮平执行任务）
//命令实现类是没有真正实现命令要求的功能的；又被称做"命令触发器"，功能是持有命令的对象
public class CommandExecute implements ICommand {
    private HouLingping lingping;
    public CommandExecute(HouLingping lingping){
        this.lingping = lingping;
    }
    @Override
    public void executeCommand() {
        //执行逮捕
        lingping.arrestedSomeBody();
    }
}
```




```java
//侯亮平 反贪局局长 （命令的执行者 Receiver）
//真正执行命令的功能是接收者
public class HouLingping {
    /**
     * 功能：实施命令：逮捕 某人
     */
    public void arrestedSomeBody(){
        System.out.println("我立刻开着我的法拉利去逮捕她~");
    }
}
```





代码实现：





```java
//功能实现类
public class ClientExecute {
    public static void main(String[] args){
        HouLingping lingping = new HouLingping();
        ICommand iCommand = new CommandExecute(lingping);
        LiDakang dakang = new LiDakang(iCommand);

        //达康书记的命令一发起，侯局长就会立刻收到命令，并开着跑车去执行命令，进行逮捕任务工作
        dakang.sendCommand();
    }
}
```









