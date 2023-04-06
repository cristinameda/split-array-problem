# Split array problem

## Description

In a given integer array A, we must move every element of A to either list B or list C (B and C initially start empty).
Return true if and only if after such a move, it is possible that the average value of B is equal to the average value
of C, and B and C are both non-empty.

### Sample input

    [1,2,3,4,5,6,7,8]

### Sample output

    True

### Explanation

We can split the array into [1,4,5,8] and [2,3,6,7] so that both have an average of 4.5.

## Solution

### First solution - recursive approach

Given that the maximum length of A is relatively small, we could use a recursive approach as it is a cleaner and simpler
way to solve this problem.

#### Algorithm

The algorithm consists of three cases. First, the best scenario is reaching the end of list A, which could indicate a
correct distribution. In this case we check that both B and C are not empty and that their mean values are the same. If
the conditions are met, then A can be split into two lists with the same average value. The other two cases try to
populate B and C with elements from A in any possible way (for each index) so that when the stopping condition is
reached, the formed distribution is valid. So, for a current index, we could add the corresponding element of A to list
B or to list C. Therefore, we check for each option whether the recursive call for the next index, with the updated list
and sum, will return true. If one of them does, it means that two formations have been found and the list A can be split
according to the imposed rule. Otherwise, it cannot (return false).

#### Implementation

The function implementing the algorithm, canBeSplit, takes the following parameters:

- a: the input array (vector) of integers (A)
- b: an array (vector) of integers, initially empty (B)
- c: an array (vector) of integers, initially empty (C)
- idx: the current index in a (integer)
- sum_b: the sum of elements in b (integer)
- sum_c: the sum of elements in c (integer)

#### Efficiency / Algorithm complexity

The time complexity of this algorithm is exponential as it has to consider all the possible combinations of elements in
the input array A, which could be up to 2^30, for the worst-case scenario.

    Time complexity: O(2^n), where n is the length of input array A.

The space complexity is also exponential, since it uses recursion (2 recursive calls), which again could lead to a value
of 2^30 in the worst-case scenario.

    Space complexity: O(2^n), where n is the length of input array A.

#### Conclusion

I chose to include this solution, although it is not that effective, because it was the first that came to my mind and
it is a simplistic and more natural approach.

### Second solution - using pruning and dynamic programming

The best solution I've come up with is first checking an initial condition to eliminate arrays that can't be split (
pruning). Then, as we are only interested in the averages of subsets, we only need to know their sum of elements and not
on their actual generation. The sums needed are for partitions of 0 to n/2 elements (n is the length of input array A),
because we know that one of the two subdivisions is going to be smaller or both will be the same size. So, the sums for
a subset will be dynamically calculated using the sums of the subsets shorter by one element.

#### Algorithm

##### Pruning

Splitting is only possible

    if sum * i % n = 0 for at least one i from 1 to n/2

where sum is the sum of the input array A, i is the subset's length and n is the size of A.

This condition is a consequence of a mathematical equality, resulted in the following way:

    Let A be the input array and B and C the disjoint subsets of A so that avg(B) = avg(C).
    Let NA be the size of A (number of elements), NB size of B and NC size of C.
    Let SA, SB and SC be the sums of elements of each array.
    
    So far we know that:
        (1) SB + SC = SA
        (2) NB + NC = NA
        (3) SB/NB = SC/NC (equal average values)

    Starting from the equality expression (3) and using the information from expression (1), we could write that
        SB/NB = (SA - SB)/NC <=> NC * SB = NB * (SA - SB) <=> NC * SB = NB * SA - NB * SB <=>
        NC * SB + NB * SB = NB * SA <=> SB * (NB + NC) = NB * SA <=(2)=> SB * NA = NB * SA <=>
        
        (4) SB = NB * SA/NA

The resulted formula indicates that the sum of elements in B is equal to size of B multiplied by sum of elements in A
divided by the size of A. Given the fact that the sum is an integer, this division should also result in an integer.

Therefore, for each possible subset length (from 1 to n/2), we check if the division results in an integer. If at least
one of the subsets verifies the condition, the input array can be split, otherwise not.

##### Calculating the sums dynamically

In order to gradually calculate sums, we need a data structure to retain all the possible sums of each subset of size i,
where i goes from 0 to n/2 and fill it with new values at each step. A suitable structure would be a vector of unordered
sets of integers (eliminates duplicated sums on each level). Each row i of this 2D array stores all the sums of all
possible subsets of size i.

The core of the algorithm consist in how the 2D array is populated. For each element e in A, we will start from the last
row (starting from the bottom will affect iteration) and we will iterate decreasingly until the first. Finally, for each
row (which indicates the size of the subset), we get each value from the previous row and add e to it, inserting the new
sum into the current row. This process will end when the iteration over A stops.

    Let subsetSums be the 2D array where we store the subsets sums.

    for each element e from A
        for each row i from top (n/2) to bottom (1)
            for each sum s from subsetSums[i - 1]
                result = s + number
                insert result in subsetSums[i]

##### Exemplification

    A = {1,2,3,4,5}

    We take 1 and we go from top to buttom to add it to existing sums. For now, we only have 0, so we'll add 1 to 0 and
    the result will be inserted in the row 1 as it represents the sum of subset {1}.

    row 0: 0
    row 1: 1
    row 2:

    We take 2. We start at the row 2 and take all the values from row 1 and add 2 to them to get the new sums for
    subsets of 2 elements. In this case, we only add 2 to 1 in order to get the sum of subset {1,2}, which will be 3.
    We add it to the current row. Then, we move to the next row (number 1) and get all the values from the row below it
    (number 0) and add to them 2 so we can find the subsets of one element, which is only one, {2}. The result will be
    inserted to the current row (0+2 = 2).

    row 0: 0
    row 1: 1 2
    row 2: 3

    We take 3. Starting from row 2, we add 3 to each element of row 1 and the result is added to the current row. The
    same process is repeated for row 1.

    row 0: 0
    row 1: 1 2 3
    row 2: 3 4 5

    We take 4.

    row 0: 0
    row 1: 1 2 3 4
    row 2: 3 4 5 6 7

    We take 5, the last element of A. The final 2D array will be:

    row 0: 0
    row 1: 1 2 3 4 5
    row 2: 3 4 5 6 7 8 9

    Note that sums are not duplicated!

Using the calculated sums, we can now determine if the input array can be split. How? The key to this answer is
expression (2) which helped us at pruning. It says that sum of elements of B should be equal to the size of B multiplied
by the sum of A, all divided by size of A (SB = NB * SA/NA). So, we check in the 2D data structure if at any level there
is a sum (SB) that is equal to NB * SA/NA. If there is one, the array can be split in two partitions with the same mean
value.

#### Implementation

The function implementing the algorithm, canArrayBeSplitEfficientlySameAverage, takes only the parameter a, the input
array of integers (type vector of integers).

The pruning condition is defined by the function isSplitPossible, which only takes the necessary parameters to verify
it:
the sum of elements from A (calculated with the accumulate function), the size of A (n) and the half size as it is often
used and it is also stored in a variable.

#### Efficiency / Algorithm complexity

    The time complexity for the isSplitPossible function is O(n/2), where n is the length of the input array.

The time complexity of the canArrayBeSplitEfficientlySameAverage function is composed of:

- pruning function iterates from 1 to n/2 => O(n/2)
- nested loops for populating the 2D array iterates n * n/2 * n/2 times => O(n^3/4)
- final loop iterates from 1 to n/2 and calls the count function every time in worst-case scenario; count could also
  have the time complexity up to O(n/2) => O(n^2/4)


    The time complexity for the canArrayBeSplitEfficientlySameAverage function is polynomyal
        O(n/2+n^2/4+n^3/4), where n is the length of the input array.

#### Conclusion

This algorithm is more efficient and also uses an optimization technique to prune the search space and improve the
worst-case performance. Even though is harder to think of this solution, its implementation is quite simple.

