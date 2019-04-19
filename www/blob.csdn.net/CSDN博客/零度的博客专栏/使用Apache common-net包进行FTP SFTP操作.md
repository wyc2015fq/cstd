# 使用Apache common-net包进行FTP/SFTP操作 - 零度的博客专栏 - CSDN博客
2017年05月24日 17:39:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：3755
```
import org.apache.commons.net.ftp.FTPSClient;
import org.apache.commons.net.io.CopyStreamEvent;
import org.apache.commons.net.io.Util;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSessionContext;
import javax.net.ssl.SSLSocket;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.net.Socket;
import java.util.Locale;
public class SSLSessionReuseFTPSClient extends FTPSClient {
    private static final Logger logger = LoggerFactory.getLogger(SSLSessionReuseFTPSClient.class);
    /**
     * @param command the command to get
     * @param remote  the remote file name
     * @param local   the local file name
     * @return true if successful
     * @throws IOException on error
     * @since 3.1
     */
    @Override
    protected boolean _retrieveFile(String command, String remote, OutputStream local)
            throws IOException {
        Socket socket = _openDataConnection_(command, remote);
        if (socket == null) {
            return false;
        }
        final InputStream input;
        input = new BufferedInputStream(socket.getInputStream());
        // Treat everything else as binary for now
        try {
            Util.copyStream(input, local, getBufferSize(),
                    CopyStreamEvent.UNKNOWN_STREAM_SIZE, null,
                    false);
        } finally {
            Util.closeQuietly(input);
            Util.closeQuietly(socket);
        }
        // Get the transfer response
        boolean ok = completePendingCommand();
        return ok;
    }
    @Override
    protected void _prepareDataSocket_(final Socket socket) throws IOException {
        if (socket instanceof SSLSocket) {
            // Control socket is SSL
            final SSLSession session = ((SSLSocket) _socket_).getSession();
            final SSLSessionContext context = session.getSessionContext();
            //context.setSessionCacheSize(preferences.getInteger("ftp.ssl.session.cache.size"));
            try {
                final Field sessionHostPortCache = context.getClass().getDeclaredField("sessionHostPortCache");
                sessionHostPortCache.setAccessible(true);
                final Object cache = sessionHostPortCache.get(context);
                final Method method = cache.getClass().getDeclaredMethod("put", Object.class, Object.class);
                method.setAccessible(true);
                final String key = String.format("%s:%s", socket.getInetAddress().getHostName(),
                        String.valueOf(socket.getPort())).toLowerCase(Locale.ROOT);
                method.invoke(cache, key, session);
            } catch (NoSuchFieldException e) {
                // Not running in expected JRE
                logger.warn("No field sessionHostPortCache in SSLSessionContext", e);
            } catch (Exception e) {
                // Not running in expected JRE
                logger.warn(e.getMessage());
            }
        }
    }
}
```
```
import org.apache.commons.lang3.StringUtils;
import org.apache.commons.net.ftp.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
public class FTPUtil {
    private static final Logger logger = LoggerFactory.getLogger(FTPUtil.class);
    /**
     * Push a file to server
     */
    public static void storeFileToServer(String server, String username, String password, String directory, File file)
            throws IOException {
        SSLSessionReuseFTPSClient ftpsClient = new SSLSessionReuseFTPSClient();
        FTPClientConfig config = new FTPClientConfig();
        ftpsClient.configure(config);
        try {
            int reply;
            ftpsClient.connect(server);
            ftpsClient.login(username, password);
            // After connection attempt, you should check the reply code to verify
            // success.
            reply = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(reply)) {
                ftpsClient.disconnect();
            }
            ftpsClient.enterLocalPassiveMode();
            ftpsClient.setFileTransferMode(FTP.STREAM_TRANSFER_MODE);
            ftpsClient.execPROT("P");
            ftpsClient.changeWorkingDirectory(directory);
            ftpsClient.storeFile(file.getName(), new FileInputStream(file));
            ftpsClient.logout();
        } catch (IOException e) {
            logger.error("storeFileToServerFTPError", e);
            throw e;
        } finally {
            if (ftpsClient.isConnected()) {
                try {
                    ftpsClient.disconnect();
                } catch (IOException ioe) {
                    logger.error("storeFileToServerFTPErrorDisconnect", ioe);
                }
            }
        }
    }
    /**
     * Get FTP server files under the directory
     *
     * @param server
     * @param username
     * @param password
     * @param keyWord
     */
    public static List<File> getFiles(String server, String username, String password, String directory, String keyWord) {
        SSLSessionReuseFTPSClient ftpsClient = new SSLSessionReuseFTPSClient();
        FTPClientConfig config = new FTPClientConfig();
        ftpsClient.configure(config);
        try {
            int reply;
            ftpsClient.connect(server);
            ftpsClient.login(username, password);
            // After connection attempt, you should check the reply code to verify
            // success.
            reply = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(reply)) {
                ftpsClient.disconnect();
            }
            ftpsClient.enterLocalPassiveMode();
            ftpsClient.setFileTransferMode(FTP.STREAM_TRANSFER_MODE);
            ftpsClient.execPROT("P");
            List<File> files = new ArrayList<>();
            FTPFile[] ftpFiles = null;
            if (StringUtils.isBlank(directory)) {
                ftpFiles = ftpsClient.listFiles(ftpsClient.printWorkingDirectory());
            } else {
                ftpFiles = ftpsClient.listFiles(directory);
            }
            for (FTPFile ftpFile : ftpFiles) {
                if (ftpFile.isFile() && ftpFile.getName().indexOf(keyWord) >= 0) {
                    //String prefix = keyWord + "_" + DateFormatUtils.format(new Date(), "yyyy_MM_dd_HHmmss");
                    String remoteName = ftpFile.getName();
                    File tmpFile = File.createTempFile("TMP_", remoteName);
                    ftpsClient.setFileType(3);
                    ftpsClient.retrieveFile(remoteName, new FileOutputStream(tmpFile));
                    files.add(tmpFile);
                }
            }
            ftpsClient.logout();
            return files;
        } catch (IOException e) {
            logger.error("FTPError", e);
            return null;
        } finally {
            if (ftpsClient.isConnected()) {
                try {
                    ftpsClient.disconnect();
                } catch (IOException ioe) {
                    // do nothing
                }
            }
        }
    }
    /**
     * Get FTP server files under the directory
     *
     * @param server
     * @param username
     * @param password
     * @param keyWord
     */
    public static void doCommandRename(String server, String username, String password, String directory, String keyWord,
                                       String targetDirectory) {
        SSLSessionReuseFTPSClient ftpsClient = new SSLSessionReuseFTPSClient();
        FTPClientConfig config = new FTPClientConfig();
        ftpsClient.configure(config);
        try {
            int reply;
            ftpsClient.connect(server);
            ftpsClient.login(username, password);
            // After connection attempt, you should check the reply code to verify
            // success.
            reply = ftpsClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(reply)) {
                ftpsClient.disconnect();
            }
            ftpsClient.enterLocalPassiveMode();
            ftpsClient.setFileTransferMode(FTP.STREAM_TRANSFER_MODE);
            ftpsClient.execPROT("P");
            FTPFile[] ftpFiles = null;
            if (StringUtils.isBlank(directory)) {
                ftpFiles = ftpsClient.listFiles(ftpsClient.printWorkingDirectory());
            } else {
                ftpFiles = ftpsClient.listFiles(directory);
            }
            for (FTPFile ftpFile : ftpFiles) {
                if (ftpFile.isFile() && ftpFile.getName().indexOf(keyWord) >= 0) {
                    ftpsClient.rename(ftpFile.getName(), targetDirectory + ftpFile.getName());
                }
            }
            ftpsClient.logout();
        } catch (IOException e) {
            logger.error("FTPError", e);
        } finally {
            if (ftpsClient.isConnected()) {
                try {
                    ftpsClient.disconnect();
                } catch (IOException ioe) {
                    // do nothing
                }
            }
        }
    }
}
```
