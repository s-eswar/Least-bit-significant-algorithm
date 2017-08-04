# Least-bit-significant-algorithm
A C++ project for the sequential code of LSB algorithm for image steganography

For running this code you need Opencv 2.4.13 version or more.
Compile using command "g++ lsb.cpppkg-config --cflags --libs opencv-o lsb"
Run using "./lsb [input image] [message text]"
The output displays the Stego image and also writes the decoded message to the terminal. Moreover, the Stego image is saved using imwrite. Both the encode and decode functions are as part of the same file. To improve, functions can be put into switch case or command line.
