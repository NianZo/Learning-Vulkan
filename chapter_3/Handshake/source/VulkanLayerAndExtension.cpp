/*
 * VulkanLayerAndExtension.cpp
 *
 *  Created on: Mar 20, 2022
 *      Author: nic
 */

#include "VulkanLayerAndExtension.hpp"

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	// Stores number of instance layers
	uint32_t instanceLayerCount;
	// Vector to store layer properties
	std::vector<VkLayerProperties> layerProperties;
	// Check Vulkan API result status
	VkResult result;

	// Query all the layers
}


