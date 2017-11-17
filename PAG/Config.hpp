//
//  Config.hpp
//  Zadanie 2
//
//  Created by Krystian Owoc on 07.10.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Config_h
#define Config_h

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640
#define WINDOW_NAME "OpenGL"

#define TICKS_PER_SECOND 60
#define SKIP_TICKS 1000/TICKS_PER_SECOND
#define MAX_FRAMESKIP 10

#define BACKGROUND_COLOR 0.2f,0.3f,0.3f,1.0f

#define NUMBER_OF_TEXTURES 2

#define ACTIVE_TEXTURE_FOR_PROGRAM 0

#define VERTEX_SHADER_LOCATION "Shaders/basic.vert"
//#define FRAGMENT_SHADER_LOCATION "Shaders/basic.frag"
#define FRAGMENT_SHADER_LOCATION "Shaders/lesson.frag"

//Uncomment to use shader for blending transparent textures
//#define FRAGMENT_SHADER_LOCATION "Shaders/transparency.frag"

#endif /* Config_h */
