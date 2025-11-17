#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
int main() {
    int width, height, channels;
    // Load an image from a file
    unsigned char* image_data =
        stbi_load("image.png", &width, &height, &channels, 0);

    if (image_data) {
        std::cout << "Image loaded successfully!" << std::endl;
        std::cout << "Width: " << width << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Channels: " << channels << std::endl;

        // You can now access pixel data, for example, the first pixel's red
        // component (assuming 3 channels, RGB)
        if (channels >= 3) {
            std::cout << "First pixel R: " << (int)image_data[0] << std::endl;
            std::cout << "First pixel G: " << (int)image_data[1] << std::endl;
            std::cout << "First pixel B: " << (int)image_data[2] << std::endl;
        }

        // Don't forget to free the image data when done
        stbi_image_free(image_data);
    } else {
        std::cerr << "Failed to load image: " << stbi_failure_reason()
                  << std::endl;
    }
    return 0;
}