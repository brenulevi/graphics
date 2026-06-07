#include <iostream>

#include "application.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    try
    {
        Application app;
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}