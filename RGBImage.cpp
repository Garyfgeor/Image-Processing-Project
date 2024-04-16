#include <iostream>
#include <fstream>
#include "RGBImage.hpp"
#include "RGBPixel.hpp"
#include "Image.hpp"
#include "GSCImage.hpp"
#include "GSCPixel.hpp"

RGBImage::RGBImage(){}

//copy constructor
RGBImage::RGBImage(const RGBImage& img){
    this->height = img.height;
    this->width = img.width;
    this->max_luminocity = img.max_luminocity;
    
    this->rgb_array = new RGBPixel*[height];
    for (int i = 0; i < height; ++i) {
        this->rgb_array[i] = new RGBPixel[width];
    }
    
    RGBPixel** array = img.rgb_array;
    int red, green, blue;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            red = (int)array[i][j].getRed();
            green = (int)array[i][j].getGreen();
            blue = (int)array[i][j].getBlue();
            this->rgb_array[i][j].setRed(red);
            this->rgb_array[i][j].setGreen(green);
            this->rgb_array[i][j].setBlue(blue);
        }
    }
}

RGBPixel** RGBImage::get_rgb_array() const{
    return rgb_array;
}

//constructor
RGBImage::RGBImage(istream& stream){
    int width, height, luminocity;
    stream >> width >> height >> luminocity;
    setWidth(width);
    setHeight(height);
    setMaxLuminocity(luminocity);
  
    this->rgb_array = new RGBPixel*[height];
    for (int i = 0; i < height; ++i) {
        this->rgb_array[i] = new RGBPixel[width];
    }

    int red, green, blue;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            stream >> red >> green >> blue;
            this->rgb_array[i][j].setRed(red);
            this->rgb_array[i][j].setGreen(green);
            this->rgb_array[i][j].setBlue(blue);
        }
    }
}

ostream& operator << (ostream& out, Image& image){
    int height = image.getHeight();
    int width = image.getWidth();
    int max_lum = image.getMaxLuminocity();

    if(dynamic_cast<RGBImage*>(&image) != nullptr){//egxrwmh
        RGBImage* rgb_image = dynamic_cast<RGBImage*>(&image);
        RGBPixel** array = rgb_image->get_rgb_array();
        out << "P3" << endl;
        out << width << " " << height << endl;
        out << max_lum << endl;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                out <<  (int)array[i][j].getRed() << " " <<  (int)array[i][j].getGreen() << " " <<  (int)array[i][j].getBlue() << endl;
            }
        }
    }
    else{
        GSCImage* gsc_image = dynamic_cast<GSCImage*>(&image);
        out << "P2" << endl;
        out << width << " " << height << endl;
        out << max_lum << endl;
        GSCPixel** array = gsc_image->get_gsc_array();
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                out <<  (int)array[i][j].getValue() << " ";
            }
            out<< endl;
        }
    }
    return out;
}

//destructor
RGBImage::~RGBImage(){
    free_array(rgb_array, this->getHeight());
}

//reverse colors of image
Image& RGBImage::operator!(){
    int height = this->getHeight();
    int width = this->getWidth();
    int max_lum = this->getMaxLuminocity();

    RGBImage* rgb_image = dynamic_cast<RGBImage*>(this);
    RGBPixel** array = rgb_image->get_rgb_array();

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            int red_val = (int)array[i][j].getRed();
            int green_val = (int)array[i][j].getGreen();
            int blue_val = (int)array[i][j].getBlue();
            array[i][j].setRed(max_lum - red_val); 
            array[i][j].setGreen(max_lum - green_val); 
            array[i][j].setBlue(max_lum - blue_val);
        }
    }
    return *this;
}

//mirror image
Image& RGBImage::operator*(){
    int height = this->getHeight();
    int width = this->getWidth();

    RGBPixel** mirrored_array = new RGBPixel*[height];
    for (int i = 0; i < height; ++i) {
        mirrored_array[i] = new RGBPixel[width];
    }
    RGBPixel** array = this->get_rgb_array();

    //diatrexw ton pinaka stili-stili
    for(int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            mirrored_array[i][width-1-j] = array[i][j];
        }
    }
    free_array(array, height);
    this->rgb_array = mirrored_array;
    return *this;
}

//rotate image times
Image& RGBImage::operator+=(int times){
    int height = this->getHeight();
    int width = this->getWidth();
    RGBPixel* line = new RGBPixel[width];
    RGBPixel** rotated_array;
    //RGBImage* rgb_image;
    RGBPixel** array;
    int rot_height;
    int rot_width; 
    int res;
    if(times<0){
        res = -times % 4;
    } 
    else{
        res = times % 4;
    }
    
    if(res == 1){
        rot_height = width;
        rot_width = height;
        rotated_array = new RGBPixel*[rot_height];
        for (int i = 0; i < rot_height; ++i) {
            rotated_array[i] = new RGBPixel[rot_width];
        }

        array = this->get_rgb_array();
        int jj=0;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                line[j] = array[i][j];
            }
            for(int ii=0; ii<rot_height; ii++){
                if(times>0){//deksiostrofa
                    rotated_array[ii][rot_width-1-jj] = line[ii];
                }
                else{//aristerostrofa
                    rotated_array[ii][jj] = line[rot_height-1-ii];
                }
            }
            jj++;
        }

        free_array(rgb_array, height);
        this->rgb_array = rotated_array;
        this->setHeight(rot_height);
        this->setWidth(rot_width);
    }
    else if(res == 2){
        rot_height = height;
        rot_width = width;
        rotated_array = new RGBPixel*[rot_height];
        for (int i = 0; i < rot_height; ++i) {
            rotated_array[i] = new RGBPixel[rot_width];
        }

        array = this->get_rgb_array();
        int ii=0;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                line[j] = array[i][j];
            }
            for(int jj=0; jj<rot_width; jj++){
                rotated_array[rot_height-1-ii][jj] = line[rot_width-1-jj];
            }
            ii++;
        }

        free_array(rgb_array, height);
        this->rgb_array = rotated_array;
        this->setHeight(rot_height);
        this->setWidth(rot_width);

    }
    else if(res == 3){
        rot_height = width;
        rot_width = height;
        rotated_array = new RGBPixel*[rot_height];
        for (int i = 0; i < rot_height; ++i) {
            rotated_array[i] = new RGBPixel[rot_width];
        }

        array = this->get_rgb_array();
        int jj=0;
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                line[j] = array[i][j];
            }
            for(int ii=0; ii<rot_height; ii++){
                if(times>0){//deksiostrofa
                    rotated_array[ii][jj] = line[rot_height-1-ii];
                }
                else{//aristerostrofa
                    rotated_array[ii][rot_width-1-jj] = line[ii];
                }
            }
            jj++;
        }
        free_array(rgb_array, height);
        this->rgb_array = rotated_array;
        this->setHeight(rot_height);
        this->setWidth(rot_width);
        
    }
    delete[] line;
    return *this;
}
  
//change image size by factor
Image& RGBImage::operator *= (double factor){
    int height = this->getHeight();
    int width = this->getWidth();
    int resized_height = factor*height;
    int resized_width = factor*width;

    RGBPixel** resized_array = new RGBPixel*[resized_height];
    for (int i = 0; i < resized_height; i++){
        resized_array[i] = new RGBPixel[resized_width];
    }
    
    int r1, r2, c1, c2, new_red, new_green, new_blue;
    for(int j=0; j<resized_width; j++){
        for(int i=0; i<resized_height; i++){
            r1 = min((int)floor(i/factor), height-1);
            r2 = min((int)ceil(i/factor), height-1);
            c1 = min((int)floor(j/factor), width-1);
            c2 = min((int)ceil(j/factor), width-1);

            Pixel& pixel = this->getPixel(r1,c1);
            RGBPixel& P11 = static_cast<RGBPixel&>(pixel);
            pixel = this->getPixel(r1, c2);
            RGBPixel& P12 = static_cast<RGBPixel&>(pixel);
            pixel = this->getPixel(r2, c1);
            RGBPixel& P21 = static_cast<RGBPixel&>(pixel);
            pixel = this->getPixel(r2, c2);
            RGBPixel& P22 = static_cast<RGBPixel&>(pixel);
        
            new_red = (P11.getRed() + P12.getRed() + P21.getRed() + P22.getRed())/4;
            new_green = (P11.getGreen() + P12.getGreen() + P21.getGreen() + P22.getGreen())/4;
            new_blue = (P11.getBlue() + P12.getBlue() + P21.getBlue() + P22.getGreen())/4;

            resized_array[i][j].setRed(new_red);
            resized_array[i][j].setGreen(new_green);
            resized_array[i][j].setBlue(new_blue);
        }
    }
    this->setHeight(resized_height);
    this->setWidth(resized_width);
    free_array(rgb_array, height);
    this->rgb_array = resized_array;
        
    return *this;
}

int RGBImage::clip(int num){
    if(num>255){
        num = 255;
    }
    else if(num<0){
        num = 0;
    }
    return num; 
}

//equalize histogram
Image& RGBImage::operator ~(){
    int histogram_array[236] = {0};
    int width = this->getWidth();
    int height = this->getHeight();
    double probability_distribution[236] = {0};
    double cumulative_probability_distribution[236] = {0};
    int num_of_pixels = width*height;

    int R, G, B;
    unsigned char Y, U, V;
    int C, D, E;

    RGBPixel** array = this->get_rgb_array();
    //STEP 1
    //rgb to yuv
    int y[height][width] ={{0}};
    int u[height][width] ={{0}};
    int v[height][width] ={{0}};
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            R = (int)array[i][j].getRed();
            G = (int)array[i][j].getGreen();
            B = (int)array[i][j].getBlue();

            Y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
            U = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
            V = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;
            
            y[i][j]=Y;
            u[i][j]=U;
            v[i][j]=V;
            histogram_array[Y] = histogram_array[Y] + 1;
        }
    }
    //STEP 2
    for(int i=0; i<236; i++){
        probability_distribution[i] = (double)histogram_array[i]/num_of_pixels;
    }

    //STEP 3
    cumulative_probability_distribution[0] = probability_distribution[0];
    for(int i=1; i<236; i++){
        cumulative_probability_distribution[i] = probability_distribution[i] + cumulative_probability_distribution[i-1];
    }

    //STEP 4-5
    int prot[236] = {0};
    for(int i=0; i<236; i++){
        prot[i] = 235*cumulative_probability_distribution[i];
    }

    //STEP 6
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            for(int x=0; x<236; x++){
                if(y[i][j] == x){
                    y[i][j] = prot[x];
                    break;
                }
            }
        }
    }

    //yuv to rgb pixel
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            C = y[i][j] - 16;
            D = u[i][j] - 128;
            E = v[i][j] - 128;

            R = clip(( 298 * C + 409 * E + 128) >> 8);
            G = clip(( 298 * C - 100 * D - 208 * E + 128) >> 8);
            B = clip(( 298 * C + 516 * D + 128) >> 8);

            array[i][j].setRed(R); 
            array[i][j].setGreen(G); 
            array[i][j].setBlue(B);
        }
    }
    return *this;
}

RGBImage& RGBImage::operator = (const RGBImage& img){
    this->height = img.height;
    this->width = img.width;
    this->max_luminocity = img.max_luminocity;
    
    this->rgb_array = new RGBPixel*[height];
    for (int i = 0; i < height; ++i) {
        this->rgb_array[i] = new RGBPixel[width];
    }
    
    RGBPixel** array = img.rgb_array;
    int red, green, blue;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            red = (int)array[i][j].getRed();
            green = (int)array[i][j].getGreen();
            blue = (int)array[i][j].getBlue();
            this->rgb_array[i][j].setRed(red);
            this->rgb_array[i][j].setGreen(green);
            this->rgb_array[i][j].setBlue(blue);
        }
    }
    return *this;
} 

Pixel& RGBImage::getPixel(int row, int col) const{
    return rgb_array[row][col];
}

void RGBImage::free_array(RGBPixel** array, int height){
    for (int i = 0; i < height; i++){
        delete[] array[i];
    }
    delete[] array;
}