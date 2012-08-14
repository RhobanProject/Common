cmake_minimum_required (VERSION 2.6)
project (RhobanCommon)

# Options
set(RHOBAN_UTILS_DIRECTORY "utils" CACHE STRING "Utils directory")

option(RHOBANMATH
    "Enables RhobanMath support" OFF)

if(NOT RHOBANMATH)
    add_definitions(-DNO_RHOBANMATH)
endif(NOT RHOBANMATH)

option(SDK
    "Compiles the SDK" OFF)

# Adding dependency with RhobanUtils
include_directories("${RHOBAN_UTILS_DIRECTORY}")

# Communication
set(COM_SOURCES
    communication/Buffer.cpp
    communication/Client.cpp
    communication/Encodings.cpp
    communication/Header.cpp
    communication/Mailbox.cpp
    communication/MailboxEntry.cpp
    communication/Message.cpp
    communication/ServerComponent.cpp
    communication/Server.cpp
    communication/ThreadedClient.cpp
    communication/ThreadedComponent.cpp
    )

if(SDK)
    set(GENERATE_DIR "${PROJECT_BINARY_DIR}/communication")

    make_directory("${GENERATE_DIR}")

    set(COM_SOURCES ${COM_SOURCES}
        communication/BaseConnection.cpp
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
        COMMAND "${GENERATE_PY}" "${GENERATE_DIR}"
        DEPENDS "${COMMAND_STORE}" "${GENERATE_PY}"
        )

    include_directories("${PROJECT_BINARY_DIR}")
endif(SDK)

# Config
set(CONFIG_SOURCES
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

include_directories("${PROJECT_SOURCE_DIR}")
set(ALL_SOURCES ${CONFIG_SOURCES} ${COM_SOURCES})
add_library(RhobanCommon ${ALL_SOURCES})