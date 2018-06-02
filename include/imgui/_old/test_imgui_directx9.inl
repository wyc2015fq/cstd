#include "imgui_dx9.inl"
void ImGui_ImplDX9_RenderDrawLists(ImDrawData* draw_data)
{
  // Avoid rendering when minimized
  ImGuiIO* io = ImGui_GetIO();
  struct CUSTOMVERTEX* vtx_dst;
  ImDrawIdx* idx_dst;
  IDirect3DStateBlock9* d3d9_state_block = NULL;
  int n, i;
  //printf("VtxBuffer_Size = %d\n", draw_data->TotalVtxCount);
  if (io->DisplaySize.x <= 0.0f || io->DisplaySize.y <= 0.0f) {
    return;
  }
  // Create and grow buffers if needed
  if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount) {
    if (g_pVB) {
      IDirect3DDevice9_Release(g_pVB);
      g_pVB = NULL;
    }
    g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
    if (IDirect3DDevice9_CreateVertexBuffer(g_pd3dDevice, g_VertexBufferSize * sizeof(struct CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL) < 0) {
      return;
    }
  }
  if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount) {
    if (g_pIB) {
      IDirect3DDevice9_Release(g_pIB);
      g_pIB = NULL;
    }
    g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
    if (IDirect3DDevice9_CreateIndexBuffer(g_pd3dDevice, g_IndexBufferSize * sizeof(ImDrawIdx), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(ImDrawIdx) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &g_pIB, NULL) < 0) {
      return;
    }
  }
  // Backup the DX9 state
  if (IDirect3DDevice9_CreateStateBlock(g_pd3dDevice, D3DSBT_ALL, &d3d9_state_block) < 0) {
    return;
  }
  // Copy and convert all vertices into a single contiguous buffer
  if (IDirect3DVertexBuffer9_Lock(g_pVB, 0, (UINT)(draw_data->TotalVtxCount * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0) {
    return;
  }
  if (IDirect3DIndexBuffer9_Lock(g_pIB, 0, (UINT)(draw_data->TotalIdxCount * sizeof(ImDrawIdx)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0) {
    return;
  }
  for (n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;
    for (i = 0; i < cmd_list->VtxBuffer.Size; i++) {
      vtx_dst->pos[0] = vtx_src->pos.x;
      vtx_dst->pos[1] = vtx_src->pos.y;
      vtx_dst->pos[2] = 0.0f;
      vtx_dst->col = (vtx_src->col & 0xFF00FF00) | ((vtx_src->col & 0xFF0000) >> 16) | ((vtx_src->col & 0xFF) << 16); // RGBA --> ARGB for DirectX9
      vtx_dst->uv[0] = vtx_src->uv.x;
      vtx_dst->uv[1] = vtx_src->uv.y;
      vtx_dst++;
      vtx_src++;
    }
    memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
    idx_dst += cmd_list->IdxBuffer.Size;
  }
  IDirect3DVertexBuffer9_Unlock(g_pVB);
  IDirect3DIndexBuffer9_Unlock(g_pIB);
  IDirect3DDevice9_SetStreamSource(g_pd3dDevice, 0, g_pVB, 0, sizeof(CUSTOMVERTEX));
  IDirect3DDevice9_SetIndices(g_pd3dDevice, g_pIB);
  IDirect3DDevice9_SetFVF(g_pd3dDevice, D3DFVF_CUSTOMVERTEX);
  // Setup render state: fixed-pipeline, alpha-blending, no face culling, no depth testing
  IDirect3DDevice9_SetPixelShader(g_pd3dDevice, NULL);
  IDirect3DDevice9_SetVertexShader(g_pd3dDevice, NULL);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_CULLMODE, D3DCULL_NONE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_LIGHTING, FALSE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ZENABLE, FALSE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ALPHABLENDENABLE, TRUE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_ALPHATESTENABLE, FALSE);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_BLENDOP, D3DBLENDOP_ADD);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_SCISSORTESTENABLE, TRUE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  IDirect3DDevice9_SetTextureStageState(g_pd3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  IDirect3DDevice9_SetSamplerState(g_pd3dDevice, 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  IDirect3DDevice9_SetSamplerState(g_pd3dDevice, 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  // Setup orthographic projection matrix
  // Being agnostic of whether <d3dx9.h> or <DirectXMath.h> can be used, we aren't relying on D3DXMatrixIdentity()/D3DXMatrixOrthoOffCenterLH() or DirectX::XMMatrixIdentity()/DirectX::XMMatrixOrthographicOffCenterLH()
  {
    const float L = 0.5f, R = io->DisplaySize.x + 0.5f, T = 0.5f, B = io->DisplaySize.y + 0.5f;
    D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } };
    D3DMATRIX mat_projection = {
      2.0f / (R - L), 0.0f, 0.0f, 0.0f,
      0.0f, 2.0f / (T - B), 0.0f, 0.0f,
      0.0f, 0.0f, 0.5f, 0.0f,
      (L + R) / (L - R), (T + B) / (B - T), 0.5f, 1.0f,
    };
    IDirect3DDevice9_SetTransform(g_pd3dDevice, D3DTS_WORLD, &mat_identity);
    IDirect3DDevice9_SetTransform(g_pd3dDevice, D3DTS_VIEW, &mat_identity);
    IDirect3DDevice9_SetTransform(g_pd3dDevice, D3DTS_PROJECTION, &mat_projection);
  }
  // Render command lists
  {
    int vtx_offset = 0, idx_offset = 0, cmd_i;
    for (n = 0; n < draw_data->CmdListsCount; n++) {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];
      for (cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
        if (pcmd->UserCallback) {
          pcmd->UserCallback(cmd_list, pcmd);
        }
        else {
          const RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
          IDirect3DDevice9_SetTexture(g_pd3dDevice, 0, (IDirect3DBaseTexture9*)pcmd->TextureId);
          IDirect3DDevice9_SetScissorRect(g_pd3dDevice, &r);
          IDirect3DDevice9_DrawIndexedPrimitive(g_pd3dDevice, D3DPT_TRIANGLELIST, vtx_offset, 0, (UINT)cmd_list->VtxBuffer.Size, idx_offset, pcmd->ElemCount / 3);
        }
        idx_offset += pcmd->ElemCount;
      }
      vtx_offset += cmd_list->VtxBuffer.Size;
    }
  }
  // Restore the DX9 state
  IDirect3DStateBlock9_Apply(d3d9_state_block);
  IDirect3DStateBlock9_Release(d3d9_state_block);
}
//#include "cstd.h"
//#include "sys_win32.inl"
int test_imgui_directx9(int, char**)
{
  imgui_dx9_init("ImGui DirectX9 Example", 100, 100, 1280, 800);
  // Load Fonts
  // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
  //ImGuiIO* io = ImGui_GetIO();
  //io->Fonts->AddFontDefault();
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  //io->Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  //io->Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io->Fonts->GetGlyphRangesJapanese());
  bool show_main_window = true;
  bool show_test_window = true;
  bool show_another_window = false;
  ImVec4 clear_col = ImColor(114, 144, 154);
  // Main loop
  MSG msg;
  ZeroMemory(&msg, sizeof(msg));
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      continue;
    }
    ImGui_ImplDX9_NewFrame();
    // 1. Show a simple window
    // Tip: if we don't call ImGui_Begin()/ImGui_End() the widgets appears in a window automatically called "Debug"
    {
      static float f = 0.0f;
      ImGui_Begin("Main Window", &show_main_window, 0);
      ImGui_Text("Hello, world!");
      ImGui_SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui_ColorEdit3("clear color", (float*)&clear_col);
      if (ImGui_Button("Test Window")) {
        show_test_window ^= 1;
      }
      if (ImGui_Button("Test Window")) {
      }
      if (ImGui_Button("Test Window")) {
      }
      if (ImGui_Button("Test Window")) {
      }
      if (ImGui_Button("Test Window")) {
      }
      if (ImGui_Button("Test Window")) {
      }
      if (ImGui_Button("Test Window")) {
      }
      if (ImGui_Button("Another Window")) {
        show_another_window ^= 1;
      }
      ImGui_Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui_GetIO()->Framerate, ImGui_GetIO()->Framerate);
      ImGui_End();
    }
    printf("Application average %.3f ms/frame (%.1f FPS)\r", 1000.0f / ImGui_GetIO()->Framerate, ImGui_GetIO()->Framerate);
    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window) {
      ImGui_SetNextWindowSize(fVec2(200, 100), ImGuiSetCond_FirstUseEver);
      ImGui_Begin("Another Window", &show_another_window, 0);
      ImGui_Text("Hello");
      ImGui_End();
    }
    // 3. Show the ImGui test window. Most of the sample code is in ImGui_ShowTestWindow()
    if (show_test_window) {
      ImGui_SetNextWindowPos(fVec2(650, 20), ImGuiSetCond_FirstUseEver);
      ImGui_ShowTestWindow(&show_test_window);
    }
    // Rendering
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    IDirect3DDevice9_SetRenderState(g_pd3dDevice, D3DRS_SCISSORTESTENABLE, false);
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_col.x * 255.0f), (int)(clear_col.y * 255.0f), (int)(clear_col.z * 255.0f), (int)(clear_col.w * 255.0f));
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
    if (g_pd3dDevice->BeginScene() >= 0) {
      ImGui_Render();
      g_pd3dDevice->EndScene();
    }
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
  }
  imgui_dx9_uninit();
  return 0;
}
#include "cstd.h"
#include "img/imgio.inl"
#include "sys_win32.inl"

