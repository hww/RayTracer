/**
*  @file picture.h
*  @poject RayTracer for online courses
*  @brief The rendering frame
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_film_h
#define RayTracer2_film_h

#include <vector>
#include "FreeImage/FreeImage.h"

#define CLAMP1(v)  (v > 1.0 ? 1.0 : v)
#define CLAMP0(v)  (v < 0.0 ? 0.0 : v)
#define CLAMP01(v) (CLAMP0(CLAMP1(v)))

using namespace std;
/**
 *   @brief Film class is represent target piicture
 *
 *   @usage:
 *
 *   Write the color to (sample.x, sample.y) on the image
 *   void commit(Sample& sample, Color& color)
 *
 *   Output image to a file
 *   void writeImage();
 *
 *   @notes:
 *
 *   Can be implemented just by a 2D array of Color (Later on, we can
 *   implement more complicated things such as multi-sample per pixel, or
 *   post processing, eg. tone mapping in this class)
 */
class Picture {
public:
    /// Construct the image with given size and antialising mode
    /// @param w The width of picture
    /// @param h The height of picture
    /// @param antialiasing value (aka how many ray-traces per single pixel)
    Picture(int w, int h)
    : defaultColor(0,0,0)
    , width(w)
    , height(h)
    {
        int size = width * height;
        image = new Color[size];
        for (int i = 0; i < size; i++) {
            image[i] = defaultColor;
        }
    }
    /// Destructor
    ~Picture()
    {
        delete[] image;
    }
    /// Write image to the file
    /// @param fileName the path to the target file
    /// @param antialiasing quantity of rays per single pixel
    void writeImage(const char* fileName, float antialias) {
        FreeImage_Initialise();
        FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
        RGBQUAD color;
        for (int y = 0; y < height; y++)
        {
            int offset = y * width;
            for (int x = 0; x < width; x++)
            {
                int idx = x + offset;
                color.rgbRed   = (BYTE)(255 * CLAMP01(image[idx].r / antialias));
                color.rgbGreen = (BYTE)(255 * CLAMP01(image[idx].g / antialias));
                color.rgbBlue  = (BYTE)(255 * CLAMP01(image[idx].b / antialias));
                FreeImage_SetPixelColor(bitmap, x, y, &color);
            }
        }

        if (FreeImage_Save(FIF_PNG, bitmap, fileName, 0))
            cout << "Image Successfully Saved!" << endl;
        FreeImage_DeInitialise();
    }
    /// Set color of given pixel
    inline void commit(int x, int y, const Color& c) {
        image[y * width + x] += c;
    }
    /// Get pixel in coordinates
    inline Color* getPixef(int x, int y) {
        return &image[y * width + x];
    }
private:

    Color* image;       //< Target image
    int width;          //< width of image
    int height;         //< height of image
    Color defaultColor; //< default color for this image

};

#endif
