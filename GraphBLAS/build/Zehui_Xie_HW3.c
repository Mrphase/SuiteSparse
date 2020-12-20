#include <stdio.h>
#include <math.h>

//Author:Zehui Xie

int main(){
	float latitude;
	float longitude;
	char ns;
	char ew;
	
	printf("Enter coordinates in decimal form:\n");
	printf("Latitude(The effective range is -90 to 90):");
	scanf("%f",&latitude);
	
	if(latitude>90||latitude<-90){
		printf("Out of valid range!The effective range of latitude is -90 to 90");
		return 0;
	}
		
	printf("Longitude(The effective range is -180 to 180):");
	scanf("%f",&longitude);
	
	if(longitude>180||longitude<-180){
		printf("Out of valid range!The effective range of longtitude is -180 to 180");
		return 0;
	}
	
	if(latitude<0){
		ns='S';
		latitude=-1*latitude;
	}
	else if(latitude==0){
		ns=' ';
	}
	else{
		ns='N';
	}
	
	if(longitude<0){
		ew='W';
		longitude=-1*longitude;
	}
	else if(longitude==0){
		ew=' ';
	}
	else{
		ew='E';
	}
	

	int lad = latitude/1;
    int lam = (latitude -lad) * 60;
    double las = ((latitude -lad) * 60 - lam) * 60;
    printf("Latitude: %d degrees, %d minutes, %f seconds %c\n", lad, lam, las, ns);
    
    int lod = longitude/1;
    int lom = (longitude -lod) * 60;
    double los = ((longitude -lod) * 60 - lom) * 60;
    printf("Longitude: %d degrees, %d minutes, %f seconds %c\n", lod, lom, los, ew);
}
