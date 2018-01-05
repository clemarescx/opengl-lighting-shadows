cmake_minimum_required(VERSION 3.9)
project(Visual_Studio_2017_solution)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/GLUtils)

add_executable(Visual_Studio_2017_solution
        include/GLUtils/BlinnPhongProgram.h
        include/GLUtils/CubeMap.hpp
        include/GLUtils/DebugOutput.hpp
        include/GLUtils/GLUtils.hpp
        include/GLUtils/Program.hpp
        include/GLUtils/ShadowProgram.h
        include/GLUtils/VBO.hpp
        include/DepthFBO.h
        include/GameException.h
        include/GameManager.h
        include/GeometryManager.h
        include/Model.h
        include/Timer.h
        include/VirtualTrackball.h
        src/DepthFBO.cpp
        src/GameManager.cpp
        src/GeometryManager.cpp
        src/main.cpp
        src/Model.cpp
        src/VirtualTrackball.cpp)
