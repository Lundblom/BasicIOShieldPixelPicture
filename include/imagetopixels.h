unsigned char* parse_image(const char* filename, unsigned *width, unsigned *height);
char* image_to_bitarray(const unsigned char *image, const unsigned w, const unsigned h);
char* resize_bitarray(char *bits, const int sourceW, const int sourceH, const int targetW, const int targetH);
unsigned char* bitarray_to_pixelimage(char* in, int size);