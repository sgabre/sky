add_executable(AverageThreeBytes_TestSequence AverageThreeBytesTests.c)

target_link_libraries(AverageThreeBytes_TestSequence module Unity)
add_test(TestSequence_Module AverageThreeBytes_TestSequence)



# TestSequence_Module = FunctionA_UnitTests, FunctionB_UnitTests

# |- unity
# |  \- (Unity sources)
# |- main
# |  \- (entry point of the main application)
# |- module
# |  \- (module_a sources)
# |- Utils
# |  \- Angles(.h, .c)
# |  \- Vector(.h, .c)
# \- test
# \- (Unit-Tests sources)
#
# Angle DMS
# Angle Radians
# Angle Degrees
#
# Unit Testing
# Component Testing
# Integration Testing
# Component integration testing
# System integration testing
# System testing
# Acceptance testing
# add_executable(Function_UnitTestSequence Function_UnitTest.c)
# target_link_libraries(Function_UnitTestSequence module Unity)
# add_test(TestSequence_Module Function_UnitTestSequence )