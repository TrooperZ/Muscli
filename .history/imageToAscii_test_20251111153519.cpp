#include <iostream>
#include <vector>
#include <algorithm> // for std::clamp

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Define the ASCII character ramp from dark to light
// Characters towards the right are denser and appear darker in a console
const std::string ascii_chars = " .:-=+*#%@";

// Function to convert a pixel's RGB values to luminance (brightness)
// Luminance formula is based on human perception of color brightness
unsigned char get_luminance(unsigned char r, unsigned char g, unsigned char b) {
    // Standard formula for luminance: Y = 0.2126*R + 0.7152*G + 0.0722*B
    // We can use integer arithmetic approximation: Y = (77*R + 257*G + 28*B) / 362
    // Or a simpler, faster average:
    return (unsigned char)((r + g + b) / 3);
}

int main() {
    int input_width, input_height, channels;
    const char* input_filename = "starboy.jpg"; // Replace with your input file

    // We force the image to load with 1 channel (Grayscale) for simplicity
    // STBI_grey means stb will convert it to grayscale automatically
    unsigned char* input_data = stbi_load(input_filename, &input_width, &input_height, &channels, STBI_grey);

    if (input_data == nullptr) {
        std::cerr << "Error: Failed to load image " << input_filename << std::endl;
        return 1;
    }

    std::cout << "Loaded image: " << input_filename << " (" << input_width << "x" << input_height << ")" << std::endl;

    // Define the target ASCII art dimensions
    const int ascii_width = 15;
    const int ascii_height = 15;
    
    std::cout << "\nGenerating " << ascii_width << "x" << ascii_height << " ASCII Art:\n" << std::endl;

    // Iterate over the target ASCII grid
    for (int y = 0; y < ascii_height; ++y) {
        for (int x = 0; x < ascii_width; ++x) {
            
            // Map the small ASCII coordinate back to the original image coordinates
            // This performs nearest-neighbor sampling/scaling implicitly
            int original_x = (int)((float)x / ascii_width * input_width);
            int original_y = (int)((float)y / ascii_height * input_height);

            // Clamp coordinates to ensure they stay within bounds
            original_x = std::clamp(original_x, 0, input_width - 1);
            original_y = std::clamp(original_y, 0, input_height - 1);

            // Get the pixel index (since we forced STBI_grey, channels is implicitly 1)
            int pixel_index = (original_y * input_width + original_x);
            unsigned char brightness = input_data[pixel_index];

            // Map the 0-255 brightness value to an index in our ASCII character ramp
            // Higher brightness should map to characters on the *left* of the string (less dense)
            int char_index = (brightness * (int)ascii_chars.length()) / 256;
            
            // Clamp index just in case of edge cases
            char_index = std::clamp(char_index, 0, (int)ascii_chars.length() - 1);

            std::cout << ascii_chars[char_index];
        }
        std::cout << std::endl; // Newline at the end of each row
    }

    // Free the image data
    stbi_image_free(input_data);

    return 0;
}
