#include <iterator>
#include <vector>
#include <cstddef>
#include <memory>
#include <iostream>

template<typename T>
class Span {
public:
    /* определите зависимые типы reference, const_reference, pointer, const_pointer, size_type здесь */
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;

    template<typename It>
    Span(It first, size_type count);

    template<typename It1, typename It2>
    Span(It1 first, It2 last);

    reference operator[](size_type idx);
    const_reference operator[](size_type idx) const;

    bool empty() const;

    pointer data();
    const_pointer data() const;

    size_type size() const;

    Span subspan(size_type offset, size_type count) {
        return Span{data_ + offset, count};
    }

private:
    pointer data_;
    size_type size_;
};

template <typename T>
template <typename It>
Span<T>::Span(It first, Span<T>::size_type count) :
    data_(std::to_address(first)),
    size_(count)
{}

template <typename T>
template <typename It1, typename It2>
Span<T>::Span(It1 first, It2 last) :
    data_(std::to_address(first)),
    size_(std::distance(first, last))
{}

template <typename T>
typename Span<T>::reference Span<T>::operator[](Span<T>::size_type idx)
{
    return data_[idx];
}

template <typename T>
typename Span<T>::const_reference Span<T>::operator[](Span<T>::size_type idx) const
{
    return data_[idx];
}

template <typename T>
bool Span<T>::empty() const
{
    return size_ == 0;
}

template <typename T>
typename Span<T>::pointer Span<T>::data()
{
    return data_;
}

template <typename T>
typename Span<T>::const_pointer Span<T>::data() const
{
    return data_;
}

template <typename T>
typename Span<T>::size_type Span<T>::size() const
{
    return size_;
}

template<typename T>
Span<const std::byte> as_bytes(Span<T> span)
{
    return Span<const std::byte>(reinterpret_cast<std::byte*>(span.data()), span.size() * sizeof(T));
}

int main()
{
    std::vector<int> test{1, 2, 3, 4};
    Span<int> span(test.begin(), test.size());
    std::cout << span[1] << " " << span.size() << " " << span.empty() << " " << span.data()[3] << std::endl;
    auto bytespan = as_bytes(span);
}

