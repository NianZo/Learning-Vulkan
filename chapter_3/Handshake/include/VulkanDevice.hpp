/*
 * VulkanDevice.hpp
 *
 *  Created on: Mar 27, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDEVICE_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDEVICE_HPP_

#include "Header.hpp"
#include "VulkanLayerAndExtension.hpp"

class VulkanDevice
{
public:
	VulkanDevice(VkPhysicalDevice* gpu);
	~VulkanDevice();

	VkDevice device;
	VkPhysicalDevice* gpu;
	VkPhysicalDeviceProperties gpuProps;
	VkPhysicalDeviceMemoryProperties memoryProperties;

	VkQueue queue;
	std::vector<VkQueueFamilyProperties> queueFamilyProps;
	uint32_t graphicsQueueFamilyIndex;
	uint32_t queueFamilyCount;
	VulkanLayerAndExtension layerExtension;

	VkResult createDevice(std::vector<const char*>& layers, std::vector<const char*>& extensions);
	void destroyDevice();
	void initializeDeviceQueue();
	void memoryTypeFromProperties();
	void getGraphicsQueueHandle();
	void getPhysicalDeviceQueuesAndProperties();
};

#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDEVICE_HPP_ */
