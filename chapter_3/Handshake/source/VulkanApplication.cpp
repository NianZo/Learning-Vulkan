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

std::vector<const char*> deviceExtensionNames =
{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

std::vector<VkPhysicalDevice> gpuList;





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

	deviceObj = nullptr;
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

	// Get list of PhysicalDevices (gpus) in the system
	std::vector<VkPhysicalDevice> gpuList;
	enumeratePhysicalDevices(gpuList);
	// Use the first GPU found for this exercise
	if (gpuList.size() > 0)
	{
		handShakeWithDevice(&gpuList[0], layerNames, deviceExtensionNames);
	}
}

VkResult VulkanApplication::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& gpuList)
{
	uint32_t gpuDeviceCount;
	vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, nullptr);
	gpuList.resize(gpuDeviceCount);
	return vkEnumeratePhysicalDevices(instanceObj.instance, &gpuDeviceCount, gpuList.data());
}

VkResult VulkanApplication::handShakeWithDevice(VkPhysicalDevice* gpu, std::vector<const char*>& layers, std::vector<const char*>& extensions)
{
	// Our abstraction of Vulkan logical and physical device
	// Manages queues and their properties
	deviceObj = new VulkanDevice(gpu);
	if (deviceObj == nullptr)
	{
		return VK_ERROR_OUT_OF_HOST_MEMORY;
	}

	// Print the devices available, layers, and extensions
	deviceObj->layerExtension.getDeviceExtensionProperties(gpu);

	// Get the physical device (GPU) properties
	vkGetPhysicalDeviceProperties(*gpu, &deviceObj->gpuProps);

	// Get memory properties from the physical device or GPU
	vkGetPhysicalDeviceMemoryProperties(*gpu, &deviceObj->memoryProperties);

	// Query queues and properties from GPU
	deviceObj->getPhysicalDeviceQueuesAndProperties();

	// Retrieve the queue which supports graphics pipeline
	deviceObj->getGraphicsQueueHandle();

	// Create logical device; ensure that the device can use the graphics queue
	deviceObj->createDevice(layers, extensions);

	return VK_SUCCESS;
}








