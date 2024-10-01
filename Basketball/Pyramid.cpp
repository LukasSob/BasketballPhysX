#include "Pyramid.h"

//Pyramid::Pyramid(PE::PxTransform pose, PE::PxReal density) :
 //   ConvexMesh(std::vector<PE::PxVec3>(std::begin(pyramid_verts), std::end(pyramid_verts)), pose, density)
//{
//}

PyramidStatic::PyramidStatic(PE::PxTransform pose) :
	TriangleMesh(std::vector<PE::PxVec3>(std::begin(pyramid_verts), std::end(pyramid_verts)), std::vector<PE::PxU32>(std::begin(pyramid_trigs), std::end(pyramid_trigs)), pose)
{
}