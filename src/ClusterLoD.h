#include "scene/Mesh.h"

class ClusterLodMesh {
public:
	Mesh* mesh;

	int currentGroupIndex = 0;
};

struct ClusterLodContext {
	ClusterLodMesh* mesh;
	unsigned int* groupIndeces = nullptr;
	size_t groupIndecesCount = 0;
};

class ClusterLodMeshFactory {
public:
	static ClusterLodMesh* createClusterLodMesh(Mesh& mesh);
};