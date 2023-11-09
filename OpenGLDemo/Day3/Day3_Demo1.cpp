//
//  Day3_Demo1.cpp
//  DemoOpenGL
// 深度测试 + 正背面剔除
//  Created by KaraShokZ on 2023/8/27.
//

#include "Day3_Demo1.hpp"

namespace  Day3_Demo1 {

GLFrame viewFrame;
GLFrustum viewFrustum;
GLTriangleBatch torusBatch;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLGeometryTransform transformPipeline;
GLShaderManager shaderManager;

int iCull = 0;
int iDepth = 0;

void ProcessMenu(int value) {
    switch (value) {
        case 1:
            iDepth = !iDepth;
            break;
        case 2:
            iCull = !iCull;
            break;
        case 3:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 4:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 5:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            break;
    }
    
    glutPostRedisplay();
}

void RenderScene(void) {
    
    // 清除
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 根据设置标记来判断是否开启背面剔除
    if (iCull) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }

    // 根据设置标记来判断是否开启深度测试
    if (iDepth) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    // 把摄像机矩阵压入模型矩阵中
    modelViewMatrix.PushMatrix(viewFrame);

    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    // 使用默认光源着色器，通过光源、阴影效果更能体现立体效果
    // 参数1：默认光源着色器
    // 参数2：模型视图矩阵
    // 参数3：投影矩阵
    // 参数4：基本颜色值
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(),transformPipeline.GetProjectionMatrix(),vRed);
    
    // 绘制
    torusBatch.Draw();

    // 出栈
    modelViewMatrix.PopMatrix();
    glutSwapBuffers();
}

void SetupRC() {
    
    // 设置背景颜色
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    
    // 初始化着色器管理器
    shaderManager.InitializeStockShaders();
    
    // 将相机向后移动 7 个单元，肉眼到物体之间的距离
    viewFrame.MoveForward(7.0f);
    
    // 创建一个甜甜圈
    // 参数1：容器帮助类
    // 参数2：外边缘半径（主半径）
    // 参数3：内边缘半径（从半径）
    // 参数4、5：主半径和从半径的细分单元数量
    gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);
    
    // 设置点的大小
    glPointSize(4.0f);
}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        viewFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_DOWN) {
        viewFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_LEFT) {
        viewFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    }
    if (key == GLUT_KEY_RIGHT) {
        viewFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
    }
    
    // 重新刷新 Window
    glutPostRedisplay();
}

void ChangeSize(int w, int h) {
    if (h <= 0) {
        h = 1;
    }
    
    // 设置视口窗口尺寸
    glViewport(0, 0, w, h);
    
    // SetPerspective 函数的参数是一个从顶点方向看去的视场角度（用角度值表示）
    // 设置透视模式，初始化其透视矩阵
    viewFrustum.SetPerspective(35.0f, float(w)/float(h), 1.0f, 100.0f);
    
    // 把透视矩阵加载到透视矩阵队列中
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    // 初始化渲染管线
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}
}

int Day3_Demo1TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Geometry Test Program");
    glutReshapeFunc(Day3_Demo1::ChangeSize);
    glutSpecialFunc(Day3_Demo1::SpecialKeys);
    glutDisplayFunc(Day3_Demo1::RenderScene);
    
    glutCreateMenu(Day3_Demo1::ProcessMenu);
    glutAddMenuEntry("Toggle depth test", 1);
    glutAddMenuEntry("Toggle cull backface", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set Line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    Day3_Demo1::SetupRC();
    
    glutMainLoop();
    return 0;
}
