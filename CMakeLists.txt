cmake_minimum_required(VERSION 3.18)

set(TARGET TestMod)
project(${TARGET})

add_library(${TARGET} SHARED "dllmain.cpp" "EventManager/EventManager.cpp" "Event/Event.cpp" "Core/Core.cpp")

set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME main)
target_include_directories(${TARGET} PRIVATE .)
target_link_libraries(${TARGET} PUBLIC cppsdk xinput1_3)