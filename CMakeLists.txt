project(pbt-report1)

include_directories(externals/cutest src test)
add_library(pbmcore src/pbm.h src/pbm.c)
add_executable(noiser src/noiser_main.c)
add_executable(pbm_test EXCLUDE_FROM_ALL test/pbm_test.c)
