project(pbt-report1)

include_directories(externals/cutest src test)
add_library(pbmcore src/pbm.h src/pbm.c)
add_library(pbmfilter
        src/filter/filter_noise.h src/filter/filter_noise.c src/filter/filter_common.h src/filter/filter_common.c
        src/filter/filter_icm.c src/filter/filter_icm.h src/filter/filters.h)
add_executable(noiser src/noiser_main.c)
add_executable(denoiser src/denoiser_main.c)
add_executable(pbm_test EXCLUDE_FROM_ALL test/pbm_test.c)
