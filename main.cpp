#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Example{

private:
    int m1;
    vector<int> vec;
    //static size_t counter = 0;

public:
    Example(){}

    Example(int i)
        : m1(i)
    {
        cout << "The constructor is caused with 1 parametr" << endl;
    }

    Example(int i,vector<int> const& v) //копирование
        : m1(i),
          vec(v)
    {
        cout << "The constructor is caused with 2 parametrs with const&\n#without use move"<< endl;
    }

    Example(int i,vector<int>&& v) //перемещение
        : m1(i),
          vec(move(v))
    {
        cout << "The constructor is caused with 2 parametrs\n#with use move" << endl;
    }

    ~Example()
    {
    cout << "The destructor is caused" << endl;
    cout << "i = " << m1 << endl;

    if(!vec.empty())
    {
        for(int i = 0;i < vec.size(); i++)
        {
            cout << vec[i] << ' ';
        }
        cout << endl;
    }
    cout << "------Destructor-----------" << endl;
    }
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

    return 0;
}
