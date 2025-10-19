/**
 * @file cnfsat2002.cpp
 * @brief CNF (Conjunctive Normal Form) SAT formula validator
 * @author K Vikashh Adaikalavan
 * @date 2025
 * 
 * This program reads CNF formulas from a file and validates them by checking
 * if clauses are valid. A clause is considered valid if it contains
 * both a literal and its negation (making it always true).
 */

#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<unordered_set>

using namespace std;

/**
 * @brief Counts the number of valid clauses in a CNF formula file
 * 
 * A clause is considered valid if it contains both a literal and its negation
 * (e.g., x and -x), which makes the clause always satisfiable (tautology).
 * 
 * @param filepath Path to the CNF file in DIMACS format
 * @return int Number of valid clauses found, or -1 if file cannot be opened
 * 
 * @note The CNF file should follow DIMACS format:
 *       - Header line: p cnf <num_variables> <num_clauses>
 *       - Each clause line contains space-separated integers ending with 0
 *       - Negative numbers represent negated literals
 * 
 * @warning This function opens and reads the file each time it's called
 */
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
    
    // Skip lines until we find the problem line (starts with 'p')
    while(getline(f,line))
    {
        if(line[0] == 'p') break;
    }
    
    // Parse the problem line: p cnf <variables> <clauses>
    stringstream ss(line);
    getline(ss,token,' ');  // 'p'
    getline(ss,token,' ');  // 'cnf'
    getline(ss,token,' ');  // number of variables
    int var_no = stoi(token);
    getline(ss,token,' ');  // number of clauses
    int clause_no = stoi(token);
    int no;

    unordered_set<int> uset;
    
    // Process each clause
    for(int i = 0; i<clause_no; i++)
    {
        uset.clear();

        getline(f,line);
        stringstream ss(line);
        
        // Read each literal in the clause
        while(getline(ss,token,' '))
        {
            no = stoi(token);
            if(no==0)  // 0 marks end of clause
            {
                continue;
            }
            else
            {
                // Check if negation of current literal already exists
                if(uset.find(no*(-1))!=uset.end())
                {
                    valid_clause_no++;
                    break;  // Found a tautology, move to next clause
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

/**
 * @brief Counts the number of non-valid (non-tautology) clauses in a CNF formula
 * 
 * Calculates the difference between total clauses and valid clauses.
 * 
 * @param filepath Path to the CNF file in DIMACS format
 * @return int Number of non-valid clauses, or -1 if file cannot be opened
 * 
 * @see cnf_validcno()
 */
int cnf_non_valid_cno(string filepath)
{
    ifstream f(filepath);

    if(!f.is_open())
    {
        cout<<"File is not opened";
        return -1;
    }

    string line;
    string token;
    
    // Skip to problem line
    while(getline(f,line))
    {
        if(line[0] == 'p') break;
    }
    
    // Parse problem line
    stringstream ss(line);
    getline(ss,token,' ');
    getline(ss,token,' ');
    getline(ss,token,' ');
    int var_no = stoi(token);
    getline(ss,token,' ');
    int clause_no = stoi(token);
    int no;
    
    return clause_no-cnf_validcno(filepath);
}

/**
 * @brief Checks if all clauses in a CNF formula are valid (tautologies)
 * 
 * A CNF formula is considered "valid" in this context if all of its clauses
 * are tautologies (each contains both a literal and its negation).
 * 
 * @param filepath Path to the CNF file in DIMACS format
 * @return bool True if all clauses are valid (tautologies), false otherwise
 * @return bool Returns false (cast from -1) if file cannot be opened
 * 
 * @note This definition of "valid" is specific to this implementation and
 *       differs from standard SAT terminology
 * 
 * @see cnf_validcno()
 */
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
    
    // Skip to problem line
    while(getline(f,line))
    {
        if(line[0] == 'p') break;
    }
    
    // Parse problem line
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

/**
 * @brief Main function to test CNF validity checker
 * 
 * Reads a CNF file and outputs:
 * - Whether the formula is valid (all clauses are tautologies)
 * - Number of valid clauses
 * - Number of invalid clauses
 * 
 * @return int Program exit status (0 for success)
 */
int main()
{
    string path = "D:\\Projects\\Logic_Assignment\\cnftest.txt";
    int n = cnf_validcno(path);
    bool ans = cnf_validitychecker(path);
    if(ans == true) cout<<"Valid" << endl;
    else cout<< "Invalid"<< endl;
    cout<< "No of valid clauses: " << n << endl;
    cout<< "No of invalid clauses: " << cnf_non_valid_cno(path)<<endl;
}