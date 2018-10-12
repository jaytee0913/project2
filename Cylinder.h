#ifndef CYLINDER_H
#define CYLINDER_H

#include <GL/gl.h>

#include "ModelView.h"
#include "ShaderIF.h"

class Cylinder : public ModelView
{
public:
  Cylinder(ShaderIF* sIF, cryph::AffPoint botIn, double radiusIn, double heightIn, float colorIn[]);
  virtual ~Cylinder();

  void getMCBoundingBox(double* xyzLimits) const;
  void render();

private:
  ShaderIF* shaderIF;
  GLuint vao[1];
  GLuint vbo[2]; //0: coordinates; 1: normal vectors
  float ka[3], kd[3], xyz[6];
  cryph::AffPoint m_bot, m_top;
  double radius, height;

  void defineCylinder();
};

#endif
