#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>

using namespace std;

const int TIME_STEPS = 60;
const int NUM_SENSORS = 8;
const int MAX_TEMP = 70;
const int MIN_TEMP = -100;

struct TemperatureReading {
    int temperature, time;

    TemperatureReading(int temp, int time) : temperature(temp), time(time) {}
};

struct Node {
    TemperatureReading data;
    Node* next;
    bool marked = false;
    mutex m;

    Node(const TemperatureReading& data, Node* next = nullptr) : data(data), next(next) {}

    void lock() {
        m.lock();
    }

    void unlock() {
        m.unlock();
    }
};

class LazyTemperatureList {
private:
    Node* tail;
    Node* head;

    bool validate(Node* pred, Node* curr) {
        return !pred->marked && !curr->marked && pred->next == curr;
    }

public:
    LazyTemperatureList() {
        tail = new Node(TemperatureReading(-1, -1), nullptr);
        head = new Node(TemperatureReading(-1, -1), tail);
    }

    bool add(const TemperatureReading& item) {
        while (true) {
            Node* predNode = head;
            Node* currNode = head->next;

            predNode->lock();
            try {
                currNode->lock();
                try {
                    if (validate(predNode, currNode)) {
                        Node* newNode = new Node(item, currNode);
                        predNode->next = newNode;
                        predNode->unlock();
                        currNode->unlock();
                        return true;
                    }
                } catch (...) {
                }
                currNode->unlock();
            } catch (...) {
            }
            predNode->unlock();
        }
    }

    TemperatureReading popFront() {
        while (true) {
            Node* predNode = head;
            Node* currNode = head->next;

            predNode->lock();
            try {
                currNode->lock();
                try {
                    if (validate(predNode, currNode)) {
                        if (currNode->data.time != -1) {
                            currNode->marked = true;
                            predNode->next = currNode->next;
                            predNode->unlock();
                            currNode->unlock();
                            return currNode->data;
                        } else {
                            predNode->unlock();
                            currNode->unlock();
                            return TemperatureReading(-1, -1);
                        }
                    }
                } catch (...) {
                }
                currNode->unlock();
            } catch (...) {
            }
            predNode->unlock();
        }
    }
};

void sensorTask(LazyTemperatureList& temperatureList, int id) {
    for (int i = 0; i < TIME_STEPS; i++) {
        int temperature = rand() % (MAX_TEMP - MIN_TEMP + 1) + MIN_TEMP;
        temperatureList.add(TemperatureReading(temperature, i));
    }
}

void simulate() {
    srand(time(0));
    LazyTemperatureList temperatureList;
    vector<thread> sensors(NUM_SENSORS);

    for (int i = 0; i < NUM_SENSORS; i++) {
        sensors[i] = thread(sensorTask, ref(temperatureList), i);
    }

    for (auto& sensor : sensors) {
        sensor.join();
    }

    vector<TemperatureReading> data;
    while (true) {
        TemperatureReading reading = temperatureList.popFront();
        if (reading.time == -1) {
            break;
        }
        data.push_back(reading);
    }

    sort(data.begin(), data.end(), [](const TemperatureReading& a, const TemperatureReading& b) {
        return a.temperature > b.temperature;
    });

    cout << "5 highest temperatures recorded:\n";
    for (int i = 0; i < 5; i++) {
        cout << data[i].temperature << " degrees F\n";
    }
    cout << endl;

    cout << "5 lowest temperatures recorded:\n";
    for (int i = data.size() - 1; i >= int(data.size()) - 5; i--) {
        cout << data[i].temperature << " degrees F\n";
    }
    cout << endl;

    sort(data.begin(), data.end(), [](const TemperatureReading& a, const TemperatureReading& b) {
        return a.time < b.time;
    });

    int largestDifference = 0;
    int bestTime = 0;

    for (int i = 0; i + 9 < data.size(); i++) {
        int maxTemp = data[i].temperature;
        int minTemp = data[i].temperature;

        for (int j = i; j <= i + 9; j++) {
            maxTemp = max(maxTemp, data[j].temperature);
            minTemp = min(minTemp, data[j].temperature);
        }

        int difference = maxTemp - minTemp;

        if (difference > largestDifference) {
            largestDifference = difference;
            bestTime = data[i].time;
        }
    }

    cout << "The largest temperature difference occurs in between " << bestTime + 1 << " and " << bestTime + 11 << " minutes." << endl;
}

int main() {
    simulate();
    return 0;
}
