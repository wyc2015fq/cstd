# RSA加密解密及RSA签名和验证  - ljx0305的专栏 - CSDN博客
2007年03月29日 13:58:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：958
此Demo包含两个文件，建立一个解决方案，然后建立两个文件，一个为Form，一个为Class，把代码分别复制进去即可
RSA正确的执行过程：
加密解密：
1、获取密钥，这里是产生密钥，实际应用中可以从各种存储介质上读取密钥
2、加密
3、解密
签名和验证：
签名：
1、获取密钥，这里是产生密钥，实际应用中可以从各种存储介质上读取密钥
2、获取待签名的Hash码
3、签名
其中，1和2的步骤无所谓，在本例中，我们将对txtSource里的内容进行签名，也可以对文件进行签名
验证签名：
1、获取密钥，这里是产生密钥，实际应用中可以从各种存储介质上读取密钥
2、获取待验证签名的Hash码
3、获取签名的字串，这里签名的字串存储在m_strEncryptedSignatureData变量中，在DEMO中必须通过签名才能获得这个字串，因此需要先执行签名，当然也可以更改之后通过别的方式获得
4、验证
其中，1和2的步骤无所谓，在本例中，我们将对txtSource里的内容进行签名验证，也可以对文件进行签名验证
 如果是文件，取得文件之后把文件的内容以byte[]的方式代入即可
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//RSACryption.cs
///////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Text;
using System.Security.Cryptography;
namespace RSAApplication
{
 /// <summary>
 /// RSACryption 的摘要说明。
 /// </summary>
 public class RSACryption
 {
  #region 构造函数
  public RSACryption()
  {
   //
   // TODO: 在此处添加构造函数逻辑
   //
  }
  #endregion
  #region RSA 加密解密
  #region RSA 的密钥产生
  //RSA 的密钥产生
  //产生私钥 和公钥
  public void RSAKey(out string xmlKeys,out string  xmlPublicKey)
  {
   try
   {
    System.Security.Cryptography.RSACryptoServiceProvider rsa=new RSACryptoServiceProvider();
    xmlKeys=rsa.ToXmlString(true);
    xmlPublicKey = rsa.ToXmlString(false);
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  #endregion
  #region RSA的加密函数
  //##############################################################################
  //RSA  方式加密 
  //说明KEY必须是XML的行式,返回的是字符串
  //在有一点需要说明！！该加密方式有 长度 限制的！！ 
  //##############################################################################
  //RSA的加密函数
  public string RSAEncrypt(string xmlPublicKey,string m_strEncryptString )
  {
   try
   {
    byte[] PlainTextBArray;
    byte[] CypherTextBArray;
    string Result;
    System.Security.Cryptography.RSACryptoServiceProvider rsa=new RSACryptoServiceProvider();
    rsa.FromXmlString(xmlPublicKey);
    PlainTextBArray = (new UnicodeEncoding()).GetBytes(m_strEncryptString);
    CypherTextBArray = rsa.Encrypt(PlainTextBArray, false);
    Result=Convert.ToBase64String(CypherTextBArray);
    return  Result;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //RSA的加密函数
  public string RSAEncrypt(string xmlPublicKey,byte[] EncryptString )
  {
   try
   {
    byte[] CypherTextBArray;
    string Result;
    System.Security.Cryptography.RSACryptoServiceProvider rsa=new RSACryptoServiceProvider();
    rsa.FromXmlString(xmlPublicKey);
    CypherTextBArray = rsa.Encrypt(EncryptString, false);
    Result=Convert.ToBase64String(CypherTextBArray);
    return  Result;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  #endregion
  #region RSA的解密函数
  //RSA的解密函数
  public string  RSADecrypt(string xmlPrivateKey, string m_strDecryptString )
  {
   try
   {
    byte[] PlainTextBArray;
    byte[] DypherTextBArray;
    string Result;
    System.Security.Cryptography.RSACryptoServiceProvider rsa=new RSACryptoServiceProvider();
    rsa.FromXmlString(xmlPrivateKey);
    PlainTextBArray =Convert.FromBase64String(m_strDecryptString);
    DypherTextBArray=rsa.Decrypt(PlainTextBArray, false);
    Result=(new UnicodeEncoding()).GetString(DypherTextBArray);
    return Result;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //RSA的解密函数
  public string  RSADecrypt(string xmlPrivateKey, byte[] DecryptString )
  {
   try
   {
    byte[] DypherTextBArray;
    string Result;
    System.Security.Cryptography.RSACryptoServiceProvider rsa=new RSACryptoServiceProvider();
    rsa.FromXmlString(xmlPrivateKey);
    DypherTextBArray=rsa.Decrypt(DecryptString, false);
    Result=(new UnicodeEncoding()).GetString(DypherTextBArray);
    return Result;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  #endregion
  #endregion
  #region RSA数字签名
  #region 获取Hash描述表
  //获取Hash描述表
  public bool GetHash(string m_strSource, ref byte[] HashData)
  {
   try
   {
    //从字符串中取得Hash描述
    byte[] Buffer;
    System.Security.Cryptography.HashAlgorithm MD5 = System.Security.Cryptography.HashAlgorithm.Create("MD5");
    Buffer = System.Text.Encoding.GetEncoding("GB2312").GetBytes(m_strSource);
    HashData = MD5.ComputeHash(Buffer);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //获取Hash描述表
  public bool GetHash(string m_strSource, ref string strHashData)
  {
   try
   {
    //从字符串中取得Hash描述
    byte[] Buffer;
    byte[] HashData;
    System.Security.Cryptography.HashAlgorithm MD5 = System.Security.Cryptography.HashAlgorithm.Create("MD5");
    Buffer = System.Text.Encoding.GetEncoding("GB2312").GetBytes(m_strSource);
    HashData = MD5.ComputeHash(Buffer);
    strHashData = Convert.ToBase64String(HashData);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //获取Hash描述表
  public bool GetHash(System.IO.FileStream objFile, ref byte[] HashData)
  {
   try
   {
    //从文件中取得Hash描述
    System.Security.Cryptography.HashAlgorithm MD5 = System.Security.Cryptography.HashAlgorithm.Create("MD5");
    HashData = MD5.ComputeHash(objFile);
    objFile.Close();
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //获取Hash描述表
  public bool GetHash(System.IO.FileStream objFile, ref string strHashData)
  {
   try
   {
    //从文件中取得Hash描述
    byte[] HashData;
    System.Security.Cryptography.HashAlgorithm MD5 = System.Security.Cryptography.HashAlgorithm.Create("MD5");
    HashData = MD5.ComputeHash(objFile);
    objFile.Close();
    strHashData = Convert.ToBase64String(HashData);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  #endregion
  #region RSA签名
  //RSA签名
  public bool SignatureFormatter(string p_strKeyPrivate, byte[] HashbyteSignature, ref byte[] EncryptedSignatureData)
  {
   try
   {
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPrivate);
    System.Security.Cryptography.RSAPKCS1SignatureFormatter RSAFormatter = new System.Security.Cryptography.RSAPKCS1SignatureFormatter(RSA);
    //设置签名的算法为MD5
    RSAFormatter.SetHashAlgorithm("MD5");
    //执行签名
    EncryptedSignatureData = RSAFormatter.CreateSignature(HashbyteSignature);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //RSA签名
  public bool SignatureFormatter(string p_strKeyPrivate, byte[] HashbyteSignature, ref string m_strEncryptedSignatureData)
  {
   try
   {
    byte[] EncryptedSignatureData;
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPrivate);
    System.Security.Cryptography.RSAPKCS1SignatureFormatter RSAFormatter = new System.Security.Cryptography.RSAPKCS1SignatureFormatter(RSA);
    //设置签名的算法为MD5
    RSAFormatter.SetHashAlgorithm("MD5");
    //执行签名
    EncryptedSignatureData = RSAFormatter.CreateSignature(HashbyteSignature);
    m_strEncryptedSignatureData = Convert.ToBase64String(EncryptedSignatureData);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //RSA签名
  public bool SignatureFormatter(string p_strKeyPrivate, string m_strHashbyteSignature, ref byte[] EncryptedSignatureData)
  {
   try
   {
    byte[] HashbyteSignature;
    HashbyteSignature = Convert.FromBase64String(m_strHashbyteSignature);
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPrivate);
    System.Security.Cryptography.RSAPKCS1SignatureFormatter RSAFormatter = new System.Security.Cryptography.RSAPKCS1SignatureFormatter(RSA);
    //设置签名的算法为MD5
    RSAFormatter.SetHashAlgorithm("MD5");
    //执行签名
    EncryptedSignatureData = RSAFormatter.CreateSignature(HashbyteSignature);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  //RSA签名
  public bool SignatureFormatter(string p_strKeyPrivate, string m_strHashbyteSignature, ref string m_strEncryptedSignatureData)
  {
   try
   {
    byte[] HashbyteSignature;
    byte[] EncryptedSignatureData;
    HashbyteSignature = Convert.FromBase64String(m_strHashbyteSignature);
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPrivate);
    System.Security.Cryptography.RSAPKCS1SignatureFormatter RSAFormatter = new System.Security.Cryptography.RSAPKCS1SignatureFormatter(RSA);
    //设置签名的算法为MD5
    RSAFormatter.SetHashAlgorithm("MD5");
    //执行签名
    EncryptedSignatureData = RSAFormatter.CreateSignature(HashbyteSignature);
    m_strEncryptedSignatureData = Convert.ToBase64String(EncryptedSignatureData);
    return true;
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  #endregion
  #region RSA 签名验证
  public bool SignatureDeformatter(string p_strKeyPublic, byte[] HashbyteDeformatter, byte[] DeformatterData)
  {
   try
   {
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPublic);
    System.Security.Cryptography.RSAPKCS1SignatureDeformatter RSADeformatter = new System.Security.Cryptography.RSAPKCS1SignatureDeformatter(RSA);
    //指定解密的时候HASH算法为MD5
    RSADeformatter.SetHashAlgorithm("MD5");
    if(RSADeformatter.VerifySignature(HashbyteDeformatter,DeformatterData))
    {
     return true;
    }
    else
    {
     return false;
    }
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  public bool SignatureDeformatter(string p_strKeyPublic, string p_strHashbyteDeformatter, byte[] DeformatterData)
  {
   try
   {
    byte[] HashbyteDeformatter;
    HashbyteDeformatter = Convert.FromBase64String(p_strHashbyteDeformatter);
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPublic);
    System.Security.Cryptography.RSAPKCS1SignatureDeformatter RSADeformatter = new System.Security.Cryptography.RSAPKCS1SignatureDeformatter(RSA);
    //指定解密的时候HASH算法为MD5
    RSADeformatter.SetHashAlgorithm("MD5");
    if(RSADeformatter.VerifySignature(HashbyteDeformatter,DeformatterData))
    {
     return true;
    }
    else
    {
     return false;
    }
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  public bool SignatureDeformatter(string p_strKeyPublic, byte[] HashbyteDeformatter, string p_strDeformatterData)
  {
   try
   {
    byte[] DeformatterData;
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPublic);
    System.Security.Cryptography.RSAPKCS1SignatureDeformatter RSADeformatter = new System.Security.Cryptography.RSAPKCS1SignatureDeformatter(RSA);
    //指定解密的时候HASH算法为MD5
    RSADeformatter.SetHashAlgorithm("MD5");
    DeformatterData =Convert.FromBase64String(p_strDeformatterData);
    if(RSADeformatter.VerifySignature(HashbyteDeformatter,DeformatterData))
    {
     return true;
    }
    else
    {
     return false;
    }
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  public bool SignatureDeformatter(string p_strKeyPublic, string p_strHashbyteDeformatter, string p_strDeformatterData)
  {
   try
   {
    byte[] DeformatterData;
    byte[] HashbyteDeformatter;
    HashbyteDeformatter = Convert.FromBase64String(p_strHashbyteDeformatter);
    System.Security.Cryptography.RSACryptoServiceProvider RSA = new System.Security.Cryptography.RSACryptoServiceProvider();
    RSA.FromXmlString(p_strKeyPublic);
    System.Security.Cryptography.RSAPKCS1SignatureDeformatter RSADeformatter = new System.Security.Cryptography.RSAPKCS1SignatureDeformatter(RSA);
    //指定解密的时候HASH算法为MD5
    RSADeformatter.SetHashAlgorithm("MD5");
    DeformatterData =Convert.FromBase64String(p_strDeformatterData);
    if(RSADeformatter.VerifySignature(HashbyteDeformatter,DeformatterData))
    {
     return true;
    }
    else
    {
     return false;
    }
   }
   catch(Exception ex)
   {
    throw ex;
   }
  }
  #endregion
  #endregion
 }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//frmRSACryptionTest.cs
///////////////////////////////////////////////////////////////////////////////////////////////////////////
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
namespace RSAApplication
{
 /// <summary>
 /// frmRSACryptionTest 的摘要说明。
 /// </summary>
 public class frmRSACryptionTest : System.Windows.Forms.Form
 {
  #region 必需的设计器变量
  /// <summary>
  /// 必需的设计器变量
  /// </summary>
  private System.Windows.Forms.Button btnBuildKey;
  private System.Windows.Forms.TextBox txtKeyPublic;
  private System.Windows.Forms.TextBox txtKeyPrivate;
  private System.ComponentModel.Container components = null;
  private System.Windows.Forms.Button btnRSAEncrypt;
  private System.Windows.Forms.TextBox txtRSADecrypt;
  private System.Windows.Forms.Button btnRSADecrypt;
  private System.Windows.Forms.TextBox txtSource;
  private System.Windows.Forms.TextBox txtRSAEncrypt;
  private System.Windows.Forms.Button btnSignature;
  private System.Windows.Forms.Button btnDeformatter;
  private System.Windows.Forms.Button btnGetHashSignature;
  private System.Windows.Forms.Button btnGetHashDeformatter;
  private System.Windows.Forms.TextBox txtSignature;
  private System.Windows.Forms.TextBox txtGetHashSignature;
  private System.Windows.Forms.TextBox txtGetHashDeformatter;
  private string m_strKeyPrivate = "";
  private string m_strKeyPublic = "";
  private string m_strHashbyteSignature   = "";
  private string m_strHashbyteDeformatter   = "";
  private string m_strEncryptedSignatureData  = "";
  #endregion
  #region 构造函数
  public frmRSACryptionTest()
  {
   //
   // Windows 窗体设计器支持所必需的
   //
   InitializeComponent();
   //
   // TODO: 在 InitializeComponent 调用后添加任何构造函数代码
   //
  }
  /// <summary>
  /// 清理所有正在使用的资源。
  /// </summary>
  protected override void Dispose( bool disposing )
  {
   if( disposing )
   {
    if (components != null) 
    {
     components.Dispose();
    }
   }
   base.Dispose( disposing );
  }
  #endregion
  #region Windows 窗体设计器生成的代码
  /// <summary>
  /// 设计器支持所需的方法 - 不要使用代码编辑器修改
  /// 此方法的内容。
  /// </summary>
  private void InitializeComponent()
  {
   this.btnBuildKey = new System.Windows.Forms.Button();
   this.txtKeyPublic = new System.Windows.Forms.TextBox();
   this.txtKeyPrivate = new System.Windows.Forms.TextBox();
   this.btnRSAEncrypt = new System.Windows.Forms.Button();
   this.txtSource = new System.Windows.Forms.TextBox();
   this.txtRSAEncrypt = new System.Windows.Forms.TextBox();
   this.txtRSADecrypt = new System.Windows.Forms.TextBox();
   this.btnRSADecrypt = new System.Windows.Forms.Button();
   this.btnDeformatter = new System.Windows.Forms.Button();
   this.btnSignature = new System.Windows.Forms.Button();
   this.txtSignature = new System.Windows.Forms.TextBox();
   this.btnGetHashSignature = new System.Windows.Forms.Button();
   this.btnGetHashDeformatter = new System.Windows.Forms.Button();
   this.txtGetHashSignature = new System.Windows.Forms.TextBox();
   this.txtGetHashDeformatter = new System.Windows.Forms.TextBox();
   this.SuspendLayout();
   // 
   // btnBuildKey
   // 
   this.btnBuildKey.Location = new System.Drawing.Point(11, 17);
   this.btnBuildKey.Name = "btnBuildKey";
   this.btnBuildKey.Size = new System.Drawing.Size(77, 34);
   this.btnBuildKey.TabIndex = 0;
   this.btnBuildKey.Text = "产生密钥";
   this.btnBuildKey.Click += new System.EventHandler(this.btnBuildKey_Click);
   // 
   // txtKeyPublic
   // 
   this.txtKeyPublic.Location = new System.Drawing.Point(137, 11);
   this.txtKeyPublic.Multiline = true;
   this.txtKeyPublic.Name = "txtKeyPublic";
   this.txtKeyPublic.Size = new System.Drawing.Size(602, 44);
   this.txtKeyPublic.TabIndex = 1;
   this.txtKeyPublic.Text = "";
   // 
   // txtKeyPrivate
   // 
   this.txtKeyPrivate.Location = new System.Drawing.Point(137, 58);
   this.txtKeyPrivate.Multiline = true;
   this.txtKeyPrivate.Name = "txtKeyPrivate";
   this.txtKeyPrivate.Size = new System.Drawing.Size(602, 44);
   this.txtKeyPrivate.TabIndex = 2;
   this.txtKeyPrivate.Text = "";
   // 
   // btnRSAEncrypt
   // 
   this.btnRSAEncrypt.Location = new System.Drawing.Point(11, 157);
   this.btnRSAEncrypt.Name = "btnRSAEncrypt";
   this.btnRSAEncrypt.Size = new System.Drawing.Size(77, 34);
   this.btnRSAEncrypt.TabIndex = 3;
   this.btnRSAEncrypt.Text = "RSA加密";
   this.btnRSAEncrypt.Click += new System.EventHandler(this.btnRSAEncrypt_Click);
   // 
   // txtSource
   // 
   this.txtSource.Location = new System.Drawing.Point(137, 108);
   this.txtSource.Multiline = true;
   this.txtSource.Name = "txtSource";
   this.txtSource.Size = new System.Drawing.Size(602, 44);
   this.txtSource.TabIndex = 4;
   this.txtSource.Text = "字串不能太长j——km,.ewm.m, .vkj中国福建";
   // 
   // txtRSAEncrypt
   // 
   this.txtRSAEncrypt.Location = new System.Drawing.Point(137, 155);
   this.txtRSAEncrypt.Multiline = true;
   this.txtRSAEncrypt.Name = "txtRSAEncrypt";
   this.txtRSAEncrypt.Size = new System.Drawing.Size(602, 44);
   this.txtRSAEncrypt.TabIndex = 5;
   this.txtRSAEncrypt.Text = "";
   // 
   // txtRSADecrypt
   // 
   this.txtRSADecrypt.Location = new System.Drawing.Point(137, 203);
   this.txtRSADecrypt.Multiline = true;
   this.txtRSADecrypt.Name = "txtRSADecrypt";
   this.txtRSADecrypt.Size = new System.Drawing.Size(602, 44);
   this.txtRSADecrypt.TabIndex = 6;
   this.txtRSADecrypt.Text = "";
   // 
   // btnRSADecrypt
   // 
   this.btnRSADecrypt.Location = new System.Drawing.Point(11, 202);
   this.btnRSADecrypt.Name = "btnRSADecrypt";
   this.btnRSADecrypt.Size = new System.Drawing.Size(77, 34);
   this.btnRSADecrypt.TabIndex = 7;
   this.btnRSADecrypt.Text = "RSA解密";
   this.btnRSADecrypt.Click += new System.EventHandler(this.btnRSADecrypt_Click);
   // 
   // btnDeformatter
   // 
   this.btnDeformatter.Location = new System.Drawing.Point(11, 396);
   this.btnDeformatter.Name = "btnDeformatter";
   this.btnDeformatter.Size = new System.Drawing.Size(77, 34);
   this.btnDeformatter.TabIndex = 10;
   this.btnDeformatter.Text = "RSA验证";
   this.btnDeformatter.Click += new System.EventHandler(this.btnDeformatter_Click);
   // 
   // btnSignature
   // 
   this.btnSignature.Location = new System.Drawing.Point(11, 297);
   this.btnSignature.Name = "btnSignature";
   this.btnSignature.Size = new System.Drawing.Size(77, 34);
   this.btnSignature.TabIndex = 9;
   this.btnSignature.Text = "RSA签名";
   this.btnSignature.Click += new System.EventHandler(this.btnSignature_Click);
   // 
   // txtSignature
   // 
   this.txtSignature.Location = new System.Drawing.Point(137, 298);
   this.txtSignature.Multiline = true;
   this.txtSignature.Name = "txtSignature";
   this.txtSignature.Size = new System.Drawing.Size(602, 44);
   this.txtSignature.TabIndex = 11;
   this.txtSignature.Text = "";
   // 
   // btnGetHashSignature
   // 
   this.btnGetHashSignature.Location = new System.Drawing.Point(11, 252);
   this.btnGetHashSignature.Name = "btnGetHashSignature";
   this.btnGetHashSignature.Size = new System.Drawing.Size(117, 36);
   this.btnGetHashSignature.TabIndex = 13;
   this.btnGetHashSignature.Text = "获取哈稀码(签名)";
   this.btnGetHashSignature.Click += new System.EventHandler(this.btnGetHashSignature_Click);
   // 
   // btnGetHashDeformatter
   // 
   this.btnGetHashDeformatter.Location = new System.Drawing.Point(11, 348);
   this.btnGetHashDeformatter.Name = "btnGetHashDeformatter";
   this.btnGetHashDeformatter.Size = new System.Drawing.Size(117, 36);
   this.btnGetHashDeformatter.TabIndex = 14;
   this.btnGetHashDeformatter.Text = "获取哈稀码(验证)";
   this.btnGetHashDeformatter.Click += new System.EventHandler(this.btnGetHashDeformatter_Click);
   // 
   // txtGetHashSignature
   // 
   this.txtGetHashSignature.Location = new System.Drawing.Point(137, 251);
   this.txtGetHashSignature.Multiline = true;
   this.txtGetHashSignature.Name = "txtGetHashSignature";
   this.txtGetHashSignature.Size = new System.Drawing.Size(602, 44);
   this.txtGetHashSignature.TabIndex = 15;
   this.txtGetHashSignature.Text = "";
   // 
   // txtGetHashDeformatter
   // 
   this.txtGetHashDeformatter.Location = new System.Drawing.Point(137, 346);
   this.txtGetHashDeformatter.Multiline = true;
   this.txtGetHashDeformatter.Name = "txtGetHashDeformatter";
   this.txtGetHashDeformatter.Size = new System.Drawing.Size(602, 44);
   this.txtGetHashDeformatter.TabIndex = 16;
   this.txtGetHashDeformatter.Text = "";
   // 
   // frmRSACryptionTest
   // 
   this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
   this.ClientSize = new System.Drawing.Size(764, 444);
   this.Controls.Add(this.txtGetHashDeformatter);
   this.Controls.Add(this.txtGetHashSignature);
   this.Controls.Add(this.txtSignature);
   this.Controls.Add(this.txtRSADecrypt);
   this.Controls.Add(this.txtRSAEncrypt);
   this.Controls.Add(this.txtSource);
   this.Controls.Add(this.txtKeyPrivate);
   this.Controls.Add(this.txtKeyPublic);
   this.Controls.Add(this.btnGetHashDeformatter);
   this.Controls.Add(this.btnGetHashSignature);
   this.Controls.Add(this.btnDeformatter);
   this.Controls.Add(this.btnSignature);
   this.Controls.Add(this.btnRSADecrypt);
   this.Controls.Add(this.btnRSAEncrypt);
   this.Controls.Add(this.btnBuildKey);
   this.Name = "frmRSACryptionTest";
   this.Text = "RSA加密解密";
   this.ResumeLayout(false);
  }
  #endregion
  #region 应用程序的主入口点
  /// <summary>
  /// 应用程序的主入口点
  /// </summary>
  [STAThread]
  static void Main() 
  {
   Application.Run(new frmRSACryptionTest());
  }
  #endregion
  #region 产生密钥
  private void btnBuildKey_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    RC.RSAKey(out m_strKeyPrivate, out m_strKeyPublic);
    this.txtKeyPrivate.Text = m_strKeyPrivate;
    this.txtKeyPublic.Text = m_strKeyPublic;
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  #endregion
  #region 加密解密
  private void btnRSAEncrypt_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    this.txtRSAEncrypt.Text = RC.RSAEncrypt(m_strKeyPublic, this.txtSource.Text);
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  private void btnRSADecrypt_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    this.txtRSADecrypt.Text = RC.RSADecrypt(m_strKeyPrivate, this.txtRSAEncrypt.Text);
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  #endregion
  #region 签名、验证
  #region 获取Hash码---针对签名
  private void btnGetHashSignature_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    if( RC.GetHash(this.txtSource.Text,ref m_strHashbyteSignature) == false)
    {
     MessageBox.Show(this,"取Hash码错误！","提示",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Warning);
    }
    this.txtGetHashSignature.Text = m_strHashbyteSignature;
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  #endregion
  #region 签名
  private void btnSignature_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    if( RC.SignatureFormatter(m_strKeyPrivate,m_strHashbyteSignature, ref m_strEncryptedSignatureData) == false)
    {
     MessageBox.Show(this,"RSA数字签名错误！","提示",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Warning);
    }
    this.txtSignature.Text = m_strEncryptedSignatureData;
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  #endregion
  #region 获取Hash码---针对验证
  private void btnGetHashDeformatter_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    if( RC.GetHash(this.txtSource.Text,ref m_strHashbyteDeformatter) == false)
    {
     MessageBox.Show(this,"取Hash码错误！","提示",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Warning);
    }
    this.txtGetHashDeformatter.Text = m_strHashbyteDeformatter;
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  #endregion
  #region 验证
  private void btnDeformatter_Click(object sender, System.EventArgs e)
  {
   try
   {
    RSACryption RC = new RSACryption();
    if( RC.SignatureDeformatter(m_strKeyPublic,m_strHashbyteDeformatter, m_strEncryptedSignatureData) == false)
    {
     MessageBox.Show(this,"身份验证失败！","提示",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Warning);
    }
    else
    {
     MessageBox.Show(this,"身份验证通过！","提示",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Warning);
    }
   }
   catch(Exception ex)
   {
    MessageBox.Show(this,ex.Message,"错误",System.Windows.Forms.MessageBoxButtons.OK,System.Windows.Forms.MessageBoxIcon.Error);
   }
  }
  #endregion
  #endregion
 }
}
Trackback: http://tb.blog.csdn.net/TrackBack.aspx?PostId=175165
 转自[http://blog.csdn.net/cnming/archive/2004/11/10/175165.aspx?Pending=true](http://blog.csdn.net/cnming/archive/2004/11/10/175165.aspx?Pending=true)
