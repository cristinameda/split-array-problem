#include <iostream>
#include <vector>
#include <unordered_set>
#include <numeric>

bool canBeSplit(std::vector<int> &a, std::vector<int> &b, std::vector<int> &c, int idx, int sum_b, int sum_c)
{
    size_t size_b = b.size();
    size_t size_c = c.size();
    if (idx == a.size())
    {
        return size_b > 0 && size_c > 0 && (double)sum_b / size_b == (double)sum_c / size_c;
    }

    b.push_back(a[idx]);
    if (canBeSplit(a, b, c, idx + 1, sum_b + a[idx], sum_c))
    {
        return true;
    }
    b.pop_back();

    c.push_back(a[idx]);
    if (canBeSplit(a, b, c, idx + 1, sum_b, sum_c + a[idx]))
    {
        return true;
    }
    c.pop_back();

    return false;
}

bool canArrayBeSplitSameAverage(std::vector<int> a)
{
    std::vector<int> b, c;
    // recursive approach
    return canBeSplit(a, b, c, 0, 0, 0);
}

bool isSplitPossible(int sum, int n, int half_n)
{
    bool is_possible = false;
    for (int i = 1; i <= half_n; i++)
    {
        if (sum * i % n == 0)
        {
            is_possible = true;
        }
    }
    return is_possible;
}

bool canArrayBeSplitEfficientlySameAverage(const std::vector<int> &a)
{
    int n = a.size();
    int half_n = n / 2;
    int sum = std::accumulate(a.begin(), a.end(), 0);

    // pruning
    if (!isSplitPossible(sum, n, half_n))
    {
        return false;
    }

    // dynamic programming
    std::vector<std::unordered_set<int>> subsetSums(half_n + 1);
    subsetSums[0].insert(0);

    for (int number : a)
    {
        for (int i = half_n; i >= 1; i--)
        {
            for (int s : subsetSums[i - 1])
            {
                subsetSums[i].insert(s + number);
            }
        }
    }

    for (int i = 1; i <= half_n; i++)
    {
        if (sum * i % n == 0 && subsetSums[i].count(sum * i / n))
        {
            return true;
        }
    }

    return false;
}

int main()
{
    std::vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << std::boolalpha << canArrayBeSplitSameAverage(a) << std::endl;
    std::cout << std::boolalpha << canArrayBeSplitEfficientlySameAverage(a) << std::endl;

    return 0;
}