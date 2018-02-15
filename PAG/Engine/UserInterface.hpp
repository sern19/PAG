// UserInterface.hpp
//
// Copyright (c) 2017 Krystian Owoc
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

#ifndef UserInterface_hpp
#define UserInterface_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <GLFW/glfw3.h>

class Model;
class Node;
class BaseLight;
class Transform;
class PostProcess;

class UserInterface
{
private:
    bool mShouldShowInterface=false;
	Node* mSelectedNode=NULL;
    Model* mSelectedModel=NULL;
	BaseLight* mSelectedLight=NULL;
	Transform* mSelectedTransform=NULL;
    
	int mMainMenuHeight, mMainMenuWidth;

	//Do wyświetlania
	float mTranslateX, mTranslateY, mTranslateZ, mScaleX, mScaleY, mScaleZ, mRotationX, mRotationY, mRotationZ;
    bool mIsReflective;

	float mLightX, mLightY, mLightZ, mLightR, mLightG, mLightB, mLightAttenuation, mLightAmbient, mLightConeX, mLightConeY, mLightConeZ, mLightConeAngle;
    
    void updateUITransformData();
	void updateUILightData();
    void updateLight();
	void updateTransform();

	void drawTransformUI();
	void drawLightUI();
    void drawPostprocessUI(std::vector<PostProcess>& pPostProcess);
public:
    UserInterface(GLFWwindow* const pWindow);
    void draw(std::vector<PostProcess>& pPostProcess);
    void setShouldShowInterface(const bool& pShouldShowInterface);
    void setSelectedModel(Model* const pSelectedModel);
	void setSelectedNode(Node* const pSelectedNode);
	void setSelectedLight(BaseLight* const pSelectedLight);
};

#endif /* UserInterface_hpp */
