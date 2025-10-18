#ifndef ALGHEADER_H
#define ALGHEADER_H

#include <vector>
#include <functional>

using SortStepCallback = std::function<void(const std::vector<int>&, int, int)>;

void bubble_sort(std::vector<int>& v, SortStepCallback on_step = {});
void quick_sort(std::vector<int>& v, SortStepCallback on_step = {});
void quick_sort_impl(std::vector<int>& arr, int low, int high, SortStepCallback on_step);
std::vector<int> smooth_sort(std::vector<int>& arr, SortStepCallback on_step);
void block_sort(std::vector<int>& arr, SortStepCallback on_step = {});
void merge_sort(std::vector<int>& arr, SortStepCallback on_step);
void insert_sort(std::vector<int>&arr, SortStepCallback on_step);
void heap_sort(std::vector<int>& arr, SortStepCallback on_step);
void gnome_sort(std::vector<int>& arr, SortStepCallback on_step);


#endif 