#ifndef LAMPSHADE_H
#define LAMPSHADE_H

#include <GL/gl.h>

#include "ModelView.h"
#include "ShaderIF.h"

class LampShade : public ModelView
{
public:
  LampShade(ShaderIF* sIF, cryph::AffPoint botIn, double radiusBotIn, double radiusTopIn, double heightIn, float colorIn[]);
  virtual ~LampShade();

  void getMCBoundingBox(double* xyzLimits) const;
  void render();

private:
  ShaderIF* shaderIF;
  GLuint vao[1];
  GLuint vbo[2]; //0: coordinates; 1: normal vectors
  float ka[3], kd[3], xyz[6];
  cryph::AffPoint m_bot, m_top;
  double radiusBot, radiusTop, height;

  void defineLampShade();
};

#endif
