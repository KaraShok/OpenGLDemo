//
//  Day3_Demo2.hpp
//  DemoOpenGL
//
//  Created by KaraShokZ on 2023/8/28.
//

#ifndef Day3_Demo2_hpp
#define Day3_Demo2_hpp

/**
 包含了大部分GLTool中类似C语言的独立函数
 */
#include "../include/GLTools.h"

/**
 在Mac 系统下，`#include<glut/glut.h>`
  在Windows 和 Linux上，我们使用freeglut的静态库版本并且需要添加一个宏
 */
#include <GLUT/GLUT.h>

/**
 着色器管理器（shader Mananger）类。没有着色器，我们就不能在OpenGL（核心框架）进行着色。
 着色器管理器不仅允许我们创建并管理着色器，还提供一组“存储着色器”，他们能够进行一些初步的基本的渲染操作
 */
#include "../include/GLShaderManager.h"

#include "../include/GLMatrixStack.h"
#include "../include/GLFrame.h"
#include "../include/GLFrustum.h"
#include "../include/GLBatch.h"
#include "../include/GLGeometryTransform.h"

int Day3_Demo2TestMain(int argc, char *argv[]);

#endif /* Day3_Demo2_hpp */
