/*
 * VulkanRenderer.hpp
 *
 *  Created on: Apr 6, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANRENDERER_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANRENDERER_HPP_

#include "Header.hpp"
//#include "VulkanApplication.hpp"
#include "VulkanSwapChain.hpp"
#include "Wrapper.hpp"

#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT

class VulkanRenderer
{
public:
	VulkanRenderer(VulkanApplication* app, VulkanDevice* deviceObject);
	~VulkanRenderer();

	void initialize();
	bool render();

	void createPresentationWindow(int width, int height);
	// method for handling window events
	void setImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits srcAccessMask, const VkCommandBuffer& cmd);

	VulkanApplication* getApplication() {return application;}
	VulkanDevice* getDevice() {return deviceObj;}
	VulkanSwapChain* getSwapChain() {return swapChainObj;}

	void createCommandPool();
	void createSwapChain();
	void createDepthImage();

	void destroyCommandBuffer();
	void destroyCommandPool();
	void destroyDepthBuffer();

	void buildSwapChainAndDepthImage();

	// Window instance / handle
	//xcb_connection_t* connection;
	//xcb_screen_t* screen;
	GLFWwindow* window;
	//xcb_intern_atom_reply_t* reply;
	char name[80]; // app name appearing on the window

	// Data structure for depth image
	struct
	{
		VkFormat format;
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} Depth;

	VkCommandBuffer cmdDepthImage;
	VkCommandPool cmdPool;

	int width;
	int height;
private:
	// Class managers
	VulkanSwapChain* swapChainObj;
	VulkanApplication* application;
	// Device object associated with this presentation layer
	VulkanDevice* deviceObj;
};

#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANRENDERER_HPP_ */
