#pragma once
// OpenGL libs

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>

#include <string>
#include <vector>

#include "Shader.h"

using namespace std;

// geometrijski objekt
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
    string type;
    string path;
};



// mesh
class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<GLuint>       indices;
        vector<Texture>      textures;

        Mesh();
        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
        void Draw(Shader &shader);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};









