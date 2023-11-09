//
//  Day5_Demo2.cpp
//  DemoOpenGL
// 管线管理（光照基础）
//  Created by KaraShokZ on 2023/9/30.
//

#include "Day5_Demo2.hpp"

namespace Day5_Demo2 {

// 着色器管理器
GLShaderManager shaderManager;

// 模型视图矩阵
GLMatrixStack modelViewMatrix;

// 投影矩阵
GLMatrixStack projectionMatrix;

// 视景体
GLFrustum viewFrustum;

// 几何图形变换管道
GLGeometryTransform transformPipeline;

// 甜甜圈批处理
GLTriangleBatch torusBatch;

// 地板批处理
GLBatch floorBatch;

// 公转球批处理
GLTriangleBatch sphereBatch;

// 照相机角色帧
GLFrame cameraFrame;

void SpecialKeys(int key, int x, int y) {
    float linear = 0.1f;
    float angular = float(m3dDegToRad(5.0f));
    
    if (key == GLUT_KEY_UP) {
        // 平移
        cameraFrame.MoveForward(linear);
    }
    
    if (key == GLUT_KEY_DOWN) {
        cameraFrame.MoveForward(-linear);
    }
    
    if (key == GLUT_KEY_LEFT) {
        // 旋转
        cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
    }
    
    if (key == GLUT_KEY_RIGHT) {
        cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
    }
}

void SetupRC() {
    
    // 初始化着色器管理器
    shaderManager.InitializeStockShaders();
    
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    // 开启多边形模型
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // 设置清屏颜色到颜色缓冲区
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 绘制甜甜圈
    gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);
    
    // 绘制公转球
    gltMakeSphere(sphereBatch, 0.1f, 26, 13);

    // 往地板 floorBatch 批处理中添加顶点数据
    floorBatch.Begin(GL_LINES, 324);
    for (GLfloat x= -20.0f; x <= 20.0f; x += 0.5f) {
        floorBatch.Vertex3f(x, -0.55f, 20.0f);
        floorBatch.Vertex3f(x, -0.55f, -20.0f);

        floorBatch.Vertex3f(20.0f, -0.55f, x);
        floorBatch.Vertex3f(-20.0f, -0.55f, x);
    }
    floorBatch.End();
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    
    // 创建投影矩阵
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 100.0f);
    
    // 获取 viewFrustum 的投影矩阵，并将其加载到投影矩阵堆栈上
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    // 设置变换管道已使用两个矩阵堆栈（变换矩阵、投影矩阵）
    // 初始化 GLGeometryTransform 的实例 transformPipeline ，通过将它的内部指针设置为模型视图矩阵堆栈和投影矩阵堆栈实例，来完成初始化
    // 当然这个操作也可以在 SetupRC 函数中完成
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void RenderScene(void) {
    
    // 地板颜色值
    static GLfloat vFloorColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
    
    // 甜甜圈颜色值
    static GLfloat vTorusColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    // 公转球颜色值
    static GLfloat vSphereColor[] = {0.0f, 0.0f, 1.0f, 1.0f};

    // 基于时间动画
    static CStopWatch rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

    // 清除颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 将当前的模型视图矩阵压入矩阵堆栈（单位矩阵）
    // 因为我们先绘制地面，而地面是不需要任何变换的。所以在开始渲染时保证矩阵状态，然后在结束时使用相应的 pop 恢复它。这样就不必在每一次渲染时重载单位矩阵了
    modelViewMatrix.PushMatrix();

    // 设置照相机矩阵
    M3DMatrix44f mCamera;
    
    // 从 mCamera 中获取一个 4 * 4 的矩阵
    cameraFrame.GetCameraMatrix(mCamera);
    
    // 将照相机矩阵压入模型视图堆栈中
    modelViewMatrix.PushMatrix(mCamera);

    // 绘制地面
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vFloorColor);
    floorBatch.Draw();

    // 顶部矩阵沿着 z 轴移动 2.5 个单位
    modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);
    
    // 保存平移
    modelViewMatrix.PushMatrix();
    
    // 顶部矩阵渲染 yRot 度
    modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f);

    // 绘制甜甜圈
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vTorusColor);
    torusBatch.Draw();

    // 推出刚刚变换的矩阵，然后恢复到单位矩阵
    modelViewMatrix.PopMatrix();
    modelViewMatrix.Rotate(yRot * -2.0f, 0.0f, 1.0f, 0.0f);
    modelViewMatrix.Translate(0.8f, 0.0f, 0.0f);
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vSphereColor);
    sphereBatch.Draw();

    // 恢复公转矩阵
    modelViewMatrix.PopMatrix();
    
    // 恢复矩阵
    modelViewMatrix.PopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}
}

int Day5_Demo2TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
        
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    
    glutCreateWindow("OpenGL SphereWorld");
    
    glutReshapeFunc(Day5_Demo2::ChangeSize);
    glutDisplayFunc(Day5_Demo2::RenderScene);
    glutSpecialFunc(Day5_Demo2::SpecialKeys);
        
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
        
    Day5_Demo2::SetupRC();
    glutMainLoop();
    return 0;
}
