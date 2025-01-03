if(EXISTS "C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra.exe")
  if(NOT EXISTS "C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra[1]_tests.cmake" OR
     NOT "C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra[1]_tests.cmake" IS_NEWER_THAN "C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra.exe" OR
     NOT "C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("C:/MinGW/share/cmake-3.30/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra.exe]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[test_algebra_TESTS]==]
      CTEST_FILE [==[C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("C:/Users/cypri/Desktop/info/C++/projects/btc_horcrux/build/test_algebra[1]_tests.cmake")
else()
  add_test(test_algebra_NOT_BUILT test_algebra_NOT_BUILT)
endif()
