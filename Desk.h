#ifndef DESK_H
#define DESK_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"

class Desk : public ModelView
{
public:
  Desk(ShaderIF* sIF, double deskx, double desky, double deskz, double height, double length, float colorIn[]);
  virtual ~Desk();
  void defineDesk();
  void getMCBoundingBox(double* xyzLimits) const;
	void render();

private:
  ShaderIF* shaderIF;

  Block* deskTop;
  Block* deskLegLeft;
  Block* deskLegRight;
  Block* deskBaseLeft;
  Block* deskBaseRight;

  cryph::AffPoint startingPoint;
  double m_height, m_length;

  float ka[3], kd[3];
};

#endif
