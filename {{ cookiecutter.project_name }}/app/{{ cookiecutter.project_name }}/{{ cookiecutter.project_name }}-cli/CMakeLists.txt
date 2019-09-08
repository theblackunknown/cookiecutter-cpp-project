cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project({{ cookiecutter.project_name }}-cli
        VERSION {{ cookiecutter.version }}
        LANGUAGES CXX)

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.14)
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "/external:I ")
endif()

find_package(Vulkan
    REQUIRED
)

find_package(Boost
    REQUIRED
    COMPONENTS
        program_options
)

find_package(glog
    REQUIRED
    CONFIG
)

add_executable({{ cookiecutter.project_name }}-cli)

# include(GenerateExportHeader)
# generate_export_header({{ cookiecutter.project_name }}-cli
#     BASE_NAME {{ cookiecutter.project_name }}-cli
#     EXPORT_MACRO_NAME {{ cookiecutter.project_name }}_CLI_API
# )

target_sources({{ cookiecutter.project_name }}-cli
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/{{ cookiecutter.project_name }}-cli.cpp"
        # "$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_LIST_DIR}/src/dllmain.cpp>"
        # "${CMAKE_CURRENT_BINARY_DIR}/{{ cookiecutter.project_name }}-cli_export.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/vLogger.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vLogger.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vApplication.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vApplication.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vPhysicalDevice.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vPhysicalDevice.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vQueueFamily.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vQueueFamily.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vCompilers.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/vUtilities.hpp"
        # "${CMAKE_CURRENT_LIST_DIR}/include/vkToolset.hpp"
        # "${CMAKE_CURRENT_LIST_DIR}/src/shaders/vShaders.cpp"
        # "${CMAKE_CURRENT_LIST_DIR}/src/shaders/vShaders.hpp"
        # "${CMAKE_CURRENT_LIST_DIR}/src/shaders/vShaderVertexDebug.hpp"
        # "${CMAKE_CURRENT_LIST_DIR}/src/vkToolset.cpp"
)

# Add project includes
target_include_directories({{ cookiecutter.project_name }}-cli
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}/src"
        "${CMAKE_CURRENT_LIST_DIR}/include"
)

message("CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
target_compile_options({{ cookiecutter.project_name }}-cli
    PRIVATE
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},19.14>>:
            /experimental:external # enable external headers features
            /external:W0           # disable warning reports in external headers
        >
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<PLATFORM_ID:Windows>,$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},19.20>>:
            # Flag Compiler header as external to avoid warning reports
            /external:I "D:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Preview\\VC\\Tools\\MSVC\\14.20.27508\\include"
        >
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<PLATFORM_ID:Windows>,$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},19.14>,$<VERSION_LESS:${CMAKE_CXX_COMPILER_VERSION},19.15>>:
            # Flag Compiler header as external to avoid warning reports
            /external:I "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Tools\\MSVC\\14.16.27023\\include"
        >
)

target_compile_options({{ cookiecutter.project_name }}-cli
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
            /permissive-   # disable some nonconforming code to compile (feature set subject to change)
            /Za            # disable extensions
            /Wall          # enable all warnings
            /WX            # treat warnings as errors
            /sdl           # Enable Additional Security Checks
            /GS            # Buffer Security Check
            /wd5045        # warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
            /wd4514        # warning C4514: 'logging::ScopedLoggingService::~ScopedLoggingService': unreferenced inline function has been removed
            /wd4191        # warning C4191: 'type cast': unsafe conversion from 'PFN_vkVoidFunction' to 'PFN_vkCreateDebugReportCallbackEXT'
        >
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:
            /JMC                    # enable to skip non-user code
        >
)

# Define project compiler requirements
target_compile_definitions({{ cookiecutter.project_name }}-cli
    PRIVATE
        VK_EXT_debug_report
        $<$<CONFIG:Release>:
            NDEBUG
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            _CRT_SECURE_NO_WARNINGS # silent warnings about using strcpy (OS-specific safe versions are suggested instead)
            STRICT                  # Strict Win32 API
            WIN32_LEAN_AND_MEAN     # exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets
            NOGDICAPMASKS           # CC_*, LC_*, PC_*, CP_*, TC_*, RC_
            NOVIRTUALKEYCODES       # VK_*
            NOWINMESSAGES           # WM_*, EM_*, LB_*, CB_*
            NOWINSTYLES             # WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
            NOSYSMETRICS            # SM_*
            NOMENUS                 # MF_*
            NOICONS                 # IDI_*
            NOKEYSTATES             # MK_*
            NOSYSCOMMANDS           # SC_*
            NORASTEROPS             # Binary and Tertiary raster ops
            NOSHOWWINDOW            # SW_*
            OEMRESOURCE             # OEM Resource values
            NOATOM                  # Atom Manager routines
            NOCLIPBOARD             # Clipboard routines
            NOCOLOR                 # Screen colors
            NOCTLMGR                # Control and Dialog routines
            NODRAWTEXT              # DrawText() and DT_*
            NOGDI                   # All GDI defines and routines
            NOKERNEL                # All KERNEL defines and routines
            NOUSER                  # All USER defines and routines
            NONLS                   # All NLS defines and routines
            NOMB                    # MB_* and MessageBox()
            NOMEMMGR                # GMEM_*, LMEM_*, GHND, LHND, associated routines
            NOMETAFILE              # typedef METAFILEPICT
            NOMINMAX                # Macros min(a,b) and max(a,b)
            NOMSG                   # typedef MSG and associated routines
            NOOPENFILE              # OpenFile(), OemToAnsi, AnsiToOem, and OF_*
            NOSCROLL                # SB_* and scrolling routines
            NOSERVICE               # All Service Controller routines, SERVICE_ equates, etc.
            NOSOUND                 # Sound driver routines
            NOTEXTMETRIC            # typedef TEXTMETRIC and associated routines
            NOWH                    # SetWindowsHook and WH_*
            NOWINOFFSETS            # GWL_*, GCL_*, associated routines
            NOCOMM                  # COMM driver routines
            NOKANJI                 # Kanji support stuff.
            NOHELP                  # Help engine interface.
            NOPROFILER              # Profiler interface.
            NODEFERWINDOWPOS        # DeferWindowPos routines
            NOMCX                   # Modem Configuration Extensions
        >
)

# Define cxx language requirements
target_compile_features({{ cookiecutter.project_name }}-cli
    PUBLIC
        cxx_std_17
)

target_link_libraries({{ cookiecutter.project_name }}-cli
    PRIVATE
        Boost::boost
        Boost::program_options
        glog::glog
        Vulkan::Vulkan
        # FIXME(amachizaud) shaderc does not play by CMake rules...
        # shaderc::shaderc_$<IF:$<BOOL:${BUILD_SHARED_LIBS}>,shared,static>
)

add_custom_command(
    TARGET
        {{ cookiecutter.project_name }}-cli
    POST_BUILD
    COMMAND
        "${CMAKE_COMMAND}" -E copy_if_different "$ENV{VULKAN_SDK}/Config/vk_layer_settings.txt" "$<TARGET_FILE_DIR:{{ cookiecutter.project_name }}-cli>/vk_layer_settings.txt"
    COMMENT
        "Copying default vk_layer_settings.txt from Vulkan SDK"
    VERBATIM
)

install(
    TARGETS  {{ cookiecutter.project_name }}-cli
    EXPORT   {{ cookiecutter.project_name }}Targets
    ARCHIVE  DESTINATION "lib"
    LIBRARY  DESTINATION "lib"
    RUNTIME  DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    DIRECTORY      "${CMAKE_CURRENT_LIST_DIR}/include/"
    DESTINATION    "include"
    FILES_MATCHING PATTERN "*.hpp"
)

# Export project configuration for other projects
install(
    EXPORT       {{ cookiecutter.project_name }}Targets
    NAMESPACE    "{{ cookiecutter.project_name }}::"
    DESTINATION  "lib"
)

# Export project configuration for this project
export(
    EXPORT    {{ cookiecutter.project_name }}Targets
    NAMESPACE "{{ cookiecutter.project_name }}::"
)
