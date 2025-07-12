cmake_minimum_required(VERSION 3.15)
project(timelib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_HEADERS
        include/
        extern/date/include
)

set(LIB_SOURCES
        src/time.cpp
        extern/date/src/tz.cpp
)

add_library(timelib STATIC ${LIB_SOURCES})

target_include_directories(timelib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extern/date/include>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/date>
)

find_package(CURL REQUIRED)
target_link_libraries(timelib PUBLIC CURL::libcurl)

# install
install(TARGETS timelib
        EXPORT timelibTargets
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY extern/date/include/ DESTINATION include)
install(EXPORT timelibTargets
        FILE timelibTargets.cmake
        NAMESPACE timelib::
        DESTINATION lib/cmake/timelib
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/timelibConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/timelibConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/timelibConfig.cmake"
        INSTALL_DESTINATION lib/cmake/timelib
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/timelibConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/timelibConfigVersion.cmake"
        DESTINATION lib/cmake/timelib
)

# packaging
set(CPACK_PACKAGE_NAME "timelib")
set(CPACK_PACKAGE_VENDOR "unium")
set(CPACK_PACKAGE_DESCRIPTION "library for time stuff")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_GENERATOR "TGZ;ZIP")
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

include(CPack)