# ELR
En liten renderare

ELR is small renderer written in C++ with DirectX 11.3. It's a project I started since I felt that I needed to write something to brush up on my graphics progamming skills.
It has a few quirks, mainly in the main-loop which is placed inside the renderer class (which kinda speaks for itself, renderer should only recieve tasks, not create them).

Current goal and aims of the project is to do a complete rewrite of the renderer class and introduce concurrency. The rewrite will be done by the book, "Game engine architecture".

# Features
Deferred rendering
Free moving camera
Mesh support
Model support
Material support
Dynamic lights
Phong shading
Obj model loading (currently using third party, TinyObjLoader)
Normal mapping

# Planned features
Multithreaded support
Physical based rendering
Skeletal animation
GUI
Some of scripting support.

Ambitions are key! 

## License
[MIT](https://choosealicense.com/licenses/mit/)
