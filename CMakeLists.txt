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
	option(MSVC_SHARED_RT "Used shared runtime libraries" OFF)

			IF(NOT MSVC_SHARED_RT)
	foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
	endif(${flag_var} MATCHES "/MD")
	endforeach(flag_var)
	ENDIF(NOT MSVC_SHARED_RT)

	add_definitions(-D_USE_MATH_DEFINES)
	add_definitions(-DMSVC)
	add_definitions(-D_WINSOCKAPI_)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (MSVC)


option(SDK
    "Compiles the SDK" OFF)

option(SERVER_USES_ZMQ
	"Rhoban Server uses Zmq" OFF)

if(SERVER_USES_ZMQ)
    add_definitions(-DSERVER_USES_ZMQ)
endif(SERVER_USES_ZMQ)

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
    communication/ServerHub.cpp
    communication/RemoteClient.cpp
    communication/NetworkComponent.cpp
    communication/BaseConnection.cpp
    communication/Connections.cpp
    communication/Mailbox.cpp
    communication/Callable.cpp
    communication/ServerComponent.cpp
    communication/Server.cpp
    communication/ZmqContext.cpp
    )

if(SERVER_USES_ZMQ)

	set(COM_SOURCES
    communication/Server2.cpp
	"${COM_SOURCES}"
	)

endif(SERVER_USES_ZMQ)

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

# ADD HEADERS FILES 
foreach (SOURCE ${ALL_SOURCES})
	set (FILENAME_PREFIX)
	set (FILENAME_PATH)
	GET_FILENAME_COMPONENT(FILENAME_PREFIX ${SOURCE} NAME_WE)
	GET_FILENAME_COMPONENT(FILENAME_PATH ${SOURCE} PATH)
		
	set (FILENAME ${FILENAME_PATH}/${FILENAME_PREFIX})
	
	if(EXISTS "${PROJECT_SOURCE_DIR}/${FILENAME}.h")
#		message("Adding header " ${FILENAME}.h)
        set (ALL_HEADERS ${ALL_HEADERS} "${FILENAME}.h")
	endif(EXISTS "${PROJECT_SOURCE_DIR}/${FILENAME}.h")
	if(EXISTS "${PROJECT_SOURCE_DIR}/${FILENAME}.hpp")
#		message("Adding header " ${FILENAME}.hpp)
        set (ALL_HEADERS ${ALL_HEADERS} "${FILENAME}.hpp")
	endif(EXISTS "${PROJECT_SOURCE_DIR}/${FILENAME}.hpp")
endforeach (SOURCE)

set (ALL_HEADERS ${ALL_HEADERS} "communication/rhobanProtocol.h")

add_library (RhobanCommon ${ALL_SOURCES} ${ALL_HEADERS})
target_link_libraries (RhobanCommon ${EXTRA_LIBS})
