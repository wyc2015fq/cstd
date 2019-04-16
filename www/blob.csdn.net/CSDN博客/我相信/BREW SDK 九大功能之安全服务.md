# BREW SDK 九大功能之安全服务 - 我相信...... - CSDN博客





2010年10月08日 11:39:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：604








BREW中的安全服务是指应用程式使用BREW提供的API来实现安全性的功能。对信源加密而言，BREW2.x采用ICipher ，BREW3.X中使用ICipher1替代ICipher，增强了很多功能，可以实现下述对称密钥加密算法：

¡  DES

l  ECB(Electronic Codebook)

l  CBC(Cipher Block Chaining)

l  CFB(Cipher FeedBack）

l  OFB(Output FeedBack)

l  CTR(Counter)

¡  3DES

¡  AES

加密实现可以通过软件，硬件或者由BREW自己决定使用最合适方案。同时，IRSA提供了非对称算法加密。非对称算法加密过程如下：发送方对明文M使用加密算法E和公开密钥K进行加密，得到密文C： EK（M）=C；接收方对密文C使用解密算法D和私人密钥K进行解密，得到明文M： DK（C）=M 。IRSA的计算复杂度是DES的100倍。

非对称算法加密中的公开密钥与私人密钥需要配对出现，但是不能由公开密钥推出私人密钥。IRSA同样可以拥有数字签名，一个典型的数字签名过程如下：

发送方对明文使用Hash算法得到摘要，对摘要使用私人密钥进行加密，得到签名文件 

明文和签名文件发送到接收方 

接收方对签名文件使用公开密钥进行解密，得到摘要，同时对明文使用Hash算法得到摘要，两个摘要如果一致，则接收方可以认为确认了发送方，因为除了发送方本人，其它任何人都无法由摘要产生签名文件。 

对信道加密而言，BREW中的ISSL接口可以认证用户和服务器，确保数据发送到正确的客户机和服务器，加密数据以防止数据中途被窃取，维护数据的完整性，确保数据在传输过程中不被改变。客户端产生的密钥只有客户端和服务器端能得到，加密的数据只有客户端和服务器端才能得到明文，客户端到服务端的通信是安全的。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/04/07/5456706.aspx](http://blog.csdn.net/wireless_com/archive/2010/04/07/5456706.aspx)



