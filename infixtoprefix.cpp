#include "infixtoprefix.hpp"
#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

// I am assuming as per the question that we are giving only a formula whcih is wellformed and is fuly paranthesised

int precedence(char x)
{
    if(x == '~') return 3;
    else if(x == '+' || x == '*') return 2;
    else if(x == '>') return 1;

    else throw -1;
}


string infixtoprefix(string s)
{
    stack<char> stk;
    reverse(s.begin(),s.end());
    string ans = "";
    for(char c : s)
    {
        if(c == ')')
        {
            stk.push(c);
        }
        else if(c == '+' || c == '*' || c == '>')
        {
            stk.push(c);
        }
        else if(c == '(')
        {
            while(stk.top() != ')')
            {
                ans = ans + stk.top();
                stk.pop();
            }
            if(!stk.empty()) stk.pop();
        }
        else
        {
            ans = ans + c;
        }
    }

    while(!stk.empty())
    {
        if(stk.top()!= ')') 
        {
            ans+= stk.top();
            stk.pop();
        }
    }

    reverse(ans.begin(),ans.end());
    return ans;

}




// int main()
// {
//     string s = "(~(p>q))";
//     cout << infixtoprefix(s);

//     return 0;

// }