void draw(GLuint programID, GLuint MatrixID, GLuint Texture, GLuint TextureID, GLuint* vertexbuffer, GLuint* uvbuffer, GLuint primitive, std::vector<glm::vec3>* verticesName, Astro Tpl, Rotation Rpl) {
	// Use our shader
	glUseProgram(programID);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	//Rotation
	glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(Tpl.currentRotation), glm::vec3(Rpl.rotX, Rpl.rotY, Rpl.rotZ));
	unsigned int r1 = glGetUniformLocation(programID, "rot");
	glUniformMatrix4fv(r1, 1, GL_FALSE, &rot[0][0]);

	//Self-Rotation no eixo dos Z
	glm::mat4 selfrotZ = glm::rotate(glm::mat4(1.0), glm::radians(Tpl.selfRotationZ), glm::vec3(0, 0, 1));
	unsigned int r2 = glGetUniformLocation(programID, "selfrotZ");
	glUniformMatrix4fv(r2, 1, GL_FALSE, &selfrotZ[0][0]);

	//Self-Rotation no eixo dos X
	glm::mat4 selfrotX = glm::rotate(glm::mat4(1.0), glm::radians(Tpl.selfRotationX), glm::vec3(1, 0, 0));
	unsigned int r3 = glGetUniformLocation(programID, "selfrotX");
	glUniformMatrix4fv(r3, 1, GL_FALSE, &selfrotX[0][0]);

	//Translação
	glm::mat4 trans;
	trans = glm::translate(glm::mat4(1.0), glm::vec3(Tpl.posX, Tpl.posY, Tpl.posZ));
	unsigned int m = glGetUniformLocation(programID, "trans");
	glUniformMatrix4fv(m, 1, GL_FALSE, &trans[0][0]);

	//Scale
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(Tpl.scale, Tpl.scale, Tpl.scale));//no glm::vec3 os argumentos são (scale x, scale y, scale z)
	unsigned int n = glGetUniformLocation(programID, "scale");
	glUniformMatrix4fv(n, 1, GL_FALSE, &scale[0][0]);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, *uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(primitive, 0, verticesName->size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void draw2D(GLuint vertexbuffer2D, GLuint texturebuffer2D, GLuint programID2D)
{
	//Making sure it´s 2D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1024 * 1.2, 768 * 1.2, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(10.0, 0.0);
	glVertex2f(10.0, 10.0);
	glVertex2f(0.0, 10.0);
	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	// Use our shader
	glUseProgram(programID2D);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2D);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : texture
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer2D);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}