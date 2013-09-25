cmake_minimum_required (VERSION 2.6)
project (RhobanCommon)

set (BASE "../")

# Options
set(RHOBAN_UTILS_DIRECTORY 
    "${BASE}/Utils"
    CACHE STRING "RhobanUtils directory")

option(RHOBANMATH
    "Enables RhobanMath support" OFF)

if (NOT RHOBANMATH)
    add_definitions(-DNO_RHOBANMATH)
endif (NOT RHOBANMATH)

if (RHOBANMATH)
    set(RHOBAN_MATH_DIRECTORY "${BASE}/Math"
        CACHE STRING "RhobanMath directory")

    include_directories ("${RHOBAN_MATH_DIRECTORY}")
    set (EXTRA_LIBS ${EXTRA_LIBS} RhobanMath)
endif (RHOBANMATH)

if(MSVC)
	add_definitions(-D_USE_MATH_DEFINES)
	add_definitions(-DMSVC)
	add_definitions(-D_WINSOCKAPI_)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (MSVC)


option(SDK
    "Compiles the SDK" OFF)

# Adding dependency with RhobanUtils
include_directories ("${RHOBAN_UTILS_DIRECTORY}" "${YAML_HEADERS}")
set (EXTRA_LIBS ${EXTRA_LIBS} RhobanUtils)

# Communication
set(COM_SOURCES
    communication/Buffer.cpp
    communication/Encodings.cpp
    communication/Header.cpp
    communication/MailboxEntry.cpp
    communication/Message.cpp
    communication/ServerComponent.cpp
    communication/Server.cpp
    communication/RemoteClient.cpp
    communication/NetworkComponent.cpp
    communication/BaseConnection.cpp
    communication/Connections.cpp
    communication/Mailbox.cpp
    communication/Callable.cpp
    )

if(SDK)
    set(GENERATE_DIR "${PROJECT_BINARY_DIR}/communication")

    make_directory("${GENERATE_DIR}")

    set(COM_SOURCES ${COM_SOURCES}
        communication/CommandsStore.cpp
        communication/tests/allTests.cpp
        "${GENERATE_DIR}/MessageBuilder.cpp"
        "${GENERATE_DIR}/Connection.cpp"
        )

    # Generator for MessageBuilder
    set (GENERATE_FILES
        "${GENERATE_DIR}/MessageBuilder.h"
        "${GENERATE_DIR}/MessageBuilder.cpp"
        "${GENERATE_DIR}/Connection.h"
        "${GENERATE_DIR}/Connection.cpp"
        )

    add_custom_command (
        OUTPUT ${GENERATE_FILES}
        COMMAND python3 "${GENERATE_PY}" "${GENERATE_DIR}"
        DEPENDS "${COMMAND_STORE}" "${GENERATE_PY}"
        )

    include_directories("${PROJECT_BINARY_DIR}")
endif(SDK)

# Config
set (CONFIG_SOURCES
    config/AnalogicSensorConfig.cpp
    config/Config.cpp
    config/DynamixelConfig.cpp
    config/JoypadConfig.cpp
    config/KinectConfig.cpp
    config/LowLevelConfig.cpp
    config/MoveSchedulerConfig.cpp
    config/MoveSchedulerSensorsConfig.cpp
    config/PinsConfig.cpp
    config/PwmConfig.cpp
    config/ServosConfig.cpp
    )

# Motion
set (MOTION_SOURCES
    motion/LinearSpline.cpp
    motion/StmSpawn.cpp
    )

# App
set (APP_SOURCES
    app/App.cpp
    )

include_directories ("${PROJECT_SOURCE_DIR}")
set (ALL_SOURCES ${CONFIG_SOURCES} ${COM_SOURCES} ${MOTION_SOURCES} ${APP_SOURCES})
add_library (RhobanCommon ${ALL_SOURCES})
target_link_libraries (RhobanCommon ${EXTRA_LIBS})
