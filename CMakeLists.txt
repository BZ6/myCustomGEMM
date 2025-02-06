cmake_minimum_required(VERSION 3.10)
project(myGEMM)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Настройка флагов компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Настройка путей для исходных файлов и выходных данных
set(SRCDIR src)
set(BINDIR bin)
set(OBJDIR obj)
set(SCRDIR scripts)

find_package(OpenCL REQUIRED)
file(MAKE_DIRECTORY ${BINDIR})

# Указание исходных файлов
set(CPPSOURCES ${SRCDIR}/main.cpp ${SRCDIR}/clGEMM.cpp)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${CPPSOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SRCDIR})

# Линковка библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE OpenCL::OpenCL)
target_compile_definitions(${PROJECT_NAME} PRIVATE CL_TARGET_OPENCL_VERSION=100)

# Установка выходного каталога для исполняемого файла
set(EXECUTABLE_OUTPUT_PATH ${BINDIR})

add_custom_command(TARGET myGEMM POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:myGEMM> ${CMAKE_SOURCE_DIR}/myGEMM.exe
)

# Цель для запуска программы
add_custom_target(run
        COMMAND ./${BINDIR}/${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
)

# Цель для очистки
add_custom_target(cleanup
        COMMAND rm -f ${OBJDIR}/*.o
        COMMAND rm -f ${BINDIR}/${PROJECT_NAME}
        COMMAND rm -f ${BINDIR}/${PROJECT_NAME}.*
)
