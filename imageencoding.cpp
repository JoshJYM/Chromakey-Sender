#include "imageencoding.h"

#define BLOCK_SIZE 163843
std::vector<JOCTET> jpegBbuffer;

ImageEncoding::ImageEncoding(QObject *parent)
{}

ImageEncoding::~ImageEncoding()
{
    qDebug("delete ImageEncoding...");
}

void ImageEncoding::setQuality(int quality)
{
    this->encodingQuality = quality;
    qDebug("Info : <%d> quality!",this->encodingQuality);
}

static void jpegInitDestination(j_compress_ptr cinfo)
{
    jpegBbuffer.resize(BLOCK_SIZE);
    cinfo->dest->next_output_byte = &jpegBbuffer[0];
    cinfo->dest->free_in_buffer = jpegBbuffer.size();
}

boolean jpegEmptyOutputBuffer(j_compress_ptr cinfo)
{
    size_t oldsize = jpegBbuffer.size();
    jpegBbuffer.resize(oldsize + BLOCK_SIZE);
    cinfo->dest->next_output_byte = &jpegBbuffer[oldsize];
    cinfo->dest->free_in_buffer = jpegBbuffer.size() - oldsize;

    return true;
}

void jpegTermDestination(j_compress_ptr cinfo)
{
    jpegBbuffer.resize(jpegBbuffer.size() - cinfo->dest->free_in_buffer);
}

void ImageEncoding::writeJPEGMem(QImage& resultImage, QByteArray& jpegEncData)
{
    JSAMPLE *image_buffer;	// Points to large array of R,G,B-order data
    int image_height;       // Number of rows in image
    int image_width;		// Number of columns in image

    image_buffer = resultImage.bits();
    image_width = resultImage.width();
    image_height = resultImage.height();

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */

    cinfo.err = jpeg_std_error(&jerr);

    unsigned char *outBuffer = NULL;
    unsigned long outSize = 0;

    jpeg_create_compress(&cinfo);

    cinfo.image_width = image_width;
    cinfo.image_height = image_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, encodingQuality, TRUE);

    jpeg_mem_dest(&cinfo, &outBuffer, &outSize);
    jpeg_start_compress(&cinfo, TRUE);

    cinfo.dest->init_destination    = &jpegInitDestination;
    cinfo.dest->empty_output_buffer = &jpegEmptyOutputBuffer;
    cinfo.dest->term_destination    = &jpegTermDestination;

    row_stride = image_width * 3;

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    for(unsigned int i = 0 ; i < outSize ; i++)
        jpegEncData.append(outBuffer[i]);

    for(unsigned int i = 0 ; i < jpegBbuffer.size() ; i++)
        jpegEncData.append(jpegBbuffer[i]);
    jpegBbuffer.clear();

}
