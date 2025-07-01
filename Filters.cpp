// FCAI - Structured Programing - 2024 - Assignment 3
// Program Name : Image Filter Application
/* Program Descreption : It asks the user to enter the path or the name of the image,
 * then it opens menu to make user able to choose any filter he wants. he can save image in any time.
 * he can apply more than one filter on the same image. he has the option to load new image without exit the program. */
// Last Modification date : 04/13/2024
//  the Black and white filter , Crop filter, Resize filter, Flip filter, and Purple hue filter.
//  Gray Scale filter , Darken and Lighten filter, Merge filter, Detect Edges filter, and Red Hue filter.
//  Invert filter, Frame maker filter, Rotate filter, Blur filter, and SunLight filter.

#include <iostream>
#include <regex>
#include <cstdlib>
#include "Image_Class.h"
#include <limits>
#include <vector>
using namespace std;

// Detect image edges function
Image DetectEdges(Image& image){
    // Initialize a 2D array to store edge detection results
    std::vector<std::vector<int>> arr(image.width, std::vector<int>(image.height, 0));
    arr[image.width][image.height];

    int sobelX[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};

    int sobelY[3][3] = {{1, 2, 1},
                        {0, 0, 0},
                        {-1, -2, -1}};

    // Apply the Sobel operator to detect edges
    for (int i = 1; i < image.height - 1; i++){
        for (int j = 1; j < image.width - 1; j++) {
            int Ix = 0, Iy = 0;
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    Ix += sobelX[x][y] * image.getPixel(j + x - 1, i + y - 1, 0);
                    Iy += sobelY[x][y] * image.getPixel(j + x - 1, i + y - 1, 0);
                }
            }
            // Calculate the magnitude of the gradient and store it in the array
            arr[j][i] = sqrt(Ix * Ix + Iy * Iy);
        }
    }

    for (int m = 0; m < image.height; ++m) {
        for (int l = 0; l < image.width; ++l) {
            if (arr[l][m] > 100) {
                // Set edge pixel to black
                image.setPixel(l, m, 0, 0);
                image.setPixel(l, m, 1, 0);
                image.setPixel(l, m, 2, 0);
            } else {
                // Set non-edge pixel to white
                image.setPixel(l, m, 0, 255);
                image.setPixel(l, m, 1, 255);
                image.setPixel(l, m, 2, 255);
            }
        }
    }
    return image;
}


// resize function for the merge filter
Image resizemin(Image& image1, Image& image2) {
    float new_width, new_height;
    // Calculate the new width and height as the minimum of the image dimensions
    new_width = min(image1.width, image2.width);
    new_height = min(image1.height, image2.height);
    double width, height;
    float resize, resize2;
    width = new_width;
    height = new_height;
    resize = (width - 1) / static_cast<float>(image1.width - 1);
    resize2 = (height - 1) / static_cast<float>(image1.height - 1);
    // Create a new image with the new dimensions
    Image resizedImage(width, height);
    // Iterate over each pixel in the resized image
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < image1.channels; k++) {
                int original_i = static_cast<int>(i / resize);
                int original_j = static_cast<int>(j / resize2);
                // Copy the pixel value from the original image if within bounds
                if (original_i < image1.width && original_j < image1.height) {
                    resizedImage(i, j, k) = image1(original_i, original_j, k);
                }
            }
        }
    }
    return resizedImage;
}
// resize function for the merge filter
Image resizemax(Image& image1, Image& image2) {
    float new_width, new_height;
    // Calculate the new width and height as the maximum of the image dimensions
    new_width = max(image1.width, image2.width);
    new_height = max(image1.height, image2.height);
    double width, height;
    float resize, resize2;
    width = new_width;
    height = new_height;
    resize = (width - 1) / static_cast<float>(image1.width - 1);
    resize2 = (height - 1) / static_cast<float>(image1.height - 1);
    // Create a new image with the new dimensions
    Image resizedImage(width, height);
    // Iterate over each pixel in the resized image
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < image1.channels; k++) {
                int original_i = static_cast<int>(i / resize);
                int original_j = static_cast<int>(j / resize2);
                if (original_i < image1.width && original_j < image1.height) {
                    // Copy the pixel value from the original image if within bounds
                    resizedImage(i, j, k) = image1(original_i, original_j, k);
                }
            }
        }
    }
    return resizedImage;
}


// Merge filter function
Image Merge(Image& image1, Image& image2) {
    // Check if the images have the same dimensions
    if (image1.width == image2.width && image1.height == image2.height) {  // If the images have the same size, merge them by taking the average pixel values
        Image mergedimage(image1.width, image1.height);
        for (int i = 0; i < image1.height; i++) {
            for (int j = 0; j < image1.width; j++) {
                unsigned char red = (image1(j, i, 0) + image2(j, i, 0)) / 2;
                unsigned char green = (image1(j, i, 1) + image2(j, i, 1)) / 2;
                unsigned char blue = (image1(j, i, 2) + image2(j, i, 2)) / 2;
                mergedimage.setPixel(j, i, 0, red);
                mergedimage.setPixel(j, i, 1, green);
                mergedimage.setPixel(j, i, 2, blue);
            }
        }
        return mergedimage;
        // If the images have different dimensions, ask the user to choose a method
    } else if (image1.width != image2.width && image1.height != image2.height) { // If the images have different dimensions, the user will choose whether to resize or crop then merge
        cout << "The images are of different sizes, please select a choice\n";
        char choice;
        while (true) {
            cout << "A- Resize both images to the maximum or minimum dimensions\nB- Merge the common area between the two images\n";
            cin >> choice;
            choice = toupper(choice);
            if (choice == 'A') {
                break;
            } else if (choice == 'B') {
                break;
            } else {
                cout << "Please select a valid choice\n";
            }
        }
        if (choice == 'A') { // If the user chooses to resize both images, ask for another choice
            char choice2;
            while (true) {
                cout << "Choose whether to resize both images to the maximum or minimum dimensions\nA- Maximum dimensions\nB- Minimum dimensions\n";
                cin >> choice2;
                choice2 = toupper(choice2);
                if (choice2 == 'A') {
                    break;
                } else if (choice2 == 'B') {
                    break;
                } else {
                    cout << "Please select a valid choice\n";
                }
            }
            if (choice2 == 'A'){ // If the user chooses to resize to maximum dimensions, resize both images and then merge
                float width = max(image1.width, image2.width), height = max(image1.height, image2.height); // Set the new width and height to the max width and height
                Image resized_image1, resized_image2;
                resized_image1 = resizemax(image1, image2);
                resized_image2 = resizemax(image2, image1);
                Image mergedimage(width, height);
                for (int i = 0; i < width; i++) {
                    for (int j = 0; j < height; j++) {
                        unsigned char red = (resized_image1(i, j, 0) + resized_image2(i, j, 0)) / 2;
                        unsigned char green = (resized_image1(i, j, 1) + resized_image2(i, j, 1)) / 2;
                        unsigned char blue = (resized_image1(i, j, 2) + resized_image2(i, j, 2)) / 2;
                        mergedimage.setPixel(i, j, 0, red);
                        mergedimage.setPixel(i, j, 1, green);
                        mergedimage.setPixel(i, j, 2, blue);
                    }
                }
                return mergedimage;
            }
            else if (choice2 == 'B'){ // If the user chooses to resize to minimum dimensions, resize both images and then merge
                float width,height;
                width = min(image1.width,image2.width);
                height = min(image1.height,image2.height);
                Image resized_image1, resized_image2;
                resized_image1 = resizemin(image1,image2);
                resized_image2 = resizemin(image2,image1);
                Image mergedimage(min(image1.width,image2.width),min(image1.height,image2.height));
                for (int i = 0; i < width; i++) {
                    for (int j = 0; j < height; j++) {
                        unsigned char red = (resized_image1(i, j, 0) + resized_image2(i, j, 0)) / 2;
                        unsigned char green = (resized_image1(i, j, 1) + resized_image2(i, j, 1)) / 2;
                        unsigned char blue = (resized_image1(i, j, 2) + resized_image2(i, j, 2)) / 2;
                        mergedimage.setPixel(i, j, 0, red);
                        mergedimage.setPixel(i, j, 1, green);
                        mergedimage.setPixel(i, j, 2, blue);
                    }
                }
                return mergedimage;
            }
        }
        else if (choice == 'B') { // The user chose to merge the common area between the two images
            float width = min(image1.width,image2.width), height = min(image1.height,image2.height);
            Image mergedimage(min(image1.width,image2.width),min(image1.height,image2.height));
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    unsigned char red = (image1(j, i, 0) + image2(j, i, 0)) / 2;
                    unsigned char green = (image1(j, i, 1) + image2(j, i, 1)) / 2;
                    unsigned char blue = (image1(j, i, 2) + image2(j, i, 2)) / 2;
                    mergedimage.setPixel(j, i, 0, red);
                    mergedimage.setPixel(j, i, 1, green);
                    mergedimage.setPixel(j, i, 2, blue);
                }
            }
            return mergedimage;
        }
    }
}


// Gray Scale filter function
Image GrayScale (Image& image){
    cout << "Applying GrayScale Conversion filter...\nPlease be patient.\n";
    for(int i = 0; i < image.width; ++i){ // iterates over the width of the image
        for(int j = 0; j < image.height; ++j){ // iterates over the height of the image
            unsigned int avg = 0;
            for(int k = 0; k < 3; ++k){ // iterates over the three color channels, Red, Green and Blue
                avg += image(i,j,k);
            }
            avg /= 3; // getting the average value of the RGB value of the pixel then assign the calculated averaged value to all three color channels to convert it into grayscale
            image(i,j,0) = avg;
            image(i,j,1) = avg;
            image(i,j,2) = avg;
        }
    }
    cout << "GrayScale Conversion filter applied successfully.\n";
    return image;
}

// Darken and lighten image filter function
Image darken_lighten_image(Image& image) {

    bool operationCompleted = false; // Track whether operations are completed
    while (!operationCompleted) {
        char choice;
        cout << "Select whether you want to lighten or darken your image\nA- Lighten\nB- Darken\n";
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'B') { // User chose to darken the image
            cout << "Applying Darken And Lighten filter...\nPlease be patient.\n";
            for (int i = 0; i < image.width; ++i) { // iterates over the width of the image
                for (int j = 0; j < image.height; ++j) { // iterates over the height of the image
                    for (int k = 0;
                         k < image.channels; ++k) { // iterates over the three color channels, Red, Green and Blue
                        if (image(i, j, k) == 255) { // If the RGB value equals 255 then it remains the same
                            continue;
                        } else {
                            image(i, j, k) *= 0.5; // We decrease its brightness by multiplying it by 0.5
                        }
                    }
                }
            }operationCompleted = true;
        } else if (choice == 'A') { // User chose to lighten the image
            cout << "Applying Darken And Lighten filter...\nPlease be patient.\n";
            for (int i = 0; i < image.width; ++i) { // iterates over the width of the image
                for (int j = 0; j < image.height; ++j) { // iterates over the height of the image
                    for (int k = 0;
                         k < image.channels; ++k) { // iterates over the three color channels, Red, Green and Blue
                        unsigned int x = image(i, j, k);
                        image(i, j, k) = min(x * 1.5,255.0); // We increase its brightness by multiplying it by 1.5 and make sure that the RGB value doesn't exceed 255
                    }
                }
            }operationCompleted = true;
        }
        else {
            cout << "Invalid choice. Please select A or B.\n";
            continue; // Restart the loop if choice is invalid
        }
    }
    cout << "Darken and Lighten filter applied successfully.\n";
    return image;
}

// Invert filter function
Image inverting(Image& image) {
    cout << "Applying Invert Images filter...\nPlease be patient.\n";
    // Invert colors
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                // Invert each color channel
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
    cout << "Invert Images filter applied successfully.\n";
    return image;
}

Image black_white(Image& image) {
    cout << "Applying Black And White filter...\nPlease be patient.\n";
    // Iterate through each pixel
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            // Calculate average intensity of RGB channels
            int avg_intensity = 0;
            for (int k = 0; k < image.channels; k++) {
                avg_intensity += image(i, j, k);
            }
            avg_intensity /= image.channels;

            // Apply thresholding to enhance contrast
            int threshold = 128; // Adjust this value as needed
            for (int k = 0; k < image.channels; k++) {
                if (avg_intensity < threshold) {
                    image(i, j, k) = 0; // Set pixel to black
                } else {
                    image(i, j, k) = 255; // Set pixel to white
                }
            }
        }
    }
    cout << "Black and White filter applied successfully.\n";
    return image;
}

Image crop(Image& image) {
    // Get the dimensions of the original image
    float width = image.width;
    float height = image.height;
    cout << "Width = " << width << endl;
    cout << "Height = " << height << endl;

    // Prompt the user to enter the starting coordinates for cropping
    cout<<"Enter the width and height to start cropping from\n";
    float width_1,height_1;
    while (true){
        cin >> width_1>>height_1;
        if (!cin){
            cout << "Invalid input. Enter only integers.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }else if (width_1 <0 || height_1<0){
            cout<<"Invalid input. Enter only positive integers.\n";
        } else if (width_1> width || height_1> height){
            cout<<"Invalid input. Enter a width less than "<< "[" << width << "]" << "and a height less than " << "[" << height << "]" <<"\n";
        } else {break;
        }
    }
    // Prompt the user to enter the ending coordinates for cropping
    cout << "Enter the width and height to stop cropping\n";
    float width_2,height_2;
    while (true){
        cin >>width_2>>height_2;
        if (!cin){
            cout << "Inavlid input. Enter only integers.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }else if (width_2 <0 || height_2<0){
            cout<<"Invalid input. Enter only positive integers.\n";
        } else if (width_2> width || height_2> height){
            cout<<"Invalid input. Enter a width less than "<< "[" << width << "]" << "and a height less than " << "[" << height << "]" <<"\n";
        } else {break;}
    }
    cout << "Applying Crop Images filter...\nPlease be patient.\n";

    // Calculate the new dimensions of the cropped image
    float new_width,new_height;
    new_width=width_2-width_1;
    new_height = height_2-height_1;
    Image image2 (new_width, new_height);

    // Copy the pixels from the original image to the cropped image
    for (int i=width_1; i<width_2; i++ ){
        for (int j=height_1; j<height_2; j++){
            for (int k=0; k<image.channels; k++){
                image2(i-width_1,j-height_1,k)=image(i,j,k);
            }
        }
    }
    cout << "Crop Images filter applied successfully.\n";
    // Return the cropped image
    return image2;
}

Image flip_width(Image& image) {
    // Display message indicating that the flip by width filter is being applied.
    cout << "Applying Flip Images filter...\nPlease be patient.\n";
    cout << "[Flip By Width]\n";

    // Create a new image to store the flipped version
    Image image2;
    image2 = image;

    // Iterate over each pixel in the original image.
    for (int i=0; i<image.width; i++ ) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < image.channels; k++) {
                // Set the pixel in the new image to the corresponding pixel from the flipped position.
                image2(i, j, k) = image(image.width - 1 - i, j, k);


            }
        }
    }
    cout << "Flip By Width filter applied successfully.\n";
    return image2;
}

Image flip_height(Image& image) {
    // Display message indicating that the flip by height filter is being applied
    cout << "Applying Flip Images filter...\nPlease be patient.\n";
    cout << "[Flip By Height]\n";
    // Create a new image to store the flipped version
    Image image2;
    image2 = image;

    // Iterate over each pixel in the original image
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < image.channels; k++) {
                // Set the pixel in the new image to the corresponding pixel from the flipped position.
                image2(i, j, k) = image(i, image.height-1 - j, k);
            }
        }
    }
    cout << "Flip by  filter applied successfully.\n";
    return image2;
}


//Resize Function.
Image resize( Image& image) {

    cout << "The Original Width: " << image.width << endl;
    cout << "The Original Height: " << image.height << endl;
    // Ask the user to enter the new dimensions
    cout << "Please enter the width and the height of the new size.\n";
    float new_width, new_height;
    while(true){
        cin >> new_width>>new_height;
        if(!cin){
            cout << "Invalid input. Enter only integers.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }else if (new_width<0 || new_height<0){
            cout<<"Invalid input. Enter only positive integers.\n";
        }
        else {break;}
    }

    // Take new dimensions to the image.
    double width, height;

    // Calculate the resize ratios for width and height
    float resize, resize2;
    width = new_width ;
    height = new_height ;
    resize = static_cast<float>(image.width - 1) / (width - 1); //calculate the ratio for width.
    resize2 = static_cast<float>(image.height - 1) / (height - 1);   //calculate the ratio for height.

    // Create a new image with the new dimensions
    Image image2(width, height);
    cout << "Applying Resizing Images filter...\nPlease be patient.\n";
    // Iterate through each pixel of the original image and copy it to the resized image
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < image.channels; k++) {
                // Copy each pixel from the original image to the resized image
                image2(i / resize, j / resize2, k) = image(i, j, k);
            }
        }
    }
    cout << "Resizing Images applied successfully.\n";
    return image2;
}

Image resize_by_ratio(Image& image){

    cout << "The Original Width: " << image.width << endl;
    cout << "The Original Height: " << image.height << endl;

    // Ask the user to enter the resize ratio
    float ratio;
    cout << "Please enter the resize ratio: ";
    while (true){
        cin >> ratio;
        if(!cin){
            cout << "Invalid input. Enter only integers.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }else if (ratio <0){
            cout<<"Invalid input. Enter only positive integers.\n";
        }
        else {break;}
    }

    // Calculate the new dimensions
    int new_width = static_cast<int>(image.width * ratio);  //calculate the ratio for width.
    int new_height = static_cast<int>(image.height * ratio); //calculate the ratio for height.

    // Create a new image with the new dimensions
    Image resized_image(new_width, new_height);

    cout << "Applying Resizing Images filter...\nPlease be patient.\n";

    // Iterate through each pixel of the resized image
    for (int i = 0; i < new_width; i++) {
        for (int j = 0; j < new_height; j++) {
            for (int k = 0; k < image.channels; k++) {
                // Copy each pixel from the original image to the resized image
                resized_image(i, j, k) = image((i / ratio), (j / ratio), k);
            }
        }
    } return resized_image;
}


// Function to add a simple frame surrounding the image
void addSimpleFrame(Image& image, int borderWidth, const int frameColor[3]) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            // Check if pixel is on the border
            if (i < borderWidth || i >= image.width - borderWidth || j < borderWidth || j >= image.height - borderWidth) {
                // Set border color
                image(i, j, 0) = frameColor[0];
                image(i, j, 1) = frameColor[1];
                image(i, j, 2) = frameColor[2];
            }
        }
    }
}

// Function to add a fancy frame surrounded by a brown frame
void addFancyFrame(Image& image, int borderWidth, const int frameColor[3], const int brownColor[3]) {
    int halfBorderWidth = borderWidth / 2;
    // Fill the image with brown color as the background frame
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            if (i < borderWidth || i >= image.width - borderWidth || j < borderWidth || j >= image.height - borderWidth) {
                image(i, j, 0) = brownColor[0];
                image(i, j, 1) = brownColor[1];
                image(i, j, 2) = brownColor[2];
            }
        }
    }
    // Add the fancy frame
    for (int i = halfBorderWidth; i < image.width - halfBorderWidth; ++i) {
        for (int j = halfBorderWidth; j < image.height - halfBorderWidth; ++j) {
            // Check if pixel is within the fancy frame area
            if ((i >= halfBorderWidth && i < image.width - halfBorderWidth && j >= halfBorderWidth && j < image.height - halfBorderWidth) &&
                ((i < halfBorderWidth * 2 || i >= image.width - halfBorderWidth * 2) || (j < halfBorderWidth * 2 || j >= image.height - halfBorderWidth * 2))) {
                // Set border color
                image(i, j, 0) = frameColor[0];
                image(i, j, 1) = frameColor[1];
                image(i, j, 2) = frameColor[2];
            }
        }
    }
}

// Function to prompt the user for input and apply frame accordingly
Image applyFrame(Image& image) {
    // Image image(filename);

    int frameType;
    cout << "Choose frame type:\n";
    cout << "1. Simple frame\n";
    cout << "2. Fancy frame\n";
    cout << "Enter your choice: \n";
    while (!(cin >> frameType) || frameType < 1 || frameType > 2) {
        cout << "Invalid choice.\nPlease enter 1 for Simple frame or 2 for Fancy frame: \n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int borderWidth;
    cout << "Please enter the width of the frame: \n";
    while (!(cin >> borderWidth) || borderWidth < 0) {
        cout << "Invalid input. Please enter a non-negative integer for the width: \n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int frameColor[3];
    int brownColor[3] = { 139, 69, 19 }; // Brown color (RGB values)
    cout << "Please enter the RGB values for the frame color (separated by spaces): \n";
    while (!(cin >> frameColor[0] >> frameColor[1] >> frameColor[2])) {
        cout << "Invalid input. Please enter three integers separated by spaces: \n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (frameType == 1) {
        cout << "Applying \"Adding a Frame to Images\" filter...\nPlease be patient.\n";
        cout << "[Simple Frame]\n";
        addSimpleFrame(image, borderWidth, frameColor);
        cout << "Simple Frame filter applied successfully.\n";
    }
    else if (frameType == 2) {
        cout << "Applying \"Adding a Frame to Images\" filter...\nPlease be patient.\n";
        cout << "[Fancy Frame]\n";
        addFancyFrame(image, borderWidth, frameColor, brownColor);
        cout << "Fancy Frame filter applied successfully.\n";
    }
    else {
        cout << "Invalid frame type selected.\n";
    }
    return image;
}


void save (Image& image, string& filename){
    string new_name;
    // Prompt the user to enter a valid name for the resized image with the correct extension
    regex pattern("^.*\\.(png|jpg|jpeg|bmp)$");
    cout << "Do you want to save it in the same file?\n";
    string answer;
    while (true){
        cin>> answer;
        // Convert the answer to uppercase
        for (int i = 0; i < answer.length(); ++i) {
            answer[i] = toupper(answer[i]);
        }

        string new_name;
        // Check user's answer
        if (answer=="NO"){
            // Prompt the user to enter a valid name for the new image with the correct extension
            regex pattern("^.*\\.(png|jpg|jpeg|bmp)$");
            while (true){
                cout << "Please enter the new name of the image to save it: \n";
                cin.ignore();
                getline (cin, new_name);
                // Validate the input using regex pattern
                if (regex_match(new_name, pattern)) {
                    break;
                } else {
                    cout << "Invalid input.\n" << endl;
                }}
            // Save the resized image
            image.saveImage(new_name);
            cout << "Image saved successfully as " << "\"" << new_name << "\"" << endl ;
            // Open the saved image using the default image viewer
            system(new_name.c_str());
            break;

        } if (answer == "YES"){
            image.saveImage(filename);
            cout << "Image saved successfully as " << "\"" << filename << "\"" << endl ;
            // Open the saved image using the default image viewer
            string command = "start \"\" \"" + filename + "\"";
            system(command.c_str());
            break;}
        else {
            cout << "Please enter only \"YES\" or \"NO\".\n";
        }
    }
}


Image Load_New_Image (string& filename){
    // Regular expression pattern to validate file extensions
    regex pattern("^.*\\.(png|jpg|jpeg|bmp)$");

    // Prompt the user to enter the file name or path of the new image
    while (true) {
        cout << "Please enter the file name or the path of the new image: \n";
        // cin.ignore();
        getline(cin, filename);
        // Check if the input matches the expected file extension pattern
        if (regex_match(filename, pattern)) {
            break;  // Break the loop if the input is valid
        } else {
            cout << "Invalid input, try again" << endl; }// Display error message for invalid input
    }
    cout << "Image loaded successfully.\n";
    // Create a new Image object with the specified filename
    Image image2 (filename);
    return image2;
}


Image Red_Hue(Image& image) {
    cout << "Applying Red Hue Images filter...\nPlease be patient.\n";
    // Adjust the colors to create a red hue effect
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                // Invert each color channel
                image(i, j, k) = 255 - image(i, j, k);
                // If it's the red channel, adjust it to a specific value to enhance red hue
                if (k==0){
                    image(i,j,k)= 227;
                }
            }
        }
    }
    cout << "Red Hue Images filter applied successfully.\n";
    return image;
}


Image Purple_Hue(Image& image){
    cout << "Applying Purple Hue Images filter...\nPlease be patient.\n";
    // Adjust the colors to create a Purple hue effect
    for(int i = 0 ; i < image.width ; i++){
        for(int j = 0 ; j < image.height ; j++) {
            for (int k = 0; k < image.channels; ++k) {
                image(i, j, k) *= 0.95;
            } int nred, nblue, ngreen;
            // Calculate new color values
            //red value.
            nred = image(i, j, 0) + 40;
            if  (nred > 255) { nred = 255; }
            image(i, j, 0) = nred;

            //green value.
            ngreen = image(i, j, 1) - 40;
            if (ngreen < 0) { ngreen = 0; }
            image(i, j, 1) = ngreen;

            //blue value.
            nblue = image(i, j, 2) + 20;
            if  (nblue > 255) { nblue = 255; }
            image(i, j, 2) = nblue;

        }    }
    cout << "Purple Hue Images filter applied successfully.\n";
    return image;
}


Image sunlightFilter(Image& image) {

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            int red = min(255,static_cast<int>(image(i, j, 0) * 1.2)); // Increase red slightly
            int green = min(255,static_cast<int>(image(i, j, 1) * 1.2)); // Increase green slightly
            int blue =min(255, static_cast<int>(image(i, j, 2) * 0.8)); // Decrease blue slightly

            // Set the adjusted values back to the image
            image(i, j, 0) = red;
            image(i, j, 1) = green;
            image(i, j, 2) = blue;
        }
    }
    return image;
}


Image rotateImage(Image& image) {
    int degrees;
    bool validInput = false;

    while (!validInput) {
        cout << "Please enter the rotation angle (90, 180, or 270 degrees): ";
        cin >> degrees;

        if (degrees == 90 || degrees == 180 || degrees == 270) {
            validInput = true;
        }
        else {
            cout << "Invalid rotation angle. Please enter 90, 180, or 270 degrees." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    }

    int width = image.width;
    int height = image.height;

    if (degrees == 90 || degrees == 270) {
        // Create a new image with the swapped dimensions
        Image rotatedImage(height, width);

        // Rotate pixels based on the specified angle
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int new_i, new_j;

                // Calculate new pixel position based on rotation angle
                if (degrees == 90) {
                    new_j = i;
                    new_i = height - 1 - j;
                }
                else { // 270 degrees
                    new_j = width - 1 - i;
                    new_i = j;
                }

                // Copy pixel values to the rotated image
                for (int k = 0; k < image.channels; ++k) {
                    rotatedImage(new_i, new_j, k) = image(i, j, k);
                }
            }
        }
        return rotatedImage;
    }
    else if (degrees == 180) {
        // Create a new image with the same dimensions as the original
        Image rotatedImage(width, height);

        // Rotate pixels based on the specified angle
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                int new_i = width - 1 - i;
                int new_j = height - 1 - j;

                // Copy pixel values to the rotated image
                for (int k = 0; k < image.channels; ++k) {
                    rotatedImage(new_i, new_j, k) = image(i, j, k);
                }
            }
        }
        return rotatedImage;
    }
    else {
        cout << "Invalid rotation angle\n";
        // Return the original image if the angle is invalid
        return image;
    }
}

Image gaussianBlur(Image& image) {

    float sigma;
    bool validInput = false;

    while (!validInput) {
        cout << "Please enter the sigma value for Gaussian blur: ";
        cin >> sigma;

        if (sigma > 0) {
            validInput = true;
        }
        else {
            cout << "Invalid sigma value. Please enter a positive number." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }
    }
    int kernelSize = static_cast<int>(6 * sigma) + 1; // Calculate kernel size based on sigma
    float* kernel = new float[kernelSize];

    // Calculate Gaussian kernel weights
    float sum = 0.0;
    for (int i = 0; i < kernelSize; ++i) {
        float x = i - kernelSize / 2;
        kernel[i] = exp(-(x * x) / (2 * sigma * sigma));
        sum += kernel[i];
    }

    // Normalize the kernel
    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] /= sum;
    }

    // Apply horizontal blur
    for (int y = 0; y < image.height; ++y) {
        for (int x = kernelSize / 2; x < image.width - kernelSize / 2; ++x) {
            float sum[3] = { 0, 0, 0 };
            for (int i = 0; i < kernelSize; ++i) {
                for (int c = 0; c < image.channels; ++c) {
                    sum[c] += image(x + i - kernelSize / 2, y, c) * kernel[i];
                }
            }
            for (int c = 0; c < image.channels; ++c) {
                image(x, y, c) = sum[c];
            }
        }
    }

    // Apply vertical blur
    for (int x = 0; x < image.width; ++x) {
        for (int y = kernelSize / 2; y < image.height - kernelSize / 2; ++y) {
            float sum[3] = { 0, 0, 0 };
            for (int i = 0; i < kernelSize; ++i) {
                for (int c = 0; c < image.channels; ++c) {
                    sum[c] += image(x, y + i - kernelSize / 2, c) * kernel[i];
                }
            }
            for (int c = 0; c < image.channels; ++c) {
                image(x, y, c) = sum[c];
            }
        }
    }

    delete[] kernel;
    return image;
}

int main () {
    cout << "Welcome to the Image Filter Application!^^\n";
    string filename;
    regex pattern("^.*\\.(png|jpg|jpeg|bmp)$");
    while (true) {
        cout << "Please enter the file name or path of the image you want to process: \n";
        getline(cin, filename);
        if (regex_match(filename, pattern)) {
            break;
        } else {
            cout << "Error: Invalid input, try again" << endl;
        }
    }
    Image image(filename);
    while (true) {
        cout << "Select the filter you want to apply to your image\n";
        int filter;
        bool x = false;
        cout
                << "1) Load a new image.\n2) Grayscale Conversion.\n3) Black and White.\n4) Invert Image.\n5) Merge Image.\n6) Flip Image.\n7) Rotate Image.\n8) Darken and Lighten Image."
                << "\n9) Crop Images.\n10) Adding a Frame to the Picture.\n11) Detect Image Edges.\n12) Resizing Images.\n13) Blur Images."
                << "\n14) Purple Hue.\n15) Red Hue.\n16) Sunlight.\n17) Save.\n18) Exit.\n";
        if (cin >> filter) {
            switch (filter) {
                case 1 :{
                    cout << "Do you want to save the applied filters on image before you load a new one?\n";
                    string ans;
                    while (true){
                        cin >> ans;
                        for (int i =0 ; i< ans.length(); i++){
                            ans[i]=toupper(ans[i]);
                        }
                        if (ans =="YES"){
                            save(image,filename);
                            break;
                        }
                        else if (ans == "NO"){
                            cout << "Image doesn't saved\n";
                            break;
                        }
                        else {cout << "Please enter a valid word.\n";
                        }
                    }
                    image = Load_New_Image(filename);
                    break;}
                case 2 :
                    image = GrayScale(image);
                    break;
                case 3 :
                    image = black_white(image);
                    break;
                case 4 :
                    image =inverting(image);
                    break;
                case 5 :{
                    string filename2;
                    regex pattern("^.*\\.(png|jpg|jpeg|bmp)$");
                    while (true) {
                        cout << "Please enter the second file name or path of the image you want to process: \n";
                        cin.ignore();
                        getline(cin, filename2);
                        if (regex_match(filename2, pattern)) {
                            break;
                        } else {
                            cout << "Error: Invalid input, try again" << endl;
                        }
                    }
                    Image image2(filename2);
                    image = Merge(image,image2);
                    break;}
                case 6 :
                {string ans;
                    while (true){
                        cout << "Do you want flip by width or height? \n";
                        cin >> ans;
                        for (int i =0 ; i< ans.length(); i++){
                            ans[i]=toupper(ans[i]);
                        }
                        if (ans =="WIDTH"){
                            image = flip_width(image);
                            break;
                        }
                        else if (ans == "HEIGHT"){
                            image = flip_height(image);
                            break;
                        }
                        else {cout << "Please enter a valid word.\n";
                        }
                    }
                    break; }
                case 7 :
                    image = rotateImage(image);
                    break;
                case 8 :
                    image = darken_lighten_image(image);
                    break;
                case 9 :
                    image = crop(image);
                    break;
                case 10 :
                    image = applyFrame(image);
                    break;
                case 11 :
                    image = DetectEdges(image);
                    break;
                case 12:
                {
                    string choice;
                    cout << "How you will resize the image?\nA) By Ratio.\nB) By Value.\n";
                    while (true){
                        cin >> choice;
                        if (choice == "A" || choice == "a"){
                            image = resize_by_ratio(image);
                            break;
                        }else if (choice == "B" || choice == "b"){
                            image = resize(image);
                            break;
                        }else {
                            cout << "Error: Invalid Input.\n";
                        }
                    }
                    break;}
                case 13 :
                    image = gaussianBlur(image);
                    break;
                case 14 :
                    image = Purple_Hue(image);
                    break;
                case 15 :
                    image = Red_Hue(image);
                    break;
                case 16 :
                    image = sunlightFilter(image);
                    break;
                case 17 :
                    save(image, filename);
                    break;
                case 18 :{
                    x = true;
                    cout << "Do you want to save the applied filters on image before you exit?\n";
                    string ans;
                    while (true){
                        cin >> ans;
                        for (int i =0 ; i< ans.length(); i++){
                            ans[i]=toupper(ans[i]);
                        }
                        if (ans =="YES"){
                            save(image,filename);
                            break;
                        }
                        else if (ans == "NO"){
                            cout << "Image doesn't saved\n";
                            break;
                        }
                        else {cout << "Please enter a valid word.\n";
                        }
                    }
                    cout << "Thank you for using the Image Filter Application. Goodbye!\n";
                    break;}
                default:
                    cout << "Please enter a valid choice.\n";
            }
            if (x == true) {
                break;
            }
        } else {
            cout << "Invalid input. Please enter a valid choice.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return 0;
}
