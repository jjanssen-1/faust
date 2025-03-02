faust_parser_header = files('parser.h', 'lexer.h')
faust_parser_src = files('parser.cpp', 'lexer.cpp')

faust_parser_lib = static_library('faust-parser', 
                                  sources: [faust_parser_header, faust_parser_src], 
                                  dependencies: [icu_dep, icu_i18_dep, icu_data_dep], 
                                  include_directories: [faust_src_root_include, icu_includes])
