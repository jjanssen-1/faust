faust_compiler_src = files('main.cpp')

message(faust_parser_lib.path())

executable('faustc', 
           faust_compiler_src,
           include_directories: faust_src_root_include,
           dependencies: [icu_dep, icu_i18_dep, icu_data_dep], 
           link_with: faust_parser_lib)
