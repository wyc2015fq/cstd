void camera_at_zero(info3d_t* device, float x, float y, float z)
{
  point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
  matrix_set_lookat(&device->transform.view, &eye, &at, &up);
  transform_update(&device->transform);
}
int test_draw3d_ctrl(ctrl_t* c)
{
  SYSIO;
  IRECT rc = c->rc;
  int w = RCW(&rc), h = RCH(&rc);
  window_host_t* host = io->cur_host;
  render_drv_t* drv = host->drv;
  static info3d_t info[1] = {0};
  static ImVertex vtx[8] = {
    { { 1, -1, 1, 1 }, { 0, 0 }, _rgb(1.0f, 0.2f, 0.2f), 1 },
    { { -1, -1, 1, 1 }, { 0, 1 }, _rgb(0.2f, 1.0f, 0.2f), 1 },
    { { -1, 1, 1, 1 }, { 1, 1 }, _rgb(0.2f, 0.2f, 1.0f), 1 },
    { { 1, 1, 1, 1 }, { 1, 0 }, _rgb(1.0f, 0.2f, 1.0f), 1 },
    { { 1, -1, -1, 1 }, { 0, 0 }, _rgb(1.0f, 1.0f, 0.2f), 1 },
    { { -1, -1, -1, 1 }, { 0, 1 }, _rgb(0.2f, 1.0f, 1.0f), 1 },
    { { -1, 1, -1, 1 }, { 1, 1 }, _rgb(1.0f, 0.3f, 0.3f), 1 },
    { { 1, 1, -1, 1 }, { 1, 0 }, _rgb(0.2f, 1.0f, 0.3f), 1 },
  };
  static uint16 idx[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 5, 1,
    1, 5, 6, 2,
    2, 6, 7, 3,
    3, 7, 4, 0,
  };
  static int inited = 0;
  static int indicator = 0;
  static float alpha = 1;
  static float pos = 3.5;
  static texture_t tex[1] = {0};
  int states[] = { GC_TEXTURE, GC_COLOR, GC_WIREFRAME };
  if (!inited) {
    inited = 1;
    info->type = GC_QUADS;
    info->state = GC_COLOR;
    info->tex = tex;
    {
      int i, n = 4 * 6, nvtx = 0;
      ImVertex* p = (ImVertex*)myAllocPush(info->vtx, nvtx, n);
      uint16* id = (uint16*)myAllocPush(info->idx, info->n, n);
      uint16* idx0 = idx;
      for (i = 0; i < n; i += 4, p += 4, idx0 += 4) {
        p[0] = vtx[idx0[0]], p[1] = vtx[idx0[1]], p[2] = vtx[idx0[2]], p[3] = vtx[idx0[3]];
        p[0].uv.x = 0, p[0].uv.y = 0, p[1].uv.x = 0, p[1].uv.y = 1;
        p[2].uv.x = 1, p[2].uv.y = 1, p[3].uv.x = 1, p[3].uv.y = 0;
      }
      for (i = 0; i < n; i++) {
        id[i] = i;
      }
      info->n = n;
    }
    transform_init(&info->transform);
    {
      static uint32 texture[256 * 256];
      int i, j;
      for (j = 0; j < 256; j++) {
        for (i = 0; i < 256; i++) {
          int x = i / 32, y = j / 32;
          texture[j * 256 + i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
        }
      }
      drv->SetTexture(drv, tex, 0, 256, 256, texture, 256 * 4, 32);
    }
  }
  transform_set_projection(&info->transform, w, h);
  camera_at_zero(info, pos, 0, 0);
  if (ISHOVEREDWIN2()) {
    int hot = iPtInRect(&rc, x, y);
    if (io->keydown[KEY_UP]) {
      pos -= 0.01f;
    }
    if (io->keydown[KEY_DOWN]) {
      pos += 0.01f;
    }
    if (io->keydown[KEY_LEFT]) {
      alpha += 0.01f;
    }
    if (io->keydown[KEY_RIGHT]) {
      alpha -= 0.01f;
    }
    switch (io->msg) {
    case MSG_MOUSEMOVE:
      if (hot) {
      }
      break;
    case MSG_KEYDOWN:
      switch (io->last_key) {
      case ' ':
        indicator = (++indicator) % countof(states);
        break;
      }
      break;
    }
  }
  {
    matrix_t m;
    matrix_set_rotate(&m, -1, -0.5, 1, alpha);
    info->transform.world = m;
    transform_update(&info->transform);
  }
  info->state = states[indicator];
  gcPrimitive(g, info);
  //gcPrimitive();
  return 0;
}

