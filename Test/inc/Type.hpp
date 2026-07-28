#pragma once

//std
#include <cstdint>

enum class Type : uint32_t
{
	Mesh, MeshWarpingT, MeshWarpingS2, MeshWarpingS3, 
	Fibers, FibersWarpingT, FibersWarpingS2, FibersWarpingS3, 
	MeshGradientT2, MeshGradientS22, MeshGradientS32, MeshGradientT3, MeshGradientS23, MeshGradientS33, 
	FibersGradientT2, FibersGradientS22, FibersGradientS32, FibersGradientT3, FibersGradientS23, FibersGradientS33, 
	Last
};