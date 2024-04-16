#include <iostream>
#include <fstream>
#include "Image.hpp"
#include "RGBImage.hpp"
#include "GSCImage.hpp"
#include "Token.hpp"
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

//function that reads the image from file
Image* readNetpbmImage(const char* filename){
    ifstream f(filename);
	
	if(!f.is_open()){
		cout << "[ERROR] Unable to open " << filename << endl;
	}
	Image* img_ptr = nullptr;
	string type;
	
	if(f.good() && !f.eof()){
		f>>type;
	}
	if(!type.compare("P3")){
		img_ptr = new RGBImage(f);
	}
	else if(!type.compare("P2")){
		img_ptr = new GSCImage(f);
	}
    else if(f.is_open()) {
        cout << "[ERROR] Invalid file format" << endl;
    }
	return img_ptr;
}

int main() {
    vector<Token*> token_vector;  
    int pos=0;
    string token_name;
    string filename;
    int start=0, end=0, startFile=0, endFile=0;
    Image* ptr;  
    while(1){
        //read the line from cin
        string command;
        getline(cin, command); 
        
        if((command.find('$') == string::npos && command[0] != 'q') || (command[1] != ' ' && command[0] != 'q') || 
           ((command[0] == 'i' || command[0] == 'e') && command.find(".ppm") == string::npos && command.find(".pgm") == string::npos) || 
           (command[0] == 'e' && command.find("as") == string::npos) || (command[0] == 'r' && (command.find("clockwise") == string::npos || command.find("times") == string::npos)) || 
           (command[0] == 's' && command.find("by") == string::npos)){
            cout << "\n-- Invalid command! -–" << endl;
        }
        else{
            if(command[0] == 'i' || command[0] == 'e'){
                //get filename
                endFile = command.find(".ppm");
                if(endFile == -1){
                    endFile = command.find(".pgm");
                }
                startFile = command.find_last_of(' ', endFile+3);
                filename = command.substr(startFile+1, endFile+3-startFile);
            }
            if(command[0] != 'q'){
                //get token
                start = command.find("$");
                end = command.find_first_of(' ', start);
                token_name = command.substr(start, end-start);
            }
            ifstream f;
            ofstream of;
            int exists = 0;
            if(command[0] != 'q'){
                for(size_t i=0; i<token_vector.size(); i++){
                    if(token_vector.at(i)->getName().compare(token_name) == 0){
                        exists = 1;
                        pos = i;
                    }
                }
                if(exists == 0 && command[0] != 'i'){ 
                    cout << "[ERROR] Token " << token_name << " not found!" << endl;
                }
            }
            switch(command[0]){
                case 'i'://import filename as token
                    if(command.find_last_of("as", start) != string::npos && command.find_first_of("as", endFile) != string::npos && command.find_last_of("as", startFile) == string::npos){
                        if(exists == 0){
                            f.open(filename);
                            const char* fn = filename.c_str();
                            if(f && f.good()){
                                ptr = readNetpbmImage(fn);
                                if(ptr != nullptr){
                                    Token *token = new Token(token_name, ptr);
                                    token_vector.push_back(token);
                                    cout << "[OK] Import " << token_name << endl;
                                }
                            }
                            else{
                                cout <<  "[ERROR] Unable to open " << filename << endl;
                            }
                        }
                        else{
                            cout <<  "[ERROR] Token " << token_name << " exists" << endl;
                        }
                    }
                    else{
                        cout << "\n-- Invalid command! -–" << endl;
                    }
                    break;

                case 'e'://export token as filename
                    if(exists == 1){
                        f.open(filename);
                        if(f){ 
                            cout << "[ERROR] File exists"<< endl;
                        }
                        else{
                            of.open(filename);
                            Image* ptr1 =  token_vector.at(pos)->getPtr();
                            of<<*ptr1;
                            cout << "[OK] Export " << token_name << endl;
                        }
                    }
                    break;
                
                case 'd'://delete token with the image
                    if(exists == 1){
                        vector<Token*>::iterator it = token_vector.begin()+pos;
                        delete *it;
                        token_vector.erase(it);
                        cout << "[OK] Delete " << token_name << endl;
                    }
                    break;

                case 'n'://invert color of token
                    if(exists==1){
                        Image* ptr1 = token_vector.at(pos)->getPtr();
                        ptr1->operator!();
                        cout <<"[OK] Color Inversion "<< token_name <<endl;
                    }
                    break;

                case 'z'://equalize histogram of token
                    if(exists==1){
                        Image* ptr1 = token_vector.at(pos)->getPtr();
                        ptr1->operator~();
                        cout <<"[OK] Equalize "<< token_name <<endl;
                    }
                    break;

                case 'm'://mirror token
                    if(exists==1){
                        Image* ptr1 = token_vector.at(pos)->getPtr();
                        ptr1->operator*();
                        cout<< "[OK] Mirror " << token_name << endl;
                    }
                    break;   

                case 'g'://turn image to grayscale
                    if(exists==1){
                        Image* ptr1 = token_vector.at(pos)->getPtr();
                        if(dynamic_cast<RGBImage*>(ptr1) != nullptr){
                            RGBImage* rgb_img = dynamic_cast<RGBImage*>(ptr1);
                            Image* img_ptr = new GSCImage(*rgb_img);
                            delete rgb_img;
                            token_vector.at(pos)->setPtr(img_ptr);
                            
                            cout<< "[OK] Grayscale " << token_name << endl;
                        }
                        else{
                            cout<< "[NOP] Already grayscale " << token_name << endl;
                        }
                    }
                    break;    

                case 's'://change image size by factor
                    if(exists == 1){
                        
                        if(command.find("by") == string::npos){
                            cout << "\n-- Invalid command! -–" << endl;
                        }
                        else{
                            Image* ptr1 = token_vector.at(pos)->getPtr();
                            string str_buf;
                            string num_str;
                            double num;
                            istringstream string_stream(command);
                            while(string_stream >> num_str){ 
                                if(isdigit(num_str[0]) || num_str[0] == '.'){                                  
                                   str_buf.append(num_str);
                                }
                            }
                            num = stod(str_buf);
                            ptr1->operator*=(num);
                            cout << "[OK] Scale " << token_name <<endl;
                        }
                    }
                    break;

                case 'r'://rotate image clockwise X times
                    if(exists == 1){
                        if(command.find("clockwise") == string::npos || command.find("times") == string::npos){
                            cout << "\n-- Invalid command! -–" << endl;
                        }
                        else{
                            Image* ptr1 = token_vector.at(pos)->getPtr();
                            string num_str;
                            int num;
                            istringstream string_stream(command);
                            while(string_stream >> num_str){ 
                                if(isdigit(num_str[0]) || (num_str[0] == '-' && isdigit(num_str[1]))){                                  
                                    num = stoi(num_str);
                                }
                            }
                            ptr1->operator+=(num);
                            cout << "[OK] Rotate " << token_name <<endl;
                        }
                    }
                    break; 

                case 'q'://exit and free memory
                    for(size_t i=0; i<token_vector.size(); i++){
                        if(token_vector.at(i) != nullptr){
                            delete token_vector[i];
                        }
                    }         
                    return 0;
                    break;

                default:
                    cout << "\n-- Invalid command! -–" << endl;
                    break;
            }
        }
    }
    return 0;
}

