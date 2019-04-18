# Visa-Api Reference 讲解 two way ssl（转） - z69183787的专栏 - CSDN博客
2019年02月14日 12:07:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：188
个人分类：[Http协议-HTTPS/SSL/TLS](https://blog.csdn.net/z69183787/article/category/7168332)
### [https://developer.visa.com/pages/working-with-visa-apis/two-way-ssl#ssl_handshake](https://developer.visa.com/pages/working-with-visa-apis/two-way-ssl#ssl_handshake)
### SSL Handshake
In Two-Way SSL authentication, the client and server need to authenticate and validate each others identities. The authentication message exchange between client and server is called an SSL handshake, and it includes the following steps:
- A client requests access to a protected resource.
- The server presents its certificate to the client.
- The client verifies the server's certificate.
- If successful, the client sends its certificate to the server.
- The server verifies the client’s credentials.
- If successful, the server grants access to the protected resource requested by the client.
In step 5 (above), the server validates the client, which is the second part of the Two-Way SSL (Mutual Authentication) process. This is typically done by making sure that the client certificate is valid (non-expired and issued by a trusted Certificate Authority), as well as the client’s digital signature is valid. Thedigital signatureis produced using the private key from the client. Anyone that has the public key can validate the digital signature. (However, only the client that has the private key can create a valid signature.)
### Establishing SSL Connection
To establish a Two-Way SSL (Mutual Authentication)connection, you must have the following:
- private key
- client certificate
- certificate authority root certificate, and 
- certificate authority intermediate certificates (**Note:** These certificates are optional for the Visa Developer sandbox)
The following steps include examples for context.
- Place your private key file (for example: ***privateKey.pem*** and your certificate file from VDP (for example: ***cert.pem ***in the same directory. Generate a keystore (for example: ***myProject_keyAndCertBundle.p12***) file as shown below.
	
> openssl pkcs12 -export -in cert.pem -inkey "privateKey.pem" -certfile cert.pem -out myProject_keyAndCertBundle.p12
**Note**: The ***myProject_keyAndCertBundle.p12*** is only a placeholder file name. You may choose to name it anything else.
- If you need a Java Key Store, run the following Java keytool command to convert your P12 file into a JKS file.
	
> keytool -importkeystore -srckeystore myProject_keyAndCertBundle.p12 -srcstoretype PKCS12 -destkeystore myProject_keyAndCertBundle.jks
- Run the following Java keytool commandto validate the contents of your new JKS file.
	
> keytool -list -v -keystore myProject_keyAndCertBundle.jks
- Run the following command to add the root certificate to your JKS file.
	
> keytool -import -alias ejbca -keystore myProject_keyAndCertBundle.jks -file VDPCA-SBX.pem -storepass <password>
***Configuring Two-Way SSL Keystore with Java Keytool***
Keytool is a certificate management utility that is part of the standard Java distribution.
**If you use a JKS file as your [truststore](https://docs.oracle.com/javase/6/docs/technotes/guides/security/jsse/JSSERefGuide.html#Stores)**, follow the instructions below to add the DigiCert Global Root CA certificate to your truststore.
- Download the DigiCert Global Root CA certificatefrom[https://dl.cacerts.digicert.com/DigiCertGlobalRootCA.crt](https://dl.cacerts.digicert.com/DigiCertGlobalRootCA.crt) to your local folder.
- Backup your truststore JKS file as a safeguard.
- Then, run the following keytool command to add it your truststore
keytool -import -alias DigiCertGlobalCA -keystore <Path to JKS (TrustStore) file> -file DigiCertGlobalRootCA.crt
