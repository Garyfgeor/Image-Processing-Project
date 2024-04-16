#include <iostream>
#include "GSCImage.hpp"
#include "GSCPixel.hpp"
#include "RGBImage.hpp"

GSCImage::GSCImage(){}

//change rgb image to grayscale
GSCImage::GSCImage(const RGBImage& grayscaled){
    int gray_val, red_val, green_val, blue_val;
    int height = grayscaled.getHeight();
    int width = grayscaled.getWidth();
    int max_lum = grayscaled.getMaxLuminocity();

    GSCPixel** gsc_array = new GSCPixel*[height];
    for (int i = 0; i < height; ++i) {
        gsc_array[i] = new GSCPixel[width];
    }
    RGBPixel** array = grayscaled.get_rgb_array();
    this->setHeight(height);
    this->setWidth(width);
    this->setMaxLuminocity(max_lum);

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            red_val = (int)array[i][j].getRed();
            green_val = (int)array[i][j].getGreen();
            blue_val = (int)array[i][j].getBlue();
            gray_val = red_val * 0.3 + green_val * 0.59 + blue_val * 0.11;
            gsc_array[i][j].setValue(gray_val);
        }
    }
    this->gsc_array = gsc_array;
}

//copy constructor
GSCImage::GSCImage(const GSCImage& img){
    this->height = img.height;
    this->width = img.width;
    this->max_luminocity = img.max_luminocity;
    
    this->gsc_array = new GSCPixel*[height];
    for (int i = 0; i < height; ++i) {
        this->gsc_array[i] = new GSCPixel[width];
    }
    
    GSCPixel** array = img.gsc_array;
    int value;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            value = (int)array[i][j].getValue();
            this->gsc_array[i][j].setValue(value);
        }
    }
}

GSCImage& GSCImage::operator = (const GSCImage& img) {
    this->height = img.height;
    this->width = img.width;
    this->max_luminocity = img.max_luminocity;
    
    this->gsc_array = new GSCPixel*[height];
    for (int i = 0; i < height; ++i) {
        this->gsc_array[i] = new GSCPixel[width];
    }
    
    GSCPixel** array = img.gsc_array;
    int value;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            value = (int)array[i][j].getValue();
            this->gsc_array[i][j].setValue(value);
        }
    }
    return *this;
}

GSCPixel** GSCImage::get_gsc_array() const{
    return gsc_array;
}

//constructor
GSCImage::GSCImage(std::istream& stream){
    int width, height, luminocity;
    stream >> width >> height >> luminocity;
    setWidth(width);
    setHeight(height);
    setMaxLuminocity(luminocity);
  
    this->gsc_array = new GSCPixel*[height];
    for (int i = 0; i < height; ++i) {
        this->gsc_array[i] = new GSCPixel[width];
    }
    
    int value;
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            stream >> value;
            this->gsc_array[i][j].setValue(value);
        }
    }
}

//destructor
GSCImage::~GSCImage(){
    for (int i = 0; i < this->getHeight(); i++){
        delete[] gsc_array[i];
    }
    delete[] gsc_array;
}

//reverse colors of image
Image& GSCImage::operator!(){
    int height = this->getHeight();
    int width = this->getWidth();
    int max_lum = this->getMaxLuminocity();

    GSCImage* gsc_image = dynamic_cast<GSCImage*>(this);
    GSCPixel** array = gsc_image->get_gsc_array();

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            int value = (int)array[i][j].getValue();
            array[i][j].setValue(max_lum - value); 
        }
    }
    return *this;
}


//mirror image
Image& GSCImage::operator*(){
    int height = this->getHeight();
    int width = this->getWidth();

    GSCPixel** mirrored_array = new GSCPixel*[height];
    for (int i = 0; i < height; ++i) {
        mirrored_array[i] = new GSCPixel[width];
    }
    GSCPixel** array = this->get_gsc_array();

    //diatrexw ton pinaka stili-stili
    for(int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            mirrored_array[i][width-1-j] = array[i][j];
        }
    }

    free_array(array, height);
    this->gsc_array = mirrored_array;
    return *this;
}

//rotate image times
Image& GSCImage::operator+=(int times){
    int height = this->getHeight();
    int width = this->getWidth();
    GSCPixel* line = new GSCPixel[width];
    GSCPixel** rotated_array;
    GSCPixel** array;
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
        rotated_array = new GSCPixel*[rot_height];
        for (int i = 0; i < rot_height; ++i) {
            rotated_array[i] = new GSCPixel[rot_width];
        }

        array = this->get_gsc_array();
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

        free_array(gsc_array, height);
        this->gsc_array = rotated_array;
        this->setHeight(rot_height);
        this->setWidth(rot_width);
    }
    else if(res == 2){
        rot_height = height;
        rot_width = width;
        rotated_array = new GSCPixel*[rot_height];
        for (int i = 0; i < rot_height; ++i) {
            rotated_array[i] = new GSCPixel[rot_width];
        }

        array = this->get_gsc_array();
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

        free_array(gsc_array, height);
        this->gsc_array = rotated_array;
        this->setHeight(rot_height);
        this->setWidth(rot_width);
    }
    else if(res == 3){
        rot_height = width;
        rot_width = height;
        rotated_array = new GSCPixel*[rot_height];
        for (int i = 0; i < rot_height; ++i) {
            rotated_array[i] = new GSCPixel[rot_width];
        }

        array = this->get_gsc_array();
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

        free_array(gsc_array, height);
        this->gsc_array = rotated_array;
        this->setHeight(rot_height);
        this->setWidth(rot_width);
    }
    delete[] line;
    return *this;
}  

//change image size by factor
Image& GSCImage::operator *= (double factor){
    int height = this->getHeight();
    int width = this->getWidth();
    int resized_height = factor*height;
    int resized_width = factor*width;

    GSCPixel** resized_array = new GSCPixel*[resized_height];
    for (int i = 0; i < resized_height; i++){
        resized_array[i] = new GSCPixel[resized_width];
    }
    
    int r1, r2, c1, c2, new_val;
    for(int j=0; j<resized_width; j++){
        for(int i=0; i<resized_height; i++){
            r1 = min((int)floor(i/factor), height-1);
            r2 = min((int)ceil(i/factor), height-1);
            c1 = min((int)floor(j/factor), width-1);
            c2 = min((int)ceil(j/factor), width-1);

            Pixel& pixel = this->getPixel(r1,c1);
            GSCPixel& P11 = static_cast<GSCPixel&>(pixel);
            pixel = this->getPixel(r1, c2);
            GSCPixel& P12 = static_cast<GSCPixel&>(pixel);
            pixel = this->getPixel(r2, c1);
            GSCPixel& P21 = static_cast<GSCPixel&>(pixel);
            pixel = this->getPixel(r2, c2);
            GSCPixel& P22 = static_cast<GSCPixel&>(pixel);
            
            new_val = (P11.getValue() + P12.getValue() + P21.getValue() + P22.getValue())/4;
            resized_array[i][j].setValue(new_val);
        }
    }
    this->setHeight(resized_height);
    this->setWidth(resized_width);
    free_array(gsc_array, height);
    this->gsc_array = resized_array;
    return *this;
}

int GSCImage::clip(int num){
    if(num>255){
        num = 255;
    }
    else if(num<0){
        num = 0;
    }
    return num; 
}

//equalize histogram
Image& GSCImage::operator ~(){
    int histogram_array[236] = {0};
    int width = this->getWidth();
    int height = this->getHeight();
    double probability_distribution[236] = {0};
    double cumulative_probability_distribution[236] = {0};
    int num_of_pixels = width*height;

    int R, G, B;
    int Y, U, V;
    int C, E;

    GSCPixel** array = this->get_gsc_array();
    //STEP 1
    //gsc to rgb and then yuv
    int y[height][width] ={{0}};
    int u[height][width] ={{0}};
    int v[height][width] ={{0}};
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            R = (int)array[i][j].getValue();
            G = (int)array[i][j].getValue();
            B = (int)array[i][j].getValue();

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
        probability_distribution[i] = static_cast<double>(histogram_array[i])/num_of_pixels;
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

    //yuv to gsc pixel
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            C = y[i][j] - 16;
            E = v[i][j] - 128;

            R = clip(( 298 * C + 409 * E + 128) >> 8);
            array[i][j].setValue(R); 
        }
    }
    return *this;
}

Pixel& GSCImage::getPixel(int row, int col) const{
    return gsc_array[row][col];
}

void GSCImage::free_array(GSCPixel** array, int height){
    for (int i = 0; i < height; i++){
        delete[] array[i];  
    }
    delete[] array;
}