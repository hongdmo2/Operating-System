//gcc Hchangoh_thd.c -o Hchangoh_thd.exe -lpthread
//./Hchangoh_thd.exe 100000 10 10

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *runner_part1(void *param); // start_routine for part 1
void *grand_runner_part3(void *arg); //start_routine for part3
long sum; //global variable for total sum for part 1
struct my_Struct{ //To define a structure, defines data types for child thread
    int index; // index number child thread
    long long start; // starting point of child thread
    long long end; // end poing of child thread
    long long answer; // sum of child thread
    long long num_grand; // number of grand child thread
};
struct my_grandStruct{ // To define a structure, defines data types for Grand child thread
    int index; // index number of grand child thread
    long long start; // start point of grand child thread
    long long end; // end point of grand child thread
    long long answer;// sum of grand child thread
};
void run_part1(char* number){
    printf("Part1 start\n");
   
    pthread_t tid;         //Declaring the identifier of the Thead in part 1
    pthread_attr_t attr;   //Each thread has a set of attributes
                           //The attr argument points to a pthread_attr_t structure whose contents are used at thread creation time to determine attributes for the new thread.
    pthread_attr_init(&attr); //function initializes the thread attributes object pointed to by attr with default attributes values.
    pthread_create(&tid, &attr, runner_part1, number); //Thread is created by this function. The pthread_create()function starts a new thread in the calling process
                                                       //start_routine();arg is passed as the sole argument of start_routine().
                                                       //sending 'number' to the start_routine() function
    pthread_join(tid,NULL);   //The parent thread waits until child threads terminates
                              //tid is childthread that parent thread is waiting for
                              //NULL is return value.
    printf("Part1 End---> sum: %ld\n\n " , sum); //print sum which is calcaulted in the childthread.
    
}
void *runner_part1(void *param){   //this is the start_routine for the part1. param is the 'number' from the parent thread

    long long i, upper = atoi(param);  //parameter which is from the parent thread is converted ASCII to integer i is start point and high is last number
    sum = 0;    //Initialize the sum which is global variable

    for(int i =1;i <=upper;i++){
        sum+=i; // from the start point to the last point, adding it to sum
    }
    pthread_exit(0); //Terminate child thread.
}
void* runner_part2(void *arg){ //this is the start_routine for the part2. arg is the &limits[i] of its parent thread
    struct my_Struct *argv_value = (struct my_Struct*) arg; //argv_value is the varibale of structure my_Struct
                                                            //arg is void argument so needs to redeclare to argv_value
    long long childsum = 0; //sum of child thread values
    
    for (long long i = argv_value->start; i<= argv_value->end; i++){ //start point of child thread is argv_value->start and number of argv_value-> end is end point.

        childsum+=i; //add numbers into childsum
    }
    argv_value->answer = childsum; //save total number into argv_value->answer in the structure
    
}
void run_part2(const char* number, const char* num_child){ //two arguemets, first one is input number that needs to calcaulted and second one is number of child thread.
    printf("Part2 start ---> Parent Start of Execution\n");
    long long totalsum=0;   
    long long value = atoi(number);//converting ASCII to integer from the first aguement
    int num_childthread = atoi(num_child);//converting ASCII to integer second argument
    
    long long increment = value / num_childthread; //increment is increasement of each child thread
    long long end=increment, start = 1; //end point of child thread is its increasement and start from 1
    int index =1;//indexing the childthread
    struct my_Struct limits[num_childthread];// make the structure to array
    pthread_t tid[num_childthread];//Declaring the identifier of the Thead in part2 and make thread ID to array
    limits[0].start = start;
    limits[0].end = end;
    limits[0].index = index;
    
    for(int i=1; i < num_childthread; i++){//dividing parents to child threads
        start += increment; // every time calculation, start point of next child thread is increasing
        end +=increment;// every time calcualtion, end point of child thread is increasing
        index += 1; // index is increasing
        limits[i].start = start; //save data into structure array from 1 to its number of child thread
        limits[i].end = end;
        limits[i].index = index;
    }
    
    for(int i=0; i < num_childthread; i++){
        pthread_attr_t attr;  //Each thread has a set of attributes
                            //The attr argument points to a pthread_attr_t structure whose contents are used at thread creation time to determine attributes for the new thread.
        pthread_attr_init(&attr);//function initializes the thread attributes
        pthread_create(&tid[i], &attr, runner_part2, &limits[i]);//Thread is created by this function. The pthread_create()function starts a new thread in the calling process
                                                                //start_routine();arg is passed as the sole argument of start_routine().
      
    }
    for(int i=0; i< num_childthread; i++){
        pthread_join(tid[i],NULL);//Parent thread waits for its child thread until child thread terminates
        printf("Child thread number %d: \t child start=%lld \t end = %lld \t  childsum = %lld \n ", limits[i].index, limits[i].start, limits[i].end, limits[i].answer);    
        //print out the child thread number, child start,end, and its calcultation from the datas in the structure.
        totalsum += limits[i].answer;  
        //each child threads' calculation is added to totalsum 
    }
      
    printf("Parent Total sum is :%lld\n",totalsum); // print out the total summation of the child threads
    printf("Part2 End ---> Parent:End of Execution\n\n ");// parents thread is ended.
    
}
void* runner_part3(void *arg){ //start routine for part3
    struct my_Struct *argv_value = (struct my_Struct*) arg; //Convert void arg to struct argv_value and using its data in the strucutre
 
    long long grand_start;//grand child start point
    long long grand_end;//grand child end point
    long long grand_increment;//increase ment for each grand child
    long long grand_index = 0;// indexing
    long long childsum =0;// total sum of child thread which is summation of grand child threads
    
    grand_start = argv_value->start; // assign start point to grand_start
    grand_increment = ((argv_value->end)-(argv_value->start)+1)/(argv_value->num_grand);// assign increasement by calculation
    grand_end =grand_start + grand_increment-1;//grand child's endpoint is its start point plus grand child's increasement plus one

    struct my_grandStruct grand[argv_value->num_grand]; // make structure to array
    pthread_t tidd[argv_value->num_grand]; //thread ID also need to be assgined as array
    
   for(int i=0; i<argv_value->num_grand; i++){ // dividing the child threads to grand child threads
            
       grand_index += 1; 
       grand[i].start = grand_start; // start point of each grand child threads
       grand[i].end = grand_end;//end point of grand child threads 
       grand[i].index = grand_index;//indexing each grand child threads
       grand_start += grand_increment;//add increasement to each grand child's start point
       grand_end += grand_increment;//add increasement to each grand child's end point

    }
    for (int i =0; i<argv_value->num_grand; i++){
        pthread_attr_t attrr;
        pthread_attr_init(&attrr);
        pthread_create(&tidd[i],&attrr, grand_runner_part3, &grand[i]);//grand child thread is created by this function. The pthread_create()function starts a new thread in the calling process
    }
    
    
    for (int i=0; i<argv_value->num_grand; i++){
        pthread_join(tidd[i],NULL); // child thread waits for its grand child thread terminates
        childsum += grand[i].answer;//summation of grand child thread is its child's summation
        printf("Child Thread number %d: \t GrandChild thread number %d: \t grandchild start=%lld \t grandchild end = %lld \t grand child sum = %lld \n ", argv_value->index,grand[i].index, grand[i].start, grand[i].end, grand[i].answer);
        //print out thild thread number, grand child thread number, grand child starting point, grand child endpoint, and grand child's summation    
    }
    
    argv_value->answer = childsum;//save the child summation into the data in the strucutre
}
void* grand_runner_part3(void *arg){//start routine for grand child threads
    struct my_Struct *argv_value = (struct my_Struct*) arg;//void arg assings to argv_value
    struct my_grandStruct *argv_grandvalue =(struct my_grandStruct*) arg;//void arg assigns to argv_grandvalue
    long long grandsum =0;
    
    for(int i=argv_grandvalue->start; i<=argv_grandvalue->end; i++){
        grandsum +=i;//calcuation of grand child threads
    }
    argv_grandvalue->answer =grandsum;//save the calculation into the data in the grand child structure
  
}
void run_part3(const char* number, const char* num_child, const char* num_grand){ // takes three arguments ,input number, number of child threads, and number of grand child threads
    printf("Part3 start ---> Parent Start Execution\n");//parent thread
    long long value = atoi(number);//converting ASCII to long long
    int num_childthread = atoi(num_child);//converting ASCII to integer
    int num_grandchildthread =atoi(num_grand);//converting ASCII to integer
    long long increment = value / num_childthread;//increment is increasement of each child thread
    long long end=increment, start = 1;//end point of child thread is its increasement and start from 1
    long long index =1; //indexing the child thread
    long long totalsum =0;//initializes the total summation of child threads

    struct my_Struct limits[num_childthread]; //declaring child strucutre to array
    pthread_t tid[num_childthread];//Declaring the identifier of the Thead in part3 and make thread ID to array

    limits[0].start = start;
    limits[0].end = end;
    limits[0].index = index;

    for(int i=0; i < num_childthread; i++){
        limits[i].num_grand = num_grandchildthread;   //each array of child strucutre contains number of grand child threads 
    }
    for(int i=1; i < num_childthread; i++){    
        start += increment; // every time calculation, start point of next child thread is increasing
        end +=increment;// every time calcualtion, end point of child thread is increasing
        index += 1;// index is increasing
        limits[i].start = start;//save data into structure array from 1 to its number of child thread
        limits[i].end = end;
        limits[i].index = index;
    }
    for(int i=0; i < num_childthread; i++){
        pthread_attr_t attr;//Each thread has a set of attributes
                            //The attr argument points to a pthread_attr_t structure whose contents are used at thread creation time to determine attributes for the new thread.
        pthread_attr_init(&attr);//function initializes the thread attributes
        pthread_create(&tid[i], &attr, runner_part3, &limits[i]);//Thread is created by this function. The pthread_create()function starts a new thread in the calling process
    }
    for(int i=0; i< num_childthread; i++){
        struct my_grandStruct grand[limits[i].num_grand];
        pthread_join(tid[i],NULL);//Parent thread waits for its child thread until child thread terminates
        totalsum += limits[i].answer;//each child threads' calculation is added to totalsum 
        printf("Child thread number %d: \t child start=%lld \t end = %lld \t  childsum = %lld \n ", limits[i].index, limits[i].start, limits[i].end, limits[i].answer);
        //print out the child thread number, child start,end, and its calcultation from the datas in the structure.
    }
    
    printf("Parent: Total sum = %lld\n", totalsum);// print out the total summation of the child threads
    printf("Part 3 End ---> Parent: End of Execution\n");// parents thread is ended.
    pthread_exit(0); //threads terminate

   
}
int main(int argc, char *argv[]){
    freopen("Hchangoh_rslt.txt", "w", stdout);//Write outputs into the textfile
    long long number = atoi(argv[1]);//Converting ASCII to long long
    int childthread = atoi(argv[2]);//Converting ASCII to integer
    int grandchildthread = atoi(argv[3]);
    
    if(argc != 4)//if number of argument is not 4, print out the error message
    {
        printf("Wrong number of command line arguments\n");
        exit(1);
        return 1;
    }
    if(number> 1000000){//if input number is greater than 1000000, then print out the error message
        printf("Number of input value should be less than and equal to 1,000,000");
        exit(1);
        return 1;
    }
    if(childthread> 50){//if number of child thread is greater than 50, print out the error message 
        printf("Number of childthread should be less than and equal to 50");
        exit(1);
        return 1;
    }
    if(grandchildthread > 50){//if number of grand child thread is greater than 50, print out the error message 
        printf("Number of grand child thread should be less than and equal to 50");
        exit(1);
        return 1;
    }
    run_part1(argv[1]);// run part1
    run_part2(argv[1],argv[2]);// run part2
    run_part3(argv[1],argv[2],argv[3]);// run part 3
    return 0;
}
