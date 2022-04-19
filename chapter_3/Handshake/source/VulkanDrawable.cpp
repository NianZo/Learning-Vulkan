/*
 * VulkanDrawable.cpp
 *
 *  Created on: Apr 16, 2022
 *      Author: nic
 */

#include "VulkanDrawable.hpp"
#include "VulkanApplication.hpp"
#include <thread>
#include <chrono>
#include <iostream>

VulkanDrawable::VulkanDrawable(VulkanRenderer* parent)
{
	// "Note: It's very important to initialize the member with 0 or respective value otherwise it will break the system"
	memset(&VertexBuffer, 0, sizeof(VertexBuffer));
	rendererObj = parent;

	VulkanDevice* deviceObj = VulkanApplication::GetInstance()->deviceObj;

	VkSemaphoreCreateInfo presentCompleteSemaphoreCreateInfo;
	presentCompleteSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	presentCompleteSemaphoreCreateInfo.pNext = nullptr;
	presentCompleteSemaphoreCreateInfo.flags = 0;
	vkCreateSemaphore(deviceObj->device, &presentCompleteSemaphoreCreateInfo, nullptr, &presentCompleteSemaphore);

	VkSemaphoreCreateInfo drawingCompleteSemaphoreCreateInfo;
	drawingCompleteSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	drawingCompleteSemaphoreCreateInfo.pNext = nullptr;
	drawingCompleteSemaphoreCreateInfo.flags = 0;
	vkCreateSemaphore(deviceObj->device, &drawingCompleteSemaphoreCreateInfo, nullptr, &drawingCompleteSemaphore);
}

void VulkanDrawable::createVertexBuffer(const void* vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture)
{
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	VulkanDevice* deviceObj = appObj->deviceObj;

	VkResult result;
	bool pass;

	//Create the buffer resource metadata information
	VkBufferCreateInfo bufInfo;
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.pNext = nullptr;
	bufInfo.flags = 0;
	bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufInfo.size = dataSize;
	bufInfo.queueFamilyIndexCount = 0; // TODO wtf
	bufInfo.pQueueFamilyIndices = nullptr;
	bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	result = vkCreateBuffer(deviceObj->device, &bufInfo, nullptr, &VertexBuffer.buffer);
	assert(result == VK_SUCCESS);

	VkMemoryRequirements memRqrmnt;
	vkGetBufferMemoryRequirements(deviceObj->device, VertexBuffer.buffer, &memRqrmnt);

	// Allocate buffer memory
	VkMemoryAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.memoryTypeIndex = 0;
	allocInfo.allocationSize = memRqrmnt.size;

	pass = deviceObj->memoryTypeFromProperties(memRqrmnt.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &allocInfo.memoryTypeIndex);
	assert(pass == true);
	result = vkAllocateMemory(deviceObj->device, &allocInfo, nullptr, &VertexBuffer.memory);
	assert(result == VK_SUCCESS);

	VertexBuffer.bufferInfo.range = memRqrmnt.size;
	VertexBuffer.bufferInfo.offset = 0;

	// Map memory
	uint8_t* pData;
	result = vkMapMemory(deviceObj->device, VertexBuffer.memory, 0, memRqrmnt.size, 0, (void**)&pData);
	assert(result == VK_SUCCESS);

	// Copy data to mapped memory
	memcpy(pData, vertexData, dataSize);

	// Unmap device memory
	vkUnmapMemory(deviceObj->device, VertexBuffer.memory);

	// Bind buffer to device memory
	result = vkBindBufferMemory(deviceObj->device, VertexBuffer.buffer, VertexBuffer.memory, 0);
	assert(result == VK_SUCCESS);

	viIpBind.binding = 0;
	viIpBind.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	viIpBind.stride = dataStride;

	viIpAttrib[0].binding = 0;
	viIpAttrib[0].location = 0;
	viIpAttrib[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	viIpAttrib[0].offset = 0;
	viIpAttrib[1].binding = 0;
	viIpAttrib[1].location = 1;
	viIpAttrib[1].format = useTexture ? VK_FORMAT_R32G32_SFLOAT : VK_FORMAT_R32G32B32A32_SFLOAT;
	viIpAttrib[1].offset = 16;
}

void VulkanDrawable::destroyVertexBuffer()
{
	VulkanDevice* deviceObj = rendererObj->getDevice();

	vkDestroyBuffer(deviceObj->device, VertexBuffer.buffer, nullptr);
	vkFreeMemory(deviceObj->device, VertexBuffer.memory, nullptr);
}

void VulkanDrawable::prepare()
{
	VulkanDevice* deviceObj = rendererObj->getDevice();
	vecCmdDraw.resize(rendererObj->getSwapChain()->scPublicVars.colorBuffer.size());
	std::cout << "vecCmdDraw.size(): " << vecCmdDraw.size() << std::endl;
	// Allocate command buffer per swapchain color image
	for (int i = 0; i < rendererObj->getSwapChain()->scPublicVars.colorBuffer.size(); i++)
	{
		// Allocate, create, and start command buffer recording
		CommandBufferMgr::allocCommandBuffer(&deviceObj->device, *rendererObj->getCommandPool(), &vecCmdDraw[i]);
		CommandBufferMgr::beginCommandBuffer(vecCmdDraw[i]);

		// Create the render pass instance
		recordCommandBuffer(i, &vecCmdDraw[i]);

		// Finish command buffer recording
		CommandBufferMgr::endCommandBuffer(vecCmdDraw[i]);
	}
}

void VulkanDrawable::render()
{
	VulkanDevice* deviceObj = rendererObj->getDevice();
	VulkanSwapChain* swapChainObj = rendererObj->getSwapChain();
	uint32_t& currentColorImage = swapChainObj->scPublicVars.currentColorBuffer;
	VkSwapchainKHR& swapChain = swapChainObj->scPublicVars.swapChain;

	// vkAcquireNextImageKHR requires either a valid fence or semaphore
	//VkSemaphore presentCompleteSemaphore;


	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// Get index of next available swapchain image
	VkResult result = swapChainObj->fpAcquireNextImageKHR(deviceObj->device, swapChain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, &currentColorImage);

	VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &presentCompleteSemaphore;
	submitInfo.pWaitDstStageMask = &submitPipelineStages;
	submitInfo.commandBufferCount = (uint32_t) sizeof(&vecCmdDraw[currentColorImage]) / sizeof(VkCommandBuffer);
	submitInfo.pCommandBuffers = &vecCmdDraw[currentColorImage];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &drawingCompleteSemaphore;

	std::cout << "currentColorImage index: " << currentColorImage << std::endl;
	CommandBufferMgr::submitCommandBuffer(deviceObj->queue, &vecCmdDraw[currentColorImage], &submitInfo);

	// Present the image in the window
	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &drawingCompleteSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;
	presentInfo.pImageIndices = &currentColorImage;
	presentInfo.pResults = nullptr;
	result = swapChainObj->fpQueuePresentKHR(deviceObj->queue, &presentInfo);
	assert(result == VK_SUCCESS);
}

void VulkanDrawable::recordCommandBuffer(int currentImage, VkCommandBuffer* cmdDraw)
{
	// Specify clear color value
	VkClearValue clearValues[2];
	switch (currentImage)
	{
	case 0:
		clearValues[0].color = {1.0f, 0.0f, 0.0f, 1.0f};
		break;
	case 1:
		clearValues[0].color = {0.0f, 1.0f, 0.0f, 1.0f};
		break;
	case 2:
		clearValues[0].color = {0.0f, 0.0f, 1.0f, 1.0f};
		break;
	default:
		clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
		break;
	}
	clearValues[1].depthStencil.depth = 1.0f;
	clearValues[1].depthStencil.stencil = 0;

	VkRenderPassBeginInfo rpBegin;
	rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBegin.pNext = nullptr;
	rpBegin.renderPass = rendererObj->renderPass;
	rpBegin.framebuffer = rendererObj->framebuffers[currentImage];
	rpBegin.renderArea.extent.width = rendererObj->width;
	rpBegin.renderArea.extent.height = rendererObj->height;
	rpBegin.renderArea.offset.x = 0;
	rpBegin.renderArea.offset.y = 0;
	rpBegin.clearValueCount = 2;
	rpBegin.pClearValues = clearValues;

	vkCmdBeginRenderPass(*cmdDraw, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(*cmdDraw, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);

	const VkDeviceSize offsets[1] = { 0 };
	vkCmdBindVertexBuffers(*cmdDraw, 0, 1, &VertexBuffer.buffer, offsets);

	initViewports(cmdDraw);
	initScissors(cmdDraw);

	vkCmdDraw(*cmdDraw, 3, 1, 0, 0);

	vkCmdEndRenderPass(*cmdDraw);
}

void VulkanDrawable::initViewports(VkCommandBuffer* cmd)
{
	viewport.height = (float) rendererObj->height;
	viewport.width = (float) rendererObj->width;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	viewport.x = 0;
	viewport.y = 0;
	vkCmdSetViewport(*cmd, 0, NUMBER_OF_VIEWPORTS, &viewport);
}

void VulkanDrawable::initScissors(VkCommandBuffer* cmd)
{
	scissor.extent.width = rendererObj->width;
	scissor.extent.height = rendererObj->height;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
	vkCmdSetScissor(*cmd, 0, NUMBER_OF_SCISSORS, &scissor);
}





























