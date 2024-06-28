#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

namespace exceptions {
    class NoFileException {
        string Message;
    public:
        NoFileException(string filename) : Message("No file found: " + filename) {}
        string getMessage() const { return Message; }
    };

    class WrongParameterException {
        string Message;
    public:
        WrongParameterException() : Message("Wrong parameters") {}
        string getMessage() const { return Message; }
    };
}

namespace algorithm1 {
    string file1 = "dat_X_1_1.dat";
    string file2 = "dat_X_00_1.dat";
    string file3 = "dat_X_1_00.dat";

    double T(double x, string filename) {
        ifstream fs(filename);
        if (!fs) throw exceptions::NoFileException(filename);

        float Xp, Tp, Up, Xc, Tc, Uc;
        fs >> Xp >> Tp >> Up;
        if (x == Xp) return Tp;

        while (fs >> Xc >> Tc >> Uc) {
            if ((filename == file1 || filename == file2) && x <= Xc) {
                if (x == Xc) return Tc;
                return Tp + (Tc - Tp) * (x - Xp) / (Xc - Xp);
            }
            if (filename == file3 && x >= Xc) {
                if (x == Xc) return Tc;
                return Tp + (Tc - Tp) * (x - Xp) / (Xc - Xp);
            }
            Xp = Xc;
            Tp = Tc;
            Up = Uc;
        }
        throw exceptions::NoFileException(filename);
    }

    double T(double x) {
        if (abs(x) <= 1) return T(x, file1);
        if (x < -1) return T(1 / x, file2);
        return T(-1 / x, file3);
    }

    double U(double x, string filename) {
        ifstream fs(filename);
        if (!fs) throw exceptions::NoFileException(filename);

        double Xp, Tp, Up, Xc, Tc, Uc;
        fs >> Xp >> Tp >> Up;
        if (x == Xp) return Up;

        while (fs >> Xc >> Tc >> Uc) {
            if ((filename == file1 || filename == file2) && x <= Xc) {
                if (x == Xc) return Uc;
                return Up + (Uc - Up) * (x - Xp) / (Xc - Xp);
            }
            if (filename == file3 && x >= Xc) {
                if (x == Xc) return Uc;
                return Up + (Uc - Up) * (x - Xp) / (Xc - Xp);
            }
            Xp = Xc;
            Tp = Tc;
            Up = Uc;
        }
        throw exceptions::NoFileException(filename);
    }

    double U(double x) {
        if (abs(x) <= 1) return U(x, file1);
        if (x < -1) return U(1 / x, file2);
        return U(1 / x, file3);
    }

    double srz(double x, double y, double z) {
        if (x > y) return T(x) + U(z) - T(y);
        return T(y) + U(y) - U(z);
    }

    double glr(double x, double y) {
        if (abs(x) < 1) return x;
        if (abs(x) >= 1 && abs(y) < 1) return y;
        if (sqrt(x * x + y * y - 4) < 0.1) throw exceptions::WrongParameterException();
        return y / sqrt(x * x + y * y - 4);
    }

    double gold(double x, double y) {
        if (y != 0 && x > y) return x / y;
        if (x != 0 && x < y) return y / x;
        throw exceptions::WrongParameterException();
    }

    double grs(double x, double y) {
        return 0.1389 * srz(x + y, gold(x, y), glr(x, x * y)) +
            1.8389 * srz(x - y, gold(y, x / 5), glr(5 * x, x * y)) +
            0.83 * srz(x - 0.9, glr(y, x / 5), gold(5 * y, y));
    }

    double fun(double x, double y, double z) {
        if (x == y && y == z) {
            return x * x + y * y + 0.33 * x * y;
        }
        return x * x * grs(y, z) + y * y * grs(x, z) + 0.33 * x * y * grs(x, z);
    }
}

namespace algorithm2 {
    double glr(double x, double y) {
        if (abs(x) < 1) return x;
        return y;
    }

    double gold(double x, double y) {
        if (x > y && abs(y) > 0.1) return x / y;
        if (x <= y && abs(x) > 0.1) return y / x;
        if (x < y && abs(x) > 0.1) return 0.15;
        return 0.1;
    }

    double grs(double x, double y) {
        return 0.14 * algorithm1::srz(x + y, gold(x, y), glr(x, x * y)) +
            1.83 * algorithm1::srz(x - y, gold(y, x / 5), glr(4 * x, x * y)) +
            0.83 * algorithm1::srz(x, glr(y, x / 4), gold(4 * y, y));
    }

    double fun(double x, double y, double z) {
        return x * grs(x, y) + y * grs(y, z) + z * grs(z, x);
    }
}

namespace algorithm3 {
    double fun(double x, double y, double z) {
        return 1.3498 * z + 2.2362 * y - 2.348 * x * y;
    }
}

int main() {
    double x, y, z;
    cout << "Input x, y, z: ";
    cin >> x >> y >> z;
    try {
        cout << "Fun = " << algorithm1::fun(x, y, z) << endl;
    }
    catch (exceptions::NoFileException& e) {
        cout << "Algorithm changed to 3" << endl;
        cout << "Fun = " << algorithm3::fun(x, y, z) << endl;
    }
    catch (exceptions::WrongParameterException& e) {
        cout << "Algorithm changed to 2" << endl;
        cout << "Fun = " << algorithm2::fun(x, y, z) << endl;
    }
    catch (...) {
        cout << "Unknown error occurred" << endl;
    }
}
