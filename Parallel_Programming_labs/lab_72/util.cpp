#include "util.h"

#include <vector>
#include <numeric>
#include <iostream>

void combinations_recursive(const std::vector<unsigned long> &elems, unsigned long req_len,
                            std::vector<std::vector<unsigned long>> &result,
                            std::vector<unsigned long> &pos, unsigned long depth,
                            unsigned long margin)
{
    if (depth >= req_len) {
        result.push_back(std::vector<unsigned long>(3));
        for (unsigned long ii = 0; ii < pos.size(); ++ii)
            result[result.size() - 1][ii] = elems[pos[ii]];
        return;
    }

    if ((elems.size() - margin) < (req_len - depth))
        return;

    for (unsigned long ii = margin; ii < elems.size(); ++ii) {
        pos[depth] = ii;
        combinations_recursive(elems, req_len, result, pos, depth + 1, ii + 1);
    }
    return;
}

std::vector<std::vector<unsigned long>> getCombinations(unsigned long nums_count, unsigned long req_len)
{
    std::vector<unsigned long> positions(req_len, 0);
    std::vector<unsigned long> elems(nums_count);
    std::iota(elems.begin(), elems.end(), 0);
    std::vector<std::vector<unsigned long>> result(0);

    combinations_recursive(elems, req_len, result, positions, 0, 0);

    return result;
}
