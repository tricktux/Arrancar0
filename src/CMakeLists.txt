# All the source files for the bot.
file(GLOB SOURCES "*.cpp" "../inc/*.hpp")

include_directories(SYSTEM
	"${SC2Api_INCLUDE_DIRS}"
	"${GLOG_INCLUDE_DIRS}"
	"${GTEST_INCLUDE_DIRS}"
	"${RAPIDJSON_INCLUDE_DIRS}"
	)

# Show more warnings at compiletime.
if (MSVC)
    # FIXME: put Windows specific options here.
else ()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif ()

# Create the executable.
add_executable(Arrancar0 ${SOURCES})
target_link_libraries(Arrancar0
	${SC2Api_LIBRARIES}
	${GLOG_LIBRARIES}
	${GTEST_BOTH_LIBRARIES}
	)

if (APPLE)
	target_link_libraries(Arrancar0 "-framework Carbon")
endif ()

# Linux specific.
if (UNIX AND NOT APPLE)
	target_link_libraries(Arrancar0 pthread dl)
endif ()
