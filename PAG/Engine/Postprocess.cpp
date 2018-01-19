// Postprocess.cpp
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

#include "Postprocess.hpp"

#include "Shader.hpp"
#include "ModelCreator.hpp"
#include "Model.hpp"

#include "Config.hpp"

PostProcess::PostProcess(const Shader& pShader, const bool& pIsEnabled): mShader(new Shader(pShader)), mPlane(ModelCreator::createPlane())
{
    mIsEnabled=new bool(pIsEnabled);
    mShader->useProgram();
    mShader->setInt("screenMap", 0);
    mShader->setInt("depthMap", 1);
    mShader->setVec2("screenSize",glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    
}

PostProcess::PostProcess(const PostProcess& pSourcePostProcess): PostProcess(*pSourcePostProcess.mShader)
{
    *mIsEnabled=*pSourcePostProcess.mIsEnabled;
}

PostProcess::~PostProcess()
{
    if (mShader) delete mShader;
    if (mIsEnabled) delete mIsEnabled;
}

void PostProcess::preparePostProcess()
{
    mShader->useProgram();
}

void PostProcess::applyPostProcess()
{
    preparePostProcess();
    mPlane.draw(mShader, glm::mat4(1));
}

bool* PostProcess::isEnabled() { return mIsEnabled; }


