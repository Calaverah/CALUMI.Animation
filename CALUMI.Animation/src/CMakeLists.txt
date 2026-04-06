add_subdirectory(math)
add_subdirectory(utilities)
add_subdirectory(univ)
add_subdirectory(sfbgs)
add_subdirectory(interfaces)
add_subdirectory(io)

target_sources(${PROJECT_NAME} PRIVATE internal/internalplatform.h)
target_sources(${PROJECT_NAME} PRIVATE internal/internalvectordef.h)