set ROOT=%~dp0
set ROOT=%ROOT:\=\\%

set WXW_ROOT=%ROOT%\libraries\wxWidgets
set WXW_LIBS=%WXW_ROOT%\lib\vc_x64_dll
set CMAKE_PREFIX_PATH=%WXW_ROOT%;%CMAKE_PREFIX_PATH%
set PATH=%WXW_LIBS%;%PATH%