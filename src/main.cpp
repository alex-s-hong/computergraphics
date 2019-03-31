// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"

// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>

// Linear Algebra Library
#include <cstdlib>
#include <ctime>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <math.h>

// Timer
#include <chrono>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// VertexBufferObject wrapper
VertexBufferObject VBO;
VertexBufferObject VBO_tex;


bool first = false;
bool second = false;
bool gameon = false;
int counter = 0;
double ballspeed = 0.0012;
// Contains the vertex positions
Eigen::MatrixXf V(2,30);
Eigen::MatrixXf Tex(2, 30);

Eigen::MatrixXf B1(2, 12);
Eigen::MatrixXf B2(2, 12);
Eigen::MatrixXf B3(2, 12);
Eigen::MatrixXf B4(2, 12);
Eigen::MatrixXf Ball(2, 6);
float one = 0.581395f;//0.166667f;
float left = 28.0f / 600.0f;
float right = 332.0f/600.0f;
float top = 0.96744186f;
float bottom = 0.61395349f;

void uv_cordinate_init() {
	Tex <<
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, left, left, right, right, left, right,
		one, 0.0f, one, one, 0.0f, 0.0f, one, 0.0f, one, one, 0.0f, 0.0f, one, 0.0f, one, one, 0.0f, 0.0f, one, 0.0f, one, one, 0.0f, 0.0f, top, bottom, top, top, bottom, bottom;
	VBO_tex.update(Tex);
	//std::cout << Tex << std::endl;
}


void barrier_impl() {
	//barrier 1
	B1 << //problem here
		-1.0f, -1.0f, -0.4f, -0.4f, -1.0f, -0.4f, 0.4f, 0.4f, 1.0f, 1.0f, 0.4f, 1.0f,
		1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f, 1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f;
	//std::cout << "B1\n" << B1 << std::endl;
	//barrier 2
	B2 <<
		-1.0f, -1.0f, -0.4f, -0.4f, -1.0f, -0.4f, -0.4f, -0.4f, 0.2f, 0.2f, -0.4f, 0.2f,
		1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f, 1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f;
	//std::cout << "B2\n" << B2 << std::endl;
	//barrier 3
	B3 <<
		-0.6f, -0.6f, 0.2f, 0.2f, -0.6f, 0.2f, 0.2f, 0.2f, 1.0f, 1.0f, 0.2f, 1.0f,
		1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f, 1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f;
	//std::cout << "B3\n" << B3 << std::endl;
	//barrier 4
	B4 <<
		-0.4f, -0.4f, 0.0f, 0.0f, -0.4f, 0.0f, 0.0f, 0.0f, 0.4f, 0.4f, 0.0f, 0.4f,
		1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f, 1.2f, 1.1f, 1.2f, 1.2f, 1.1f, 1.1f;
	//std::cout << "B4\n" << B4 << std::endl;
}

void ball_init() {
	Ball <<
//		-0.2f, -0.2f, 0.2f, 0.2f, -0.2f, 0.2f,
//		-0.7f, -0.98f, -0.7f, -0.7f, -0.98f, -0.98f;
		-0.05f, -0.05f, 0.05f, 0.05f, -0.05f, 0.05f,
		-0.8f, -0.87f, -0.8f, -0.8f, -0.87f, -0.87f;
}


void barrier_putter(int index){
	
	srand((unsigned int)time(NULL));
	int ran = rand() % 4;
	int position;
	//std::cout << "random number"<<ran << std::endl;
	
	if (index == 1) {
		position = 0;
		first = true;
	}
	else { //index == 2
		position = 12;
		second = true;
	}
	// position is either 0 or 12
	if (ran == 0) {
		for (int i = 0; i < 12; i++) {
			V.col(position + i) << B1.col(i);
		}
	}
	else if (ran == 1) {
		for (int i = 0; i < 12; i++) {
			V.col(position + i) << B2.col(i);
		}
	}
	else if (ran == 2) {
		for (int i = 0; i < 12; i++) {
			V.col(position + i) << B3.col(i);
		}
	}
	else {
		for (int i = 0; i < 12; i++) {
			V.col(position + i) << B4.col(i);
		}
	}
}


void reset() {
	gameon = false;
	first = false;
	second = false;
	for (int i = 0; i < 24; i++) {
		V.col(i) << 0.0, -1.2;
	}
	for (int i = 24; i < 30; i++) {
		V.col(i) << Ball.col(i - 24);
	}
	VBO.update(V);
	std::cout << "reset" << std::endl;
}

void onestep(double speed) {
	//std::cout << "onestep" << std::endl;
	if (first) {
		for (int i = 0; i < 12; i++) {
			V(1, i) = V(1, i) - speed; //0.01;
		}
	}
	if (second) {
		for (int i = 12; i < 24; i++) {
			V(1, i) = V(1, i) - speed; // 0.01;
		}
	}
	VBO.update(V);
}

void collision_check(int first, int second, int third) {
	// 3, 4, 5, 9, 10, 11; +12 for each (15 16 17 21 22 23) with ball box(or their midpoint)
	//barycentric rasterization
	
	Eigen::Vector3f a; 
	Eigen::Vector3f b;
	Eigen::Vector3f c; 
	Eigen::Vector3f p; // 4 points of the ball
	
	Eigen::Vector3f alpha;
	Eigen::Vector3f beta;
	Eigen::Vector3f gamma;
	Eigen::Vector3f pc;
	Eigen::Vector3f pb;
	Eigen::Vector3f pa;

	c << V.col(third), 0;
	b << V.col(second), 0;
	a << V.col(first), 0;
	//first case
	p << V.col(24), 0;
	pc = p - c;
	pb = p - b;
	pa = p - a;
	alpha = pb.cross(pc);
	beta = pc.cross(pa);
	gamma = pa.cross(pb);
	float a1 = alpha(2);
	float a2 = beta(2);
	float a3 = gamma(2);

	if (a1 * a2 > 0 && a2 * a3 > 0) {
		gameon = false;
		counter = 0;
		ballspeed = 0.001;
	}
	else {
		//other 3 points
		for (int j = 27; j < 30; j++) {
			p << V.col(j), 0;
			pc = p - c;
			pb = p - b;
			pa = p - a;
			alpha = pb.cross(pc);
			beta = pc.cross(pa);
			gamma = pa.cross(pb);
			a1 = alpha(2);
			a2 = beta(2);
			a3 = gamma(2);
			if (a1 * a2 > 0 && a2 * a3 > 0) {
				gameon = false;
				counter = 0;
				ballspeed = 0.0012;
				break;
			}

		}
	}



	//using a circle, implicit rasterization
	//
	// center = mid point of 27, 28
	//double center_x = (V(0, 27) + V(0, 28)) / 2;
	//double center_y = (V(1, 27) + V(1, 28)) / 2;



}

void swap() {
	if (first) {
		if (V(1, 0) <= -1.0 && V(1.0)> -1.1) {
			barrier_putter(2);
			//std::cout << "second slot assigned" << std::endl;
			//counter++;
		}
		else if (V(1, 0) <= -1.1) {
			first = false;
			counter++;
			std::cout << "counter" << counter<< std::endl;
			//if (counter % 10 == 0) {
			//	ballspeed += 0.0005;
			//	std::cout << "ballspeed! " << ballspeed << std::endl;
			//}
			//std::cout << "first slot cleared" << std::endl;
		}
	}
	if (second) {
		if (V(1, 12) <= -1.0 && V(1,12)> -1.1) {
			barrier_putter(1);
			//counter++;
		}
		else if (V(1, 12) <= -1.1) {
			second = false;
			counter++;
			std::cout << "counter" << counter << std::endl;
			if (counter % 10 == 0) {
				ballspeed += 0.0003;
				std::cout << "ballspeed " << ballspeed << std::endl;
			}
		}
	}

}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Eigen::Vector4f CursorPosInfo(GLFWwindow* window) {
	// Get the position of the mouse in the window
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//Get the size of the window
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	// Convert screen position to world coordinates

	double xworld = ((xpos / double(width)) * 2) - 1;
	double yworld = (((height - 1 - ypos) / double(height)) * 2) - 1; // NOTE: y axis is flipped in glfw

	Eigen::Vector4f p_world;
	p_world[0] = xworld;
	p_world[1] = yworld;


	return p_world;
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	Eigen::Vector4f p_world = CursorPosInfo(window);

	 
	VBO.update(V);

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the position of the mouse in the window
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Get the size of the window
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Convert screen position to world coordinates
    double xworld = ((xpos/double(width))*2)-1;
    double yworld = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw

    // Update the position of the first vertex if the left button is pressed
    //if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        //V.col(0) << xworld, yworld;

    // Upload the change to the GPU
    VBO.update(V);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Update the position of the first vertex if the keys 1,2, or 3 are pressed
    switch (key)
    {	
		//change the ball position
        case  GLFW_KEY_LEFT: //problem
			if (gameon) {
				double tmp = V(0, 24);
				for (int i = 24; i < 30; i++) {
					if (tmp - 0.1 < -1.0) {
						double gap = 1.0 + tmp;
						V(0, i) = V(0, i) - gap;
					}
					else {
						V(0, i) = V(0, i) - 0.1;
					}
				}
			}
		    break;

        case GLFW_KEY_RIGHT:
			if (gameon) {
				double tmp = V(0, 29);
				for (int i = 24; i < 30; i++) {
					if (tmp + 0.1 > 1.0) {
						double gap = 1.0 - tmp;
						V(0, i) = V(0, i) + gap;
					}
					else {
						V(0, i) = V(0, i) + 0.1;
					}
				}
			}
			break;
		case GLFW_KEY_SPACE:
			//game start
			if (gameon == false) {
				gameon = true;
				first = true;
				second = false;
				// put barrier into first block
				// first will be used
				barrier_putter(1);
							   
			}
			
			break;
		


		
		case  GLFW_KEY_3:
            //V.col(0) << 0.5,  0.5;
			// debug purpose
			if (gameon) {
				gameon = false;
				std::cout << V << std::endl;
			}
            break;
        default:
            break;
		

    }

    // Upload the change to the GPU
    VBO.update(V);
}

int main(void)
{
	barrier_impl();
	ball_init();
	//reset();
	//std::cout << V << std::endl;

    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Ensure that we get at least a 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//texture upload
	// source code referred from https://learnopengl.com
	

	//ourShader.use();	
	
	//I don't know how to put the texture in the uniform!!!!!

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(420, 600, "Final Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      }
      glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
    VBO.init();
	VBO_tex.init();
	/*
    V.resize(2,3);
    V << 0,  0.5, -0.5, 0.5, -0.5, -0.5;
    VBO.update(V);
	*/
	reset();
	uv_cordinate_init();
	//std::cout << V << std::endl;
    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
    Program program;
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec2 position;"
					"in vec2 aTexCoord;"
					"out vec2 TexCoord;"
                    "void main()"
                    "{"
                    "    gl_Position = vec4(position, 0.0, 1.0);"
					"    TexCoord = aTexCoord;"	
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "out vec4 outColor;"
					"in vec2 TexCoord;"
                    //"uniform vec3 triangleColor;"
					"uniform sampler2D texture1;"
                    "void main()"
                    "{"
					"    outColor = texture(texture1, TexCoord);"
                    //"    outColor = vec4(triangleColor, 1.0);"
                    "}";

    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader,fragment_shader,"outColor");
    program.bind();
	
	unsigned int texture1;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); //do I need to call this once again?
	//set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("../image2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		std::cout << "width "<< width << "height "<< height << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



    // The vertex shader wants the position of the vertices as an input.
    // The following line connects the VBO we defined above with the position "slot"
    // in the vertex shader
    program.bindVertexAttribArray("position",VBO);
	program.bindVertexAttribArray("aTexCoord", VBO_tex);

    // Save the current time --- it will be used to dynamically change the triangle color
    //auto t_start = std::chrono::high_resolution_clock::now();

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Register the mouse callback
	glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glUniform1i(glGetUniformLocation(fragment_shader, "texBlock"), 0);


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        VAO.bind();

        // Bind your program
        program.bind();

        // Set the uniform value depending on the time difference
        //auto t_now = std::chrono::high_resolution_clock::now();
        //float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        //glUniform3f(program.uniform("triangleColor"), (float)(sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
			   
        // Clear the framebuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		if (first) {
			//glUniform3f(program.uniform("triangleColor"), 1.0f, 0.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 12);

		}
			
		if (second) {
			//glUniform3f(program.uniform("triangleColor"), 0.0f, 1.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 12, 12);
		}
		//ball draw
		//glUniform3f(program.uniform("triangleColor"), 1.0f, 0.5f, 0.5f);
		glDrawArrays(GL_TRIANGLES, 24, 6);

		if (gameon){
			if (first) {
				collision_check(3, 4, 5);
				collision_check(9, 10, 11);
			}
			if (second) {
				collision_check(15, 16, 17);
				collision_check(21, 22, 23);
			}
			
			onestep(ballspeed);
			swap();
			//std::cout << "counter" << counter << std::endl;
		}
		else {



		}


        // Draw a triangle
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Deallocate opengl memory
    program.free();
    VAO.free();
    VBO.free();

    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}
