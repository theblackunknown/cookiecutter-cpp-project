#include "vShaders.hpp"

#include "vShaderVertexDebug.hpp"

#include <vUtilities.hpp>

#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.hpp>

#include <glog/logging.h>

GeneratedModules loadShaders( const vk::Device& device )
{
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    options.AddMacroDefinition( "author", "theblackunknown" );

    constexpr const shaderc_optimization_level optimization{
#ifdef NDEBUG
        shaderc_optimization_level_size
#else
        shaderc_optimization_level_zero
#endif
    };

    options.SetOptimizationLevel( optimization );

#ifndef NDEBUG
    options.SetGenerateDebugInfo( );
#endif

    using ShaderData = std::tuple<const char*, std::string>;
    // using ShaderData = std::tuple<const char*, const char*, std::size_t>;

    auto shaders = make_array<ShaderData>(
        ShaderData{ "vHelloWorld-Vertex", kVertexDebugBuffer }
        // ShaderData{ "vHelloWorld-Vertex", kVertexDebugBuffer, std::strlen( kVertexDebugBuffer ) }
    );

    GeneratedModules modules;

    for( auto&& shaderData : shaders )
    {
        const std::string& source( std::get<1>( shaderData ) );
        auto compilation{ compiler.CompileGlslToSpv(
            source.c_str( ),
            source.size( ),
            shaderc_glsl_infer_from_source,
            std::get<0>( shaderData ),
            options
        ) };

        CHECK_EQ( shaderc_compilation_status_success, compilation.GetCompilationStatus( ) ) << compilation.GetErrorMessage();

        const uint32_t* pBuffer = compilation.cbegin( );
        const size_t bufferSize = static_cast<size_t>( compilation.cend( ) - compilation.cbegin( ) ) * sizeof(uint32_t);
        vk::ShaderModuleCreateInfo iShaderModule( {}, bufferSize, pBuffer );

        modules.emplace( std::get<0>( shaderData ), device.createShaderModuleUnique( iShaderModule ) );
    }

    return modules;
}
