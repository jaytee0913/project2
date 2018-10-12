// main.c++: Starter for EECS 672 Projects 2-4
//https://people.eecs.ku.edu/~jrmiller/cryph/cryphDocumentation/annotated.html

#include "ExtendedController.h"
#include "Block.h"
#include "PCScreen.h"
#include "Desk.h"
#include "Cylinder.h"
#include "LampShade.h"

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	// TODO: Implement this function
	float grass[] = {0.486, 0.988, 0};
	//c.addModel(new Block(sIF, 0.0, 0.0, 0.0, 20, 20, 20, grass));
	c.addModel(new Desk(sIF, 0.0, 0.0, 0.0, 15.0, 20.0, grass));
}

void set3DViewingInformation(double overallBB[])
{
	// IF we are using Viewing Strategy #1, THEN
	//     Notify class ModelView that we initially want to see the entire scene:
	   ModelView::setMCRegionOfInterest(overallBB);
	// ELSE (Viewing Strategy #2)
	//     Modify all three of deltaX, deltaY, and deltaZ of the incoming overallBB
	//     to have the dimensions of the desired field of view, then invoke:
	//   ModelView::setMCRegionOfInterest(modified_overallBB);
	//   Tell the ModelView class that dynamic rotations are to be done about the eye.

	// MC -> EC:

	// TODO: Compute/set eye, center, up
	//done: might need changes to eye

	//find maxDelta
	double maxDelta = overallBB[1] - overallBB[0];
	double delta = overallBB[3] - overallBB[1];
	if(delta > maxDelta)
	{
		maxDelta = delta;
	}
	delta = overallBB[5] - overallBB[4];
	if(delta > maxDelta)
	{
		maxDelta = delta;
	}

	//find xmid, ymid, zmid for center
	double xmid = 0.5 * (overallBB[1] - overallBB[0]);
	double ymid = 0.5 * (overallBB[3] - overallBB[2]);
	double zmid = 0.5 * (overallBB[5] - overallBB[4]);
	cryph::AffPoint center(xmid, ymid, zmid);

	//find eye
	double distEyeCenter = 2 * maxDelta;
	cryph::AffVector dir(1, 1, -1); //check/change later?????
	dir.normalize();
	cryph::AffPoint eye = center + distEyeCenter*(dir);
	//cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);

	//find up
	cryph::AffVector up = cryph::AffVector::yu;

	ModelView::setEyeCenterUp(eye, center, up);

	// EC -> LDS:

	// Specify the initial projection type desired
	ModelView::setProjection(PERSPECTIVE);

	// TODO: Compute/set ecZmin, ecZmax (It is often useful to exaggerate
	//       these limits somewhat to prevent unwanted depth clipping.)
	double ecZmin, ecZmax;
	double ecZpp = -(distEyeCenter - 0.5*maxDelta);
	ecZmin = ecZpp - 1.5*maxDelta; // maybe change numbers????
	ecZmax = ecZpp + 0.5*maxDelta; //maybe change numbers??
	ModelView::setECZminZmax(ecZmin, ecZmax);

	// TODO: Set ecZpp
	//double ecZpp;
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[])
{
	ExtendedController c("GIVE ME A NICE TITLE", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	createScene(c, sIF);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	std::cout << "Bounding box: " << xyz[0] << " <= x <= " << xyz[1] << '\n';
	std::cout << "              " << xyz[2] << " <= y <= " << xyz[3] << '\n';
	std::cout << "              " << xyz[4] << " <= z <= " << xyz[5] << "\n\n";
	set3DViewingInformation(xyz);

	c.run();

	delete sIF;

	return 0;
}
