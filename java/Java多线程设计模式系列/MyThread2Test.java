public class MyThread2Test {
    public static void main(String[] args) {
        MyThread2 t1 = new MyThread2("good!");
        MyThread2 t2 = new MyThread2("nice!");
         
        t1.start();
        t2.start();
    }
     
}
 
class MyThread2 extends Thread {
    private String message;
 
    public MyThread2(String message) {
        this.message = message;
    }
 
    @Override
    public void run() {
        for (int i = 0; i < 10000; i++) {
            System.out.println(message);
        }
    }
}