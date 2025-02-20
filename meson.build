project('oak', ['cpp'],
  version: '0.1.0',
  default_options: ['cpp_std=20'],
)

oak_root_dir = meson.current_source_dir()

# Googletest imports
gtest = subproject('gtest')
gtest_dep = gtest.get_variable('gtest_dep')
gmock_dep = gtest.get_variable('gmock_dep')
gmock_main_dep = gtest.get_variable('gmock_main_dep')

icu = subproject('icu')
icu_dep = icu.get_variable('icuuc_dep')

subdir('src')
subdir('tests')
