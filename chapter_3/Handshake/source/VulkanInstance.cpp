/*
 * VulkanInstance.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: nic
 */

#include "VulkanInstance.hpp"

VkResult VulkanInstance::createInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* const applicationName)
{
	// Set the instance specific layer and extension information
	layerExtension.instanceExtensionNames = extensions;
	layerExtension.instanceLayerNames = layers;

	// Define the Vulkan application structure
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = applicationName;
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = applicationName;
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Define the Vulkan instance create info structure
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = layers.size();
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledLayerCount = extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	VkResult res = vkCreateInstance(&instanceInfo, nullptr, &instance);
	return res;
}

void VulkanInstance::destroyInstance()
{
	vkDestroyInstance(instance, nullptr);
}

