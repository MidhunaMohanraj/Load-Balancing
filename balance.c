/*  Team members
eyhuang1, Eric, Huang
mmohanra, Midhuna, Mohanraj
wongdq, Destin, Wong   */


#include<stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>

int Lmin=10;
int Lmax=1000;

// struct to represent processor
typedef struct {
    int load; // load of the processor
    int time_interval; // interval at which the processor balances its load
    int isSteady;
} Processor;

int rand_uniform(int max, int min){ // gives us value between min, max using uniform random distribution
    return (rand() % (max - min + 1)) + min;
    }

  // make a struct containing the load + balance interval
    // make array of structs repping the processors
    // make a for loop and start the balance simulation
    // during the loop, if cycle_number % time_interval = 0, then processor schedules load balancing activity
    // interval 10: 10, 20, 30
    // implement load balancing strategy from pdf
    // load balancing strat: 
        // each processor looks at two neighbors P(i-1) mod k and P(i+1) mod k
        // compute average number of load units to equalize all 3 load units
        // give load units to balance out neighbor processors (cannot take loads)
        // if balancing by giving units isn't possible, then do nothing
        // theoretical situation: if load unit has 10, neighbors have 2 and 3, give 3 units to left and 2 units to right
        // if load unit has 5, left has 6 and right has 4, cannot balance because they cant give
        // can give load units to neighboring processors
                    // theoretical: current proc: 10 left: 2 right: 4
                    // avg: 16/3=5 (int) 5.333333 (float)
                    // give left = 5 - 2 = 3
                    // give right = 5 - 4 = 1
                    // current remaining = 6

                    // theoretical situation where no balancing occurs:
                        // 2 10 10 10 2.  // (2+10+10)/3=7, 7-2=5; 7-10=-3
                        // with new algorithm, end result is 2 10 10 5 7
                       
                    // check if left processor is less than the average
                    // another hypothetical: current has 10, left has 2, right has 4, avg is 5 ;  // 2 10 4=16/3=5; // 5 6 5
                    // another hypothetical: 5 6 5
                    // avg is 5, so dont give
int main() {
    int k = 5;  //set number of processors
    srand(time(NULL)); // seed the random number generator

    int Dmin=100;
    int Dmax=1000;
    Processor loads[k]; //array to hold load struct for each proocessor
    
    
    // define arry of processors(already done)
    // assign each processor's load units with discrete uniform random variable
        for(int i=0; i<k; i++) {
            loads[i].load = rand_uniform(Lmax, Lmin); // assign random load units
            loads[i].time_interval = rand_uniform(Dmax, Dmin); // assign random time interval
            loads[i].isSteady = 0; // initialize steady state to false
    }
    
    for(int time=1; time < 1000000; time++){ // initialize and check if i is less than 100000
        
        // check if each processor has to do load balancing activity
        for(int j=0; j<k;j++){
            int left = (j-1+k)%k; // left neighbor index
            int right = (j+1)%k; // right neighbor index
            int swaps = 0; // variable to count number of swaps
            // TODO: check if the system is in a balanced state
            if (time % loads[j].time_interval == 0){
                // load balancing activity
                int avg = (loads[j].load + loads[left].load + loads[right].load)/3; // average load of 3 processors
                if(loads[j].load > avg){
                    if(loads[left].load < avg){
                        // distrubite to left processor
                        // give until left processor reaches average
                        int amounttogiveleft = avg - loads[left].load;
                        loads[left].load += amounttogiveleft;
                        loads[j].load -= amounttogiveleft;
                        swaps +=1; // increment swap count)
                    }
                    // check if right processor is less than the average
                    // distribute to right processor
                    if(loads[right].load < avg){
                        // distribute to right processor
                        // give until right reaches average
                        int amounttogiveright = avg - loads[right].load;
                        loads[right].load += amounttogiveright;
                        loads[j].load -= amounttogiveright;
                        swaps +=1; // increment swap count
                        
                    }
                    
                    
                } // else do nothing
                if(swaps == 0){
                    loads[j].isSteady = 1; // mark processor as steady
                } else {
                    loads[j].isSteady = 0; // mark processor as not steady
                }
            }
        }
        // check if steady state
        //After performing all the balancing compare current to prev, if no change it's a steady state
        int steadyFlag=1;
        for(int j=0; j<k; j++){
            // check if each proc is steady
            if(loads[j].isSteady == 1){
                steadyFlag = 1; // if any processor is not steady, set flag to 0
            }
            else{
                steadyFlag=0;
                break;
            }
        }
        
        int balancedFlag =0;
        
        if(steadyFlag == 1){
            printf("Steady state reached at time %d\n", time);
            // check if balanced state
            // look at first processor, check if all processors are within 1 of the first processor's load
            int average = 0;
            for (int i = 0; i < k; i++){
                average += loads[i].load;
            }
            average = average / k;
            
            for(int j=0; j<k; j++){
                if(abs(loads[j].load - average) > k){ // check if load is within 3 of first processor's load. Cases like 476 478 476, the average of these is 476, so no need to give load units.
                    printf("Not balanced at time %d\n", time);
                    balancedFlag=0;
                    break;
                }
                else{
                    balancedFlag=1;
                }
            }

            
            if(balancedFlag == 1){
                printf("Balanced state reached at time %d\n", time);
                break; // exit loop if balanced state is reached
            }
            break;
        }
        // if(balancedFlag==1){
        //     for(int j=0; j<k; j++){
        //         printf("Processor %d load: %d\n", j, loads[j].load); // print loads of each processor
        //         printf("Processor %d interval: %d\n", j, loads[j].time_interval); 
        //     }
        //     break;
        // }
        //     printf("Balanced state reached at time %d\n", time);
        //     for(int j=0; j<k; j++){
        //         printf("Processor %d load: %d\n", j, loads[j].load); // print loads of each processor
        //         printf("Processor %d interval: %d\n", j, loads[j].time_interval); 
        //     }
        //     break; // exit loop if balanced state is reached
        // }

        if (time == 1000000 - 1 ){
            printf("failed to reach a steady state");
        }
}

    
}
