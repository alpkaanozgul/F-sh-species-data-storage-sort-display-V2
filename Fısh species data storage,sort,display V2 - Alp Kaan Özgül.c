// Alp Kaan Özgül 2638096


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//defined structures
struct fishnames{
    char fishname[100];
    int size;
    struct fishnames* next;
    struct fishproperties* data; // pointer points to fishprperties

};

struct fishproperties{
    float weight;
    float verlength;
    float diolength;
    float crosslength;
    float height;
    float length;
    int day;
    int month;
    int year;
    char city[100];
    struct fishproperties* next;

};
//function prototypes
struct fishnames* createfishname();
struct fishproperties* fishdata(struct fishnames* created);
void saveUpdatedList(struct fishnames* rewritedata);
void AddSpeciesList(struct fishnames* fishnames);
void addFishData (struct fishnames* datafishadd);
void searchFishData (struct fishnames* datasearch);
void printStatistic (struct fishnames* datastatic);
void deleteFish(struct fishnames** delete, float threshold);



struct fishnames* initializeFishing(char filename[100]);

int main() {
    //main function to control through program
    char filename[100] = "fishingArchive.txt";
    struct fishnames* headrecieved;
    int x=1,choise;
    float threshold;

    headrecieved = initializeFishing(filename); //here is initalized the program from fishingArchive.txt
    if (headrecieved == NULL) {
        printf("Error initializing fishing data.\n");
        return 1;
    }
    printf("-----MENU---------------------------------------\n");
    printf("1. Add Fish Data\n"
           "2. Delete Fish Data\n"
           "3. Print Fish Statistics\n"
           "4. Search Fish Data\n"
           "5. Add Species List\n"
           "6. Exit\n\n");
    printf("Enter your option: ");
    scanf("%d",&choise);
    while (choise != 6){
        if (choise==1)
        {
            addFishData(headrecieved->next);
        }
        else if(choise==2)
        {
            printf("Provide fish weight threshold in grams:");
            scanf("%f",&threshold);
            deleteFish(&(headrecieved->next),threshold);
        }
        else if(choise==3)
        {

            printStatistic(headrecieved->next);


        }
        else if(choise==4)
        {
            searchFishData(headrecieved->next);
        }
        else if(choise==5)
        {
            AddSpeciesList(headrecieved);
        }


        printf("-----MENU---------------------------------------\n");
        printf("1. Add Fish Data\n"
               "2. Delete Fish Data\n"
               "3. Print Fish Statistics\n"
               "4. Search Fish Data\n"
               "5. Add Species List\n"
               "6. Exit\n\n");
        printf("Enter your option: ");
        scanf("%d",&choise);


    }

    saveUpdatedList(headrecieved->next); //overwrite the txt file at the exit




    return 0;
}



struct fishnames* initializeFishing(char filename[100]){

    FILE* filefish = fopen(filename, "r"); // opening file in read mode to store the data

    if (filefish == NULL) {
        printf("The file couldn't be found.\n");
        return NULL; // Return NULL if file not found
    } else {
        printf("The %s file has been loaded successfully!\n", filename);
    }



    struct fishnames* head;
    struct fishnames* tail;
    struct fishnames* tmp;  //initializing the pointers in order to manupilate the linked list





    head = createfishname(); //initializes the beginning of the linked list

    head->data = fishdata(head);
    tail = head;
    char line[200];

    while (fgets(line, sizeof(line), filefish) != NULL){
        tmp = createfishname();                                                 //in this while loop it it stores to fishnames and data
                                                                                // and itterates through txt file and if same fishname is found it addes a new node to the fishproperties linked list
        tmp->data = fishdata(tmp);

        sscanf(line, "%[^;];%f;%f;%f;%f;%f;%f;%d/%d/%d;%[^\n]",
               tmp->fishname, &tmp->data->weight, &tmp->data->verlength, &tmp->data->diolength,
               &tmp->data->crosslength, &tmp->data->height, &tmp->data->length,
               &tmp->data->day, &tmp->data->month, &tmp->data->year, tmp->data->city);
        if (strcmp(tail->fishname, tmp->fishname) == 0) {
            tail->data->next=tmp->data;

        } else {
            tail->next = tmp;
            tail = tmp;

        }



    }

    fclose(filefish);

    if(head->next==NULL){

        AddSpeciesList(head);               //if the list is empty head of the linked list is passed to the function
    }

    return head;
}

struct fishnames* createfishname(){
    struct fishnames* l = (struct fishnames*)malloc(sizeof (struct fishnames));
    if (l == NULL)
        printf("Out of memory!\n");                 //allocates memory for the node creation for the initilazation of the linked list fishnames
    l->next=NULL;


    return l;

}

struct fishproperties* fishdata(struct fishnames* created){         //allocates memory for the node creation for the initilazation of the nested linked list of fishproperties
    struct fishproperties* data = created->data;
    data = (struct fishproperties*) malloc(sizeof (struct fishproperties));
    data->next=NULL;
    return data;
}

void saveUpdatedList(struct fishnames* rewritedata) {  //this function in the exit overwrites the fishingArchive.txt with newly linked list's
    struct fishnames* tmp =rewritedata;

    FILE *outFile;
    outFile= fopen("fishingArchive.txt","w");
    if (outFile==NULL){
        printf("Fail to open file");
        exit(1);
    }


    for (; tmp != NULL; tmp = tmp->next) {  //this for loop prints the linked list and nested linked list as the format of the txt file

        struct fishproperties *dataTmp = tmp->data;

        for (; dataTmp != NULL; dataTmp = dataTmp->next) {
            fprintf(outFile, "%s;%.2f;%.2f;%.2f;%.2f;%.4f;%.4f;%02d/%02d/%04d;%s\n",
                    tmp->fishname, dataTmp->weight, dataTmp->verlength,
                    dataTmp->diolength, dataTmp->crosslength, dataTmp->height,          //if there is nested linked list structure it prints as the format as below the previous node
                    dataTmp->length, dataTmp->day, dataTmp->month,
                    dataTmp->year, dataTmp->city);
        }
    }

    fclose(outFile);

    printf("The FishingArchive.txt file has been updated successfully!!");
}

void AddSpeciesList(struct fishnames* fishnames){ // in this function it addes new fish specie data and populates that fish specie with the entered data's
    struct fishnames* addnode = (struct fishnames*) malloc(sizeof (struct fishnames));
    struct fishnames* headaddnode;
    struct fishnames* headaddnode2 = fishnames;
    int checker=0,checkerpos1=0;
    float weight,verlen,dialen,crosslen,height,fishlength;
    int dateday,datemonth,dateyear;
    char speciename[100],city[100];

    if (headaddnode2->next ==NULL) // in this if condition it add new fishname to the linked list at the end and gets the name of it
    {
        struct fishnames* addnode1 = (struct fishnames*) malloc(sizeof (struct fishnames));
        addnode1->data=NULL;
        addnode1->next=NULL;
        headaddnode2->next= addnode1;
        char newspecies[100];
        printf("Enter the new species: ");
        scanf("%s",newspecies);
        strcpy(addnode1->fishname, newspecies);
    }


    else{ // int this else statement it finds the correct added node

        headaddnode = fishnames->next;

        while( headaddnode->next!=NULL)
        {
            headaddnode= headaddnode->next;
        }
        addnode->data=NULL;
        addnode->next=NULL;
        headaddnode->next= addnode;
        char newspecies[100];
        printf("Enter the new species: ");
        scanf("%s",newspecies);



        strcpy(addnode->fishname, newspecies);
    }

    printf("Provide the following info:");

    struct fishproperties* addnode1 = (struct fishproperties*) malloc(sizeof (struct fishproperties));
    addnode->data= addnode1; //addes fishproperties node to newly created fishspecies

    addnode->data->next = NULL;

    //asks for data for the newly added fishdata node

    printf("Weight of the fish in grams: ");
    scanf("%f", &weight);
    addnode1->weight = weight;

    printf("Vertical length in CM: ");
    scanf("%f", &verlen);
    addnode1->verlength = verlen;

    printf("Diagonal length in CM: ");
    scanf("%f", &dialen);
    addnode1->diolength = dialen;

    printf("Cross length in CM: ");
    scanf("%f", &crosslen);
    addnode1->crosslength = crosslen;

    printf("Height in CM: ");
    scanf("%f", &height);
    addnode1->height = height;

    printf("Fish Length in CM: ");
    scanf("%f", &fishlength);
    addnode1->length = fishlength;

    printf("Fishing date: ");
    scanf("%d/%d/%d", &dateday, &datemonth, &dateyear);
    addnode1->day = dateday;
    addnode1->month = datemonth;
    addnode1->year = dateyear;

    printf("City: ");
    scanf("%s", city);
    strcpy(addnode1->city, city);






}


void addFishData (struct fishnames* datafishadd){ //this function addes fish properties to existing fishes
    struct fishnames* tmp= datafishadd;
    struct fishnames* tmp1= datafishadd;

    int checker=0,checkerpos1=0;
    float weight,verlen,dialen,crosslen,height,fishlength;
    int dateday,datemonth,dateyear;
    char speciename[100],city[100];
    printf("Specie: ");
    scanf("%s",speciename);

    int counter =0;


    while(tmp !=NULL)
    {
        if(strcmp(speciename,(tmp->fishname))==0)
        {
            checkerpos1=1;
            break;
        }
        counter++;
        tmp = tmp->next;                    //traverses through linked list to find the correct one
    }
    struct fishproperties* addnode = (struct fishproperties*) malloc(sizeof (struct fishproperties));


    if (checkerpos1 == 1 && tmp->data != NULL) {            //this if statement addes the fish data to the traversed linked list
        addnode->next = NULL;
        if (tmp->data->next != NULL) {
            struct fishproperties* last = tmp->data->next;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = addnode;
        }
        else {
            tmp->data->next = addnode;
        }

        printf("\n prev known fish\n");
        printf("Weight of the fish in grams: ");
        scanf("%f", &weight);
        addnode->weight = weight;

        printf("Vertical length in CM: ");
        scanf("%f", &verlen);
        addnode->verlength = verlen;

        printf("Diagonal length in CM: ");
        scanf("%f", &dialen);
        addnode->diolength = dialen;

        printf("Cross length in CM: ");
        scanf("%f", &crosslen);
        addnode->crosslength = crosslen;

        printf("Height in CM: ");
        scanf("%f", &height);
        addnode->height = height;

        printf("Fish Length in CM: ");
        scanf("%f", &fishlength);
        addnode->length = fishlength;

        printf("Fishing date: ");
        scanf("%d/%d/%d", &dateday, &datemonth, &dateyear);
        addnode->day = dateday;
        addnode->month = datemonth;
        addnode->year = dateyear;

        printf("City: ");
        scanf("%s", city);
        strcpy(addnode->city, city);
    }
    else {                          //this else statements addes fish data if there isnt fishproperties node for that fishname node
        printf("\n newly added fish\n");

        addnode->next = NULL;

        if (tmp->data == NULL) {
            tmp->data = addnode;
        }

        printf("Weight of the fish in grams: ");
        scanf("%f", &weight);
        addnode->weight = weight;

        printf("Vertical length in CM: ");
        scanf("%f", &verlen);
        addnode->verlength = verlen;

        printf("Diagonal length in CM: ");
        scanf("%f", &dialen);
        addnode->diolength = dialen;

        printf("Cross length in CM: ");
        scanf("%f", &crosslen);
        addnode->crosslength = crosslen;

        printf("Height in CM: ");
        scanf("%f", &height);
        addnode->height = height;

        printf("Fish Length in CM: ");
        scanf("%f", &fishlength);
        addnode->length = fishlength;

        printf("Fishing date: ");
        scanf("%d/%d/%d", &dateday, &datemonth, &dateyear);
        addnode->day = dateday;
        addnode->month = datemonth;
        addnode->year = dateyear;

        printf("City: ");
        scanf("%s", city);
        strcpy(addnode->city, city);
    }


    printf("\nThe data has been added, successfully!\n");
}

void searchFishData (struct fishnames* datasearch){  //this function searches and prints the fishes which are in desired search catagory
    char option;
    int month;
    char city[100];
    fflush(stdin);
    printf("Enter your search option (C for city/M for month): ");
    scanf(" %c", &option);

    if (option == 'c' || option == 'C') {
        fflush(stdin);
        printf("Enter the city: ");
        scanf(" %[^\n]", city);
        struct fishnames* tmp1 = datasearch;
        while(tmp1 != NULL) {               //traverses the linked list and prints the searched fishes for city
            if (strcmp(tmp1->data->city, city) == 0) {
                printf("\n%s;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2d/%.2d/%d;%s\n",
                       tmp1->fishname, tmp1->data->weight, tmp1->data->verlength,
                       tmp1->data->diolength, tmp1->data->crosslength, tmp1->data->height,
                       tmp1->data->length, tmp1->data->day, tmp1->data->month,
                       tmp1->data->year, tmp1->data->city);
            }
            tmp1 = tmp1->next;
        }
        struct fishnames* tmp2 = datasearch;
        while(tmp2!=NULL)           //traverses the nested linked list and prints the searched fishes for city
        {
            if(tmp2->data->next !=NULL )
            {

                struct fishproperties* nested = tmp2->data->next;
                while (nested != NULL)
                {
                    if (strcmp(nested->city, city) == 0) {
                        printf("\n%s;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2d/%.2d/%d;%s\n",
                               tmp2->fishname, nested->weight, nested->verlength,
                               nested->diolength, nested->crosslength, nested->height,
                               nested->length, nested->day, nested->month,
                               nested->year, nested->city);
                    }

                    nested = nested->next;
                }
            }

            tmp2=tmp2->next;
        }
    }
    else if (option == 'm' || option == 'M') {
        printf("Enter the month number: ");
        scanf("%d", &month);
        while (!(month >= 1 && month <= 12)) {
            printf("You entered the wrong number!!");
            scanf("%d", &month);
        }
        struct fishnames* tmp = datasearch;
        while(tmp != NULL) { //traverses the linked list and prints the searched fishes for month
            if (tmp->data->month == month) {
                printf("\n%s;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2d/%.2d/%d;%s\n",
                       tmp->fishname, tmp->data->weight, tmp->data->verlength,
                       tmp->data->diolength, tmp->data->crosslength, tmp->data->height,
                       tmp->data->length, tmp->data->day, tmp->data->month,
                       tmp->data->year, tmp->data->city);
            }
            tmp = tmp->next;
        }
        struct fishnames* tmp3 = datasearch;
        while(tmp3!=NULL) //traverses the nested linked list and prints the searched fishes for month
        {
            if(tmp3->data->next !=NULL )
            {

                struct fishproperties* nested1 = tmp3->data->next;
                while (nested1 != NULL)
                {
                    if (nested1->month == month) {
                        printf("\n%s;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2d/%.2d/%d;%s\n",
                               tmp3->fishname, nested1->weight, nested1->verlength,
                               nested1->diolength, nested1->crosslength, nested1->height,
                               nested1->length, nested1->day, nested1->month,
                               nested1->year, nested1->city);
                    }

                    nested1 = nested1->next;
                }
            }

            tmp3=tmp3->next;
        }
    }
}

void printStatistic (struct fishnames* datastatic){ //this function prints the amount of the nested nodes for data in the linked list structure
    char specie [100];
    printf("Provide the species: ");
    scanf("%s",specie);
    int counter = 1,counter1=0,counter2= 0,counter3=0,counter4 = 0;
    struct fishnames* tmp = datastatic;
    struct fishnames* tmp3 = datastatic;

    while(tmp3 != NULL)
    {
        tmp3 = tmp3->next;
        counter3++;
    }                   //for fish data 0


    while(tmp != NULL)
    {
        if (strcmp(specie,tmp->fishname)==0)
        {
            break;
        }
        else{
            counter4++;
        }
        tmp = tmp->next;
        counter1++;
    }               //for fish data 0

    if (counter3==counter4  )
    {
        printf("\nThe number of available fish data is: 0\n");
        return;
    }


    struct fishnames* tmp2 = tmp;

    struct fishproperties* nested = tmp2->data;

    while(nested!=NULL)
    {
        counter2++;             //travesres through nested linked list nodes in order to determine how many nodes there are inorder to show amount of data
        nested=nested->next;
    }

    printf("\nThe number of available fish data is: %d\n",counter2);
}


void deleteFish(struct fishnames** head, float threshold) { //in this function it deletes desired fish below the threshold by traversing through linked and nested linked list
    struct fishnames* current = *head;
    struct fishnames* prev = NULL;

    while (current != NULL) {
        struct fishproperties* fishData = current->data;
        struct fishproperties* prevFishData = NULL;

        while (fishData != NULL) {  //traverses through linked list and deletes the fish data from nested linked list
            if (fishData->weight < threshold) {
                if (prevFishData == NULL) {
                    current->data = fishData->next;
                    free(fishData);
                    fishData = current->data;
                } else {
                    prevFishData->next = fishData->next;
                    free(fishData);
                    fishData = prevFishData->next;

                }
            } else {
                prevFishData = fishData;
                fishData = fishData->next;
            }
        }

        if (current->data == NULL) {        //this statement deletes if the fishname if there is no data left hence deletes the fish specie because no data left
            if (prev == NULL) {
                *head = current->next;
                free(current);
                current = *head;

            } else {
                prev->next = current->next;
                free(current);
                current = prev->next;
            }

        } else {
            prev = current;
            current = current->next;
        }
    }
}






