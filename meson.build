project('oak', ['cpp'],
  version: '0.1.0',
)

add_project_arguments('-fPIC', language: 'cpp')

oak_root_dir = meson.current_source_dir()

# Googletest imports
gtest = subproject('gtest')
gtest_dep = gtest.get_variable('gtest_dep')
gmock_dep = gtest.get_variable('gmock_dep')
gmock_main_dep = gtest.get_variable('gmock_main_dep')

icu = subproject('icu', default_options: ['default_library=static'])
icu_dep = icu.get_variable('icuuc_dep')
icu_i18_dep = icu.get_variable('icui18n_dep')
icu_data_dep = icu.get_variable('icudata_dep')

icu_includes = [
  icu.get_variable('i18ninc')
]

subdir('src')
subdir('tests')
