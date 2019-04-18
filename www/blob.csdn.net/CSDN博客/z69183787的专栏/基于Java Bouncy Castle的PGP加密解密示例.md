# 基于Java Bouncy Castle的PGP加密解密示例 - z69183787的专栏 - CSDN博客
2017年05月31日 10:40:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4076
GPG文件加解密 百科及教程：
[http://www.ruanyifeng.com/blog/2013/07/gpg.html](http://www.ruanyifeng.com/blog/2013/07/gpg.html)
[http://www.alexgao.com/2009/01/24/gpg/](http://www.alexgao.com/2009/01/24/gpg/)
pom：
```
<dependency>
            <groupId>commons-net</groupId>
            <artifactId>commons-net</artifactId>
        </dependency>
        <dependency>
            <groupId>org.bouncycastle</groupId>
            <artifactId>bcpg-jdk15on</artifactId>
        </dependency>
```
UTIL 工具类：
```java
package util;
import org.apache.commons.io.FileUtils;
import org.bouncycastle.bcpg.PublicKeyAlgorithmTags;
import org.bouncycastle.bcpg.sig.KeyFlags;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openpgp.*;
import org.bouncycastle.openpgp.operator.PBESecretKeyDecryptor;
import org.bouncycastle.openpgp.operator.bc.BcPBESecretKeyDecryptorBuilder;
import org.bouncycastle.openpgp.operator.bc.BcPGPDigestCalculatorProvider;
import org.bouncycastle.openpgp.operator.bc.BcPublicKeyDataDecryptorFactory;
import java.io.*;
import java.security.NoSuchProviderException;
import java.security.Security;
import java.util.Iterator;
public class PGPUtils {
    private static final int BUFFER_SIZE = 1 << 16; // should always be power of 2
    private static final int KEY_FLAGS = 27;
    private static final int[] MASTER_KEY_CERTIFICATION_TYPES = new int[]{
            PGPSignature.POSITIVE_CERTIFICATION,
            PGPSignature.CASUAL_CERTIFICATION,
            PGPSignature.NO_CERTIFICATION,
            PGPSignature.DEFAULT_CERTIFICATION
    };
    @SuppressWarnings("unchecked")
    public static PGPPublicKey readPublicKey(InputStream in)
            throws IOException, PGPException {
        PGPPublicKeyRingCollection keyRingCollection = new PGPPublicKeyRingCollection(PGPUtil.getDecoderStream(in));
        //
        // we just loop through the collection till we find a key suitable for encryption, in the real
        // world you would probably want to be a bit smarter about this.
        //
        PGPPublicKey publicKey = null;
        // iterate through the key rings.
        Iterator<PGPPublicKeyRing> rIt = keyRingCollection.getKeyRings();
        while (publicKey == null && rIt.hasNext()) {
            PGPPublicKeyRing kRing = rIt.next();
            Iterator<PGPPublicKey> kIt = kRing.getPublicKeys();
            while (publicKey == null && kIt.hasNext()) {
                PGPPublicKey key = kIt.next();
                if (key.isEncryptionKey()) {
                    publicKey = key;
                }
            }
        }
        if (publicKey == null) {
            throw new IllegalArgumentException("Can't find public key in the key ring.");
        }
        if (!isForEncryption(publicKey)) {
            throw new IllegalArgumentException("KeyID " + publicKey.getKeyID() + " not flagged for encryption.");
        }
        return publicKey;
    }
    @SuppressWarnings("unchecked")
    public static PGPSecretKey readSecretKey(InputStream in)
            throws IOException, PGPException {
        PGPSecretKeyRingCollection keyRingCollection = new PGPSecretKeyRingCollection(PGPUtil.getDecoderStream(in));
        PGPSecretKey secretKey = null;
        Iterator<PGPSecretKeyRing> rIt = keyRingCollection.getKeyRings();
        while (secretKey == null && rIt.hasNext()) {
            PGPSecretKeyRing keyRing = rIt.next();
            Iterator<PGPSecretKey> kIt = keyRing.getSecretKeys();
            while (secretKey == null && kIt.hasNext()) {
                PGPSecretKey key = kIt.next();
                if (key.isSigningKey()) {
                    secretKey = key;
                }
            }
        }
        // Validate secret key
        if (secretKey == null) {
            throw new IllegalArgumentException("Can't find private key in the key ring.");
        }
        if (!secretKey.isSigningKey()) {
            throw new IllegalArgumentException("Private key does not allow signing.");
        }
        if (secretKey.getPublicKey().isRevoked()) {
            throw new IllegalArgumentException("Private key has been revoked.");
        }
        if (!hasKeyFlags(secretKey.getPublicKey(), KeyFlags.SIGN_DATA)) {
            throw new IllegalArgumentException("Key cannot be used for signing.");
        }
        return secretKey;
    }
    /**
     * Load a secret key ring collection from keyIn and find the private key corresponding to
     * keyID if it exists.
     * <p/>
     *
     * @param keyIn input stream representing a key ring collection.
     * @param keyID keyID we want.
     * @param pass  passphrase to decrypt secret key with.
     * @return 156
     * @throws IOException             157
     * @throws PGPException            158
     * @throws NoSuchProviderException 159
     */
    public static PGPPrivateKey findPrivateKey(InputStream keyIn, long keyID, char[] pass)
            throws IOException, PGPException, NoSuchProviderException
    {
        PGPSecretKeyRingCollection pgpSec = new PGPSecretKeyRingCollection(PGPUtil.getDecoderStream(keyIn));
        return findPrivateKey(pgpSec.getSecretKey(keyID), pass);
    }
    /**
     * 169
     * Load a secret key and find the private key in it
     * 170
     *
     * @param pgpSecKey The secret key
     * @param pass      passphrase to decrypt secret key with
     * @throws PGPException
     */
    public static PGPPrivateKey findPrivateKey(PGPSecretKey pgpSecKey, char[] pass)
            throws PGPException {
        if (pgpSecKey == null) return null;
        PBESecretKeyDecryptor decryptor = new BcPBESecretKeyDecryptorBuilder(new BcPGPDigestCalculatorProvider()).build(pass);
        return pgpSecKey.extractPrivateKey(decryptor);
    }
    @SuppressWarnings("unchecked")
    public static void decryptFile(InputStream in, OutputStream out, InputStream keyIn, char[] passwd)
            throws Exception {
        Security.addProvider(new BouncyCastleProvider());
        in = org.bouncycastle.openpgp.PGPUtil.getDecoderStream(in);
        PGPObjectFactory pgpF = new PGPObjectFactory(in);
        PGPEncryptedDataList enc;
        Object o = pgpF.nextObject();
        // the first object might be a PGP marker packet.
        if (o instanceof PGPEncryptedDataList) {
            enc = (PGPEncryptedDataList) o;
        } else {
            enc = (PGPEncryptedDataList) pgpF.nextObject();
        }
        Iterator<PGPPublicKeyEncryptedData> it = enc.getEncryptedDataObjects();
        PGPPrivateKey sKey = null;
        PGPPublicKeyEncryptedData pbe = null;
        while (sKey == null && it.hasNext()) {
            pbe = it.next();
            sKey = findPrivateKey(keyIn, pbe.getKeyID(), passwd);
        }
        if (sKey == null) {
            throw new IllegalArgumentException("Secret key for message not found.");
        }
        InputStream clear = pbe.getDataStream(new BcPublicKeyDataDecryptorFactory(sKey));
        PGPObjectFactory plainFact = new PGPObjectFactory(clear);
        Object message = plainFact.nextObject();
        if (message instanceof PGPCompressedData) {
            PGPCompressedData cData = (PGPCompressedData) message;
            PGPObjectFactory pgpFact = new PGPObjectFactory(cData.getDataStream());
            message = pgpFact.nextObject();
        }
        if (message instanceof PGPLiteralData) {
            PGPLiteralData ld = (PGPLiteralData) message;
            InputStream unc = ld.getInputStream();
            int ch;
            while ((ch = unc.read()) >= 0) {
                out.write(ch);
            }
        } else if (message instanceof PGPOnePassSignatureList) {
            throw new PGPException("Encrypted message contains a signed message - not literal data.");
        } else {
            throw new PGPException("Message is not a simple encrypted file - type unknown.");
        }
        if (pbe.isIntegrityProtected()) {
            if (!pbe.verify()) {
                throw new PGPException("Message failed integrity check");
            }
        }
    }
    /**
     * From LockBox Lobs PGP Encryption tools.
     * http://www.lockboxlabs.org/content/downloads
     * <p/>
     * I didn't think it was worth having to import a 4meg lib for three methods
     */
    public static boolean isForEncryption(PGPPublicKey key) {
        if (key.getAlgorithm() == PublicKeyAlgorithmTags.RSA_SIGN
                || key.getAlgorithm() == PublicKeyAlgorithmTags.DSA
                || key.getAlgorithm() == PublicKeyAlgorithmTags.EC
                || key.getAlgorithm() == PublicKeyAlgorithmTags.ECDSA) {
            return false;
        }
        return hasKeyFlags(key, KeyFlags.ENCRYPT_COMMS | KeyFlags.ENCRYPT_STORAGE);
    }
    /**
     * From LockBox Lobs PGP Encryption tools.
     * http://www.lockboxlabs.org/content/downloads
     * <p/>
     * I didn't think it was worth having to import a 4meg lib for three methods
     */
    @SuppressWarnings("unchecked")
    private static boolean hasKeyFlags(PGPPublicKey encKey, int keyUsage) {
        if (encKey.isMasterKey()) {
            for (int i = 0; i != PGPUtils.MASTER_KEY_CERTIFICATION_TYPES.length; i++) {
                for (Iterator<PGPSignature> eIt = encKey.getSignaturesOfType(PGPUtils.MASTER_KEY_CERTIFICATION_TYPES[i]); eIt.hasNext(); ) {
                    PGPSignature sig = eIt.next();
                    if (!isMatchingUsage(sig, keyUsage)) {
                        return false;
                    }
                }
            }
        } else {
            for (Iterator<PGPSignature> eIt = encKey.getSignaturesOfType(PGPSignature.SUBKEY_BINDING); eIt.hasNext(); ) {
                PGPSignature sig = eIt.next();
                if (!isMatchingUsage(sig, keyUsage)) {
                    return false;
                }
            }
        }
        return true;
    }
    /**
     * From LockBox Lobs PGP Encryption tools.
     * http://www.lockboxlabs.org/content/downloads
     * <p/>
     * I didn't think it was worth having to import a 4meg lib for three methods
     */
    private static boolean isMatchingUsage(PGPSignature sig, int keyUsage) {
        if (sig.hasSubpackets()) {
            PGPSignatureSubpacketVector sv = sig.getHashedSubPackets();
            if (sv.hasSubpacket(PGPUtils.KEY_FLAGS)) {
                // code fix suggested by kzt (see comments)
                if ((sv.getKeyFlags() == 0 && keyUsage == 0)) {
                    return false;
                }
            }
        }
        return true;
    }
    public static void main(String[] args) throws Exception {
        FileInputStream in = new FileInputStream("/Users/fusheng/Desktop/DPXK_EBC17030001_14_20170306_26_1.csv.gpg");
        File keyInFile = new File("/Users/fusheng/Desktop/xieyuanshun.asc");
        FileInputStream keyIn = new FileInputStream(keyInFile);
        File outputFile = File.createTempFile("CITIC_DECRYPTED", ".csv");
        FileOutputStream out = new FileOutputStream(outputFile);
        PGPUtils.decryptFile(in, out, keyIn, "1qaz2wsx".toCharArray());
        System.out.println(FileUtils.readFileToString(outputFile));
    }
}
```
使用密钥解密文件并读取数据：
```java
if (!"gpg".equalsIgnoreCase(FilenameUtils.getExtension(file.getName()))) {
                continue;
            }
            FileInputStream in = new FileInputStream(file);
            File keyInFile = new File("/Users/fusheng/Desktop/xieyuanshun.asc");
            FileInputStream keyIn = new FileInputStream(keyInFile);
            File outputFile = File.createTempFile("CITIC_DECRYPTED", ".csv");
            FileOutputStream out = new FileOutputStream(outputFile);
            PGPUtils.decryptFile(in, out, keyIn, "1qaz2wsx".toCharArray());
            List<File> fileLST = new ArrayList<>();
            fileLST.add(file);
            List<String> referenceIds = FileUtils.readLines(outputFile);
```
