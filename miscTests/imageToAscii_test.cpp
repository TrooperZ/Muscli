#include <iostream>
#include <vector>
#include <algorithm>

// Custom clamp function
int clamp_val(int value, int min_val, int max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    int input_width, input_height, channels;
    const char* input_filename = "starboy.png"; // Replace with your input file

    unsigned char* input_data = stbi_load(input_filename, &input_width, &input_height, &channels, STBI_grey);

    if (input_data == nullptr) {
        std::cerr << "Error: Failed to load image " << input_filename << std::endl;
        return 1;
    }

    std::cout << "Loaded image: " << input_filename << " (" << input_width << "x" << input_height << ")" << std::endl;

    // Define the target ASCII art dimensions, adjusting the width
    const int ascii_width = 30; // Double the width for a roughly correct aspect ratio
    const int ascii_height = 15;
    
    std::cout << "\nGenerating " << ascii_width << "x" << ascii_height << " ASCII Art (Corrected Aspect Ratio):\n" << std::endl;

    const std::string ascii_chars = " .:-=+*#%@";

    for (int y = 0; y < ascii_height; ++y) {
        for (int x = 0; x < ascii_width; ++x) {
            
            // Map the small ASCII coordinate back to the original image coordinates
            int original_x = (int)((float)x / ascii_width * input_width);
            int original_y = (int)((float)y / ascii_height * input_height);

            original_x = clamp_val(original_x, 0, input_width - 1);
            original_y = clamp_val(original_y, 0, input_height - 1);

            int pixel_index = (original_y * input_width + original_x);
            unsigned char brightness = input_data[pixel_index];

            int char_index = (brightness * (int)ascii_chars.length()) / 256;
            char_index = clamp_val(char_index, 0, (int)ascii_chars.length() - 1);

            std::cout << ascii_chars[char_index];
        }
        std::cout << std::endl;
    }

    stbi_image_free(input_data);

    return 0;
}
