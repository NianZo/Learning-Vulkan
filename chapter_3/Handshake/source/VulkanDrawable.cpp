/*
 * VulkanDrawable.cpp
 *
 *  Created on: Apr 16, 2022
 *      Author: nic
 */

#include "VulkanDrawable.hpp"
#include "VulkanApplication.hpp"

VulkanDrawable::VulkanDrawable(VulkanRenderer* parent)
{
	// "Note: It's very important to initialize the member with 0 or respective value otherwise it will break the system"
	memset(&VertexBuffer, 0, sizeof(VertexBuffer));
	rendererObj = parent;
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











