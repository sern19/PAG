// UserInterface.cpp
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

#include "UserInterface.hpp"
#include "Node.hpp"
#include "Transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <algorithm>
#include <iterator>

void UserInterface::updateTransform()
{
	glm::vec3 temporaryValues;
	//Position
	temporaryValues.x=mTranslateX;
	temporaryValues.y=mTranslateY;
	temporaryValues.z=mTranslateZ;
	mSelectedTransform->setPosition(temporaryValues);
	//Scale
	temporaryValues.x=mScaleX;
	temporaryValues.y=mScaleY;
	temporaryValues.z=mScaleZ;
	mSelectedTransform->setScale(temporaryValues);
	//Rotation
	temporaryValues.x=mRotationAxisX;
	temporaryValues.y=mRotationAxisY;
	temporaryValues.z=mRotationAxisZ;
	mSelectedTransform->setRotation(temporaryValues, mRotationAngle);
}

UserInterface::UserInterface(GLFWwindow* const pWindow)
{
    ImGui_ImplGlfwGL3_Init(pWindow, true);
}

void UserInterface::draw()
{
    ImGui_ImplGlfwGL3_NewFrame();
    
    if (mShouldShowInterface)
    {
		if (mSelectedNode != NULL)
		{
			ImGui::Begin("Selected transform");
			ImGui::Text("Transform level %i", mSelectedTransformLevel);
			if (ImGui::Button("Select parent node")) if (mSelectedNode->getParentNode()!=NULL) setSelectedNode(mSelectedNode->getParentNode());
			if (mSelectedTransform != NULL)
			{
				if (mSelectedTransform->getChildrensCount() == 0)
				{
					if (ImGui::Button("Add transform child")) mSelectedTransform->pushChildren();
				}
				else
				{
					//if (ImGui::Button("Select"))
				}
			}

			ImGui::Text("Position");
			ImGui::InputFloat(":x", &mTranslateX, 0.01f, 1.0f);
			ImGui::InputFloat(":y", &mTranslateY, 0.01f, 1.0f);
			ImGui::InputFloat(":z", &mTranslateZ, 0.01f, 1.0f);
			ImGui::Text("Scale");
			ImGui::InputFloat(":x", &mScaleX, 0.01f, 1.0f);
			ImGui::InputFloat(":y", &mScaleY, 0.01f, 1.0f);
			ImGui::InputFloat(":z", &mScaleZ, 0.01f, 1.0f);
			ImGui::Text("Rotate");
			ImGui::InputFloat(":angle", &mRotationAngle, 0.01f, 1.0f);
			ImGui::InputFloat(":x", &mRotationAxisX, 0.01f, 1.0f);
			ImGui::InputFloat(":y", &mRotationAxisY, 0.01f, 1.0f);
			ImGui::InputFloat(":z", &mRotationAxisZ, 0.01f, 1.0f);
			if (ImGui::Button("Update transform")) if (mSelectedTransform != NULL) updateTransform();
			ImGui::End();
		}
    }
    
    // ImGui functions end here
    ImGui::Render();
    
}

void UserInterface::setShouldShowInterface(const bool& pShouldShowInterface) { mShouldShowInterface=pShouldShowInterface; }

void UserInterface::setSelectedNode(Node * const pSelectedNode)
{
	if (mSelectedNode!=NULL) mSelectedNode->setIsSelected(false);
	mSelectedNode = pSelectedNode;
	mSelectedTransform=NULL;
	if (mSelectedNode!=NULL)
	{
		mSelectedNode->setIsSelected(true);
		mSelectedTransform=mSelectedNode->getNodeTransform();
	}
	if (mSelectedTransform!=NULL)
	{
		mTranslateX=mSelectedTransform->getPosition().x;
		mTranslateY=mSelectedTransform->getPosition().y;
		mTranslateZ=mSelectedTransform->getPosition().z;
		mScaleX=mSelectedTransform->getScale().x;
		mScaleY=mSelectedTransform->getScale().y;
		mScaleZ=mSelectedTransform->getScale().z;
		mRotationAngle=mSelectedTransform->getRotation().second;
		mRotationAxisX=mSelectedTransform->getRotation().first.x;
		mRotationAxisY=mSelectedTransform->getRotation().first.y;
		mRotationAxisZ=mSelectedTransform->getRotation().first.z;
	}
}
