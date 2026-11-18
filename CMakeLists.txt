cmake_minimum_required(VERSION 3.16)
project(CRMSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------------------------------------------------
# MySQL Paths Configuration
# ----------------------------------------------------------------------------
if(DEFINED ENV{MYSQL_DIR})
    set(MYSQL_ROOT "$ENV{MYSQL_DIR}")
elseif(EXISTS "C:/Program Files/MySQL/MySQL Server 8.0")
    set(MYSQL_ROOT "C:/Program Files/MySQL/MySQL Server 8.0")
else()
    message(WARNING "MySQL directory not explicitly found. Ensure headers and libraries are reachable.")
endif()

set(MYSQL_INCLUDE_DIR "${MYSQL_ROOT}/include")
set(MYSQL_LIB_DIR "${MYSQL_ROOT}/lib")
find_library(MYSQL_LIB NAMES libmysql mysqlclient PATHS "${MYSQL_LIB_DIR}")

message(STATUS "Using MySQL Include: ${MYSQL_INCLUDE_DIR}")
message(STATUS "Using MySQL Library: ${MYSQL_LIB}")

# ----------------------------------------------------------------------------
# Include Directories
# ----------------------------------------------------------------------------
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${MYSQL_INCLUDE_DIR}
)

# ----------------------------------------------------------------------------
# Source Files
# ----------------------------------------------------------------------------
set(CORE_SOURCES
    src/database/DatabaseManager.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/EmployeeRepository.cpp
    src/repositories/ProductRepository.cpp
    src/repositories/OrderRepository.cpp
    src/repositories/TicketRepository.cpp
    src/repositories/AnalyticsRepository.cpp
    src/services/CustomerService.cpp
    src/services/EmployeeService.cpp
    src/services/ProductService.cpp
    src/services/OrderService.cpp
    src/services/TicketManager.cpp
    src/services/AnalyticsService.cpp
    src/ui/MenuUI.cpp
)

# ----------------------------------------------------------------------------
# Main Executable: crm_system
# ----------------------------------------------------------------------------
add_executable(crm_system
    src/main.cpp
    ${CORE_SOURCES}
)

target_link_libraries(crm_system PRIVATE ${MYSQL_LIB})

# ----------------------------------------------------------------------------
# Test Executable: crm_tests
# ----------------------------------------------------------------------------
add_executable(crm_tests
    tests/test_runner.cpp
    src/database/DatabaseManager.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/EmployeeRepository.cpp
    src/repositories/ProductRepository.cpp
    src/repositories/OrderRepository.cpp
    src/repositories/TicketRepository.cpp
    src/repositories/AnalyticsRepository.cpp
    src/services/CustomerService.cpp
    src/services/EmployeeService.cpp
    src/services/ProductService.cpp
    src/services/OrderService.cpp
    src/services/TicketManager.cpp
    src/services/AnalyticsService.cpp
)

target_link_libraries(crm_tests PRIVATE ${MYSQL_LIB})

# ----------------------------------------------------------------------------
# Post-build: Copy runtime DLLs to executable directory
# ----------------------------------------------------------------------------
if(WIN32)
    set(MYSQL_BIN_DIR "${MYSQL_ROOT}/bin")
    set(DLLS_TO_COPY
        "${MYSQL_LIB_DIR}/libmysql.dll"
        "${MYSQL_BIN_DIR}/libssl-3-x64.dll"
        "${MYSQL_BIN_DIR}/libcrypto-3-x64.dll"
    )
    foreach(DLL_PATH ${DLLS_TO_COPY})
        if(EXISTS "${DLL_PATH}")
            add_custom_command(TARGET crm_system POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL_PATH}" $<TARGET_FILE_DIR:crm_system>
            )
            add_custom_command(TARGET crm_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DLL_PATH}" $<TARGET_FILE_DIR:crm_tests>
            )
        endif()
    endforeach()
endif()
