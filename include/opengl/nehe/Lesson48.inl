#include "ArcBall.inl"
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glaux.lib" )
// Start Of User Initialization
// Any User DeInitialization Goes Here
//gluDeleteQuadric(quadratic);
int Lesson48(sys_event_t* event)
{
  static Matrix4fT Transform = { 1.0f, 0.0f, 0.0f, 0.0f, // NEW: Final Transform
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f
                               };
  static Matrix3fT LastRot = { 1.0f, 0.0f, 0.0f, // NEW: Last Rotation
                   0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 1.0f
                             };
  static Matrix3fT ThisRot = { 1.0f, 0.0f, 0.0f, // NEW: This Rotation
                   0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 1.0f
                             };
  static ArcBallT ArcBall[1];
  Point2fT MousePt; // NEW: Current Mouse Point
  static bool isClicked = false; // NEW: Clicking The Mouse?
  static int inited = 0;
  static GLUquadricObj* quadratic = NULL; // Used For Our Quadric
  // Any GL Init Code & User Initialiazation Goes Here
  int update = 0;
  if (!inited) {
    inited = 1;
    ArcBall_init(ArcBall, event->w, event->h); // NEW: ArcBall Instance
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
    glClearDepth(1.0f); // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing (Less Or Equal)
    glEnable(GL_DEPTH_TEST); // Enable Depth Testing
    glShadeModel(GL_FLAT); // Select Flat Shading (Nice Definition Of Objects)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
    quadratic = gluNewQuadric(); // Create A Pointer To The Quadric Object
    gluQuadricNormals(quadratic, GLU_SMOOTH); // Create Smooth Normals
    gluQuadricTexture(quadratic, GL_TRUE); // Create Texture Coords
    glEnable(GL_LIGHT0); // Enable Default Light
    glEnable(GL_LIGHTING); // Enable Lighting
    glEnable(GL_COLOR_MATERIAL); // Enable Color Material
  }
  MousePt.s.X = (GLfloat)event->x;
  MousePt.s.Y = (GLfloat)event->y;
  switch (event->msg) {
  case MSG_MOUSEMOVE:
    if (isClicked) {
      Quat4fT ThisQuat;
      ArcBall_drag(ArcBall, &MousePt, &ThisQuat); // Update End Vector And Get Rotation As Quaternion
      Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat); // Convert Quaternion Into Matrix3fT
      Matrix3fMulMatrix3f(&ThisRot, &LastRot); // Accumulate Last Rotation Into This One
      Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot); // Set Our Final Transform's Rotation From This One
    }
    break;
  case MSG_LBUTTONUP:
    isClicked = false;
    break;
  case MSG_LBUTTONDOWN:
    isClicked = true;
    LastRot = ThisRot; // Set Last Static Rotation To Last Dynamic One
    ArcBall_click(ArcBall, &MousePt); // Update Start Vector And Prepare For Dragging
    break;
  case MSG_RBUTTONDOWN:
    // If Right Mouse Clicked, Reset All Rotations
    Matrix3fSetIdentity(&LastRot); // Reset Rotation
    Matrix3fSetIdentity(&ThisRot); // Reset Rotation
    Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot); // Reset Rotation
    break;
  case MSG_SIZE: {
    int width = event->w, height = event->h;
    glViewport(0, 0, (GLsizei)(width), (GLsizei)(height)); // Reset The Current Viewport
    glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
    glLoadIdentity(); // Reset The Projection Matrix
    gluPerspective(45.0f, (GLfloat)(width) / (GLfloat)(height), 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix
    ArcBall_setBounds(ArcBall, (GLfloat)width, (GLfloat)height); //*NEW* Update mouse bounds for arcball
  }
  break;
  }
  if (1) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
    glLoadIdentity(); // Reset The Current Modelview Matrix
    glTranslatef(-1.5f, 0.0f, -6.0f); // Move Left 1.5 Units And Into The Screen 6.0
    glPushMatrix(); // NEW: Prepare Dynamic Transform
    glMultMatrixf(Transform.M); // NEW: Apply Dynamic Transform
    glColor3f(0.75f, 0.75f, 1.0f);
    Torus(0.30f, 1.00f);
    glPopMatrix(); // NEW: Unapply Dynamic Transform
    glLoadIdentity(); // Reset The Current Modelview Matrix
    glTranslatef(1.5f, 0.0f, -6.0f); // Move Right 1.5 Units And Into The Screen 7.0
    glPushMatrix(); // NEW: Prepare Dynamic Transform
    glMultMatrixf(Transform.M); // NEW: Apply Dynamic Transform
    glColor3f(1.0f, 0.75f, 0.75f);
    //gluSphere(quadratic, 1.3f, 20, 20);
    glCube(1, 2, 3);
    glPopMatrix(); // NEW: Unapply Dynamic Transform
    glFlush(); // Flush The GL Rendering Pipeline
  }
  return 0;
}

