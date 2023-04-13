// 1 hr = 1 min real time
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include <atomic>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

std::mutex mtx;

//define an int array that will be used to store the tempurature readings
// the size is 60*8 = 480, 60 for the total reading in an hour, 8 for the number of threads
const int MAX_SIZE = 480;
int arr[MAX_SIZE];
//make current indec atomic
atomic<int> currentIndex(0);

void push(int value) {
    arr[currentIndex++] = value;

    // If adding a new element exceeds the size of the array,
    // pop the oldest value
    if (currentIndex >= MAX_SIZE) {
        currentIndex = 0;
    }
}
int getTempReading(){
    // set the seed for the random number generator
    srand(time(nullptr));
    // return a random int from -100 to 70
    int temp = rand() % 171 - 100;
   // cout << "Temp reading: " << temp << endl;
    return temp;
}
void getTop5(){
    // return the top 5 readings
    int top5[5];
    int max = -100;
    int maxIndex = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < MAX_SIZE; j++){
            if(arr[j] > max){
                max = arr[j];
                maxIndex = j;
            }
        }
        top5[i] = max;
        arr[maxIndex] = -100;
        max = -100;
    }
    // print the top 5 readings
    cout << "Top 5 readings: ";
    for(int i = 0; i < 5; i++){
        cout << top5[i] << " ";
    }
    cout << endl;
    return;
}
void getBottom5(){
    // return the bottom 5 readings
    int bottom5[5];
    int min = 70;
    int minIndex = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < MAX_SIZE; j++){
            if(arr[j] < min){
                min = arr[j];
                minIndex = j;
            }
        }
        bottom5[i] = min;
        arr[minIndex] = 70;
        min = 70;
    }
    
    // print the bottom 5 readings
    cout << "Bottom 5 readings: ";
    for(int i = 0; i < 5; i++){
        cout << bottom5[i] << " ";
    }
    cout << endl;
    return;
}
void doTask(int threadNum)
{
    int temp = getTempReading();
    mtx.lock();
    // generate a random number from 0 or 1
    int randNum = rand() % 2;
    if(randNum == 0){
        // if 0, push the temp reading to the array
        if (temp + threadNum > 70){
            push(70 - threadNum);
        }else{
            push(temp + threadNum);
        }
    }
    else{
        // if 1, push the temp reading to the array twice
        if (temp - threadNum < -100){
            push(-100 + threadNum);
        }else{
            push(temp - threadNum);
        }
    }
    mtx.unlock();
}

//function that will look at chunks of 10 elements in the array and return the largest 10 chuck average
void get10MinAvg(){
    int sum = 0;
    int max = -100;
    int maxIndex = 0;
    int j = 1;
    for(int i = 0; i < MAX_SIZE; i++){
        sum += arr[i];
        if(i % 10 == 0 && i != 0){
            if(sum / 10 > max){
                max = sum / 10;
                maxIndex = j;
            }
            j++;
            sum = 0;
        }
    }

    cout << "Section of the hour with the highest average: minute " << maxIndex * 10  << " - minute " << ((maxIndex*10)+10) << endl;
    return;
}

int main() {
    // this will be used to tell how much time has past since the start of the simulation
    //start with 3 hrs = 180 minutes
    int totalMinutes = 181;
    cout << "Enter the number of minutes you want to run the simulation: ";
    cin >> totalMinutes;

    int minutes = 0;
    while(minutes < totalMinutes)
    {
        // every minute each thread will get a new temp reading
        thread t1(doTask,1);
        thread t2(doTask,2);
        thread t3(doTask,3);
        thread t4(doTask,4);
        thread t5(doTask,5);
        thread t6(doTask,6);
        thread t7(doTask,7);
        thread t8(doTask,8);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();

        // every 10 minutes, print the top 5 and bottom 5 readings
        if(minutes % 10 == 0){
            getTop5();
            getBottom5();
            get10MinAvg();
        }
        minutes++;
        // sleep for 1 sec
        sleep(1);
        cout << "Minutes: " << minutes << endl;
    }
    return 0;
}