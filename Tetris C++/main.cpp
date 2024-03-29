#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point {
    int x,y;
} a[4], b[4];

int figures[7][4] = {
    2,4,5,7, // Z
    1,3,5,7, // I
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

bool check()
{
    for (int i = 0; i < 4; i++)
        if(a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
        else if (field[a[i].y][a[i].x]) return 0;
    
    return 1;
}

int main()
{
    //std::srand(time(0));
    sf::RenderWindow window(sf::VideoMode(320,480), "Tetris");
    
    sf::Texture texture, texture2, texture3;
    texture.loadFromFile(resourcePath() + "images/tiles.png");
    texture2.loadFromFile(resourcePath() + "images/background.png");
    texture3.loadFromFile(resourcePath() + "images/frame.png");

    sf::Sprite sprite(texture), background(texture2), frame(texture3);
    
    int dx = 0;
    bool rotate = false;
    int colorNum = 1;
    
    sf::Clock clock;
    float timer = 0;
    float delay = 0.3;
    
    sf::Event event;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed)
                if(event.key.code == sf::Keyboard::Up) rotate = true;
                else if (event.key.code == sf::Keyboard::Left) dx = -1;
                else if (event.key.code == sf::Keyboard::Right) dx = 1;
                else if (event.key.code == sf::Keyboard::Down) delay = 0.05;
        }
        
        // tick
        if(timer > delay) {
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].y += 1;
            }
            
            if (!check()) {
                for (int i = 0; i < 4; i++)
                    field[b[i].y][b[i].x] = colorNum;
                
                colorNum = 1 + std::rand() % 7;
                int n = std::rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }
            timer = 0;
        }
        
        // move
        for (int i = 0; i < 4; i++)
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        
        if(!check()) {
            for (int i = 0; i < 4; i++)
                a[i]=b[i];
        }
        // rotate
        if(rotate)
        {
            Point p = a[1]; // center of rotate
            
            for (int i = 0; i < 4; i++) {
                int x = a[i].y-p.y;
                int y = a[i].x-p.x;
                a[i].x = p.x-x;
                a[i].y = p.y+y;
            }
            
            if(!check()) {
                for (int i = 0; i < 4; i++)
                    a[i]=b[i];
            }
        }
        
        // check lines
        int k = M - 1;
        for (int i = M - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if(field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if(count < N) k--;
        }
        // figures
//        int n = 2;
//
//        if(a[0].x == 0) {
//            for (int i = 0; i < 4; i++) {
//                a[i].x = figures[n][i] % 2;
//                a[i].y = figures[n][i] / 2;
//            }
//        }
        // move and rotate end
        dx = 0;
        rotate = false;
        delay = 0.3;
        
        window.clear(sf::Color::White);
        window.draw(background);
        
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if(field[i][j] == 0) continue;
                sprite.setTextureRect(sf::IntRect(field[i][j]*18,0,18,18));
                sprite.setPosition(j*18, i*18);
                sprite.move(28,31);
                window.draw(sprite);
            }
        }
        
        // figures position
        for (int i = 0; i < 4; i++) {
            sprite.setTextureRect(sf::IntRect(colorNum*18,0,18,18));
            sprite.setPosition(a[i].x*18, a[i].y*18);
            sprite.move(28,31);
            window.draw(sprite);
        }
        window.draw(frame);
        window.display();
    }
    
    return 0;
}
