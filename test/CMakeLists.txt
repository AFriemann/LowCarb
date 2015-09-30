FILE (GLOB_RECURSE test_util_SRCS "utils/*.cpp" "utils/*.hpp")
FILE (GLOB test_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (test_LIBS ${thirdparty_TEST_LIBRARIES})
SET (test_BIN ${PROJECT_NAME}-unittests)

ADD_EXECUTABLE(${test_BIN} ${test_SRCS} ${test_util_SRCS})
TARGET_LINK_LIBRARIES(${test_BIN} src ${test_LIBS})

# reactivate this to run tests with cmake:
# ADD_CUSTOM_TARGET(check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
# ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS} )
