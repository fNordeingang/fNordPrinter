project(fNordPrinter)
cmake_minimum_required(VERSION 2.8)

SET( CMAKE_MODULE_PATH
     "${PROJECT_SOURCE_DIR}/cmake"
     "${CMAKE_MODULE_PATH}" )

Find_Package(CURL REQUIRED)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
if ( NOT CURL_FOUND )
    message(FATAL_ERROR "CURL not found !")
endif ( NOT CURL_FOUND )

Find_Package(Iconv REQUIRED)
INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})
if ( NOT ICONV_FOUND )
    message(FATAL_ERROR "ICONV not found !")
endif ( NOT ICONV_FOUND )


aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(
    fNordPrinter
    ${CURL_LIBRARY}
	${ICONV_LIBRARY}
)
