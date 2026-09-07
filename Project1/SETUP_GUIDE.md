# BasicMesh Setup Guide

## Project Structure

```
D:\OpenGL\Project1\
├── Project1\
│   ├── Application.cpp           (Main application using BasicMesh)
│   ├── BasicMesh.h              (Mesh loader class)
│   ├── BasicMesh.cpp            (Mesh implementation)
│   ├── Camera.h / Camera.cpp     (Camera control)
│   ├── world_transform.h         (Transform management)
│   └── shader.vs / shader.fs     (Shader files)
├── Content\
│   └── models\                   (Place your 3D models here)
│       ├── spider.obj            (Default model - needs to be placed here)
│       ├── cube.obj
│       └── sphere.obj
├── Dependencies\
│   ├── include\                  (Assimp, GLEW, GLFW, GLM headers)
│   └── lib-vc2022\               (Precompiled libraries)
└── shader files                  (In project root)
```

## Setup Instructions

### 1. Create Model Directory
```powershell
mkdir Content\models
```

### 2. Place Model Files
Place your 3D model files in `Content\models\`:
- `spider.obj` (default model)
- Any other OBJ, FBX, or DAE files

### 3. Model Path Configuration

In `BasicMesh.h`, you'll find these placeholders:

```cpp
#define DEFAULT_MESH_PATH       "../Content/models/"
#define SPIDER_MODEL_PATH       DEFAULT_MESH_PATH "spider.obj"
#define CUBE_MODEL_PATH         DEFAULT_MESH_PATH "cube.obj"
#define SPHERE_MODEL_PATH       DEFAULT_MESH_PATH "sphere.obj"
```

### 4. Using Different Models

Edit `Application.cpp` and modify this line:

```cpp
// Current (default):
if (!m_pMesh->LoadMesh(DEFAULT_MODEL)) {

// Use custom path:
if (!m_pMesh->LoadMesh("../Content/models/your_model.obj")) {
```

## Supported Model Formats

The following formats are supported via Assimp:
- **OBJ** (.obj) - Wavefront OBJ
- **FBX** (.fbx) - Autodesk FBX
- **DAE** (.dae) - COLLADA
- **STL** (.stl) - STereoLithography
- **3DS** (.3ds) - 3D Studio Max
- **BLEND** (.blend) - Blender (with plugins)
- And many more...

## Model Loading Process

### Automatic:
1. Application::Init() is called
2. BasicMesh is created
3. LoadMesh() is called with DEFAULT_MODEL path
4. Assimp loads and parses the file
5. Vertices, normals, textures are processed
6. GPU buffers are created

### On Load:
```
Loading mesh: '../Content/models/spider.obj'
Successfully loaded mesh '../Content/models/spider.obj'
Loaded texture '<path>/texture.png'
```

## Troubleshooting

### "Failed to load mesh" Error

1. **Check file path** - Make sure the file exists in Content\models\
   ```powershell
   ls Content\models\
   ```

2. **Verify file format** - OBJ files must:
   - Have valid vertex data (v x y z)
   - Have faces defined (f v/vt/vn)
   - Reference textures with absolute or relative paths

3. **Check file permissions** - File should be readable

### "Cannot find texture" Error

1. Place texture files in the same directory as the OBJ
   OR
2. Update OBJ material file (.mtl) with correct texture paths

### Low Frame Rate

1. Check model complexity (vertex/face count)
2. Reduce texture resolution
3. Use simpler models for testing

## Camera Controls

- **Mouse Movement**: Look around (requires camera implementation)
- **W/A/S/D**: Move forward/left/backward/right
- **Space/Ctrl**: Move up/down
- **Z/X**: Decrease/increase camera speed
- **ESC/Q**: Exit application

## Model Transformation

Edit `Application.cpp` in `RenderScene()` to transform the mesh:

```cpp
WorldTransform& transform = m_pMesh->GetWorldTransform();

// Scale the model
transform.SetScale(0.01f);

// Position in world space
transform.SetPosition(0.0f, 0.0f, 2.0f);

// Rotate (in radians, using Y-axis)
transform.Rotate(0.0f, rotationAngle, 0.0f);
```

## Testing Models

### Simple Test: Create a Cube

1. Use Blender or similar tool to create a cube
2. Export as OBJ
3. Place in Content\models\
4. Update path and rebuild

### Recommended Test Models:

- **Stanford Bunny**: ~70K vertices (good test)
- **Utah Teapot**: ~3K vertices (simple)
- **Suzanne Monkey**: ~8K vertices (medium)

Available from: https://www.turbosquid.com/ (many free models)

## Build & Run

```powershell
# Clean build
msbuild Project1.sln /t:Clean /t:Build /p:Configuration=Debug /p:Platform=x64

# Run
.\Project1\x64\Debug\Project1.exe
```

## Performance Tips

1. **Optimize Models**:
   - Reduce polygon count
   - Remove unused vertices
   - Combine materials

2. **Texture Optimization**:
   - Use compressed formats
   - Resize to power-of-2 (512x512, 1024x1024)

3. **Rendering Optimization**:
   - Enable frustum culling
   - Use LOD (Level of Detail) meshes
   - Batch render similar meshes

## References

- Assimp Documentation: https://assimp-docs.readthedocs.io/
- OBJ File Format: https://en.wikipedia.org/wiki/Wavefront_.obj_file
- FBX Format: https://www.autodesk.com/developer/fbx/

---

For more information, see `BASICMESH_REFACTORING.txt` in the project directory.
