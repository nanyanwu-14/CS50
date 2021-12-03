#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check that only one file name has been entered
    if (argc != 2)
    {
        printf("Usage: ./recover input.raw\n");
        return 1;
    }
    
    // Open the file/ check that it is openable
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("File cannot be opened.\n");
        return 1;
    }
   
    // Initialize buffer size
    BYTE buffer[BLOCK_SIZE];
    
    // Initialize counter for the number of image files
    int images = 0;
    
    // Initialize a string to store the names of files
    char filename[8];
    
    // Initialize a variable, img, of the type FILE
    FILE *img;
    
    // Iterate throughout the file and save JPEGs
    while (fread(buffer, BLOCK_SIZE, 1, file))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (images == 0)
            {
                sprintf(filename, "%03i.jpg", images);
                img = fopen(filename, "w");
                fwrite(buffer, BLOCK_SIZE, 1, img);
                images++;
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", images);
                img = fopen(filename, "w");
                fwrite(buffer, BLOCK_SIZE, 1, img);
                images++;
            }
        }
        else if (images > 0)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }
    
    // Close files
    fclose(img);
    fclose(file);
}