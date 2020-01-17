/*Theodore Church G01127117
 * CS 262 Section 218
 * Project 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Trip_struct{
	char membershipType[12];
	int startStationId;
	int endStationId;
	int bikeId;
	int duration;
	int startHr;
	int startMin; 
}Trip;

typedef struct Bike_struct{
   char maintenanceFlag;
   int bikeId, endStationId;
   int numTrips, totalDuration;
   float totalMinutes;
}Bike;
typedef struct Station_info{
	int id;
	int tripCount;
}Station;

void mainMenu();
void metricsMenu();
void printMenu();
int compareBikeId(const void*,const void *);
int bikeCompareBikeId(const void*,const void*);
int compareStartS(const void*,const void *);
int compareEndS(const void*,const void *);
int compareDuration(const void*,const void *);
int compareStartHr(const void*,const void *);
int compareStartMin(const void*,const void *);
int compareMinutes(const void*,const void *);
int compareTripCount(const void*,const void*);
double averageTripsPerHour(int);
int hourMostTrips(Trip*,int);
int tripsPerEachHour(Trip*,int,int*);
double averageDuration(Trip*,int);
int longestTrip(Trip*,int);
void fiveBusiestStart(Trip*,int,Station*);
void fiveBusiestEnd(Trip*,int,Station*);
double membershipPercent(Trip*,int);
void longestBikes(Bike*,Bike*);
void tripCountBikes(Bike*,Bike*);
int twoLess(Bike*,int*);
void addBike(Bike*,int*,int,int);
void removeBike(Bike*,int*,int);
int findBike(Bike*,int,int*);

int main(void){
	char inBuf[100];
	int lines = 0;
	int i;/*loops*/
	int j;/*loops*/
	int b;/*loops*/
	int bikesInt;/*loops*/
	int currentLine;
	int quit = 0;
	int innerQuit = 0;
	int updateBike = 0;
	char fname[100];
	char menuChoice;
	int printChoice;
	while(quit == 0){
		mainMenu();
		fgets(inBuf,sizeof(inBuf),stdin);
		sscanf(inBuf,"%c",&menuChoice);
		if(menuChoice == '1'){
			printf("Please input the name of the file you'd like to read trip data from. Maximum 100 characters\n");
			fgets(inBuf,sizeof(inBuf),stdin);
			sscanf(inBuf,"%s",fname);
			FILE *input = fopen(fname,"r");
			if(input == NULL){
				printf("File not found.\n");
				exit(1);
			}		
			else{
				printf("File opened.\n");
			}
			fgets(inBuf,sizeof(inBuf),input);
			sscanf(inBuf,"%d",&lines);
			Trip *trips = (Trip*)malloc(lines*sizeof(Trip));
			Bike *bikes = (Bike*)malloc(4000*sizeof(Bike));
			Station startStations[5];
			Station endStations[5];
			Bike topTen[10];
			if (trips == NULL||bikes ==NULL){
				printf("Memory not allocated correctly");
				exit(1);
			}
			else{
				for(i = 0; i<lines; i++){
					fgets(inBuf,sizeof(inBuf),input);
					Trip *thisTrip = &trips[i];
					sscanf(inBuf,"%s %d %d %d %d %d %d",(thisTrip->membershipType),
									   &(thisTrip->startStationId),
									   &(thisTrip->endStationId),
									   &(thisTrip->bikeId),
									   &(thisTrip->duration),
									   &(thisTrip->startHr),
									   &(thisTrip->startMin));
				}
				printf("Trip file read.\n");
				currentLine = 1;
				int new = 0;
				for(i = 0; i<lines;i++){
					for(j=0;j<currentLine;j++){
						if(trips[i].bikeId == bikes[j].bikeId){
							/* new 1 means this bike isn't new, new bikes need to be entered */
							new = 1;
							bikes[j].numTrips +=1;
							bikes[j].endStationId = trips[i].endStationId;
							bikes[j].totalDuration += trips[i].duration;
							bikes[j].totalMinutes += ((float)trips[i].duration/60000);
						}
					}
					if(new==0){
						bikes[currentLine].bikeId = trips[i].bikeId;
						bikes[currentLine].numTrips = 1;
						bikes[currentLine].endStationId = trips[i].endStationId;
						bikes[currentLine].totalDuration = trips[i].duration;
						bikes[currentLine].totalMinutes +=((float)trips[i].duration/60000);
						currentLine++;
					}	
				}
				printf("Bikes have been entered.\n");
			}
			mainMenu();
			/* Switch Case starts here */
			/*I misread the prompt, CEO metrics are at bottom, the switch case is out of order. */
			/* All metrics follow 1 = print to prompt. 2 = print to file */
			while(innerQuit == 0){
				fgets(inBuf,sizeof(inBuf),stdin);
				sscanf(inBuf,"%c",&menuChoice);
				switch(menuChoice){
					/*Q and R at top check to quit / return first */
					case 'Q':
						printf("Quitting.\n");
						innerQuit=1;
						quit=1;
						break;
					case 'R':
						printf("Returning to main menu.\n");
						mainMenu();
						break;
					case '1':
						/*there might be a reason to enter more/different trip data this can be changed */
						printf("Data already entered.\n");
						break;
					case '2':
						metricsMenu();
						break;
					case '3':
						printMenu();
						break;
					case '4': /*flag bike for maintenance */
						longestBikes(bikes,topTen);
						printf("Flagging Done.\n");
						break;
					case '5':/* update bike inventory */
						printf("Enter 1 to add a bike or 2 to remove a bike. You will need bikeId and station number to add\n");
						printf("and bikeId to remove.\n");
						fgets(inBuf,sizeof(inBuf),stdin);
						sscanf(inBuf,"%d",&updateBike);
						if(updateBike == 1){
							int addId = 0;
							int stationNumber = 0;
							printf("Please enter bikeId to add.\n");
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%d",&addId);
							printf("Please enter StationId the bike is at.\n");
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%d",&stationNumber);
							addBike(bikes,&currentLine,addId,stationNumber);
						}
						else if(updateBike == 2){
							int removeId = 0;
							printf("Please enter bikeId to remove.\n");
							fgets(inBuf,sizeof(inBuf),stdin);
							sscanf(inBuf,"%d",&removeId);
							removeBike(bikes,&currentLine,removeId);
						}
						else{
							printf("Invalid entry. Returning to main menu.\n");
							mainMenu();
						}
						break;
					case '6': /*Average trips per hour */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
						printChoice = 0;
						fgets(inBuf,sizeof(inBuf),stdin);
                                                sscanf(inBuf,"%d",&printChoice);
						if(printChoice == 1){
							printf("Average Trips per hour: %.2lf.\n",averageTripsPerHour(lines));
						}
						else if(printChoice == 2){
							printf("Please enter your filename for output. Max 100 chars.\n");
							char outputFile[100];
							fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
							FILE *output = fopen(outputFile,"w");
							fprintf(output,"Average Trips per hour: %.2lf.\n",averageTripsPerHour(lines));
							fclose(output);
						}
						else{
							printf("Not a valid option. Returning.\n");
						}
						mainMenu();
						break;
					case '7': /* Largest number of trips */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");                                                              printChoice = 0;
                                                fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							printf("The hour with the largest amount of trips is: %d.\n", hourMostTrips(trips,lines));
						}
						else if(printChoice == 2){
                                                	printf("Please enter your filename for output. Max 100 chars.\n");                                                                                                      char outputFile[100];                                                                                                                                                   fgets(inBuf,sizeof(inBuf),stdin);
                                                	sscanf(inBuf,"%s",outputFile);                                                                                                                                          FILE *output = fopen(outputFile,"w");
                                                	fprintf(output,"The hour with the largest amount of trips is: %d.\n",hourMostTrips(trips,lines));
							fclose(output);
						}
                                                else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
						mainMenu();
						break;
					case '8': /*Count trips per hour */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");                                                              printChoice = 0;
                                                fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice ==1){                                                                                                                                                 	
							int array[24];
							tripsPerEachHour(trips,lines,array);
							int e;
							for(e =0; e<24;e++){
								printf("Hour %d: Trips: %d\n",e,array[e]);
							}
						}
                                                else if(printChoice == 2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");                                                                                                      char outputFile[100];                                                                                                                                                   fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);                                                                                                                                          FILE *output = fopen(outputFile,"w");
                                                	int array[24];
							tripsPerEachHour(trips,lines,array);
							int e;
							for(e=0;e<24;e++){
								fprintf(output,"Hour %d: Trips %d\n",e,array[e]);
							}
							fclose(output);
						}
                                                else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
						mainMenu();
						break;
					case '9': /* Average Duration */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
                                                        printf("Average duration of trips in minutes: %.2lf\n",(double)(averageDuration(trips,lines))/60);
							printf("Average duration of trips in milliseconds: %.0lf\n",(averageDuration(trips,lines)*1000));
                                                }
                                                else if(printChoice == 2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
                                                        fprintf(output,"Average duration of trips in minutes: %.2lf\n",(double)(averageDuration(trips,lines))/60);
	                                                fprintf(output,"Average duration of trips in milliseconds: %.0lf\n",(averageDuration(trips,lines)*1000));
							fclose(output);                                                                                                                                                 }
                                                else{                                                                                                                                                                           printf("Not a valid option. Returning.\n");
                                                }
						mainMenu();
                                                break;
					case 'A': /* longest duration trip */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							printf("The longest trip was %d milliseconds.\n",longestTrip(trips,lines));
							printf("The longest trip was %d minutes.\n",(longestTrip(trips,lines)/6000));
						}
						else if(printChoice ==2){
							printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							fprintf(output,"The longest trip was %d milliseconds.\n",(longestTrip(trips,lines)));
							fprintf(output,"The longest trip was %d minutes.\n",(longestTrip(trips,lines)/6000));
							fclose(output);
						}
						else{                                                                                                                                                                           printf("Not a valid option. Returning.\n");
                                                }
						mainMenu();
                                                break;
					case 'B':/*5 busiest start*/
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);
						fiveBusiestStart(trips,lines,startStations);
						if(printChoice == 1){                                                                                                                                                           for(b=0; b<5;b++){
                                                                printf("Station: %d Station Number: %d Trip Count: %d\n",(b+1),                                                                                                                                                                 (startStations[b].id),                                                                                                                                                  (startStations[b].tripCount));                                                                             }
                                                }
                                                else if(printChoice == 2){
                                                	printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
                                                        for(b=0; b<5;b++){
                                                                fprintf(output,"Station: %d Station Number: %d Trip Count: %d\n",(b+1),                                                                                                                                                         (startStations[b].id),
                                                                                                                        (startStations[b].tripCount));
                                                        }
                                                        fclose(output);
						}
                                                else{                                                                                                                                                                           printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break; 
					case 'C':/*5 busiest end*/
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);
						fiveBusiestEnd(trips,lines,endStations);
						if(printChoice == 1){
							for(b=0; b<5;b++){
								printf("Station: %d Station Number: %d Trip Count: %d\n",(b+1),
															(endStations[b].id),
															(endStations[b].tripCount));
							}
						}
						else if(printChoice == 2){
							printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							for(b=0; b<5;b++){
                                                                fprintf(output,"Station: %d Station Number: %d Trip Count: %d\n",(b+1),
                                                                                                                        (endStations[b].id),
                                                                                                                        (endStations[b].tripCount));
                                                        }
							fclose(output);
						}
						else{
							printf("Not a valid option. Returning.\n");
						}
						mainMenu();
						break;
					case 'D':/*Membership Percent*/
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							printf("This is the percent of riders who are members: %.2lf\n",membershipPercent(trips,lines));
						}
						else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							fprintf(output,"This it the percent of riders who are members: %.2lf\n",membershipPercent(trips,lines));
							fclose(output);
						}
						else{                                                                                                                                                                           printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;
					case 'E':/*All trip data */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							printf("Average Trips per hour: %.2lf.\n",averageTripsPerHour(lines));
							printf("The hour with the largest amount of trips is: %d.\n", hourMostTrips(trips,lines));
							int array[24];
                                                        tripsPerEachHour(trips,lines,array);
                                                        int e;
                                                        for(e =0; e<24;e++){
                                                                printf("Hour %d: Trips: %d\n",e,array[e]);
                                                        }
							printf("Average duration of trips in minutes: %.2lf\n",(double)(averageDuration(trips,lines))/60);                                                                      printf("Average duration of trips in milliseconds: %.0lf\n",(averageDuration(trips,lines)*1000));
							printf("The longest trip was %d milliseconds.\n",longestTrip(trips,lines));
                                                        printf("The longest trip was %d minutes.\n",(longestTrip(trips,lines)/6000));
							fiveBusiestStart(trips,lines,startStations);
							for(b=0; b<5;b++){
                                                                printf("Station: %d Station Number: %d Trip Count: %d\n",(b+1),                                                                                                                                                                 (startStations[b].id),                                                                                                                                                  (startStations[b].tripCount));                                                                             }
							fiveBusiestEnd(trips,lines,endStations);
							for(b=0; b<5;b++){
                                                                printf("Station: %d Station Number: %d Trip Count: %d\n",(b+1),
                                                                                                                        (endStations[b].id),
                                                                                                                        (endStations[b].tripCount));
                                                        }
							printf("This is the percent of riders who are members: %.2lf\n",membershipPercent(trips,lines));
							longestBikes(bikes,topTen);
							for(bikesInt=0;bikesInt<10;bikesInt++){
                                                                printf("Bike number: %d duration: %.2lf\n",topTen[bikesInt].bikeId,topTen[bikesInt].totalMinutes);
                                                        }
							for(b=0;b<10;b++){
                                                                printf("Bike number: %d Last Station: %d\n",topTen[b].bikeId,topTen[b].endStationId);
                                                        }
							tripCountBikes(bikes,topTen);
							for(b=0;b<10;b++){                                                                                                                                                              printf("Bike number: %d Number of Trips: %d\n",topTen[b].bikeId,topTen[b].numTrips);                                                                            }
							printf("The amount of bikes with 2 or less trips is: %d.\n",twoLess(bikes,&currentLine));
						}
						else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							fprintf(output,"Average Trips per hour: %.2lf.\n",averageTripsPerHour(lines));
                                                        fprintf(output,"The hour with the largest amount of trips is: %d.\n", hourMostTrips(trips,lines));
                                                        int array[24];
                                                        tripsPerEachHour(trips,lines,array);
                                                        int e;
                                                        for(e =0; e<24;e++){
                                                                fprintf(output,"Hour %d: Trips: %d\n",e,array[e]);
                                                        }
                                                        fprintf(output,"Average duration of trips in minutes: %.2lf\n",(double)(averageDuration(trips,lines))/60);                                                              fprintf(output,"Average duration of trips in milliseconds: %.0lf\n",(averageDuration(trips,lines)*1000));
                                                        fprintf(output,"The longest trip was %d milliseconds.\n",longestTrip(trips,lines));
                                                        fprintf(output,"The longest trip was %d minutes.\n",(longestTrip(trips,lines)/6000));
                                                        fiveBusiestStart(trips,lines,startStations);
                                                        for(b=0; b<5;b++){
                                                                fprintf(output,"Station: %d Station Number: %d Trip Count: %d\n",(b+1),                                                                                                                                                                 (startStations[b].id),                                                                                                                                                  (startStations[b].tripCount));                                                                     }                                                                                                                                                                    fiveBusiestEnd(trips,lines,endStations);                                                                                                                                for(b=0; b<5;b++){                                                                                                                                                              fprintf(output,"Station: %d Station Number: %d Trip Count: %d\n",(b+1),
                                                                                                                        (endStations[b].id),
                                                                                                                        (endStations[b].tripCount));
                                                        }
                                                        fprintf(output,"This is the percent of riders who are members: %.2lf\n",membershipPercent(trips,lines));
							longestBikes(bikes,topTen);
							for(bikesInt=0;bikesInt<10;bikesInt++){                                                                                                                                         fprintf(output,"Bike number: %d duration: %.2lf\n",(topTen[bikesInt].bikeId),
														   (topTen[bikesInt].totalMinutes));                                                                            }                                                                                                                                                                       for(b=0;b<10;b++){
                                                                fprintf(output,"Bike number: %d Last Station: %d\n",topTen[b].bikeId,topTen[b].endStationId);
                                                        }
							tripCountBikes(bikes,topTen);
							for(b=0;b<10;b++){                                                                                                                                                             fprintf(output,"Bike number: %d Number of Trips: %d\n",topTen[b].bikeId,topTen[b].numTrips);                                                                     }
							fprintf(output,"The amount of bikes with 2 or less trips is: %d.\n",twoLess(bikes,&currentLine));
							fclose(output);	
						}
						else{                                                                                                                                                                           printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();                                                                                                                                                             break;
					case 'F':/*All Bike inventory */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
                                                        int kcontrol;
							printf("Currently in inventory:\n"); 
							for(kcontrol=0;kcontrol<currentLine;kcontrol++){
								printf("Bike ID: %d\n",bikes[kcontrol].bikeId);
							}
                                                }
                                                else if(printChoice ==2){
							printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							int kcontrol;                                                                                                                                                          	fprintf(output,"Currently in inventory:\n"); 
							for(kcontrol=0;kcontrol<currentLine;kcontrol++){
                                                                fprintf(output,"Bike ID: %d\n",bikes[kcontrol].bikeId);
                                                        }
							fclose(output);
						}
						else{
							printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;
					case 'G':/*CEO report, output file only.*/
						printf("Please enter your filename for output. Max 100 chars.\n");                                                                                                      char outputFile[100];                                                                                                                                                   fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%s",outputFile);
                                                FILE *output = fopen(outputFile,"w");
                                                fprintf(output,"Average Trips per hour: %.2lf.\n",averageTripsPerHour(lines));
                                                fprintf(output,"The hour with the largest amount of trips is: %d.\n", hourMostTrips(trips,lines));
                                                int array[24];
                                                tripsPerEachHour(trips,lines,array);
                                                int e;
                                                for(e =0; e<24;e++){
                                                        fprintf(output,"Hour %d: Trips: %d\n",e,array[e]);
                                                }
                                                fprintf(output,"Average duration of trips in minutes: %.2lf\n",(double)(averageDuration(trips,lines))/60);                                                              fprintf(output,"Average duration of trips in milliseconds: %.0lf\n",(averageDuration(trips,lines)*1000));
                                                fprintf(output,"The longest trip was %d milliseconds.\n",longestTrip(trips,lines));
                                                fprintf(output,"The longest trip was %d minutes.\n",(longestTrip(trips,lines)/6000));
                                                fiveBusiestStart(trips,lines,startStations);
                                                for(b=0; b<5;b++){                                                                                                                                                              fprintf(output,"Station: %d Station Number: %d Trip Count: %d\n",(b+1),                                                                                                                                                                 (startStations[b].id),                                                                                                                                                  (startStations[b].tripCount));                                                                     }                                                                                                                                                                    fiveBusiestEnd(trips,lines,endStations);                                                                                                                                for(b=0; b<5;b++){                                                                                                                                                              fprintf(output,"Station: %d Station Number: %d Trip Count: %d\n",(b+1),
                                                                                                                        (endStations[b].id),
                                                                                                                        (endStations[b].tripCount));                                                                    }
                                                fprintf(output,"This is the percent of riders who are members: %.2lf\n",membershipPercent(trips,lines));
                                                longestBikes(bikes,topTen);
                                                for(bikesInt=0;bikesInt<10;bikesInt++){                                                                                                                                         fprintf(output,"Bike number: %d duration: %.2lf\n",(topTen[bikesInt].bikeId),
                                                                                                                   (topTen[bikesInt].totalMinutes));                                                                    }                                                                                                                                                                       for(b=0;b<10;b++){
                                                	fprintf(output,"Bike number: %d Last Station: %d\n",topTen[b].bikeId,topTen[b].endStationId);
                                                }
                                                tripCountBikes(bikes,topTen);
                                                for(b=0;b<10;b++){                                                                                                                                                             fprintf(output,"Bike number: %d Number of Trips: %d\n",topTen[b].bikeId,topTen[b].numTrips);                                                                     }
                                                fprintf(output,"The amount of bikes with 2 or less trips is: %d.\n",twoLess(bikes,&currentLine));
						break;
					case 'H':/*single bike */
						printf("Please enter the ID for the bike you want to print.\n");
						int bikePrintId = 0;
						fgets(inBuf,sizeof(inBuf),stdin);
						sscanf(inBuf,"%d",&bikePrintId);
						int placement = findBike(bikes,bikePrintId,&currentLine);
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							printf("Bike ID: %d Maintenance Flag: %c Number of trips: %d \n",(bikes[placement].bikeId),
															 (bikes[placement].maintenanceFlag),
															 (bikes[placement].numTrips));
							printf("Total Duration: %d Total Minutes %lf\n",(bikes[placement].totalDuration),
												       (bikes[placement].totalMinutes)); 
						}
						else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							fprintf(output,"Bike ID: %d Maintenance Flag: %c Number of trips: %d \n",(bikes[placement].bikeId),
                                                                                                                         (bikes[placement].maintenanceFlag),
                                                                                                                         (bikes[placement].numTrips));
							fprintf(output,"Total Duration: %d Total Minutes %lf\n",(bikes[placement].totalDuration),
                                                                                                       (bikes[placement].totalMinutes));
							fclose(output);
						}
						else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;
					/*CEO metrics */
					case 'I':/*10 longest bikes in minutes */
						longestBikes(bikes,topTen);
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							for(bikesInt=0;bikesInt<10;bikesInt++){
								printf("Bike number: %d duration: %.2lf\n",topTen[bikesInt].bikeId,topTen[bikesInt].totalMinutes);
							}	
						}
						else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							for(bikesInt = 0; bikesInt<10;bikesInt++){
								fprintf(output,"Bike number: %d duration: %.2lf\n",(topTen[bikesInt].bikeId),
														   (topTen[bikesInt].totalMinutes));
							}
							fclose(output);
				
						}
						else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;
					case 'J':/* location of 10 bikes */
						longestBikes(bikes,topTen);
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
                                                        for(b=0;b<10;b++){
                                                                printf("Bike number: %d Last Station: %d\n",topTen[b].bikeId,topTen[b].endStationId);
                                                        }
                                                }
                                                else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
                                                        for(b = 0; b<10;b++){
                                                                fprintf(output,"Bike number: %d Last Station: %d\n",topTen[b].bikeId,topTen[b].endStationId);
                                                        }
							fclose(output);
                                                }
                                                else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;
					case 'K':/*10 bikes with most trips */
						tripCountBikes(bikes,topTen);
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							for(b=0;b<10;b++){
								printf("Bike number: %d Number of Trips: %d\n",topTen[b].bikeId,topTen[b].numTrips);
							}
						}
						else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							for(b=0;b<10;b++){
								fprintf(output,"Bike number: %d Number of Trips: %d\n",topTen[b].bikeId,topTen[b].numTrips);
							}
							fclose(output);
						}
						 else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;
					case 'L':/* how many bikes <=2 trips */
						printf("Enter 1 for this information to display on screen or 2 for this info to be written to a file.\n");
                                                printChoice = 0;                                                                                                                                                        fgets(inBuf,sizeof(inBuf),stdin);                                                                                                                                       sscanf(inBuf,"%d",&printChoice);                                                                                                                                        if(printChoice == 1){
							printf("The amount of bikes with 2 or less trips is: %d.\n",twoLess(bikes,&currentLine));
						}
						else if(printChoice ==2){
                                                        printf("Please enter your filename for output. Max 100 chars.\n");
                                                        char outputFile[100];
                                                        fgets(inBuf,sizeof(inBuf),stdin);
                                                        sscanf(inBuf,"%s",outputFile);
                                                        FILE *output = fopen(outputFile,"w");
							fprintf(output,"The amount of bikes with 2 or less trips is: %d.\n",twoLess(bikes,&currentLine));
						}
						else{
                                                        printf("Not a valid option. Returning.\n");
                                                }
                                                mainMenu();
                                                break;	
					case 'M':/* <=2 bikes, startID, Number of trips */
						printf("Current under construction.\n");
						break;
					default:
						printf("Invalid character. Remember you can use 'Q' to quit.\n");
						break;
				}
			}
			/* While loop ends */
			free(trips);
			free(bikes);
		}
		else if(menuChoice == 'Q'){
			printf("Quitting.\n");
			exit(0);
		}
		else {
			printf("Can not continue without adding trip data\n");
		}
	}
	return 0;
}
/*Trip functions*/
double averageTripsPerHour(int lines){
	/*Average trips per hour */
	return (double)lines/24;	
}
int hourMostTrips(Trip *trip,int lines){
	/* Finding the hour with the most trips*/
	int hours[24];
	int largest = -1;
	int i;
	int finalHour;
	tripsPerEachHour(trip,lines,hours);
	for(i=0;i<24;i++){
		if(hours[i] > largest){
			largest = hours[i];
			finalHour = i;
		}
	}
	return finalHour;
}
int tripsPerEachHour(Trip *trip,int lines, int *array){
	/* amount of trips taken in each hour, needs to have an array input */
	int *hours = array;
        int i;
        for(i=0;i<24;i++){
                hours[i]=0;
        }
        for(i=0;i<lines;i++){
                int x = trip[i].startHr;
                hours[x]++;
        }
	return 0;
}
double averageDuration(Trip *trip,int lines){
	/*given milliseconds, need to convert to seconds */
	int i;
	double avgDur;
	for(i=0;i<lines;i++){
		avgDur += (trip[i].duration/1000); /* converts milliseconds to seconds*/
	}
	avgDur = avgDur/lines; /* computes average */
	/*Returns average seconds. For milliseconds multiply by 1000 */
	return avgDur;
}
int longestTrip(Trip *trip, int lines){
	qsort(trip,lines,sizeof(trip[0]),compareDuration);
	return trip[0].duration;
}
void fiveBusiestStart(Trip *trip, int lines, Station *stations){
	/* Stations should = 5 */
	/*Return the 5 busiest start stations */
	int i;
	int j;
	/*loop control above, info control below */
	int x = 0;
	int y= 0;
	int tripCount = 0;
	for(i=0;i<5;i++){
		stations[i].tripCount = 0;
		stations[i].id = 0;
	}
	qsort(trip,lines,sizeof(trip[0]),compareStartS);
	for(i = 0; i<lines; i++){
		x = trip[i].startStationId;
		if(x == y){
			tripCount++;	
		}
		else{
			for(j=4;j>=0;j--){
				if(tripCount > stations[j].tripCount){
					stations[j].tripCount = tripCount;
					stations[j].id = y;
					j=-1;
				}	
			}
			y=x;
			tripCount=1;
		}
	}
}
void fiveBusiestEnd(Trip *trip,int lines, Station *stations){
	/* Stations should = 5 */
        /*Return the 5 busiest end stations */
        int i;
        int j;
        /*loop control above, info control below */
        int x = 0;
        int y= 0;
        int tripCount;
	for(i=0;i<5;i++){
                stations[i].tripCount = 0;
                stations[i].id = 0;
        }
        qsort(trip,lines,sizeof(trip[0]),compareEndS);
        for(i = 0; i<lines; i++){
                x = trip[i].endStationId;
                if(x == y){
                        tripCount++;
                }
                else{
                        for(j=4;j>=0;j--){
                                if(tripCount > stations[j].tripCount){
                                        stations[j].tripCount = tripCount;
                                        stations[j].id = y;
					j=-1;
                                }
                        }
                        y=x;
                        tripCount=1;
                }
       }
}
double membershipPercent(Trip *trip, int lines){
	int i;
	int members = 0;
	for(i = 0;i<lines;i++){
		if(trip[i].membershipType[0]=='R'){
			members++;
		}
	}
	/* members/lines give us a decimal .xxxx *100 to get xx.xx*/
	return ((double)members/lines)*100;
}
/* Bike functions */
void longestBikes(Bike *bike,Bike *topTen){
	/* 10 bikes with most miles, flag for maintenance */
	int i;
	qsort(bike,4000,sizeof(Bike),compareMinutes);
	for(i = 0; i<10;i++){
		bike[i].maintenanceFlag = '1';
		topTen[i].bikeId = bike[i].bikeId;
		topTen[i].endStationId = bike[i].endStationId;
		topTen[i].totalMinutes = bike[i].totalMinutes;
	}
}
void tripCountBikes(Bike *bike, Bike *topTen){
	/* 10 bikes with the highest trip count */
	int i;
	qsort(bike,4000,sizeof(Bike),compareTripCount);
	for(i = 0;i<10;i++){
		topTen[i].bikeId = bike[i].bikeId;
		topTen[i].numTrips = bike[i].numTrips;
	}
}
int twoLess(Bike *bike,int *currentLine){
	int i;
	int count;
	for(i=0;i<*currentLine;i++){
		if(bike[i].numTrips <= 2){
			count++;
		}
	}
	return count;
}
void addBike(Bike *bike,int *currentLine,int bikeId,int station){
	bike[*currentLine].bikeId = bikeId;
	bike[*currentLine].totalDuration = 0;
	bike[*currentLine].totalMinutes = 0;
	bike[*currentLine].numTrips = 0;
	bike[*currentLine].endStationId = station;
	*currentLine++;
}
void removeBike(Bike *bike,int *currentLine,int bikeId){
	int i;
	int x;
	for(i=0;i<*currentLine;i++){
		if(bike[i].bikeId == bikeId){
			x=i;
		}	
	}
	for(i=x-1;i<*currentLine-1;i++){
		bike[i] = bike[i+1];
	}
	*currentLine--;
}
int findBike(Bike *bike, int bikeId, int *currentLine){
	int i;
        int x;
        for(i=0;i<*currentLine;i++){
                if(bike[i].bikeId == bikeId){
                        x=i;
                }
        }
	return x;
}
/*Compare functions */
int compareBikeId(const void *a, const void *b){
        Trip *tripCompareA = (Trip *)a;
        Trip *tripCompareB = (Trip *)b;
        return (tripCompareB->bikeId - tripCompareA->bikeId);
}
int bikeCompareBikeId(const void *a, const void *b){
        Bike *bikeCompareA = (Bike *)a;
        Bike *bikeCompareB = (Bike *)b;
        return (bikeCompareB->bikeId - bikeCompareA->bikeId);
}
int compareStartS(const void *a,const void *b){
        Trip *tripCompareA = (Trip *)a;
        Trip *tripCompareB = (Trip *)b;
        return (tripCompareB->startStationId - tripCompareA->startStationId);
}
int compareEndS(const void *a,const void *b){
        Trip *tripCompareA = (Trip *)a;
        Trip *tripCompareB = (Trip *)b;
        return (tripCompareB->endStationId - tripCompareA->endStationId);
}
int compareDuration(const void *a,const void *b){
        Trip *tripCompareA = (Trip *)a;
        Trip *tripCompareB = (Trip *)b;
        return (tripCompareB->duration - tripCompareA->duration);
}
int compareStartHr(const void *a,const void *b){
        Trip *tripCompareA = (Trip *)a;
        Trip *tripCompareB = (Trip *)b;
        return (tripCompareB->startHr - tripCompareA->startHr);
}
int compareStartMin(const void *a,const void *b){
        Trip *tripCompareA = (Trip *)a;
        Trip *tripCompareB = (Trip *)b;
        return (tripCompareB->startMin - tripCompareA->startMin);
}
int compareMinutes(const void *a,const void *b){
	Bike *bikeCompareA = (Bike *)a;
	Bike *bikeCompareB = (Bike *)b;
	return (bikeCompareB->totalMinutes - bikeCompareA->totalMinutes);
}
int compareTripCount(const void *a,const void *b){
        Bike *bikeCompareA = (Bike *)a;
        Bike *bikeCompareB = (Bike *)b;
        return (bikeCompareB->numTrips - bikeCompareA->numTrips);
}
/*Menus functions*/
void mainMenu(){
        printf("Welcome to the main menu.\n");
        printf("Read trip data:                '1'\n");
        printf("Metrics menu:                  '2'\n");
        printf("Print menu:                    '3'\n");
        printf("Flag bike for maintence:       '4'\n");
        printf("Update bike inventory:         '5'\n");
        printf("Quit:                          'Q'\n");
}
void metricsMenu(){
        printf("Welcome to the metrics menu.\n");
        printf("Average trips per hour:             '6'\n");
        printf("Hour with most trips:               '7'\n");
        printf("Report: number of trips each hour   '8'\n");
        printf("Average duration of trips:          '9'\n");
        printf("Longest trip:                       'A'\n");
        printf("5 busiest starting stations:        'B'\n");
        printf("5 busiest ending stations:          'C'\n");
        printf("Membership trip percentage:         'D'\n");
	printf("10 Bikes with the longest ride time:'I'\n");
	printf("Location of longest ride Bikes:	    'J'\n");
	printf("10 Bikes with the most trips:	    'K'\n");
	printf("Bikes with 2 or less trips:	    'L'\n");
	printf("Start Station IDs, and Trip Count\n");
	printf("for bikes with 2 or less trips:     'M'\n");
        printf("Return to main menu:                'R'\n");
}
void printMenu(){
        printf("Welcome to the print menu.\n");
        printf("Print all trip data:    'E'\n");
        printf("Print bike inventory:   'F'\n");
        printf("Print CEO report:       'G'\n");
        printf("Print one bike:         'H'\n");
        printf("Return to main menu:    'R'\n");
}
