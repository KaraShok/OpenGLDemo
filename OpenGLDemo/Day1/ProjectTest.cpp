//
//  ProjectTest.cpp
//  DemoOpenGL
//
//  Created by KaraShokZ on 2023/7/30.
//

#include "ProjectTest.hpp"

namespace  ProjectTest {

GLShaderManager shaderManager;
GLBatch triangleBatch;

// 窗口大小改变时接受新的宽高，其中 0,0 代表窗口中视口的左下角坐标，w,h 代表像素
void ChangeSize(int w, int h)
{
    glViewport(0,0,w,h);
}

// 为程序做一次性设置
void SetupRC()
{
    // 设置背景颜色
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    // 初始化着色器管理
    shaderManager.InitializeStockShaders();
    
    // 设置三角形，其中数组 vVerts 所有三个顶点的 x,y 笛卡尔坐标点
    GLfloat vVerts[] = {
        -0.5f,0.0f,0.0f,
        0.5f,0.0f,0.0f,
        0.0f,0.5f,0.0f
    };

    // 批次处理
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}

// 开始渲染
void RenderScene(void)
{
    // 清除一个或一组缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // 设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};

    // 传递到存储着色器，即 GLT_SHADER_IDENTITY 着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标，在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);

    // 提交着色器
    triangleBatch.Draw();

    // 将在后台缓冲区进行渲染，然后在结束时交换到前台
    glutSwapBuffers();
}
}

int ProjectTestMain(int argc, char *argv[])
{
    // 设置当前工作目录
    gltSetWorkingDirectory(argv[0]);
    
    // 初始化 GLUT 库
    glutInit(&argc, argv);
    
    // 初始化双缓冲窗口，其中标志 GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL 分别指双缓冲窗口、RGBA 颜色模式、深度测试、模板缓冲区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    
    // GLUT 设置窗口大小
    glutInitWindowSize(800, 600);
    
    // 设置标题
    glutCreateWindow("ProjectTest");
    
    // 注册回调函数
    glutReshapeFunc(ProjectTest::ChangeSize);
    glutDisplayFunc(ProjectTest::RenderScene);
    
    // 确保程序的初始化中没有问题
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("GLEW Error: %s\n",glewGetErrorString(status));
        return 1;
    }
    
    ProjectTest::SetupRC();
    glutMainLoop();
    return 0;
}
