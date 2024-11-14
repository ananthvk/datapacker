gtest_dep = dependency('gtest')

serialization_test = executable(
    'serialization_test',
    sources: ['serialization_test.cpp'],
    dependencies : [ gtest_dep ],
    include_directories: include_dirs,
    cpp_args: extra_args
)
test('serialization_test', serialization_test)

