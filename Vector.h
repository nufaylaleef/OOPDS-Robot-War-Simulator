#include <iostream>
#include <cstring>
#include <stdexcept>
#include <limits>
using namespace std;

constexpr size_t MAX_VECTOR_SIZE = std::numeric_limits<size_t>::max();
constexpr size_t DEFAULT_RESERVED_SIZE = 4U;

template<typename T>
class Vector
{
private:
    size_t reserved_size = DEFAULT_RESERVED_SIZE;
    size_t vector_size = 0;
    T* storage = nullptr;
    inline void reallocate();
    inline void move_storage(T* dest, T* from, size_t n);

public:
    // using alias names
    using pointer           = T*;
    using const_pointer     = const T*; // two symbolic words to one word
    using iterator          = T*;
    using const_iterator    = const T*;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

    // constructor
    Vector() noexcept;
    explicit Vector(size_t n);
    Vector(size_t n, const T& val);
    Vector(std::initializer_list<T> init);
    Vector(const Vector& origin); // copy constructor
    Vector(Vector<T> && origin) noexcept; // move constructor

    // operator assignment
    Vector<T>& operator=(const Vector<T>& origin);  // constructor assignment operator
    Vector<T>& operator=(Vector<T>&& origin); // move assignment operator
    Vector<T>& operator=(std::initializer_list<T> init);

    // destructor
    ~Vector();

    // element access
    T& operator[](size_t index); // no check exception
    const T& operator[](size_t index) const;
    T& at(size_t index);  // check and throw out_of_range exception
    const T& at(size_t index) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    // iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;


    // capacity
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t max_size() const noexcept;
    size_t capacity() const noexcept;
    void reserve(size_t n);

    // member functions
    void clear() noexcept;
    template <typename ...Args> T& emplace_back(Args&& ...args);
    void push_back(const T& val);
    void push_back(T&& rval);
    void pop_back();
    void resize(size_t n);
    void resize(size_t n, const T& val);
    void swap(Vector<T>& other);

    // friend non-member functions
    template<typename U> friend bool operator==(const Vector<U>& lhs, const Vector<U>& rhs);
    template<typename U> friend bool operator!=(const Vector<T>& lhs, const Vector<U>& rhs);
    template<typename U> friend bool operator<(const Vector<U>& lhs, const Vector<U>& rhs);
    template<typename U> friend bool operator<=(const Vector<U>& lhs, const Vector<U>& rhs);
    template<typename U> friend bool operator>(const Vector<U>& lhs, const Vector<U>& rhs);
    template<typename U> friend bool operator>=(const Vector<U>& lhs, const Vector<U>& rhs);
    template<typename U> friend void swap(Vector<U>& lhs, Vector<U>& rhs);
};

template<typename T>
inline Vector<T>::Vector() noexcept
{
    storage = new T[reserved_size];
}

template<typename T>
inline Vector<T>::Vector(size_t n)
{
    vector_size = n;
    reserved_size = n + n / 2 + 1;
    storage = new T[reserved_size];
    for (size_t i = 0; i < n; i++)
        storage[i] = T();
}

template<typename T>
inline Vector<T>::Vector(size_t n, const T & val)
{
    vector_size = n;
    reserved_size = n + n / 2 + 1;
    storage = new T[reserved_size];
    for (size_t i = 0; i < n; i++)
        storage[i] = val;
}

template<typename T>
inline Vector<T>::Vector(std::initializer_list<T> init)
{
    size_t count = init.size();
    vector_size = 0;
    reserved_size = count + count / 2 + 1;
    storage = new T[reserved_size];
    for (const T& elem : init)
        storage[vector_size++] = elem;
}

template<typename T>
inline Vector<T>::Vector(const Vector &origin) // copy constructor
{
    vector_size = origin.vector_size;
    reserved_size = origin.reserved_size;
    storage = new T[reserved_size];
    for (size_t i = 0; i < vector_size; i++)
        storage[i] = origin.storage[i];
}

template<typename T>
inline Vector<T>::Vector(Vector<T>&& origin) noexcept // move constructor
{
    swap(origin);
}

template<typename T>
inline Vector<T>::~Vector()
{
    if (storage != nullptr)
        delete[] storage;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& origin) // constructor assignment operator
{
    swap(Vector<T>(origin));
    return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(Vector<T>&& origin) // move assignment operator
{
    swap(origin);
    return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(std::initializer_list<T> init)
{
    swap(Vector<T>(init));
    return *this;
}

template<typename T>
inline void Vector<T>::reallocate()
{
    T* new_storage = new T[reserved_size];
    memcpy(static_cast<void*>(new_storage), static_cast<void*>(storage), sizeof(T) * vector_size);
    delete[] storage;
    storage = new_storage;
}

template<typename T>
inline void Vector<T>::move_storage(T * dest, T * from, size_t n)
{
    memmove(static_cast<void*>(dest), static_cast<void*>(from), n * sizeof(T));
}

template<typename T>
inline T& Vector<T>::operator[](size_t index)
{
    return storage[index];
}

template<typename T>
inline const T& Vector<T>::operator[](size_t index) const
{
    return storage[index];
}

template<typename T>
inline T& Vector<T>::at(size_t pos)
{
    if (pos < vector_size)
        return storage[pos];
    throw std::out_of_range{ "non-const method Accessed position is out of range!" };
}

template<typename T>
inline const T& Vector<T>::at(size_t pos) const
{
    if (pos < vector_size)
        return storage[pos];
    throw std::out_of_range{ "const method Accessed position is out of range!" };
}

template<typename T>
inline T& Vector<T>::front()
{
    return storage[0];
}

template<typename T>
inline const T& Vector<T>::front() const
{
    return storage[0];
}

template<typename T>
inline T& Vector<T>::back()
{
    return storage[vector_size - 1];
}

template<typename T>
inline const T& Vector<T>::back() const
{
    return storage[vector_size - 1];
}

template<typename T>
template<typename ...Args>
inline T& Vector<T>::emplace_back(Args && ...args)
{
    if (vector_size == reserved_size)
    {
        reserved_size += reserved_size / 2 + 1;
        reallocate();
    }
    // forward lvalue "args" as either lvalue or as rvalue, depending on T
    // parameter packs "...args" must expand the first parameter args with the rest of parameters with "..."
    return storage[vector_size++] = T(std::forward<Args>(args) ...);
}

template<typename T>
inline void Vector<T>::push_back(const T & val)
{
    emplace_back(val);
}

template<typename T>
inline void Vector<T>::push_back(T && rval)
{
    // forward lvalue "rval" as either lvalue or as rvalue, depending on T
    emplace_back(std::forward<T>(rval));
}

template<typename T>
inline void Vector<T>::pop_back()
{
    storage[--vector_size].~T();
}

template<typename T>
inline void Vector<T>::resize(size_t n)
{
    if (n > vector_size)
    {
        if (n > reserved_size)
        {
            reserved_size = n;
            reallocate();
        }
    }
    else
    {
        for (size_t i = n; i < vector_size; i++)
            storage[i].~T();
    }
    vector_size = n;
}

template<typename T>
inline void Vector<T>::resize(size_t n, const T& val)
{
    if (n > vector_size)
    {
        if (n > reserved_size)
        {
            reserved_size = n + n / 2 + 1;
            reallocate();
        }
        for (size_t i = vector_size; i < n; i++)
            storage[i] = val;
    }
    else
    {
        for (size_t i = n; i < vector_size; i++)
            storage[i].~T();
    }
    vector_size = n;
}

template<typename T>
inline void Vector<T>::swap(Vector<T>& rhs)

{
    using std::swap;
    swap(vector_size, rhs.vector_size);
    swap(reserved_size, rhs.reserved_size);
    swap(storage, rhs.storage);
}

template<typename T>
inline typename Vector<T>::iterator Vector<T>::begin() noexcept
{
    return storage;
}

template<typename T>
inline typename Vector<T>::const_iterator Vector<T>::begin() const noexcept
{
    return storage;
}

template<typename T>
inline typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept
{
    return begin();
}

template<typename T>
inline typename Vector<T>::iterator Vector<T>::end() noexcept
{
    return storage + vector_size;
}

template<typename T>
inline typename Vector<T>::const_iterator Vector<T>::end() const noexcept
{
    return storage + vector_size;
}

template<typename T>
inline typename Vector<T>::const_iterator Vector<T>::cend() const noexcept

{
    return end();
}

template<typename T>
inline typename Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept
{
    return reverse_iterator(storage + vector_size);
}

template<typename T>
inline typename Vector<T>::const_reverse_iterator Vector<T>::rbegin() const noexcept
{
    return reverse_iterator(storage + vector_size);
}

template<typename T>
inline typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() const noexcept
{
    return rbegin();
}

template<typename T>
inline typename Vector<T>::reverse_iterator Vector<T>::rend() noexcept
{
    return reverse_iterator(storage);
}

template<typename T>
inline typename Vector<T>::const_reverse_iterator Vector<T>::rend() const noexcept
{
    return reverse_iterator(storage);
}

template<typename T>
inline typename Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept
{
    return rend();
}

template<typename T>
inline bool Vector<T>::empty() const noexcept
{
    return vector_size == 0;
}

template<typename T>
inline size_t Vector<T>::size() const noexcept
{
    return vector_size;
}

template<typename T>
inline size_t Vector<T>::max_size() const noexcept
{
    return MAX_VECTOR_SIZE;
}

template<typename T>
inline size_t Vector<T>::capacity() const noexcept
{
    return reserved_size;
}

template<typename T>
inline void Vector<T>::reserve(size_t n)
{
    if (n > reserved_size)
    {
        reserved_size = n;
        reallocate();
    }
}

template<typename T>
inline void Vector<T>::clear() noexcept
{
    vector_size = 0;
    for (size_t i = 0; i < vector_size; i++)
        storage[i].~T();
}

template<typename U>
inline bool operator==(const Vector<U>& lhs, const Vector<U>& rhs)
{
    if (lhs.vector_size != rhs.vector_size)
        return false;
    for (size_t i = 0; i < lhs.vector_size; i++)
        if (lhs.storage[i] != rhs.storage[i])
            return false;
    return true;
}

template<typename U>
inline bool operator!=(const Vector<U>& lhs, const Vector<U>& rhs)
{
    return !(lhs == rhs);
}

template<typename U>
inline bool operator<(const Vector<U>& lhs, const Vector<U>& rhs)
{
    size_t n = (lhs.vector_size < rhs.vector_size) ? lhs.vector_size : rhs.vector_size;
    for (size_t i = 0; i < n; i++)
        if (lhs.storage[i] != rhs.storage[i])
            return lhs.storage[i] < rhs.storage[i];
    return lhs.vector_size < rhs.vector_size;
}

template<typename U>
inline bool operator>(const Vector<U>& lhs, const Vector<U>& rhs)
{
    size_t n = lhs.vector_size < rhs.vector_size ? lhs.vector_size : rhs.vector_size;
    for (size_t i = 0; i < n; i++)
        if (lhs.storage[i] != rhs.storage[i])
            return lhs.storage[i] > rhs.storage[i];
    return lhs.vector_size > rhs.vector_size;
}

template<typename U>
inline bool operator<=(const Vector<U>& lhs, const Vector<U>& rhs)
{
    return !(lhs > rhs);
}

template<typename U>
inline bool operator>=(const Vector<U>& lhs, const Vector<U>& rhs)
{
    return !(lhs < rhs);
}

template<typename U>
inline void swap(Vector<U>& lhs, Vector<U>& rhs)
{
    lhs.swap(rhs);
}
