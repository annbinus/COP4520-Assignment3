#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>

using namespace std;


const int NUM_SERVANTS = 4;
const int TOTAL_GIFTS = 500000;

// Atomic variables for thread synchronization
atomic<int> bagIndex(0), cardsWritten(0);

// Structure for a node in the concurrent linked list
struct GiftNode {
    int value;
    GiftNode* next;
    bool marked = false;
    mutex m;

    // Constructor
    GiftNode(int val, GiftNode* next) : value(val), next(next) {}

    void lockNode() {
        m.lock();
    }

    void unlockNode() {
        m.unlock();
    }
};

// Concurrent linked-list implementation
struct ConcurrentLinkedList {
    GiftNode* tail = new GiftNode(1e9, nullptr);
    GiftNode* head = new GiftNode(0, tail);

    // Function to validate nodes for consistency
    bool validateNodes(GiftNode* pred, GiftNode* curr) {
        return !pred->marked && !curr->marked && pred->next == curr;
    }

    bool addGift(int item) {
        while (true) {
            GiftNode* pred = head;
            GiftNode* curr = head->next;
    
            while (curr->value < item) {
                pred = curr;
                curr = curr->next;
            }

            // Lock the predecessor and current nodes
            pred->lockNode();
            try {
                curr->lockNode();
                try {
                    if (validateNodes(pred, curr)) {
                        if (curr->value == item) {
                            // If gift already exists, unlock nodes and return false
                            pred->unlockNode();
                            curr->unlockNode();
                            return false;
                        }
                        else {
                            // Add the new gift to the list
                            GiftNode* newNode = new GiftNode(item, curr);
                            pred->next = newNode;
                            pred->unlockNode();
                            curr->unlockNode();
                            return true;
                        }
                    }
                }
                catch (...) {}
                curr->unlockNode();
            }
            catch (...) {}
            pred->unlockNode();
        }
    }

    int removeGift() {
        while (true) {
            GiftNode* pred = head;
            GiftNode* curr = head->next;

            pred->lockNode();
            try {
                curr->lockNode();
                try {
                    if (validateNodes(pred, curr)) {
                        if (curr->value < 1e9) {
                            // Mark the node as removed
                            curr->marked = true;
                            pred->next = curr->next;
                            pred->unlockNode();
                            curr->unlockNode();
                            return curr->value;
                        }
                        else {
                            // If tail node is reached, unlock nodes and return false
                            pred->unlockNode();
                            curr->unlockNode();
                            return false;
                        }
                    }
                }
                catch (...) {}
                curr->unlockNode();
            }
            catch (...) {}
            pred->unlockNode();
        }
    }
    bool contains(int item) {
        GiftNode* curr = head;
        while (curr->value < item)
            curr = curr->next;

        return curr->value == item && !curr->marked;
    }
};

// Function to simulate the Minotaur's birthday party
void partySimulation() {
    ConcurrentLinkedList chain;
    vector<thread> servants(NUM_SERVANTS);
    vector<int> giftOrder(TOTAL_GIFTS);

     // Generate random order of grabbing gifts from the large bag
    iota(giftOrder.begin(), giftOrder.end(), 1);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(giftOrder.begin(), giftOrder.end(), default_random_engine(seed));

    // Create threads for servants to handle gifts and writing cards
    for (int i = 0; i < NUM_SERVANTS; i++)
        servants[i] = thread([&](int id) {
            bool action = false; // False = add to chain, true = remove from chain
            while (cardsWritten < TOTAL_GIFTS) {
                if (!action) {
                    int idx = bagIndex++;
                    if (idx < TOTAL_GIFTS) {
                        chain.addGift(giftOrder[idx]);
                    }
                    else {
                        action = true;
                    }
                }

                if (action) {
                    int gift = chain.removeGift();

                    if (gift >= 1) {
                        cardsWritten++;
                    }
                    else {
                        break;
                    }
                } 
                action ^= 1; // Alternate between adding and removing gifts
            }
        }, i);

    // Join all servant threads
    for (thread& servant : servants)
        servant.join();

    cout << cardsWritten << " \"Thank you\" cards have been written." << endl;
}

int main() {
    partySimulation();
    return 0;
}