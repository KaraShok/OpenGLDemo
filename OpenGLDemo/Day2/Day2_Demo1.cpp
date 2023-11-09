//
//  Day2_Demo1.cpp
//  DemoOpenGL
//
//  Created by KaraShokZ on 2023/8/6.
//

#include "Day2_Demo1.hpp"

namespace Day2_Demo1 {

GLShaderManager shaderManager;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLFrame cameraFrame;
GLFrame objectFrame;

// 投影矩阵
GLFrustum viewFrustum;

// 容器类（7 种不同的图元对应 7 种不同的容器对象）
GLBatch pointBatch;
GLBatch lineBatch;
GLBatch lineStripBatch;
GLBatch lineLoopBatch;
GLBatch triangleBatch;
GLBatch triangleStripBatch;
GLBatch triangleFanBatch;

// 几何变换管道
GLGeometryTransform transformPipeline;
M3DMatrix44f shadowMatrix;

GLfloat vGreen[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat vBlack[] = {0.0f, 0.0f, 0.0f, 1.0f};

// 跟踪效果步骤
int nStep = 0;

// 此函数在呈现上下文中进行任何必要的初始化
void SetupRC() {
    
    // 灰色背景
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    
    shaderManager.InitializeStockShaders();
    
    glEnable(GL_DEPTH_TEST);
    
    // 设置变换管线以使用两个矩阵堆栈
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    cameraFrame.MoveForward(-15.0f);
    
    // 随意点的坐标值
    GLfloat vCoast[24][3] = {
            {2.8f, 1.2f, 0.0f},{2.0f, 1.2f, 0.0f},
            {2.0f, 1.08f, 0.0f},{2.0f, 1.08f, 0.0f},
            {0.0f, 0.8f, 0.0f},{-0.32f, 0.4f, 0.0f},
            {-0.48f, 0.2f, 0.0f},{-0.4f, 0.0f, 0.0f},
            {-0.6f, -0.4f, 0.0f},{-0.8f, -0.8f, 0.0f},
            {-0.8f, -1.4f, 0.0f},{-0.4f, -1.6f, 0.0f},
            {0.0f, -1.2f, 0.0f},{0.2f, -0.8f, 0.0f},
            {0.48f, -0.4f, 0.0f},{0.52f, -0.2f, 0.0f},
            {0.48f, 0.2f, 0.0f},{0.8f, 0.4f, 0.0f},
            {1.2f, 0.8f, 0.0f},{1.6f, 0.6f, 0.0f},
            {2.0f, 0.6f, 0.0f},{2.2f, 0.8f, 0.0f},
            {2.4f, 1.0f, 0.0f},{2.8f, 1.0f, 0.0f}
        };
    
    pointBatch.Begin(GL_POINTS, 24);
    pointBatch.CopyVertexData3f(vCoast);
    pointBatch.End();
    
    lineBatch.Begin(GL_LINES, 24);
    lineBatch.CopyVertexData3f(vCoast);
    lineBatch.End();
    
    lineStripBatch.Begin(GL_LINE_STRIP, 24);
    lineStripBatch.CopyVertexData3f(vCoast);
    lineStripBatch.End();
    
    lineLoopBatch.Begin(GL_LINE_LOOP, 24);
    lineLoopBatch.CopyVertexData3f(vCoast);
    lineLoopBatch.End();
    
    // 金字塔坐标值
    GLfloat vPyramid[12][3] = {
            -2.0f,0.0f,-2.0f,
            2.0f,0.0f,-2.0f,
            0.0f,4.0f,0.0f,
            
            2.0f,0.0f,-2.0f,
            2.0f,0.0f,2.0f,
            0.0f,4.0f,0.0f,
            
            2.0f,0.0f,2.0f,
            -2.0f,0.0f,2.0f,
            0.0f,4.0f,0.0f,
            
            -2.0f,0.0f,2.0f,
            -2.0f,0.0f,-2.0f,
            0.0f,4.0f,0.0f,
        };
    
    triangleBatch.Begin(GL_TRIANGLES, 12);
    triangleBatch.CopyVertexData3f(vPyramid);
    triangleBatch.End();
    
    GLfloat vPoints[100][3];
    int nVerts = 0;
    GLfloat radius = 3.0f;
    
    vPoints[nVerts][0] = 0.0f;
    vPoints[nVerts][1] = 0.0f;
    vPoints[nVerts][2] = 0.0f;
    
    // M3D_2PI 就是 2 Pi 的意思，就是一个圆，绘制圆形
    for (GLfloat angle = 0.0f; angle < M3D_2PI; angle += M3D_2PI / 6.0f) {
        
        // 数组下标自增
        nVerts++;
        
        // 弧长 = 半径 * 角度；这里的角度是弧度制
        vPoints[nVerts][0] = float(cos(angle)) * radius;
        vPoints[nVerts][1] = float(sin(angle)) * radius;
        vPoints[nVerts][2] = -0.5f;
    }
    
    // 添加闭合点
    nVerts++;
    vPoints[nVerts][0] = radius;
    vPoints[nVerts][1] = 0.0f;
    vPoints[nVerts][2] = 0.0f;
    
    triangleFanBatch.Begin(GL_TRIANGLE_FAN, 8);
    triangleFanBatch.CopyVertexData3f(vPoints);
    triangleFanBatch.End();
    
    // 三角形条带，一个小环或圆柱端
    int iCounter = 0;
    // 从 0 - 360 度，以 0.3 弧度为步长
    for (GLfloat angle = 0.0f; angle <= (2.0f * M3D_PI);  angle += 0.3f) {
        
        // 获取圆形的顶点 x,y
        GLfloat x = radius * sin(angle);
        GLfloat y = radius * cos(angle);
        
        // 绘制 2 个三角形（它们的 x,y 顶点一样，只是 z 点不一样）
        vPoints[iCounter][0] = x;
        vPoints[iCounter][1] = y;
        vPoints[iCounter][2] = -0.5f;
        iCounter++;
        
        vPoints[iCounter][0] = x;
        vPoints[iCounter][1] = y;
        vPoints[iCounter][2] = 0.5f;
        iCounter++;
    }
    
    // 结束循环，在循环位置生成 2 个三角形。达到闭合效果
    vPoints[iCounter][0] = vPoints[0][0];
    vPoints[iCounter][1] = vPoints[0][1];
    vPoints[iCounter][2] = -0.5f;
    iCounter++;
    
    vPoints[iCounter][0] = vPoints[1][0];
    vPoints[iCounter][1] = vPoints[1][1];
    vPoints[iCounter][2] = 0.5f;
    iCounter++;
    
    // GL_TRIANGLE_STRIP 公用一条带（strip）上的顶点的一组三角形
    triangleStripBatch.Begin(GL_TRIANGLE_STRIP, iCounter);
    triangleStripBatch.CopyVertexData3f(vPoints);
    triangleStripBatch.End();
}

void DrawWrieFramedBatch(GLBatch *pBatch) {
    
    // 填充图形，Uniform 值——平面着色器
    // 参数 1：平面着色器
    // 参数 2：运行为几何图形变换指定一个 4 * 4 变换矩阵，transformPipeline 变换管线（指定了 2 个矩阵堆栈）
    // 参数 3：颜色值
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(),vGreen);
    pBatch->Draw();
    
    // 画黑色边框
    glPolygonOffset(-1.0f, -1.0f);
    
    // 偏移深度，在同一位置要绘制填充和边线，会产生 z 冲突，所以要偏移
    glEnable(GL_POLYGON_OFFSET_LINE);
    
    // 画反锯齿，让黑边好看些
    glEnable(GL_LINE_SMOOTH);
    
    // 启用颜色混合
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // 绘制线框几何黑色版三种模式，实心、边框和点，可以作用在正面、背面或者两面
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glLineWidth(2.5f);
    
    // 画线
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vBlack);
    pBatch->Draw();
    
    // 复原原本的设置
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}

void RenderScene(void) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 压栈
    modelViewMatrix.PushMatrix();
    
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    // 矩阵乘以矩阵堆栈的顶部矩阵，相乘的结果随后存储在堆栈的顶部
    modelViewMatrix.MultMatrix(mCamera);
    
    // 只有使用 GetMatrix 函数就可以获取矩阵堆栈顶部的值，这个函数可以进行 2 次重载
    // 原来使用 GLShaderManager 的使用，或者是获取顶部矩阵的顶底副本数据
    M3DMatrix44f mObjectFrame;
    objectFrame.GetMatrix(mObjectFrame);
    modelViewMatrix.MultMatrix(mObjectFrame);
    
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),vBlack);
    
    switch (nStep) {
        case 0:
            // 设置点大小
            glPointSize(4.0f);
            pointBatch.Draw();
            glPointSize(1.0f);
            break;
        case 1:
            // 设置线宽度
            glLineWidth(2.0f);
            lineBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 2:
            glLineWidth(2.0f);
            lineStripBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 3:
            glLineWidth(2.0f);
            lineLoopBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 4:
            DrawWrieFramedBatch(&triangleBatch);
            break;
        case 5:
            DrawWrieFramedBatch(&triangleStripBatch);
            break;
        case 6:
            DrawWrieFramedBatch(&triangleFanBatch);
            break;
        default:
            break;
    }
    
    // 还原到以前的模型视图矩阵（单位矩阵）
    modelViewMatrix.PopMatrix();
    // 进行缓冲区交换
    glutSwapBuffers();
}

// 特殊键位的处理（上、下、左、右）
void SpecialKeys(int key, int x, int y) {
    // RotateWorld 旋转世界坐标系
    if (key == GLUT_KEY_UP) {
        objectFrame.RotateWorld(m3dDegToRad(-0.5f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_DOWN) {
        objectFrame.RotateWorld(m3dDegToRad(0.5f), 1.0f, 0.0f, 0.0f);
    }
    if (key == GLUT_KEY_LEFT) {
        objectFrame.RotateWorld(m3dDegToRad(-0.5f), 0.0f, 1.0f, 0.0f);
    }
    if (key == GLUT_KEY_RIGHT) {
        objectFrame.RotateWorld(m3dDegToRad(0.5f), 0.0f, 1.0f, 0.0f);
    }
    glutPostRedisplay();
}

// 根据按空格次数，切换不同的窗口标题
void KeyPressFunc(unsigned char key, int x, int y) {
    if (key == 32) {
        nStep++;
        if (nStep > 6) {
            nStep = 0;
        }
    }
    switch (nStep) {
            case 0:
                glutSetWindowTitle("GL_POINTS");
                break;
            case 1:
                glutSetWindowTitle("GL_LINES");
                break;
            case 2:
                glutSetWindowTitle("GL_LINE_STRIP");
                break;
            case 3:
                glutSetWindowTitle("GL_LINE_LOOP");
                break;
            case 4:
                glutSetWindowTitle("GL_TRIANGLES");
                break;
            case 5:
                glutSetWindowTitle("GL_TRIANGLES_STRIP");
                break;
            case 6:
                glutSetWindowTitle("GL_TRIANGLES_FAN");
                break;
            default:
                break;
        }
        glutPostRedisplay();
}

// 窗口已更改大小
void ChangeSize(int w, int h) {
    
    glViewport(0, 0, w, h);
    
    // 创建投影矩阵，并将它载入投影矩阵的堆栈中
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    // 调用顶部载入单元矩阵
    modelViewMatrix.LoadIdentity();
}

}

int Day2_Demo1TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    // 申请一个颜色缓冲区、深度缓冲区、双缓冲区、模板缓冲区
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    // 创建窗口大小
    glutInitWindowSize(800, 600);
    // 创建窗口标题
    glutCreateWindow("GL_POINTS");
    // 改变尺寸调用的函数
    glutReshapeFunc(Day2_Demo1::ChangeSize);
    // 点击空格时，调用的函数
    glutKeyboardFunc(Day2_Demo1::KeyPressFunc);
    // 特殊键位函数
    glutSpecialFunc(Day2_Demo1::SpecialKeys);
    // 显示函数
    glutDisplayFunc(Day2_Demo1::RenderScene);
    
    // 判断一下是否能初始化 glew 库
    GLenum state = glewInit();
    if (GLEW_OK != state) {
        printf("GLEW Error: %s\n",glewGetErrorString(state));
        return 1;
    }
    // 绘制
    Day2_Demo1::SetupRC();
    // 运行循环
    glutMainLoop();
    return 0;
}
