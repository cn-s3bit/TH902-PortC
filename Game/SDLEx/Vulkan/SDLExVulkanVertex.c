#include "SDLExVulkan.h"
#include <VkExt/vk_mem_alloc.h>
VkBuffer VulkanVertexBuffer;
VmaAllocation VulkanVertexBufferMemory;
VmaAllocator VkVma;

void * SDLExVertexBufferMemory;
size_t _lastVertexBufferSize;

VkBuffer get_vk_vertex_buffer(void) {
	return VulkanVertexBuffer;
}

void create_buffer_vma(VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer * out_buffer, VmaAllocation * out_memory) {
	if (!VkVma) {
		VmaAllocatorCreateInfo ci = { .flags = 0 };
		ci.device = get_vk_device();
		ci.physicalDevice = get_vk_physical_device();
		vmaCreateAllocator(&ci, &VkVma);
	}	VkBufferCreateInfo bufferInfo = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	int ret;
	VmaAllocationCreateInfo aci = { .flags = 0 };
	aci.flags = VMA_ALLOCATION_CREATE_STRATEGY_BEST_FIT_BIT;
	aci.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	
	ret = vmaCreateBuffer(VkVma, &bufferInfo, &aci, out_buffer, out_memory, NULL);
	if (ret != VK_SUCCESS) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"Failed to Create Buffer: vmaCreateBuffer returns %d", ret);
		return;
	}

	SDL_Log("Created Buffer at %u with VMAVRAM at %u",
		(unsigned)*out_buffer,
		(unsigned)*out_memory);
}

void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer * out_buffer, VkDeviceMemory * out_memory) {
	VkBufferCreateInfo bufferInfo = { .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	int ret;
	ret = vkCreateBuffer(get_vk_device(), &bufferInfo, NULL, out_buffer);
	if (ret != VK_SUCCESS) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"Failed to Create Buffer: vkCreateBuffer returns %d", ret);
		return;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(get_vk_device(), *out_buffer, &memRequirements);
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(get_vk_physical_device(), &memProperties);

	unsigned typeFilter = memRequirements.memoryTypeBits;

	unsigned found = SDL_MAX_UINT32;
	for (unsigned i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			found = i;
			break;
		}
	}

	if (found == SDL_MAX_UINT32) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Find Suitable Memory Type!");
		return;
	}

	VkMemoryAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = found;
	
	ret = vkAllocateMemory(get_vk_device(), &allocInfo, NULL, out_memory);
	if (ret != VK_SUCCESS) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"Failed to Allocate Buffer Memory: vkAllocateMemory returns %d", ret);
		return;
	}

	vkBindBufferMemory(get_vk_device(), *out_buffer, *out_memory, 0);

	SDL_Log("Created Buffer at %u with VRAM at %u",
		(unsigned)*out_buffer,
		(unsigned)*out_memory);
}

void recreate_vertex_buffer(unsigned nVertices) {
	if (_lastVertexBufferSize >= sizeof(Vertex) * nVertices)
		return;
	cleanup_vertex_buffer();
	create_vertex_buffer(nVertices);
}

void create_vertex_buffer(unsigned nVertices) {
	_lastVertexBufferSize = sizeof(Vertex) * nVertices;
	create_buffer_vma(
		sizeof(Vertex) * nVertices,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		&VulkanVertexBuffer,
		&VulkanVertexBufferMemory
	);
}

void * request_vertex_buffer_memory(void) {
	vmaMapMemory(VkVma, VulkanVertexBufferMemory, &SDLExVertexBufferMemory);
	return SDLExVertexBufferMemory;
}

void flush_vertex_buffer_memory(void) {
	vmaUnmapMemory(VkVma, VulkanVertexBufferMemory);
}

void cleanup_vertex_buffer(void) {
	if (VulkanVertexBuffer == VK_NULL_HANDLE)
		return;
	vkDestroyBuffer(get_vk_device(), VulkanVertexBuffer, NULL);
	vmaFreeMemory(VkVma, VulkanVertexBufferMemory);
	VulkanVertexBuffer = VK_NULL_HANDLE;
}
