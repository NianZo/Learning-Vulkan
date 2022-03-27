/*
 * VulkanApplication.cpp
 *
 *  Created on: Mar 26, 2022
 *      Author: nic
 */

#include "VulkanApplication.hpp"

std::vector<const char*> instanceExtensionNames =
{
		VK_KHR_SURFACE_EXTENSION_NAME,
		//VK_KHR_WIN32_SURFACE_EXTENSION_NAME
};

std::vector<const char*> layerNames =
{
		"VK_LAYER_LUNARG_api_dump"
};



// Returns the Singleton object of VulkanApplication
VulkanApplication& VulkanApplication::GetInstance()
{
	static VulkanApplication instance;
	return instance;
}

VulkanApplication::VulkanApplication()
{
	// At application start up, enumerate instance layers
	instanceObj.layerExtension.getInstanceLayerProperties();
}

VulkanApplication::~VulkanApplication()
{

}

VkResult VulkanApplication::createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* appName)
{
	instanceObj.createInstance(layers, extensions, appName);
	return VK_SUCCESS;
}

void VulkanApplication::initialize()
{
	char title[] = "Hello World!";

	// Create the Vulkan instance with
	// specified layer and extension names.
	createVulkanInstance(layerNames, instanceExtensionNames, title);
}
