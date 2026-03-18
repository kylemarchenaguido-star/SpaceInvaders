// Space Invaders.cpp : Defines the entry point for the application.
//

#include "Space Invaders.h"

using namespace std;


int main()
{
    const size_t buffer_width = 224;
    const size_t buffer_height = 256;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//This is the openGL window and context we will be rendering to
    GLFWwindow* window = glfwCreateWindow(buffer_width, buffer_height, "Space Invaders", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();

    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    int glVersion[2] = { -1, 1 };
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);

    gl_debug(__FILE__, __LINE__);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
    printf("Renderer used: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSwapInterval(1);

    uint32_t clear_color = rgb_to_uint32(0, 128, 0);

    glClearColor(1.0, 0.0, 0.0, 1.0);


    // Create buffer
    Buffer buffer;
    buffer.width = buffer_width;
    buffer.height = buffer_height;
    buffer.data = new uint32_t[buffer.width * buffer.height];
    buffer_clear(&buffer, clear_color);

    // VAO
    GLuint fullscreen_triangle_vao;
    glGenVertexArrays(1, &fullscreen_triangle_vao);

    // Shaders
    GLuint shader_id = glCreateProgram();
    {
        GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader_vp, 1, &vertex_shader, 0);
        glCompileShader(shader_vp);
        validate_shader(shader_vp, vertex_shader);
        glAttachShader(shader_id, shader_vp);
        glDeleteShader(shader_vp);
    }
    {
        GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader_fp, 1, &fragment_shader, 0);
        glCompileShader(shader_fp);
        validate_shader(shader_fp, fragment_shader);
        glAttachShader(shader_id, shader_fp);
        glDeleteShader(shader_fp);
    }

    //Link the program
    glLinkProgram(shader_id);
    if (!validate_program(shader_id)) {
        fprintf(stderr, "Error while validating shader.\n");
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreen_triangle_vao);
        delete[] buffer.data;
        return -1;
    }

	// Set uniform (program must be linked before this step)
    glUseProgram(shader_id);
    GLint location = glGetUniformLocation(shader_id, "buffer");
    glUniform1i(location, 0);

    // Texture
    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,buffer.width, buffer.height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //Open
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(fullscreen_triangle_vao);

	// Game objects
    Sprite alien_sprite;
    alien_sprite.width = 11;
    alien_sprite.height = 8;
    alien_sprite.data = new uint8_t[88]
    {
        0,0,1,0,0,0,0,0,1,0,0,
        0,0,0,1,0,0,0,1,0,0,0,
        0,0,1,1,1,1,1,1,1,0,0,
        0,1,1,0,1,1,1,0,1,1,0,
        1,1,1,1,1,1,1,1,1,1,1,
        1,0,1,1,1,1,1,1,1,0,1,
        1,0,1,0,0,0,0,0,1,0,1,
        0,0,0,1,1,0,1,1,0,0,0
    };

    Sprite alien_sprite1;
    alien_sprite1.width = 11;
    alien_sprite1.height = 8;
    alien_sprite1.data = new uint8_t[88]
    {
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        1,0,0,1,0,0,0,1,0,0,1, // @..@...@..@
        1,0,1,1,1,1,1,1,1,0,1, // @.@@@@@@@.@
        1,1,1,0,1,1,1,0,1,1,1, // @@@.@@@.@@@
        1,1,1,1,1,1,1,1,1,1,1, // @@@@@@@@@@@
        0,1,1,1,1,1,1,1,1,1,0, // .@@@@@@@@@.
        0,0,1,0,0,0,0,0,1,0,0, // ..@.....@..
        0,1,0,0,0,0,0,0,0,1,0  // .@.......@.
    };

    SpriteAnimation* alien_animation = new SpriteAnimation;

    alien_animation->loop = true;
    alien_animation->num_frames = 2;
    alien_animation->frame_duration = 10;
    alien_animation->time = 0;

    alien_animation->frames = new Sprite * [2];
    alien_animation->frames[0] = &alien_sprite;
    alien_animation->frames[1] = &alien_sprite1;


    Sprite player_sprite;
    player_sprite.width = 11;
    player_sprite.height = 7;
    player_sprite.data = new uint8_t[77]
    {
        0,0,0,0,0,1,0,0,0,0,0, 
        0,0,0,0,1,1,1,0,0,0,0,
        0,0,0,0,1,1,1,0,0,0,0, 
        0,1,1,1,1,1,1,1,1,1,0, 
        1,1,1,1,1,1,1,1,1,1,1, 
        1,1,1,1,1,1,1,1,1,1,1, 
        1,1,1,1,1,1,1,1,1,1,1, 
    };

    Game game;
    game.width = buffer_width;
    game.height = buffer_height;
    game.num_aliens = 55;
    game.aliens = new Alien[game.num_aliens];

    game.player.x = 112 - 5;
    game.player.y = 32;

    game.player.life = 3;

    for (size_t yi = 0; yi < 5; ++yi)
    {
        for (size_t xi = 0; xi < 11; ++xi)
        {
            game.aliens[yi * 11 + xi].x = 16 * xi + 20;
            game.aliens[yi * 11 + xi].y = 17 * yi + 128;
        }
    }

    while (!glfwWindowShouldClose(window))
    {
		// Clear the CPU buffer 
        buffer_clear(&buffer, clear_color);

        for (size_t ai = 0; ai < game.num_aliens; ++ai)
        {
            const Alien& alien = game.aliens[ai];
            buffer_draw_sprite(&buffer, alien_sprite,
                alien.x, alien.y, rgb_to_uint32(128, 0, 0));
        }

        //Draws the player into the CPU buffer
        buffer_draw_sprite(&buffer, player_sprite, game.player.x, game.player.y, rgb_to_uint32(128, 0, 0));

		//Draws the aliens into the CPU buffer using the current animation frame
        for (size_t ai = 0;  ai < game.num_aliens; ++ai)
        {
            const Alien& alien = game.aliens[ai];
            size_t current_frame = alien_animation->time / alien_animation->frame_duration;
            const Sprite& sprite = *alien_animation->frames[current_frame];
            buffer_draw_sprite(&buffer, sprite, alien.x, alien.y, rgb_to_uint32(128, 0, 0));
        }

		//Update the animation time
                                        /*Check if a full animation cycle has completed */
        ++alien_animation->time;
        if (alien_animation->time == alien_animation->num_frames * alien_animation->frame_duration)
        {
            if (alien_animation->loop) alien_animation->time = 0;
            else
            {
                delete alien_animation;
                alien_animation = nullptr;
            }
        }

        // Upload the CPU buffer into the GPU
        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.width, buffer.height,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );
        //Clears the openGL buffer
        glClear(GL_COLOR_BUFFER_BIT);

        //Samples the CPU buffer
        glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swaps the back buffer with the front buffer, and after the swaping the back buffer becomes the canvas and for the next swap
        glfwSwapBuffers(window);

        if (game.player.x + player_sprite.width + player_move_dir >= game.width - 1)
        {
            game.player.x = game.width - player_sprite.width - player_move_dir - 1;
            player_move_dir *= -1;
        }
        else if ((int)game.player.x + player_move_dir <= 0)
        {
            game.player.x = 0;
            player_move_dir *= -1;
        }
        else game.player.x += player_move_dir;


        // Prpces all pending events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteVertexArrays(1, &fullscreen_triangle_vao);

    delete[] buffer.data;
	delete[] alien_sprite.data;



    return 0;
}


