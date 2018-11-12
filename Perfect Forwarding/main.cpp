#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <functional>

using namespace std;

class Example{
public:
    Example(){}
    Example(const Example& other)//
        : m1_(other.m1_)
        , vec_(other.vec_)
    {
        cout <<"copy\n";
    }

    Example(Example&& other)//
        : m1_(other.m1_)
        , vec_(std::move(other.vec_))
    {
        cout << "move\n";
    }

    Example(int i)
        : m1_(i)
    {
        cout << "The constructor is caused with 1 parametr" << endl;
    }

    Example(int i,vector<int> const& v) //копирование
        : m1_(i),
          vec_(v)
    {
        cout << "The constructor is caused with 2 parametrs with const&\n#without use move"<< endl;
    }

    Example(int i,vector<int>&& v) //перемещение
        : m1_(i),
          vec_(move(v))
    {
        cout << "The constructor is caused with 2 parametrs\n#with use move" << endl;
    }

    ~Example()
    {
    cout << "The destructor is caused" << endl;
    cout << "i = " << m1_ << endl;

    if(!vec_.empty())
    {
        for(int i = 0;i < vec_.size(); i++)
        {
            cout << vec_[i] << ' ';
        }
        cout << endl;
    }
    cout << "------Destructor-----------" << endl;
    }

    int vecSize() const noexcept { return vec_.size(); }
private:
    int m1_;
    vector<int> vec_;
    //static size_t counter = 0;

};

class B{

public:
    template<typename ... Args>
    void Add(Args&& ... arg)
    {
        v.emplace_back(forward<Args>(arg)...);
    }

    template<typename ... Args>
    void Insert(Args&& ... arg)
    {
        v.emplace(v.begin(),forward<Args>(arg)...);
    }

private:
    vector<Example> v;
};

int main()
{
    B b;
    int i = 2;
    vector<int> v = {1,2,3,4,5,7,8,9};
    b.Add(i,v);
    //b.Add(122,move(v));

    std::vector<Example> v1;
    v1.push_back(Example(1, v));

    Example ex1(2, v);
    v1.emplace_back(std::move(ex1));
    cout << ex1.vecSize() << endl;

    return 0;
}
