//runs the game itself
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

constexpr int TILE_SIZE = 32;
constexpr int GRID_WIDTH = 20;
constexpr int GRID_HEIGHT = 20;

int main()
{
    // Create a new window (width: 800px, height: 600px, title: "My SFML Window")
    sf::RenderWindow window(sf::VideoMode(800, 600), "My SFML Window", sf::Style::Close | sf::Style::Resize);

    // The main application loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Check if the window close button was pressed
            if (event.type == sf::Event::Closed)
                window.close(); // Close the window
        }

        // --- Add your drawing code here (e.g., drawing shapes or sprites) ---

        // Display the contents of the window
        window.display();
    }

    return 0;
}
