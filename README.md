# physim   
physim is a 2D physics simulator

# Platforms
Windows is the only supported platform   
* compiled with MSVC

# Features (TODO)
1. Easy to use graphical user interface
2. Modifying the passing of time, stopping it is an option.
3. A paused simulation can easily be saved for later use.
4. ...

# Dependencies
physim depends on 3 libraries:   
* [imgui](https://github.com/ocornut/imgui) for graphical user interface
* [SFML](https://www.sfml-dev.org/) as a wrapper for rendering backend, which imgui uses
* [imgui-sfml](https://github.com/SFML/imgui-sfml) which is the glue between the former dependencies

To compile this project use the existing `CMakeLists.txt` file, it expects the folders:   
`dependencies/imgui` `dependencies/imgui-sfml` `dependencies/SFML-2.6.1` to contain their respective files   

Note: We compile `imgui` and `imgui-sfml` from source, but `SFML` is expected to be a x64 statically linked library, thus the `SFML-2.6.1` name of the dependency folder
