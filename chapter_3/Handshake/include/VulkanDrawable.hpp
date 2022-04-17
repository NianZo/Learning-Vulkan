/*
 * VulkanDrawable.hpp
 *
 *  Created on: Apr 16, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDRAWABLE_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDRAWABLE_HPP_

#include "Header.hpp"

class VulkanRenderer;

class VulkanDrawable
{
public:
	VulkanDrawable(VulkanRenderer* parent = nullptr);
	~VulkanDrawable();

	void createVertexBuffer(const void* vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture);
	void destroyVertexBuffer();

	// Prepares the drawing object befoer rendering
	// Allocate, create, record command buffer
	void prepare();
	void render();

	// Structure storing vertex buffer metadata
	struct {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo bufferInfo;
	} VertexBuffer;

	// Stores the vertex input rate
	VkVertexInputBindingDescription viIpBind;
	// Store metadata helpful in data interpretation
	VkVertexInputAttributeDescription viIpAttrib[2];
	VulkanRenderer* rendererObj;

private:
	// Command buffer for drawing
	std::vector<VkCommandBuffer> vecCmdDraw;
	// Prepares render pass instance
	void recordCommandBuffer(int currentImage, VkCommandBuffer* cmdDraw);
};

#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDRAWABLE_HPP_ */
