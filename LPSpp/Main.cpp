#include <iostream>
#include "Vm.h"

int main(int argc, char *argv[]) {
    if (argc < 2)
        std::cout << "Please provide a file!" << "\n";
    else {
        Vm vm;
        vm.execute_program(argv[1]);
    }
}
