#define M_PI2 (2.0 * M_PI) // PI Squared
void Torus(float MinorRadius, float MajorRadius) // Draw A Torus With Normals
{
  int i, j;
  int n = 30, m = 30;
  glBegin(GL_TRIANGLE_STRIP); // Start A Triangle Strip
  for (i = 0; i < n; i++) { // Stacks
    int i1 = (i + 1) % n;
    for (j = -1; j < m; j++) { // Slices
      float wrapFrac = (j % m) / (float)m;
      float phi = M_PI2 * wrapFrac;
      float sinphi = sinf(phi);
      float cosphi = cosf(phi);
      float r = MajorRadius + MinorRadius * cosphi;
      glNormal3f(sinf(M_PI2 * (i + wrapFrac) / (float)n)*cosphi, sinphi, cosf(M_PI2 * (i + wrapFrac) / (float)n)*cosphi);
      glVertex3f(sinf(M_PI2 * (i + wrapFrac) / (float)n)*r, MinorRadius * sinphi, cosf(M_PI2 * (i + wrapFrac) / (float)n)*r);
      glNormal3f(sinf(M_PI2 * (i1 + wrapFrac) / (float)n)*cosphi, sinphi, cosf(M_PI2 * (i1 + wrapFrac) / (float)n)*cosphi);
      glVertex3f(sinf(M_PI2 * (i1 + wrapFrac) / (float)n)*r, MinorRadius * sinphi, cosf(M_PI2 * (i1 + wrapFrac) / (float)n)*r);
    }
  }
  glEnd(); // Done Torus
}
void glCube(float cx, float cy, float cz)
{
  //画立方体的6个面
  float x = cx * 0.5f;
  float y = cy * 0.5f;
  float z = cz * 0.5f;
#if 1
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(x, y, z);
  glVertex3f(-x, y, z);
  glVertex3f(-x, -y, z);
  glVertex3f(x, -y, z);
  //1----------------------------
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(-x, -y, -z);
  glVertex3f(-x, y, -z);
  glVertex3f(x, y, -z);
  glVertex3f(x, -y, -z);
  //2----------------------------
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(x, y, z);
  glVertex3f(x, y, -z);
  glVertex3f(-x, y, -z);
  glVertex3f(-x, y, z);
  //3----------------------------
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(-x, -y, -z);
  glVertex3f(x, -y, -z);
  glVertex3f(x, -y, z);
  glVertex3f(-x, -y, z);
  //4----------------------------
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f(x, y, z);
  glVertex3f(x, -y, z);
  glVertex3f(x, -y, -z);
  glVertex3f(x, y, -z);
  //5----------------------------
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-x, -y, -z);
  glVertex3f(-x, -y, z);
  glVertex3f(-x, y, z);
  glVertex3f(-x, y, -z);
  glEnd();
#else
  glBegin(GL_QUADS);
  glNormal3f(0.0F, 0.0F, 1.0F);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  //1----------------------------
  glNormal3f(0.0F, 0.0F, -1.0F);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  //2----------------------------
  glNormal3f(0.0F, 1.0F, 0.0F);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  //3----------------------------
  glNormal3f(0.0F, -1.0F, 0.0F);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  //4----------------------------
  glNormal3f(1.0F, 0.0F, 0.0F);
  glVertex3f(0.5f, 0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, 0.5f);
  glVertex3f(0.5f, -0.5f, -0.5f);
  glVertex3f(0.5f, 0.5f, -0.5f);
  //5----------------------------
  glNormal3f(-1.0F, 0.0F, 0.0F);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  //6----------------------------*/
  glEnd();
#endif
}
int test_draw(sys_event_t* event)
{
  if (MSG_SIZE == event->msg) {
    glViewport(0, 0, event->w, event->h);
  }
  if (0) {
    float v[] = {
      -0.5, -0.5, 0.0,
      0.5, 0.0, 0.0,
      0.0, 0.5, 0.0
    };
    int ind[] = {0, 1, 2};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, v);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_INT, ind);
  }
  if (0) {
    static int inited = 0;
    if (1) {
      inited = 1;
      // select clearing color: blue
      glClearColor(0.0, 1.0, 0.0, 0.0);
      // initialize viewing values
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    }
    {
      // clear all pixels
      // draw yellow polygon (rectangle) with corners at
      float z = -1;
      glColor3f(1.0, 1.0, 0.0);
      glBegin(GL_POLYGON);
      glVertex3f(0.20, 0.20, 0.0);
      glVertex3f(0.80, 0.20, 0.0);
      glVertex3f(0.80, 0.80, 0.0);
      glVertex3f(0.20, 0.80, 0.0);
      glEnd();
      glColor3f(1.0, 0.0, 0.0);
      glBegin(GL_POLYGON);
      glVertex3f(0.30, 0.30, z);
      glVertex3f(0.70, 0.30, z);
      glVertex3f(0.70, 0.70, z);
      glVertex3f(0.30, 0.70, z);
      glEnd();
    }
  }
  if (0) {
    float angle = 0;
    int iPivot = 0;
    glBegin(GL_TRIANGLE_FAN);
    // 三角形扇的共享顶点，z轴中心点上方
    glVertex3f(0.0f, 0.0f, 0.75);
    for (angle = 0.0f; angle < (2.0f * M_PI + M_PI / 8.0f); angle += (M_PI / 8.0f)) {
      // 计算下一个顶点的位置
      double x = 0.50f * sin(angle);
      double y = 0.50f * cos(angle);
      if ((iPivot % 2) == 0) {
        glColor3f(1.0f, 1.0f, 0.0f);
      }
      else {
        glColor3f(0.0f, 1.0f, 1.0f);
      }
      // 增加基准值，下次改变颜色
      ++iPivot;
      // 指定三角形扇的下一个顶点
      glVertex2f(x, y);
    }
    glEnd();
  }
  if (0) {
    static GLfloat colors1[] = {
      1, 0, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 0, 1
    };
    gcobj_t o[1] = {0};
    int glmode;
    //cone(o, 1, 1, 5, 1);
    //cube(o, 1, 1, 1, NULL);
    cube1(o);
    //gcobj_printf(o);
    glVertexPointer(3, GL_FLOAT, sizeof(*o->vertex), &o->vertex->pos);
    glNormalPointer(GL_FLOAT, sizeof(*o->vertex), &o->vertex->n);
    //glColorPointer(3, GL_FLOAT, 0, colors1);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(*o->vertex), &o->vertex->col);
    glmode = mode2glmode(o->mode);
    glDrawArrays(glmode, 0, o->index_num);
    //glDrawElements(glmode, o->index_num, GL_SHORT, o->index);
    gcobj_free(o);
  }
  if (0) {
    //设置视图投影
    glMatrixMode(GL_PROJECTION);
    glFrustum(-1, 1, -1, 1, 1.0F, 8.0F);
    //视区定位
    //glMatrixMode(GL_MODELVIEW);
    //glTranslatef(0.0F, 0.0F, -4.0F);
    //物体定位
    //glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    //glRotatef(30.0F, 0.0F, 1.0F, 0.0F);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_INDEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    //glEnable(GL_COLOR_ARRAY);
    //glClearColor(1, 0, 0, 0);
    //glColor4f(1, 1, 0, 0);
  }
  if (1) {
    //清除颜色和深度缓冲区
    //glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    //glClearDepth(2.0);
  }
  if (0) {
    static GLfloat vertexs[] = {
      -0.5, -0.5, 0.0,
      0.5, 0.0, 0.0,
      0.0, 0.5, 0.0
    };
    static GLfloat colors[] = {
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
    };
    GLubyte array[] = {0, 1, 2};
    //绑定顶点数组
    glVertexPointer(3, GL_FLOAT, 0, vertexs);
    glColorPointer(3, GL_FLOAT, 0, colors);
    //解引用 和 绘制
    //type 必须是 无符号整形： GL_BYTE -- > gl
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, array);
  }
  if (0) {
    static GLfloat vertexs[] = {
      -0.5, -0.5, 0.0,
      0.5, 0.0, 0.0,
      0.0, 0.5, 0.0
    };
    static GLfloat colors[] = {
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
    };
    static short array[] = {0, 1, 2};
    glVertexPointer(3, GL_FLOAT, 0, vertexs);
    glColorPointer(3, GL_FLOAT, 0, colors);
    //解引用 和 绘制
    //type 必须是 无符号整形： GL_BYTE -- > gl
    glDrawElements(GL_TRIANGLES, 3, GL_SHORT, array);
  }
  if (0) {
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
  }
#if 0
  if (0) {
    static GLuint VBO = 0;
    static int inited = 0;
    if (!inited) {
      inited = 1;
      float Vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
      };
      GLenum res = glewInit();
      if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return ;
      }
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    }
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
  }
#endif
  return 0;
}
//绘制回调函数
void display1(void)
{
  //利用顶点数组，绘制三角形
  // 顶点
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f
  };
  glClear(GL_COLOR_BUFFER_BIT);
  // 启用vertex arrays
  glEnableClientState(GL_VERTEX_ARRAY);
  //定义顶点数组
  glVertexPointer(
      3, // 每个顶点的维度
      GL_FLOAT, // 顶点数据类型
      0, // 连续顶点之间的间隙，这里为0
      vertices //指向第一个顶点的第一个坐标的指针
  );
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);
}
//用 glArrayElement方式绘制
void useGlArrayElement()
{
  static GLfloat vertexs[] = {
    -1, -1, 0,
    -0.5, -0.5, 0,
    -1, -0.5, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //绑定顶点数组
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //解引用 和 绘制
  glBegin(GL_TRIANGLES);
  glArrayElement(1);
  glArrayElement(0);
  glArrayElement(2);
  glEnd();
}
void useGlDrawElements()
{
  static GLfloat vertexs[] = {
    -0.4, -1, 0,
    -0.2, -1, 0,
    -0.2, -0.5, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  GLubyte array[] = {0, 1, 2};
  //绑定顶点数组
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //解引用 和 绘制
  //type 必须是 无符号整形： GL_BYTE -- > gl
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, array);
}
//glDraw
void useGlDrawArrays()
{
  static GLfloat vertexs[] = {
    0, 0, 0,
    0, 0.5, 0,
    0.3, 0.4, 0
  };
  static GLfloat colors[] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  };
  //绑定顶点数组
  glVertexPointer(3, GL_FLOAT, 0, vertexs);
  glColorPointer(3, GL_FLOAT, 0, colors);
  //gldraw
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
void display()
{
  //glClearColor(0,0,0,0);
  //开启顶点，颜色数组
  //useGlArrayElement();
  useGlDrawElements();
  //useGlDrawArrays();
}

