// Space Invaders.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cstdio>
#include <cstdint>


int player_move_dir = 1;

struct Buffer {
    size_t width, height;
    uint32_t* data;
};

struct Sprite {
    size_t width, height;
    uint8_t* data;
};

struct Alien {
    size_t x, y;
    uint8_t type;
};

struct Player {
    size_t x, y;
    size_t life;
};

struct Game {
    size_t width, height;
    size_t num_aliens;
    Alien* aliens;
    Player player;
};

struct SpriteAnimation {

    bool loop;
    size_t num_frames;
    size_t frame_duration;
    size_t time;
    Sprite** frames;

};




// Handling error cases
#define GL_ERROR_CASE(glerror)\
    case glerror: snprintf(error, sizeof(error), "%s", #glerror)

inline void gl_debug(const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        char error[128];

        switch (err) {
            GL_ERROR_CASE(GL_INVALID_ENUM); break;
            GL_ERROR_CASE(GL_INVALID_VALUE); break;
            GL_ERROR_CASE(GL_INVALID_OPERATION); break;
            GL_ERROR_CASE(GL_INVALID_FRAMEBUFFER_OPERATION); break;
            GL_ERROR_CASE(GL_OUT_OF_MEMORY); break;
        default: snprintf(error, sizeof(error), "%s", "UNKNOWN_ERROR"); break;
        }

        fprintf(stderr, "%s - %s: %d\n", error, file, line);
    }
}

#undef GL_ERROR_CASE


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void buffer_clear(Buffer* buffer, uint32_t color)
{
    for (size_t i = 0; i < buffer->width * buffer->height; i++) {
        buffer->data[i] = color;
    }
}

void validate_shader(GLuint shader, const char* file = 0)
{
    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

    if (length > 0)
    {
        printf("Shader %d(%s) compile error: %s\n",
            shader, (file ? file : ""), buffer);
    }
}

// Draws a sprite into the CPU buffer, using the provided color for the pixels that are set in the sprite data
void buffer_draw_sprite(
    Buffer* buffer, const Sprite& sprite,
    size_t x, size_t y, uint32_t color
) {
    for (size_t xi = 0; xi < sprite.width; ++xi)
    {
        for (size_t yi = 0; yi < sprite.height; ++yi)
        {
            size_t sy = sprite.height - 1 + y - yi;
            size_t sx = x + xi;
            if (sprite.data[yi * sprite.width + xi] &&
                sy < buffer->height && sx < buffer->width)
            {
                buffer->data[sy * buffer->width + sx] = color;
            }
        }
    }
}

bool validate_program(GLuint program)
{
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);

    if (length > 0)
    {
        printf("Program %d link error: %s\n", program, buffer);
        return false;
    }

    return true;
}
// Converts RGB values to a single uint32_t value in the format 0xRRGGBBAA
uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

// Parameters for the shader
const char* vertex_shader =
"\n"
"#version 330\n"
"\n"
"noperspective out vec2 TexCoord;\n"
"\n"
"void main(void){\n"
"\n"
"    TexCoord.x = (gl_VertexID == 2)? 2.0: 0.0;\n"
"    TexCoord.y = (gl_VertexID == 1)? 2.0: 0.0;\n"
"    \n"
"    gl_Position = vec4(2.0 * TexCoord - 1.0, 0.0, 1.0);\n"
"}\n"
;


const char* fragment_shader =
"\n"
"#version 330\n"
"\n"
"uniform sampler2D buffer;\n"
"noperspective in vec2 TexCoord;\n"
"\n"
"out vec3 outColor;\n"
"\n"
"void main(void){\n"
"    outColor = texture(buffer, TexCoord).rgb;\n"
"}\n"
;
