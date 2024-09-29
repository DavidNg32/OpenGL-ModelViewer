#pragma once

#include "GLAD/include/glad/glad.h"
#include "glm/include/glm/glm.hpp"
#include "shader.h"
#include <vector>
#include <string>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Texture{
    GLuint id;
    std::string type;
    std::string path;
};


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    
    void draw(Shader &shader);
    
private:
    GLuint VertexArrayObject;
    GLuint VertexBufferObject;
    GLuint ElementBufferObject;
    
    void setupMesh();
};

