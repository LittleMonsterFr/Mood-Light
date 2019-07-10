# defined since 2.8.3
if (CMAKE_VERSION VERSION_LESS 2.8.3)
  get_filename_component (CMAKE_CURRENT_LIST_DIR FFTW3fConfig.cmake PATH)
endif ()

# Allows loading FFTW3 settings from another project
set (FFTW3_CONFIG_FILE "FFTW3fConfig.cmake")

set (FFTW3f_LIBRARIES fftw3f)
set (FFTW3f_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set (FFTW3f_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

if (CMAKE_VERSION VERSION_LESS 2.8.3)
  set (CMAKE_CURRENT_LIST_DIR)
endif ()
