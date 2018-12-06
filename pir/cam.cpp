#include <unistd.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <raspicam/raspicam.h>
#include <string.h> 
using namespace std;


void capture (void)
{
    time_t curr_time;
    struct tm *curr_tm;
    curr_time= time(NULL);
    curr_tm=localtime(&curr_time);
   

    raspicam::RaspiCam Camera; //Cmaera object
 
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;}
 
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

    sprintf(name, "image_%d-%d-%d", curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);
    
    //save
    std::ofstream outFile ( name, std::ios::binary );
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
    cout<<name<<endl;
   
    //free resrources
    delete data;
    return ;
}

int main(){
	int c = 0;
	while(1)
	{	
		scanf("%d", &c);
		if(c==1)
			capture();
	    else if(c!=1)
			return 0;	
	}
 return 0;
 
}




