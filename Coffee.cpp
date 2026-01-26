#import <iostream>
bool coffee = true;
void brewCoffee() {
    std::cout << "Brewing a fresh cup of coffee!" << std::endl;
    coffee = true;
}
void drinkCoffee() {
    std::cout << "Drinking the coffee!" << std::endl;
    coffee = false;
}
 
int main() {
    if(coffee) {
        drinkCoffee();
    } else {
        brewCoffee();
    }
    return 0;
}