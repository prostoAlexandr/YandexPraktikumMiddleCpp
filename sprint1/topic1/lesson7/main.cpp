class Image {};

class Box {
public:
    Box() : imageData_(new Image{}) {}
    // Ваш код здесь
    Box(const Box &rhs) {
        if (!rhs.imageData_) {
            imageData_ = nullptr;
            return;
        }
        imageData_ = new Image(*rhs.imageData_);
    }
    Box(Box &&rhs) noexcept : imageData_(rhs.imageData_) { rhs.imageData_ = nullptr; }
    Box &operator=(Box &&rhs) noexcept {
        Image *tmp = imageData_;
        imageData_ = rhs.imageData_;
        rhs.imageData_ = tmp;
        return *this;
    }
    Box &operator=(const Box &rhs) {
        if (&rhs == this) {
            return *this;
        }

        Image *tmp = new Image(*rhs.imageData_);
        delete imageData_;
        imageData_ = tmp;
        return *this;
    }
    ~Box() { delete imageData_; }

private:
    Image *imageData_;
};

int main()
{
    Box box1;
    Box box2(box1);
    Box box3{Box()};
    box1 = box2;
    box1 = Box();
}