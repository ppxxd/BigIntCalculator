#include "bigint.hpp"
using namespace std;

void action(const BigNumber& first_n);
string typein()
{
    string value;
    cout << "Enter your number:" << endl;
    while (true)
    {
        cin >> value;
        int counter =   value.length();
        for (int i = value[0] == '-' ? 1 : 0; i < value.length(); i++)
            if (!isdigit(value[i])) {
                counter--;
            }
        if (counter != value.length()) {
            cout << "It's not a number! Try again: \n";
            cin.clear();
            while (cin.get() != '\n');
        }
        else
        {
            break;
        }
    }
    return value;
}
void clear()
    {
        BigNumber first_n = typein();
        action(first_n);
    }
void action(const BigNumber& first_n)
    {
        string action_с;
        cout << "Type: + | - | * | / | % | ± | AC" << endl;
        while (true)
        {
            cin >> action_с;
            if (action_с != "-" and action_с != "+" and action_с != "*" and action_с != "/" and action_с != "AC" and action_с != "%" and action_с != "±")
            {
                cout << "It's not an action! Try again: \n";
                cin.clear();
                while (cin.get() != '\n');
            }
            else
            {
                break;
            }
        }

        BigNumber second_n;
        if (action_с != "±" and action_с != "AC")
        {

            second_n = typein();
        }

        BigNumber result;
        if (action_с == "+")
        {
            result = first_n + second_n;
            cout << "Result: " << result << endl;
            action(result);
        }
        else if (action_с == "-")
        {
            result = first_n - second_n;
            cout << "Result: " << result << endl;
            action(result);
        }
        else if (action_с == "*")
        {
            result = first_n * second_n;
            cout << "Result: " << result << endl;
            action(result);
        }
        else if (action_с == "/")
        {
            result = first_n / second_n;
            cout << "Result: " << result << endl;
            action(result);
        }
        else if (action_с == "AC")
        {
            clear();
        }
        else if (action_с == "±")
        {
            result = -first_n;
            cout << "Result: " << result << endl;
            action(result);
        }
        else if (action_с == "%")
        {
            result = first_n % second_n;
            cout << "Result: " << result << endl;
            action(result);
        }


    }

