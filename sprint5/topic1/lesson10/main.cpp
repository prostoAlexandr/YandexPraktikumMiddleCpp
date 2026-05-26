#include <memory>
#include <stdexcept>

struct Point2D {
    int x, y;
    Point2D(int x, int y) : x(x), y(y) {}
};

struct Point3D {
    int x, y, z;
    Point3D(int x, int y, int z) : x(x), y(y), z(z) {}
};

class Data {
public:
    enum class Type { None, P3D, P2D, Int };

    void setPoint3D(const Point3D &p) {
        Destroy();
        std::construct_at(std::addressof(data.p3d), p);
        type = Type::P3D;
    }

    void setPoint2D(const Point2D &p) {
        Destroy();
        std::construct_at(std::addressof(data.p2d), p);
        type = Type::P2D;
    }

    void setInt(int value) {
        Destroy();
        data.i = value;
        type = Type::Int;
    }

    Type getType() { return type; }

    ~Data() { Destroy(); }

private:
    union Storage {
        Point3D p3d;
        Point2D p2d;
        int i;

        Storage() {}
        ~Storage() {}
    } data;

    Type type = Type::None;

    void Destroy() {
        switch (type) {
        case Type::P2D:
            std::destroy_at(std::addressof(data.p2d));
            break;
        case Type::P3D:
            std::destroy_at(std::addressof(data.p3d));
            break;
        default:
            break;
        }
        type = Type::None;
    }
};

void ProcessStruct(Data &d) {
    switch (d.getType()) {
    case Data::Type::P3D: { /* ... */
    }
    case Data::Type::Int: { /* ... */
    }
    default:
        throw std::runtime_error("Unknown type in ProcessStruct");
    }
}

int main() {}