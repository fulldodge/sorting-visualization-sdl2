#include "myalglist.h" 


//quick sort partition
//
int partition(std::vector<int>& arr, int low, int high, SortStepCallback on_step = {}) 
    {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
            if (on_step) on_step(arr, i, j); // visualization on every swap
        }
    }
    std::swap(arr[i + 1], arr[high]);
    if (on_step) on_step(arr, i + 1, high); // final swap
    return (i + 1);
}

// Build the Leonardo heap by merging
// pairs of adjacent trees
void heapify_smooth(std::vector<int>& arr, int start, int end, SortStepCallback on_step = {})
{
    int i = start;
    int j = 0;
    int k = 0;

    while (k < end - start + 1) {
        if (k & 0xAAAAAAAA) {
            j = j + i;
            i = i >> 1;
        } else {
            i = i + j;
            j = j >> 1;
        }
        k++;
    }

    while (i > 0) {
        j = j >> 1;
        k = i + j;
        while (k < end) {
            if (arr[k] > arr[k - i]) {
                break;
            }
            std::swap(arr[k], arr[k - i]);
            if (on_step) on_step(arr, k, k - i); // visualize swap
            k += i;
        }
        i = j;
    }
}

//Smooth Sort function
std::vector<int> smooth_sort(std::vector<int>& arr, SortStepCallback on_step = {})
{
    int n = arr.size();
    if (n <= 1) return arr;

   
    std::vector<int> L;
    L.push_back(1); // L[0]
    L.push_back(1); // L[1]
    while (L.back() < n) {
        L.push_back(L[L.size()-1] + L[L.size()-2] + 1);
    }

    static int step_counter = 0;
    const int SKIP = 50;
    //visualize every 50 steps to desrease total time
    auto maybe_on_step = [&](const std::vector<int>& a, int i, int j) {
        if (on_step) {
            if (++step_counter % SKIP == 0) {
                on_step(a, i, j);
            }
        }
    };

    int p = n - 1;
    int q = p;
    int r = 0;

    //Build the Leonardo heap by merging
    //pairs of adjacent trees
    while (p > 0) {
        if ((r & 0x03) == 0) {
            heapify_smooth(arr, r, q, maybe_on_step); 
        }

        if (r < static_cast<int>(L.size()) && L[r] == p + 1) {
            r = r + 1;
        }
        else {
            r = r - 1;
            if (r < 0) r = 0;
            int L_r = (r < static_cast<int>(L.size())) ? L[r] : (p + 1);
            q = p - L_r;
            heapify_smooth(arr, r, q, maybe_on_step); 
            q = r - 1;
            r = r + 1;
            std::swap(arr[0], arr[p]);
            maybe_on_step(arr, 0, p); 
            p = p - 1;
            q = p;
        }
    }

    //Convert the Leonardo heap
    //back into an array
    for (int i = 0; i < n - 1; i++) {
        int j = i + 1;
        while (j > 0 && arr[j] < arr[j - 1]) {
            std::swap(arr[j], arr[j - 1]);
            maybe_on_step(arr, j, j - 1);
            j = j - 1;
        }
    }

    return arr;
}

void heapify_heap(std::vector<int>& arr, int n, int i, SortStepCallback on_step) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        if (on_step) on_step(arr, i, largest);
        heapify_heap(arr, n, largest, on_step); // recursive down
    }
}

void heap_sort_impl(std::vector<int>& arr, int n, SortStepCallback on_step) {
    // Build max-heap

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify_heap(arr, n, i, on_step); 

    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        on_step(arr,0,i);
        heapify_heap(arr, i, 0, on_step);
        
    }
}

void heap_sort(std::vector<int>& arr, SortStepCallback on_step) {
    if (!arr.empty()) {
        heap_sort_impl(arr, static_cast<int>(arr.size()), on_step); 
    }
}
// Merges two sorted subarrays into a single sorted array
// reference wikipedia
void merge(std::vector<int>& arr, int left, int mid, int right, SortStepCallback on_step) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays to hold the two halves
    std::vector<int> L(n1);
    std::vector<int> R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        if (on_step) on_step(arr, k, -1);
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        if (on_step) on_step(arr, k, -1);
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        if (on_step) on_step(arr, k, -1);
        j++;
        k++;
    }
}
// Main Merge Sort function that recursively divides and merges
//
void merge_sort_impl(std::vector<int>& arr, int left, int right, SortStepCallback on_step) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_impl(arr, left, mid, on_step);
        merge_sort_impl(arr, mid + 1, right, on_step);
        merge(arr, left, mid, right, on_step);
    }
}
//Public function for merge sort.
void merge_sort(std::vector<int>& arr, SortStepCallback on_step) {
    if (!arr.empty()) {
        merge_sort_impl(arr, 0, static_cast<int>(arr.size()) - 1, on_step);
    }
}

//Block sort
//reference geeksforgeeks
std::vector<int> block_sort_impl(std::vector<int> arr, int blockSize, SortStepCallback on_step)
{
    std::vector<std::vector<int>> blocks;

    // Divide the input array into blocks of size blockSize
    for (int i = 0; i < arr.size(); i += blockSize) {
        std::vector<int> block;

        for (int j = i; j < i + blockSize && j < arr.size(); j++) {
            block.push_back(arr[j]);
        }

        // Sort each block
        std::sort(block.begin(), block.end());

        // Update arr and visualize every swap
        for (int j = 0; j < block.size(); ++j) {
            arr[i + j] = block[j];
            on_step(arr, i + j, i); 
        }

        blocks.push_back(block);
    }

    // Merge the sorted blocks into a single sorted list
    std::vector<int> result;
    while (!blocks.empty()) {
        int minIdx = 0;
        for (int i = 1; i < blocks.size(); i++) {
            if (blocks[i][0] < blocks[minIdx][0]) {
                minIdx = i;
            }
        }

        result.push_back(blocks[minIdx][0]);
        blocks[minIdx].erase(blocks[minIdx].begin());

        // Update array to show current state of sorting
        if (result.size() <= arr.size()) {
            arr[result.size() - 1] = result.back();
            on_step(arr, result.size() - 1, -1); 
        }

        if (blocks[minIdx].empty()) {
            blocks.erase(blocks.begin() + minIdx);
        }
    }

    return result;
}

//Public function for block sort.
void block_sort(std::vector<int>& arr, SortStepCallback on_step)
{
    arr = block_sort_impl(arr, 100, on_step); // 100 numbers block size
}

//Public function for quick sort.
void quick_sort(std::vector<int>& arr, SortStepCallback on_step) {
    if (!arr.empty()) {
        quick_sort_impl(arr, 0, arr.size() - 1, on_step);
    }
};

//quick sort implementation
void quick_sort_impl(std::vector<int>& arr, int low, int high, SortStepCallback on_step) {
    if (low < high) {
        int pi = partition(arr, low, high);

        if (on_step) on_step(arr, low, high);

        quick_sort_impl(arr, low, pi - 1, on_step);
        quick_sort_impl(arr, pi + 1, high, on_step);
    }
}

void bubble_sort(std::vector<int>& arr, SortStepCallback on_step) {

    static int step_counter = 0;
    const int SKIP = 50;
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                if (on_step) {
                    if (++step_counter % SKIP == 0) {
                        on_step(arr, j, j + 1);
                    }
                }
            }
        }
    }
}

void insert_sort(std::vector<int>&arr, SortStepCallback on_step)
{
    for(int i=1;i<arr.size();i++)
    {
        int point = arr[i];
        int j=i-1;
        while(j >= 0 && arr[j] > point)
        {
            arr[j+1]=arr[j];
            j=j-1;
        }
        int final_pos = j + 1;
        arr[final_pos] = point;
        on_step(arr, final_pos, i);
    }
};

void gnome_sort(std::vector<int>& arr, SortStepCallback on_step) {
    int i = 0;
    static int step_counter = 0;
    const int SKIP = 50;
    while (i < static_cast<int>(arr.size())) 
    {
        if (i == 0 || arr[i - 1] <= arr[i]) 
        {
            i++;
        } else 
        {
            std::swap(arr[i], arr[i - 1]);
            i--; 
            if (on_step) {
            if (++step_counter % SKIP == 0) 
            {
            on_step(arr, i+1,i);
            }
        }
        }
}
}