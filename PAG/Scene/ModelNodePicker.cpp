// ModelNodePicker.cpp
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

#include "ModelNodePicker.hpp"

#include "Scene.hpp"
#include "Model.hpp"

Node* const ModelNodePicker::pickNode(Scene *const pScene, std::vector<Model>* pModels, const std::pair<int, int>& pScreenSize, const std::pair<double, double>& pMousePos)
{
    int i;
    glm::vec4 rayStartPoint(
                            (2.0f*pMousePos.first)/pScreenSize.first-1.0f, //Normalizacja szerokości
                            1.0f-((2.0f*pMousePos.second)/pScreenSize.second), //Normalizacja wysokości
                            -1.0f, 1.0f); //-1.0 w znormalizowanej postaci to plan bliski
    glm::vec4 rayEndPoint(
                            (2.0f*pMousePos.first)/pScreenSize.first-1.0f, //Normalizacja szerokości
                            1.0f-((2.0f*pMousePos.second)/pScreenSize.second), //Normalizacja wysokości
                            1.0f, 1.0f); //0.0 w znormalizowanej postaci to plan daleki
    
    //Odwrotne transformacje przez macierze świata
    glm::mat4 inverseVP=glm::inverse(pScene->getProjectionSpace()*pScene->getViewSpace());
    
    //Obliczanie punktów w świecie
    glm::vec4 rayStartPointWorld=inverseVP*rayStartPoint;
    rayStartPointWorld/=rayStartPointWorld.w;
    glm::vec4 rayEndPointWorld=inverseVP*rayEndPoint;
    rayEndPointWorld/=rayEndPointWorld.w;
    
    //Obliczanie kierunku promienia
    glm::vec3 rayDirectionWorld(rayEndPointWorld-rayStartPointWorld);
    rayDirectionWorld=glm::normalize(rayDirectionWorld);
    
    //Obliczanie najbliższego Node
    std::vector<std::pair<Node*,float>> intersectedNodes;
    for (i=0;i<pModels->size();i++)
        intersectedNodes.push_back(pModels->at(i).testRayOBBIntersection(rayStartPointWorld, rayDirectionWorld));
    if (intersectedNodes.size()==0) return NULL;
    
    std::pair<Node*,float> closestNode=intersectedNodes[0];
    for (i=1;i<intersectedNodes.size();i++)
        if (intersectedNodes[i].second<closestNode.second) closestNode=intersectedNodes[i];
    
    return closestNode.first;
}
