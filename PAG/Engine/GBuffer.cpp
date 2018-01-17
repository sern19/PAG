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
    std::vector<GLenum> drawBuffers;
    GLenum status;
    int i;
    
    //Generowanie framebuffera
    glGenFramebuffers(1, &mFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);

    mFramebufferTextures.resize(GBUFFER_TEXTURE_COUNT);
    createTextures(pScreenWidth, pScreenHeight);
    
    //Włączenie zapisu do tekstur
    drawBuffers.resize(4);
    for (i=0; i<drawBuffers.size(); i++)
        drawBuffers[i]=GL_COLOR_ATTACHMENT0+i;
    glDrawBuffers(drawBuffers.size(), &drawBuffers.at(0));
    
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
    
    glGenTextures(mFramebufferTextures.size(), &mFramebufferTextures.at(0));
    glGenTextures(1, &mDepthTexture);
    
    //Inicjalizacja tekstur dla framebuffera
    for (i=0; i<mFramebufferTextures.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, mFramebufferTextures[i]);       
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pScreenWidth, pScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mFramebufferTextures[i], 0);
    }
    
    //Tekstura głębokości
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, pScreenWidth, pScreenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
}

void GBuffer::bindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFramebuffer);
}

void GBuffer::bindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFramebuffer);
}

void GBuffer::setReadBuffer(const unsigned int& textureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}
