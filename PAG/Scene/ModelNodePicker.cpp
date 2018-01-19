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
#include "BaseLight.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>

const bool ModelNodePicker::checkRayIntersectionTriangle(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const glm::vec3 pTriangle[3], float& pDistanceOutput)
{
    const float epsilon = 0.00000001;
    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v,t;
    
    //Obliczanie krawędzi
    edge1=pTriangle[1]-pTriangle[0];
    edge2=pTriangle[2]-pTriangle[0];
    h=glm::cross(pRayDirection, edge2);
    a=glm::dot(edge1, h);
    if (fabs(a)< epsilon)
        return false;
    f=1/a;
    s=pRaySource-pTriangle[0];
    u=f*glm::dot(s,h);
    if (u<0.0||u>1.0)
        return false;
    q=glm::cross(s, edge1);
    v=f*glm::dot(pRayDirection, q);
    if (v<0.0||(u+v)>1.0)
        return false;
    t=f*glm::dot(edge2,q);
    if (t>epsilon)
    {
        pDistanceOutput=glm::distance(pRaySource, pRaySource+(pRayDirection*t));
        return true;
    }
    else
        return false;
}

const bool ModelNodePicker::checkRayIntersectionOBB(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const std::pair<glm::vec4,glm::vec4>& pOBB, const glm::mat4& pTransform, float& pDistanceOutput)
{
    //Brudny fix dla braku wsparcia dla skalowania w algorytmie
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    
    glm::decompose(pTransform, scale, rotation, translation, skew, perspective);    
    
    std::pair<glm::vec3, glm::vec3> transformedOBB;
    transformedOBB.first=glm::vec3(pOBB.first);
    transformedOBB.second=glm::vec3(pOBB.second);
    transformedOBB.first.x*=scale.x;
    transformedOBB.first.y*=scale.y;
    transformedOBB.first.z*=scale.z;
    transformedOBB.second.x*=scale.x;
    transformedOBB.second.y*=scale.y;
    transformedOBB.second.z*=scale.z;
    
    
    //Inicjalizacja zmiennych
    float f,e,t1,t2;
    float tMin=0.0f;
    float tMax=FLT_MAX;
    
    //Używana do przeliczania przecięć z płaszczyznami
    glm::vec3 delta=glm::vec3(pTransform[3].x, pTransform[3].y, pTransform[3].z)-pRaySource;
    
    //Sprawdzanie z dwiema płaszczyznami równoległymi do osi x
    glm::vec3 axisX(pTransform[0].x, pTransform[0].y, pTransform[0].z);
    f=glm::dot(pRayDirection, axisX);
    e=glm::dot(axisX, delta);
    
    if (fabs(f) > 0.001f)
    {
        //Odległości między źródłem, a przecięciami płaszczyzn
        t1=(e+transformedOBB.first.x)/f; //Przecięcie z lewą płaszczyzną
        t2=(e+transformedOBB.second.x)/f; //Przecięcie z prawą płaszczyzną
        
        if (t1>t2)
            std::swap(t1,t2);
        
        if (t2<tMax) tMax=t2; //Najbliższe(bliższa płaszczyzna) dalekie przecięcie
        if (t1>tMin) tMin=t1; //Najdalsze(dalsza płaszczyzna) bliskie przecięcie
        
        //Z rysunku - jeżeli dalekie przecięcie jest bliżej niż bliskie, to nie ma przecięcia
        if (tMin>tMax)
            return false;
    } else //Gdy promień jest prawie równoległy do płaszczyzny
        if (-e+transformedOBB.first.x>0.0f || -e+transformedOBB.second.x<0.0f)
            return false;
    
    //Sprawdzanie z dwiema płaszczyznami równoległymi do osi y
    glm::vec3 axisY(pTransform[0][1], pTransform[1][1], pTransform[2][1]);
    f=glm::dot(pRayDirection, axisY);
    e=glm::dot(axisY, delta);
    
    if (fabs(f) > 0.001f)
    {
        //Odległości między źródłem, a przecięciami płaszczyzn
        t1=(e+transformedOBB.first.y)/f; //Przecięcie z lewą płaszczyzną
        t2=(e+transformedOBB.second.y)/f; //Przecięcie z prawą płaszczyzną
        
        if (t1>t2)
            std::swap(t1,t2);
        
        if (t2<tMax) tMax=t2; //Najbliższe(bliższa płaszczyzna) dalekie przecięcie
        if (t1>tMin) tMin=t1; //Najdalsze(dalsza płaszczyzna) bliskie przecięcie
        
        //Z rysunku - jeżeli dalekie przecięcie jest bliżej niż bliskie, to nie ma przecięcia
        if (tMin>tMax)
            return false;
    } else //Gdy promień jest prawie równoległy do płaszczyzny
        if (-e+transformedOBB.first.y>0.0f || -e+transformedOBB.second.y<0.0f)
            return false;
    
    //Sprawdzanie z dwiema płaszczyznami równoległymi do osi z
    glm::vec3 axisZ(pTransform[0][2], pTransform[1][2], pTransform[2][2]);
    f=glm::dot(pRayDirection, axisZ);
    e=glm::dot(axisZ, delta);
    
    if (fabs(f) > 0.001f)
    {
        //Odległości między źródłem, a przecięciami płaszczyzn
        t1=(e+transformedOBB.first.z)/f; //Przecięcie z lewą płaszczyzną
        t2=(e+transformedOBB.second.z)/f; //Przecięcie z prawą płaszczyzną
        
        if (t1>t2)
            std::swap(t1,t2);
        
        if (t2<tMax) tMax=t2; //Najbliższe(bliższa płaszczyzna) dalekie przecięcie
        if (t1>tMin) tMin=t1; //Najdalsze(dalsza płaszczyzna) bliskie przecięcie
        
        //Z rysunku - jeżeli dalekie przecięcie jest bliżej niż bliskie, to nie ma przecięcia
        if (tMin>tMax)
            return false;
    } else //Gdy promień jest prawie równoległy do płaszczyzny
        if (-e+transformedOBB.first.z>0.0f || -e+transformedOBB.second.z<0.0f)
            return false;
    
    pDistanceOutput=tMin;
    return true;
}

std::pair<BaseLight*, float> const ModelNodePicker::pickBaseLight(Model * pModel, Scene *const pScene, std::vector<BaseLight*>* pLights, const std::pair<int, int>& pScreenSize, const std::pair<double, double>& pMousePos)
{
	int i;
	glm::vec4 rayStartPoint(
		(2.0f*pMousePos.first) / pScreenSize.first - 1.0f, //Normalizacja szerokości
		1.0f - ((2.0f*pMousePos.second) / pScreenSize.second), //Normalizacja wysokości
		-1.0f, 1.0f); //-1.0 w znormalizowanej postaci to plan bliski
	glm::vec4 rayEndPoint(
		(2.0f*pMousePos.first) / pScreenSize.first - 1.0f, //Normalizacja szerokości
		1.0f - ((2.0f*pMousePos.second) / pScreenSize.second), //Normalizacja wysokości
		1.0f, 1.0f); //1.0 w znormalizowanej postaci to plan daleki

					 //Odwrotne transformacje przez macierze świata
	glm::mat4 inverseVP = glm::inverse(pScene->getProjectionSpace()*pScene->getViewSpace());

	//Obliczanie punktów w świecie
	glm::vec4 rayStartPointWorld = inverseVP*rayStartPoint;
	rayStartPointWorld /= rayStartPointWorld.w;
	glm::vec4 rayEndPointWorld = inverseVP*rayEndPoint;
	rayEndPointWorld /= rayEndPointWorld.w;

	//Obliczanie kierunku promienia
	glm::vec3 rayDirectionWorld(rayEndPointWorld - rayStartPointWorld);
	rayDirectionWorld = glm::normalize(rayDirectionWorld);

	//Obliczanie najbliższego Node
	std::vector<std::pair<BaseLight*, float>> intersectedLights;
	for (i = 0;i<pLights->size();i++)
	{
		intersectedLights.push_back(pLights->at(i)->testRay(pModel, rayStartPointWorld, rayDirectionWorld));
	}
	if (intersectedLights.size() == 0) return std::pair<BaseLight*, float>(NULL, FLT_MAX);

	std::pair<BaseLight*, float> closestLight = intersectedLights[0];
	for (i = 1;i<intersectedLights.size();i++)
		if (intersectedLights[i].second<closestLight.second) closestLight = intersectedLights[i];

	return closestLight;
}

std::pair<Node*, float> const ModelNodePicker::pickNode(Scene *const pScene, std::vector<Model>* pModels, const std::pair<int, int>& pScreenSize, const std::pair<double, double>& pMousePos)
{
    int i;
    glm::vec4 rayStartPoint(
                            (2.0f*pMousePos.first)/pScreenSize.first-1.0f, //Normalizacja szerokości
                            1.0f-((2.0f*pMousePos.second)/pScreenSize.second), //Normalizacja wysokości
                            -1.0f, 1.0f); //-1.0 w znormalizowanej postaci to plan bliski
    glm::vec4 rayEndPoint(
                            (2.0f*pMousePos.first)/pScreenSize.first-1.0f, //Normalizacja szerokości
                            1.0f-((2.0f*pMousePos.second)/pScreenSize.second), //Normalizacja wysokości
                            1.0f, 1.0f); //1.0 w znormalizowanej postaci to plan daleki
    
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
    {
        intersectedNodes.push_back(pModels->at(i).testRayOBBIntersection(rayStartPointWorld, rayDirectionWorld));
    }
    if (intersectedNodes.size()==0) return std::pair<Node*, float>(NULL, FLT_MAX);
    
    std::pair<Node*,float> closestNode=intersectedNodes[0];
    for (i=1;i<intersectedNodes.size();i++)
        if (intersectedNodes[i].second<closestNode.second) closestNode=intersectedNodes[i];
    
    return closestNode;
}
