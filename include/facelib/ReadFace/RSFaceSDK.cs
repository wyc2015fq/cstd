using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace readface_demo
{
    [StructLayout(LayoutKind.Sequential)]
    public struct rs_rect
    {
        public int left;
        public int top;
        public int width;
        public int height;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct rs_point
    {
        public float x;
        public float y;
    }

    public enum rs_image_rotation
    {
        RS_IMG_CLOCKWISE_ROTATE_0,
        RS_IMG_CLOCKWISE_ROTATE_90,
        RS_IMG_CLOCKWISE_ROTATE_180,
        RS_IMG_CLOCKWISE_ROTATE_270
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct rs_face
    {
        public rs_rect rect;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 21)]
        public rs_point[] landmarks21;
        public float yaw;
        public float pitch;
        public float roll;
        public int trackId;
        public float confidence;
    }

    public enum rs_pixel_format
    {
        PIX_FORMAT_GRAY,
        PIX_FORMAT_BGR888,
        PIX_FORMAT_NV21,
        PIX_FORMAT_BGRA8888,
        PIX_FORMAT_NV12,
        PIX_FORMAT_YUV_I420
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct rs_face_feature
    {
        public int version;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
        public float[] feature;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct rs_face_attr
    {
        public int glasses; // 0: without glasses, 1: with glasses
        public int mouth_open; // 0: mouth close, 1: mouth open
        public int gender; // 0: female, 1: male, -1: not sure
        public int age;
        public int beauty;
        public int eye_close; // 0: eye close, 1: eye opened
        public int smile;
    }

    public class RSFaceSDK
    {
        public static List<T> MarshalPtrToStrctArray<T>(IntPtr p, int count)
        {
            List<T> L = new List<T>();
            for (int i = 0; i < count; i++)
            {
                T t = (T)Marshal.PtrToStructure(p, typeof(T));
                L.Add(t);

                p = new IntPtr(p.ToInt32() + Marshal.SizeOf(typeof(T)));
            }
            return L;
        }

        ////////////////////////////
        // License Manager
        ////////////////////////////

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitLicenseManager(out IntPtr handle, string appId, string appSrcret);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitLicenseManager(out IntPtr handle);


        ////////////////////////////
        // Face Detect
        ////////////////////////////
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitFaceDetect(out IntPtr handle, IntPtr lm);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRunFaceDetect(IntPtr handle, byte[] image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
            rs_image_rotation img_rotate,
            out IntPtr pFaceArray,
            ref int pFaceCount);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitFaceDetect(out IntPtr handle);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void releaseFaceDetectResult(IntPtr pFaceArray, int faceCount);


        ////////////////////////////
        // Face Track
        ////////////////////////////
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitFaceTrack(out IntPtr handle, IntPtr lm);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRunFaceTrack(IntPtr handle, IntPtr image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
            rs_image_rotation img_rotate,
            out IntPtr pFaceArray,
            ref int pFaceCount);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitFaceTrack(out IntPtr handle);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void releaseFaceTrackResult(IntPtr pFaceArray, int faceCount);

        ////////////////////////////
        // Face Quality
        ////////////////////////////
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitFaceQuality(out IntPtr handle, IntPtr lm);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsGetFaceQualityScore(IntPtr handle, IntPtr image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
            IntPtr pFace);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitFaceQuality(IntPtr handle);

        ////////////////////////////
        // Emotion Detect
        ////////////////////////////
        string[] EMOTION_LABELS = { "joy", "sad", "surprise", "anger", "normal" };
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitEmotionDetect(out IntPtr handle, IntPtr lm);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRunEmotionDetect(IntPtr handle, IntPtr image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
            IntPtr pFace,
            ref float pEmotionProbs,
            ref int pEmotionId
            );

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitEmotionDetect(out IntPtr handle);

        ////////////////////////////
        // Face Attribute
        ////////////////////////////
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitAttributeDetect(out IntPtr handle, IntPtr lm);
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRunAttributeDetect(IntPtr handle, IntPtr image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
            IntPtr pFace,
            ref rs_face_attr pAttrVal
            );
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitAttributeDetect(out IntPtr handle);

        ////////////////////////////
        // Face Recognition
        ////////////////////////////
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitFaceRecognition(out IntPtr handle, IntPtr lm, string dbPath);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionSetConfidence(IntPtr handle, int confidence);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionResetAlbum(IntPtr handle);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionGetFaceFeature(IntPtr handle, IntPtr image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
            IntPtr pFace,
            IntPtr pFeature
            );

        // Person API
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionPersonCreate(IntPtr handle, IntPtr pFeature);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionPersonAddFace(IntPtr handle, IntPtr pFeature, int personId);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionPersonDelete(IntPtr handle, int personId);

        //int  rsRecognitionPersonRemoveFace(RSHandle handle,int faceIndex, int personId);

        //int  rsRecognitionPersonEmptyFace(RSHandle handle,int personId);

        // find the similairy of two faces
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static float rsRecognitionFaceVerification(IntPtr handle, IntPtr pFeature1, IntPtr pFeature2);

        // identify person from database
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionFaceIdentification(IntPtr handle, IntPtr pFeature);

        // get identify confidence
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static float rsRecognitionGetConfidence(IntPtr handle);


        // get how many people stored
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionGetAlbumSize(IntPtr handle);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionGetFaceFeaturesByPersonId(IntPtr handle, int personId, IntPtr pFeatureArray, int featureCount);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRecognitionReleaseFaceFeatureResult(IntPtr handle, IntPtr pFeatureArray, int featureCount);
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitFaceRecognition(out IntPtr handle);

        ////////////////////////////
        // Face Liveness Detect
        ////////////////////////////
        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsInitLivenessDetect(IntPtr handle, IntPtr lm);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int rsRunLivenessDetect(IntPtr handle, IntPtr image,
            rs_pixel_format pix_format,
            int width,
            int height,
            int stride,
           IntPtr pFace,
           IntPtr livenessState);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsResetLivenessDetect(IntPtr handle);

        [DllImport("libReadFace.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void rsUnInitLivenessDetect(IntPtr handle);
    }
}

