#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

struct LogEntry {
    std::string date;
    std::string time;
    std::string ip;
    std::string message;

    // Function to convert date and time into a single sortable string
    std::string getSortableDateTime() const {
        return date + " " + time;
    }
};

std::vector<LogEntry> readLogFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<LogEntry> logs;
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return logs;
    }

    std::string line;
    while (std::getline(file, line)) {
        LogEntry entry;
        std::istringstream iss(line);
        iss >> entry.date >> entry.time >> entry.ip;
        std::getline(iss, entry.message); // Get the rest of the line as message
        logs.push_back(entry);
    }

    file.close();
    return logs;
}

void writeSortedLogs(const std::vector<LogEntry>& logs, int teamNumber) {
    std::ofstream file("bitacoraOrdenada1.3-eq" + std::to_string(teamNumber) + ".txt");
    
    for (const auto& log : logs) {
        file << log.getSortableDateTime() << " " << log.ip << " " << log.message << "\n";
    }

    file.close();
}

std::vector<LogEntry> searchLogs(const std::vector<LogEntry>& logs, const std::string& startDateTime, const std::string& endDateTime) {
    std::vector<LogEntry> results;

    for (const auto& log : logs) {
        if (log.getSortableDateTime() >= startDateTime && log.getSortableDateTime() <= endDateTime) {
            results.push_back(log);
        }
    }

    return results;
}

void writeSearchResults(const std::vector<LogEntry>& results, int searchNumber, int teamNumber) {
    std::ofstream file("salida" + std::to_string(searchNumber) + "-eq" + std::to_string(teamNumber) + ".txt");
	
    for (const auto& log : results) {
        file << log.getSortableDateTime() << " " << log.ip << " " << log.message << "\n";
    }

    file.close();
}

int main() {
    int teamNumber;
    std::cout << "Ingrese el número del equipo: ";
    std::cin >> teamNumber;

    // Read and sort logs
    auto logs = readLogFile("bitacora.txt");
    
    // Sort logs by date and time
    std::sort(logs.begin(), logs.end(), [](const LogEntry& a, const LogEntry& b) {
        return a.getSortableDateTime() < b.getSortableDateTime();
    });

    writeSortedLogs(logs, teamNumber);

    int searchNumber = 1;
    
    while (true) {
        std::string startDate, endDate;
        std::cout << "Ingrese la fecha de inicio (Ej. Oct 5 10:00:00): ";
        std::getline(std::cin >> std::ws, startDate);
        
        std::cout << "Ingrese la fecha de fin (Ej. Oct 10 23:59:59): ";
        std::getline(std::cin >> std::ws, endDate);

        auto results = searchLogs(logs, startDate, endDate);
        
        writeSearchResults(results, searchNumber, teamNumber);

        char again;
        std::cout << "¿Desea hacer una nueva búsqueda? (s/n): ";
        std::cin >> again;

        if (again != 's') break;

        searchNumber++;
    }

    return 0;
}
