# 数据加密：什么是数据签名（What is a Digital Signature?） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年05月15日 12:55:24[boonya](https://me.csdn.net/boonya)阅读数：1212








From article：[http://www.youdzone.com/signature.html](http://www.youdzone.com/signature.html)

有个家伙已经将此文章翻译，如果要看可以点[here](http://www.blogjava.net/yxhxj2006/archive/2012/10/15/389547.html)[](http://www.youdzone.com/signature.html)


What is a Digital Signature?An introduction to Digital Signatures, by David Youd



|![](http://www.youdzone.com/images/sig/face4.gif)Bob|![](http://www.youdzone.com/images/sig/trans_half_inch.gif)|![](http://www.youdzone.com/images/sig/greenkey.GIF)(Bob's public key)![](http://www.youdzone.com/images/sig/redkey.GIF)(Bob's private key)|
|----|----|----|

Bob has been given two keys. One of Bob's keys is called a Public Key, the other is called a Private Key.



|Bob's Co-workers:| | | | |
|----|----|----|----|----|
|![](http://www.youdzone.com/images/sig/face1.gif)|![](http://www.youdzone.com/images/sig/face2.gif)|![](http://www.youdzone.com/images/sig/face3.gif)|![](http://www.youdzone.com/images/sig/trans_half_inch.gif)|![](http://www.youdzone.com/images/sig/greenkey.GIF)Anyone can get Bob's Public Key, but Bob keeps his Private Key to himself|
|Pat|Doug|Susan| | |

Bob's Public key is available to anyone who needs it, but he keeps his Private Keyto himself. Keys are used to encrypt information. Encrypting information means"scrambling it up", so that only a person with the appropriate key can make itreadable again.
 Either one of Bob's two keys can encrypt data, and the other key can decrypt that data.

Susan (shown below) can encrypt a message using Bob's Public Key. Bob uses hisPrivate Key to decrypt the message. Any of Bob's coworkers might have accessto the message Susan encrypted, but without Bob's Private Key, the data isworthless.



|![](http://www.youdzone.com/images/sig/face3.gif)|![](http://www.youdzone.com/images/sig/trans_half_inch.gif)|"Hey Bob, how about lunch at Taco Bell. I hear they have free refills!"|![](http://www.youdzone.com/images/sig/Encrypt_with_pub.gif)|HNFmsEm6UnBejhhyCGKOKJUxhiygSBCEiC0QYIh/Hn3xgiKBcyLK1UcYiYlxx2lCFHDC/A|
|----|----|----|----|----|


|![](http://www.youdzone.com/images/sig/face4.gif)|![](http://www.youdzone.com/images/sig/trans_half_inch.gif)|HNFmsEm6UnBejhhyCGKOKJUxhiygSBCEiC0QYIh/Hn3xgiKBcyLK1UcYiYlxx2lCFHDC/A|![](http://www.youdzone.com/images/sig/Decrypt_with_pri.gif)|"Hey Bob, how about lunch at Taco Bell. I hear they have free refills!"|
|----|----|----|----|----|

With his private key and the right software, Bob can put digital signatures ondocuments and other data. A digital signature is a "stamp" Bob places on thedata which is unique to Bob, and is very difficult to forge. In addition, thesignature assures that
 any changes made to the data that has been signed cannot go undetected.


|![](http://www.youdzone.com/images/sig/text.GIF)|![](http://www.youdzone.com/images/sig/hash.gif)|![](http://www.youdzone.com/images/sig/Message_digest.gif)|
|----|----|----|


|![](http://www.youdzone.com/images/sig/face4.gif)|To sign a document, Bob's software will crunch down the data into just a few linesby a process called "hashing". These few lines are called a message digest. (Itis not possible to change a message digest back into the original data from whichit was created.)|
|----|----|


|![](http://www.youdzone.com/images/sig/Message_digest.gif)|![](http://www.youdzone.com/images/sig/Encrypt_with_pri.gif)|![](http://www.youdzone.com/images/sig/signature.gif)|
|----|----|----|

Bob's software then encrypts the message digest with his private key. The result is the digital signature.


|![](http://www.youdzone.com/images/sig/signature.gif)|![](http://www.youdzone.com/images/sig/Append.gif)|![](http://www.youdzone.com/images/sig/signed_text.GIF)|
|----|----|----|

Finally, Bob's software appends the digital signature to document. All of thedata that was hashed has been signed.


|![](http://www.youdzone.com/images/sig/signed_text.GIF)|![](http://www.youdzone.com/images/sig/hash.gif)|![](http://www.youdzone.com/images/sig/Message_digest.gif)|
|----|----|----|
|![](http://www.youdzone.com/images/sig/Decrypt_with_pub.gif)|![](http://www.youdzone.com/images/sig/Message_digest.gif)| |

Bob now passes the document on to Pat.


|![](http://www.youdzone.com/images/sig/face1.gif)|First, Pat's software decrypts the signature (using Bob's public key) changing it backinto a message digest. If this worked, then it proves that Bob signed thedocument, because only Bob has his private key. Pat's software then hashesthe document data into a message digest. If the message digest is the sameas the message digest created when the signature was decrypted, then Patknows that the signed data has not been changed.|
|----|----|


Plot complication...

|![](http://www.youdzone.com/images/sig/face2.gif)|Doug (our disgruntled employee) wishes to deceive Pat. Doug makes surethat Pat receives a signed message and a public key that appears to belongto Bob. Unbeknownst to Pat, Doug deceitfully sent a key pair hecreated using Bob's name. Short of receiving Bob's public key from himin person, how can Pat be sure that Bob's public key is authentic?|
|----|----|

It just so happens that Susan works at the company's certificateauthority center. Susan can create a digital certificate for Bob simplyby signing Bob's public key as well as some information about Bob.


||Bob Info:    Name    Department    Cubical NumberCertificate Info:    Expiration Date    Serial NumberBob's Public Key:![](http://www.youdzone.com/images/sig/greenkey.GIF)||----||Bob Info:    Name    Department    Cubical NumberCertificate Info:    Expiration Date    Serial NumberBob's Public Key:![](http://www.youdzone.com/images/sig/greenkey.GIF)|![](http://www.youdzone.com/images/sig/trans_half_inch.gif)|![](http://www.youdzone.com/images/sig/sign_data.gif)![](http://www.youdzone.com/images/sig/face3.gif)|![](http://www.youdzone.com/images/sig/trans_half_inch.gif)|![](http://www.youdzone.com/images/sig/certificate.gif)|
|----|----|----|----|----|----|
|Bob Info:    Name    Department    Cubical NumberCertificate Info:    Expiration Date    Serial NumberBob's Public Key:![](http://www.youdzone.com/images/sig/greenkey.GIF)| | | | | |
![](http://www.youdzone.com/images/sig/trans_half_inch.gif)![](http://www.youdzone.com/images/sig/sign_data.gif)
![](http://www.youdzone.com/images/sig/face3.gif)![](http://www.youdzone.com/images/sig/trans_half_inch.gif)![](http://www.youdzone.com/images/sig/certificate.gif)
Now Bob's co-workers can check Bob's trusted certificate to make surethat his public key truly belongs to him. In fact, no one at Bob'scompany accepts a signature for which there does not exist a certificategenerated by Susan. This gives Susan the power
 to revoke signatures ifprivate keys are compromised, or no longer needed. There are even morewidely accepted certificate authorities that certify Susan.

Let's say that Bob sends a signed document to Pat. To verify the signatureon the document, Pat's software first uses Susan's (the certificateauthority's) public key to check the signature on Bob's certificate.Successful de-encryption of the certificate proves
 that Susan created it.After the certificate is de-encrypted, Pat's software can check ifBob is in good standingwith the certificate authority and that all of the certificate informationconcerning Bob's identity has not been altered.

Pat's software then takes Bob's public key from the certificate and uses itto check Bob's signature. If Bob's public key de-encrypts the signaturesuccessfully, then Pat is assured that the signature was created using Bob'sprivate key, for Susan has certified
 the matching public key. And of course,if the signature is valid, then we know that Doug didn't try to change thesigned content.



![](http://www.youdzone.com/images/sig/sig_image.gif)
Although these steps may sound complicated, they are all handled behindthe scenes by Pat's user-friendly software. To verify a signature, Pat needonly click on it.



(c) 1996, David Youd

Permission to change or distribute is at the discretion of the author

**Warning:** You may be missing a few lines of text if you print this document.This seems to occur on pages following pages that have blank space nearthe bottom due to moving tables with large graphics in them to
 the nextpage so that the images are not split across pages. If this happensto you, simply print out document in sections. (Ex: I have the problem onpage 4, so I print pages 1-3, then pages 4-5.)




![](http://www.youdzone.com/images/back.gif)





