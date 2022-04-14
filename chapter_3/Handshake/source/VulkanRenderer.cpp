/*
 * VulkanRenderer.cpp
 *
 *  Created on: Apr 6, 2022
 *      Author: nic
 */

#include "VulkanRenderer.hpp"
#include <iostream>

VulkanRenderer::VulkanRenderer(VulkanApplication* app, VulkanDevice* deviceObject)
{
	assert(application != nullptr);
	assert(deviceObj != nullptr);

	// Note: very important to initialize with 0 or it will break the system
	memset(&Depth, 0, sizeof(Depth));
	//memset(&connection, 0, sizeof(HINSTANCE));

	application = app;
	deviceObj = deviceObject;

	swapChainObj = new VulkanSwapChain(this);
}

VulkanRenderer::~VulkanRenderer()
{
	delete swapChainObj;
	swapChainObj = nullptr;
}

void VulkanRenderer::createPresentationWindow(const int windowWidth, const int windowHeight)
{
	// Do a linux thing here :)
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Learning Vulkan Window", nullptr, nullptr);

	// For window resizing
	//glfwSetWindowUserPointer(window, this);
	//glfwSetWindowSizeCallback(window, onWindowResized);
}

void VulkanRenderer::initialize()
{
	// Create an empty window with dimension 500x500
	createPresentationWindow(500, 500);
	// Initialize swapchain
	swapChainObj->initializeSwapChain();

	// Need command buffers, so create a command buffer pool
	createCommandPool();
	// Let's create the swapchain color images
	buildSwapChainAndDepthImage();
}

void VulkanRenderer::createCommandPool()
{
	VulkanDevice* deviceObj = application->deviceObj;
	VkCommandPoolCreateInfo cmdPoolInfo;
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = deviceObj->graphicsQueueWithPresentIndex;

	vkCreateCommandPool(deviceObj->device, &cmdPoolInfo, nullptr, &cmdPool);
}

void VulkanRenderer::buildSwapChainAndDepthImage()
{
	// Get the appropriate queue to submit the command into
	deviceObj->getDeviceQueue();

	// Create swapchain and get the color images
	swapChainObj->createSwapChain(cmdDepthImage);

	// Create the depth image
	createDepthImage();
}

bool VulkanRenderer::render()
{
	// Do some more linux stuff here
	glfwPollEvents();
	return glfwWindowShouldClose(window);
}

void VulkanRenderer::createDepthImage()
{
	VkResult result;
	bool pass;

	VkImageCreateInfo imageInfo;

	// If format undefined, fall back to 16bit value
	if (Depth.format == VK_FORMAT_UNDEFINED)
	{
		Depth.format = VK_FORMAT_D16_UNORM;
	}
	const VkFormat depthFormat = Depth.format;

	VkFormatProperties props;
	vkGetPhysicalDeviceFormatProperties(*deviceObj->gpu, depthFormat, &props);

	if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	}
	else if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
	}
	else
	{
		std::cout << "Unsupported depth format, try other depth formats.\n";
		exit(-1);
	}

	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.pNext = nullptr;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.format = depthFormat;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = NUM_SAMPLES;
	imageInfo.queueFamilyIndexCount = 0;
	imageInfo.pQueueFamilyIndices = nullptr;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageInfo.flags = 0;

	result = vkCreateImage(deviceObj->device, &imageInfo, nullptr, &Depth.image);
	assert(result == VK_SUCCESS);

	VkMemoryRequirements memRqrmnt;
	vkGetImageMemoryRequirements(deviceObj->device, Depth.image, &memRqrmnt);

	VkMemoryAllocateInfo memAlloc;
	memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAlloc.pNext = nullptr;
	memAlloc.allocationSize = 0;
	memAlloc.memoryTypeIndex = 0;
	memAlloc.allocationSize = memRqrmnt.size;
	pass = deviceObj->memoryTypeFromProperties(memRqrmnt.memoryTypeBits, 0, &memAlloc.memoryTypeIndex);

	result = vkAllocateMemory(deviceObj->device, &memAlloc, nullptr, &Depth.mem);
	assert(result == VK_SUCCESS);

	result = vkBindImageMemory(deviceObj->device, Depth.image, Depth.mem, 0);
	assert(result == VK_SUCCESS);

	VkImageViewCreateInfo imgViewInfo;
	imgViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imgViewInfo.pNext = nullptr;
	imgViewInfo.flags = 0;
	imgViewInfo.image = VK_NULL_HANDLE;
	imgViewInfo.format = depthFormat;
	imgViewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY};
	imgViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
	imgViewInfo.subresourceRange.baseMipLevel = 0;
	imgViewInfo.subresourceRange.levelCount = 1;
	imgViewInfo.subresourceRange.baseArrayLayer = 0;
	imgViewInfo.subresourceRange.layerCount = 1;
	imgViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

	if (depthFormat == VK_FORMAT_D16_UNORM_S8_UINT || depthFormat == VK_FORMAT_D24_UNORM_S8_UINT || depthFormat == VK_FORMAT_D32_SFLOAT_S8_UINT)
	{
		imgViewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	// Set image layout
	CommandBufferMgr::allocCommandBuffer(&deviceObj->device, cmdPool, &cmdDepthImage);
	CommandBufferMgr::beginCommandBuffer(cmdDepthImage);
}









