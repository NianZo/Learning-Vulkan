/*
 * Wrapper.hpp
 *
 *  Created on: Apr 5, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_WRAPPER_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_WRAPPER_HPP_

#include "Header.hpp"

class CommandBufferMgr
{
public:
	// Allocate memory for command buffers from the command pool
	static void allocCommandBuffer(const VkDevice* device, const VkCommandPool cmdPool, VkCommandBuffer* cmdBuf, const VkCommandBufferAllocateInfo* commandBufferInfo = nullptr);

	// Start command buffer recording
	static void beginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferBeginInfo* inCmdBufInfo = nullptr);

	// End the command buffer recording
	static void endCommandBuffer(VkCommandBuffer cmdBuf);

	// Submit the command buffer for execution
	static void submitCommandBuffer(const VkQueue& queue, const VkCommandBuffer* cmdBufList, const VkSubmitInfo* submitInfo = nullptr, const VkFence& fence = VK_NULL_HANDLE);
};


#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_WRAPPER_HPP_ */
