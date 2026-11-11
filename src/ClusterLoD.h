#include "scene/Mesh.h"

class ClusterLodMesh {
public:
	Mesh* mesh;

	int currentGroupIndex = 0;
};

struct ClusterLodContext {
	ClusterLodMesh* mesh;
};

class ClusterLodMeshFactory {
public:
	static ClusterLodMesh* createClusterLodMesh(Mesh& mesh);
};