/*
 * main.cpp
 *
 *  Created on: Mar 26, 2022
 *      Author: nic
 */

#include "Header.hpp"
#include "VulkanApplication.hpp"



int main(int argc, char** argv)
{
	VkResult res;

	// Create singleton object, calls VulkanApplication constructor
	VulkanApplication* appObj = VulkanApplication::GetInstance();
	appObj->initialize();
	appObj->prepare();

	while(!appObj->render()) {
		appObj->update();
	};
}

