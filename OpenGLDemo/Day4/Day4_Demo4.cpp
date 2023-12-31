//
//  Day4_Demo4.cpp
//  DemoOpenGL
// 透视投影
//  Created by KaraShokZ on 2023/9/18.
//

#include "Day4_Demo4.hpp"

namespace Day4_Demo4 {

GLFrame             viewFrame;
GLFrustum           viewFrustum;
GLBatch             tubeBatch;
GLBatch             innerBatch;
GLMatrixStack       modelViewMatix;
GLMatrixStack       projectionMatrix;
GLGeometryTransform transformPipeline;
GLShaderManager     shaderManager;

void SetupRC() {
    glClearColor(0.0f, 0.0f, 0.75f, 1.0f );
        
       
        glEnable(GL_DEPTH_TEST);
        
        shaderManager.InitializeStockShaders();
        viewFrame.MoveForward(450.0f);
    
    
    tubeBatch.Begin(GL_QUADS, 200);
        
        float fZ = 100.0f;
        float bZ = -100.0f;
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, 100.0f);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f,50.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(50.0f,-50.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, 35.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 35.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 50.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -35.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -35.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f,50.0f,bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f,50.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(50.0f,-50.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, 35.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 35.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 50.0f,fZ);
      
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -35.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -35.0f,fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, 1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f,50.0f,bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(0.0f, -1.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(50.0f, 50.0f, bZ);
       
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, fZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, bZ);
        
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, fZ);
        

        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f,50.0f,bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -50.0f, bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-50.0f, -50.0f, bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-50.0f, 50.0f, bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        
        tubeBatch.Vertex3f(50.0f,-50.0f,bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        
        tubeBatch.Vertex3f(35.0f, -50.0f, bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        
        tubeBatch.Vertex3f(35.0f, 50.0f, bZ);
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        
        tubeBatch.Vertex3f(50.0f, 50.0f, bZ);
      
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 50.0f, bZ);
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, 35.0f, bZ);
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, 35.0f, bZ);
        
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, 50.0f, bZ);
        
       
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -35.0f,bZ);
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(35.0f, -50.0f, bZ);
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -50.0f, bZ);
        
        
        tubeBatch.Normal3f(0.0f, 0.0f, -1.0f);
        tubeBatch.Color4f(1.0f, 0.0f, 0.0f, 1.0f);
        tubeBatch.Vertex3f(-35.0f, -35.0f, bZ);
        
        tubeBatch.End();
        
        
        innerBatch.Begin(GL_QUADS, 40);
        
        
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, 35.0f, fZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, 35.0f, fZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, 35.0f, bZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f,35.0f,bZ);
        
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, -35.0f, fZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, -35.0f, bZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, -35.0f, bZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(0.0f, 1.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, -35.0f, fZ);
        
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, 35.0f, fZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, 35.0f, bZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, -35.0f, bZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(-35.0f, -35.0f, fZ);
        
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, 35.0f, fZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, -35.0f, fZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, -35.0f, bZ);
        innerBatch.Color4f(0.75f, 0.75f, 0.75f, 1.0f);
        innerBatch.Normal3f(-1.0f, 0.0f, 0.0f);
        innerBatch.Vertex3f(35.0f, 35.0f, bZ);
        
        innerBatch.End();
}

void SpecialKeys(int key, int x, int  y) {
    if(key == GLUT_KEY_UP)
            viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);
        
        if(key == GLUT_KEY_DOWN)
            viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);
        
        if(key == GLUT_KEY_LEFT)
            viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);
        
        if(key == GLUT_KEY_RIGHT)
            viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);
        
        // Refresh the Window
        glutPostRedisplay();
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    modelViewMatix.PushMatrix(viewFrame);
    
    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat vGray[] = {0.75f, 0.75f, 0.75f, 1.0f};
    
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vRed);
    tubeBatch.Draw();
    
    shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vGray);
    innerBatch.Draw();
    
    modelViewMatix.PopMatrix();
    
    glutSwapBuffers();
}

void ChangeSize(int w, int h) {
    if (h == 0) {
        h = 1;
    }
    
    glViewport(0, 0, w, h);
    
    viewFrustum.SetPerspective(35.0f, (float)w/(float)h, 1.0f, 1000.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    transformPipeline.SetMatrixStacks(modelViewMatix, projectionMatrix);
}

}

int Day4_Demo4TestMain(int argc, char *argv[]) {
    gltSetWorkingDirectory(argv[0]);
        
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Perspective Projection Example");
        glutReshapeFunc(Day4_Demo4::ChangeSize);
        glutSpecialFunc(Day4_Demo4::SpecialKeys);
        glutDisplayFunc(Day4_Demo4::RenderScene);
        
        
        GLenum err = glewInit();
        if (GLEW_OK != err) {
            fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
            return 1;
        }
        
       Day4_Demo4::SetupRC();
        
        glutMainLoop();
        return 0;
}
