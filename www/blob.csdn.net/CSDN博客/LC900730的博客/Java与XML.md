# Java与XML - LC900730的博客 - CSDN博客
2017年07月24日 15:58:44[lc900730](https://me.csdn.net/LC900730)阅读数：179标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Java读写XML
```
public void addContent(Contact contact){
    try{
            //如果文件存在
            File file=new File("/Users/edz/Desktop/contact.xml");
            Document doc=null;
            if(!file.exists()){
                Document doc=DocumentHelper.createDocument();
            }else{
                doc=new SAXReader().read(file);
            }
            //创建根标签
            Element rootElem=doc.addElement("contactList");
            Element contactElem=rootElem.addElement("contact");
            contactElem.addAttribute("id", contact.getId());
            contactElem.addElement("name").setText( contact.getName());
            contactElem.addElement("gender").setText(contact.getGender());
            contactElem.addElement("email").setText( contact.getEmail());
            contactElem.addElement("age").setText(contact.getAge()+"");
            contactElem.addElement("phone").setText(contact.getPhone());
            contactElem.addElement("qq").setText(contact.getQq());
//          doc.addElement("contactList");
            //把Documen写出到xml文件中
    FileOutputStream out=new FileOutputStream("/Users/edz/Desktop/contact.xml");
            OutputFormat format=OutputFormat.createPrettyPrint();
            format.setEncoding("utf-8");
            XMLWriter writer=new XMLWriter(out,format);
            writer.write(doc);
            writer.close();
    }catch(Exception e){
        e.print();
    }
```
但是如果文件存在，那么
