#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <bitset>
#include <typeinfo>

using namespace cv;
using namespace std;

// convert text to ascii
void convert_to_ascii(string letter, int *ascii)
{
int sum = 0;
 for (int k = 0; k < letter.length(); k++)
    {
        char x = letter.at(k);
        ascii[k]=int(x);
    }
}


// convert ascii values to binary string
string convert(int value)
{
    char* result = (char*)malloc(sizeof(char) * 32);

    std::string s = std::bitset< 8 >( value ).to_string();

    //to_bin_str(value, result, &j);
    //result[j] = '\0';

    return (s);
}


// reading file and doing conversions
void readfile(char *filename,char *binary)
{
       ifstream infile;
       infile.open(filename);
  
       // writing contents of the fie to string variable contents
       std::string content( (std::istreambuf_iterator<char>(infile) ), (std::istreambuf_iterator<char>() ) );   

       int ascii[content.length()];
     
       // function call to convert to ascii
       convert_to_ascii(content,ascii);

       int m=0;
       string temp; 
       for(int j = 0; j<content.length() ; j++)
       {  temp=convert(ascii[j]);                // function call to convert to binary.
          int k=0;
          while(k<8)                             //putting char one by one into binary, array of char.
          {
            binary[m]=temp[k];
            m++;k++;
          }
          
       }
}


// retruning the size of the file
int Get_Size(char *path)
{
  FILE *p_file = NULL;
  p_file = fopen(path,"rb");
  fseek(p_file,0,SEEK_END);
  int size = ftell(p_file); 
  fclose(p_file);
  return size;
}


// Binary to ascii conversion function
void bin_to_ascii(const char* input, char *ascii)
{
	int length = strlen(input);     //get length of string
	
	int binary[8];    //array used to store 1 byte of binary number (1 character)
	int asciiNum = 0;      //the ascii number after conversion from binary
	
	int z = 0;   //counter used
	int m = 0;   //counter used
	
	for(int x = 0; x < length / 8; x++)     //reading in bytes. total characters = length / 8
	{
		for(int a = 0; a < 8; a++)      //store info into binary[0] through binary[7]
		{
			binary[a] = (int) input[z] - 48;      //z never resets
			z++;
		}
		
		int power[8];    //will set powers of 2 in an array
		int counter = 7;        //power starts at 2^0, ends at 2^7
		for(int x = 0; x < 8; x++)
		{
			power[x] = counter;      //power[] = {7, 6, 5, ..... 1, 0}
			counter--;    //decrement counter each time
		}
		
		for(int y = 0; y < 8; y++)    //will compute asciiNum
		{
			double a = binary[y];    //store the element from binary[] as "a"
			double b = power[y];    //store the lement from power[] as "b"
			
			asciiNum += a* pow(2, b);   //asciiNum = sum of a * 2^power where 0 <= power <= 7, power is int
		}
		
		ascii[m] = asciiNum;   //assign the asciiNum value to ascii, to change it into an actual character
                m++;
		asciiNum = 0;    //reset asciiNum for next loop
		
	}
}


// encoding the text file into image
Mat encode(Mat new_image,int size,char *binary)
{
    unsigned int out;
    int m=0;
    for(int i=0;i<size;i++)
    {
      for(int k=0;k<8;k++)
      {
        out=new_image.at<uchar>(i,k) & 254;       // ANDing each pixeel with the mask size of 254 
        if(binary[m]=='1' && m<size*8)             // changing the last bit based on the binary value of hiding text. Needs to be changed if it 1
           out=out|1;
        new_image.at<uchar>(i,k)=out;             // writing back to the new_image as its new pixel value
        m++;

      }
    }
    return new_image;
}


//decoding function
void decode(Mat image,int size,char *out_binary)
{
  unsigned int out;
  int m=0;
  for(int i=0;i<size;i++)
  {
      for(int k=0;k<8;k++)
      {
        out=image.at<uchar>(i,k) & 1 ;             // ANDing with 1 to get the lastbit of the byte
        if(out==1)                                 // putting binary character into an out_binary array
           out_binary[m]='1';                    
        else
           out_binary[m]='0';
        m++;
      }
    }
}


// main
int main( int argc, char** argv )
{
    
    Mat image,new_image;
    image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);   // Read the image
    new_image=image;
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    imshow( "original", image );
    waitKey(0);                                             // Wait for a keystroke in the window
    
    int size=Get_Size(argv[2]);
    char *binary=(char *)malloc(sizeof(char)*size*8);       // memory allocation for binary
    char *out_binary=(char *)malloc(sizeof(char)*size*8);   // memory allocation for binary
    char *ascii=(char *)malloc(sizeof(char)*size);          // memory allocation for binary
    
    readfile(argv[2],binary);                               // reading the text file function, passing filename and the char * binary
    
    new_image=encode(image,size,binary);                    // encoding function call
    
    decode(new_image,size,out_binary);                      // decoding function call
    
    bin_to_ascii(out_binary,ascii);                         // converting binary to ascii. retrieving the message
    
    //namedWindow( " Display window", 1 );
    imshow( "stego image", new_image );
    imwrite( "stego_image.bmp", new_image );
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
