#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For sort
#include <climits>   // For INT_MAX
#include <limits>    // For numeric_limits
#include <cctype>    // For isdigit
#include <queue>     // For priority_queue

using namespace std;

const int V = 5;  // Number of locations
const int H = 2;  // Number of hospitals per location

// Function for login
bool login() {
    string username, password, phoneNumber;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Check if username and password match the allowed credentials
    if ((username == "Ashutosh" && password == "Ashutosh") ||
        (username == "Satkar" && password == "Satkar")) {

        // Ask for phone number and validate it
        cout << "Enter your 10-digit phone number: ";
        cin >> phoneNumber;

        // Validate phone number length and check if all characters are digits
        if (phoneNumber.length() != 10) {
            cout << "Invalid phone number. Please enter a valid 10-digit number.\n";
            return false;
        }

        for (char c : phoneNumber) {
            if (!isdigit(c)) {
                cout << "Invalid phone number. Please enter a valid 10-digit number.\n";
                return false;
            }
        }

        cout << "Login successful!\n";
        return true;
    } else {
        cout << "Invalid credentials. Access denied!\n";
        return false;
    }
}

// Function to display choices to the user
int getUserChoice() {
    int choice;
    cout << "\nWhat would you like to do?\n";
    cout << "1. Emergency: I need an ambulance\n";
    cout << "2. Check the nearest hospital and its distance\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;
    return choice;
}

// Function to perform Dijkstra's algorithm
vector<double> dijkstra(double graph[V][H], int start) {
    vector<double> distances(V * H, INT_MAX); // Distance to each hospital
    distances[start] = 0; // Distance to the starting node is 0
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDist > distances[currentNode]) {
            continue; // Skip outdated entry
        }

        // Explore neighbors (hospitals)
        for (int i = 0; i < H; i++) {
            int neighbor = currentNode * H + i; // Calculate neighbor index
            double edgeWeight = graph[currentNode][i];

            if (edgeWeight >= 0) { // Only consider valid distances
                double newDist = currentDist + edgeWeight;
                if (newDist < distances[neighbor]) {
                    distances[neighbor] = newDist;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    return distances; // Return the distances array
}

// Function to calculate distances between user's location and hospitals
void calculateDistances(double graph[V][H], int locationIndex, const vector<string>& hospitals) {
    vector<double> distances = dijkstra(graph, locationIndex); // Get shortest distances
    vector<pair<double, int>> nearestHospitals;

    // Collect nearest hospitals based on distances
    for (int i = 0; i < H; i++) {
        int hospitalIndex = locationIndex * H + i;
        if (distances[hospitalIndex] < INT_MAX) {
            nearestHospitals.push_back({distances[hospitalIndex], i});
        }
    }

    // Sort by distance and show up to two nearest hospitals
    sort(nearestHospitals.begin(), nearestHospitals.end());

    // Display the correct output message
    cout << "\nThe two nearest hospitals with ambulance service:\n";

    for (int i = 0; i < 2 && i < nearestHospitals.size(); i++) {
        cout << i + 1 << ". " << hospitals[locationIndex * H + nearestHospitals[i].second] 
             << " - " << nearestHospitals[i].first << " km\n";
    }

    // Ask user to select a hospital
    int hospitalChoice;

    cout << "\nEnter the number of the hospital you want an ambulance from: ";

    while (true) { // Loop until valid input is received
        cin >> hospitalChoice;

        if (cin.fail() || hospitalChoice < 1 || hospitalChoice > nearestHospitals.size()) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid selection. Please select a valid option.\n";
            cout << "\nEnter the number of the hospital you want an ambulance from: ";
        } else {
            break; // Valid input received, exit loop
        }
    }

    cout << "Processing... We will reach your location soon. Please be ready!\n";
}

// Function to display available locations
void displayLocations(const vector<string>& locations) {
    cout << "\nAvailable locations:\n";
    for (const auto& location : locations) {
        cout << "- " << location << endl;
    }
}

// Function to display locations and check hospital distances
void checkNearestHospitals(double graph[V][H], const vector<string>& hospitals, const vector<string>& locations) {
    cout << "\nAvailable locations and their hospitals with distances:\n";
    for (int i = 0; i < V; i++) {
        cout << locations[i] << ":\n";
        for (int j = 0; j < H; j++) {
            cout << "  " << hospitals[i * H + j] << " - " << graph[i][j] << " km\n"; // Show distances
        }
    }

    // Show nearest hospitals (no need to ask for user input)
    cout << "\nThank you for using our service. Exiting...\n";
}

// Function to convert string to lowercase
string toLower(const string& str) {
    string lowerStr = str;
    for (char& c : lowerStr) {
        c = tolower(c);
    }
    return lowerStr;
}

int main() {
    if (!login()) {
        return 0; // Terminate if login fails
    }

    // List of locations
    vector<string> locations = {
        "Gongabu", "Durbarmarg", "Buddhanilkantha", "Chabahil", "Teku"
    };

    // Unique hospital names near each location
    vector<string> hospitals = {
        // Hospitals for Gongabu
        "Grande International Hospital",  // Index 0 for Gongabu
        "HAMS Hospital",                  // Index 1 for Gongabu

        // Hospitals for Durbarmarg
        "Grande City Hospital",            // Index 2 for Durbarmarg
        "Bir Hospital",                    // Index 3 for Durbarmarg

        // Hospitals for Buddhanilkantha
        "Karuna Hospital",                 // Index 4 for Buddhanilkantha
        "Neuro Hospital",                  // Index 5 for Buddhanilkantha

        // Hospitals for Chabahil
        "Om Hospital & Research Center",   // Index 6 for Chabahil
        "Dirghayu Guru Hospital & Research Center", // Index 7 for Chabahil

        // Hospitals for Teku
        "Teku Hospital",                   // Index 8 for Teku
        "Sukraraj Tropical & Infectious Disease Hospital" // Index 9 for Teku
    };

    // Actual distance matrix between locations and hospitals (in km)
    double graph[V][H] = {
        {0.0, 1.0},  // Gongabu: 0 km to itself, 1 km to Grande International Hospital, 3.9 km to HAMS Hospital
        {1.4, 2.3},  // Durbarmarg: Grande City Hospital, Bir Hospital
        {3.2, 4.4},  // Buddhanilkantha: Karuna Hospital, Neuro Hospital
        {1.0, 1.0},  // Chabahil: Om Hospital, Dirghayu Guru Hospital
        {1.0, 1.0}   // Teku: Teku Hospital, Sukraraj Tropical
    };

    // Ask user for choice
    int choice = getUserChoice();
    if (choice == 1) {
        // Display available locations
        displayLocations(locations);
        
        // Handle ambulance request
        cout << "Please enter your location to request an ambulance.\n";
        string location;
        cout << "Enter your location: ";
        cin.ignore(); // To ignore any leftover newline from previous input
        getline(cin, location); // Use getline to capture the whole line

        // Check for valid location
        bool validLocation = false;
        int locationIndex = -1; // Initialize index

        for (int i = 0; i < V; i++) {
            if (toLower(locations[i]) == toLower(location)) {
                validLocation = true;
                locationIndex = i; // Save the index
                break;
            }
        }

        if (validLocation) {
            calculateDistances(graph, locationIndex, hospitals); // Proceed to calculate distances
        } else {
            cout << "Invalid location entered. Please try again.\n";
        }
    } else if (choice == 2) {
        // Display nearest hospitals
        checkNearestHospitals(graph, hospitals, locations);
    } else {
        cout << "Invalid choice. Exiting...\n"; // Handle invalid choice
    }

    return 0; // Successful exit
}
