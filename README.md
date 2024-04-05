# COP4520-Assignment3


## Running the Code:

To run the code, you first need to clone the repository to your local machine. Here are the general steps to do so:

1. Clone the Repository:

Open a terminal or command prompt and navigate to the directory where you want to clone the repository. Then, use the git clone command followed by the repository URL. For example:

```
git clone https://github.com/annbinus/COP4520-Assignment3.git
```

2. Navigate to the Directory:

After cloning the repository, navigate to the directory containing the code files using the cd command:

3. Compile and Run the Code:

Once inside the repository directory, you can compile and run the code files using the appropriate commands. For C++ code, you typically compile using a compiler like g++ and then execute the compiled binary.

```
g++ -o Problem1 Problem1.cpp
./Problem1
```


# Problem 1: The Birthday Presents Party

The Minotaur's servants were tasked with sorting presents and writing "Thank you" cards in a concurrent manner. Each servant was responsible for either adding presents to the sorted chain, writing cards, or checking for specific presents. They alternated between these tasks until all presents were sorted and cards were written. However, at the end of the day, they realized they had more presents than "Thank you" notes. The flaw in this scenario likely arises from the concurrency strategy employed. Since each servant is performing tasks concurrently and independently, there is no coordination or synchronization between them regarding the number of presents added to the chain and the number of "Thank you" cards written. As a result, it's possible that multiple servants could have been writing cards simultaneously without ensuring that there were enough cards for each present.


The provided code simulates a scenario where the Minotaur organizes a birthday party and receives a large number of presents from guests. The Minotaur wants to sort these presents and write "Thank you" cards to each guest. To accomplish this task efficiently, the code implements a concurrent linked-list data structure. This linked-list supports adding, removing, and checking for the presence of gifts in a thread-safe manner.

The strategy used in the code involves several steps. First, a concurrent linked-list is created with head and tail nodes. Each gift is represented as a node in this linked-list. Gifts are added to the linked-list in sorted order based on their tag numbers. The addition operation is thread-safe and ensures that the linked-list remains sorted.

Next, multiple servants (threads) are spawned to handle the task of adding gifts to the linked-list and writing "Thank you" cards. Each servant alternates between these two tasks, ensuring a balanced workload distribution. When a servant attempts to add a gift, it checks whether the gift is already present in the linked-list. If not, it adds the gift to the appropriate position in the list.

Similarly, when a servant is tasked with writing a "Thank you" card, it removes the first gift from the linked-list. This removal operation is also thread-safe and ensures that the linked-list remains consistent.

The process continues until all the gifts have been processed, and the required number of "Thank you" cards have been written. Throughout the process, the code ensures thread safety and efficient handling of the presents and cards.

I confirmed the program's functionality by executing it with a reduced number of gifts while incorporating informative debug statements. It complies with the specified requirements. The program's runtime exhibits linearity relative to the quantity of presents. This is because, at any given point, the chain contains a maximum of four presents due to the alternating process, and each gift is accessed a constant number of times. To validate this behavior, I tested the program with progressively larger numbers of gifts and monitored the runtime.


# Problem 2: Atmospheric Temperature Reading Module

The task is to design a module for the Mars Rover's atmospheric temperature measurement system. The system must utilize 8 temperature sensors to collect readings at regular intervals. At the end of each hour, the module should compile a report featuring the top 5 highest and lowest temperatures recorded during that hour, along with the 10-minute interval showing the largest temperature difference. The solution should handle shared memory access efficiently to avoid delays in sensor readings. Using 8 threads, the solution must simulate sensor operations by generating random temperature readings between -100°F and 70°F every minute. The efficiency, correctness, and progress guarantee of the program need to be discussed in the report.

The code simulates the operation of a system responsible for collecting atmospheric temperature readings from multiple sensors over a span of time. Each sensor records temperature readings at regular intervals, akin to timesteps in the simulation. The program employs a lazy synchronization linked list to manage these readings efficiently, ensuring that multiple threads can safely add and retrieve readings concurrently without contention.

During simulation, each sensor thread generates temperature readings for each timestep, and these readings are added to the shared linked list. Once all sensor threads finish their task, the program retrieves the recorded data from the linked list and processes it further. Firstly, it sorts the readings in decreasing order of temperature, presenting the highest and lowest temperatures recorded during the simulation. Then, it identifies the interval of ten consecutive timesteps with the largest temperature difference, highlighting the start and end times of this interval.

This strategy enables efficient data management and analysis of atmospheric temperature readings, demonstrating a practical approach to handling concurrent operations and extracting meaningful insights from collected data.

To simulate multiple hours, the simulation can be iterated multiple times. The concurrent linked list implementation ensures mutual exclusion, guaranteeing progress even with simultaneous access from multiple threads. To validate the correctness of the program, I conducted an in-depth analysis using debug statements, confirming that the program behaves as expected according to the specified instructions. The time complexity of the program is O(nlogn) due to the sorting operations involved, where n represents the number of time steps per sensor. However, considering the input constraints limit the number of time steps to 60, the runtime impact of sorting is negligible. This thorough analysis assures that the program not only functions correctly but also performs efficiently within the given constraints.