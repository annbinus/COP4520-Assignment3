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