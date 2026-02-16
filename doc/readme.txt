Skeletal Animation Demo (Linux version)

1. Description & Requirements
  Application demonstrates rendering and animation of 3d skeletal meshes.
  Uses:
  - G++ supporting std C++ 17 - tested on 9.3.0
  - OpenGL, glu and freeglut libraries installed in OS

3. Building:
   $ make
   
   ./bin/demo.linux will be built

4. Running
  Launch to get the help:
  $ ./bin/demo.linux
  
  Launch the script to play example animation:
  $ ./scripts/play_run_anim.sh

  Control: 
  - arrow keys - move the camera
  - left mouse + mouse move - rotates the camera around its Y axis
  - right mouse + mouse move - rotates the camera around its X axis
  - 'p' key - changes the shader program, possible programs:
    PHONG, 
    GUARD, 
    SOLID COLOR, 
    PHONG + BONE ANIMATION
