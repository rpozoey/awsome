#include <windows.h>
#include <osg/Node>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/Billboard>
#include <osg/LineWidth>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>


// A simple demo demonstrating different texturing modes,
// Describe a coordinate system

// make a new name to simplify
typedef std::vector< osg::ref_ptr<osg::Image> > ImageList;

// create quad at specified position.
osg::Drawable* createSquare(const osg::Vec3& corner, const osg::Vec3& width, const osg::Vec3& height, osg::ref_ptr<osg::Image> image)
{
	// set up the Geometry.
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	//
	osg::Vec3Array* coords = new osg::Vec3Array(4);
	(*coords)[0] = corner;
	(*coords)[1] = corner + width;
	(*coords)[2] = corner + width + height;
	(*coords)[3] = corner + height;

	//set vertex array
	geom->setVertexArray(coords);

	osg::Vec3Array* norms = new osg::Vec3Array(1);
	(*norms)[0] = width ^ height;
	(*norms)[0].normalize();

	geom->setNormalArray(norms, osg::Array::BIND_OVERALL);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[0].set(0.0f, 0.0f);
	(*tcoords)[1].set(1.0f, 0.0f);
	(*tcoords)[2].set(1.0f, 1.0f);
	(*tcoords)[3].set(0.0f, 1.0f);
	geom->setTexCoordArray(0, tcoords);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	if (image)
	{
		osg::StateSet* stateset = new osg::StateSet;
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
		geom->setStateSet(stateset);
	}

	return geom.release();
}

osg::Drawable* createAxis(const osg::Vec3& corner, const osg::Vec3& xdir, const osg::Vec3& ydir, const osg::Vec3& zdir)
{
	// set up the Geometry.
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::Vec3Array* coords = new osg::Vec3Array(6);
	(*coords)[0] = corner;
	(*coords)[1] = corner + xdir;
	(*coords)[2] = corner;
	(*coords)[3] = corner + ydir;
	(*coords)[4] = corner;
	(*coords)[5] = corner + zdir;
	//Set vertex array
	geom->setVertexArray(coords);

	osg::Vec4 x_color(0.0f, 1.0f, 1.0f, 1.0f);
	osg::Vec4 y_color(0.0f, 1.0f, 1.0f, 1.0f);
	osg::Vec4 z_color(1.0f, 0.0f, 0.0f, 1.0f);

	osg::Vec4Array* color = new osg::Vec4Array(6);
	(*color)[0] = x_color;
	(*color)[1] = x_color;
	(*color)[2] = y_color;
	(*color)[3] = y_color;
	(*color)[4] = z_color;
	(*color)[5] = z_color;
	//Binding method is Single vertex
	geom->setColorArray(color, osg::Array::BIND_PER_VERTEX);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 6));

	osg::StateSet* stateset = new osg::StateSet;
	osg::LineWidth* linewidth = new osg::LineWidth();
	linewidth->setWidth(4.0f);
	//put line on the geometry
	stateset->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
	//turn off the light
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	geom->setStateSet(stateset);

	return geom.release();
}

osg::ref_ptr<osg::Node> createModel()
{

	// create the root node which will hold the model.
	osg::ref_ptr<osg::Group> root = new osg::Group();

	// add the drawable into a single geode to be shared...
	// The graphic is drawn on the surface of the polygon facing the viewpoint... 
	// and the direction of the polygon is determined according to the viewing direction of the viewpoint.
	osg::Billboard* center = new osg::Billboard();
	//POINT_ROT_EYE   Around point of view
	center->setMode(osg::Billboard::POINT_ROT_EYE);
	center->addDrawable(
		createSquare(osg::Vec3(-0.5f, 0.0f, -0.5f), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f), osgDB::readRefImageFile("E:\\OSG\\Data\\Images\\reflect.rgb")),
		osg::Vec3(0.0f, 0.0f, 0.0f));

	osg::Billboard* x_arrow = new osg::Billboard();
	//AXIAL_ROT  Axis
	x_arrow->setMode(osg::Billboard::AXIAL_ROT);
	//Set the rotation axis
	x_arrow->setAxis(osg::Vec3(1.0f, 0.0f, 0.0f));
	x_arrow->setNormal(osg::Vec3(0.0f, -1.0f, 0.0f));
	//Draw x square 
	x_arrow->addDrawable(
		createSquare(osg::Vec3(-0.5f, 0.0f, -0.5f), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f), osgDB::readRefImageFile("E:\\OSG\\Data\\Cubemap_axis\\posx.png")),
		osg::Vec3(5.0f, 0.0f, 0.0f));   //location

	osg::Billboard* y_arrow = new osg::Billboard();
	y_arrow->setMode(osg::Billboard::AXIAL_ROT);
	y_arrow->setAxis(osg::Vec3(0.0f, 1.0f, 0.0f));
	y_arrow->setNormal(osg::Vec3(1.0f, 0.0f, 0.0f));
	y_arrow->addDrawable(
		createSquare(osg::Vec3(0.0f, -0.5f, -0.5f), osg::Vec3(0.0f, 1.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f), osgDB::readRefImageFile("E:\\OSG\\Data\\Cubemap_axis\\posy.png")),
		osg::Vec3(0.0f, 5.0f, 0.0f));

	osg::Billboard* z_arrow = new osg::Billboard();
	z_arrow->setMode(osg::Billboard::AXIAL_ROT);
	z_arrow->setAxis(osg::Vec3(0.0f, 0.0f, 1.0f));
	z_arrow->setNormal(osg::Vec3(0.0f, -1.0f, 0.0f));
	z_arrow->addDrawable(
		createSquare(osg::Vec3(-0.5f, 0.0f, -0.5f), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f), osgDB::readRefImageFile("E:\\OSG\\Data\\Cubemap_axis\\posz.png")),
		osg::Vec3(0.0f, 0.0f, 5.0f));



	osg::Geode* axis = new osg::Geode();
	axis->addDrawable(createAxis(osg::Vec3(0.0f, 0.0f, 0.0f), osg::Vec3(5.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 5.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 5.0f)));


	root->addChild(center);
	root->addChild(x_arrow);
	root->addChild(y_arrow);
	root->addChild(z_arrow);
	root->addChild(axis);

	return root;
}

int main(int, char**)
{
	// construct the viewer
	osgViewer::Viewer viewer;

	// set the scene to render
	viewer.setSceneData(createModel());

	// run the viewers frame loop
	return viewer.run();
}
