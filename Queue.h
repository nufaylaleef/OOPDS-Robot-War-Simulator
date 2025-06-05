#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

template<class T>
struct Node
{
    T data;
    Node<T>* next;

    Node(T x)
    {
        data = x;
        next = nullptr;
    }
};

template<class T>
class Queue
{
private:
    Node<T>* front_;
    Node<T>* back_;
    int sz;

public:
    Queue()
    {
        sz = 0;
        front_ = back_ = nullptr;
    }

    ~Queue() // Destructor
    {
        sz = 0;
        while (!empty())
            dequeue();
    }

    Queue(const Queue<T>& q) // Copy constructor
    {
        cout << "CC" << "\n";
        sz = 0;
        front_ = back_ = nullptr;
        Node<T>* ptr = q.front_;
        while (ptr != nullptr)
        {
            enqueue(ptr->data);
            ptr = ptr->next;
        }
    }

    Queue& operator=(const Queue<T>& q) // Copy Assignment Operator
    {
        cout << "CAO" << "\n";
        if (this != &q)
        {
            sz = 0;
            front_ = back_ = nullptr;
            Node<T>* ptr = q.front_;
            while (ptr != nullptr)
            {
                enqueue(ptr->data);
                ptr = ptr->next;
            }
        }
        return *this;
    }

    Queue(Queue<T>&& q) // Move constructor
    {
        cout << "MC" << "\n";
        sz = q.size();
        front_ = q.front_;
        back_ = q.back_;

        q.sz = 0;
        q.front_ = nullptr;
        q.back_ = nullptr;
    }

    Queue& operator=(Queue<T>&& q) // Move assignment operator
    {
        cout << "MAO" << "\n";
        if (this != &q)
        {
            swap(sz, q.sz);
            swap(front_, q.front_);
            swap(back_, q.back_);
        }
        return *this;
    }

    bool empty()
    {
        return front_ == nullptr;
    }

    int size()
    {
        return sz;
    }

    T front()
    {
        return front_->data;
    }

    T back()
    {
        return back_->data;
    }

    void enqueue(T x)
    {
        if (empty())
        {
            front_ = new Node<T>(x);
            back_ = front_;
        }
        else
        {
            back_->next = new Node<T>(x);
            back_ = back_->next;
        }
        sz++;
    }

    void dequeue()
    {
        if (!empty())
        {
            Node<T>* p = front_;
            T x = front_->data;
            front_ = front_->next;
            delete p;
            sz--;
        }
        else
        {
            cout << "Queue is empty\n";
            exit(EXIT_FAILURE);
        }
    }

    void pop() {
    if (empty()) {
        throw out_of_range("Queue is empty, cannot pop.");
    }

    }

        void printQueue(ostream& out) const {
        Node<T>* current = front_;
        while (current != nullptr) {
            out << *current->data << " | ";
            current = current->next;
        }
        out << endl;
    }

    // Iterator class
    class Iterator
    {
    private:
        Node<T>* current;
    public:
        Iterator(Node<T>* start) : current(start) {}

        T& operator*() { return current->data; }

        Iterator& operator++()
        {
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return current != other.current;
        }
    };

    Iterator begin() { return Iterator(front_); }
    Iterator end() { return Iterator(nullptr); }

    template <class U>
    friend ostream& operator<< (ostream& os, const Queue<U>& q);
};

template <class T>
ostream& operator<< (ostream& os, const Queue<T>& q)
{
    Node<T>* ptr = q.front_;
    while (ptr != nullptr)
    {
        os << ptr->data << ' ';
        ptr = ptr->next;
    }
    return os;
}

#endif // QUEUE_H
