faust_parser_header = files('parser.h')
faust_parser_src = files('parser.cpp')


faust_parser_lib = library('faust-parser', 
                           [faust_parser_header, faust_parser_src], 
                           dependencies: icu_dep, 
                           include_directories: faust_src_root_include)
