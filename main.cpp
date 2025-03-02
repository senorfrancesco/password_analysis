#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <windows.h>

// Структура для хранения данных о пароле
struct PasswordData {
    std::string value;       // Сам пароль
    int occurrence;          // Количество встречаемости
    double probability;      // Вероятность появления
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::ifstream file("occurrence.100K.txt");
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл!" << std::endl;
        return 1;
    }

    std::vector<PasswordData> passwords;
    std::string line;
    long totalPasswords = 168853376; // выборка данных с корректным введением паролей  || 742330412

    // Пропускаем первую строку (заголовок)
    std::getline(file, line);

    long sum_occur = 0;
    double prob = 0.0;
    double entropy = 0.0;
    double math_e = 0.0;
    int i = 1;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('|');
        std::string password = line.substr(0, delimiterPos);
        int occurrence = std::stoi(line.substr(delimiterPos + 1));
        sum_occur += occurrence;
        double probability = static_cast<double>(occurrence) / totalPasswords;
        prob += probability;

        passwords.push_back({password, occurrence, probability});
        
        if (probability > 0) {
            entropy -= probability * log2(probability);
        }

        math_e += i * probability; 
        ++i;
    }

    // Нормировка
    double norm = (1 - prob)/100000;
    

    // for (const auto& k : passwords)
    // std::cout << k.value << " | " << k.occurrence << " | " << k.probability << "\n";

    std::cout << "\nСуммарная вероятность: " << prob << "\nСуммарное количество паролей: " << sum_occur << "\n";
    
    std::cout << "Энтропия: " << entropy << " бит" << std::endl;

    std::cout << "Среднее количество переборов (мат. ожидание): " << math_e << " | i = " << i-1 <<  std::endl;

    double math_e_low_bound = (1.0 / 4.0) * pow(2, entropy) + 1;
    std::cout << "Нижняя граница с мат.ожидания: " << math_e_low_bound << std::endl;

    return 0;
}