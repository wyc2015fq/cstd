# Qt WebRTC demo - DoubleLi - 博客园






This is a very simple demonstration of how to stream from a native application to the browser using WebRTC.

But first, a small demonstration of how a call is set up in WebRTC. A call is established by exchanging SDP and ICE information using some unspecified method. In the movie below, I simply copy-paste the required information from a 'sender' web page to a 'receiver' web page. The sender is the one that captures the webcam and streams it to the receiver where it is displayed in a video tag.

(best viewed full screen)



If you'd like to try this yourself, these are the pages:
- [sender.html](http://research.edm.uhasselt.be/jori/qtwebrtc/sender.html)
- [receiver.html](http://research.edm.uhasselt.be/jori/qtwebrtc/receiver.html)

I've noticed that the copy-pasting needs to be done rather swiftly to get it to work.

Although my hand waving skills are undoubtedly very impressive, it would be great to be able to stream something else to the browser. The following video demonstrates a small native Qt application which streams the contents of a widget to a browser. The call set up is still done using the copy-paste technique, to keep the code in the application to a minimum.



The source of this application is available in two formats:


- A [patch](http://research.edm.uhasselt.be/jori/qtwebrtc/qtwebrtc_textedit.patch) against the [WebRTC source tree](http://www.webrtc.org/reference/getting-started) (I used revision 5628).


- First, do the 'gclient config' and 'gclient sync' steps as described on that page.
- Apply the patch
- Go to the `trunk/talk/examples/qtwebrtc_textedit` directory and run the `runme.sh` script (runs the `uic` and `moc` commands, I didn't figure out how to do this in the gyp file)
- Run the `./webrtc/build/gyp_webrtc` command to generate the ninja build files
- Run ninja to build everything



- A [tarball](http://research.edm.uhasselt.be/jori/qtwebrtc/qtwebrtc_textedit.tar.gz) containing a qmake project file to build everything.


- Make sure you've got [WebRTC](http://www.webrtc.org/reference/getting-started) compiled somewhere.
- Extract the tarball somewhere
- Run 'qmake WEBRTCTRUNK=/path/to/webrtc/trunk' and make


Although the source code itself probably also works in Windows, I've only tested it on Linux and the patch/qmake file are Linux specific. For the qmake project, you may need to remove some libraries or add some extra libraries, depending on the WebRTC build settings on your platform. The project file is for Qt 5.

Big thanks to Kam for making the [modified peerconnection code](http://sourcey.com/webrtc-native-to-browser-video-streaming-example/) available, it really helped getting me started using the native WebRTC library.



from：http://research.edm.uhasselt.be/jori/page/Misc/QtWebRTC.html









