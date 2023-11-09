//
//  Day6_Demo3.cpp
//  DemoOpenGL
// 带倒影的球
//  Created by KaraShokZ on 2023/10/4.
//

#include "Day6_Demo3.hpp"

namespace Day6_Demo3 {

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

// 计数器
CStopWatch rotTimer;

// 地板
GLBatch floorBatch;

// 球 01
GLTriangleBatch sphere01Batch;

// 球 02
GLTriangleBatch sphere02Batch;

// 小球数量
const int SPHERE_COUNT = 6;
// 纹理数量
const int   TEXTURE_COUNT = 8;

GLfloat sphere01Radius = 0.4f;
GLfloat sphere02Radius = 0.12f;

// 小球的轨迹半径
GLfloat spheresRadius[SPHERE_COUNT];

// 小球每秒转动角度
GLfloat spheresRotate[SPHERE_COUNT];

// 摄像机角色帧
GLFrame cameraFrame;

const char *szTextureFiles[TEXTURE_COUNT] = {
    "brick.tga",
    "ceiling.tga",
    "floor.tga",
    "Marble.tga",
    "Marslike.tga",
    "MoonLike.tga",
    "SphereMap.tga",
    "stone.tga"
};

GLuint textures[TEXTURE_COUNT];

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode) {
    GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    
    // 读取纹理文件
    pBytes = gltReadTGABits(szFileName, &iWidth, &iHeight, &iComponents, &eFormat);
    
    if (pBytes == NULL) {
        return  false;
    }
    
    // 设置过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    // 设置环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    // 包装数据
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // 载入纹理
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
    
    // 为纹理对象生成一组完整的 Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    
    free(pBytes);
    
    return true;
}

void SetupRC() {
    
    // 清屏颜色
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    // 初始化着色器管理器
    shaderManager.InitializeStockShaders();
    
    // 分配纹理对象
    glGenTextures(TEXTURE_COUNT, textures);
    
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        
        // 绑定纹理，指定下面要操作的是哪个纹理
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        
        LoadTGATexture(szTextureFiles[i], GL_NEAREST, GL_LINEAR, GL_MIRRORED_REPEAT);
    }
    
    // 设置地板顶点数据
    GLfloat y = -0.5f;
    floorBatch.Begin(GL_TRIANGLE_FAN, 4, 1);
    
    floorBatch.MultiTexCoord2f(0, 0.0, 0.0);
    floorBatch.Vertex3f(-20.0, y, 20.0f);
    
    floorBatch.MultiTexCoord2f(0, 10.0, 0.0);
    floorBatch.Vertex3f(20.0, y, 20.0f);
    
    floorBatch.MultiTexCoord2f(0, 10.0, 10.0);
    floorBatch.Vertex3f(20.0f, y, -20.0);
    
    floorBatch.MultiTexCoord2f(0, 0.0, 10.0);
    floorBatch.Vertex3f(-20.0f, y, -20.0);
    
    floorBatch.End();
    
    // 设置球 01
    gltMakeSphere(sphere01Batch, sphere01Radius, 40, 80);
    
    // 设置球 02
    gltMakeSphere(sphere02Batch, sphere02Radius, 20, 40);
    
    for (int i = 0; i < SPHERE_COUNT; i ++) {
        // 设置小球的轨迹半径
        spheresRadius[i] = sphere01Radius + 2.5 * (i + 1) * sphere02Radius;
        // 转动角度
        GLfloat rotate = (rand() % 2 + 2) * (i % 6 + 1) * 10.0f;
        spheresRotate[i] = rotate;
    }
    
    // 开启正背面剔除
    glEnable(GL_CULL_FACE);
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    cameraFrame.MoveForward(-5.0);
    
}

void ShutdownRC(void) {
    // 删除纹理
    glDeleteTextures(TEXTURE_COUNT, textures);
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 压栈
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.PushMatrix(mCamera);
    
    // 向屏幕里面平移 3.0
    modelViewMatrix.Translate(0.0f, 0.0f, -4.0f);
    
    // 点光源位置
    M3DVector4f vLightPos = {10.0f, 10.0f, 10.0f, 1.0f};
    
    // 白色
    GLfloat whiteColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    // 地板颜色
    GLfloat floorColor[] = {0.7f, 0.7f, 0.7f, 0.7f};
    
    // 画倒影
    modelViewMatrix.PushMatrix();
    
    // 翻转 y 轴
    modelViewMatrix.Scale(1.0f, -1.0f, 1.0f);
    
    // 镜面世界围绕 y 轴平移一定距离
    modelViewMatrix.Translate(0.0f, 1.0f, 0.0f);
    
    // 指定顺时针为正面
    glFrontFace(GL_CW);
    
    // 画大球
    modelViewMatrix.PushMatrix();
    
    // 环绕运动
    modelViewMatrix.Rotate(rotTimer.GetElapsedSeconds() * 20.0f, 0.0f, 1.0f, 0.0f);
    
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textures[6]);
    
    // 纹理光源着色器
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightPos, whiteColor, 0);
    sphere01Batch.Draw();
    modelViewMatrix.PopMatrix();
    
    // 画小球
    for (int i = 0; i < SPHERE_COUNT; i++) {
        modelViewMatrix.PushMatrix();
        
        // 环绕运动
        modelViewMatrix.Rotate(rotTimer.GetElapsedSeconds() * spheresRotate[i], 0.0f, 1.0f, 0.0f);
        modelViewMatrix.Translate(spheresRadius[i], 0.0f, 0.0f);
        
        // 绑定纹理
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        
        // 纹理光源着色器
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,
                                     transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightPos, whiteColor, 0);
        sphere02Batch.Draw();
        modelViewMatrix.PopMatrix();
    }
    
    // 恢复逆时针为正面
    glFrontFace(GL_CCW);
    modelViewMatrix.PopMatrix();
    
    // 开启混合功能（绘制地板）
    glEnable(GL_BLEND);
    
    // 指定颜色混合方式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // 绘制地板，绑定地板纹理
    glBindTexture(GL_TEXTURE_2D, textures[7]);
    
    // 纹理调整着色器（将一个基本色乘以一个取自纹理的单元 nTextureUnit 纹理）
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_MODULATE, transformPipeline.GetModelViewProjectionMatrix(), floorColor, 0);
    floorBatch.Draw();
    
    // 关闭混合
    glDisable(GL_BLEND);
    
    // 画大球
    modelViewMatrix.PushMatrix();
    
    // 环绕运动
    modelViewMatrix.Rotate(rotTimer.GetElapsedSeconds() * 20.0f, 0.0f, 1.0f, 0.0f);
    
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, textures[6]);
    
    // 纹理光源着色器
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightPos, whiteColor, 0);
    sphere01Batch.Draw();
    modelViewMatrix.PopMatrix();
    
    // 画小球
    for (int i = 0; i < SPHERE_COUNT; i++) {
        modelViewMatrix.PushMatrix();
        
        // 环绕运动
        modelViewMatrix.Rotate(rotTimer.GetElapsedSeconds() * spheresRotate[i], 0.0f, 1.0f, 0.0f);
        modelViewMatrix.Translate(spheresRadius[i], 0.0f, 0.0f);
        
        // 绑定纹理
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        
        // 纹理光源着色器
        shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vLightPos, whiteColor, 0);
        sphere02Batch.Draw();
        modelViewMatrix.PopMatrix();
    }
    
    // 出栈
    modelViewMatrix.PopMatrix();
    
    // 交换缓冲区
    glutSwapBuffers();
    
    // 进行实时绘制
    glutPostRedisplay();
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void KeyboardKeys(unsigned char key, int x, int y) {
    float linear = 0.1f;
    float angular = float(m3dDegToRad(5.0f));
    switch (key) {
        case 'w':
            // 前进
            cameraFrame.MoveForward(linear);
            break;
        case 's':
            // 后退
            cameraFrame.MoveForward(-linear);
            break;
        case 'a':
            // 左移
            cameraFrame.MoveRight(linear);
            break;
        case 'd':
            // 右移
            cameraFrame.MoveRight(-linear);
            break;
        case 'q':
            // 左转头
            cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);
            break;
        case 'e':
            // 右转头
            cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
            break;
        default:
            break;
    }
}
}

int Day6_Demo3TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("带倒影的球");
    glutReshapeFunc(Day6_Demo3::ChangeSize);
    glutDisplayFunc(Day6_Demo3::RenderScene);
    glutKeyboardFunc(Day6_Demo3::KeyboardKeys);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    Day6_Demo3::SetupRC();
    glutMainLoop();
    Day6_Demo3::ShutdownRC();
    return 0;
}
