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
#include <filesystem>
#include <chrono>
#include <windows.h>
#include <psapi.h>

using namespace std;
namespace fs = std::filesystem;

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
        cout<<"File is not opened"<<endl;
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
        cout<<"File is not opened"<<endl;
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
        cout<<"File is not opened"<<endl;
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
 * @brief Get the current memory usage of the running process.
 * 
 * This function uses the Windows API to retrieve the current working set size
 * of the process (i.e., the amount of memory currently in RAM for the process).
 * It returns the memory usage in kilobytes (KB).
 * 
 * @return size_t The current memory usage of the process in KB. Returns 0 if 
 *                the memory information cannot be retrieved.
 * 
 * @note This measures the memory of the entire process, not a specific function 
 *       or file. Use it to approximate memory usage changes between code sections.
 * 
 * @warning Requires including <windows.h> and <psapi.h>. Also link against 
 *          Psapi.lib if compiling in Visual Studio.
 */

size_t getMemoryKB() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024; // KB
    }
    return 0;
}

/**
 * @brief Entry point for CNF formula analysis and validation.
 *
 * This program iterates through all CNF files in a specified folder,
 * applies CNF validity checking algorithms on each file, and generates
 * an HTML report summarizing the results.
 *
 * Each file is analyzed for:
 * - Validity of the CNF formula (tautological or non-tautological)
 * - Number of valid and invalid clauses
 * - Execution time for the algorithm
 * - Memory used during the analysis
 *
 * The results are presented in an HTML table with color-coded rows:
 * - **Green** for valid CNF formulas
 * - **Red** for invalid CNF formulas
 *
 * @return Returns 0 on successful execution, -1 if the output file cannot be opened.
 *
 *
 * @see getMemoryKB()
 * @see cnf_validcno()
 * @see cnf_non_valid_cno()
 * @see cnf_validitychecker()
 *
 * @par Output
 * Generates an HTML file named **Analysis.html** containing a formatted table of results.
 *
 * @par Example Output:
 * | File | Result | Valid Clauses | Invalid Clauses | Time (ms) | Memory (KB) |
 * |------|---------|---------------|-----------------|------------|--------------|
 * | example.cnf | Invalid | 0 | 125 | 45.2 | 678 |
 *
 * @par Dependencies
 * Requires the C++17 `<filesystem>`, `<chrono>`, `<fstream>`, and `<windows.h>` headers.
 */

int main() {
    string folder_path = "D:\\Projects\\ParseTree-Generator-and-CNF-validity-checker\\cnfextractedfiles";
    string output_file = "D:\\Projects\\ParseTree-Generator-and-CNF-validity-checker\\Analysis.html";

    ofstream out(output_file);
    if (!out) {
        cerr << "Failed to open output file!" << endl;
        return -1;
    }

    // --- HTML header ---
    out << "<!DOCTYPE html>\n<html>\n<head>\n<title>CNF Analysis and Results</title>\n"
        << "<style>\n"
        << "table { border-collapse: collapse; width: 100%; }\n"
        << "th, td { border: 1px solid black; padding: 8px; text-align: left; }\n"
        << "th { background-color: #f2f2f2; }\n"
        << "tr.invalid { background-color: #fdd; }\n"
        << "tr.valid { background-color: #dfd; }\n"
        << "</style>\n</head>\n<body>\n";

    out << "<h1>CNF Files Analysis and Results</h1>\n";
    out << "<table>\n";
    out << "<tr><th>File</th><th>Result</th><th>Valid Clauses</th>"
           "<th>Invalid Clauses</th><th>Time (ms)</th><th>Memory (KB)</th></tr>\n";

    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if(entry.is_regular_file()) {
            string path = entry.path().string();

            size_t memory_before = getMemoryKB();
            auto start = chrono::high_resolution_clock::now();

            int n1 = cnf_validcno(path);
            bool ans = cnf_validitychecker(path);
            int n2 = cnf_non_valid_cno(path);

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed_ms = end - start;
            size_t memory_after = getMemoryKB();
            size_t memory_used = (memory_after > memory_before) ? 
                                 (memory_after - memory_before) : 
                                 (memory_before - memory_after);

            // --- Table row with color based on validity ---

            string filename = fs::path(path).filename().string();
            out << "<tr class='" << (ans ? "valid" : "invalid") << "'>";
            out << "<td>" << filename << "</td>";
            out << "<td>" << (ans ? "Valid" : "Invalid") << "</td>";
            out << "<td>" << n1 << "</td>";
            out << "<td>" << n2 << "</td>";
            out << "<td>" << elapsed_ms.count() << "</td>";
            out << "<td>" << memory_used << "</td>";
            out << "</tr>\n";
        }
    }

    // --- Close HTML ---
    out << "</table>\n</body>\n</html>\n";
    out.close();

    cout << "HTML analysis generated: " << output_file << endl;
    return 0;
}
    
// int main()
// {
//     string folder_path = "D:\\Projects\\ParseTree-Generator-and-CNF-validity-checker\\cnfextractedfiles";
//     string output_file = "D:\\Projects\\ParseTree-Generator-and-CNF-validity-checker\\Analysis.txt";

//     std::ofstream out(output_file);
//     if (!out) {
//         std::cerr << "Failed to open output file!" << std::endl;
//         return -1;
//     }

//     for (const auto& entry : fs::directory_iterator(folder_path))
//     {
//         if(entry.is_regular_file())
//         {
//             string path = entry.path().string();

//             size_t memory_before = getMemoryKB();
//             auto start = chrono::high_resolution_clock::now();

//             int n1 = cnf_validcno(path);
//             bool ans = cnf_validitychecker(path);
//             int n2 = cnf_non_valid_cno(path);

//             auto end = chrono::high_resolution_clock::now();
//             chrono::duration<double, std::milli> elapsed_ms = end - start;
//             size_t memory_after = getMemoryKB();
//             size_t memory_used = (memory_after > memory_before) ? (memory_after - memory_before) : (memory_before-memory_after);

//             out<< path<<endl;
//             out<<endl;
//             if(ans == true) out<<"Valid" << endl;
//             else out<< "Invalid"<< endl;
//             out<< "No of valid clauses: " << n1 << endl;
//             out<< "No of invalid clauses: " << n2 <<endl;
//             out << "Time taken: " << elapsed_ms.count() << " ms" << endl;
//             out << "Memory used: " << memory_used << " KB" << endl;
//             out <<endl<<endl;
//         }

//     }
// }

    