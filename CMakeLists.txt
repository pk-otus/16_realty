cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
   project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
   project(realty VERSION 0.0.1)
endif()

file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h)

add_executable(rclst rclst.cpp ${HEADER_FILES})
add_executable(rclss rclss.cpp ${HEADER_FILES})

set_target_properties(rclst rclss PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	find_package(DLIB REQUIRED )

	set_target_properties(rclst rclss PROPERTIES COMPILE_OPTIONS -W3)
	target_link_libraries(rclst dlib::dlib)
	target_link_libraries(rclss dlib::dlib)
else()
	find_package(BLAS REQUIRED )

	set_target_properties(rclst rclss PROPERTIES COMPILE_OPTIONS -Wpedantic -Wall -Wextra)
	target_link_libraries(rclst pthread dlib openblas)
	target_link_libraries(rclss pthread dlib openblas)
endif()

install(TARGETS rclst RUNTIME DESTINATION bin)
install(TARGETS rclss RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT a@a.a)

include (CPack)

