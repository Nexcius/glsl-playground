#pragma once
#include <string>
#include <fstream>
#include <istream>
#include <GL/glew.h>
#include <GL/gl.h>

void printProgramLog( GLuint program )
{
    //Make sure name is shader
    if( glIsProgram( program ) )
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }
        
        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a program\n", program );
    }
}

void printShaderLog( GLuint shader )
{
    //Make sure name is shader
    if( glIsShader( shader ) )
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a shader\n", shader );
    }
}

GLuint loadShaderFromFile(std::string path, GLenum shaderType) {
    GLuint shaderId = 0;
    std::string shaderString;

    std::ifstream sourceFile( path.c_str() );
    if(sourceFile) {
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        shaderId = glCreateShader( shaderType );

        const GLchar* shaderSource = shaderString.c_str();
        glShaderSource(shaderId, 1, (const GLchar**)&shaderSource, NULL );
        glCompileShader(shaderId);

        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
        
        if(shaderCompiled != GL_TRUE) {
            printf( "Unable to compile shader %d!\n\nSource:\n%s\n", shaderId, shaderSource );
            printShaderLog( shaderId );
            glDeleteShader( shaderId );
            shaderId = 0;
        }

        printf("Successfully loaded shader %s\n", path.c_str());
    }
    else {
        printf( "Unable to open file %s\n", path.c_str() );
    }
    
    return shaderId;
}

GLuint createProgram(const std::string vertexSrc, const std::string fragmentSrc) {
    GLuint programId = glCreateProgram();

    GLuint vertexShader = loadShaderFromFile(vertexSrc, GL_VERTEX_SHADER);
    if (vertexShader == 0) {
        glDeleteProgram(programId);
        programId = 0;
        return programId;
    }
    glAttachShader(programId, vertexShader);

    GLuint fragmentShader = loadShaderFromFile(fragmentSrc, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0) {
        glDeleteProgram(programId);
        programId = 0;
        return programId;
    }
    glAttachShader(programId, fragmentShader);

    glLinkProgram(programId);

    GLint programSuccess = GL_TRUE;
    glGetProgramiv( programId, GL_LINK_STATUS, &programSuccess );
    if( programSuccess != GL_TRUE )
    {
        printf( "Error linking program %d!\n", programId );
        printProgramLog( programId );
        glDeleteShader( vertexShader );
        glDeleteShader( fragmentShader );
        glDeleteProgram( programId );
        programId = 0;
        return programId;
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    return programId;
}
