//
//  Day5_Demo1.cpp
//  DemoOpenGL
// 使用矩阵创建几何图形
//  Created by KaraShokZ on 2023/9/28.
//

#include "Day5_Demo1.hpp"

namespace Day5_Demo1 {

GLShaderManager shaderManager;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;

//  观察者位置
GLFrame cameraFrame;

//  世界坐标位置
GLFrame objectFrame;

//  视景体，用来构建投影矩阵
GLFrustum viewFrustum;

//  三角形批次类
GLTriangleBatch ccTriangle;

//  球
GLTriangleBatch sphereBatch;

//  环
GLTriangleBatch torusBatch;

//  圆柱
GLTriangleBatch cylinderBatch;

//  锥
GLTriangleBatch coneBatch;

//  磁盘
GLTriangleBatch diskBatch;

GLGeometryTransform transformPipeline;
M3DMatrix44f shadowMatrix;

GLfloat vGreen[] = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat vBlack[] = {0.0f, 0.0f, 0.0f, 1.0f};

int nStep = 0;

void SetupRC() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    
    //  初始化固定管线
    shaderManager.InitializeStockShaders();
    
    //  开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    //  通过 GLGeometryTransform 管理矩阵堆栈
    //  使用 transformPipeline 管道管理模型视图矩阵堆栈和投影矩阵堆栈
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
    
    //  将观察者坐标位置 z 往屏幕里移动 15 个单位位置
    //  表示离屏幕间距离：负数，往屏幕后移动；正数，往屏幕前移动
    cameraFrame.MoveForward(-15.0f);
    
    //  利用三角形批次类构造图形对象
    
    //  1. 球
    /**
     gltMakeSphere(GLTriangleBatch& sphereBatch, GLfloat fRadius, GLint iSlices, GLint iStacks)
     参数1：sphereBatch，三角形批次类对象
     参数2：fRadius，球体半径
     参数3：iSlices，从球体底部堆叠到顶部的三角形带的数量；其实球体是一圈一圈三角形带组成
     参数4：iStacks，围绕球体一圈排列的三角形对数
     
     一个对称性较好的球体的片段数量是堆叠数量的 2 倍：iStacks = iSlices * 2
     绘制球体都是围绕 z 轴，这样 +z 就是球体的顶点，-z 就是球体的底部
     */
    gltMakeSphere(sphereBatch, 3.0f, 10, 20);
    
    //  2. 环
    /**
     gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
     参数1：torusBatch，三角形批次类对象
     参数2：majorRadius，换面中心到外边缘的半径（主半径）
     参数3：minorRadius，换面中心到内边缘的半径
     参数4：numMajor，沿着外边缘半径的三角形数量
     参数5：numMinor，沿着内边缘半径的三角形数量
     */
    gltMakeTorus(torusBatch, 3.0f, 0.75f, 15, 15);
    
    //  3. 圆柱
    /**
     gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks)
     参数1：cylinderBatch，三角形批次类对象
     参数2：baseRadius，底部半径
     参数3：topRadius，顶部半径
     参数4：fLength，圆形长度
     参数5：numSlices，围绕 z 轴的三角形对的数量
     参数6：numStacks，圆柱底部堆叠到顶部圆环的三角形数量
     */
    gltMakeCylinder(cylinderBatch, 2.0f, 2.0f, 3.0f, 15, 2);
    
    //  4. 锥
    /**
     gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks)
     参数1：cylinderBatch，三角形批次类对象
     参数2：baseRadius，底部半径
     参数3：topRadius，顶部半径
     参数4：fLength，圆形长度
     参数5：numSlices，围绕 z 轴的三角形对的数量
     参数6：numStacks，圆柱底部堆叠到顶部圆环的三角形数量
     */
    gltMakeCylinder(coneBatch, 2.0f, 0.0f, 3.0f, 13, 2);
    
    //  5. 磁盘
    /**
     gltMakeDisk(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks)
     参数1：diskBatch，三角形批次类对象
     参数2：innerRadius，内圆半径
     参数3：outerRadius，外圆半径
     参数4：nSlices，围绕 z 轴的三角形对的数量
     参数5：nStacks，圆盘外边缘到内边缘的三角形数量
     */
    gltMakeDisk(diskBatch, 1.5f, 3.0f, 13, 3);
}

void DrawWireFramedBatch(GLTriangleBatch *pBatch) {
    
    //  平面着色器，绘制三角形
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
    
    //  传过来的参数，对应不同的图形对象
    pBatch->Draw();
    
    //  画出黑色轮廓
    glPolygonOffset(-1.0f, -1.0f);
    
    //  开启处理线
    glEnable(GL_LINE_SMOOTH);
    
    //  开启混合功能
    glEnable(GL_BLEND);
    
    //  颜色混合
    //  表示源颜色乘以自身的 alpha 值，目标颜色乘以 1.0 减去源颜色的 alpha 值，这样一来，源颜色的 alpha 值越大，则产生的新颜色中源颜色所占比例就越大，
    //  而目标颜色所占比例则减小。这种情况下，我们可以简单的将源颜色的 alpha 值理解为“不透明度”。这也是混合时最常用的方式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //  通过程序点大小模式来设置点的大小
    glEnable(GL_POLYGON_OFFSET_LINE);
    
    //  多边形模型（背面、线）将多边形背面设为线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //  线条宽度
    glLineWidth(2.5f);
    
    //  平面做舍弃绘制线条
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBlack);
    
    pBatch->Draw();
    
    //  恢复多边形模式和深度测试
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}

void RenderScene(void) {
    
    //  清空当前缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 模型视图矩阵堆栈，压栈
    modelViewMatrix.PushMatrix();
    
    //  获取摄像头矩阵
    M3DMatrix44f mCamera;
    
    //  从 cameraFrame 中获取矩阵到 mCamera
    cameraFrame.GetCameraMatrix(mCamera);
    
    //  模型视图堆栈的矩阵与 mCamera 矩阵相乘之后，存储到 modelViewMatrix 矩阵堆栈中
    modelViewMatrix.MultMatrix(mCamera);
    
    //  创建矩阵 mObjectFrame
    M3DMatrix44f mObjectFrame;
    
    //  从 objectFrame 获取矩阵到 mObjectFrame 中
    objectFrame.GetMatrix(mObjectFrame);
    
    //  将modelViewMatrix  的堆栈中的矩阵与 mObjectFrame 矩阵相乘，存储到 modelViewMatrix 矩阵堆栈中
    modelViewMatrix.MultMatrix(mObjectFrame);
    
    //  使用平面着色器
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBlack);
    
    //  判断是第几个图形
    switch (nStep) {
        case 0:
            DrawWireFramedBatch(&sphereBatch);
            break;
        case 1:
            DrawWireFramedBatch(&torusBatch);
            break;
        case 2:
            DrawWireFramedBatch(&cylinderBatch);
            break;
        case 3:
            DrawWireFramedBatch(&coneBatch);
            break;
        case 4:
            DrawWireFramedBatch(&diskBatch);
            break;
        default:
            break;
    }
    
    modelViewMatrix.PopMatrix();
    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y) {
    //  移动世界坐标系，而不是移动物体
    if (key == GLUT_KEY_UP) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    }
    
    if (key == GLUT_KEY_DOWN) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
    }
    
    if (key == GLUT_KEY_LEFT) {
        objectFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    }
    
    if (key == GLUT_KEY_RIGHT) {
        objectFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
    }
    
    glutPostRedisplay();
}

void KeyPressFunc(unsigned char key, int x, int y) {
    if (key == 32) {
        nStep++;
        if (nStep > 4) {
            nStep = 0;
        }
    }
    
    switch (nStep) {
        case 0:
            glutSetWindowTitle("Sphere");
            break;
        case 1:
            glutSetWindowTitle("Torus");
            break;
        case 2:
            glutSetWindowTitle("Cylinder");
            break;
        case 3:
            glutSetWindowTitle("Cone");
            break;
        case 4:
            glutSetWindowTitle("Disk");
            break;
        default:
            break;
    }
    
    glutPostRedisplay();
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    
    //  透视投影
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
    
    //  加载透视投影矩阵
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    
    //  加载单元矩阵
    modelViewMatrix.LoadIdentity();
}
}

int Day5_Demo1TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
        
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sphere");
    glutReshapeFunc( Day5_Demo1::ChangeSize);
    glutKeyboardFunc( Day5_Demo1::KeyPressFunc);
    glutSpecialFunc( Day5_Demo1::SpecialKeys);
    glutDisplayFunc( Day5_Demo1::RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
        
    
    Day5_Demo1::SetupRC();
        
    glutMainLoop();
    return 0;
}
