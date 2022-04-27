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
#include "VulkanDrawable.hpp"
#include "VulkanShader.hpp"
#include "VulkanPipeline.hpp"

#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT

class VulkanRenderer
{
public:
	VulkanRenderer(VulkanApplication* app, VulkanDevice* deviceObject);
	~VulkanRenderer();

	void initialize();
	void prepare();
	bool render();

	void createPresentationWindow(int width, int height);
	//void onWindowResized(GLFWwindow* window, int width, int height);
	// method for handling window events
	void setImageLayout(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkAccessFlagBits srcAccessMask, const VkCommandBuffer& cmd);

	VulkanApplication* getApplication() {return application;}
	VulkanDevice* getDevice() {return deviceObj;}
	VulkanSwapChain* getSwapChain() {return swapChainObj;}
	VkCommandPool* getCommandPool() {return &cmdPool;}
	VulkanShader* getShader() {return &shaderObj;}
	VulkanPipeline* getPipelineObject() {return &pipelineObj;}

	void createCommandPool();
	void createSwapChain();
	void createDepthImage();

	void destroyCommandBuffer();
	void destroyCommandPool();
	void destroyDepthBuffer();
	void destroyRenderpass();
	void destroyDrawableVertexBuffer();

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
	VkCommandBuffer cmdVertexBuffer;
	VkCommandBuffer cmdIndexBuffer;

	int width;
	int height;

	// Render pass stuff
	void createRenderPassCB(bool includeDepth);
	void createRenderPass(bool includeDepth, bool clear = true);
	void destroyRenderPass();
	VkRenderPass renderPass;

	// Framebuffer stuff
	void createFramebuffers(bool includeDepth, bool clear = true);
	void destroyFramebuffers();
	std::vector<VkFramebuffer> framebuffers;
	std::vector<VkPipeline*> pipelineList;

	void createVertexBuffer();
	void createIndexBuffer();
	void createShaders();
	void createPipelineStateManagement();
	void destroyPipeline();
private:
	// Class managers
	VulkanSwapChain* swapChainObj;
	VulkanApplication* application;
	// Device object associated with this presentation layer
	VulkanDevice* deviceObj;

	std::vector<VulkanDrawable*> drawableList;
	VulkanShader shaderObj;
	VulkanPipeline pipelineObj;
};

#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANRENDERER_HPP_ */
