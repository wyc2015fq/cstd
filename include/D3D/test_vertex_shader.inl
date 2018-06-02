/*
This is a simple vertex shader program which illustrate how to use vertex shader instead of fixed pipeline
to perform world, view and projection transform
*/
#define D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY 0
#include <d3dx9.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
LPDIRECT3D9 g_pD3D = NULL ; // Used to create the D3DDevice
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL ; // Our rendering device
ID3DXMesh* g_pMesh = NULL ; // Hold the sphere
IDirect3DVertexShader9* g_pVertexShader = NULL ; // vertex shader
ID3DXConstantTable* g_pConstantTable = NULL ; // shader constant table
// Handle for world, view and projection matrix
// We use this variable to communicate between the effect file Shader.txt and the C++ code
D3DXHANDLE ViewProjMatrixHanle = 0 ;
void SetupMatrix() ;
bool PrepareShader() ;
HRESULT InitD3D(HWND hWnd)
{
  // Create the D3D object, which is needed to create the D3DDevice.
  if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
    return E_FAIL;
  }
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory(&d3dpp, sizeof(d3dpp));
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  // Create device
  if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
      &d3dpp, &g_pd3dDevice))) {
    return E_FAIL;
  }
  // Turn off culling, so we see the front and back of the triangle
  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  //g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
  g_pd3dDevice->SetRenderState(D3DRS_LIGHTING , FALSE);
  // Create a teapot
  D3DXCreateTeapot(g_pd3dDevice, &g_pMesh, NULL) ;
  // Prepare Shader
  PrepareShader() ;
  // Setup matrix
  SetupMatrix() ;
  return S_OK;
}
VOID Cleanup()
{
  if (g_pd3dDevice != NULL) {
    g_pd3dDevice->Release();
  }
  if (g_pD3D != NULL) {
    g_pD3D->Release();
  }
  if (g_pMesh != NULL) {
    g_pMesh->Release() ;
  }
}
bool PrepareShader()
{
  // Buffer to hold the compiled code
  ID3DXBuffer* codeBuffer = NULL;
  // Buffer to hold the error message if complile failed
  ID3DXBuffer* errorBuffer = NULL;
  // Compile shader from file
  HRESULT hr = D3DXCompileShaderFromFileA("E:/code/cstd/shader/vertexshader.txt", 0, 0, "Main", "vs_1_1",
      D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &codeBuffer, &errorBuffer, &g_pConstantTable) ;
  // output any error messages
  if (errorBuffer) {
    MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
    errorBuffer->Release() ;
    return false ;
  }
  if (FAILED(hr)) {
    MessageBox(0, "D3DXCompileShaderFromFile() - FAILED", 0, 0);
    return false;
  }
  // Create vertex shader
  hr = g_pd3dDevice->CreateVertexShader((DWORD*)codeBuffer->GetBufferPointer(), &g_pVertexShader) ;
  // handling error
  if (FAILED(hr)) {
    MessageBox(0, "CreateVertexShader - FAILED", 0, 0);
    return false;
  }
  // Release code buffer
  if (codeBuffer != NULL) {
    codeBuffer->Release() ;
    codeBuffer = NULL ;
  }
  // Release DX buffer
  if (errorBuffer != NULL) {
    errorBuffer->Release() ;
    errorBuffer = NULL ;
  }
  // Set handle
  ViewProjMatrixHanle = g_pConstantTable->GetConstantByName(0, "gWVP") ;
  return true ;
}
void SetupMatrix()
{
  D3DXVECTOR3 eyePt(0.0f, 0.0f, -10.0f) ;
  D3DXVECTOR3 upVec(0.0f, 1.0f, 0.0f) ;
  D3DXVECTOR3 lookCenter(0.0f, 0.0f, 0.0f) ;
  // Set view matrix
  D3DXMATRIX view ;
  D3DXMatrixLookAtLH(&view, &eyePt, &lookCenter, &upVec) ;
  // Set projection matrix
  D3DXMATRIX proj ;
  D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f) ;
  D3DXMATRIX viewproj = view * proj ;
  g_pConstantTable->SetMatrix(g_pd3dDevice, ViewProjMatrixHanle, &viewproj) ;
  // this line is mandatory if you have used Constant table in your code
  g_pConstantTable->SetDefaults(g_pd3dDevice);
}
VOID Render()
{
  // Clear the back-buffer to a red color
  g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  // Begin the scene
  if (SUCCEEDED(g_pd3dDevice->BeginScene())) {
    // Set Vertex Shader
    g_pd3dDevice->SetVertexShader(g_pVertexShader) ;
    // Render teapot
    g_pMesh->DrawSubset(0) ;
    // End the scene
    g_pd3dDevice->EndScene();
  }
  // Present the back-buffer contents to the display
  g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
  case WM_KEYDOWN: {
    switch (wParam) {
    case VK_ESCAPE:
      SendMessage(hWnd, WM_CLOSE, 0, 0);
      break ;
    default:
      break ;
    }
  }
  break ;
  case WM_DESTROY:
    Cleanup();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}
int test_vertex_shader()
{
  HINSTANCE hInstance = NULL;
  //, HINSTANCE hPrevInstance, LPWSTR szCmdLine, int iCmdShow;
  WNDCLASSEX winClass ;
  winClass.lpszClassName = "Teapot";
  winClass.cbSize = sizeof(WNDCLASSEX);
  winClass.style = CS_HREDRAW | CS_VREDRAW;
  winClass.lpfnWndProc = MsgProc;
  winClass.hInstance = hInstance;
  winClass.hIcon = NULL ;
  winClass.hIconSm = NULL ;
  winClass.hCursor = NULL ;
  winClass.hbrBackground = NULL ;
  winClass.lpszMenuName = NULL ;
  winClass.cbClsExtra = 0;
  winClass.cbWndExtra = 0;
  RegisterClassEx(&winClass) ;
  HWND hWnd = CreateWindowExA(NULL,
      winClass.lpszClassName, // window class name
      "Teapot", // window caption
      WS_OVERLAPPEDWINDOW, // window style
      32, // initial x position
      32, // initial y position
      600, // initial window width
      600, // initial window height
      NULL, // parent window handle
      NULL, // window menu handle
      hInstance, // program instance handle
      NULL) ; // creation parameters
  // Create window failed
  if (hWnd == NULL) {
    MessageBoxA(hWnd, "Create Window failed!", "Error", 0) ;
    return -1 ;
  }
  // Initialize Direct3D
  if (SUCCEEDED(InitD3D(hWnd))) {
    // Show the window
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
    // Enter the message loop
    MSG msg ;
    ZeroMemory(&msg, sizeof(msg));
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);
    // Get last time
    static DWORD lastTime = timeGetTime();
    while (msg.message != WM_QUIT) {
      if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0) {
        TranslateMessage(&msg) ;
        DispatchMessage(&msg) ;
      }
      else { // Render the game if there is no message to process
        // Get current time
        DWORD currTime = timeGetTime();
        // Calculate time elapsed
        float timeDelta = (currTime - lastTime) * 0.001f;
        // Render
        Render() ;
        // Update last time to current time for next loop
        lastTime = currTime;
      }
    }
  }
  UnregisterClass(winClass.lpszClassName, hInstance) ;
  return 0;
}

