add_library(nimble INTERFACE)

target_include_directories(nimble INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>  # if you plan to install
)
