#include "GeometryUtil.h"
#include <vector>

// This is not a optimized way to get vertices
std::vector<float> CreateCube() {
	std::vector<float> vertices =
	{
		 // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right             
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top-left
        // Front face       
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom-right             
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-left
        // Left face        
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-right              
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-right
        // Right face       
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right             
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right                   
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right             
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-left                 
        // Bottom face      
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-right              
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
        // Top face         
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // bottom-right             
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right              
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // bottom-right             
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f  // bottom-left 
	};
	return vertices;
}

std::vector<float> CreateCubeWithNormal()
{
	std::vector<float> vertices = {
		 // Back face
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // bottom-right             
         0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // top-left
        // Front face        
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // bottom-right             
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
        // Left face         
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-right              
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-right
        // Right face        
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right             
         0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // top-right                   
         0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // bottom-right             
         0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // bottom-left                 
        // Bottom face       
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // bottom-right              
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // top-right
        // Top face          
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right             
         0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // top-right              
         0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // bottom-right             
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // bottom-left 
	};
	return vertices;
}

std::vector<float> CreatePlane() {
	std::vector<float> planeVertices = {
        // positions         //Normal      // texture Coords
         0.5f, 0.0f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.0f,  0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, 0.0f,  0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, 0.0f, -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f								
    };
	return planeVertices;
}

std::vector<float> CreateScreenQuad() {
    std::vector<float> screenQuadVertices = {
        // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    
    };

    return screenQuadVertices;
}