#include "config.inc.h"

#ifdef USE_GTK

#include "codecs.h"
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pbm.h>
#include "logger.h"

static void free_pixels(guchar* pixels, gpointer data);

pbm_error_t pbmcodec_gtk_write(const pbm_info* info, FILE* fp) {
	UNUSED_VAR(fp);

	gtk_init(0, NULL);

	gsize allocation_size = 3u * sizeof(guint8) * (gsize) (info->height * info->width);
	guint8* rgb_image = g_malloc(allocation_size);
	guint8* out_p = rgb_image;
	uint8_t** row_p = info->data;
	for (int y = 0; y < info->height; ++y) {
		uint8_t* col_p = *row_p++;
		for (int x = 0; x < info->width; ++x) {
			guint8 brightness = (guint8) (*col_p++ ? 0 : 255);
			*out_p++ = brightness; // R
			*out_p++ = brightness; // G
			*out_p++ = brightness; // B
		}
	}

	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_data(rgb_image, GDK_COLORSPACE_RGB, FALSE, 8, info->width, info->height,
	                                             info->width * 3, free_pixels, NULL);
	if (pixbuf == NULL) {
		return PBM_ALLOCATION_FAILED;
	}

	GtkWidget* image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_container_add(GTK_CONTAINER(window), image);
	gtk_widget_show_all(window);
	gtk_main();

	return PBM_SUCCESS;
}

static void free_pixels(guchar* pixels, gpointer data) {
	UNUSED_VAR(data);
	g_free(pixels);
}

#else
typedef int make_iso_compilers_happy; // suppress "ISO C forbids an empty translation unit"
#endif
