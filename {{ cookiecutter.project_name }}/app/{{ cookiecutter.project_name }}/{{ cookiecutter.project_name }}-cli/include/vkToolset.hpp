#pragma once

#include <vCompilers.hpp>
#include <vkViewer_export.h>

class vkViewer_API vkToolsetApplication
{
public:
    vkToolsetApplication();
    vkToolsetApplication( vkToolsetApplication&& ) = delete;
    vkToolsetApplication( const vkToolsetApplication& ) = delete;
    vkToolsetApplication& operator=( vkToolsetApplication&& ) = delete;
    vkToolsetApplication& operator=( const vkToolsetApplication& ) = delete;
    ~vkToolsetApplication();
private:
    struct Implementation;
    Implementation* mImplementation;
};
