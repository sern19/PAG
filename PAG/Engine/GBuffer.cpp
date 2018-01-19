// GBuffer.cpp
//
// Copyright (c) 2018 Krystian Owoc
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "GBuffer.hpp"
#include "Config.hpp"

#include <stdexcept>

GBuffer::GBuffer(const int& pScreenWidth, const int& pScreenHeight)
{
    createFramebuffer(pScreenWidth, pScreenHeight);
}

void GBuffer::createFramebuffer(const int& pScreenWidth, const int& pScreenHeight)
{
    GLenum status;
    
    //Generowanie framebuffera
    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);

    mFramebufferTextures.resize(GBUFFER_TEXTURE_COUNT);
    mFinalTexture.resize(2);
    createTextures(pScreenWidth, pScreenHeight);
    
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("(GBuffer::createFramebuffer): Nie można utworzyć framebuffera");
    }
    
    //Przywrócenie domyślnego Framebuffera
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::createTextures(const int& pScreenWidth, const int& pScreenHeight)
{
    int i;
    glGenTextures((int)mFramebufferTextures.size(), &mFramebufferTextures.at(0));
    glGenTextures(1, &mDepthTexture);
    glGenTextures((int)mFinalTexture.size(), &mFinalTexture.at(0));
    
    //Inicjalizacja tekstur dla framebuffera
    for (i=0; i<mFramebufferTextures.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, mFramebufferTextures[i]);       
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, pScreenWidth, pScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mFramebufferTextures[i], 0);
    }
    
    //Tekstura głębokości
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, pScreenWidth, pScreenHeight, 0, GL_DEPTH_STENCIL, 
                 GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
    
    for (i=0; i<mFinalTexture.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, mFinalTexture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pScreenWidth, pScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+(int)mFramebufferTextures.size()+i, GL_TEXTURE_2D, mFinalTexture[i], 0);   
    }
}

void GBuffer::clearFinalTexture()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0+(int)mFramebufferTextures.size());
    glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::bindForWritingGeometryPass()
{
    int i;
    std::vector<GLenum> drawBuffers(mFramebufferTextures.size());
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
    
    //Włączenie zapisu do tekstur
    for (i=0; i<(int)drawBuffers.size(); i++)
        drawBuffers[i]=GL_COLOR_ATTACHMENT0+i;
    glDrawBuffers((int)drawBuffers.size(), &drawBuffers.at(0));
}

void GBuffer::bindForStencilPass()
{
    glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass()
{
    int i;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0+(int)mFramebufferTextures.size());
    for (i=0; i<mFramebufferTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, mFramebufferTextures[i]);
    }
}

void GBuffer::bindForPostProcess()
{
    mTextureSwaper=!mTextureSwaper;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0+(int)mFramebufferTextures.size()+(int)mTextureSwaper);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFinalTexture[(int)!mTextureSwaper]);
}

void GBuffer::bindForFinalPass()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
    glReadBuffer(GL_COLOR_ATTACHMENT0+(int)mFramebufferTextures.size()+(int)mTextureSwaper);
    mTextureSwaper=false;
}

void GBuffer::setReadBuffer(const unsigned int& textureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}
