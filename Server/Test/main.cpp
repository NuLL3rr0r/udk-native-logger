#include <iostream>
#include <string>
#include <cstdio>
#include <Windows.h>
#include "log.hpp"

using namespace std;

class Test
{
public:
    Test()
    {
        Log::Initialize("Testbed");
    }

    ~Test()
    {
        cout << "Shutting down...." << endl;
    }

    void Start()
    {
        Log::OnLogSent = boost::bind(&Test::OnMessageSent, this, _1);
        size_t i = 0;
        while(true) {
            cout << "LOGGING : " << i << endl << endl;
            DBG("Hello", i);
            Sleep(1);
            ++i;
        }
    }

private:
    void OnMessageSent(const string &msg)
    {
        cout << "Sent Message :  " << endl << msg << endl;
    }
};

int main()
{
    Test t;
    t.Start();

    return 0;
}

