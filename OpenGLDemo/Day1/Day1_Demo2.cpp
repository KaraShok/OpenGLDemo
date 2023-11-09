//
//  Day1_Demo2.cpp
//  DemoOpenGL
//
//  Created by KaraShokZ on 2023/7/31.
//

#include "Day1_Demo2.hpp"

namespace  Day1_Demo2 {

// 定义一个，着色管理器
GLShaderManager shaderManager;

// 简单的批次容器，是GLTools的一个简单的容器类
GLBatch batch;

// blockSize 边长
GLfloat blockSize = 0.1f;

// 正方形的4个点坐标
GLfloat vVerts[] = {
    // A 点
    -blockSize, -blockSize, 0.0f,
    // B 点
    blockSize, -blockSize, 0.0f,
    // C 点
    blockSize, blockSize, 0.0f,
    //  D 点
    -blockSize, blockSize, 0.0f
};

// Z 轴旋转角度
GLfloat zRot = 0.0f;

// D 点 x 坐标
GLfloat blockX;

// D 点 y 坐标
GLfloat blockY;

/**
 在窗口大小改变时，接收新的宽度&高度
 */
void ChangeSize(int w,int h) {
    
    /**
     x,y 参数代表窗口中视图的左下角坐标，而宽度、高度是像素为表示，通常x,y 都是为0
     */
    glViewport(0, 0, w, h);
}


void RenderScene(void) {
    
    /**
     1.清除一个或者一组特定的缓存区
     缓冲区是一块存在图像信息的储存空间，红色、绿色、蓝色和alpha分量通常一起分量通常一起作为颜色缓存区或像素缓存区引用。
     OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
     清除缓存区对数值进行预置
     参数：指定将要清除的缓存的
     GL_COLOR_BUFFER_BIT :指示当前激活的用来进行颜色写入缓冲区
     GL_DEPTH_BUFFER_BIT :指示深度缓存区
     GL_STENCIL_BUFFER_BIT:指示模板缓冲区
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    /**
     2.设置一组浮点数来表示红色
     */
    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    M3DMatrix44f mTransformMatrix, mFinalTransform, mRotationMatrix;
    
    m3dTranslationMatrix44(mTransformMatrix,blockX , blockY, 0.0f);
   
    m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(zRot), 0.0f, 0.0f, 1.0f);
    
    m3dMatrixMultiply44(mFinalTransform, mTransformMatrix, mRotationMatrix);
    
    zRot += 5.0f;
    
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mFinalTransform, vRed);
    
    /**
     传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
     */
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY,vRed);
    
    /**
     提交着色器
     */
    batch.Draw();
    
    /**
     在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。
     这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
     将后台缓冲区进行渲染，然后结束后交换给前台
     */
    glutSwapBuffers();
}

void SetupRC() {
    
    /**
     设置清屏颜色（背景颜色）
     */
    glClearColor(0.98f, 0.4f, 0.7f, 1.0f);
    
    /**
     没有着色器，在OpenGL 核心框架中是无法进行任何渲染的。初始化一个渲染管理器。
     在前面的课程，我们会采用固管线渲染，后面会学着用OpenGL着色语言来写着色器
     */
    shaderManager.InitializeStockShaders();
    batch.Begin(GL_TRIANGLE_FAN, 4);
    batch.CopyVertexData3f(vVerts);
    batch.End();
}

void SpecialKeys(int key, int x, int y){
    
    // 以 D 点为基准点进行移动
    GLfloat stepSize = 0.05f;
    blockX = vVerts[9];
    blockY = vVerts[10];
    
    if (key == GLUT_KEY_UP) {
        blockY += stepSize;
    }

    if (key == GLUT_KEY_DOWN) {
        blockY -= stepSize;
    }

    if (key == GLUT_KEY_LEFT) {
        blockX -= stepSize;
    }

    if (key == GLUT_KEY_RIGHT) {
        blockX += stepSize;
    }
    
    if (blockY > 1.0f) {
        blockY = 1.0f;
    }
    
    if (blockY < (-1.0f + blockSize * 2)) {
        blockY = -1.0f + blockSize *2;
    }
    
    if (blockX < -1.0f) {
        blockX = -1.0f;
    }
    
    if (blockX > (1.0f - blockSize * 2)) {
        blockX = 1.0f - blockSize * 2;
    }
    
    vVerts[0] = blockX;
    vVerts[1] = blockY - blockSize * 2;
    
    vVerts[3] = blockX + blockSize * 2;
    vVerts[4] = blockY - blockSize * 2;
    
    vVerts[6] = blockX + blockSize * 2;
    vVerts[7] = blockY;
    
    vVerts[9] = blockX;
    vVerts[10] = blockY;
    
    batch.CopyVertexData3f(vVerts);
    
    glutPostRedisplay();
}
}

int Day1_Demo2TestMain(int argc, char *argv[]) {
    
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_STENCIL);
    
    glutInitWindowSize(800, 600);
    
    glutCreateWindow("Day1_Demo2");
    
    glutReshapeFunc(Day1_Demo2::ChangeSize);
    
    glutDisplayFunc(Day1_Demo2::RenderScene);
    
    glutSpecialFunc(Day1_Demo2::SpecialKeys);
    
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("GLEW Error: %s", glewGetErrorString(status));
        return 1;
    }
    
    Day1_Demo2::SetupRC();
    glutMainLoop();
    return 0;
}
