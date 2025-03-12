set(CMAKE_BUILD_TYPE Debug)
add_definitions( -DDEBUG )


add_executable(atomic_bitset_calculator_tester atomic_bitset_calculator_tester.cpp)
add_executable(queue_tester queue_tester.cpp)
add_executable(bitset_tester bitset_tester.cpp)

target_link_libraries(atomic_bitset_calculator_tester PRIVATE nimble)
target_link_libraries(queue_tester PRIVATE nimble)
target_link_libraries(bitset_tester PRIVATE nimble)

add_test(
	NAME
		queue_tester
	COMMAND
		$<TARGET_FILE:queue_tester>
	)
