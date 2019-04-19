# Untiy Native Render Plugin在VR中的绘制 - 逍遥剑客 - CSDN博客
2016年04月10日 21:49:50[xoyojank](https://me.csdn.net/xoyojank)阅读数：3351
官方的NativeRenderPlugin Sample只是画了一个屏幕空间的三角形, 怎么改成世界空间的呢? 以D3D11为例:
Native:
```
struct ConstantBuffer
{
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Projection;
} g_CB;
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetCameraMatrix(float vm[], float pm[])
{
    g_CB.View = XMLoadFloat4x4(&DirectX::XMFLOAT4X4(vm));
    g_CB.Projection = XMLoadFloat4x4(&DirectX::XMFLOAT4X4(pm));
}
```
由于DirectX Math已经是row major的, 所以不用转置了. 
Shader:
```
cbuffer MyCB : register(b0) 
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
}
void VS (float3 pos : POSITION, float4 color : COLOR, out float4 ocolor : COLOR, out float4 opos : SV_Position) 
{
    opos = mul(float4(pos, 1), worldMatrix);
    opos = mul(opos, viewMatrix);
    opos = mul(opos, projectionMatrix);
    ocolor = color;
}
```
C#脚本这边有个细节, 就是投影矩阵需要转换一下, 不能直接取相机的:
```
[DllImport("RenderingPlugin")]
    private static extern void SetCameraMatrix(float[] viewMatrix, float[] projectionMatrix);
    private static float[] MatrixToArray(Matrix4x4 _matrix)
    {
        float[] result = new float[16];
        for (int _row = 0; _row < 4; _row++)
        {
            for (int _col = 0; _col < 4; _col++)
            {
                result[_col + _row * 4] = _matrix[_row, _col];
            }
        }
        return result;
    }
    void OnPostRender()
    {
        // Set time for the plugin
        SetTimeFromUnity(Time.timeSinceLevelLoad);
        if (Camera.current != null)
        {
            var viewMatrix = Camera.current.worldToCameraMatrix;
            var projectionMatrix = GL.GetGPUProjectionMatrix(Camera.current.projectionMatrix, VRSettings.enabled);
            SetCameraMatrix(MatrixToArray(viewMatrix), MatrixToArray(projectionMatrix));
        }
        // Issue a plugin event with arbitrary integer identifier.
        // The plugin can distinguish between different
        // things it needs to do based on this ID.
        // For our simple plugin, it does not matter which ID we pass here.
        GL.IssuePluginEvent(GetRenderEventFunc(), 1);
    }
```
这个脚本挂到Camera上即可, OnPostRender会分别针对左右眼调用两次, 所以Native那边会产生两次绘制.  
另外, Native这边也可以从视图矩阵中还原出眼睛位置:
```
XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, g_CB.View);
        XMVECTOR eyePos = XMMatrixTranspose(invViewMatrix).r[3];
```
# 参考资料
[http://forum.unity3d.com/threads/native-c-plugin-in-world-space.196512/](http://forum.unity3d.com/threads/native-c-plugin-in-world-space.196512/)
[https://github.com/obviousjim/ofxUnity/blob/master/UnityProject/Assets/UseRenderingPlugin.cs](https://github.com/obviousjim/ofxUnity/blob/master/UnityProject/Assets/UseRenderingPlugin.cs)
[http://docs.unity3d.com/ScriptReference/Camera-projectionMatrix.html](http://docs.unity3d.com/ScriptReference/Camera-projectionMatrix.html)
[http://docs.unity3d.com/ScriptReference/Camera.Render.html](http://docs.unity3d.com/ScriptReference/Camera.Render.html)
[https://forums.oculus.com/community/discussion/16313/native-rendering-plugin-with-oculus-rift](https://forums.oculus.com/community/discussion/16313/native-rendering-plugin-with-oculus-rift)
[http://forum.unity3d.com/threads/different-content-in-each-eye.332575/](http://forum.unity3d.com/threads/different-content-in-each-eye.332575/)
