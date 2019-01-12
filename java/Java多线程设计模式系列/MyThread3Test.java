public class MyThread3Test {
    public static void main(String[] args) {
        MyThread3 t1 = new MyThread3("good!");
        MyThread3 t2 = new MyThread3("nice!");
         
        new Thread(t1).start();
        new Thread(t2).start();
    }
}
 
class MyThread3 implements Runnable {
    private String message;
 
    public MyThread3(String message) {
        this.message = message;
    }
 
    @Override
    public void run() {
        for (int i = 0; i < 10000; i++) {
            System.out.println(message);
        }
    }
}
