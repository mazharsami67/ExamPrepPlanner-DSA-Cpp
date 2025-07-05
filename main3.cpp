#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
using namespace std;

// Topic structure
struct Topic {
    string name;
    string status;       
    string difficulty;    
    int estimatedTime;    // in hours
    string dueDate;
};

// Subject structure
struct Subject {
    string name;
    string examDate;
    vector<Topic> topics;
};

// For undo functionality
struct Change {
    string subjectName;
    string topicName;
    string oldStatus;
};

// Global variables
vector<Subject> subjects;
stack<Change> undoStack;
queue<string> studyScheduleQueue;  

// Function prototypes
void displayMenu();
void addSubject();
void addTopic();
void updateStatus();
void viewProgress();
void undoLastChange();
void sortTopics();
void manageStudySchedule();
void bubbleSortByTime(vector<Topic>& topics);
int findSubject(string name);
int findTopic(int subjectIndex, string topicName);

int main() {
    int choice;
    
      cout << " EXAM PREPARATION PLANNER " << endl;
      cout << "Welcome to your study organizer!" << endl;
    
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addSubject();
                break;
            case 2:
                addTopic();
                break;
            case 3:
                updateStatus();
                break;
            case 4:
                viewProgress();
                break;
            case 5:
                undoLastChange();
                break;
            case 6:
                sortTopics();
                break;
            case 7:
                manageStudySchedule();
                break;
            case 8:
                cout << "\nThank you for using Exam Preparation Planner!" << endl;
                cout << "Good luck with your studies!" << endl;
                break;
            default:
                cout << "\nInvalid choice! Please try again." << endl;
        }
        
        if(choice != 8) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while(choice != 8);
    
    return 0;
}

void displayMenu() {
    
    cout << "        MAIN MENU" << endl;
    cout << " " << endl;
    cout << "1. Add New Subject" << endl;
    cout << "2. Add New Topic" << endl;
    cout << "3. Update Topic Status" << endl;
    cout << "4. View Progress" << endl;
    cout << "5. Undo Last Change" << endl;
    cout << "6. Sort Topics by Time" << endl;
    cout << "7. Manage Study Schedule" << endl;
    cout << "8. Exit" << endl;
   
    
    // Show queue size if not empty
    if(!studyScheduleQueue.empty()) {
        cout << "📚 " << studyScheduleQueue.size() << " topics in study queue" << endl;
    }
}

void addSubject() {
    Subject newSubject;
    
    cout << "\nADD NEW SUBJECT" << endl;
    cout << "Enter subject name: ";
    cin.ignore();
    getline(cin, newSubject.name);
    
    cout << "Enter exam date (dd-mm-yyyy): ";
    getline(cin, newSubject.examDate);
    
    subjects.push_back(newSubject);
    cout << "\nSubject '" << newSubject.name << "' added successfully!" << endl;
}

void addTopic() {
    if(subjects.empty()) {
        cout << "\nNo subjects available! Please add a subject first." << endl;
        return;
    }
    
    cout << "\n ADD NEW TOPIC" << endl;
    cout << "Available subjects:" << endl;
    for(int i = 0; i < subjects.size(); i++) {
        cout << (i+1) << ". " << subjects[i].name << endl;
    }
    
    string subjectName;
    cout << "Enter subject name: ";
    cin.ignore();
    getline(cin, subjectName);
    
    int subjectIndex = findSubject(subjectName);
    if(subjectIndex == -1) {
        cout << "Subject not found!" << endl;
        return;
    }
    
    Topic newTopic;
    cout << "Enter topic name: ";
    getline(cin, newTopic.name);
    
    cout << "Enter difficulty (Easy/Medium/Hard): ";
    getline(cin, newTopic.difficulty);
    
    cout << "Enter estimated time (hours): ";
    cin >> newTopic.estimatedTime;
    
    cout << "Enter due date (dd-mm-yyyy): ";
    cin.ignore();
    getline(cin, newTopic.dueDate);
    
    newTopic.status = "Not Started";
    subjects[subjectIndex].topics.push_back(newTopic);
    
    cout << "\nTopic '" << newTopic.name << "' added successfully!" << endl;
}

void updateStatus() {
    if(subjects.empty()) {
        cout << "\nNo subjects available!" << endl;
        return;
    }
    
    cout << "\n UPDATE TOPIC STATUS " << endl;
    
    string subjectName, topicName;
    cout << "Enter subject name: ";
    cin.ignore();
    getline(cin, subjectName);
    
    int subjectIndex = findSubject(subjectName);
    if(subjectIndex == -1) {
        cout << "Subject not found!" << endl;
        return;
    }
    
    if(subjects[subjectIndex].topics.empty()) {
        cout << "No topics in this subject!" << endl;
        return;
    }
    
    cout << "Topics in " << subjectName << ":" << endl;
    for(int i = 0; i < subjects[subjectIndex].topics.size(); i++) {
        cout << (i+1) << ". " << subjects[subjectIndex].topics[i].name 
             << " [" << subjects[subjectIndex].topics[i].status << "]" << endl;
    }
    
    cout << "Enter topic name: ";
    getline(cin, topicName);
    
    int topicIndex = findTopic(subjectIndex, topicName);
    if(topicIndex == -1) {
        cout << "Topic not found!" << endl;
        return;
    }
    
    Topic& topic = subjects[subjectIndex].topics[topicIndex];
    cout << "Current status: " << topic.status << endl;
    cout << "Choose new status:" << endl;
    cout << "1. Not Started" << endl;
    cout << "2. In Progress" << endl;
    cout << "3. Completed" << endl;
    
    int statusChoice;
    cout << "Enter choice (1-3): ";
    cin >> statusChoice;
    
    // Save old status for undo
    Change change;
    change.subjectName = subjectName;
    change.topicName = topicName;
    change.oldStatus = topic.status;
    undoStack.push(change);
    
    // Update status
    switch(statusChoice) {
        case 1: topic.status = "Not Started"; break;
        case 2: topic.status = "In Progress"; break;
        case 3: topic.status = "Completed"; break;
        default: 
            cout << "Invalid choice!" << endl;
            undoStack.pop(); // Remove the change since it wasn't applied
            return;
    }
    
    cout << "Status updated successfully!" << endl;
}

void viewProgress() {
    if(subjects.empty()) {
        cout << "\nNo subjects available!" << endl;
        return;
    }
    
    cout << "\nPROGRESS REPORT:" << endl;
    
    for(int i = 0; i < subjects.size(); i++) {
        Subject& subject = subjects[i];
        cout << "\nSubject: " << subject.name << endl;
        cout << "Exam Date: " << subject.examDate << endl;
        cout << "Topics:" << endl;
        
        int completed = 0, inProgress = 0, notStarted = 0;
        
        for(int j = 0; j < subject.topics.size(); j++) {
            Topic& topic = subject.topics[j];
            cout << "  " << (j+1) << ". " << topic.name 
                 << " [" << topic.status << "] "
                 << "(" << topic.difficulty << ", " 
                 << topic.estimatedTime << "hrs)" << endl;
            
            if(topic.status == "Completed") completed++;
            else if(topic.status == "In Progress") inProgress++;
            else notStarted++;
        }
        
        int total = subject.topics.size();
        if(total > 0) {
            double percentage = (completed * 100.0) / total;
            cout << "Progress: " << completed << "/" << total 
                 << " (" << percentage << "% completed)" << endl;
            cout << "Not Started: " << notStarted << ", In Progress: " 
                 << inProgress << ", Completed: " << completed << endl;
        } else {
            cout << "No topics added yet." << endl;
        }
        cout << "------------------------" << endl;
    }
}

void undoLastChange() {
    if(undoStack.empty()) {
        cout << "\nNothing to undo!" << endl;
        return;
    }
    
    Change lastChange = undoStack.top();
    undoStack.pop();
    
    int subjectIndex = findSubject(lastChange.subjectName);
    if(subjectIndex == -1) {
        cout << "Error: Subject not found for undo!" << endl;
        return;
    }
    
    int topicIndex = findTopic(subjectIndex, lastChange.topicName);
    if(topicIndex == -1) {
        cout << "Error: Topic not found for undo!" << endl;
        return;
    }
    
    subjects[subjectIndex].topics[topicIndex].status = lastChange.oldStatus;
    cout << "\nUndo successful! '" << lastChange.topicName 
         << "' status reverted to '" << lastChange.oldStatus << "'" << endl;
}

void sortTopics() {
    if(subjects.empty()) {
        cout << "\nNo subjects available!" << endl;
        return;
    }
    
    cout << "\n  SORT TOPICS BY TIME :" << endl;
    cout << "Available subjects:" << endl;
    for(int i = 0; i < subjects.size(); i++) {
        cout << (i+1) << ". " << subjects[i].name << endl;
    }
    
    string subjectName;
    cout << "Enter subject name to sort: ";
    cin.ignore();
    getline(cin, subjectName);
    
    int subjectIndex = findSubject(subjectName);
    if(subjectIndex == -1) {
        cout << "Subject not found!" << endl;
        return;
    }
    
    if(subjects[subjectIndex].topics.empty()) {
        cout << "No topics to sort!" << endl;
        return;
    }
    
    bubbleSortByTime(subjects[subjectIndex].topics);
    cout << "Topics sorted by estimated time (shortest first)!" << endl;
    
    // Show sorted topics
    cout << "\nSorted topics:" << endl;
    for(int i = 0; i < subjects[subjectIndex].topics.size(); i++) {
        Topic& topic = subjects[subjectIndex].topics[i];
        cout << (i+1) << ". " << topic.name << " (" 
             << topic.estimatedTime << " hours)" << endl;
    }
}

void manageStudySchedule() {
    cout << "\n STUDY SCHEDULE QUEUE (FIFO) " << endl;
    cout << "1. Add topic to study queue" << endl;
    cout << "2. View next topic to study" << endl;
    cout << "3. Remove completed topic from queue" << endl;
    cout << "4. View entire study queue" << endl;
    cout << "5. Clear study queue" << endl;
    
    int choice;
    cout << "Enter choice: ";
    cin >> choice;
    
    switch(choice) {
        case 1: {
            // Add topic to queue
            if(subjects.empty()) {
                cout << "No subjects available!" << endl;
                return;
            }
            
            string subjectName, topicName;
            cout << "Enter subject name: ";
            cin.ignore();
            getline(cin, subjectName);
            cout << "Enter topic name: ";
            getline(cin, topicName);
            
            int subjectIndex = findSubject(subjectName);
            if(subjectIndex == -1) {
                cout << "Subject not found!" << endl;
                return;
            }
            
            int topicIndex = findTopic(subjectIndex, topicName);
            if(topicIndex == -1) {
                cout << "Topic not found!" << endl;
                return;
            }
            
            string fullTopicName = subjectName + " - " + topicName;
            studyScheduleQueue.push(fullTopicName);
            cout << "'" << topicName << "' added to study queue!" << endl;
            cout << "Queue size: " << studyScheduleQueue.size() << endl;
            break;
        }
        
        case 2: {
            // View next topic
            if(studyScheduleQueue.empty()) {
                cout << "Study queue is empty!" << endl;
            } else {
                cout << "Next topic to study: " << studyScheduleQueue.front() << endl;
                cout << "Queue size: " << studyScheduleQueue.size() << endl;
            }
            break;
        }
        
        case 3: {
            // Remove completed topic
            if(studyScheduleQueue.empty()) {
                cout << "Study queue is empty!" << endl;
            } else {
                cout << "Completed topic: " << studyScheduleQueue.front() << endl;
                studyScheduleQueue.pop();
                cout << "Topic removed from queue!" << endl;
                cout << "Remaining in queue: " << studyScheduleQueue.size() << endl;
            }
            break;
        }
        
        case 4: {
            // View entire queue
            if(studyScheduleQueue.empty()) {
                cout << "Study queue is empty!" << endl;
            } else {
                cout << "Study Queue (First In, First Out):" << endl;
                cout << "Queue size: " << studyScheduleQueue.size() << endl;
                cout << "Next topic: " << studyScheduleQueue.front() << endl;
                cout << "(Use option 3 to process topics one by one)" << endl;
            }
            break;
        }
        
        case 5: {
            // Clear queue
            while(!studyScheduleQueue.empty()) {
                studyScheduleQueue.pop();
            }
            cout << "Study queue cleared!" << endl;
            break;
        }
        
        default:
            cout << "Invalid choice!" << endl;
    }
}

void bubbleSortByTime(vector<Topic>& topics) {
    int n = topics.size();
    
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(topics[j].estimatedTime > topics[j+1].estimatedTime) {
                // Swapping topics
                Topic temp = topics[j];
                topics[j] = topics[j+1];
                topics[j+1] = temp;
            }
        }
    }
}

int findSubject(string name) {
    for(int i = 0; i < subjects.size(); i++) {
        if(subjects[i].name == name) {
            return i;
        }
    }
    return -1; // Not found
}

int findTopic(int subjectIndex, string topicName) {
    for(int i = 0; i < subjects[subjectIndex].topics.size(); i++) {
        if(subjects[subjectIndex].topics[i].name == topicName) {
            return i;
        }
    }
    return -1; // Not found
}