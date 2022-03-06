/* Higher constant - smaller splits */
constexpr double splitConst = 0.7;
const double splitRatio = 
    threshold + (1 - threshold)*splitConst;
const auto begin = last+1;
const auto end = std::min(
    std::uint32_t(begin / splitRatio), 
    50'000u
);

