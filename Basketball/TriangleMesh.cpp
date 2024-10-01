#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::vector<PE::PxVec3>& verts, const std::vector<PE::PxU32>& trigs, const PE::PxTransform& pose = PE::PxTransform(PE::PxIdentity))
	: StaticActor(pose)
{
	PE::PxTriangleMeshDesc mesh_desc;
	mesh_desc.points.count = (PE::PxU32)verts.size();
	mesh_desc.points.stride = sizeof(PE::PxVec3);
	mesh_desc.points.data = &verts.front();
	mesh_desc.triangles.count = (PE::PxU32)trigs.size() / 3;
	mesh_desc.triangles.stride = 3 * sizeof(PE::PxU32);
	mesh_desc.triangles.data = &trigs.front();

	CreateShape(PE::PxTriangleMeshGeometry(CookMesh(mesh_desc)));
}

//mesh cooking (preparation)
PE::PxTriangleMesh* TriangleMesh::CookMesh(const PE::PxTriangleMeshDesc& mesh_desc)
{
	PE::PxDefaultMemoryOutputStream stream;

	if (!PE::GetCooking()->cookTriangleMesh(mesh_desc, stream))
		throw new Exception("TriangleMesh::CookMesh, cooking failed.");

	PE::PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

	return PE::GetPhysics()->createTriangleMesh(input);
}