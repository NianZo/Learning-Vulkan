/*
 * VulkanLayerAndExtension.hpp
 *
 *  Created on: Mar 20, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANLAYERANDEXTENSION_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANLAYERANDEXTENSION_HPP_

#include <vector>
#include "Header.hpp"

struct LayerProperties
{
	VkLayerProperties properties;
	std::vector<VkExtensionProperties> extensions;
};

class VulkanLayerAndExtension
{
public:
	std::vector<const char*> instanceExtensionNames;
	std::vector<const char*> instanceLayerNames;
	// Layers and corresponding extension list
	std::vector<LayerProperties> layerPropertyList;
	VkResult getInstanceLayerProperties();

	// Global extensions
	VkResult getExtensionProperties(LayerProperties& layerProps, VkPhysicalDevice* gpu = nullptr);

	// Device based extensions
	VkResult getDeviceExtensionProperties(VkPhysicalDevice* gpu);
};



#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANLAYERANDEXTENSION_HPP_ */
