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

Find_Package(ICONV REQUIRED)
INCLUDE_DIRECTORIES(${ICONV_INCLUDE_DIR})
if ( NOT ICONV_FOUND )
    message(FATAL_ERROR "ICONV not found !")
endif ( NOT ICONV_FOUND )

Find_Package(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
if ( NOT Boost_FOUND )
    message(FATAL_ERROR "Boost not found !")
endif ( NOT Boost_FOUND )



aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(
    fNordPrinter
    ${CURL_LIBRARY}
	${ICONV_LIBRARY}
	${Boost_LIBRARY}
)
