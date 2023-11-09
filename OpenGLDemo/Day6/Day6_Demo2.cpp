//
//  Day6_Demo2.cpp
//  DemoOpenGL
// 通道
//  Created by KaraShokZ on 2023/10/2.
//

#include "Day6_Demo2.hpp"

namespace Day6_Demo2 {

// 着色器管理器
GLShaderManager shaderManager;

// 模型视图矩阵
GLMatrixStack modelViewMatrix;

// 投影矩阵
GLMatrixStack projectionMatrix;

// 视景体
GLFrustum viewFrustum;

// 几何变换管线
GLGeometryTransform transformPipeline;

// 地面
GLBatch floorBatch;

// 天花板
GLBatch ceilingBatch;

// 左墙面
GLBatch leftBatch;

// 右墙面
GLBatch rightBatch;

// 深度初始值
GLfloat viewZ = -65.0f;

// 纹理文件名符号
const int TEXTURE_BRICK = 0;
const int TEXTURE_FLOOR = 1;
const int TEXTURE_CEILING = 2;

// 纹理个数
const int TEXTURE_COUNT = 3;

// tga 文件名数组
const char *szTextureFiles[TEXTURE_COUNT] = {"brick.tga", "floor.tga", "ceiling.tga"};

// 纹理标记数组
GLuint textures[TEXTURE_COUNT];

// 菜单属性数组
char *title[8] = {
    "GL_NEAREST",
    " GL_LINEAR",
    "GL_NEAREST_MIPMAP_NEAREST",
    "GL_NEAREST_MIPMAP_LINEAR",
    "GL_LINEAR_MIPMAP_NEAREST",
    "GL_LINEAR_MIPMAP_LINEAR",
    "Anisotropic_ON",
    "Anisotropic_OFF"
};

bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode) {
    GLbyte *pBytes;
    GLint iWidth, iHeight, iComponents;
    GLenum eFormat;
    
    // 读取原始纹理信息
    pBytes = gltReadTGABits(szFileName, &iWidth, &iHeight, &iComponents, &eFormat);
    
    if (pBytes == NULL) {
        return false;
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    // 包装数据
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // 载入数据
    glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
    
    // 加载 Mip 纹理生成所有 Mip 层
    glGenerateMipmap(GL_TEXTURE_2D);
    
    free(pBytes);
    
    return true;
}

void SetupRC() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    
    // 分配纹理对象
    glGenTextures(TEXTURE_COUNT, textures);
    for (GLint i = 0; i < TEXTURE_COUNT; i++) {
        
        // 绑定纹理，指定后续操作的哪个纹理
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        LoadTGATexture(szTextureFiles[i], GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE);
    }
    
    GLfloat step = 10.0f;
    
    // 地面
    floorBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for (GLfloat z = 0.0f; z <= 60.0f; z += step) {
        floorBatch.MultiTexCoord2f(0, 0.0, 0.0);
        floorBatch.Vertex3f(10.0f, -10.0f, z);
        
        floorBatch.MultiTexCoord2f(0, 1.0, 0.0);
        floorBatch.Vertex3f(-10.0f, -10.0f, z);
        
        floorBatch.MultiTexCoord2f(0, 0.0, 1.0);
        floorBatch.Vertex3f(10.0f, -10.0f, z + step);
        
        floorBatch.MultiTexCoord2f(0, 1.0, 1.0);
        floorBatch.Vertex3f(-10.0f, -10.0f, z + step);
    }
    floorBatch.End();
    
    //天花板
    ceilingBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for (GLfloat z = 0.0f; z <= 60.0f; z += step) {
        ceilingBatch.MultiTexCoord2f(0, 1.0, 1.0);
        ceilingBatch.Vertex3f(-10.0f, 10.0f, z);
        
        ceilingBatch.MultiTexCoord2f(0, 0.0, 1.0);
        ceilingBatch.Vertex3f(10.0f, 10.0f, z);
        
        ceilingBatch.MultiTexCoord2f(0, 1.0, 0.0);
        ceilingBatch.Vertex3f(-10.0f, 10.0f, z + step);
        
        ceilingBatch.MultiTexCoord2f(0, 0.0, 0.0);
        ceilingBatch.Vertex3f(10.0f, 10.0f, z + step);
    }
    ceilingBatch.End();
    
    //左面墙壁
    leftBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for (GLfloat z = 0.0f; z <= 60.0f; z += step) {
        leftBatch.MultiTexCoord2f(0, 0.0, 1.0);
        leftBatch.Vertex3f(-10.0f, 10.0f, z);
        
        leftBatch.MultiTexCoord2f(0, 0.0, 0.0);
        leftBatch.Vertex3f(-10.0f, -10.0f, z);
        
        leftBatch.MultiTexCoord2f(0, 1.0, 1.0);
        leftBatch.Vertex3f(-10.0f, 10.0f, z + step);
        
        leftBatch.MultiTexCoord2f(0, 1.0, 0.0);
        leftBatch.Vertex3f(-10.0f, -10.0f, z + step);
    }
    leftBatch.End();
    
    //右面墙壁
    rightBatch.Begin(GL_TRIANGLE_STRIP, 28, 1);
    for (GLfloat z = 0.0f; z <= 60.0f; z += step) {
        rightBatch.MultiTexCoord2f(0, 0.0, 1.0);
        rightBatch.Vertex3f(10.0f, -10.0f, z);
        
        rightBatch.MultiTexCoord2f(0, 0.0, 0.0);
        rightBatch.Vertex3f(10.0f, 10.0f, z);
        
        rightBatch.MultiTexCoord2f(0, 1.0, 1.0);
        rightBatch.Vertex3f(10.0f, -10.0f, z + step);
        
        rightBatch.MultiTexCoord2f(0, 1.0, 0.0);
        rightBatch.Vertex3f(10.0f, 10.0f, z + step);
    }
    rightBatch.End();
}

void ShutdownRC() {
    // 删除纹理
    glDeleteTextures(TEXTURE_COUNT, textures);
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    
    viewFrustum.SetPerspective(60.0f, (float)w / (float)h, 1.0f, 100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        viewZ += 0.5f;
    }
    
    if (key == GLUT_KEY_DOWN) {
        viewZ -= 0.5f;
    }
    
    glutPostRedisplay();
}

void RenderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    modelViewMatrix.PushMatrix();
    
    // 移动
    modelViewMatrix.Translate(0.0f, 0.0f, viewZ);
    
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_REPLACE, transformPipeline.GetModelViewProjectionMatrix(),0);
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLOOR]);
    floorBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_CEILING]);
    ceilingBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BRICK]);
    leftBatch.Draw();
    
    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BRICK]);
    rightBatch.Draw();
    
    modelViewMatrix.PopMatrix();
    
    glutSwapBuffers();
}

void ProcessMenu(int value) {
    static GLint params1 = 0;
    static GLint params2 = 0;
    
    for (GLint i = 0; i < TEXTURE_COUNT; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        
        switch (value) {
            case 0:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                break;
            case 1:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                break;
            case 2:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                break;
            case 3:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                break;
            case 4:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                break;
            case 5:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                break;
            case 6:
                GLfloat fLargest;
                glGetFloatv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
                break;
            case 7:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
                break;
            default:
                break;
        }
    }
    
    glutPostRedisplay();
}
}

int Day6_Demo2TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pyramid");
    glutReshapeFunc(Day6_Demo2::ChangeSize);
    glutSpecialFunc(Day6_Demo2::SpecialKeys);
    glutDisplayFunc(Day6_Demo2::RenderScene);
    
    glutCreateMenu(Day6_Demo2::ProcessMenu);
    for (int i = 0; i < 8; i++) {
        glutAddMenuEntry(Day6_Demo2::title[i], i);
    }
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    Day6_Demo2::SetupRC();
    glutMainLoop();
    Day6_Demo2::ShutdownRC();
    
    return 0;
}
