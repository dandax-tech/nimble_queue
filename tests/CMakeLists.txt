set(CMAKE_BUILD_TYPE Debug)
add_definitions( -DDEBUG )


add_executable(atomic_bitset_calculator_tester atomic_bitset_calculator_tester.cpp)
add_executable(queue_tester_st queue_tester_st.cpp)
add_executable(bitset_tester bitset_tester.cpp)
add_executable(queue_print queue_print.cpp)


target_link_libraries(atomic_bitset_calculator_tester PRIVATE nimble)
target_link_libraries(queue_tester_st PRIVATE nimble)
target_link_libraries(bitset_tester PRIVATE nimble)
target_link_libraries(queue_print PRIVATE nimble)

enable_testing()

add_test(
	NAME
		atomic_bitset_calculator_tester
	COMMAND
		$<TARGET_FILE:atomic_bitset_calculator_tester>
	)

add_test(
	NAME
		queue_tester_st
	COMMAND
		$<TARGET_FILE:queue_tester_st>
	)

add_test(
	NAME
		bitset_tester
	COMMAND
		$<TARGET_FILE:bitset_tester>
	)
	