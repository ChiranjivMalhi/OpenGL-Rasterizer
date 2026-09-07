# OpenGL Rasterizer

A real-time 3D renderer built in **C++ and OpenGL** to explore the fundamentals of modern graphics programming. The project covers the complete process of loading 3D assets, managing GPU buffers, transforming geometry, applying textures and lighting, and rendering an interactive scene through GLSL shaders.

## Demo

<p align="center">
  <video src="Content/README/demo.mp4" width="700" controls> 

https://github.com/user-attachments/assets/ba9c23f5-722b-4650-bdea-76e6fd90472d

</video>
</p>

---

## Features

* Real-time 3D rendering using OpenGL
* 3D model loading with **Assimp**
* VAO/VBO-based mesh management
* GLSL vertex and fragment shaders
* World, view, and projection transformations
* Interactive camera movement
* Diffuse, specular, and normal textures
* Directional lighting
* Depth testing
* Back-face culling
* 4× MSAA anti-aliasing

---

## Rendering

The renderer follows the standard programmable OpenGL pipeline. 3D models are imported using Assimp and their geometry, material, and texture information is converted into GPU resources. Vertex data is stored in OpenGL buffers and processed by the vertex shader, after which OpenGL performs rasterization. The fragment shader then combines texture and lighting information to determine the final pixel color.

The project uses separate components for **mesh loading, camera control, transformations, shader management, textures, and lighting**, keeping the rendering code modular and easier to extend.

### Abmbient
<img width="500" height="500" alt="Screenshot 2026-09-07 142006" src="https://github.com/user-attachments/assets/f6be7b39-1140-4f73-8125-2455e58b39c9" />

### Ambient + Diffuse (Directional Lighting)
<img width="500" height="500" alt="Screenshot 2026-09-07 142122" src="https://github.com/user-attachments/assets/347beecd-729d-49c9-98c1-2324173c8960" />


### Ambient + Diffuse + Specular (Directional Lighting)
<img width="500" height="500" alt="Screenshot 2026-09-07 141924" src="https://github.com/user-attachments/assets/2b636667-4cb1-48ae-99ff-44d8546c297d" />



### Final Render with Normal Mapping
<img width="500" height="500" alt="Screenshot 2026-09-07 142207" src="https://github.com/user-attachments/assets/b070e05c-f7e5-4ed9-b290-5c672f151810" />

<img width="500" height="500" alt="Screenshot 2026-09-06 231125" src="https://github.com/user-attachments/assets/c7d6c4fa-f3e3-49e0-8464-36e2570f9927" />




---

## Project Structure

```text
OpenGL-Rasterizer/
│
├── Content/
│   └── Models & Textures
│
├── Dependencies/
│   └── Third-party libraries
│
├── Project1/
│   ├── Application.cpp
│   ├── BasicMesh.cpp/.h
│   ├── Camera.cpp/.h
│   ├── texture.cpp/.h
│   ├── technique.cpp/.h
│   ├── lighting_technique.cpp/.h
│   ├── world_transform.h
│   ├── shader.vs
│   └── shader.fs
│
└── Project1.sln
```

### Main Components

**`Application`**
Handles application initialization, the rendering loop, input, and OpenGL state.

**`BasicMesh`**
Loads models through Assimp, extracts mesh/material data, and creates the required OpenGL buffers.

**`Camera`**
Handles interactive camera movement and view transformation.

**`WorldTransform`**
Manages object transformations and the world-view-projection matrix.

**`Technique` / `LightingTechnique`**
Handles GLSL shader programs and the uniforms required by the rendering pipeline.

**`Texture`**
Loads image data and manages OpenGL texture resources.

---

## Technologies

| Technology    | Purpose                       |
| ------------- | ----------------------------- |
| **C++**       | Renderer implementation       |
| **OpenGL**    | Real-time graphics API        |
| **GLSL**      | Vertex and fragment shaders   |
| **GLFW**      | Window creation and input     |
| **GLEW**      | OpenGL extension loading      |
| **GLM**       | Vector and matrix mathematics |
| **Assimp**    | 3D model importing            |
| **stb_image** | Texture loading               |

---

## Setup

### Requirements

* Windows
* Visual Studio
* C++ development tools
* OpenGL-compatible GPU

### Running the Project

```bash
git clone https://github.com/ChiranjivMalhi/OpenGL-Rasterizer.git
```

Open `Project1.sln` in Visual Studio, build the `Project1` project, and run the application.

---

## Future Improvements

The renderer is designed to be extended with more advanced real-time rendering techniques.

* **Shadow Mapping** — Implement dynamic shadows for directional, point, and spot lights.
* **Physically Based Rendering (PBR)** — Introduce metallic/roughness-based materials and a physically based lighting model.
* **HDR & Tone Mapping** — Add high-dynamic-range rendering and tone-mapping operators.
* **Deferred Rendering** — Implement a G-buffer based rendering pipeline for handling larger numbers of dynamic lights.
* **Post-Processing** — Add screen-space effects such as bloom, SSAO, depth of field, and motion blur.
* **Environment Lighting** — Add HDR environment maps and image-based lighting.
* **GPU Profiling & Optimization** — Profile CPU/GPU workloads and optimize buffer updates, draw calls, and shader performance.
* **Instanced Rendering** — Efficiently render large numbers of repeated meshes.
* **Advanced Material System** — Support more complex material properties and multiple texture workflows.

---

## References

This project was developed using the following resources and libraries:

* **Overall Architecture**
https://ogldev.org/

* **LearnOpenGL** — Joey de Vries
  https://learnopengl.com/

* **OpenGL Wiki** — Khronos Group
  https://www.khronos.org/opengl/wiki/

* **OpenGL Documentation** — Khronos Group
  https://docs.gl/

* **Assimp — Open Asset Import Library**
  https://assimp.org/

* **GLM — OpenGL Mathematics**
  https://github.com/g-truc/glm

* **GLFW — Window and Input Library**
  https://www.glfw.org/

* **stb_image — Image Loading Library**
  https://github.com/nothings/stb
  
---

## Author

**Chiranjiv Malhi**

[GitHub](https://github.com/ChiranjivMalhi)
