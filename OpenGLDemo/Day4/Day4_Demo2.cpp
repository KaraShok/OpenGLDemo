//
//  Day4_Demo2.cpp
//  DemoOpenGL
// 图形移动（矩阵变换）
//  Created by KaraShokZ on 2023/9/17.
// 实现矩阵的移动，利用矩阵的平移、旋转、综合变化等

#include "Day4_Demo2.hpp"

namespace Day4_Demo2 {

GLBatch squareBatch;
GLShaderManager shaderManager;

GLfloat blockSize = 0.1f;
GLfloat vVerts[] = {
    -blockSize, -blockSize, 0.0f,
    blockSize, -blockSize, 0.0f,
    blockSize, blockSize, 0.0f,
    -blockSize, blockSize, 0.0f
};
GLfloat xPos = 0.0f;
GLfloat yPos = 0.0f;

M3DVector3f vVector;
M3DVector4f vVector4 = {0.0f, 0.0f, 0.0f,1.0f};
M3DVector3f vVert[] = {
    -0.5f,0.0f,0.0f,
    0.5f,0.0f,0.0f,
    0.0f,0.5f,0.0f
};

void SetupRC() {
    M3DVector3f v1 = {1.0f, 0.0f, 0.0f};
    M3DVector3f v2 = {0.0f, 1.0f, 0.0f};
    
    GLfloat value1 = m3dDotProduct3(v1, v2);
    printf("v1 v2 余弦值：%d\n", value1);
    
    GLfloat value2 = acos(value1);
    printf("v1 v2 弧度值：%d\n", value2);
    
    GLfloat value3 = m3dGetAngleBetweenVectors3(v1, v2);
    printf("v1 v2 弧度值：%d\n", value3);
    
    GLfloat value4 = m3dRadToDeg(value3);
    printf("v1 v2 角度值：%d\n", value4);
    
    GLfloat value5 = m3dDegToRad(90);
    printf("弧度值：%d\n", value5);
    
    M3DVector3f vector2 = {0.0f, 0.0f, 0.0f};
    m3dCrossProduct3(vector2, v1, v2);
    printf("%f, %f, %f\n", vector2[0], vector2[1], vector2[2]);
    
    M3DMatrix33f mv1;
    M3DMatrix44f mv2;
    
    //创建单元矩阵
    GLfloat m[] = {
        // X列
        1.0f,0.0f,0.0f,0.0f,
        
        // Y列
        0.0f,1.0f,0.0f,0.0f,
        
        // Z列
        0.0f,0.0f,1.0f,0.0f,
        
        // 变换
        0.0f,0.0f,0.0f,1.0f
    };
        
    //创建单元矩阵
    M3DMatrix44f m2 = {
        // X列
        1.0f,0.0f,0.0f,0.0f,
        
        // Y列
        0.0f,1.0f,0.0f,0.0f,
        
        // Z列
        0.0f,0.0f,1.0f,0.0f,
        
        //变换
        0.0f,0.0f,0.0f,1.0f
    };
    
    M3DMatrix44f m3;
    m3dLoadIdentity44(m3);
    m3dTranslationMatrix44(m2, 0.0f, 10.0f, 0.0f);
    
    m3dRotationMatrix44(m2, m3dDegToRad(45.0f), 1.0f, 0.0f, 0.0f);
    
    m3dScaleMatrix44(m2, 1.0f, 10.0f, 1.0f);
    
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    
    shaderManager.InitializeStockShaders();
    
    squareBatch.Begin(GL_TRIANGLE_FAN, 4);
    squareBatch.CopyVertexData3f(vVerts);
    squareBatch.End();
}

void SpecialKeys(int key, int x, int  y) {
    GLfloat stepSize = 0.025f;
    
    if(key == GLUT_KEY_UP)
        yPos += stepSize;
    
    if(key == GLUT_KEY_DOWN)
        yPos -= stepSize;
    
    if(key == GLUT_KEY_LEFT)
        xPos -= stepSize;
    
    if(key == GLUT_KEY_RIGHT)
        xPos += stepSize;
    
    // 碰撞检测
    if(xPos < (-1.0f + blockSize)) xPos = -1.0f + blockSize;
    
    if(xPos > (1.0f - blockSize)) xPos = 1.0f - blockSize;
    
    if(yPos < (-1.0f + blockSize))  yPos = -1.0f + blockSize;
    
    if(yPos > (1.0f - blockSize)) yPos = 1.0f - blockSize;
    
    glutPostRedisplay();
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    M3DMatrix44f mFinalTransform, mTranslationMatrix, mRotationMatrix;
    
    m3dTranslationMatrix44(mTranslationMatrix, xPos, yPos, 0.0f);
    
    static float yRot =0.0f;
    yRot += 5.0f;
    m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(yRot), 0.0f, 0.0f, 1.0f);
    
    m3dMatrixMultiply44(mFinalTransform, mTranslationMatrix, mRotationMatrix);
    
    shaderManager.UseStockShader(GLT_SHADER_FLAT, mFinalTransform, vRed);
    squareBatch.Draw();
    
    glutSwapBuffers();
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
}

}

int Day4_Demo2TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Move Block with Arrow Keys");
    
    GLenum err = glewInit();
    if (GLEW_OK != err){
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    glutReshapeFunc(Day4_Demo2::ChangeSize);
    glutDisplayFunc(Day4_Demo2::RenderScene);
    glutSpecialFunc(Day4_Demo2::SpecialKeys);
    
    Day4_Demo2::SetupRC();
    glutMainLoop();
    return 0;
}
