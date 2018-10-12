#include "PCScreen.h"

PCScreen::PCScreen(ShaderIF* sIF, double basex, double basey, double basez, double height, double length, float colorIn[]) : shaderIF(sIF)
{
  base = cryph::AffPoint(basex, basey, basez);
  m_height = height;
  m_length = length;
  for (int i=0 ; i<3 ; i++)
  {
    ka[i] = colorIn[i];
    kd[i] = colorIn[i];
  }
  definePCScreen();
}

PCScreen::~PCScreen()
{
  delete pcBase;
  delete pcStand;
  delete fillerBlock;
  delete pcScreen;
}

void PCScreen::definePCScreen()
{
  float baseColor[3] = {0, 0, 0}; //black
  float standColor[3] = {0.75, 0.75, 0.75}; // silver
  float screenColor[3] = {0, 0, 0}; // black

  double baseLength = 0.5*m_length;
  double baseHeight = 0.02*m_height;
  double baseDepth = 0.8*baseLength;
  double standLength = baseLength/6;
  double standHeight = 0.6*m_height;
  double standDepth = baseHeight;
  double screenLength = m_length;
  double screenHeight = 0.75*m_height;
  double screenDepth = 2*baseHeight;
  double fillerLength = standLength;
  double fillerHeight = standHeight/6;
  double fillerDepth = 0.7*baseDepth - screenDepth;

  pcBase = new Block(shaderIF, base.x, base.y, base.z, baseLength, baseHeight, baseDepth, baseColor);
  pcStand = new Block(shaderIF, base.x+(1/3)*baseLength, base.y+baseHeight, base.z+0.7*baseDepth, standLength, standHeight, standDepth, standColor);
  fillerBlock = new Block(shaderIF, base.x+(1/3)*baseLength, base.y+baseHeight+standHeight-(0.8*fillerLength), base.z+screenDepth, fillerLength, fillerHeight, fillerDepth, standColor);
  pcScreen = new Block(shaderIF, base.x-0.5*baseLength, base.y+0.25*m_height, base.z, screenLength, screenHeight, screenDepth, screenColor);
}

void PCScreen::getMCBoundingBox(double* xyzLimits) const
{
    double* temp = new double[6];
    double* temp2 = new double[6];
    pcBase->getMCBoundingBox(temp);
    pcScreen->getMCBoundingBox(temp2);
    xyzLimits[0] = temp2[0]; xyzLimits[1] = temp2[1]; //x
    xyzLimits[2] = temp[2]; xyzLimits[3] = temp2[3]; //y
    xyzLimits[4] = temp[4]; xyzLimits[5] = temp[5]; //z

    delete[] temp;
    delete[] temp2;
}

void PCScreen::render()
{
  pcBase->render();
  pcStand->render();
  fillerBlock->render();
  pcScreen->render();
}
