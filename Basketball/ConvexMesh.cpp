#include "ConvexMesh.h"

ConvexMesh::ConvexMesh(const std::vector<PE::PxVec3>& verts, const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity), PE::PxReal density = 1.f)
	: DynamicActor(pose)
{
	PE::PxConvexMeshDesc mesh_desc;
	mesh_desc.points.count = (PE::PxU32)verts.size();
	mesh_desc.points.stride = sizeof(PE::PxVec3);
	mesh_desc.points.data = &verts.front();
	mesh_desc.flags = PE::PxConvexFlag::eCOMPUTE_CONVEX;
	mesh_desc.vertexLimit = 256;



	CreateShape(PE::PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
}

//mesh cooking (preparation)
PE::PxConvexMesh* ConvexMesh::CookMesh(const PE::PxConvexMeshDesc& mesh_desc)
{
	PE::PxDefaultMemoryOutputStream stream;

	if (!PE::GetCooking()->cookConvexMesh(mesh_desc, stream))
		throw new Exception("ConvexMesh::CookMesh, cooking failed.");

	PE::PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

	return PE::GetPhysics()->createConvexMesh(input);
}