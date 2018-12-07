#include <unistd.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <ctime>
#include <string.h>

#define PATH "/home/pi/Desktop/sp/raspicam/build"
using namespace std;


int main ( int argc,char **argv )
{
    time_t curr_time;
    struct tm *curr_tm;
    curr_time= time(NULL);
    curr_tm=localtime(&curr_time);

    raspicam::RaspiCam Camera; //Cmaera object

    //Open camera
    cout<<"Opening Camera..."<<endl;
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}

    //wait a while until camera stabilizes
    cout<<"Sleeping for 3 secs"<<endl;
    sleep(3);


    //capture
    Camera.grab();

    //allocate memory
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];

    //extract the image in rgb format
    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image

    char name[100];

    sprintf(name, "%s/image_%d-%d-%d", PATH, curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);

    //save
    std::ofstream outFile ( name, std::ios::binary );
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
    cout<< "File : " << name <<endl;

    sleep(5);
    char convert_com[100];
    sprintf(convert_com, "convert %s %s.jpg", name,  name);
    printf("convert : %s\n", convert_com);
    system(convert_com);
   
    //upload routine
    char command[200];
    sprintf(command, "curl -F \"msg=image\" -F \"photo=@%s.jpg\" -F \"pet_idx=1\" \"http://challenge.ajou-whois.org/sp/index.php?page=addEvent\"", name);
    system(command);
    printf(command);

    //free resrources
    delete data;
    return 0;
}
