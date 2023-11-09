//
//  Day5_Demo3.cpp
//  DemoOpenGL
//
//  Created by KaraShokZ on 2023/9/30.
//

#include "Day5_Demo3.hpp"

namespace Day5_Demo3 {

GLFrustum viewFrustum;
GLShaderManager shaderManager;
GLTriangleBatch torusBatch;

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 1000.0f);
}

void RenderScene(void) {
    static CStopWatch rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    M3DMatrix44f mTranslate, mRotate, mModelView, mModelViewProjection;
    m3dTranslationMatrix44(mTranslate, 0.0f, 0.0f, -2.5f);
    m3dRotationMatrix44(mRotate, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
    m3dMatrixMultiply44(mModelView, mTranslate, mRotate);
    m3dMatrixMultiply44(mModelViewProjection, viewFrustum.GetProjectionMatrix(), mModelView);
    
    GLfloat vBlack[] = {0.0f, 0.0f, 0.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mModelViewProjection,vBlack);
    torusBatch.Draw();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void SetupRC() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    shaderManager.InitializeStockShaders();
    
    gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);
    
    gltMakeSphere(torusBatch, 0.4f, 10, 20);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
}

int Day5_Demo3TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
        
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("ModelViewProjection Example");
    glutReshapeFunc(Day5_Demo3::ChangeSize);
    glutDisplayFunc(Day5_Demo3::RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    Day5_Demo3::SetupRC();
    
    glutMainLoop();
    return 0;
}
