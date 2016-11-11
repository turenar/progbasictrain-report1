project(pbt-report1)

add_definitions(-DUSE_LIBPNG)

include_directories(externals/cutest src test)
add_library(pbmcore STATIC src/pbm.h src/pbm.c src/logger.c src/logger.h)
add_library(pbmfilter STATIC
        src/filter/filter_noise.c src/filter/filter_common.h src/filter/filter_common.c
        src/filter/filter_icm.c src/filter/filters.h)
add_library(pbmcodecs STATIC
        src/codec/codecs.h src/codec/codecs.c src/codec/codec_pbm.c src/codec/codec_png.c)
add_executable(noiser src/noiser_main.c)
target_link_libraries(noiser pbmcore pbmfilter pbmcodecs)
add_executable(denoiser src/denoiser_main.c src/codec/codec_pbm.c)
target_link_libraries(denoiser pbmcore pbmfilter pbmcodecs)
add_executable(pbm_test EXCLUDE_FROM_ALL test/pbm_test.c)
include_directories(pbm_test src)