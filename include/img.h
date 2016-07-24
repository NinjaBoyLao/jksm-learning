#ifndef IMG_H
#define IMG_H
extern "C"
{
    extern const struct
    {
        unsigned int width;
        unsigned int height;
        unsigned int bytes_per_pixel;
        unsigned char pixel_data[];
    } TopBar;

    extern const struct
    {
        unsigned int width;
        unsigned int height;
        unsigned int bytes_per_pixel;
        unsigned char pixel_data[];
    } tbox_img;

    extern const struct
    {
        unsigned int width;
        unsigned int height;
        unsigned int bytes_per_pixel;
        unsigned char pixel_data[];
    } tboxdown_img;

    extern const struct
    {
        unsigned int width;
        unsigned int height;
        unsigned int bytes_per_pixel;
        unsigned char pixel_data[];
    } pbar_empty;

    extern const struct
    {
        unsigned int width;
        unsigned int height;
        unsigned int bytes_per_pixel;
        unsigned char pixel_data[];
    } pbar_full;
}
#endif // IMG_H
