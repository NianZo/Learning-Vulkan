/*
 * VulkanDrawable.hpp
 *
 *  Created on: Apr 16, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDRAWABLE_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDRAWABLE_HPP_

#include "Header.hpp"
#include "VulkanDescriptor.hpp"

class VulkanRenderer;

class VulkanDrawable : VulkanDescriptor
{
public:
	VulkanDrawable(VulkanRenderer* parent = nullptr);
	~VulkanDrawable();

	void createVertexBuffer(const void* vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture);
	void createIndexBuffer(const void* vertexData, uint32_t dataSize, uint32_t dataStride);
	void destroyVertexBuffer();
	void destroyIndexBuffer();

	// Prepares the drawing object befoer rendering
	// Allocate, create, record command buffer
	void prepare();
	void render();
	void initViewports(VkCommandBuffer* cmd);
	void initScissors(VkCommandBuffer* cmd);

	void setPipeline(VkPipeline* vulkanPipeline) {pipeline = vulkanPipeline;}
	VkPipeline* getPipeline() {return pipeline;}

	void createDescriptorLayout(bool useTexture) override;
	void createPipelineLayout() override;
	void createDescriptorPool(bool useTexture) override;
	void createDescriptorResources() override;

	// Structure storing vertex buffer metadata
	struct {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo bufferInfo;
	} VertexBuffer;

	struct {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDescriptorBufferInfo bufferInfo;
	} IndexBuffer;

	// Stores the vertex input rate
	VkVertexInputBindingDescription viIpBind;
	// Store metadata helpful in data interpretation
	VkVertexInputAttributeDescription viIpAttrib[2];
	VulkanRenderer* rendererObj;

private:
	// Command buffer for drawing
	std::vector<VkCommandBuffer> vecCmdDraw;

	VkViewport viewport;
	VkRect2D scissor;
	VkSemaphore presentCompleteSemaphore;
	VkSemaphore drawingCompleteSemaphore;

	// Prepares render pass instance
	void recordCommandBuffer(int currentImage, VkCommandBuffer* cmdDraw);

	VkPipeline* pipeline;
};

#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDRAWABLE_HPP_ */
