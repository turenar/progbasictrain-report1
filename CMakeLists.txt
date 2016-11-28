project(pbt-report1)

find_package(PkgConfig)
add_definitions(-DUSE_LIBPNG -DUSE_LIBSIXEL -DUSE_GTK)

pkg_check_modules(GTK REQUIRED gtk+-3.0)

# use the variable <RESULTNAME>_INCLUDE_DIRS to get include paths
include_directories(${GTK_INCLUDE_DIRS})

include_directories(externals/cutest src test)
add_library(pbmcore STATIC src/pbm.h src/pbm.c src/logger.c src/logger.h src/pbmutil.h src/pbmutil.c)
add_library(pbmfilter STATIC
        src/filter/filter_noise.c src/filter/filter_common.h src/filter/filter_common.c
        src/filter/filter_icm.c src/filter/filter_icm2.c src/filter/filters.h
        src/filter/filters.c src/filter/filter_diff.c)
add_library(pbmcodecs STATIC
        src/codec/codecs.h src/codec/codecs.c src/codec/codec_pbm.c src/codec/codec_png.c src/codec/codec_sixel.c
        src/codec/codec_gtk.c src/codec/codec_count.c)
add_executable(noiser src/noiser_main.c)
target_link_libraries(noiser pbmcore pbmfilter pbmcodecs)
add_executable(denoiser src/denoiser_main.c src/codec/codec_pbm.c test/filter/filter_noise_test.c)
target_link_libraries(denoiser pbmcore pbmfilter pbmcodecs)
add_executable(general src/general_main.c)
target_link_libraries(general pbmcore pbmfilter pbmcodecs)
add_executable(pbmcore_test EXCLUDE_FROM_ALL test/pbm_test.c test/test_pbmcore.h test/test_pbmcore.c)
include_directories(pbmcore_test src)
add_executable(pbmcodec_test EXCLUDE_FROM_ALL
        test/codec/codec_pbm_test.c test/codec/test_pbmcodec.c test/codec/test_pbmcodec.h test/codec/codec_png_test.c)
include_directories(pbmcodec_test src)
add_executable(pbmfilter_test EXCLUDE_FROM_ALL test/filter/filter_noise_test.c test/filter/test_pbmfilter.c test/filter/test_pbmfilter.h)
include_directories(pbmfilter_test src)
