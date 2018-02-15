// GBuffer.hpp
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

#ifndef GBuffer_hpp
#define GBuffer_hpp

#include <stdio.h>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GBuffer
{
private:
    GLuint mFramebuffer;
    std::vector<GLuint> mFramebufferTextures;
    GLuint mDepthTexture;
    std::vector<GLuint> mFinalTexture;
    
    bool mTextureSwaper=false;
    
    void createFramebuffer(const int& pScreenWidth, const int& pScreenHeight);
    void createTextures(const int& pScreenWidth, const int& pScreenHeight);
public:
    GBuffer(const int& pScreenWidth, const int& pScreenHeight);
    
    void clearFinalTexture();
    void bindForWritingGeometryPass();
    void bindForStencilPass();
    void bindForReading();
    void bindForLightPass();
    void bindForPostProcess();
    void bindForFinalPass();
    void setReadBuffer(const unsigned int& textureType);
};

#endif /* GBuffer_hpp */
