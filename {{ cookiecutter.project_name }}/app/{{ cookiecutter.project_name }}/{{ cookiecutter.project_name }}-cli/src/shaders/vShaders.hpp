#pragma once

#include <vulkan/vulkan.hpp>

#include <string>
#include <unordered_map>

using GeneratedModules = std::unordered_map< std::string, vk::UniqueShaderModule >;

GeneratedModules loadShaders( const vk::Device& device );
