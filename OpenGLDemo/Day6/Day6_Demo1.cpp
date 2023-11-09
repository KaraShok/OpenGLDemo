//
//  Day6_Demo1.cpp
//  DemoOpenGL
// 金字塔
//  Created by KaraShokZ on 2023/10/2.
//

#include "Day6_Demo1.hpp"

namespace Day6_Demo1 {

GLShaderManager shaderManager;
GLMatrixStack modelViewMatrix;
GLMatrixStack projectionMatrix;
GLFrame cameraFrame;
GLFrame objectFrame;
GLFrustum viewFrustum;
GLBatch pyramidBatch;
GLuint textureId;
GLGeometryTransform transformPipeline;
M3DMatrix44f shadowMatrix;

// 绘制金字塔
void MakePyramid(GLBatch &pyramidBatch) {
    
    // 通过pyramidBatch组建三角形批次
    // 参数1：类型
    // 参数2：顶点数
    // 参数3：这个批次中将会应用1个纹理(如果不写这个参数，默认为0)
    pyramidBatch.Begin(GL_TRIANGLES, 18, 1);
    
    // 各个顶点向量
    M3DVector3f vTop = {0.0f, 1.0f, 0.0f};
    M3DVector3f vLeftFront = {-1.0f, -1.0f, -1.0f};
    M3DVector3f vRightFront = {1.0f, -1.0f, -1.0f};
    M3DVector3f vLeftBack = {-1.0f, -1.0f, 1.0f};
    M3DVector3f vRightBack = {1.0f, -1.0f, 1.0f};
    
    // 法线向量
    M3DVector3f vNormal;
    
    /*
     要正确使用光照，需要设置法线
     要正确的把纹理设置到物体上，需要设置纹理坐标
     */
    /*
     设置纹理坐标
     MultiTexCoord2f(GLuint texture, GLclampf s, GLclampf t);
     参数1：texture，纹理层次，对于使用存储着色器来进行渲染，设置为0
     参数2：s：对应顶点坐标中的x坐标
     参数3：t:对应顶点坐标中的y
     (s,t,r,q对应顶点坐标的x,y,z,w)
     */
    
    // 前面三角形(观察者方向正面绘制纹理)
    // 获取法线
    m3dFindNormal(vNormal, vTop, vRightFront, vLeftFront);
    // 设置法线
    pyramidBatch.Normal3fv(vNormal);
    // 设置纹理坐标
    pyramidBatch.MultiTexCoord2f(0, 0.5, 1.0);
    // 设置顶点
    pyramidBatch.Vertex3fv(vTop);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 0.0);
    pyramidBatch.Vertex3fv(vRightFront);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 0.0);
    pyramidBatch.Vertex3fv(vLeftFront);
    
    //后面三角形(观察者方向背面绘制纹理)
    m3dFindNormal(vNormal, vTop, vLeftBack, vRightBack);
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.5, 1.0);
    pyramidBatch.Vertex3fv(vTop);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 0.0);
    pyramidBatch.Vertex3fv(vLeftBack);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 0.0);
    pyramidBatch.Vertex3fv(vRightBack);
    
    //左面三角形 观察者方向背面绘制纹理
    m3dFindNormal(vNormal, vTop, vRightBack, vRightFront);
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.5, 1.0);
    pyramidBatch.Vertex3fv(vTop);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 0.0);
    pyramidBatch.Vertex3fv(vRightBack);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 0.0);
    pyramidBatch.Vertex3fv(vRightFront);
    
    //右面三角形 观察者方向背面绘制纹理
    m3dFindNormal(vNormal, vTop, vLeftFront, vLeftBack);
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.5, 1.0);
    pyramidBatch.Vertex3fv(vTop);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 0.0);
    pyramidBatch.Vertex3fv(vLeftFront);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 0.0);
    pyramidBatch.Vertex3fv(vLeftBack);
    
    //底部正方形（2个三角形）观察者方向背面绘制纹理
    m3dFindNormal(vNormal, vLeftBack, vLeftFront, vRightFront);
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 1.0);
    pyramidBatch.Vertex3fv(vLeftBack);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 0.0);
    pyramidBatch.Vertex3fv(vLeftFront);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 0.0);
    pyramidBatch.Vertex3fv(vRightFront);
    
    m3dFindNormal(vNormal, vRightFront, vRightBack, vLeftBack);
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 0.0);
    pyramidBatch.Vertex3fv(vRightFront);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 1.0, 1.0);
    pyramidBatch.Vertex3fv(vRightBack);
    
    pyramidBatch.Normal3fv(vNormal);
    pyramidBatch.MultiTexCoord2f(0, 0.0, 1.0);
    pyramidBatch.Vertex3fv(vLeftBack);
    
    pyramidBatch.End();
}

// 加载文件为 2D 纹理
bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode) {
    GLbyte *pBits;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;
    
    // 读取像素
    // 参数1：szFileName，纹理文件的名称
    // 参数2：iWidth，纹理的宽度
    // 参数3：iHeight，纹理的高度
    // 参数4：iComponents，纹理的组件地址
    // 参数5：eFormat，文件的格式地址
    // 返回值：数据指针
    pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
    
    if (pBits == NULL) {
        return false;
    }
    
    // 设置纹理参数
    
    // 过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
    // 环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    
    // 包装数据
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // 载入纹理
    glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
    
    // 释放对象
    free(pBits);
    
    // minFilter 等于以下四种模式，才可以生成 Mip 贴图
    // GL_LINEAR_MIPMAP_LINEAR 三线性 Mip 贴图，纹理过滤的黄金准则，具有最高的精度
    // GL_LINEAR_MIPMAP_NEAREST 常常用于对游戏进行加速，它使用了高质量的线性过滤器
    // GL_LINEAR_MIPMAP_LINEAR 和 GL_NEAREST_MIPMAP_LINEAR 过滤器在 Mip 层之间执行了一些额外的插值，以消除他们之间的过滤痕迹
    // GL_NEAREST_MIPMAP_NEAREST 具备非常好的性能，并且闪烁现象非常弱
    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST) {
        
        // 加载 Mip 纹理生成所有 Mip 层
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    return true;
}

void SetupRC() {
    
    glClearColor(0.7, 0.7, 0.7, 1.0);//设置清屏颜色
    shaderManager.InitializeStockShaders();
    glEnable(GL_DEPTH_TEST);
    
    LoadTGATexture("brick.tga", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
    
    MakePyramid(pyramidBatch);
    
    // 调整观察者
    cameraFrame.MoveForward(-10.0f);
}

void ShutdownRC(void) {
    glDeleteTextures(1, &textureId);
}

void RenderScene(void) {
    static GLfloat vLightPos [] = { 2.0f, 2.0f, 0.0f };//光源位置
    static GLfloat vWhite [] = { 1.0f, 1.0f, 1.0f, 1.0f };//基本漫反射颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 压栈
    modelViewMatrix.PushMatrix();
    
    // 模型变换(平移/旋转)
    M3DMatrix44f mCamera;
    cameraFrame.GetCameraMatrix(mCamera);
    modelViewMatrix.MultMatrix(mCamera);
    
    M3DMatrix44f mObject;
    objectFrame.GetMatrix(mObject);
    modelViewMatrix.MultMatrix(mObject);
    
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    //使用点光源着色器
    shaderManager.UseStockShader(GLT_SHADER_TEXTURE_POINT_LIGHT_DIFF,//点光源着色器标记
                                 transformPipeline.GetModelViewMatrix(),//模型视图矩阵
                                 transformPipeline.GetProjectionMatrix(),//投影矩阵
                                 vLightPos,//视点坐标系中的光源位置
                                 vWhite,//基本漫反射颜色
                                 0);//图形颜色（用纹理就不需要设置颜色。设置为0）
    // 绘制
    pyramidBatch.Draw();
    
    //出栈
    modelViewMatrix.PopMatrix();
    
    //交换缓存区
    glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y) {
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

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
    
    viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

}

int Day6_Demo1TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pyramid");
    glutReshapeFunc(Day6_Demo1::ChangeSize);
    glutSpecialFunc(Day6_Demo1::SpecialKeys);
    glutDisplayFunc(Day6_Demo1::RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    
    Day6_Demo1::SetupRC();
    glutMainLoop();
    Day6_Demo1::ShutdownRC();
    return 0;
}
