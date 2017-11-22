// Camera.cpp
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

#include "Camera.hpp"
#include "Config.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(): mCameraSpeed(CAMERA_MOVE_SPEED)
{
    mYaw=CAMERA_STARTING_FRONT_YAW;
    mPitch=CAMERA_STARTING_FRONT_PITCH;
    mCameraPos=CAMERA_STARTING_POS;
    mCameraUp=CAMERA_UP;
    mCameraFront=calculateCameraFront();
}

const glm::vec3 Camera::calculateCameraFront()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
    newFront.y = sin(glm::radians(mPitch));
    newFront.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
    return glm::normalize(newFront);
}

void Camera::moveInDirection(const glm::vec3& pDirection)
{
    if (pDirection[0]>0)
        mCameraPos+=mCameraSpeed * mCameraFront;
    else if (pDirection[0]<0)
        mCameraPos-=mCameraSpeed * mCameraFront;
    if (pDirection[1]>0) //Specjalnie jedynie tak, by było niezależne od Front
        mCameraPos.y -= mCameraSpeed;
    else if (pDirection[1]<0)
        mCameraPos.y += mCameraSpeed;
    if (pDirection[2]>0)
        mCameraPos += glm::normalize(glm::cross(mCameraFront, mCameraUp)) * mCameraSpeed;
    else if (pDirection[2]<0)
        mCameraPos -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * mCameraSpeed;
    mCameraFront=calculateCameraFront();
}

void Camera::rotateByOffset(const float& pOffsetX, const float& pOffsetY)
{
    mPitch+=pOffsetY;
    mYaw+=pOffsetX;
    if (mPitch > 89.0f)
        mPitch =  89.0f;
    else if (mPitch < -89.0f)
        mPitch = -89.0f;
    mCameraFront=calculateCameraFront();
}

const glm::vec3& Camera::getCameraPos() { return mCameraPos; }

const glm::mat4 Camera::generateViewSpace()  { return glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp); }

Camera::~Camera() {}
