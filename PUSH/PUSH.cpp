// PUSH.cpp : définit le point d'entrée de l'application.
//

#include "PUSH.h"

using namespace std;
namespace fs = std::filesystem;
using namespace glm;

GLFWwindow* window;

void cd(const string& pCommand, string& pPath) {
    string vStartPath = pPath;
    int vEnd = pCommand.size();
    int StartWord = 0;
    char c[100];
    for (int i = 0; i < pCommand.size(); i++) {
        if (pCommand[i] == '/' || i+1 == pCommand.size() || pCommand[i] == ' ') {
            string vCommand = pCommand.substr(StartWord, i + 1 - StartWord);
            if (pPath == "."&& vCommand.substr(0,2) == "..") {
                StartWord = i + 1;
                return;
            }
            int Error = chdir(vCommand.c_str());
            if (Error == -1) {
                chdir(getcwd(c, 100));
                pPath = vStartPath;
                return;
            }
            StartWord = i+1;
            string vPath = getcwd(c, 100);
            int vPush = vPath.rfind("PUSH");
            pPath = "." + vPath.substr(vPush + 16, vPath.size() - vPush);
        }
        if (pCommand[i] == ' ') return;
    }
}

//my_popen allow us to get the output of a command(string) in a vector of string
//Only right commands will give a result
//Other will give an empty line

void my_popen(const string& pCommand, vector<string>& pOutput, string& pPath) {
    FILE* vFile;
    const int vSizeBuf = 1234;
    char vBuff[vSizeBuf];
    pOutput = vector<string>();
    char c[100];
    string vPath = "cd " + string(getcwd(c,100)) + " && " + pCommand;
    if ((vFile = popen(vPath.c_str(), "r")) == NULL)
    {
        cout << "error, File NULL" << endl;
    }

    string vCurrent_string = "";

    while (fgets(vBuff, sizeof(vBuff), vFile))
    {
        vCurrent_string += vBuff;
    }

    pOutput.push_back(vCurrent_string.substr(0, vCurrent_string.size() - 1));
    if (pCommand.substr(0, 2) == "cd") {
        cd(pCommand.substr(3, pCommand.size() - 3), pPath);
    }
    pclose(vFile);
}//Code from stackoverflow

void init(string& pPath) {
    vector<string> vOutput;
    const auto vWorkingDir = fs::current_path();
    if(!fs::is_directory(vWorkingDir / "Environment"))
        my_popen("mkdir Environment", vOutput, pPath);
    my_popen("cd Environment", vOutput, pPath);
}


int main()
{
    //LOL
	cout << "Hello CMake." << endl;
    string path = ".";
    init(path);
    //cout << current_path.back() << "\n";
    //We have a command
    string vCommand = "cd test/test2";
    //We save the result of the command in output
    vector<string> vOutput;
    my_popen(vCommand, vOutput, path);
    //We check if the output has the good values
    for (vector<string>::iterator vIterator = vOutput.begin(); vIterator != vOutput.end(); ++vIterator)
    {
       cout << *vIterator << endl;
    }//It has
    vCommand = "cd ../../test";
    my_popen(vCommand, vOutput, path);
    char c[100];

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 1024, "PUSH", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(10, 10, 872, 872);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders();

	/*
	static const GLfloat g_vertex_buffer_data[] = {
		// Pi ce 3
		0.0f, 0.0f, 0.0f,
		1.0f, 0.2f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.2f, 0.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 0.8f, 0.0f,

		0.0f, 0.8f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.8f, 0.0f,

		0.2f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.2f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.2f, 0.0f, 0.0f,

		0.0f, 0.8f, 0.0f,
		0.2f, 0.8f, 0.0f,
		0.2f, 0.5f, 0.0f,

		0.0f, 0.2f, 0.0f,
		0.2f, 0.2f, 0.0f,
		0.2f, 0.5f, 0.0f,
	};*/

	/*static const GLfloat g_vertex_buffer_data[] = {
		// Pi ce 1
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.3f, 0.0f,

		-0.5f, 0.5f, 0.0f,
		-0.5f, 0.3f, 0.0f,
		0.5f, 0.3f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		-0.5f, -0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, -0.3f, 0.0f,

		-0.3f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

		-0.3f, 0.5f, 0.0f,
		-0.3f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

		0.5f, 0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		0.7f, 0.0f, 0.0f,

		-0.3f, -0.5f, 0.0f,
		0.3f, -0.5f, 0.0f,
		0.0f, -0.7f, 0.0f,
	};*/

	/*
	static const GLfloat g_vertex_buffer_data[] = {
		// Pi ce 2
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,

		0.5f, 0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		0.7f, 0.3f, 0.0f,

		0.7f, 0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		0.7f, -0.3f, 0.0f,
	};*/

	/*static const GLfloat g_vertex_buffer_data[] = {
		// Pi ce 4
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.2f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.2f, 1.0f, 0.0f,
		0.2f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.8f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		0.8f, 1.0f, 0.0f,
		0.8f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.8f, 0.0f,

		1.0f, 0.8f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.8f, 0.0f,

		0.8f, 1.0f, 0.0f,
		0.5f, 0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,

		0.2f, 1.0f, 0.0f,
		0.5f, 0.8f, 0.0f,
		0.2f, 0.8f, 0.0f,
	};*/

	static const GLfloat g_vertex_buffer_data[] = {
		// Pi ce 5
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.3f, 0.0f,

		-0.5f, 0.5f, 0.0f,
		-0.5f, 0.3f, 0.0f,
		0.5f, 0.3f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		-0.5f, -0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,

		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, -0.3f, 0.0f,

		0.5f, 0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		-0.3f, 0.3f, 0.0f,

		-0.3f, -0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		-0.3f, 0.3f, 0.0f,

		-0.5f, -0.3f, 0.0f,
		-0.3f, 0.0f, 0.0f,
		-0.3f, -0.3f, 0.0f,

		-0.5f, 0.3f, 0.0f,
		-0.3f, 0.0f, 0.0f,
		-0.3f, 0.3f, 0.0f,

		0.5f, 0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		0.7f, 0.3f, 0.0f,

		0.7f, 0.3f, 0.0f,
		0.5f, -0.3f, 0.0f,
		0.7f, -0.3f, 0.0f,
	};


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 30); // 24 indices starting at 0 -> 1 triangle
		//glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 1 -> 2 triangle
		//glDrawArrays(GL_TRIANGLES, 2, 3); // 3 indices starting at 1 -> 4 triangle
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);



		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;


	return 0;
}
