project (Catch)

# frame_header only library, therefore INTERFACE
add_library(catch INTERFACE)

# INTERFACE targets only have INTERFACE properties
target_include_directories(catch INTERFACE catch)