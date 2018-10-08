/**
*  @file sampler.h
*  @poject RayTracer for online courses
*  @brief Sampler is an iterator for the screen space coordinates
*  @autor Valery P. (github.com/hww)
*/

#ifndef RayTracer2_sampler_h
#define RayTracer2_sampler_h

#include "vector.h"

/**
 * Info:
 *   Sampler is similar to iterator. It will generate (x,y) of a screen sample and return true.
 *
 *   Next time it gets called, it will generate another sample for the next pixel.
 *   It will return false when all the samples from all the pixels
 *
 *   are generated. (In our case, we generate 1 sample per pixel, at the pixel sample. Later on,
 *   if we want to do multi-sample per pixel, we need to modify this class.
 *
 * Methods:
 *   bool getSample(Sample* sample);
 */
class Sampler {
public:
    /// Construct new sampler
    /// @param w The width of picture
    /// @param h The height of picture
    /// @param aalias Mount of rays/samples per each pixel
    Sampler(float w, float h, int antialysing) {
        width = w;
        height = h;
        pxWidth = 1.0f / width;
        pxHeight = 1.0f / height;
        centerX = 0.5f * pxWidth;
        centerY = 0.5f * pxHeight;
        // Build antialysing tables
        int aaPow2 = antialysing * antialysing;
        for (int x = 0; x < antialysing; x++)
        {
            for (int y = 0; y < antialysing; y++)
            {
                int aaIndex = x + y * antialysing;
                float xaa = 1.0f / (float)antialysing * (float)x;
                float yaa = 1.0f / (float)antialysing * (float)y;
                aaTableX[aaIndex] = pxWidth * xaa;
                aaTableY[aaIndex] = pxHeight * yaa;
                printf("AA: %d %.3f %.3f\n", aaIndex, xaa, yaa);
            }
        }
    }
    /// Get random screen position in given pixel coordinates
    /// @param index is the sample's index. For antialising 1
    /// will be all the time 0. Also for index 0 sampler return
    /// exact coordinate of pixel
    /// @param px pixel's x coordinate
    /// @param py pixel's y coordinate
    /// @result random position inside pixel
    Vector getRandomSample(int index, float px, float py) {
        float x, y;
        if (index == 0) {
            // first sample caclulate perfectly
            x = centerX + px * pxWidth;
            y = centerY + py * pxHeight;
        }
        else {
            // all other calculate randomply inside one pixel
            x = ((rand() % 50) / 50.0f + px) * pxWidth;
            y = ((rand() % 50) / 50.0f + py) * pxHeight;
        }
        return Vector(x * 2.0f - 1.0f, y * 2.0f - 1.0f, 0.0f);
    }
    /// Get random screen position in given pixel coordinates
    /// @param index is the sample's index. For antialising 1
    /// will be all the time 0. Also for index 0 sampler return
    /// exact coordinate of pixel
    /// @param px pixel's x coordinate
    /// @param py pixel's y coordinate
    /// @param sampleWeight
    /// @result random position inside pixel
    Vector getGridSample(int index, float px, float py) {
        float x, y;
        // first sample caclulate perfectly
        x = centerX + px * pxWidth + aaTableX[index];
        y = centerY + py * pxHeight + aaTableY[index];
        return Vector(x * 2.0f - 1.0f, y * 2.0f - 1.0f, 0.0f);
    }

private:
    float width, height;        //< Target image width and height
    float pxWidth, pxHeight;    //< One divided by width and height
    float centerX, centerY;     //< Screen center position
    float aaTableX[128];        //< Antialising's pixel coordinates
    float aaTableY[128];        //< Antialising's pixel coordinates
};

#endif
