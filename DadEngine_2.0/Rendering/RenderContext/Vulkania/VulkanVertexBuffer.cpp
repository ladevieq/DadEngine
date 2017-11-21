#include "VulkanVertexBuffer.hpp"

namespace DadEngine::Rendering
{
	VulkanVertexBuffer::VulkanVertexBuffer(uint32 _InVertexCount, TArray<VertexInput>& _InVerticesInputLayout, TArray<float>& _InData, uint32 _InStride, VkDevice _InDevice, VkPhysicalDevice _InPhysicalDevice)
		: VertexBuffer(_InVertexCount, _InVerticesInputLayout, _InData, _InStride)
	{
		size_t BufferSize = _InData.Size() * sizeof(_InData[0U]);

		VkBufferCreateInfo buffer_create_info = {};
		buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.pNext = VK_NULL_HANDLE;
		buffer_create_info.queueFamilyIndexCount = 0U;
		buffer_create_info.pQueueFamilyIndices = VK_NULL_HANDLE;
		buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_create_info.size = (uint64)BufferSize;
		buffer_create_info.flags = 0U;

		VK_CHECK_RESULT(vkCreateBuffer(_InDevice, &buffer_create_info, VK_NULL_HANDLE, &m_buffer));


		VulkanHelper::AllocateBufferMemory(_InDevice, _InPhysicalDevice, m_buffer, m_memory);

		VK_CHECK_RESULT(vkBindBufferMemory(_InDevice, m_buffer, m_memory, 0U));

		// Upload data to the buffers memory
		void* device_memory = nullptr;
		VK_CHECK_RESULT(vkMapMemory(_InDevice, m_memory, 0U, BufferSize, 0U, &device_memory));

		MemoryManager::Copy(_InData.GetData(), device_memory, BufferSize);

		VkMappedMemoryRange mapped_memory_range = {};
		mapped_memory_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mapped_memory_range.pNext = VK_NULL_HANDLE;
		mapped_memory_range.memory = m_memory;
		mapped_memory_range.size = VK_WHOLE_SIZE;
		mapped_memory_range.offset = 0U;

		VK_CHECK_RESULT(vkFlushMappedMemoryRanges(_InDevice, 1U, &mapped_memory_range));
		vkUnmapMemory(_InDevice, m_memory);
	}
}