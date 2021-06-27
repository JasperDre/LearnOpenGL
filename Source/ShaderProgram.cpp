#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram()
	: myID(0)
{

}

void ShaderProgram::Create(const std::string& aVertexShaderFilePath, const std::string& aFragmentShaderFilePath)
{
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
	std::ifstream vertexShaderFileStream;
	std::ifstream fragmentShaderFileStream;
	
	vertexShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexShaderFileStream.open(aVertexShaderFilePath);
		fragmentShaderFileStream.open(aFragmentShaderFilePath);

		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;
		vertexShaderStream << vertexShaderFileStream.rdbuf();
		fragmentShaderStream << fragmentShaderFileStream.rdbuf();

		vertexShaderFileStream.close();
		fragmentShaderFileStream.close();

		vertexShaderSource = vertexShaderStream.str();
		fragmentShaderSource = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure anException)
	{
		std::cout << anException.what() << std::endl;
	}

	const char* vertexShaderCode = vertexShaderSource.c_str();
	const char* fragmentShaderCode = fragmentShaderSource.c_str();

	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShaderID);
	int vertexShaderResult;
	char vertexShaderInfo[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexShaderResult);
	if (!vertexShaderResult)
	{
		glGetShaderInfoLog(vertexShaderID, 512, nullptr, vertexShaderInfo);
		std::cout << vertexShaderInfo << std::endl;
	}

	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShaderID);
	int fragmentShaderResult;
	char fragmentShaderInfo[512];
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentShaderResult);
	if (!fragmentShaderResult)
	{
		glGetShaderInfoLog(fragmentShaderID, 512, nullptr, fragmentShaderInfo);
		std::cout << fragmentShaderInfo << std::endl;
	}

	myID = glCreateProgram();
	glAttachShader(myID, vertexShaderID);
	glAttachShader(myID, fragmentShaderID);
	glLinkProgram(myID);
	int shaderLinkResult;
	char shaderLinkInfo[512];
	glGetProgramiv(myID, GL_LINK_STATUS, &shaderLinkResult);
	if (!shaderLinkResult)
	{
		glGetProgramInfoLog(myID, 512, nullptr, shaderLinkInfo);
		std::cout << shaderLinkInfo << std::endl;
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void ShaderProgram::Use()
{
	glUseProgram(myID);
}

void ShaderProgram::Delete()
{
	glDeleteProgram(myID);
}

void ShaderProgram::SetBool(const std::string& aName, const bool aValue)
{
	glUniform1i(GetLocation(aName), static_cast<int>(aValue));
}

void ShaderProgram::SetInt(const std::string& aName, const int aValue)
{
	glUniform1i(GetLocation(aName), aValue);
}

void ShaderProgram::SetFloat(const std::string& aName, const float aValue)
{
	glUniform1f(GetLocation(aName), aValue);
}

void ShaderProgram::SetMatrix4x4(const std::string& aName, const glm::mat4x4& aValue)
{
	glUniformMatrix4fv(GetLocation(aName), 1, GL_FALSE, glm::value_ptr(aValue));
}

void ShaderProgram::SetVector3(const std::string& aName, const glm::vec3& aValue)
{
	glUniform3f(GetLocation(aName), aValue.x, aValue.y, aValue.z);
}

void ShaderProgram::SetVector4(const std::string& aName, const glm::vec4& aValue)
{
	glUniform4f(GetLocation(aName), aValue.x, aValue.y, aValue.z, aValue.w);
}

unsigned int ShaderProgram::GetLocation(const std::string& aName) const
{
	return glGetUniformLocation(myID, aName.c_str());
}
