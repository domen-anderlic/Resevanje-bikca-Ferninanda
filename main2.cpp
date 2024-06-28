#include <iostream>
#include <deque>
#include <chrono>
#include <SDL.h>
#include <SDL_ttf.h>
#include "wtypes.h"
#include "global.hpp"
#include "Player.hpp"
#include "MapManager.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Arena.hpp"
#include "Bikec.hpp"
#include <fstream>
#include "Text.hpp"
#include <sstream>


void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}

void save_game(unsigned char current_level, Player Player, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Bullet>> bullets, std::vector<Arena> arenas, std::vector<Bikec> bikci);
void start_menu(bool& isRunning, SDL_Renderer*& renderer, MapManager& mapManager, TTF_Font* bigfont, unsigned char& current_level, std::vector<std::shared_ptr<Bullet>>& bullets, std::deque<std::pair<float,float>>& prev_player_pos, std::vector<Bikec>& bikci, std::string& kolosej, std::vector<std::shared_ptr<Enemy>>& enemies, Player& Player, std::vector<Arena>& arenas, TTF_Font* font, std::string& svet);
std::string get_player_name(bool& isRunning, SDL_Renderer*& renderer, TTF_Font* bigfont, MapManager& mapManager);
void win_menu(SDL_Renderer*& renderer, std::string name, TTF_Font* bigfont, int my_score, TTF_Font* font, MapManager& mapManager);

int time_left = 300;

int main(int, char**)
{

    srand(time(NULL));
    int horizontal = 0;
    int vertical = 0;
    GetDesktopResolution(horizontal, vertical);
   
    //const int FPS = 60;
    //const int frameDelay = 1000 / FPS;

    //Uint32 frameStart;
    //int frameTime;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool isRunning, fullscreen = false;

    

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) //SDL_INIT_VIDEO
    {
        std::cout << "Subsystems Initialised!..." << std::endl;
        window = SDL_CreateWindow("Resevanje Bikca Ferninanda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (window)
        {
            std::cout << "Window created!" << std::endl;
        }
        
       renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_SetRenderDrawColor(renderer, 0, 219, 255, 255);
        if (renderer)
        {
            std::cout << "Renderer created!" << std::endl;
        }
        if (0 == TTF_Init())
        {
            std::cout << "TTF Initialised!...\n";
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    SDL_ShowCursor(1);



    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 18);
    TTF_Font* bigfont = TTF_OpenFont("assets/arial.ttf", 25);
    if (!font || !bigfont)
    {
        std::cout << "Failed to load Font assets/arial.ttf!\n";
        isRunning = false;
    }
    SDL_Color Black = { 0, 0, 0, 255 };
    SDL_Color Gray = { 128,128,128,255 };
    SDL_Color Red = { 255,0,0,255 };

    std::ofstream replay("assets/replay.txt");
    if (!replay.is_open())
    {
        std::cout << "Cannot open assets/replay.txt!\n";
    }


    unsigned char current_level = 0;
    Player Player(renderer);
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Bullet>> bullets;
    unsigned short int b_cooldown = BULLET_COOLDOWN;
    bool can_PLAYER_shoot = 1;
    bool did_PLAYER_shoot = 0;
    bool bypass = 0;

    std::vector<Arena> arenas;
    std::deque<std::pair<float, float>> prev_player_pos;
    std::vector<Bikec> bikci;
    std::string svet = "Svet " + std::to_string(current_level+1) + " od 2", kolosej = "Unicene arene: " + std::to_string(bikci.size()) + " od 2", score = "Cas: " + std::to_string(time_left);
   
    SDL_Event event;
    MapManager mapManager(renderer);

    //vpis imena
    std::string name = get_player_name(isRunning, renderer, bigfont, mapManager);
    //start menu
    start_menu(isRunning, renderer, mapManager, bigfont, current_level, bullets, prev_player_pos, bikci, kolosej, enemies, Player, arenas, font, svet);


    auto start = std::chrono::high_resolution_clock::now();
    for(int frames = 0; isRunning; ++frames)
    {
        Uint64 startFrames = SDL_GetPerformanceCounter();
        //frameStart = SDL_GetTicks();

        SDL_PollEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
            {
                isRunning = false;
                break;
            }
            case SDL_KEYDOWN:
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_R)
                {                    
                    bullets.clear();                    
                    prev_player_pos.clear();
                    bikci.clear();
                    kolosej = "Unicene arene: " + std::to_string(bikci.size()) + " od 2";
                    mapManager.update_map_sketch(current_level, enemies, Player, arenas);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_F)
                {
                    //T = horizontal / (25 * 32);
                    fullscreen = !fullscreen;
                    SDL_SetWindowFullscreen(window, (fullscreen)?SDL_WINDOW_FULLSCREEN_DESKTOP:0);
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
                {
                    isRunning = false;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    save_game(current_level, Player, enemies, bullets, arenas, bikci);
                    start_menu(isRunning, renderer, mapManager, bigfont,current_level, bullets, prev_player_pos, bikci,kolosej,enemies, Player,arenas,font, svet);
                }
                break;
            }
            default:
            { 
                break;
            }
        }
        
        did_PLAYER_shoot = Player.update(event, mapManager, bullets, can_PLAYER_shoot);
        replay << Player.get_x() <<' ' << Player.get_y() << '\n';
        for (int i = 0; i<bullets.size();++i)
        {
            bullets[i]->update(enemies, mapManager, Player.get_x(), arenas);
        }
        for (unsigned short int i = 0; i < bullets.size(); ++i)
        {
            if (1 == bullets[i]->get_dead())
            {
                bullets.erase(bullets.begin() + i);
            }
        }

        for (int i = 0; i<enemies.size(); ++i)
        {
            enemies[i]->update(enemies, mapManager, Player, Player.get_x());
        }
        for (unsigned short int i = 0; i < enemies.size(); ++i)
        {
            if (1 == enemies[i]->get_dead(1))
            {
                enemies.erase(enemies.begin() + i);
                --i;
            }
        }
        for (int i = 0; i < arenas.size(); ++i)
        {
            if (1 == arenas[i].get_dead())
            {
                bikci.push_back(Bikec());
                kolosej = "Unicene arene: " + std::to_string(bikci.size()) + " od 2";
                arenas.erase(arenas.begin() + i);
            }            
        }

        
        

        prev_player_pos.push_back(std::pair<float,float>(Player.get_x(),Player.get_y()));
        if (prev_player_pos.size() >= 20) {
            prev_player_pos.pop_front();
        }

        SDL_RenderClear(renderer);
        mapManager.draw_background();
        mapManager.draw_map(Player.get_x());//da offsetamo vse cell-e

        for (int i = 0; i<arenas.size(); ++i)
        {
            arenas[i].draw(renderer, Player);
        }
        for (int i = 0; i<bikci.size(); ++i)
        {
            bikci[i].draw(renderer, prev_player_pos.at(0));
        }
        for (int i = 0; i<bullets.size(); ++i)
        {
            bullets[i]->draw(renderer, Player.get_x());
        }
        for (int i = 0; i < enemies.size(); ++i)
        {
            enemies[i]->draw(renderer, Player.get_x());
        }

        if (time_left < 0)
        {
            time_left = 0;
            Player.die(1);
        }
        score = "Cas: " + std::to_string(time_left);        

        
        
        Text svetText(renderer, svet, font, Black);
        Text kolosejText(renderer, kolosej, font, Black);
        Text scoreText(renderer, score, font, Black);


        svetText.draw(renderer, 0, 0);
        kolosejText.draw(renderer, SCREEN_WIDTH / 2 - kolosejText.get_w() / 2, 0);
        scoreText.draw(renderer, SCREEN_WIDTH - scoreText.get_w(), 0);
        Player.draw();

        if (Player.get_dead())
        {
            Text deathText(renderer, "DAMN! (R za ponovni poizkus, ESC za Menu, ` za Izhod", bigfont, Red);
            deathText.draw(renderer, SCREEN_WIDTH / 2 - deathText.get_w() / 2, SCREEN_HEIGHT / 2 - deathText.get_h() / 2);
        }

        SDL_RenderPresent(renderer);
        
        if (1 == did_PLAYER_shoot || bypass)
        {
            bypass = 1;
            can_PLAYER_shoot = 0;
            b_cooldown--;
            if (0 == b_cooldown)
            {
                can_PLAYER_shoot = 1;
                b_cooldown = BULLET_COOLDOWN;
                bypass = 0;
            }
        }


        if (bikci.size() >= 2)
        {
            ++current_level;
            svet = "Svet " + std::to_string(current_level+1) + " od 2";
            Player.reset();
            enemies.clear();
            bullets.clear();
            arenas.clear();
            prev_player_pos.clear();
            bikci.clear();
            kolosej = "Unicene arene: " + std::to_string(bikci.size()) + " od 2";
            if (current_level > 1)
            {
                win_menu(renderer, name, bigfont, time_left, font, mapManager);
            }
            mapManager.update_map_sketch(current_level, enemies, Player, arenas);
        }
        if (frames == 59)
        {
            time_left--;
            frames = 0;
        }

        //frameTime = SDL_GetTicks() - frameStart;
        //if (frameDelay > frameTime)
        //{
        //    SDL_Delay(frameDelay - frameTime);
        //}
        Uint64 endFrames = SDL_GetPerformanceCounter();
        float elapsedMS = (endFrames - startFrames) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        // Cap to 60 FPS
        SDL_Delay(floor(16.666f - elapsedMS));
    }
    
    save_game(current_level, Player, enemies, bullets, arenas, bikci);
    replay.close();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();
    std::cout << "Game quit" << std::endl;
    return 0;
}

void save_game(unsigned char current_level, Player Player, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Bullet>> bullets, std::vector<Arena> arenas, std::vector<Bikec> bikci)
{
    std::ofstream savepos("assets/positions.txt");
    if (!savepos.is_open())
    {
        std::cout << "could not save postition!\n";
    }
    else
    {
        savepos << "c " << (int)current_level << '\n';
        savepos << "p " << Player.get_x() << ' ' << Player.get_y() << '\n';
        for (int i = 0; i < enemies.size(); ++i)
        {
            savepos << "r " << enemies[i]->type << ' ' << enemies[i]->get_hit_box().x << ' ' << enemies[i]->get_hit_box().y << '\n';
        }
        for (int i = 0; i < bullets.size(); ++i)
        {
            savepos << "b " << bullets[i]->get_hit_box().x << ' ' << bullets[i]->get_hit_box().y << ' ' << bullets[i]->get_direction() << '\n';
        }
        for (int i = 0; i < arenas.size(); ++i)
        {
            savepos << "a " << arenas[i].get_hit_box().x << ' ' << arenas[i].get_hit_box().y << '\n';
        }
        savepos << "x " << bikci.size() << '\n';
        savepos << time_left << '\n';
        savepos.close();
    }
}

void start_menu(bool &isRunning,SDL_Renderer*& renderer, MapManager&mapManager, TTF_Font* bigfont, unsigned char& current_level, std::vector<std::shared_ptr<Bullet>>& bullets, std::deque<std::pair<float,float>>& prev_player_pos, std::vector<Bikec>& bikci, std::string& kolosej, std::vector<std::shared_ptr<Enemy>> & enemies, Player& Player, std::vector<Arena>& arenas, TTF_Font* font, std::string& svet )
{
    SDL_Color Black = { 0, 0, 0, 255 };
    SDL_Color Gray = { 128,128,128,255 };
    SDL_Color Red = { 255,0,0,255 };

   int button = 1;
    std::ifstream readpos("assets/positions.txt");
    bool can_continue = readpos.is_open();
    if (readpos.is_open())
    {
        readpos.close();
    }
    else
    {
        std::cout << "could not open asstes/positions.txt\n";
    }
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP)
                {
                    if (--button < 1)
                    {
                        button = 1;
                    }
                    else if (button == 2 && !can_continue)
                    {
                        button = 1;
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                {
                    if (++button > 3)
                    {
                        button = 3;
                    }
                    else if (button == 2 && !can_continue)
                    {
                        ++button;
                    }
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                {
                    //zacni z tistim sranjem
                    if (button != 2 || can_continue) {
                        done = true;
                    }

                }
                if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
                {
                    done = 1;
                    isRunning = false;
                }
                break;
            }
            }
        }
        SDL_RenderClear(renderer);
        mapManager.draw_background();
        Text Button1(renderer, "NOVA IGRA", bigfont, (button == 1) ? Red : Black);
        Text Button2(renderer, "NADALJUJ", bigfont, (can_continue) ? ((button == 2) ? Red : Black) : Gray);
        Text Button3(renderer, "NAVODILA", bigfont, (button == 3) ? Red : Black);
        Button1.draw(renderer, SCREEN_WIDTH / 2 - Button1.get_w() / 2, SCREEN_HEIGHT / 4);
        Button2.draw(renderer, SCREEN_WIDTH / 2 - Button2.get_w() / 2, SCREEN_HEIGHT / 2);
        Button3.draw(renderer, SCREEN_WIDTH / 2 - Button3.get_w() / 2, 3 * SCREEN_HEIGHT / 4);
        SDL_RenderPresent(renderer);
    }
    
    switch (button)
    {
    case 1://new game
    {
        time_left = 300;
        current_level = 0;
        bullets.clear();
        prev_player_pos.clear();
        bikci.clear();
        kolosej = "Unicene arene: " + std::to_string(bikci.size()) + " od 2";
        svet = "Svet " + std::to_string(current_level + 1) + " od 2";
        mapManager.update_map_sketch(current_level, enemies, Player, arenas);
        break;
    }
    case 2://continue
    {
        mapManager.continue_map_sketch(current_level, enemies, Player, arenas, bullets, bikci, time_left);
        kolosej = "Unicene arene: " + std::to_string(bikci.size()) + " od 2";
        svet = "Svet " + std::to_string(current_level + 1) + " od 2";
        break;
    }
    case 3:
    {
        std::ifstream t("assets/instructions.txt");
        if (!t.is_open())
        {
            std::cout << "Could not open instructions.txt!\n";
        }
        std::stringstream buffer;
        buffer << t.rdbuf();
        Text Navodila(renderer, buffer.str(), font, Black, 1);
        SDL_RenderClear(renderer);
        mapManager.draw_background();
        Navodila.draw(renderer, SCREEN_WIDTH * 0.15, SCREEN_HEIGHT * 0.15);
        SDL_RenderPresent(renderer);
        t.close();
        done = false;
        while (!done)
        {
            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                {

                    if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                    {
                        done = 1;
                        start_menu(isRunning, renderer, mapManager, bigfont, current_level, bullets, prev_player_pos, bikci, kolosej, enemies, Player, arenas, font, svet);
                        return;
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
                    {
                        done = 1;
                        isRunning = false;
                    }
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    {
                        start_menu(isRunning, renderer, mapManager, bigfont, current_level, bullets, prev_player_pos, bikci, kolosej, enemies, Player, arenas, font, svet);
                        return;
                    }
                    break;
                }
                }
            }
        }
        //start_menu(isRunning, renderer, mapManager, bigfont, current_level, bullets, prev_player_pos, bikci, kolosej, enemies, Player, arenas, font, svet);
    }
    }
}

std::string get_player_name(bool &isRunning,SDL_Renderer*& renderer, TTF_Font* bigfont, MapManager& mapManager)
{
    SDL_Color Black = { 0, 0, 0, 255 };
    std::string name;
    bool done = false;
    SDL_StartTextInput();
    while (!done) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                    done = 1;
                if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE) {
                    done = 1;
                    isRunning = false;
                }
                if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && name.size()) {
                    name.pop_back();
                }
                break;
            case SDL_TEXTINPUT:
                
                name += event.text.text;
                name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
                
                break;
            }
        }
        Text NameText(renderer, name, bigfont, Black);
        SDL_RenderClear(renderer);
        mapManager.draw_background();
        Text StartText(renderer, "IME IGRALCA:", bigfont, Black);
        StartText.draw(renderer, SCREEN_WIDTH / 2 - StartText.get_w() / 2, SCREEN_HEIGHT / 2 - StartText.get_h());
        NameText.draw(renderer, SCREEN_WIDTH / 2 - NameText.get_w() / 2, SCREEN_HEIGHT / 2);
        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput();
    return name;
}

void win_menu(SDL_Renderer*& renderer, std::string name, TTF_Font* bigfont, int my_score, TTF_Font* font, MapManager& mapManager)
{
    SDL_Color Black{ 0,0,0,255 };
    Text WinText(renderer, "BRAVO " + name + "!", bigfont, Black);
    Text CloseText(renderer, "Za izhod pritisni `(Grave)", font, Black);
    std::ifstream t("assets/best.txt");
    if (!t.is_open())
    {
        std::cout << "Could not open assets/best.txt!\n";
    }
    
    std::ofstream u("assets/new.txt");
    bool found = false;
    for (int i = 0; i < 5; ++i)
    {
        char* a = new char;
        int b;
        t >> a;
        t >> b;
        if (b < my_score && !found)
        {
            u << name <<' ' <<my_score<<'\n';
            found = true;
            continue;
        }
        u << a << ' ' << b << '\n';
    }
    t.close();
    u.close();
    std::remove("assets/best.txt");
    std::rename("assets/new.txt", "assets/best.txt");
    t.open("assets/best.txt");

    std::stringstream buffer;
    buffer << t.rdbuf();
    Text Best(renderer, buffer.str(), bigfont, Black, 1);
    SDL_RenderClear(renderer);
    mapManager.draw_background();
    WinText.draw(renderer, SCREEN_WIDTH/2 - WinText.get_w()/2, 0);
    Best.draw(renderer, SCREEN_WIDTH/ - Best.get_w()/2, SCREEN_HEIGHT/2 - Best.get_h()/2);
    CloseText.draw(renderer, 0, SCREEN_HEIGHT - CloseText.get_h());
    SDL_RenderPresent(renderer);
    t.close();
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
                {
                    std::remove("assets/positions.txt");
                    exit(0);
                }
                break;
            }
            }
        }
    }
}