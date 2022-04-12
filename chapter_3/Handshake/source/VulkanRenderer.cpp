/*
 * VulkanRenderer.cpp
 *
 *  Created on: Apr 6, 2022
 *      Author: nic
 */

#include "VulkanRenderer.hpp"

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










