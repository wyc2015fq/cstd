public class MyThreadTest {
 
    public static void main(String[] args) {
        MyThread t = new MyThread();
        t.start();
 
        for (int i = 0; i < 10000; i++) {
            System.out.println("good!");
        }
    }
}

class MyThread extends Thread {
 
    @Override
    public void run() {
        for (int i = 0; i < 10000; i++) {
            System.out.println("nice!");
        }
    }
}

