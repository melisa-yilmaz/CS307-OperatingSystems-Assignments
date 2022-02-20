// Melisa Yılmaz 26777
// Date: 8.01.2022
// Programming Assignment 5
// File Systems


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>
#include <stdlib.h>

typedef struct Person
{
    char gender[10], name[20], lastName[20];
} Person;
int length = 0;
Person people[];
//first of all find the length of the database.txt in the root directory to create array of struct from that databa.txt
int findingLengthOfDatabase()
{
    FILE* database = fopen("./database.txt", "r");
    if (database == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char buffer[256];
    fgets(buffer, 256, database);
    int len = 0;
    while(!feof(database))
    {
        len++;
        fgets(buffer, 256, database);
    }

   
    fclose(database);
    length = len;
    return len;
}

//read all the .txt files and if there is mistake, update them
void readFiles(const char *directory)
{
    FILE *txt = fopen(directory,"r+");
    if(txt == NULL)
        return;
   
    char chr;
    chr = fgetc(txt);
    char word[200] = "";
 
    while(!feof(txt))
    {
        // if the chr is equal to the ' ' check it whether it is the name in the database.txt
        if(chr == ' ')
        {
        
            for(int i = 0; i < length; i++)
            {
                //if word is in the givn database, correct all the mistakes in the file
                if(strcmp(word, people[i].name) == 0)
                {
                    char *gender = people[i].gender;
                    char *name = people[i].name;
                    char *lastName = people[i].lastName;
                    
                    
                    long nameLen = strlen(name)+1; long genderLen = strlen(gender)+1;long lastNameLen = strlen(lastName)+1;
                    fseek(txt, -(nameLen + genderLen), SEEK_CUR);
                    fputs(gender,txt);
                    

                    fseek ( txt , (nameLen + 1), SEEK_CUR);
                    fputs(lastName,txt);

                }
            }
            
            strcpy(word,"");
            
        }
           
        else {
            strncat(word, &chr,1);
         
        }
        chr =fgetc(txt);
        
    }
    fclose(txt);
}

//list all the files and sub-directories in given directory
void insideRoot(const char* directoryName)
{
    DIR* root = opendir(directoryName);
    if(root == NULL)
        return;
    
    struct dirent* entity;
    entity = readdir(root);
    while(entity != NULL)
    {
        //printf( "entity name is: %s/%s\n", directoryName, entity->d_name);
        if (strstr(entity->d_name , ".txt") )
        {
            if(strcmp(directoryName,".") == 0)
            {
                if(strcmp(entity->d_name, "database.txt") != 0)
                {
                    printf( "   search into name is:%s/%s\n",directoryName, entity->d_name);
                    char directory[256] = "";
                    strcat(directory, directoryName);
                    strcat(directory, "/");
                    strcat(directory, entity->d_name);
                    readFiles(directory);
                }
            }
            else
            {
                printf( "   search into name is:%s/%s\n",directoryName, entity->d_name);
                char directory[256] = "";
                strcat(directory, directoryName);
                strcat(directory, "/");
                strcat(directory, entity->d_name);
                readFiles(directory);
            }
         
            
        }
        if(entity->d_type == DT_DIR && strcmp(entity->d_name,".") != 0 && strcmp(entity->d_name,"..") != 0)
        {
           // printf( "   directory name is:%s/%s\n",directoryName, entity->d_name);
            char path[100] = {0};
            strcat(path, directoryName);
            strcat(path, "/");
            strcat(path, entity->d_name);
            insideRoot(path);
        }
        entity = readdir(root);
    }
    
    closedir(root);
}
int main(int argc, const char * argv[]) {
    // count the length of the database
    int len = findingLengthOfDatabase();
    
    //accordng to that length create array of struct
    FILE* fp = fopen("./database.txt", "r");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    char line[256];
    fgets(line, 256, fp);
    
    people[length];
    int i = 0;
    while(!feof(fp))
    {
        Person* p = people + i;
        //put all the name to array of struct
        sscanf(line,"%s %s %s ", p->gender, p->name, p->lastName);
        
        char *updateGender = "Mr.";
        
        if(strcmp(p->gender, "f") == 0)
        {
            updateGender = "Ms.";
        }
        strcpy(p->gender, updateGender);
        fgets(line, 256, fp);
        i++;
    }
    i++;
    fclose(fp);
    
    insideRoot(".");
    return 0;
}
