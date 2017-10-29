set(test_result_folder ${CMAKE_SOURCE_DIR}/test_reports)
file(MAKE_DIRECTORY ${test_result_folder})
file(REMOVE ${test_result_folder}/test_result_${CMAKE_BUILD_TYPE}.xml)
add_test(
	NAME TestProject
	COMMAND $<TARGET_FILE:${PROJECT}>  "--gtest_output=xml:${test_result_folder}/test_result_$<CONFIGURATION>.xml"
)
