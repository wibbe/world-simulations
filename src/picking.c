
#include "game.h"

Vec3 mouse_pick()
{
  int mouse_x, mouse_y, width, height;
  glfwGetMousePos(&mouse_x, &mouse_y);
  glfwGetWindowSize(&width, &height);

  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  glGetIntegerv(GL_VIEWPORT, viewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  
  // obtain the Z position (not world coordinates but in range 0 - 1)
  GLfloat z_cursor;
  glReadPixels(mouse_x, height - mouse_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);
  
  // obtain the world coordinates
  GLdouble x, y, z;
  gluUnProject(mouse_x, height - mouse_y, z_cursor, modelview, projection, viewport, &x, &y, &z);

  return vec3(x, y, z);
}

