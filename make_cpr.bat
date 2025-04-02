@echo off
setlocal

REM Define the installation directory inside the project root
set "INSTALL_DIR=%ROOT%\libraries\cpr"

if exist "%INSTALL_DIR%" (
    echo Deleting existing installation directory...
    rmdir /s /q "%INSTALL_DIR%"
)

REM Create a temporary build directory
mkdir cpr_build
cd cpr_build

REM Clone CPR repository (shallow clone for faster download)
git clone --depth=1 https://github.com/libcpr/cpr.git
cd cpr

REM Build Debug version
mkdir build_debug
cd build_debug
cmake .. -DCPR_BUILD_TESTS=OFF -DCPR_USE_SYSTEM_CURL=ON -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
cmake --install .
cd ..

REM Build Release version
mkdir build_release
cd build_release
cmake .. -DCPR_BUILD_TESTS=OFF -DCPR_USE_SYSTEM_CURL=ON -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install .
cd ..

REM Go back and clean up
cd ..
rmdir /s /q cpr_build

echo CPR installed in %INSTALL_DIR%
endlocal
