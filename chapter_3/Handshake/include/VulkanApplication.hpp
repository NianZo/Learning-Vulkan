/*
 * VulkanApplication.hpp
 *
 *  Created on: Mar 26, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANAPPLICATION_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANAPPLICATION_HPP_

#include "VulkanInstance.hpp"
#include "VulkanLayerAndExtension.hpp"
#include <mutex>

class VulkanApplication
{
public:
	~VulkanApplication();

	static VulkanApplication& GetInstance();

	// Program life cycle
	void initialize(); // Initialize and allocate resources
	void prepare(); // Prepare resources
	void update(); // Update data
	void render(); // Render primitives
	void deInitialize(); // Release resources

	VulkanInstance instanceObj;

private:
	VulkanApplication(); // Private to enforce Singleton pattern

	static std::unique_ptr<VulkanApplication> instance;
	static std::once_flag onlyOnce;

	VkResult createVulkanInstance(std::vector<const char*>& layers, std::vector<const char*>& extensions, const char* applicationName);
};



#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANAPPLICATION_HPP_ */
