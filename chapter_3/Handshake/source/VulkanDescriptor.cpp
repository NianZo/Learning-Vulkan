/*
 * VulkanDescriptor.cpp
 *
 *  Created on: Apr 26, 2022
 *      Author: nic
 */

#include "VulkanDescriptor.hpp"

void VulkanDescriptor::destroyPipelineLayout()
{
	vkDestroyPipelineLayout(deviceObj->device, pipelineLayout, nullptr);
}

void VulkanDescriptor::destroyDescriptorPool()
{
	vkDestroyDescriptorPool(deviceObj->device, descriptorPool, nullptr);
}

void VulkanDescriptor::destroyDescriptorSet()
{
	vkFreeDescriptorSets(deviceObj->device, descriptorPool, (uint32_t)descriptorSets.size(), &descriptorSets[0]);
}
