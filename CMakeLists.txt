cmake_minimum_required (VERSION 2.6)
project (RhobanCommon)

# Options
set(UTILS_DIRECTORY "utils" CACHE STRING "Utils directory")

option(RHOBANMATH
    "Enables RhobanMath support" OFF)

if(NOT RHOBANMATH)
    add_definitions(-DNO_RHOBANMATH)
endif(NOT RHOBANMATH)

option(SDK
    "Compiles the SDK" OFF)

# Adding dependency with RhobanUtils
include_directories("${UTILS_DIRECTORY}")

# Communication
set(COM_SOURCES
    communication/Buffer.cpp
    communication/Client.cpp
    communication/Connection.cpp
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
    set(COM_SOURCES ${COM_SOURCES}
        communication/CommandsStore.cpp
	communication/tests/allTests.cpp
        ${PROJECT_BINARY_DIR}/MessageBuilder.cpp
        )

    # Generator for MessageBuilder
    set(MESSAGE_BUILDER "${PROJECT_BINARY_DIR}/MessageBuilder.h"
        "${PROJECT_BINARY_DIR}/MessageBuilder.cpp")

    add_custom_command (
      OUTPUT ${MESSAGE_BUILDER}
      COMMAND ${GENERATE_PY} ${MESSAGE_BUILDER}
      DEPENDS ${COMMAND_STORE}
    )
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
include_directories("${PROJECT_BINARY_DIR}")
set(ALL_SOURCES ${CONFIG_SOURCES} ${COM_SOURCES})
add_library(RhobanCommon ${ALL_SOURCES})
