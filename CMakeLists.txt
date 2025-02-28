cmake_minimum_required(VERSION 3.10)
project(FinanceManager)

set(CMAKE_CXX_STANDARD 17)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Add executable
add_executable(FinanceManager main.cpp)
target_link_libraries(FinanceManager ${wxWidgets_LIBRARIES})
