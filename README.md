# Learning-Vulkan
A refresher on Vulkan following 'Learning Vulkan' by Parminder Singh

## Important Notes for Eclipse IDE
Eclipse typically gets VERY ornery when I start working with Vulkan. My current BKM is to do the following:
1. Import the git repository as a Makefile project
2. Choose cross-gcc for the build system. NOT CMake driven! This will mess with the include paths and Eclipse will struggle to locate system headers for some reason
3. Add the path to the VulkanSDK headers in Project->Propterties->C/C++ General->Preprocessor Include Paths->CDT User Setting Entries. On archlinux this path was /usr/include/vulkan for me.
4. Enjoy Eclipse properly indexing your project!

## Current Notes for Building
Until I find a better way, building this project takes an extra step. First open a terminal that you intend to build the application from. Then run setup-env.sh from the VulkanSDK. This will set up the ${VULKAN_PATH} environment variable.
