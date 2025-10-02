#!bash

if [ ! -d stb-image ]; then
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -P stb-image
fi

if [ ! -d glm ]; then
git clone https://github.com/g-truc/glm.git
fi

if [ ! -d glfw ]; then
  git clone https://github.com/glfw/glfw.git
fi