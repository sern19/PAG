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

#include "BaseLight.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"

#include <algorithm>
#include <iterator>
#include <typeinfo>


UserInterface::UserInterface(GLFWwindow* const pWindow)
{
    ImGui_ImplGlfwGL3_Init(pWindow, true);
}

void UserInterface::updateUITransformData()
{
    if (mSelectedTransform!=NULL)
    {
        mTranslateX=mSelectedTransform->getPosition().x;
        mTranslateY=mSelectedTransform->getPosition().y;
        mTranslateZ=mSelectedTransform->getPosition().z;
        mScaleX=mSelectedTransform->getScale().x;
        mScaleY=mSelectedTransform->getScale().y;
        mScaleZ=mSelectedTransform->getScale().z;
        mRotationX=mSelectedTransform->getRotation().x;
        mRotationY=mSelectedTransform->getRotation().y;
        mRotationZ=mSelectedTransform->getRotation().z;
    }
}

void UserInterface::updateUILightData()
{
	mLightX=mSelectedLight->getLightPos().x;
	mLightY=mSelectedLight->getLightPos().y;
	mLightZ=mSelectedLight->getLightPos().z;
    mLightR=mSelectedLight->getLightColor().x;
    mLightG=mSelectedLight->getLightColor().y;
    mLightB=mSelectedLight->getLightColor().z;
    mLightAttenuation=mSelectedLight->getLightAttenuation();
    mLightAmbient=mSelectedLight->getLightAmbientCoefficient();
    if (dynamic_cast<SpotLight*>(mSelectedLight)!=NULL)
    {
        mLightConeX=((SpotLight*)mSelectedLight)->getConeDirection().x;
        mLightConeY=((SpotLight*)mSelectedLight)->getConeDirection().y;
        mLightConeZ=((SpotLight*)mSelectedLight)->getConeDirection().z;
        mLightConeAngle=((SpotLight*)mSelectedLight)->getConeAngle();
    }
    
}

void UserInterface::updateLight()
{
    glm::vec3 temporaryValues;
    //Position
    temporaryValues.x=mLightX;
    temporaryValues.y=mLightY;
    temporaryValues.z=mLightZ;
    mSelectedLight->setLightPos(temporaryValues);
    //Color
    temporaryValues.x=mLightR;
    temporaryValues.y=mLightG;
    temporaryValues.z=mLightB;
    mSelectedLight->setLightColor(temporaryValues);
    mSelectedLight->setLightAttenuation(mLightAttenuation);
    mSelectedLight->setLightAmbientCoefficient(mLightAmbient);
    if (dynamic_cast<SpotLight*>(mSelectedLight)!=NULL)
    {
        temporaryValues.x=mLightConeX;
        temporaryValues.y=mLightConeY;
        temporaryValues.z=mLightConeZ;
        ((SpotLight*)mSelectedLight)->setConeDirection(temporaryValues);
        ((SpotLight*)mSelectedLight)->setConeAngle(mLightConeAngle);
    }
}

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
	temporaryValues.x=mRotationX;
	temporaryValues.y=mRotationY;
	temporaryValues.z=mRotationZ;
	mSelectedTransform->setRotation(temporaryValues);
}

void UserInterface::drawTransformUI()
{
	static bool* temporaryBool = NULL; //Fix dla pokazującego się przyciusku X
    //Przyciski selekcji modyfikowanego obiektu/informacje
    ImGui::SetNextWindowPos(ImVec2(0, mMainMenuHeight));
    ImGui::SetNextWindowSize(ImVec2(240, 480));
    ImGui::Begin("Selected node", temporaryBool, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("  Node level %i", mSelectedNode->getNodeLevel());
    ImGui::Text("  Transform level %i", mSelectedTransform->getTransformLevel());
    if (mSelectedNode->getParentNode() != NULL && mSelectedNode->getNodeLevel()>0 && ImGui::Button("Select parent node"))setSelectedNode(mSelectedNode->getParentNode());
    if (mSelectedTransform != NULL)
    {
        if (!mSelectedTransform->getChildren())
        {
            if (ImGui::Button("Add transform child")) mSelectedTransform->addChildren();
        }
        else
        {
            if (ImGui::Button("Select transform child"))
            {
                mSelectedTransform = mSelectedTransform->getChildren();
                updateUITransformData();
            }
            if (ImGui::Button("Delete transform child"))
            {
                mSelectedTransform->removeChildren();
            }
        }
        if (mSelectedTransform->getParent() != NULL && ImGui::Button("Select parent transform"))
        {
            mSelectedTransform = mSelectedTransform->getParent();
            updateUITransformData();
        }
    }

    //Pola z danymi
    ImGui::Text("Position");
    ImGui::PushID(0); //Imgui używa nazw pól jako id
    ImGui::InputFloat(":x", &mTranslateX, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":y", &mTranslateY, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":z", &mTranslateZ, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopID();
    ImGui::Text("Scale");
    ImGui::PushID(1); //Więc aby dalsze przyciski były responsywne
    ImGui::InputFloat(":x", &mScaleX, 0.001f, 0.1f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":y", &mScaleY, 0.001f, 0.1f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":z", &mScaleZ, 0.001f, 0.1f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopID();
    ImGui::Text("Rotate");
    ImGui::PushID(2); //Używamy PushID
    ImGui::InputFloat(":x", &mRotationX, 0.1f, 1.0f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":y", &mRotationY, 0.1f, 1.0f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":z", &mRotationZ, 0.1f, 1.0f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopID();

    //Przyciski aktualizacji transforma
    if (mSelectedTransform != NULL && ImGui::Button("Reset transform"))
    {
        if (mSelectedTransform->getTransformLevel() == 0) mSelectedNode->resetNodeTransform();
        else mSelectedTransform->resetTransform();
        updateUITransformData();
    }
    if (mSelectedTransform != NULL && ImGui::Button("Update transform")) updateTransform();
    ImGui::End();
}

void UserInterface::drawLightUI()
{
	static bool* temporaryBool = NULL; //Fix dla pokazującego się przyciusku X
    //Przyciski selekcji modyfikowanego obiektu/informacje
    ImGui::SetNextWindowPos(ImVec2(0, mMainMenuHeight));
    ImGui::SetNextWindowSize(ImVec2(240, 480));
    ImGui::Begin("Selected light", temporaryBool, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    //ImGui::Text("  Node level %i", mSelectedNode->getNodeLevel());
    //ImGui::Text("  Transform level %i", mSelectedTransform->getTransformLevel());

    //Pola z danymi
    if (dynamic_cast<DirectionalLight*>(mSelectedLight)!=NULL) ImGui::Text("Direction");
    else ImGui::Text("Position");
    ImGui::PushID(0); //Imgui używa nazw pól jako id
    ImGui::InputFloat(":x", &mLightX, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":y", &mLightY, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":z", &mLightZ, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopID();
    ImGui::Text("Color");
    ImGui::PushID(1); //Więc aby dalsze przyciski były responsywne
    ImGui::InputFloat(":r", &mLightR, 0.001f, 0.1f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":g", &mLightG, 0.001f, 0.1f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":b", &mLightB, 0.001f, 0.1f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopID();
    ImGui::PushID(2); //Używamy PushID
    if (dynamic_cast<DirectionalLight*>(mSelectedLight)==NULL) ImGui::InputFloat(":Attenuation", &mLightAttenuation, 0.1f, 1.0f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputFloat(":Ambient", &mLightAmbient, 0.1f, 1.0f, 3, ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopID();
    if (dynamic_cast<SpotLight*>(mSelectedLight)!=NULL)
    {
        ImGui::Text("Cone direction");
        ImGui::PushID(3);
        ImGui::InputFloat(":x", &mLightConeX, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputFloat(":y", &mLightConeY, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputFloat(":z", &mLightConeZ, 0.01f, 1, 3, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputFloat(":Cone Angle", &mLightConeAngle, 0.1f, 1.0f, 3, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopID();
    }

    if (mSelectedLight != NULL && ImGui::Button("Update light")) updateLight();
    ImGui::End();
}

void UserInterface::draw()
{    
    ImGui_ImplGlfwGL3_NewFrame();
    if (mShouldShowInterface)
    {
        ImGui::BeginMainMenuBar();
        ImGui::Text("Edit mode - press 'T' to enter explore mode");
        mMainMenuHeight=ImGui::GetWindowSize().y;
        ImGui::EndMainMenuBar();
        if (mSelectedTransform!=NULL) drawTransformUI();
        else if (mSelectedLight!=NULL) drawLightUI();
		else
        {
            static bool* temporaryBool = NULL; //Fix dla pokazującego się przyciusku X
            ImGui::SetNextWindowPos(ImVec2(0, mMainMenuHeight));
            ImGui::SetNextWindowSize(ImVec2(208, 48));
            ImGui::Begin("No object selected", temporaryBool, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::Text("  Press RMB to select light");
            ImGui::End();
        }
    }
    else
    {
        ImGui::BeginMainMenuBar();
        ImGui::Text("Explore mode - press 'T' to enter edit mode");
        ImGui::EndMainMenuBar();
    }
    
    // ImGui functions end here
    ImGui::Render();
    
}

void UserInterface::setShouldShowInterface(const bool& pShouldShowInterface)
{
    mShouldShowInterface=pShouldShowInterface;
    if (mSelectedNode!=NULL) mSelectedNode->setIsSelected(pShouldShowInterface);
}

void UserInterface::setSelectedNode(Node * const pSelectedNode)
{
	if (mSelectedNode!=NULL) mSelectedNode->setIsSelected(false);
	mSelectedNode = pSelectedNode;
	mSelectedTransform=NULL;
	if (mSelectedNode!=NULL)
	{
		mSelectedNode->setIsSelected(true);
		mSelectedTransform=mSelectedNode->getNodeTransform();
        updateUITransformData();
	}
}

void UserInterface::setSelectedLight(BaseLight* const pSelectedLight)
{
	mSelectedLight = pSelectedLight;
	if (pSelectedLight != NULL)
	{
		updateUILightData();
	}
}
