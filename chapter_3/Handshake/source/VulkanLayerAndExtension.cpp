/*
 * VulkanLayerAndExtension.cpp
 *
 *  Created on: Mar 20, 2022
 *      Author: nic
 */

#include "VulkanLayerAndExtension.hpp"
#include <iostream>

VkResult VulkanLayerAndExtension::getInstanceLayerProperties()
{
	// Stores number of instance layers
	uint32_t instanceLayerCount;
	// Vector to store layer properties
	std::vector<VkLayerProperties> layerProperties;
	// Check Vulkan API result status
	VkResult result;

	// Query all the layers
	do
	{
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

		if (result)
			return result;

		if (instanceLayerCount == 0)
			return VK_INCOMPLETE; // return fail

		layerProperties.resize(instanceLayerCount);
		result  = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
	} while (result == VK_INCOMPLETE);

	// Query all the extensions for each layer and store it.
	std::cout << "\nInstanced Layers" << std::endl;
	std::cout << "===================" << std::endl;
	for (auto globalLayerProp : layerProperties)
	{
		// Print layer name and its description
		std::cout << "\n" << globalLayerProp.description << "\n\t|\n\t|---[Layer Name]--> " << globalLayerProp.layerName << "\n";

	}
}


