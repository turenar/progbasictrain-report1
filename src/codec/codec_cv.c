#include "config.inc.h"

#ifdef USE_OPENCV

#include "codecs.h"
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include "logger.h"

#define WINDOW_TITLE "Image Preview"

pbm_error_t pbmcodec_cv_write(const pbm_info* info, FILE* fp) {
	UNUSED_VAR(fp);

	CvSize size = {info->width, info->height};
	IplImage* image = cvCreateImage(size, IPL_DEPTH_8U, 1);
	char* out_row_p = image->imageData;
	uint8_t** in_row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		char* out_col_p = out_row_p;
		uint8_t* in_col_p = *in_row_p++;
		for (int x = 0; x < info->width; ++x) {
			*out_col_p++ = (char) (*in_col_p++ ? 0 : 255);
		}
		out_row_p += image->widthStep;
	}

	cvNamedWindow(WINDOW_TITLE, CV_WINDOW_AUTOSIZE);
	cvShowImage(WINDOW_TITLE, image);
	LOG(info, "Press ESC on window to exit.");
	LOG(info, "Press Ctrl+C in console to exit if you closed window...");
	for (;;) {
		int key = cvWaitKey(1000);
		LOG(debug, "cvWaitKey: %d", key);
		if ((key & 0xff) == 0x1b || cvGetWindowHandle(WINDOW_TITLE) == NULL) {
			// cvGetWindowHandle can return null after window closed
			// but some environments (opengl?) does not(?)
			break;
		}
	}
	cvDestroyAllWindows();
	cvReleaseImage(&image);

	return PBM_SUCCESS;
}

#else
typedef int make_iso_compilers_happy; // suppress "ISO C forbids an empty translation unit"
#endif

