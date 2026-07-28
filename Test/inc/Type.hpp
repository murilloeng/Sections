#pragma once

//std
#include <cstdint>

enum class Type : uint32_t
{
	Mesh, MeshWarpingT, MeshWarpingS2, MeshWarpingS3, 
	Fibers, FibersWarpingT, FibersWarpingS2, FibersWarpingS3,
	Last
};