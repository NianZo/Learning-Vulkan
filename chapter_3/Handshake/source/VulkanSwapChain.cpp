/*
 * VulkanSwapChain.cpp
 *
 *  Created on: Apr 11, 2022
 *      Author: nic
 */

#include "VulkanSwapChain.hpp"

#include "VulkanDevice.hpp"
#include "VulkanInstance.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanApplication.hpp"

VulkanSwapChain::VulkanSwapChain(VulkanRenderer* renderer)
{
	rendererObj = renderer;
	appObj = VulkanApplication::GetInstance();
}

VulkanSwapChain::~VulkanSwapChain()
{
	scPrivateVars.swapchainImages.clear();
	scPrivateVars.surfFormats.clear();
	scPrivateVars.presentModes.clear();
}

VkResult VulkanSwapChain::createSwapChainExtensions()
{
	// Dependency on createPresentationWindow()
	VkInstance& instance = appObj->instanceObj.instance;
	VkDevice& device = appObj->deviceObj->device;

	// Get instance based swapchain extension function pointers
	// TODO I'm not convinced these all need to be dynamically queried
	fpGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR) vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	fpGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR) vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	fpGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR) vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	fpGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR) vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	fpDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR) vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");

	// Get device based swapchain extension function pointers
	// TODO '''
	fpCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR) vkGetInstanceProcAddr(instance, "vkCreateSwapchainKHR");
	fpDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR) vkGetInstanceProcAddr(instance, "vkDestroySwapchainKHR");
	fpGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR) vkGetInstanceProcAddr(instance, "vkGetSwapchainImagesKHR");
	fpAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR) vkGetInstanceProcAddr(instance, "vkAcquireNextImageKHR");
	fpQueuePresentKHR = (PFN_vkQueuePresentKHR) vkGetInstanceProcAddr(instance, "vkQueuePresentKHR");

	return VK_SUCCESS;
}

VkResult VulkanSwapChain::createSurface()
{
	VkInstance& instance = appObj->instanceObj.instance;

	return glfwCreateWindowSurface(instance, rendererObj->window, nullptr, &scPublicVars.surface);
}

uint32_t VulkanSwapChain::getGraphicsQueueWithPresentationSupport()
{
	VulkanDevice* device = appObj->deviceObj;
	uint32_t queueCount = device->queueFamilyCount;
	VkPhysicalDevice gpu = *device->gpu;
	std::vector<VkQueueFamilyProperties>& queueProps = device->queueFamilyProps;

	// Iterate each queue family and get presentation status for each
	std::vector<VkBool32> supportsPresent;
	supportsPresent.resize(queueCount);
	for (uint32_t i = 0; i < queueCount; i++)
	{
		supportsPresent[i] = VK_FALSE;
		fpGetPhysicalDeviceSurfaceSupportKHR(gpu, i, scPublicVars.surface, &supportsPresent[i]);
	}

	// Search for a graphics queue that supports presentation
	// TODO this algorithm makes no sense... both paths look for the same thing
	uint32_t graphicsQueueNodeIndex = UINT32_MAX;
	uint32_t presentQueueNodeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < queueCount; i ++)
	{
		if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
		{
			graphicsQueueNodeIndex = i;
		}
		if (supportsPresent[i] == VK_TRUE)
		{
			graphicsQueueNodeIndex = i;
			presentQueueNodeIndex = i;
			break;
		}
	}
	if (presentQueueNodeIndex == UINT32_MAX)
	{
		// If didn't find a queue that supports both graphics and present, then find a separate present queue
		for (uint32_t i = 0; i < queueCount; i++)
		{
			if (supportsPresent[i] == VK_TRUE)
			{
				presentQueueNodeIndex = i;
				break;
			}
		}
	}

	// Generate error if could not find queue with present capabilities
	if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX)
	{
		return UINT32_MAX;
	}
	return graphicsQueueNodeIndex;
}

void VulkanSwapChain::getSupportedFormat()
{
	VkPhysicalDevice gpu = *rendererObj->getDevice()->gpu;
	VkResult result;
	// Get number of formats supported
	uint32_t formatCount;
	fpGetPhysicalDeviceSurfaceFormatsKHR(gpu, scPublicVars.surface, &formatCount, nullptr);
	scPrivateVars.surfFormats.clear();
	scPrivateVars.surfFormats.resize(formatCount);

	// Get formats in allocated objects
	result = fpGetPhysicalDeviceSurfaceFormatsKHR(gpu, scPublicVars.surface, &formatCount, scPrivateVars.surfFormats.data());
	// If we just get VK_FORMAT_UNDEFINED then there is no preferred format; use RGBA32 format
	if (formatCount == 1 && scPrivateVars.surfFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		scPublicVars.format = VK_FORMAT_B8G8R8A8_UNORM;
	} else
	{
		scPublicVars.format = scPrivateVars.surfFormats[0].format;
	}
}

void VulkanSwapChain::getSurfaceCapabilitiesAndPresentMode()
{
	VkResult result;
	VkPhysicalDevice gpu = *appObj->deviceObj->gpu;

	fpGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, scPublicVars.surface, &scPrivateVars.surfCapabilities);

	fpGetPhysicalDeviceSurfacePresentModesKHR(gpu, scPublicVars.surface, &scPrivateVars.presentModeCount, nullptr);
	scPrivateVars.presentModes.clear();
	scPrivateVars.presentModes.resize(scPrivateVars.presentModeCount);
	fpGetPhysicalDeviceSurfacePresentModesKHR(gpu, scPublicVars.surface, &scPrivateVars.presentModeCount, scPrivateVars.presentModes.data());

	if (scPrivateVars.surfCapabilities.currentExtent.width == (uint32_t)-1)
	{
		// If surface width and height aren't defined then set equal to image size
		scPrivateVars.swapChainExtent.width = rendererObj->width;
		scPrivateVars.swapChainExtent.height = rendererObj->height;
	} else
	{
		scPrivateVars.swapChainExtent = scPrivateVars.surfCapabilities.currentExtent;
	}
}

void VulkanSwapChain::managePresentMode()
{
	// Prefere MAILBOX - lowest latency non-tearing mode
	// Then prefer IMMEDIATE - fastest (but tears)
	// Then fall back to FIFO
	scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

	for (size_t i = 0; i < scPrivateVars.presentModeCount; i++)
	{
		if (scPrivateVars.presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		if (scPrivateVars.swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR && scPrivateVars.presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			scPrivateVars.swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	// Determine number of VkImages to use in swapchain
	scPrivateVars.desiredNumberOfSwapChainImages = scPrivateVars.surfCapabilities.minImageCount + 1;
	if ((scPrivateVars.surfCapabilities.maxImageCount > 0) && (scPrivateVars.desiredNumberOfSwapChainImages > scPrivateVars.surfCapabilities.maxImageCount))
	{
		// Settle for fewer images than desired
		scPrivateVars.desiredNumberOfSwapChainImages = scPrivateVars.surfCapabilities.maxImageCount;
	}

	if (scPrivateVars.surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		scPrivateVars.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	} else
	{
		scPrivateVars.preTransform = scPrivateVars.surfCapabilities.currentTransform;
	}
}

void VulkanSwapChain::createSwapChainColorBufferImages()
{
	VkSwapchainCreateInfoKHR scInfo;
	scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	scInfo.pNext = nullptr;
	scInfo.surface = scPublicVars.surface;
	scInfo.minImageCount = scPrivateVars.desiredNumberOfSwapChainImages;
	scInfo.imageFormat = scPublicVars.format;
	scInfo.imageExtent.width = scPrivateVars.swapChainExtent.width;
	scInfo.imageExtent.height = scPrivateVars.swapChainExtent.height;
	scInfo.preTransform = scPrivateVars.preTransform;
	scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	scInfo.imageArrayLayers = 1;
	scInfo.presentMode = scPrivateVars.swapchainPresentMode;
	scInfo.oldSwapchain = VK_NULL_HANDLE;
	scInfo.clipped = true;
	scInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	scInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	scInfo.queueFamilyIndexCount = 0;
	scInfo.pQueueFamilyIndices = nullptr;

	fpCreateSwapchainKHR(rendererObj->getDevice()->device, &scInfo, nullptr, &scPublicVars.swapChain);

	fpGetSwapchainImagesKHR(rendererObj->getDevice()->device, scPublicVars.swapChain, &scPublicVars.swapchainImageCount, nullptr);
	scPrivateVars.swapchainImages.clear();
	scPrivateVars.swapchainImages.resize(scPublicVars.swapchainImageCount);
	fpGetSwapchainImagesKHR(rendererObj->getDevice()->device, scPublicVars.swapChain, &scPublicVars.swapchainImageCount, scPrivateVars.swapchainImages.data());
}

void VulkanSwapChain::createColorImageView(const VkCommandBuffer& cmd)
{

}

























