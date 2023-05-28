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

    bool isValidDateFormat() const { // Проверяет, является ли формат даты действительным
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

        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));

        if (day < 1 || day > 31) {
            return false;
        }

        if (month < 1 || month > 12) {
            return false;
        }

        return true;
    }

    bool isValidTimeFormat() const { // Проверяет, является ли формат времени действительным
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

    void printTask(int taskNumber) const { // Выводит информацию о задаче
        std::cout << taskNumber << ". " << date << " " << time << " | " << task << " | " << description << std::endl;
    }

    bool operator==(const std::string& dateToCompare) const { // Проверяет, совпадает ли дата с заданной датой
        return date == dateToCompare;
    }

    const std::string& getDate() const { // Возвращает дату
        return date;
    }

    const std::string& getTime() const { // Возвращает время
        return time;
    }

    const std::string& getTask() const { // Возвращает задачу
        return task;
    }

    const std::string& getDescription() const { // Возвращает описание задачи
        return description;
    }
};

std::vector<std::string> splitString(const std::string& str, char delimiter) { // Разделяет строку на подстроки по заданному разделителю
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void addTask(const std::string& filename) { // Добавляет новую задачу в файл
    std::ofstream file(filename, std::ios::app); // Открывает файл для добавления данных в конец
    if (!file) {
        std::cout << "Ошибка открытия файла!" << std::endl;
        return;
    }

    std::string date, time, task, description;
    std::cout << "Введите дату (ДД.ММ.ГГГГ): ";
    std::cin >> date;
    if (!Task(date, "", "", "").isValidDateFormat()) { // Проверяет формат даты
        std::cout << "Неверный формат даты!" << std::endl;
        return;
    }
    std::cout << "Введите время (ЧЧ:ММ): ";
    std::cin >> time;
    if (!Task("", time, "", "").isValidTimeFormat()) { // Проверяет формат времени
        std::cout << "Неверный формат времени!" << std::endl;
        return;
    }
    std::cin.ignore();  // Очистка буфера ввода
    std::cout << "Введите задачу: ";
    getline(std::cin, task);
    std::cout << "Введите описание задачи: ";
    getline(std::cin, description);

    // Получение текущего количества задач в файле
    std::ifstream inputFile(filename);
    int taskCount = std::count(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>(), '\n');
    inputFile.close();

    file << taskCount + 1 << ". " << date << " " << time << " | " << task << " | " << description << std::endl;

    std::cout << "Задача добавлена!" << std::endl;

    file.close();
}

void deleteTask(const std::string& filename) { // Удаляет задачу из файла
    int taskNumber;
    std::cout << "Введите номер задачи для удаления: ";
    std::cin >> taskNumber;
    if (taskNumber < 1) {
        std::cout << "Неверный номер задачи!" << std::endl;
        return;
    }

    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cout << "Ошибка открытия файла!" << std::endl;
        return;
    }

    std::ofstream tempFile("temp.txt");
    if (!tempFile) {
        std::cout << "Ошибка создания временного файла!" << std::endl;
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
        remove(filename.c_str()); // Удаляет исходный файл
        rename("temp.txt", filename.c_str()); // Переименовывает временный файл в исходное имя файла
        std::cout << "Задача удалена!" << std::endl;
    }
    else {
        remove("temp.txt");
        std::cout << "Задача не найдена!" << std::endl;
    }
}

void editTask(const std::string& filename) { // Редактирует задачу в файле
    int taskNumber;
    std::cout << "Введите номер задачи для редактирования: ";
    std::cin >> taskNumber;
    if (taskNumber < 1) {
        std::cout << "Неверный номер задачи!" << std::endl;
        return;
    }

    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cout << "Ошибка открытия файла!" << std::endl;
        return;
    }

    std::ofstream tempFile("temp.txt");
    if (!tempFile) {
        std::cout << "Ошибка создания временного файла!" << std::endl;
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
            std::cin.ignore();  // Очистка буфера ввода

            do {
                std::cout << "Введите дату (ДД.ММ.ГГГГ): ";
                getline(std::cin, editedDate);
                Task tempTask(editedDate, "", "", "");
                if (!tempTask.isValidDateFormat()) {
                    std::cout << "Некорректный формат даты. Попробуйте еще раз." << std::endl;
                }
            } while (!Task(editedDate, "", "", "").isValidDateFormat());

            do {
                std::cout << "Введите время (ЧЧ:ММ): ";
                getline(std::cin, editedTime);
                Task tempTask("", editedTime, "", "");
                if (!tempTask.isValidTimeFormat()) {
                    std::cout << "Некорректный формат времени. Попробуйте еще раз." << std::endl;
                }
            } while (!Task("", editedTime, "", "").isValidTimeFormat());

            std::cout << "Введите отредактированную задачу: ";
            getline(std::cin, editedTask);
            std::cout << "Введите отредактированное описание задачи: ";
            getline(std::cin, editedDescription);
            tempFile << currentTaskNumber << ". " << editedDate << " " << editedTime << " | " << editedTask << " | " << editedDescription << std::endl;

            std::cout << "Задача отредактирована!" << std::endl;
        }
        else {
            tempFile << line << std::endl;
        }
        currentTaskNumber++;
    }

    inputFile.close();
    tempFile.close();

    if (taskFound) {
        remove(filename.c_str()); // Удаляет исходный файл
        rename("temp.txt", filename.c_str()); // Переименовывает временный файл в исходное имя файла
    }
    else {
        remove("temp.txt");
        std::cout << "Задача не найдена!" << std::endl;
    }
}

void displayTasks(const std::string& filename) { // Выводит все задачи из файла
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Ошибка открытия файла!" << std::endl;
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
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");
    std::string filename = "tasks.txt";
    std::ifstream file(filename);
    if (!file) { // Если файл не существует, создается новый файл с именем "tasks.txt"
        std::ofstream newFile(filename);
        if (!newFile) {
            std::cout << "Ошибка создания файла!" << std::endl;
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
        std::cout << "|             ЕЖЕДНЕВНИК           |" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "1. Добавить задачу" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "2. Удалить задачу" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "3. Редактировать задачу" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "4. Показать все задачи" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "5. Выйти" << std::endl;
        std::cout << "==================================== \n";
        std::cout << "Введите номер действия: ";
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
            std::cout << "До свидания!" << std::endl;
            break;
        default:
            std::cout << "Неверный номер действия!" << std::endl;
            break;
        }
    } while (choice != 5);

    return 0;
}