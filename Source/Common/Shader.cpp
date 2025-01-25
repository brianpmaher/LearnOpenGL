#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::cout << "Loading shader: (\"" << vertexPath << "\", \"" << fragmentPath << "\")" << std::endl;

	std::string vertexCode;
	std::string fragmentCode;

	// Read code into strings
	{
		std::ifstream vertexFile;
		std::ifstream fragmentFile;

		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);

			std::stringstream vertexStream;
			std::stringstream fragmentStream;

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertexCode = vertexStream.str();
			fragmentCode = fragmentStream.str();
		}
		catch (std::ifstream::failure e)
		{
			throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
		}
	}

	// Compile shaders
	{
		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();
		GLuint vertexShader;
		GLuint fragmentShader;
		GLint success;
		constexpr int infoLogSize = 512;
		char infoLog[infoLogSize];

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, infoLogSize, nullptr, infoLog);
			throw std::runtime_error("ERROR::SHADER:VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, infoLogSize, nullptr, infoLog);
			throw std::runtime_error("ERROR::SHADER:FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
		}

		id = glCreateProgram();
		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		glLinkProgram(id);
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, infoLogSize, nullptr, infoLog);
			throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	std::cout << "Compiled and linked program: " << id << std::endl;
}

Shader::~Shader()
{
	std::cout << "Deleting program: " << id << std::endl;

	glDeleteProgram(id);
}
