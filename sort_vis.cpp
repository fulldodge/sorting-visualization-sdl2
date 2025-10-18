#include <iostream>
#include <algorithm>
#include <random>
#include <ranges>
#include <functional> 
#include "myalglist.h"
#include <SDL2/SDL.h>
#include <chrono>

static bool completed = false;

void draw_state(
    const std::vector<int>& v,
    SDL_Renderer* renderer,
    int red = -1, int blue = -1)
{
    int index=0;
    for(int i : v)
    {
        if(index==red)
        {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }   
        else if (index==blue)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer,index,999,index,i);
        index+=1;
    }
}

void run_sort(
    std::vector<int>& data,
    std::function<void(std::vector<int>&, SortStepCallback)> sort_fn,
    const std::string& title = "Sorting Visualization"
) {
    completed=false;
    int step_count=0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (SDL_CreateWindowAndRenderer(1900, 1000, 0, &window, &renderer) != 0) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    SDL_SetWindowTitle(window, title.c_str());
    SDL_RenderSetScale(renderer, 1.5f, 1.0f);

    bool running = true;
    SortStepCallback visual_callback = [&](const std::vector<int>& arr, int i, int j) {
        //step count
        step_count++;
        // Window exit
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        if (!running) return;

        // Clear window and start sort drawing
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_state(arr, renderer, i, j);
        SDL_RenderPresent(renderer);
        SDL_Delay(1);


    };
    auto start = std::chrono::high_resolution_clock::now();
    //Sort start
    sort_fn(data, visual_callback);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    long long microseconds = duration.count();
    //Final state
    if (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_state(data, renderer, -1, -1);
        SDL_RenderPresent(renderer);
        //Show result in ms. ~3s
        SDL_Delay(3000);
        completed = true;
    }
    std::cout << "Visualization time spended in miliseconds: " << duration.count()/1000 << "\n"; 
    //Clear Window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main()
{

std::random_device rand_Num;
std::uniform_int_distribution num_range(1,999);
std::vector<int> v;

//fill vector with random numbers between 1 and 999
for (int i=0;i<1000;i++)
{
    v.push_back(num_range(rand_Num));
}
int choice;
while (true) 
{
    std::cout << "\n-------------------\nPlease choose your sort (0 for exit):\n-------------------\n"
        << "1.Heap sort\n2.Merge sort\n3.Smooth sort\n"
        << "4.Block sort\n5.Quick sort\n6.Bubble sort\n" << "7.Insertion sort\n8.Gnome Sort\n"
        << "-------------------\n Your number is: ";
    std::cin >> choice;
    std::cout << "-------------------\n";
    if (choice == 0) break;
    std::vector<int> v_copy = v;
switch(choice)
    {
    case 1:
        run_sort(v_copy,heap_sort,"Heap sort");
        break;
    case 2:
        run_sort(v_copy,merge_sort,"Merge_sort");
        break;
    case 3:
        run_sort(v_copy,smooth_sort,"Smooth sort");
        break; 
    case 4:
        run_sort(v_copy,block_sort, "Block sort");
        break; 
    case 5:
        run_sort(v_copy,quick_sort,"Quick sort");
        break; 
    case 6:
        run_sort(v_copy,bubble_sort,"Bubble sort");
        break;
    case 7:
        run_sort(v_copy,insert_sort,"Insert sort");
        break;
    case 8:
        run_sort(v_copy,gnome_sort,"Gnome sort");
        break;
    case 0:
        return 0;
    default:
        std::cout << "Invalid choice.\n";
        continue;
    }

if(completed)
{
    if (std::ranges::is_sorted(v_copy)) 
        {
            std::cout << "-------------------\nSorted array:\n\n";
            for (int i : v_copy) 
            {
            std::cout << i << " ";
            }
            std::cout << "\n";
        }
}
}
}