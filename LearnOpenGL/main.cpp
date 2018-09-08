#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <iostream>

// We need this to be able to resize our window.
void framebuffer_size_callback( GLFWwindow* window, int width, int height );
// We need this to be able to close our window when pressing a key.
void processInput( GLFWwindow* window );

// Our image size.
const int WIDTH = 1768;
const int HEIGHT = 992;

int main() 
{
	
	// We initialize glfw and specify which versions of OpenGL to target.
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// Our window object.
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;

	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load glad, we need this library to specify system specific functions.
	if( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
	{

		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;

	}

	// We build and compile our shader program.
	Shader ourShader( "vertex.glsl", "waves.glsl" );

	// We define the points in space that we want to render.
	float vertices[] =
	{

		
		// Positions.            // FragCoord
		-1.0f,  -1.0f, 0.0f,     //-1.0f,  1.0f,
		-1.0f,   1.0f, 0.0f,     //1.0f,  1.0f,
		 1.0f,   1.0f, 0.0f,     //1.0f, -1.0f,
		 1.0f,  -1.0f, 0.0f      //-1.0f, -1.0f 
		 
		/*
		0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
		*/

	};

	// We define our Element Buffer Object indices so that if we have vertices that overlap,
	// we dont have to render twice, 50% overhead.
	unsigned int indices[] = 
	{
	
		//0, 2, 3, 4, 0
		0, 1, 3,
		1, 2, 3

	};

	// We create a buffer ID so that we can later assign it to our buffers.
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &EBO );

	// Bind Vertex Array Object.
	glBindVertexArray( VAO );

	// Copy our vertices array in a buffer for OpenGL to use.
	// We assign our buffer ID to our new buffer and we overload it with our triangles array.
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	// Copy our indices in an array for OpenGL to use.
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	// Set our vertex attributes pointers.
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), ( void* ) 0 );
	glEnableVertexAttribArray( 0 );

	// Unbind the VBO.
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	// Unbind the VAO.
	glBindVertexArray( 0 );

	// Render Loop.
	while( !glfwWindowShouldClose( window ) )
	{
	
		// Input.
		processInput( window );

		// Render.
		glClearColor( 0.2f, 0.3f, 0.1f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		// Draw our triangle.
		ourShader.use();

		// Set the iTime uniform.
		float timeValue = glfwGetTime();
		ourShader.setFloat("iTime", timeValue);

		// Set the iResolution uniform.
		ourShader.setVec2( "iResolution", WIDTH, HEIGHT );

		glBindVertexArray( VAO );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

		glfwSwapBuffers( window );
		glfwPollEvents();
	
	}

	// De-allocate all resources once they've outlived their purpose.
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &EBO );

	glfwTerminate();
	return 0;
}

void processInput( GLFWwindow *window )
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose( window, true );

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);

}