#include <math.h>
#include "LampShade.h"

LampShade::LampShade(ShaderIF* sIF, cryph::AffPoint botIn, double radiusBotIn, double radiusTopIn, double heightIn, float colorIn[]) : shaderIF(sIF)
{
  radiusBot = radiusBotIn;
  radiusTop = radiusTopIn;
  height = heightIn;
  m_bot = botIn; //center of bottom of LampShade
  m_top = cryph::AffPoint(m_bot.x, m_bot.y, m_bot.z+height); //center of top of LampShade
  xyz[0] = m_bot.x - radiusBot; xyz[1] = m_bot.x - radiusBot; //x limits
  xyz[2] = m_bot.y - radiusBot; xyz[3] = m_bot.y - radiusBot; //y limits
  xyz[4] = m_bot.z; xyz[5] = m_top.z //z limits MIGHT HAVE TO SWAP Y & Z
  for (int i=0 ; i<3 ; i++)
  {
    ka[i] = colorIn[i];
    kd[i] = colorIn[i];
  }
  defineLampShade();
}

LampShade::~LampShade()
{
  glDeleteBuffers(2, vbo);
  glDeleteVertexArrays(1, vao);
}

const int N_POINTS_AROUND_SLICE = 18; // number of points around a cross-section

void LampShade::defineLampShade()
{
  typedef float vec3[3];
  int nPoints = 2*(N_POINTS_AROUND_SLICE + 1) // +1 because last point = first
  vec3* coords = new vec3[nPoints];
  vec3* normals = new vec3[nPoints];
  double theta = 0.0;
  double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;
  cryph::AffVector U(1, 0, 0);
  cryph::AffVector V(0, 1, 0);
  for (int i=0 ; i<=N_POINTS_AROUND_SLICE ; i++)
	{
    cryph::AffPoint bot = m_bot + radiusBot * (cos(theta)*U + sin(theta)*V);
    cryph::AffPoint top = m_top + radiusTop * (cos(theta)*U + sin(theta)*V);
		theta += dTheta;
    int j = i*2;

    bot.aCoords(coords, j);
    top.aCoords(coords. j+1);

    cryph::AffVector normal = bot - m_bot;
    normal.normalize();
    normals[j][0] = normal.dx; normals[j][1] = normal.dy; normals[j][2] = normal.dz;
    normals[j+1][0] = normal.dx; normals[j+1][1] = normal.dy; normals[j+1][2] = normal.dz;
	}

  // ************************************************************************
  // EXERCISE: Create/fill VAOs and VBOs here.
  //           Also use glVertexAttribPointer and glEnableVertexAttribArray
  // ************************************************************************
  glGenVertexArrays(1, vao);
  glBindVertexArray(vao[0]);

  glGenBuffers(2, vbo);

  //vbo#1 coords
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), coords, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(sIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0 ,0);
  glEnableVertexAttribArray(sIF->pvaLoc("mcNormal"));

  //vbo#2 normals
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), normals, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(sIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0 ,0);
  glEnableVertexAttribArray(sIF->pvaLoc("mcNormal"));

  delete[] coords;
  delete[] normals;
}

void LampShade::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void LampShade::render()
{
  typedef float vec3[3];
  GLint pgm;
  glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
  glUseProgram(shaderIF->getShaderPgmID());

  cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

  glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);
  glBindVertexArray(vao[0]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));

  glUseProgram(pgm);
}
