/*
 * MeshData.hpp
 *
 *  Created on: Apr 16, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_MESHDATA_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_MESHDATA_HPP_

#include <array>

struct VertexWithColor
{
	float x, y, z, w; // Vertex position
	float r, g, b, a; // Color format Red, Green, Blue, Alpha
};

// Interleaved data containing position and color information
static const std::array<VertexWithColor, 3> triangleData =
{{
	{0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	{1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
	{-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f}
}};


#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_MESHDATA_HPP_ */
