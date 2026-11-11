#include "ClusterLoD.h"
#define CLUSTERLOD_IMPLEMENTATION
#include <meshoptimizer.h>
#include <meshoptimizer/demo/clusterlod.h>

// callback used by mesoptimizer's clusterlod generator to provide the result cluster group for further processing.
int clodGroupOutput(void* output_context, clodGroup group, const clodCluster* clusters, size_t cluster_count)
{
	ClusterLodContext* context = reinterpret_cast<ClusterLodContext*>(output_context);
	println("Hey, here is something: {}", cluster_count);

	return context->mesh->currentGroupIndex++;
}

ClusterLodMesh* ClusterLodMeshFactory::createClusterLodMesh(Mesh& mesh) {
	if (mesh.compressed)
	{
		println("It is not supported yet to generate Clods from compressed data.");
		return new ClusterLodMesh();
	}

	const clodConfig& config = clodDefaultConfig(256);

	clodMesh clodmesh = {};
	clodmesh.vertex_positions_stride = sizeof(glm::vec3);
	clodmesh.vertex_count = mesh.numVertices;
	size_t positions_size = clodmesh.vertex_count * clodmesh.vertex_positions_stride;
	float* vertex_positions = reinterpret_cast<float*>(malloc(positions_size));
	CUresult result = cuMemcpyDtoH(vertex_positions, mesh.cptr_position, positions_size);
	CURuntime::assertCudaSuccess(result);
	clodmesh.vertex_positions = vertex_positions;

	clodmesh.index_count = mesh.numTriangles * 3;
	size_t index_size = clodmesh.index_count * sizeof(unsigned int);
	unsigned int* indices = reinterpret_cast<unsigned int*>(malloc(index_size));
	result = cuMemcpyDtoH(indices, mesh.cptr_indices, index_size);
	CURuntime::assertCudaSuccess(result);
	clodmesh.indices = indices;

	clodmesh.attribute_count = 0;

	ClusterLodMesh* resultingMesh = new ClusterLodMesh();
	resultingMesh->mesh = &mesh;

	ClusterLodContext* context = new ClusterLodContext();
	context->mesh = resultingMesh;

	clodBuild(config, clodmesh, context, clodGroupOutput);

	free(vertex_positions);
	free(indices);

	return resultingMesh;
}