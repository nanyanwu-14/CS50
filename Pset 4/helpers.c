#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Average red, blue and green values in a pixel and set the new red, blue, and green values equal to this average
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round((float)(image[i][j].rgbtBlue +  image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3);
            image[i][j].rgbtBlue = image[i][j].rgbtRed = image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    const int MAX = 255;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the sepia values
            int sepiaRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue));
            int sepiaGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue));
            int sepiaBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue));
            
            // If any of the sepia values are greater than 255, set them equal to 255
            if (sepiaRed > MAX)
            {
                sepiaRed = MAX;
            }
            if (sepiaGreen > MAX)
            {
                sepiaGreen = MAX;
            }
            if (sepiaBlue > MAX)
            {
                sepiaBlue = MAX;
            }
            
            // Set red, green, and blue values equal to their new sepia values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int sampleRed, sampleGreen, sampleBlue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < round(width / 2); j++)
        {
            // Store RGB values from the left side of the image
            sampleRed = image[i][j].rgbtRed;
            sampleGreen = image[i][j].rgbtGreen;
            sampleBlue = image[i][j].rgbtBlue;
           
            // Copy RGB values from the right side of the image to their new position on the left side
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            
            // Copy RGB values from the left side of the image to their new position on the right side
            image[i][width - 1 - j].rgbtRed = sampleRed;
            image[i][width - 1 - j].rgbtGreen = sampleGreen;
            image[i][width - 1 - j].rgbtBlue = sampleBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a duplicate of the image matrix
    RGBTRIPLE(duplicate[height][width]);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            duplicate[i][j] = image[i][j];
        }
    }
    // Sum the RGB values around the point located at (i , j). Average these values and input them into the image matrix. 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int points = 0;
            int redcount = 0;
            int greencount = 0;
            int bluecount = 0;

            for (int k = -1; k < 2 ; k++)
            {
                if (i + k < 0 || i + k > (height - 1))
                {
                    continue;
                }
                for (int l = -1; l < 2; l++)
                {                  
                    if (j + l < 0 || j + l > (width - 1))
                    {
                        continue;
                    }
                    points++;
                    redcount += duplicate[i + k][j + l].rgbtRed;
                    greencount += duplicate[i + k][j + l].rgbtGreen;
                    bluecount += duplicate[i + k][j + l].rgbtBlue;
                }
            }
            image[i][j].rgbtRed = round((float)redcount / points);
            image[i][j].rgbtGreen = round((float)greencount / points);
            image[i][j].rgbtBlue = round((float)bluecount / points);
        }
    }
    return;
}