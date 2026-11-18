@echo off
setlocal

echo ========================================================
echo        Building CRM Management System (C++17)
echo ========================================================

REM 1. Set 64-bit modern GCC in PATH
if exist "%LOCALAPPDATA%\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\g++.exe" (
    set "PATH=%LOCALAPPDATA%\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin;%PATH%"
)
if exist "C:\mingw64\bin\g++.exe" (
    set "PATH=C:\mingw64\bin;%PATH%"
)

where g++ >nul 2>nul
if errorlevel 1 (
    echo [ERROR] g++ compiler not found in PATH.
    echo Please install a 64-bit MinGW-w64 compiler.
    exit /b 1
)

echo [INFO] Found C++ compiler.

REM 2. Detect MySQL Installation
if defined MYSQL_DIR (
    set "MYSQL_ROOT=%MYSQL_DIR%"
) else if exist "C:\Program Files\MySQL\MySQL Server 8.0" (
    set "MYSQL_ROOT=C:\Program Files\MySQL\MySQL Server 8.0"
) else (
    echo [ERROR] MySQL Server 8.0 not found at C:\Program Files\MySQL\MySQL Server 8.0.
    echo Set the MYSQL_DIR environment variable to your MySQL installation directory.
    exit /b 1
)

set "MYSQL_INC=%MYSQL_ROOT%\include"
set "MYSQL_LIB=%MYSQL_ROOT%\lib"
set "MYSQL_BIN=%MYSQL_ROOT%\bin"

echo [INFO] Found MySQL at: %MYSQL_ROOT%

REM 3. Create output directory
if not exist "bin" mkdir "bin"

REM 4. Source files
set "SOURCES=src\main.cpp src\database\DatabaseManager.cpp src\repositories\CustomerRepository.cpp src\repositories\EmployeeRepository.cpp src\repositories\ProductRepository.cpp src\repositories\OrderRepository.cpp src\repositories\TicketRepository.cpp src\repositories\AnalyticsRepository.cpp src\services\CustomerService.cpp src\services\EmployeeService.cpp src\services\ProductService.cpp src\services\OrderService.cpp src\services\TicketManager.cpp src\services\AnalyticsService.cpp src\ui\MenuUI.cpp"

set "TEST_SOURCES=tests\test_runner.cpp src\database\DatabaseManager.cpp src\repositories\CustomerRepository.cpp src\repositories\EmployeeRepository.cpp src\repositories\ProductRepository.cpp src\repositories\OrderRepository.cpp src\repositories\TicketRepository.cpp src\repositories\AnalyticsRepository.cpp src\services\CustomerService.cpp src\services\EmployeeService.cpp src\services\ProductService.cpp src\services\OrderService.cpp src\services\TicketManager.cpp src\services\AnalyticsService.cpp"

REM 5. Compile Main Application
echo [1/3] Compiling CRM System executable: bin\crm_system.exe...
g++ -std=c++17 -O2 -static-libgcc -static-libstdc++ -Iinclude -I"%MYSQL_INC%" %SOURCES% -L"%MYSQL_LIB%" -lmysql -o "bin\crm_system.exe"
if errorlevel 1 (
    echo [ERROR] Compilation of crm_system.exe failed.
    exit /b 1
)

REM 6. Compile Automated Test Runner
echo [2/3] Compiling Test Runner executable: bin\crm_tests.exe...
g++ -std=c++17 -O2 -static-libgcc -static-libstdc++ -Iinclude -I"%MYSQL_INC%" %TEST_SOURCES% -L"%MYSQL_LIB%" -lmysql -o "bin\crm_tests.exe"
if errorlevel 1 (
    echo [ERROR] Compilation of crm_tests.exe failed.
    exit /b 1
)

REM 7. Copy required runtime DLLs to bin\
echo [3/3] Copying runtime DLLs to bin...
if exist "%MYSQL_LIB%\libmysql.dll" copy /y "%MYSQL_LIB%\libmysql.dll" "bin\" >nul
if exist "%MYSQL_BIN%\libssl-3-x64.dll" copy /y "%MYSQL_BIN%\libssl-3-x64.dll" "bin\" >nul
if exist "%MYSQL_BIN%\libcrypto-3-x64.dll" copy /y "%MYSQL_BIN%\libcrypto-3-x64.dll" "bin\" >nul
if exist "%GCC_BIN%\libwinpthread-1.dll" copy /y "%GCC_BIN%\libwinpthread-1.dll" "bin\" >nul
where libwinpthread-1.dll >nul 2>nul
if %ERRORLEVEL% equ 0 (
    for /f "delims=" %%p in ('where libwinpthread-1.dll') do (
        copy /y "%%p" "bin\" >nul
    )
)

echo ========================================================
echo [SUCCESS] Build completed successfully!
echo.
echo Executables generated:
echo   - Main Application : bin\crm_system.exe
echo   - Test Runner      : bin\crm_tests.exe
echo.
echo Run the application:
echo   bin\crm_system.exe
echo.
echo Run the test suite:
echo   bin\crm_tests.exe
echo ========================================================
