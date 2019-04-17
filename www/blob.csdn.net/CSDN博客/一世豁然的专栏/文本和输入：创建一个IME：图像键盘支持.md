# 文本和输入：创建一个IME：图像键盘支持 - 一世豁然的专栏 - CSDN博客





2017年05月08日 10:57:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：678








用户经常想要使用emojis，贴纸，以及其他的丰富内容进行交流。在以前的Android版本中，软键盘（也称为输入法编辑器或IME）只能将unicode表情符号发送到应用程序。 对于丰富的内容，应用程序必须构建不能在其他应用程序中使用的特定于应用程序的API，或使用解决方法，例如通过“简易共享操作”或剪贴板发送图像。




使用Android 7.1（API级别25），Android SDK包含提交内容API，为IME提供了将图像和其他丰富内容直接发送到应用程序中的文本编辑器的通用方式。 该版本也可从版本25.0.0的v13支持库中获得。 我们建议使用支持库，因为它早在Android 3.2（API Level 13）上运行在设备上，它包含简化实现的帮助方法。





使用此API，您可以构建可从任何键盘接收丰富内容的消息传递应用程序，以及可向任何应用程序发送丰富内容的键盘。 Google键盘和Google Messenger等应用程序支持Android 7.1中的Commit Content API（参见图1）。


![](https://img-blog.csdn.net/20170508102524175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





此页面显示了如何在IME和应用程序中实现Commit Content API。








一、如何运行

键盘图像插入需要IME和应用程序的参与。 以下序列描述了图像插入过程中的每个步骤：


1、当用户点击EditText时，编辑器会在EditorInfo.contentMimeTypes中发送它接受的MIME内容类型列表。




2、IME读取支持的类型列表，并在编辑器可以接受的软键盘中显示内容。




3、当用户选择一个图像时，IME调用commitContent（）并向编辑器发送一个InputContentInfo。 commitContent（）调用类似于commitText（）调用，但是对于丰富的内容。 InputContentInfo包含一个标识内容提供者内容的URI。 然后，您的应用程序可以从URI请求许可并阅读内容。

![](https://img-blog.csdn.net/20170508102837523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








二、将图像支持添加到应用程序

要从IME接收丰富的内容，应用程序必须告诉IME它接受哪些内容类型，并指定在收到内容时执行的回调方法。 以下示例演示如何创建接受PNG图像的EditText：




```java
EditText editText = new EditText(this) {
    @Override
    public InputConnection onCreateInputConnection(EditorInfo editorInfo) {
        final InputConnection ic = super.onCreateInputConnection(editorInfo);
        EditorInfoCompat.setContentMimeTypes(editorInfo,
                new String [] {"image/png"});

        final InputConnectionCompat.OnCommitContentListener callback =
            new InputConnectionCompat.OnCommitContentListener() {
                @Override
                public boolean onCommitContent(InputContentInfoCompat inputContentInfo,
                        int flags, Bundle opts) {
                    // read and display inputContentInfo asynchronously
                    if (BuildCompat.isAtLeastNMR1() && (flags &
                        InputConnectionCompat.INPUT_CONTENT_GRANT_READ_URI_PERMISSION) != 0) {
                        try {
                            inputContentInfo.requestPermission();
                        }
                        catch (Exception e) {
                            return false; // return false if failed
                        }
                    }

                    // read and display inputContentInfo asynchronously.
                    // call inputContentInfo.releasePermission() as needed.

                    return true;  // return true if succeeded
                }
            };
        return InputConnectionCompat.createWrapper(ic, editorInfo, callback);
    }
};
```


有很多事情要做，所以让我们来解释一下发生了什么。

1、这个例子使用了支持库，所以有一些引用android.support.v13.view.inputmethod而不是android.view.inputmethod。




2、此示例创建一个EditText并覆盖其onCreateInputConnection（EditorInfo）方法来修改InputConnection。 InputConnection是IME和正在接收其输入的应用程序之间的通信通道。




3、调用super.onCreateInputConnection（）保留了内置的行为（发送和接收文本），并给出了对InputConnection的引用。




4、setContentMimeTypes（）将支持的MIME类型列表添加到EditorInfo。 确保在setContentMimeTypes（）之前调用super.onCreateInputConnection（）。




5、只要IME提交内容，都会执行回调。 方法onCommitContent（）有一个InputContentInfoCompat的引用，它包含一个内容URI。

 1）、如果应用程序在API 25级或更高版本上运行，并且IME设置了INPUT_CONTENT_GRANT_READ_URI_PERMISSION标志，则应该请求并释放权限。 否则，您应该已经具有内容URI的访问权限，因为它是由IME授予的，或者因为内容提供商不限制访问。 有关更多信息，请参阅[将图像支持添加到IME](https://developer.android.google.cn/guide/topics/text/image-keyboard.html?#adding_image_support_to_imes)。




6、createWrapper（）将inputConnection，修改的editorInfo和回调包装到一个新的InputConnection中并返回。




以下是一些推荐做法：


1、不支持丰富内容的编辑器不应调用setContentMimeTypes（），并将其EditorInfo.contentMimeTypes设置为null。




2、如果InputContentInfo中指定的MIME类型与其接受的任何类型不匹配，编辑者应忽略该内容。




3、丰富的内容不会影响文本光标的位置。 使用内容时，编辑者可以忽略光标位置。




4、在编辑器的OnCommitContentListener.onCommitContent（）方法中，即使在加载内容之前，也可以异步返回true。




5、与提交之前可以在IME中编辑的文本不同，丰富的内容将立即提交。 请注意，如果要向用户提供编辑或删除内容的功能，则必须自行实施逻辑。




要测试您的应用程序，请确保您的设备或仿真器具有能够发送丰富内容的键盘。 您可以使用Android 7.1或更高版本的Google键盘或安装CommitContent IME示例。





有关完整的代码示例，请参阅CommitContent App示例。








三、向IME添加图像支持

希望向应用发送丰富内容的IME应实现Commit Content API，如下所示：


1、覆盖onStartInput（）或onStartInputView（），并从目标编辑器读取支持的内容类型列表。 以下代码片段显示了如何检查目标编辑器是否接受GIF图像。



```java
@Override
public void onStartInputView(EditorInfo info, boolean restarting) {
    String[] mimeTypes = EditorInfoCompat.getContentMimeTypes(editorInfo);

    boolean gifSupported = false;
    for (String mimeType : mimeTypes) {
        if (ClipDescription.compareMimeTypes(mimeType, "image/gif")) {
            gifSupported = true;
        }
    }

    if (gifSupported) {
        // the target editor supports GIFs. enable corresponding content
    } else {
        // the target editor does not support GIFs. disable corresponding content
    }
}
```


2、当用户选择图像时，向应用提交内容。 当有任何撰写文字时，避免调用commitContent（），因为它可能导致编辑器失去焦点。 以下代码片段显示了如何提交GIF图像。



```java
/**
 * Commits a GIF image
 *
 * @param contentUri Content URI of the GIF image to be sent
 * @param imageDescription Description of the GIF image to be sent
 */
public static void commitGifImage(Uri contentUri, String imageDescription) {
    InputContentInfoCompat inputContentInfo = new InputContentInfoCompat(
            contentUri,
            new ClipDescription(imageDescription, new String[]{"image/gif"}));
    InputConnection inputConnection = getCurrentInputConnection();
    EditorInfo editorInfo = getCurrentInputEditorInfo();
    Int flags = 0;
    if (android.os.Build.VERSION.SDK_INT >= 25) {
        flags |= InputConnectionCompat.INPUT_CONTENT_GRANT_READ_URI_PERMISSION;
    }
    InputConnectionCompat.commitContent(
            inputConnection, editorInfo, inputContentInfo, flags, opts);
}
```


3、作为IME作者，您很可能必须实现自己的内容提供商来回应内容URI请求。 例外是——如果您的IME支持MediaStore等现有内容提供商的内容。 有关构建内容提供者的信息，请参阅CommitContent IME示例，内容提供者文档和文件提供者文档。




4、如果您正在构建自己的内容提供商，我们建议您不要导出它（设置android：exported为false）。 而是通过将android：grantUriPermission设置为true来启用在提供程序中的权限。 然后，当内容提交时，您的IME可以授予访问内容URI的权限。 有两种方法可以做到这一点：

 1）、在Android 7.1（API 25级）和更高版本上，当调用commitContent（）时，将标志参数设置为INPUT_CONTENT_GRANT_READ_URI_PERMISSION。 然后，应用程序接收的InputContentInfo对象可以通过调用requestPermission（）和releasePermission（）来请求和释放临时读取权限。

 2）、在Android 7.0（API级别24）及更低版本上，INPUT_CONTENT_GRANT_READ_URI_PERMISSION将被忽略，因此您需要手动授予内容权限。 一种方法是使用grantUriPermission（），但是您可以实现自己的机制来满足自己的需求。




有关授权许可的示例，请参阅CommitContent IME示例中的doCommitContent（）方法。





要测试您的IME，请确保您的设备或仿真器具有能够收到丰富内容的应用程序。 您可以在Android 7.1或更高版本中使用Google Messenger应用，也可以安装CommitContent App示例。





有关完整的代码示例，请参阅CommitContent IME示例。




