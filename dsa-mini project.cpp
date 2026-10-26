#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

// ============================================================
// Ambulance Management System - 10 Data Structures
// 1. Array                 -> cityNames[], ambulances[]
// 2. Singly linked list    -> driver records
// 3. Doubly linked list    -> emergency records
// 4. Linked-list queue     -> incoming emergency requests
// 5. Circular queue        -> waiting list with capacity 5
// 6. Stack                 -> undo latest dispatch
// 7. Adjacency matrix      -> roads between locations
// 8. Hash table chaining   -> patient and ambulance lookup
// 9. Binary search tree    -> sorted emergency IDs
// 10. Min-heap             -> Dijkstra's shortest-path algorithm
// ============================================================

const int MAX_LOCATIONS = 8;
const int MAX_AMBULANCES = 6;
const int WAITING_CAPACITY = 5;
const int INF = 1'000'000;

// -------------------- 1. ARRAY ------------------------------
struct Ambulance {
    string id;
    int location;
    bool available;
    string type;
    string driverId;
};

string cityNames[MAX_LOCATIONS + 1] = {
    "", "Central", "North", "East", "South",
    "West", "Airport", "University", "Industrial"
};

Ambulance ambulances[MAX_AMBULANCES] = {
    {"A01", 1, true,  "Advanced", "D01"},
    {"A02", 3, true,  "Basic",    "D02"},
    {"A03", 5, false, "Advanced", "D03"},
    {"A04", 7, true,  "Basic",    "D04"},
    {"A05", 2, true,  "Advanced", "D05"},
    {"A06", 8, true,  "Basic",    "D06"}
};

// -------------------- 2. SINGLY LINKED LIST -----------------
struct DriverNode {
    string id;
    string name;
    string phone;
    DriverNode* next;

    DriverNode(string driverId, string driverName, string driverPhone)
        : id(driverId), name(driverName), phone(driverPhone), next(nullptr) {}
};

class DriverList {
private:
    DriverNode* head = nullptr;

public:
    ~DriverList() {
        while (head) {
            DriverNode* old = head;
            head = head->next;
            delete old;
        }
    }

    void add(const string& id, const string& name, const string& phone) {
        DriverNode* node = new DriverNode(id, name, phone);
        node->next = head;
        head = node;
    }

    DriverNode* find(const string& id) const {
        for (DriverNode* p = head; p; p = p->next)
            if (p->id == id) return p;
        return nullptr;
    }

    void display() const {
        cout << "\n--- Drivers (Singly Linked List) ---\n";
        for (DriverNode* p = head; p; p = p->next)
            cout << p->id << " | " << p->name << " | " << p->phone << '\n';
    }
};

// -------------------- EMERGENCY RECORD ----------------------
struct Emergency {
    int id;
    string patientId;
    int location;
    int severity;
    string type;
    string status;
    string ambulanceId;
};

// -------------------- 3. DOUBLY LINKED LIST -----------------
struct EmergencyNode {
    Emergency data;
    EmergencyNode* previous;
    EmergencyNode* next;

    EmergencyNode(const Emergency& emergency)
        : data(emergency), previous(nullptr), next(nullptr) {}
};

class EmergencyList {
private:
    EmergencyNode* head = nullptr;
    EmergencyNode* tail = nullptr;

public:
    ~EmergencyList() {
        while (head) {
            EmergencyNode* old = head;
            head = head->next;
            delete old;
        }
    }

    void add(const Emergency& emergency) {
        EmergencyNode* node = new EmergencyNode(emergency);
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            node->previous = tail;
            tail = node;
        }
    }

    Emergency* find(int id) const {
        for (EmergencyNode* p = head; p; p = p->next)
            if (p->data.id == id) return &p->data;
        return nullptr;
    }

    void displayForward() const {
        cout << "\n--- Emergency History (Doubly Linked List) ---\n";
        if (!head) {
            cout << "No emergency records.\n";
            return;
        }
        for (EmergencyNode* p = head; p; p = p->next) print(p->data);
    }

    void displayBackward() const {
        cout << "\n--- Emergency History in Reverse ---\n";
        for (EmergencyNode* p = tail; p; p = p->previous) print(p->data);
    }

    static void print(const Emergency& e) {
        cout << "E" << e.id << " | Patient: " << e.patientId
             << " | Location: " << cityNames[e.location]
             << " | Severity: " << e.severity
             << " | Type: " << e.type
             << " | Status: " << e.status;
        if (!e.ambulanceId.empty()) cout << " | Ambulance: " << e.ambulanceId;
        cout << '\n';
    }
};

// -------------------- 4. LINKED-LIST QUEUE ------------------
struct RequestNode {
    int emergencyId;
    RequestNode* next;
    explicit RequestNode(int id) : emergencyId(id), next(nullptr) {}
};

class RequestQueue {
private:
    RequestNode* frontNode = nullptr;
    RequestNode* rearNode = nullptr;

public:
    ~RequestQueue() {
        while (frontNode) pop();
    }

    bool empty() const { return frontNode == nullptr; }

    void push(int id) {
        RequestNode* node = new RequestNode(id);
        if (!rearNode) frontNode = rearNode = node;
        else {
            rearNode->next = node;
            rearNode = node;
        }
    }

    int front() const { return frontNode ? frontNode->emergencyId : -1; }

    int pop() {
        if (!frontNode) return -1;
        RequestNode* old = frontNode;
        int id = old->emergencyId;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete old;
        return id;
    }
};

// -------------------- 5. CIRCULAR QUEUE ---------------------
class CircularQueue {
private:
    int values[WAITING_CAPACITY];
    int frontIndex = 0;
    int rearIndex = -1;
    int count = 0;

public:
    bool full() const { return count == WAITING_CAPACITY; }
    bool empty() const { return count == 0; }

    bool push(int id) {
        if (full()) return false;
        rearIndex = (rearIndex + 1) % WAITING_CAPACITY;
        values[rearIndex] = id;
        ++count;
        return true;
    }

    int pop() {
        if (empty()) return -1;
        int id = values[frontIndex];
        frontIndex = (frontIndex + 1) % WAITING_CAPACITY;
        --count;
        return id;
    }

    void display() const {
        cout << "\nWaiting list: ";
        if (empty()) {
            cout << "empty\n";
            return;
        }
        for (int i = 0; i < count; ++i)
            cout << "E" << values[(frontIndex + i) % WAITING_CAPACITY] << ' ';
        cout << "\n";
    }
};

// -------------------- 6. STACK -------------------------------
struct DispatchAction {
    int emergencyId;
    int ambulanceIndex;
    int previousLocation;
};

class DispatchStack {
private:
    struct Node {
        DispatchAction action;
        Node* next;
        Node(DispatchAction a, Node* n) : action(a), next(n) {}
    };
    Node* topNode = nullptr;

public:
    ~DispatchStack() {
        while (topNode) pop();
    }

    void push(DispatchAction action) {
        topNode = new Node(action, topNode);
    }

    bool empty() const { return topNode == nullptr; }

    DispatchAction pop() {
        DispatchAction result{-1, -1, -1};
        if (!topNode) return result;
        Node* old = topNode;
        result = old->action;
        topNode = topNode->next;
        delete old;
        return result;
    }
};

// -------------------- 7. GRAPH: ADJACENCY MATRIX ------------
class RoadGraph {
private:
    int matrix[MAX_LOCATIONS + 1][MAX_LOCATIONS + 1]{};

public:
    RoadGraph() {
        for (int i = 1; i <= MAX_LOCATIONS; ++i)
            for (int j = 1; j <= MAX_LOCATIONS; ++j)
                matrix[i][j] = (i == j ? 0 : INF);
    }

    void addRoad(int a, int b, int minutes) {
        if (a >= 1 && a <= MAX_LOCATIONS && b >= 1 && b <= MAX_LOCATIONS) {
            matrix[a][b] = matrix[b][a] = minutes;
        }
    }

    int weight(int a, int b) const { return matrix[a][b]; }

    void display() const {
        cout << "\n--- Road Graph (Adjacency Matrix) ---\n    ";
        for (int i = 1; i <= MAX_LOCATIONS; ++i) cout << i << "\t";
        cout << '\n';
        for (int i = 1; i <= MAX_LOCATIONS; ++i) {
            cout << i << " | ";
            for (int j = 1; j <= MAX_LOCATIONS; ++j)
                cout << (matrix[i][j] == INF ? -1 : matrix[i][j]) << "\t";
            cout << '\n';
        }
    }
};

// -------------------- 8. HASH TABLE WITH CHAINING ------------
class HashTable {
private:
    struct Entry {
        string key;
        int value;
        Entry* next;
        Entry(string k, int v, Entry* n) : key(k), value(v), next(n) {}
    };

    static const int SIZE = 17;
    Entry* table[SIZE]{};

    int hashFunction(const string& key) const {
        unsigned long hash = 0;
        for (char c : key) hash = (hash * 31 + c) % SIZE;
        return static_cast<int>(hash);
    }

public:
    ~HashTable() {
        for (int i = 0; i < SIZE; ++i) {
            while (table[i]) {
                Entry* old = table[i];
                table[i] = table[i]->next;
                delete old;
            }
        }
    }

    void insert(const string& key, int value) {
        int index = hashFunction(key);
        for (Entry* p = table[index]; p; p = p->next) {
            if (p->key == key) {
                p->value = value;
                return;
            }
        }
        table[index] = new Entry(key, value, table[index]);
    }

    bool find(const string& key, int& value) const {
        int index = hashFunction(key);
        for (Entry* p = table[index]; p; p = p->next) {
            if (p->key == key) {
                value = p->value;
                return true;
            }
        }
        return false;
    }
};

// -------------------- 9. BINARY SEARCH TREE -----------------
struct TreeNode {
    int id;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int value) : id(value), left(nullptr), right(nullptr) {}
};

class EmergencyBST {
private:
    TreeNode* root = nullptr;

    TreeNode* insert(TreeNode* node, int id) {
        if (!node) return new TreeNode(id);
        if (id < node->id) node->left = insert(node->left, id);
        else if (id > node->id) node->right = insert(node->right, id);
        return node;
    }

    void inorder(TreeNode* node) const {
        if (!node) return;
        inorder(node->left);
        cout << "E" << node->id << ' ';
        inorder(node->right);
    }

    void destroy(TreeNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ~EmergencyBST() { destroy(root); }
    void insert(int id) { root = insert(root, id); }
    void displaySorted() const {
        cout << "\nEmergency IDs in sorted order (BST): ";
        inorder(root);
        cout << '\n';
    }
};

// -------------------- 10. MIN-HEAP / DIJKSTRA ---------------
struct HeapItem {
    int distance;
    int city;
};

class MinHeap {
private:
    HeapItem items[MAX_LOCATIONS * MAX_LOCATIONS];
    int size = 0;

    void swapItems(int a, int b) { swap(items[a], items[b]); }

public:
    bool empty() const { return size == 0; }

    void push(HeapItem item) {
        if (size >= MAX_LOCATIONS * MAX_LOCATIONS) return;
        int i = size++;
        items[i] = item;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (items[parent].distance <= items[i].distance) break;
            swapItems(parent, i);
            i = parent;
        }
    }

    HeapItem pop() {
        HeapItem result{INF, -1};
        if (empty()) return result;
        result = items[0];
        items[0] = items[--size];
        int i = 0;
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
            if (left < size && items[left].distance < items[smallest].distance)
                smallest = left;
            if (right < size && items[right].distance < items[smallest].distance)
                smallest = right;
            if (smallest == i) break;
            swapItems(i, smallest);
            i = smallest;
        }
        return result;
    }
};

void dijkstra(const RoadGraph& graph, int source, int distance[], int parent[]) {
    for (int i = 1; i <= MAX_LOCATIONS; ++i) {
        distance[i] = INF;
        parent[i] = -1;
    }

    MinHeap heap;
    distance[source] = 0;
    heap.push({0, source});

    while (!heap.empty()) {
        HeapItem current = heap.pop();
        if (current.distance != distance[current.city]) continue;

        for (int next = 1; next <= MAX_LOCATIONS; ++next) {
            int roadTime = graph.weight(current.city, next);
            if (roadTime != INF &&
                distance[next] > current.distance + roadTime) {
                distance[next] = current.distance + roadTime;
                parent[next] = current.city;
                heap.push({distance[next], next});
            }
        }
    }
}

void printRoute(int target, const int parent[]) {
    int route[MAX_LOCATIONS];
    int length = 0;
    for (int city = target; city != -1; city = parent[city])
        route[length++] = city;

    cout << "Route: ";
    for (int i = length - 1; i >= 0; --i) {
        cout << cityNames[route[i]];
        if (i > 0) cout << " -> ";
    }
    cout << '\n';
}

// -------------------- SYSTEM OPERATIONS ---------------------
DriverList drivers;
EmergencyList emergencies;
RequestQueue incomingRequests;
CircularQueue waitingList;
DispatchStack undoStack;
RoadGraph roads;
HashTable patientHash;
HashTable ambulanceHash;
EmergencyBST emergencyIds;
int nextEmergencyId = 1001;

void loadData() {
    drivers.add("D01", "Arun Kumar", "900000001");
    drivers.add("D02", "Beena Shah", "900000002");
    drivers.add("D03", "Carlos Lee", "900000003");
    drivers.add("D04", "Divya Rao", "900000004");
    drivers.add("D05", "Ethan Roy", "900000005");
    drivers.add("D06", "Fatima Ali", "900000006");

    for (int i = 0; i < MAX_AMBULANCES; ++i)
        ambulanceHash.insert(ambulances[i].id, i);

    roads.addRoad(1, 2, 4);
    roads.addRoad(1, 4, 7);
    roads.addRoad(2, 3, 3);
    roads.addRoad(2, 5, 6);
    roads.addRoad(3, 6, 5);
    roads.addRoad(4, 5, 2);
    roads.addRoad(5, 6, 4);
    roads.addRoad(5, 7, 5);
    roads.addRoad(6, 8, 6);
    roads.addRoad(7, 8, 3);

    Emergency sample{1000, "P001", 6, 5, "Cardiac", "Waiting", ""};
    emergencies.add(sample);
    patientHash.insert(sample.patientId, sample.id);
    emergencyIds.insert(sample.id);
    incomingRequests.push(sample.id);
    waitingList.push(sample.id);
}

void listAmbulances() {
    cout << "\n--- Ambulances (Array) ---\n";
    for (const Ambulance& a : ambulances) {
        DriverNode* driver = drivers.find(a.driverId);
        cout << a.id << " | Location: " << cityNames[a.location]
             << " | Type: " << a.type
             << " | Driver: " << (driver ? driver->name : "Unknown")
             << " | " << (a.available ? "Available" : "Busy") << '\n';
    }
}

void registerEmergency() {
    Emergency e;
    e.id = nextEmergencyId++;
    cout << "Patient ID: ";
    cin >> e.patientId;
    int ignored;
    if (patientHash.find(e.patientId, ignored)) {
        cout << "Patient ID already exists.\n";
        return;
    }

    cout << "Location (1-" << MAX_LOCATIONS << "): ";
    cin >> e.location;
    if (e.location < 1 || e.location > MAX_LOCATIONS) {
        cout << "Invalid location.\n";
        return;
    }

    cout << "Severity (1-5): ";
    cin >> e.severity;
    if (e.severity < 1 || e.severity > 5) {
        cout << "Invalid severity.\n";
        return;
    }

    cout << "Emergency type: ";
    cin >> e.type;
    e.status = "Waiting";
    e.ambulanceId = "";

    emergencies.add(e);
    patientHash.insert(e.patientId, e.id);
    emergencyIds.insert(e.id);
    incomingRequests.push(e.id);

    if (!waitingList.push(e.id))
        cout << "Circular waiting list is full; request remains in the main queue.\n";
    else
        cout << "Emergency E" << e.id << " registered successfully.\n";
}

void dispatchAmbulance() {
    int emergencyId;
    cout << "Emergency ID to dispatch: ";
    cin >> emergencyId;
    Emergency* emergency = emergencies.find(emergencyId);
    if (!emergency || emergency->status != "Waiting") {
        cout << "Emergency not found or is not waiting.\n";
        return;
    }

    int distance[MAX_LOCATIONS + 1], parent[MAX_LOCATIONS + 1];
    dijkstra(roads, emergency->location, distance, parent);

    int best = -1;
    for (int i = 0; i < MAX_AMBULANCES; ++i) {
        if (ambulances[i].available &&
            (best == -1 || distance[ambulances[i].location] <
             distance[ambulances[best].location]))
            best = i;
    }

    if (best == -1 || distance[ambulances[best].location] == INF) {
        cout << "No available ambulance can reach the emergency.\n";
        return;
    }

    int oldLocation = ambulances[best].location;
    ambulances[best].available = false;
    emergency->status = "Dispatched";
    emergency->ambulanceId = ambulances[best].id;
    undoStack.push({emergencyId, best, oldLocation});

    cout << "Ambulance " << ambulances[best].id << " dispatched. ETA: "
         << distance[oldLocation] << " minutes.\n";
    printRoute(oldLocation, parent);
}

void undoDispatch() {
    if (undoStack.empty()) {
        cout << "Nothing to undo.\n";
        return;
    }
    DispatchAction action = undoStack.pop();
    Emergency* emergency = emergencies.find(action.emergencyId);
    if (!emergency) return;

    ambulances[action.ambulanceIndex].available = true;
    ambulances[action.ambulanceIndex].location = action.previousLocation;
    emergency->status = "Waiting";
    emergency->ambulanceId = "";
    cout << "Dispatch of E" << action.emergencyId << " undone.\n";
}

void completeEmergency() {
    int id;
    cout << "Emergency ID to complete: ";
    cin >> id;
    Emergency* e = emergencies.find(id);
    if (!e || e->status != "Dispatched") {
        cout << "Dispatched emergency not found.\n";
        return;
    }

    int index;
    if (ambulanceHash.find(e->ambulanceId, index)) {
        ambulances[index].available = true;
        ambulances[index].location = e->location;
    }
    e->status = "Completed";
    cout << "Emergency completed and ambulance released.\n";
}

void searchPatient() {
    string patientId;
    cout << "Patient ID to search: ";
    cin >> patientId;
    int emergencyId;
    if (!patientHash.find(patientId, emergencyId)) {
        cout << "Patient not found.\n";
        return;
    }
    Emergency* e = emergencies.find(emergencyId);
    if (e) EmergencyList::print(*e);
}

void processNextRequest() {
    int id = incomingRequests.pop();
    if (id == -1) cout << "No incoming requests.\n";
    else cout << "Next FIFO request is E" << id << ".\n";
}

void showMenu() {
    cout << "\n========== AMBULANCE MANAGEMENT SYSTEM ==========\n"
         << "1. Register emergency\n"
         << "2. Dispatch nearest ambulance\n"
         << "3. Undo latest dispatch\n"
         << "4. Complete emergency\n"
         << "5. List ambulances and drivers\n"
         << "6. Search patient\n"
         << "7. View emergency history\n"
         << "8. View reverse history\n"
         << "9. Show waiting circular queue\n"
         << "10. Process next FIFO request\n"
         << "11. Show sorted emergency IDs\n"
         << "12. Show road matrix\n"
         << "0. Exit\n"
         << "Choose: ";
}

int main() {
    loadData();
    cout << "Ambulance Management System using 10 data structures\n";

    int choice;
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: registerEmergency(); break;
            case 2: dispatchAmbulance(); break;
            case 3: undoDispatch(); break;
            case 4: completeEmergency(); break;
            case 5: listAmbulances(); drivers.display(); break;
            case 6: searchPatient(); break;
            case 7: emergencies.displayForward(); break;
            case 8: emergencies.displayBackward(); break;
            case 9: waitingList.display(); break;
            case 10: processNextRequest(); break;
            case 11: emergencyIds.displaySorted(); break;
            case 12: roads.display(); break;
            case 0: cout << "Exiting safely.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
