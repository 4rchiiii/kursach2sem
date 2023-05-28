#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <Windows.h>

const int NotUsed = system("color 30");

class Task {
private:
    std::string date;
    std::string time;
    std::string task;
    std::string description;

public:
    Task(const std::string& date, const std::string& time, const std::string& task, const std::string& description)
        : date(date), time(time), task(task), description(description) {}

    bool isValidDateFormat() const { // ���������, �������� �� ������ ���� ��������������
        if (date.length() != 10) {
            return false;
        }
        if (date[2] != '.' || date[5] != '.') {
            return false;
        }
        for (int i = 0; i < 10; ++i) {
            if (i == 2 || i == 5) {
                continue;
            }
            if (!isdigit(date[i])) {
                return false;
            }
        }
        return true;
    }

    bool isValidTimeFormat() const { // ���������, �������� �� ������ ������� ��������������
        if (time.length() != 5) {
            return false;
        }
        if (time[2] != ':') {
            return false;
        }
        for (int i = 0; i < 5; ++i) {
            if (i == 2) {
                continue;
            }
            if (!isdigit(time[i])) {
                return false;
            }
        }
        int hours = stoi(time.substr(0, 2));
        int minutes = stoi(time.substr(3, 2));
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
            return false;
        }
        return true;
    }

    void printTask(int taskNumber) const { // ������� ���������� � ������
        std::cout << taskNumber << ". " << date << " " << time << " | " << task << " | " << description << std::endl;
    }

    bool operator==(const std::string& dateToCompare) const { // ���������, ��������� �� ���� � �������� �����
        return date == dateToCompare;
    }

    const std::string& getDate() const { // ���������� ����
        return date;
    }

    const std::string& getTime() const { // ���������� �����
        return time;
    }

    const std::string& getTask() const { // ���������� ������
        return task;
    }

    const std::string& getDescription() const { // ���������� �������� ������
        return description;
    }
};

std::vector<std::string> splitString(const std::string& str, char delimiter) { // ��������� ������ �� ��������� �� ��������� �����������
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void addTask(const std::string& filename) { // ��������� ����� ������ � ����
    std::ofstream file(filename, std::ios::app); // ��������� ���� ��� ���������� ������ � �����
    if (!file) {
        std::cout << "������ �������� �����!" << std::endl;
        return;
    }

    std::string date, time, task, description;
    std::cout << "������� ���� (��.��.����): ";
    std::cin >> date;
    if (!Task(date, "", "", "").isValidDateFormat()) { // ��������� ������ ����
        std::cout << "�������� ������ ����!" << std::endl;
        return;
    }
    std::cout << "������� ����� (��:��): ";
    std::cin >> time;
    if (!Task("", time, "", "").isValidTimeFormat()) { // ��������� ������ �������
        std::cout << "�������� ������ �������!" << std::endl;
        return;
    }
    std::cin.ignore();  // ������� ������ �����
    std::cout << "������� ������: ";
    getline(std::cin, task);
    std::cout << "������� �������� ������: ";
    getline(std::cin, description);

    // ��������� �������� ���������� ����� � �����
    std::ifstream inputFile(filename);
    int taskCount = std::count(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>(), '\n');
    inputFile.close();

    file << taskCount + 1 << ". " << date << " " << time << " | " << task << " | " << description << std::endl;

    std::cout << "������ ���������!" << std::endl;

    file.close();
}

void deleteTask(const std::string& filename) { // ������� ������ �� �����
    int taskNumber;
    std::cout << "������� ����� ������ ��� ��������: ";
    std::cin >> taskNumber;
    if (taskNumber < 1) {
        std::cout << "�������� ����� ������!" << std::endl;
        return;
    }

    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cout << "������ �������� �����!" << std::endl;
        return;
    }

    std::ofstream tempFile("temp.txt");
    if (!tempFile) {
        std::cout << "������ �������� ���������� �����!" << std::endl;
        inputFile.close();
        return;
    }

    std::string line;
    int currentTaskNumber = 1;
    bool taskFound = false;
    while (getline(inputFile, line)) {
        if (currentTaskNumber == taskNumber) {
            taskFound = true;
        }
        else {
            tempFile << line << std::endl;
        }
        currentTaskNumber++;
    }

    inputFile.close();
    tempFile.close();

    if (taskFound) {
        remove(filename.c_str()); // ������� �������� ����
        rename("temp.txt", filename.c_str()); // ��������������� ��������� ���� � �������� ��� �����
        std::cout << "������ �������!" << std::endl;
    }
    else {
        remove("temp.txt");
        std::cout << "������ �� �������!" << std::endl;
    }
}

void editTask(const std::string& filename) { // ����������� ������ � �����
    int taskNumber;
    std::cout << "������� ����� ������ ��� ��������������: ";
    std::cin >> taskNumber;
    if (taskNumber < 1) {
        std::cout << "�������� ����� ������!" << std::endl;
        return;
    }

    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cout << "������ �������� �����!" << std::endl;
        return;
    }

    std::ofstream tempFile("temp.txt");
    if (!tempFile) {
        std::cout << "������ �������� ���������� �����!" << std::endl;
        inputFile.close();
        return;
    }

    std::string line;
    int currentTaskNumber = 1;
    bool taskFound = false;
    while (getline(inputFile, line)) {
        if (currentTaskNumber == taskNumber) {
            taskFound = true;

            std::string editedDate, editedTime, editedTask, editedDescription;
            std::cin.ignore();  // ������� ������ �����
            std::cout << "������� ���� (��.��.����): ";
            getline(std::cin, editedDate);
            std::cout << "������� ����� (��:��): ";
            getline(std::cin, editedTime);
            std::cout << "������� ����������������� ������: ";
            getline(std::cin, editedTask);
            std::cout << "������� ����������������� �������� ������: ";
            getline(std::cin, editedDescription);
            tempFile << currentTaskNumber << ". " << editedDate << " " << editedTime << " | " << editedTask << " | " << editedDescription << std::endl;

            std::cout << "������ ���������������!" << std::endl;
        }
        else {
            tempFile << line << std::endl;
        }
        currentTaskNumber++;
    }

    inputFile.close();
    tempFile.close();

    if (taskFound) {
        remove(filename.c_str()); // ������� �������� ����
        rename("temp.txt", filename.c_str()); // ��������������� ��������� ���� � �������� ��� �����
    }
    else {
        remove("temp.txt");
        std::cout << "������ �� �������!" << std::endl;
    }
}

void displayTasks(const std::string& filename) { // ������� ��� ������ �� �����
    std::ifstream file(filename);
    if (!file) {
        std::cout << "������ �������� �����!" << std::endl;
        return;
    }

    std::string line;
    int taskNumber = 1;
    while (getline(file, line)) {
        std::cout << taskNumber << ". " << line << std::endl;
        ++taskNumber;
    }

    file.close();
}

int main() {
    SetConsoleOutputCP(1251); // ������������� ��������� ������� ��� ������ �� ������� �����
    SetConsoleCP(1251); // ������������� ��������� ������� ��� ����� �� ������� �����
    setlocale(LC_ALL, "Russian"); // ������������� ������ ��� ����������� ����������� ������� ��������
    std::string filename = "tasks.txt";
    std::ifstream file(filename);
    if (!file) { // ���� ���� �� ����������, ��������� ����� ���� � ������ "tasks.txt"
        std::ofstream newFile(filename);
        if (!newFile) {
            std::cout << "������ �������� �����!" << std::endl;
            return 1;
        }
        newFile.close();
    }
    else {
        file.close();
    }

    int choice;
    do {
        std::cout << "==================================== \n";
        std::cout << "|             ����������           |" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "1. �������� ������" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "2. ������� ������" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "3. ������������� ������" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "4. �������� ��� ������" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "5. �����" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "������� ����� ��������: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            addTask(filename);
            break;
        case 2:
            deleteTask(filename);
            break;
        case 3:
            editTask(filename);
            break;
        case 4:
            displayTasks(filename);
            break;
        case 5:
            std::cout << "�� ��������!" << std::endl;
            break;
        default:
            std::cout << "�������� ����� ��������!" << std::endl;
            break;
        }
    } while (choice != 5);

    return 0;
}