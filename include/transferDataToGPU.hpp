void transferDataToGPU(GLuint* VertexArrayID, GLuint programID, const char* textureName, const char* objectName, GLuint MatrixID, GLuint* Texture, GLuint* TextureID, GLuint* vertexbuffer, GLuint* uvbuffer, std::vector<glm::vec3>* vertices, std::vector<glm::vec2>* uvs, std::vector<glm::vec3>* normals) {
    glGenVertexArrays(1, VertexArrayID);
    glBindVertexArray((*VertexArrayID));

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");

    // Load the texture
    *Texture = loadDDS(textureName);

    // Get a handle for our "myTextureSampler" uniform
    *TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // Read our .obj file
    bool res = loadOBJ(objectName, *vertices, *uvs, *normals);

    // Load it into a VBO

    glGenBuffers(1, vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(glm::vec3), &vertices[0][0], GL_STATIC_DRAW);

    glGenBuffers(1, uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs->size() * sizeof(glm::vec2), &uvs[0][0], GL_STATIC_DRAW);
}


//TD2GPU
void transferDataToGPU2D_HUD(GLuint *VertexArrayID2D, GLuint programID2D, GLuint *vertexbuffer2D, GLuint *colorbuffer2D, GLuint *Texture2D, GLuint *TextureID2D, const char *textureName)
{

    //Stuff
    GLfloat g_vertex_buffer_data[] = {
        //rectangle
        -1.0f, -1.0f, 0.0f,
        -1.0f, -0.8f, 0.0f,
        1.0f,  -1.0f, 0.0f,

        -1.0f, -0.8f, 0.0f,
        1.0f, -0.8f, 0.0f,
        1.0f,  -1.0f, 0.0f,
    };

    //white color
    GLfloat g_uv_buffer_data[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f,  1.0f
    };
	// VAO
	glGenVertexArrays(1, VertexArrayID2D);
	glBindVertexArray(*VertexArrayID2D);

    // Load the texture
    *Texture2D = loadDDS(textureName);

    // Get a handle for our "myTextureSampler" uniform
    *TextureID2D = glGetUniformLocation(programID2D, "myTextureSampler2D");

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, vertexbuffer2D);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer2D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, colorbuffer2D);
    glBindBuffer(GL_ARRAY_BUFFER, *colorbuffer2D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}

void transferDataToGPU2D_Menu(GLuint* VertexArrayID2D, GLuint programID2D, GLuint* vertexbuffer2D, GLuint* colorbuffer2D, GLuint* Texture2D, GLuint* TextureID2D, const char* textureName)
{

    //Stuff
    GLfloat g_vertex_buffer_data[] = {
        //rectangle
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,
    };

    //white color
    GLfloat g_uv_buffer_data[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f,  1.0f
    };
    // VAO
    glGenVertexArrays(1, VertexArrayID2D);
    glBindVertexArray(*VertexArrayID2D);

    // Load the texture
    *Texture2D = loadDDS(textureName);

    // Get a handle for our "myTextureSampler" uniform
    *TextureID2D = glGetUniformLocation(programID2D, "myTextureSampler2D");

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, vertexbuffer2D);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer2D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, colorbuffer2D);
    glBindBuffer(GL_ARRAY_BUFFER, *colorbuffer2D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
}

