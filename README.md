# SpaceInvaders

A from-scratch recreation of the classic **Space Invaders** arcade game, 
built using C++ and modern OpenG

This project is being developed as a learning exercise to explore:
- Modern OpenGL rendering (shaders, textures, VAOs)
- Low-level pixel buffer manipulation in C++
- Game loop architecture
- Sprite rendering without external game engines

The game renders entirely through a custom software pixel buffer that is 
uploaded to the GPU each frame as a texture, then displayed via a 
fullscreen triangle trick.

---

## What i used

- **C++** — core language
- **OpenGL 3.3 Core Profile** — GPU rendering
- **GLFW** — window creation and input handling
- **GLEW** — OpenGL extension loading
- **vcpkg** — package management

---

## Current State

- [x] Window creation with GLFW
- [x] OpenGL 3.3 context setup
- [x] Custom pixel buffer (CPU-side rendering)
- [x] Vertex and fragment shader setup
- [x] Buffer uploaded to GPU as texture
- [x] Alien sprite rendering
- [ ] Player character
- [ ] Input handling (movement, shooting)
- [ ] Enemy movement and AI
- [ ] Collision detection
- [ ] Score system
- [ ] Sound

---

## How to Build

### Requirements
- Visual Studio 2022
- vcpkg with GLFW and GLEW installed

### vcpkg setup
```bash
vcpkg install glfw3
vcpkg install glew

!! Not complete yet !!
