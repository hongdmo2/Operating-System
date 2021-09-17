#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
RQ = request 
RL = release
STAT = STATUS
QUIT = END PROGRAM
C = Compact
*/
FILE * pFile;
int Total_memory;

struct Process
{
    int index;
    int start;
    int limit;
    int size;
};

struct Process search_hole(struct Process p[]){//this only search and return ,not modify.
    struct Process h;
    int count = 0;
    if(p[count].index == -1){
        h.start = 0;
        h.limit = Total_memory;
        h.size = Total_memory;
        return h;
    }
    
    if(p[count].start == 0){
        h.start = 0;
        h.limit = 0;
        h.size = h.limit - h.start;
    }
    else{
        h.start = 0;
        h.limit = p[0].start -1;
        h.size = h.limit - h.start;
    }
    if(p[count+1].index == -1){
        if(Total_memory - p[count].limit > h.size){
            h.start = p[count].limit+1;
            h.limit = Total_memory;
        }
        return h;
    }
    count++;
    while(p[count].index != -1){
        if(p[count].start - p[count-1].limit > h.size){
            h.start = p[count-1].limit + 1;
            h.limit = p[count].start -1;
            h.size = h.limit - h.start;
        }
        if(p[count+1].index == -1 && Total_memory - p[count].limit > h.size){
            h.start = p[count].limit + 1;
            h.limit = Total_memory;
            h.size = h.limit - h.start;
        }
        count++;
    }
    return h;

}
void printArray(struct Process arr[]){ //Priting function. 
    if(arr[0].index == -1){
        printf("Addresses [0 : %d] Free\n", Total_memory);
        fprintf(pFile, "Addresses [0 : %d] Free\n", Total_memory);
    }
    if(arr[0].start != 0){
        printf("Addresses [0 : %d] Free\n", arr[0].start -1);
        fprintf(pFile, "Addresses [0 : %d] Free\n", arr[0].start -1);
    }
    int count = 0;
    while(arr[count].index != -1){
        if(count > 0 && arr[count].start - arr[count-1].limit > 1){
            printf("Addresses [%d - %d] Free \n", arr[count-1].limit +1, arr[count].start -1);
            fprintf(pFile, "Addresses [%d - %d] Free \n", arr[count-1].limit +1, arr[count].start -1);
        }
        printf("Addresses [%d : %d] Process %d\n",  arr[count].start, arr[count].limit, arr[count].index);
        fprintf(pFile, "Addresses [%d : %d] Process %d\n",  arr[count].start, arr[count].limit, arr[count].index);
        count++;
        if(arr[count].index == -1 && arr[count-1].limit != Total_memory){
            printf("Addresses [%d : END] Free\n", arr[count-1].limit + 1);
            fprintf(pFile, "Addresses [%d : END] Free\n", arr[count-1].limit + 1);
        }
    }
}
int main(int argc, char * argv[]){
    
    Total_memory = atoi(argv[1]);
    int length = 10;
    int counter=0;
    struct Process process_array[length];
    for(int i=0;i<length;i++){
        process_array[i].index = -1;
    }
    int buffer = 128;
    char user_input[buffer], *token;
    const char s[1] = " ";
   
    pFile = fopen("HChangoh_rslt.txt", "w");
    printf("Allocator>STAT\n");
    fprintf(pFile, "Allocator>STAT\n");
    while(1){
        printf("Allocator: ");
        fprintf(pFile, "Allocator: ");
        fgets(user_input, buffer, stdin);//reading 
        fprintf(pFile, "%s", user_input);
        token = strtok(user_input,s);// through string tokenize, s = space  
        if(strcmp(token, "RQ")== 0){ // first pointer is RQ
            //Get data from user
            token = strtok(NULL, s); // pointer to next token, NULL prevents re-initializing the data.
            int input_index = atoi(&token[1]);//ex)PO [0]=P ,[1]=0
            token = strtok(NULL, s); //SIZE 
            int input_size = atoi(token);
            //Allocate
            int temp_count=0;
            struct Process temp[length]; //copying the array to the temp for re-allocating the data to the actual array
            struct Process h = search_hole(process_array); //Get biggest hole to store the process
            //in biggest empty space to search the first point and end point
            if(h.limit - h.start < input_size){
                printf("Release memory first\n");
                continue;
            }
            h.index = input_index;
            h.size = input_size;
            h.limit = h.start + input_size; //limit is end point

            process_array[counter++]= h;
            //End allocate
            //compact array
            for(int i=0;i<length;i++){
                if(process_array[i].index != -1){
                    temp[temp_count++] = process_array[i];
                }
            }
            for(int i=temp_count;i<length;i++){
                temp[i].index = -1;
                temp[i].start = 0;
                temp[i].limit = 0;
                temp[i].size = 0;
            }
            //End compact array
            // temp for not messing up the process_array
            for(int i=0;i<length;i++){
                process_array[i] = temp[i];
            }
            //re-initialize counter
            for(int i =0;i<length;i++){
                if(process_array[i].index == -1) 
                {
                    counter = i;//counter is for next empty array to input the next data. 
                    break;
                }                
            }
            //end re-initialize counter
        } //End Command RQ
        else if(strcmp(token, "RL")==0 ){
            token = strtok(NULL, s);
            int input_index = atoi(&token[1]);
            struct Process temp[length];
            int temp_count=0;
            //struct Process temp[length];
            //Find the process and remove its data.
            for(int i=0;i<length;i++){
                if(process_array[i].index == input_index){
                    process_array[i].index = -1;
                    process_array[i].start = 0;
                    process_array[i].limit = 0;
                    process_array[i].size = 0;
                }
            }
            //End find the process
            //Compact array
            for(int i=0;i<length;i++){
                if(process_array[i].index != -1){
                    temp[temp_count++] = process_array[i];
                }
            }
            for(int i=temp_count;i<length;i++){
                temp[i].index = -1;
                temp[i].start = 0;
                temp[i].limit = 0;
                temp[i].size = 0;
            }
            //End compaction
            for(int i=0;i<length;i++){
                process_array[i] = temp[i];
            }
            //End copy
            for(int i =0;i<length;i++){
                if(process_array[i].index == -1) 
                {
                    counter = i;
                    break;
                }                
            }
            //End re-initialize counter
        }
        else if(strcmp(token, "STAT")== 10 ){
            struct Process min_start;
            for(int i=0;i<10;i++){
                if(process_array[i].index == -1) {
                    break;
                }
                for(int j=i+1;j<10;j++){
                    if(process_array[j].index == -1){
                        break;
                    }
                    if(process_array[i].start > process_array[j].start){
                        min_start = process_array[i];
                        process_array[i] = process_array[j];
                        process_array[j] = min_start;
                    }
                } 
            }
            printArray(process_array);
        }
        else if(strcmp(token, "C") == 10){
            int new_start= 0;
            for(int i =0;i<length;i++){
                process_array[i].start = new_start;
                process_array[i].limit = new_start + process_array[i].size;
                new_start += process_array[i].size +1;
            }
        }
        else if(strcmp(token, "QUIT")==10){

            break;
        }else{
            printf("Invalid input. Please try again.\n");
            continue;
        }
        
    }
    
}