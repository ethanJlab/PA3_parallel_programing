# Programming Assignment 3

## How To run
- Run the command ``` g++ -o problem1  problem1.cpp ``` to compile the source code
- Run the executable with the command ``` .\problem1.exe ```
- Run the command ``` g++ -o problem2  problem2.cpp ``` to compile the source code
- Run the executable with the command ``` .\problem2.exe ```

## Summary
### Problem 1
In my approach for problem 1, I had a servent (thread) dedicated to each task. The workflow would start by servent 1 pulling a present from a bag which was represented by a random number.
After getting the ID for the present, servent 2 looks ath the chain of presents and confirms that the present has not been added yet.
After this confirmation, servent 3 inserts the present into the chain while servent 4 writes a thank you letter.
This is done until all presents have been removed from the bag.

### Problem 2
In my approach for problem 2, I had each thread take a temperature reading every minute. To ensure that the 10 minute reports do not interfiere with the threads, I added a push function so threads do not overwrite eachothers data.
These reports also can run while the other theads are still taking their reads, thus further ensuring that the reports do no interfiere with the readings.
