#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<unordered_set>

using namespace std;

int cnf_validcno(string filepath)
{
    int valid_clause_no = 0;
    ifstream f(filepath);

    if(!f.is_open())
    {
        cout<<"File is not opened";
        return -1;
    }

    string line;
    string token;
    while(getline(f,line))
    {
        if(line[0] == 'p') break;
    }
    stringstream ss(line);
    getline(ss,token,' ');
    getline(ss,token,' ');
    getline(ss,token,' ');
    int var_no = stoi(token);
    getline(ss,token,' ');
    int clause_no = stoi(token);
    int no;

    unordered_set<int> uset;
    for(int i = 0; i<clause_no; i++)
    {
        uset.clear();

        getline(f,line);
        stringstream ss(line);
        while(getline(ss,token,' '))
        {
            no = stoi(token);
            if(no==0)
            {
                break;
            }
            else
            {
                if(uset.find(no*(-1))!=uset.end())
                {
                    valid_clause_no++;
                    break;
                }
                else
                {
                    uset.insert(no);
                }
            }
        }

    }
    f.close();

    return valid_clause_no;

}

bool cnf_validitychecker(string filepath)
{
    ifstream f(filepath);

    if(!f.is_open())
    {
        cout<<"File is not opened";
        return -1;
    }

    string line;
    string token;
    while(getline(f,line))
    {
        if(line[0] == 'p') break;
    }
    stringstream ss(line);
    getline(ss,token,' ');
    getline(ss,token,' ');
    getline(ss,token,' ');
    getline(ss,token,' ');
    int clause_no = stoi(token);
    int valid_clauseno = cnf_validcno(filepath);
    f.close();
    if(valid_clauseno == clause_no) return true;
    else return false;
}


int main()
{
    string path = "D:\\Projects\\Logic_Assignment\\cnftest.txt";
    int n = cnf_validcno(path);
    bool ans = cnf_validitychecker(path);
    if(ans == true) cout<<"Valid" << endl;
    else cout<< "Invalid"<< endl;
    cout<< "No of valid clauses: " << n << endl;
}
