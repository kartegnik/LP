#include <cmath>
#include <stdexcept>

struct Tools {
private:
    inline static double eps_ = 1e-6;

public:
    static double   setEps(double e) {
        if (e < 0) throw std::invalid_argument("eps must be >= 0");
        double oldEps = eps_;
        eps_ = e;
        return oldEps;
    }
    static double getEps() {
        return eps_;
    }

    // cmp возвращает:
    //   0   если |a-b| <= eps
    //  -1   если a < b−eps
    //  +1   если a > b+eps
    static int cmp(double a, double b) {
        double d = a - b;
        double ad = std::fabs(d);
        if (ad <= eps_)       return 0;
        return (d < 0 ? -1 : +1);
    }

    static bool EQ(double a, double b) { return cmp(a, b) == 0; }
    static bool NE(double a, double b) { return cmp(a, b) != 0; }
    static bool LT(double a, double b) { return cmp(a, b) < 0; }
    static bool LE(double a, double b) { return cmp(a, b) <= 0; }
    static bool GT(double a, double b) { return cmp(a, b) > 0; }
    static bool GE(double a, double b) { return cmp(a, b) >= 0; }
};