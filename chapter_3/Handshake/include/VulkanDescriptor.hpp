/*
 * VulkanDescriptor.hpp
 *
 *  Created on: Apr 26, 2022
 *      Author: nic
 */

#ifndef CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDESCRIPTOR_HPP_
#define CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDESCRIPTOR_HPP_

#include "Header.hpp"
#include "VulkanDevice.hpp"

class VulkanDescriptor
{
public:
	VulkanDescriptor();
	virtual ~VulkanDescriptor();

	void createDescriptor(bool useTexture);
	void destroyDescriptor();

	virtual void createDescriptorLayout(bool useTexture) = 0;
	void destroyDescriptorLayout();

	virtual void createDescriptorPool(bool useTexture) = 0;
	void destroyDescriptorPool();

	virtual void createDescriptorSet(bool useTexture) = 0;
	void destroyDescriptorPool();

	virtual void createPipelineLayout() = 0;
	void destroyPipelineLayout();

	VkPipelineLayout pipelineLayout;
	std::vector<VkDescriptorSetLayout> descriptorLayouts;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VulkanDevice* deviceObj;
};

#endif /* CHAPTER_3_HANDSHAKE_INCLUDE_VULKANDESCRIPTOR_HPP_ */
