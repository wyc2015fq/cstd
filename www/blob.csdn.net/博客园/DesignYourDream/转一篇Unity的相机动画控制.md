# 转一篇Unity的相机动画控制 - DesignYourDream - 博客园
# [转一篇Unity的相机动画控制](https://www.cnblogs.com/designyourdream/p/4271731.html)
最近真是忙，连研究细看的时间都没有了，原帖地址：https://alastaira.wordpress.com/2013/11/08/smooth-unity-camera-transitions-with-animation-curves/
先贴到这里
I’m creating a game in which I want to transition between two camera views – a top-down, overhead camera that gives a zoomed-out world view, and an over-the-shoulder style forward-facing camera to give a close-up view. Let’s say for the sake of discussion that these positions are defined relative to the player, as follows:
- Overview: Position x/y/z: (0, 100, 0). Rotation x/y/z: (-90, 0, 0)  (60 units above, rotated straight down)
- Close-up: Position x/y/z: (0, 0, –0.5). Rotation x/y/z: (0, 0, 0) (0.5 units behind, aligned with player direction)
Rather than simply cut between the two camera views, or present the top-down view in a second minimap camera alongside the main game view,  I want to use a single camera and transition smoothly between the two view locations. Initially, I tried to simply interpolate the position and rotation of the camera between the two locations (using [lerp()](http://docs.unity3d.com/Documentation/ScriptReference/Vector3.Lerp.html) for linear interpolation of the position vectors and [slerp()](http://docs.unity3d.com/Documentation/ScriptReference/Quaternion.Slerp.html) for the rotation quaternions), based on a single zoom parameter:
`transform.position = Vector3.Lerp(zoomstart.position, zoomend.position, zoom);`
`transform.rotation = Quaternion.Slerp(zoomstart.rotation, zoomend.rotation, zoom);`
This gives:
![image](https://alastaira.files.wordpress.com/2013/11/image_thumb15.png?w=422&h=561)
However, this didn’t create the effect I wanted – I wanted to remain largely looking down as the camera zoomed in, and only pull up to match that player’s look vector at relatively close zoom levels, and also to ease the movement at both ends of the zoom.
My next thought was to ease in/out the camera motion using one of the functions described in this [earlier post](https://alastaira.wordpress.com/2013/01/25/at-a-glance-functions-for-modelling-utility-based-game-ai/) – perhaps a sigmoid curve, so that the rate of rotation was greatest at either extreme of the zoom spectrum. But I had trouble finding a function that exactly described the motion I wanted to create.
And then I discovered Unity’s [AnimationCurves](http://docs.unity3d.com/Documentation/ScriptReference/AnimationCurve.html). I don’t know why they’re called animation curves, because they’re not specific to animations at all – they allow you to define custom curves based on an arbitrary number of control points (keys), and then evaluate the value of the function at any point along the curve.
![image](https://alastaira.files.wordpress.com/2013/11/image_thumb16.png?w=340&h=329)
To look up the value at any point along the curve use the Evaluate()  method. For example, in the curve above, the y value of the curve at x=0.5 is given by Curve.Evaluate(0.5) , which results in 0.692. Using this you can smoothly adjust any variable based on a single dimensional input.
## Curves for Smooth Camera Movement
I have three camera transform parameters that change between my two camera locations – the offset position in y and z axes, and the rotation in the x axis. Since each animation curve can express only one dimension, I thought I might need to create three animation curves – one to describe each parameter that varies as the camera zooms in. However, on more thought I decided a better way: to have one curve to control the rotation around the x axis (i.e. adjusting the pitch of the camera), and then a second curve to set the distance which the camera should be offset back *based on the forward direction of the camera at that point*. Since the camera rotates about the x axis, translating back along its normal means that the second curve will govern both the height (y) and offset (z) in a single curve.
You can create animation curves in the inspector, but I initialised them in code as follos:
`public class CameraScript : MonoBehaviour {`
`    ``// How camera pitch (i.e. rotation about x axis) should vary with zoom`
`    ``public AnimationCurve pitchCurve;`
`    ``// How far the camera should be placed back along the chosen pitch based on zoom`
`    ``public AnimationCurve distanceCurve;`
`    ``// Use this for initialization`
`    ``void Start () {`
`        ``// Create 'S' shaped curve to adjust pitch`
`        ``// Varies from 0 (looking forward) at 0, to 90 (looking straight down) at 1`
`        ``pitchCurve = AnimationCurve.EaseInOut(0.0f, 0.0f, 1.0f, 90.0f);`
`       `
`        ``// Create exponential shaped curve to adjust distance`
`        ``// So zoom control will be more accurate at closer distances, and more coarse further away`
`        ``Keyframe[] ks = new Keyframe[2];`
`        ``// At zoom=0, offset by 0.5 units`
`        ``ks[0] = new Keyframe(0, 0.5f);`
`        ``ks[0].outTangent = 0;`
`        ``// At zoom=1, offset by 60 units`
`        ``ks[1] = new Keyframe(1, 60);`
`        ``ks[1].inTangent = 90;`
`        ``distanceCurve = new AnimationCurve(ks);`
`    ``}`
`}`
This generates the following curves:
|Pitch![image](https://alastaira.files.wordpress.com/2013/11/image_thumb17.png?w=288&h=279)|Distance![image](https://alastaira.files.wordpress.com/2013/11/image_thumb18.png?w=289&h=280)|
I then evaluate the appropriate point from each curve based on the camera zoom level, as follows:
`// Calculate the appropriate pitch (x rotation) for the camera based on current zoom       `
`float targetRotX = pitchCurve.Evaluate(zoom);`
`// The desired yaw (y rotation) is to match that of the target object`
`float targetRotY = target.rotation.eulerAngles.y;`
`// Create target rotation as quaternion`
`// Set z to 0 as we don't want to roll the camera`
`Quaternion targetRot = Quaternion.Euler(targetRotX, targetRotY, 0.0f);`
`       `
`// Calculate in world-aligned axis, how far back we want the camera to be based on current zoom`
`Vector3 offset = Vector3.forward * distanceCurve.Evaluate(zoom);`
`// Then subtract this offset based on the current camera rotation`
`Vector3 targetPos = target.position - targetRot * offset;`
![image](https://alastaira.files.wordpress.com/2013/11/image_thumb19.png?w=422&h=563)
Finally, rather than setting the camera position/rotation directly, I lerp/slerp between the current transform.position/rotation towards the target position/rotation to smooth the movement and get the result as follows, which I’m pretty happy with (scene view on the left, resulting camera view on the right):

