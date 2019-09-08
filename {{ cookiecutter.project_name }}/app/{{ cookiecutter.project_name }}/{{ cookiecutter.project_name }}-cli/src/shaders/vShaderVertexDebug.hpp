#pragma once

#include <string>

static const std::string kVertexDebugBuffer = R"VERTEX(
#version 400
#line 5
#pragma shader_stage(vertex)
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (std140, binding = 0) uniform bufferVals {
    mat4 mvp;
} myBufferVals;
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;
layout (location = 0) out vec4 outColor;
void main() {
   outColor = inColor;
   gl_Position = myBufferVals.mvp * pos;
}
)VERTEX";


// static const char *kVertexDebugBuffer =
//     "#version 400\n"
//     "#extension GL_ARB_separate_shader_objects : enable\n"
//     "#extension GL_ARB_shading_language_420pack : enable\n"
//     "layout (std140, binding = 0) uniform bufferVals {\n"
//     "    mat4 mvp;\n"
//     "} myBufferVals;\n"
//     "layout (location = 0) in vec4 pos;\n"
//     "layout (location = 1) in vec4 inColor;\n"
//     "layout (location = 0) out vec4 outColor;\n"
//     "void main() {\n"
//     "   outColor = inColor;\n"
//     "   gl_Position = myBufferVals.mvp * pos;\n"
//     "}\n";
