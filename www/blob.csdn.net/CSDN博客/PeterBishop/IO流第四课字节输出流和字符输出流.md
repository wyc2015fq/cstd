# IO流第四课字节输出流和字符输出流 - PeterBishop - CSDN博客





2018年11月02日 21:57:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：38








  这节课讲字节输出流和字符输出流



  输出流和输入流差不多，只不过输入流是读取文件内容，输出流是向文件中写入内容



  直接看Demo吧:



Demo1: 通过字节输出流写入文件
public static void main(String[] args) {

    File file = new File("F:\\code\\java\\output.txt"); //还没有这个文件

    OutputStream os = null;

    try {

        file.createNewFile();

    } catch (IOException e) {

        e.printStackTrace();

    }

    try {

        os = new FileOutputStream(file);

        String content = "OutputStream";

        byte[] bytes = content.getBytes();

        os.write(bytes);

    } catch (FileNotFoundException e) {

        e.printStackTrace();

    } catch (IOException e) {

        e.printStackTrace();

    }finally {

        try{

            if (os != null)

                os.close();

        } catch (IOException e) {

            e.printStackTrace();

        }

    }

}




Demo2: 字符输出流写入文件
public static void main(String[] args) {

    File file = new File("F:\\code\\java\\writer.txt"); //这个文件显然不存在

    Writer writer = null;



    try {

        file.createNewFile();

    } catch (IOException e) {

        e.printStackTrace();

    }



    try {

        writer= new PrintWriter(file);

        String content = "Writer";

        writer.write(content);  //这里可以直接传String类型,

        // 看底层源码可以发现实际上还是讲content转换成char[]，然后调用writer.write(char cbuf[], int off, int len)

    } catch (FileNotFoundException e) {

        e.printStackTrace();

    } catch (IOException e) {

        e.printStackTrace();

    }finally {

        try {

            if (writer != null)

                writer.close();

        } catch (IOException e) {

            e.printStackTrace();

        }

    }

}




