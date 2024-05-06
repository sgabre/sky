## CMakeLists @ SoftwareComponent/sources Directory 
target_sources( ${SOFWARE_COMPONENT_NAME}
    PRIVATE SoftwareComponent.c SoftwareComponent.h
    PUBLIC FILE_SET HEADERS 
    BASE_DIRS ${PROJECT_SOURCE_DIR})
