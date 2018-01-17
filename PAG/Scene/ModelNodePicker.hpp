// ModelNodePicker.hpp
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

#ifndef ModelNodePicker_hpp
#define ModelNodePicker_hpp

#define GLFW_INCLUDE_NONE

#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Node;
class Scene;
class Model;
class BaseLight;

class ModelNodePicker
{
private:
    ModelNodePicker(){};
public:
    static std::pair<Node*, float> const pickNode(Scene *const pScene, std::vector<Model>* pModels, const std::pair<int, int>& pScreenSize, const std::pair<double, double>& pMousePos);
	static std::pair<BaseLight*, float> const pickBaseLight(Model * pModel, Scene *const pScene, std::vector<BaseLight*>* pLights, const std::pair<int, int>& pScreenSize, const std::pair<double, double>& pMousePos);
    static const bool checkRayIntersectionTriangle(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const glm::vec3 pTriangle[3], float& pDistanceOutput);
    static const bool checkRayIntersectionOBB(const glm::vec3& pRaySource, const glm::vec3& pRayDirection, const std::pair<glm::vec4,glm::vec4>& pOBB, const glm::mat4& pTransform, float& pDistanceOutput);
};

#endif /* ModelNodePicker_hpp */
