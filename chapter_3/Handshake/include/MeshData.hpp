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

static const std::array<VertexWithColor, 4> squareData =
{{
	{-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
	{0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	{0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
	{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f}
}};

static const std::array<uint16_t, 6> squareIndices = {{0, 3, 1, 3, 2, 1}};


#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_MESHDATA_HPP_ */
