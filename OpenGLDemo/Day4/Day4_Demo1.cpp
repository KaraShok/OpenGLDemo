//
//  Day4_Demo1.cpp
//  DemoOpenGL
// 抗锯齿、多重采样
//  Created by KaraShokZ on 2023/9/17.
//

#include "Day4_Demo1.hpp"

namespace Day4_Demo1 {

GLShaderManager shaderManager;
GLFrustum viewFrustum;
GLBatch smallStarBatch;
GLBatch mediumStarBatch;
GLBatch largeStarBatch;
GLBatch mountainRangeBatch;
GLBatch moonBatch;

#define SMALL_STARS 100
#define MEDIUM_STARS 40
#define LARGE_STARS 15
#define SCREEN_X 800
#define SCREEN_Y 600

// 选择菜单
void ProcessMenu(int value) {
    switch (value) {
        case 1:
            // 打开抗锯齿，并给出关于尽可能进行最佳的处理提示
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            break;
        case 2:
            // 关闭抗锯齿
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POINT_SMOOTH);
            glDisable(GL_POLYGON_SMOOTH);
            break;
        default:
            break;
    }
    
    // 触发重新绘制
    glutPostRedisplay();
}

// 对渲染进行必要的初始化
void SetupRC() {
    M3DVector3f vVerts[SMALL_STARS];
    int i;
    
    shaderManager.InitializeStockShaders();
    
    // 小星星
    smallStarBatch.Begin(GL_POINTS, SMALL_STARS);
    for (i = 0; i < SMALL_STARS; i++) {
        vVerts[i][0] = (GLfloat)(rand() % SCREEN_X);
        vVerts[i][1] = (GLfloat)(rand() % (SCREEN_Y - 100)) + 100.0f;
        vVerts[i][2] = 0.0f;
    }
    smallStarBatch.CopyVertexData3f(vVerts);
    smallStarBatch.End();
    
    // 中星星
    mediumStarBatch.Begin(GL_POINTS, MEDIUM_STARS);
    for (i = 0; i < MEDIUM_STARS; i++) {
        vVerts[i][0] = (GLfloat)(rand() % SCREEN_X);
        vVerts[i][1] = (GLfloat)(rand() % (SCREEN_Y - 100)) + 100.0f;
        vVerts[i][2] = 0.0f;
    }
    mediumStarBatch. CopyVertexData3f(vVerts);
    mediumStarBatch.End();
    
    // 大星星
    largeStarBatch.Begin(GL_POINTS, LARGE_STARS);
    for (i = 0; i < LARGE_STARS; i++) {
        vVerts[i][0] = (GLfloat)(rand() % SCREEN_X);
        vVerts[i][1] = (GLfloat)(rand() % (SCREEN_Y - 100)) + 100.0f;
        vVerts[i][2] = 0.0f;
    }
    largeStarBatch.CopyVertexData3f(vVerts);
    largeStarBatch.End();
    
    // 地平线
    M3DVector3f vMountains[12] = { 0.0f, 25.0f, 0.0f,
            50.0f, 100.0f, 0.0f,
            100.0f, 25.0f, 0.0f,
            225.0f, 125.0f, 0.0f,
            300.0f, 50.0f, 0.0f,
            375.0f, 100.0f, 0.0f,
            460.0f, 25.0f, 0.0f,
            525.0f, 100.0f, 0.0f,
            600.0f, 20.0f, 0.0f,
            675.0f, 70.0f, 0.0f,
            750.0f, 25.0f, 0.0f,
            800.0f, 90.0f, 0.0f
    };
    mountainRangeBatch.Begin(GL_LINE_STRIP, 12);
    mountainRangeBatch.CopyVertexData3f(vMountains);
    mountainRangeBatch.End();

    // 月亮
    GLfloat x = 700.0f;
    GLfloat y = 500.0f;
    GLfloat r = 50.0f;
    GLfloat angle = 0.0f;

    moonBatch.Begin(GL_TRIANGLE_FAN, 34);

    int nVerts = 0;
    vVerts[nVerts][0] = x;
    vVerts[nVerts][1] = y;
    vVerts[nVerts][2] = 0.0f;
    for (angle = 0; angle < 2.0f * 3.141592f; angle += 0.2f) {
        nVerts++;
        vVerts[nVerts][0] = x + float(cos(angle)) * r;
        vVerts[nVerts][1] = y + float(sin(angle)) * r;
        vVerts[nVerts][2] = 0.0f;
    }

    nVerts++;
    vVerts[nVerts][0] = x + r;
    vVerts[nVerts][1] = y;
    vVerts[nVerts][2] = 0.0f;
    moonBatch.CopyVertexData3f(vVerts);
    moonBatch.End();
    
    // 设置黑色背景
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void SpecialKeys(int key, int x, int  y) {
}

// 场景召唤
void RenderScene(void) {
    
    // 执行 clear（颜色缓存区、深度缓冲区）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GLfloat vWhite[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    // 使用存储着色器管理器中的平面着色器
    // 参数 1：平面着色器
    // 参数 2：模型视图投影矩阵
    // 参数 3：颜色，白色
    shaderManager.UseStockShader(GLT_SHADER_FLAT, viewFrustum.GetProjectionMatrix(),vWhite);
    
    // 绘制小星星，点的大小
    glPointSize(1.0f);
    smallStarBatch.Draw();
    
    // 绘制中星星
    glPointSize(4.0f);
    mediumStarBatch.Draw();
    
    // 绘制大星星
    glPointSize(8.0f);
    largeStarBatch.Draw();
    
    // 绘制月亮
    moonBatch.Draw();
    
    // 绘制遥远的地平线
    glLineWidth(3.5f);
    mountainRangeBatch.Draw();
    
    moonBatch.Draw();
    
    // 交换缓冲区
    glutSwapBuffers();
}

void ChangeSize(int w, int h) {
    if (h == 0) {
        h = 1;
    }
    glViewport(0, 0, w, h);
    viewFrustum.SetOrthographic(0.0f, SCREEN_X, 0.0f, SCREEN_Y, -1.0f, 1.0f);
}

}

int Day4_Demo1TestMain(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Smoothing Out The Jaggies");
    
    glutCreateMenu(Day4_Demo1::ProcessMenu);
    glutAddMenuEntry("Antialiased Rendering", 1);
    glutAddMenuEntry("Normal Rendering", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutReshapeFunc(Day4_Demo1::ChangeSize);
    glutDisplayFunc(Day4_Demo1::RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n",glewGetErrorString(err));
        return 1;
    }
    
    Day4_Demo1::SetupRC();
    glutMainLoop();
    return 0;
}
