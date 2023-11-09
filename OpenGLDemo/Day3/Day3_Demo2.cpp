//
//  Day3_Demo2.cpp
//  DemoOpenGL
// 裁剪
//  Created by KaraShokZ on 2023/8/28.
//

#include "Day3_Demo2.hpp"

namespace Day3_Demo2 {

void RenderScene(void) {
    
    // 设置清屏颜色为蓝色
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 1. 裁剪成小红色分区
    // 设置裁剪区域颜色为红色
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    // 设置裁剪尺寸
    glScissor(100, 100, 600, 400);
    // 开启裁剪测试
    glEnable(GL_SCISSOR_TEST);
    // 开启清屏，执行裁剪
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 2. 裁剪一个小绿色分区
    // 设置清屏颜色为绿色
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    // 设置裁剪尺寸
    glScissor(200, 200, 400, 200);
    // 开始清屏执行裁剪
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 关闭裁剪测试
    glDisable(GL_SCISSOR_TEST);
    
    // 强制执行缓冲区
    glutSwapBuffers();
}

void ChangeSize(int w, int h) {
    if (h <= 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
}
}

int Day3_Demo2TestMain(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scissor");
    glutReshapeFunc(Day3_Demo2::ChangeSize);
    glutDisplayFunc(Day3_Demo2::RenderScene);
    glutMainLoop();
    return 0;
}
