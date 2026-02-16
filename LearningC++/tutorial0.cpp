#include <iostream>
#include <string>
#include <random>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distr(1, 100);

    int target = distr(gen);
    
    std::cout << "This is a number guessing game, guess a number from 1-100: " << "\n";
    int guess;
    std::cin >> guess;

    while (guess != target) {
        if(guess > target) { 
            std::cout << "That number was too high try again: " << "\n";
            std:: cin >> guess;
        }

        else { 
            std::cout << "That number was too low try again: " << "\n";
            std::cin >> guess;
        }
    }

    std::cout << "Congrats, you guessed correct the number was " << target << std::endl;


    return 0;
}