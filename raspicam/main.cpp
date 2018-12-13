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
    time_t curr_time;	// current time is used for saving file name
    struct tm *curr_tm;
    curr_time= time(NULL);
    curr_tm=localtime(&curr_time);

    raspicam::RaspiCam Camera; //Cmaera object

    if (!Camera.open()){
	cerr<<"Error opening camera"<<endl;
	return -1;
    }
    //wait for camera stabilization
    sleep(3);

    // taking a picture
    Camera.grab();

    // allocate memory for photo data
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];

    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB ); //get camera image

    char name[100]; // photo name

    sprintf(name, "%s/image_%d-%d-%d", PATH, curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);

    std::ofstream outFile ( name, std::ios::binary ); // save photo in name
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );

    sleep(5);

    // the default photo format that raspicam creates is ppm
    // so we should convert the photo to jpg format from ppm
    // we used linux utility 'imagemagick' to convert image, and this is array for the command
    char convert_com[100];

    sprintf(convert_com, "convert %s %s.jpg", name,  name); // convert ppm file to jpg file
    printf("convert : %s\n", convert_com); //
    system(convert_com);

    // array for upload command
    char command[200];
    // make upload command
    sprintf(command, "curl -F \"msg=image\" -F \"photo=@%s.jpg\" -F \"pet_idx=1\" \"http://challenge.ajou-whois.org/sp/index.php?page=addEvent\"", name);
    system(command);
    //free resrources
    delete data;
    return 0;
}
