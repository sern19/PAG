// Config.hpp
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

#ifndef Config_h
#define Config_h

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_NAME "OpenGL"

#define TICKS_PER_SECOND 60
#define SKIP_TICKS TICKS_PER_SECOND/1000000.0f
#define MAX_FRAMESKIP 10

#define BACKGROUND_COLOR 0.2f,0.3f,0.3f,1.0f

#define ACTIVE_TEXTURE_FOR_PROGRAM 0

#define VERTEX_SHADER_LOCATION "Shaders/basic.vert"
#define FRAGMENT_SHADER_LOCATION "Shaders/basic.frag"

#define CAMERA_STARTING_POS glm::vec3(0.0f, 1.0f, 1.5f)
#define CAMERA_STARTING_FRONT_YAW 270
#define CAMERA_STARTING_FRONT_PITCH -20
#define CAMERA_UP glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_MOVE_SPEED 0.006f

#define MOUSE_SENSIVITY 0.05f

//Material stuff
#define DIFFUSE_NAME "diffuse"
#define SPECULAR_NAME "specular"
#define NORMAL_NAME "normal"
#define DIFFUSE_STARTING_INDEX 0
#define SPECULAR_STARTING_INDEX 10
#define NORMAL_STARTING_INDEX 20
#define MAXIMUM_NUMBER_OF_TEXTURES_PER_TYPE 1

#endif /* Config_h */
