#ifndef PCSCREEN_H
#define PCSCREEN_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"

class PCScreen : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	PCScreen(ShaderIF* sIF, double basex, double basey, double basez, double height, double length, float colorIn[]);
	virtual ~PCScreen();
  void definePCScreen();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void render();
private:
	ShaderIF* shaderIF;

  Block* pcBase;
  Block* pcStand;
  Block* fillerBlock;
  Block* pcScreen;

  cryph::AffPoint base;
  double m_height, m_length;

	float ka[3], kd[3];
};

#endif
