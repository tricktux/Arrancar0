include_directories(${CMAKE_SOURCE_DIR}/inc)

include_directories(SYSTEM
		"${RAPIDJSON_INCLUDE_DIRS}"
		"${GLOG_INCLUDE_DIRS}"
		"${GTEST_INCLUDE_DIRS}"
	)

# Copy test configuration file to output
file(
		COPY config/
		DESTINATION ${EXECUTABLE_OUTPUT_PATH}
		FILES_MATCHING PATTERN "*.json"
	)

# Add executable and list sources to the executable
add_executable(test_config test_config.cpp ${CMAKE_SOURCE_DIR}/src/config.cpp)

# Link test executable against gtest & gtest_main
target_link_libraries(test_config
		${GTEST_BOTH_LIBRARIES}
		${GLOG_LIBRARIES}
	)

add_test(test_config ${EXECUTABLE_OUTPUT_PATH})
