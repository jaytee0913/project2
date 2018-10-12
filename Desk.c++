#include "Desk.h"

Desk::Desk(ShaderIF* sIF, double deskx, double desky, double deskz, double height, double length, float colorIn[]) : shaderIF(sIF)
{
  startingPoint = cryph::AffPoint(deskx, desky, deskz);
  m_height = height;
  m_length = length;
  for (int i=0 ; i<3 ; i++)
  {
    ka[i] = colorIn[i];
    kd[i] = colorIn[i];
  }
  defineDesk();
}

Desk::~Desk()
{
  delete deskTop;
  delete deskLegLeft;
  delete deskLegRight;
  delete deskBaseLeft;
  delete deskBaseRight;
}

void Desk::defineDesk()
{
  float deskTopColor[3] = {0.25, 0.41, 0.88}; //royal blue
  float baseColor[3] = {0, 0, 0}; //black

  double deskTopLength = m_length;
  double deskTopHeight = m_height/25;
  double deskTopDepth = m_length*0.75;
  double legLength = m_length/30;
  double legHeight = m_height-deskTopHeight*3;
  double legDepth = legLength;
  double baseLength = legLength;
  double baseHeight = deskTopHeight*2;
  double baseDepth = 0.8*deskTopDepth;

  deskTop = new Block(shaderIF, startingPoint.x, startingPoint.y, startingPoint.z, deskTopLength, -(deskTopHeight), deskTopDepth, deskTopColor);
  deskLegLeft = new Block(shaderIF, startingPoint.x, startingPoint.y-m_height+baseHeight, startingPoint.z+(deskTopDepth-legDepth), legLength, legHeight, legDepth, baseColor);
  deskLegRight = new Block(shaderIF, startingPoint.x+(m_length-legLength), startingPoint.y-m_height+baseHeight, startingPoint.z+(deskTopDepth-legDepth), legLength, legHeight, legDepth, baseColor);
  deskBaseLeft = new Block(shaderIF, startingPoint.x, startingPoint.y-m_height, startingPoint.z+(deskTopDepth-baseDepth), baseLength, baseHeight, baseDepth, baseColor);
  deskBaseRight = new Block(shaderIF, startingPoint.x+m_length, startingPoint.y-m_height, startingPoint.z+(deskTopDepth-baseDepth), -(baseLength), baseHeight, baseDepth, baseColor);
}

void Desk::getMCBoundingBox(double* xyzLimits) const
{
  double* temp = new double[6];
  double* temp2 = new double[6];
  deskTop->getMCBoundingBox(temp);
  deskBaseLeft->getMCBoundingBox(temp2);
  xyzLimits[0] = temp[0]; xyzLimits[1] = temp[1]; //x
  xyzLimits[2] = temp[2]; xyzLimits[3] = temp[3]; //y
  xyzLimits[4] = temp[4]; xyzLimits[5] = temp[5]; //z

  delete[] temp;
  delete[] temp2;
}

void Desk::render()
{
  deskTop->render();
  deskLegLeft->render();
  deskLegRight->render();
  deskBaseLeft->render();
  deskBaseRight->render();
}
