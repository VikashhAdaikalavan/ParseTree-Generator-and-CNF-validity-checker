/**
 * @file parserandcnfconverter.cpp
 * @brief Propositional Logic Formula to CNF (Conjunctive Normal Form) Converter
 * @author K Vikashh Adaikalavan
 * @date 2025
 * 
 * This program converts propositional logic formulas from infix notation to CNF form.
 * It builds parse trees, evaluates truth values, and validates CNF formulas by checking
 * for tautological clauses.
 * 
 * Supported operators:
 * - '+' : OR (disjunction)
 * - '*' : AND (conjunction)
 * - '~' : NOT (negation)
 * - '>' : IMPLIES (implication)
 * 
 * @note Assumes input formulas are well-formed and fully parenthesized
 */

#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <bitset>
#include <cstdlib>
#include <stack>

using namespace std;

// I am assuming as per the question that we are giving only a formula which is wellformed and is fully parenthesised


/**
 * @brief Converts a fully parenthesized infix formula to prefix notation
 * 
 * Algorithm:
 * 1. Reverse the input string
 * 2. Process each character:
 *    - ')' → push to stack (becomes opening delimiter after reversal)
 *    - Operators (+, *, >) → push to stack
 *    - '(' → pop operators from stack until ')' is found (becomes closing delimiter)
 *    - Variables/literals → add directly to result
 * 3. Pop any remaining operators from stack
 * 4. Reverse the result to get prefix notation
 * 
 * @param s Infix notation string (must be well-formed and fully parenthesized)
 * @return string The formula in prefix notation
 * 
 * @note This algorithm works by:
 *       - Reversing the input to process right-to-left
 *       - Using a stack to handle operator ordering
 *       - Swapping parentheses roles (due to reversal)
 *       - Reversing the result to get correct prefix order
 * 
 * @warning Assumes input is well-formed and fully parenthesized. 
 *          Malformed input may produce incorrect results.
 * 
 * Example:
 * @code
 * infixtoprefix("(p>q)") returns ">pq"
 * infixtoprefix("(~(p>q))") returns "~>pq"
 * infixtoprefix("((p+q)*(r>s))") returns "*+pq>rs"
 * @endcode
 */

string infixtoprefix(string s)
{
    stack<char> stk;
    reverse(s.begin(),s.end());
    string ans = "";
    
    // Process each character in the reversed string
    for(char c : s)
    {
        if(c == ')')  // After reversal, ')' acts as opening parenthesis
        {
            stk.push(c);
        }
        else if(c == '+' || c == '*' || c == '>')  // Binary operators
        {
            stk.push(c);
        }
        else if(c == '(')  // After reversal, '(' acts as closing parenthesis
        {
            // Pop operators until matching ')' is found
            while(stk.top() != ')')
            {
                ans = ans + stk.top();
                stk.pop();
            }
            if(!stk.empty()) stk.pop();  // Remove the matching ')'
        }
        else  // Variables and literals (including '~')
        {
            ans = ans + c;
        }
    }

    // Pop any remaining operators from stack
    while(!stk.empty())
    {
        if(stk.top()!= ')') 
        {
            ans+= stk.top();
            stk.pop();
        }
    }

    reverse(ans.begin(),ans.end());  // Reverse to get final prefix notation
    return ans;

}




// int main()
// {
//     string s = "(~(p>q))";
//     cout << infixtoprefix(s);

//     return 0;

// }



/**
 * @class Treenode
 * @brief Represents a node in the parse tree for propositional formulas
 * 
 * Each node can represent either:
 * - An operator (+, *, >, ~)
 * - A propositional variable (a-z, A-Z)
 */
class Treenode
{
    public :
        char nodeval;        ///< The value stored in the node (operator or variable)
        Treenode* left;      ///< Pointer to left child node
        Treenode* right;     ///< Pointer to right child node
        int truthvalue = 0;  ///< Truth value of the node (0 or 1)

        /**
         * @brief Constructor for Treenode
         * @param nv Node value (operator or variable)
         * @param l Pointer to left child
         * @param r Pointer to right child
         */
        Treenode(char nv, Treenode* l, Treenode* r)
        {
            this->left = l;
            this->right = r;
            this->nodeval = nv;
        }
};


/**
 * @class Parsetree
 * @brief Builds and manages a parse tree from prefix notation strings
 * 
 * Creates a binary tree representation of a logical formula where:
 * - Internal nodes are operators
 * - Leaf nodes are propositional variables
 * - Unary operators (~) have only right child
 * - Binary operators (+, *, >) have both children
 */
class Parsetree
{
    public:
        int i;                ///< Index for parsing the prefix string
        Treenode* root = nullptr;  ///< Root node of the parse tree

        /**
         * @brief Constructor that builds a parse tree from prefix notation
         * @param s Prefix notation string of the logical formula
         */
        Parsetree(string s)
        {
            i = 0;
            if(s == "") return;
            else root = buildTree(s,i);
        }

        /**
         * @brief Recursively builds the parse tree from prefix notation
         * @param s Prefix notation string
         * @param i Reference to current parsing index
         * @return Treenode* Pointer to the constructed subtree
         * 
         * @note This uses recursive descent parsing:
         *       - For binary operators: builds left then right subtree
         *       - For unary NOT: builds only right subtree
         *       - For variables: creates leaf node
         */
        Treenode* buildTree(string s, int &i)
        {
            if (i >= s.size()) return nullptr;

            char c = s[i++];
            Treenode* node = new Treenode(c, nullptr, nullptr);

            if (c == '+' ||c == '*' || c == '>')
            {
                node->left = buildTree(s, i);
                node->right = buildTree(s, i);
            }
            else if( c == '~')
            {
                node->left = nullptr;
                node->right = buildTree(s,i);
            }

            return node;
        }

        /**
         * @brief Prints the tree in infix notation with parentheses (private helper)
         * @param node Current node being printed
         */
        void printtree(Treenode* node)
        {
            if (node == nullptr)
                return;

            if (node->nodeval == '~') {
                cout << "(~";
                printtree(node->right);
                cout << ")";
            } else {
                cout << "(";
                printtree(node->left);
                cout << node->nodeval;
                printtree(node->right);
                cout << ")";
            }
        }

        /**
         * @brief Prints the entire tree in infix notation with parentheses
         */
        void printtree()
        {
            printtree(root);
        }

        /**
         * @brief Computes the height of the parse tree
         * @return int Height of the tree (number of edges in longest path from root to leaf)
         */
        int height()
        {
            return height(root);
        }

        /**
         * @brief Recursively computes height of subtree (private helper)
         * @param node Root of subtree
         * @return int Height of the subtree
         */
        int height(Treenode* node)
        {
            if(node == nullptr) return 0;
            else
            {
                return 1+max(height(node->left),height(node->right));
            }
        }

};

/**
 * @class valuecomputer
 * @brief Computes truth values of propositional formulas
 * 
 * Assigns truth values to propositional variables and evaluates
 * the entire formula. Can generate complete truth tables for all
 * possible variable assignments.
 */
class valuecomputer
{
    public :
    Treenode* root;                ///< Root of the parse tree to evaluate
    map<char,int> atomvals;        ///< Original variable assignments
    map<char,int> atomvals_temp;   ///< Temporary variable assignments for truth table
    set<char> atomslist;         ///< Set of unique variables in the logic formula
        /**
     * @brief Constructor for valuecomputer with variable assignments
     * @param r Pointer to root of parse tree
     * @param d Map of variable names to truth values (0 or 1)
     * 
     * @note Initializes the evaluator with a parse tree and predefined truth
     *       value assignments for all variables in the formula.
     */
    valuecomputer(Treenode* r, map<char,int> d)
    {
        root = r;
        atomvals = d;
        atomvals_temp = d;
    }

        /**
     * @brief Constructor for valuecomputer without initial variable assignments
     * @param r Pointer to root of parse tree
     * 
     * @note This constructor automatically discovers all unique variables in
     *       the parse tree by calling calc_no_of_atoms(). The atomslist member
     *       will be populated, but atomvals and atomvals_temp will remain empty
     *       until truth values are assigned separately.
     * 
     * @note Use this constructor when you want to analyze the formula structure
     *       before assigning specific truth values, or when generating truth tables.
     */
    valuecomputer(Treenode* r)
    {
        root = r;
        calc_no_of_atoms();
    }


        /**
     * @brief Collects all unique variables (atoms) present in the parse tree
     * @param node Current node being traversed in the parse tree
     * @param atomslist Reference to vector that will store unique variable names
     * 
     * @note This function performs a recursive traversal of the parse tree
     *       to identify all leaf nodes (variables). Using a set ensures
     *       automatic handling of duplicates with O(log n) insertion time.
     * 
     * @note The function treats leaf nodes (nodes with no children) as atoms/variables
     *       and internal nodes as operators that are not added to the list.
     */
    void calc_no_of_atoms(Treenode* node, set<char> &atomset)
    {
        if(node == nullptr)
        {
            return;
        }
        
        // If it's a leaf node (variable/atom)
        if(node->left == nullptr && node->right == nullptr)
        {
            atomset.insert(node->nodeval);  // Set automatically handles duplicates
            return;
        }
        
        // Recursively traverse left and right subtrees
        if(node->left != nullptr)
        {
            calc_no_of_atoms(node->left, atomset);
        }
        if(node->right != nullptr)
        {
            calc_no_of_atoms(node->right, atomset);
        }
    }

    /**
     * @brief Calculates and returns all unique variables in the formula
     * @return set<char> Set containing all unique variable names (atoms) in sorted order
     * 
     * @note Using set provides automatic duplicate handling and maintains sorted order.
     *       The set is stored as a class member for potential reuse.
     */
    set<char> calc_no_of_atoms()
    {
        calc_no_of_atoms(root, atomslist);
        return atomslist;
    }


    /**
     * @brief Assigns truth values to leaf nodes (variables) in the tree
     * @param node Current node being processed
     * @param atomval Map of variable assignments
     * 
     * @note This traverses the tree and sets truthvalue field for all variables
     */
    void assignatoms(Treenode* node,map<char,int> &atomval)
    {
        if(node ->left == nullptr && node ->right == nullptr)
        {
            node->truthvalue = atomval[node->nodeval];
            return ;
        }
        else if(node->nodeval == '~')
        {
            assignatoms(node->right,atomval);
        }
        else
        {
            assignatoms(node->left,atomval);
            assignatoms(node->right,atomval);
        }
    }


    /**
     * @brief Sets the map of the valuecomputer to a give state
     * @param mpp The map to be assigned to it
     * @return void
     */
    void set_atomvals(map<char,int> mpp)
    {
        atomvals = mpp;
    }

    /**
     * @brief Computes truth value using original variable assignments
     * @return int Truth value of the formula (0 or 1)
     */
    int computetruth()
    {
        assignatoms(root,atomvals);
        return computetruth(root);
    }

    /**
     * @brief Computes truth value using provided variable assignments
     * @param atomval Map of variable assignments
     * @return int Truth value of the formula (0 or 1)
     */
    int computetruth(map<char,int> atomval)
    {
        assignatoms(root,atomval);
        return computetruth(root);
    }

    /**
     * @brief Recursively evaluates truth value of formula (private helper)
     * @param node Current node being evaluated
     * @return int Truth value at this node (0 or 1)
     * 
     * Evaluation rules:
     * - OR (+): left | right
     * - AND (*): left & right
     * - IMPLIES (>): !left | right
     * - NOT (~): !right
     * - Variable: assigned truth value
     */
    int computetruth(Treenode* node)
    {
        if(node->nodeval == '+') 
        {
            node->truthvalue = computetruth(node->left)|computetruth(node->right);
            return node->truthvalue;
        }
        else if( node->nodeval == '*')
        {
            node->truthvalue = computetruth(node->left)&computetruth(node->right);
            return node->truthvalue;
        }
        else if(node-> nodeval == '>')
        {
            node->truthvalue = (!computetruth(node->left))|computetruth(node->right);
            return node->truthvalue;
        }
        else if(node->nodeval == '~')
        {
            node->truthvalue = !(computetruth(node->right));
            return node->truthvalue;
        }
        else
        {
            return node->truthvalue;
        }

    }

    /**
     * @brief Generates and prints complete truth table for the formula
     * 
     * Iterates through all possible truth value assignments for variables
     * and prints the resulting truth value of the formula for each assignment.
     * Uses binary counting to generate all 2^n combinations where n is
     * the number of variables.
     */
    void computealltruth()
    {
        vector<char> v;
        calc_no_of_atoms();
        for(auto k: atomslist)
        {
            v.push_back(k);
            cout << k;
        }
        cout<<" Truth value"<<endl;
        int n = 1<< atomslist.size();
        for(int i = 0; i<n; i++)
        {
            bitset<64> bits(i);
            string bitstring = bits.to_string().substr(64-atomslist.size());
            cout << bitstring << " ";
            for(int j = 0; j<atomslist.size();j++)
            {
                atomvals_temp[v[j]] = bitstring[j]-'0';
            } 
            cout << computetruth(atomvals_temp)<<endl;

        }
    }
};


/**
 * @class CNFConverter
 * @brief Converts propositional logic formulas to Conjunctive Normal Form (CNF)
 * 
 * Performs a three-step conversion process:
 * 1. Eliminate implications (convert > to + and ~)
 * 2. Convert to Negation Normal Form (NNF) - push negations inward
 * 3. Distribute OR over AND to get CNF
 * 
 * Also validates CNF formulas by checking for tautological clauses
 * (clauses containing both a literal and its negation).
 */
class CNFConverter{

    public:
    Treenode* roottree;      ///< Root of the parse tree being converted
    string prefix_string;    ///< Prefix notation of the formula
    int valid_clause = 0;    ///< Count of valid (tautological) clauses

    /**
     * @brief Constructor that initializes the converter with an infix formula
     * @param s Infix notation string of the logical formula
     */
    CNFConverter(string s)
    {
        prefix_string = infixtoprefix(s);
        roottree = (new Parsetree(prefix_string))->root;
    }

    /**
     * @brief Eliminates implications from the formula (private helper)
     * @param node Current node being processed
     * @return Treenode* Transformed subtree with implications removed
     * 
     * Transformation rule: (A > B) becomes (~A + B)
     * Recursively processes all implication operators in the tree.
     */
    Treenode* impfree(Treenode * node)
    {
        if(node == nullptr)
        {
            return nullptr;
        }
        else if((node->nodeval >= 'a' && node-> nodeval <= 'z')||(node->nodeval >= 'A' && node-> nodeval <= 'Z'))
        {
            return node;
        }
        else if(node->nodeval == '>') 
        {
            node->nodeval = '+';
            // Treenode* templ = impfree(node->left);
            // Treenode* tempr = impfree(node->right);
            node-> left = new Treenode('~',nullptr,impfree(node->left));
            node-> right = impfree(node->right);
            return node;
        }
        else
        {
            node->left = impfree(node->left);
            node->right = impfree(node->right);
            return node;
        }
    }

    /**
     * @brief Eliminates implications from the entire tree
     * @return Treenode* Root of the transformed tree
     */
    Treenode* impfree()
    {
        impfree(roottree);
        return roottree;
    }

    /**
     * @brief Converts formula to Negation Normal Form (private helper)
     * @param node Current node being processed
     * @return Treenode* Transformed subtree in NNF
     * 
     * NNF transformation rules (De Morgan's laws):
     * - ~(A + B) becomes (~A * ~B)
     * - ~(A * B) becomes (~A + ~B)
     * - ~~A becomes A
     * 
     * Pushes all negations down to the variable level.
     */
    Treenode* nnf(Treenode* node)
    {
        if(node == nullptr)
        {
            return nullptr;
        }
        if(node->nodeval == '~')
        {
            if(node->right->nodeval == '+')
            {
                node->nodeval = '*';
                Treenode* templ = new Treenode('~',nullptr,node->right->left);
                Treenode* tempr = new Treenode('~',nullptr,node->right->right);
                node->left = nnf(templ);
                node->right = nnf(tempr);
                return node;
            }
            else if(node->right->nodeval == '*')
            {
                node->nodeval = '+';
                Treenode* templ = new Treenode('~',nullptr,node->right->left);
                Treenode* tempr = new Treenode('~',nullptr,node->right->right);
                node->left = nnf(templ);
                node->right = nnf(tempr);
                return node;
            }
            else if(node->right->nodeval == '~')
            {
                node->nodeval = node->right->right->nodeval;
                node->left = nnf(node->right->right->left);
                node->right = nnf(node->right->right->right);
                return node;
            }
            else
            {
                return node;
            }

        }
        else if((node->nodeval >= 'a' && node-> nodeval <= 'z')||(node->nodeval >= 'A' && node-> nodeval <= 'Z'))
        {
            return node;
        }
        else
        {
           node->left = nnf(node->left);
           node->right = nnf(node->right);
           return node;
        }

    }

    /**
     * @brief Converts entire formula to Negation Normal Form
     */
    void nnf()
    {
        nnf(roottree);
    }


    /**
     * @brief Distributes OR over AND to maintain CNF structure
     * @param A Left subtree
     * @param B Right subtree
     * @return Treenode* New tree with OR distributed over AND
     * 
     * Distribution rules:
     * - (A * B) + C becomes (A + C) * (B + C)
     * - A + (B * C) becomes (A + B) * (A + C)
     * - literal + literal stays as is
     * 
     * This is the key operation for converting to CNF.
     */
    Treenode* DISTR(Treenode* A, Treenode* B)
    {
        if (!A) return B;
        if (!B) return A;

        
        if ((A->nodeval >= 'a' && A->nodeval <= 'z') || (A->nodeval >= 'A' && A->nodeval <= 'Z') || A->nodeval == '~')
            if ((B->nodeval >= 'a' && B->nodeval <= 'z') || (B->nodeval >= 'A' && B->nodeval <= 'Z') || B->nodeval == '~')
                return new Treenode('+', A, B);

        if (A->nodeval == '*')
        {
            Treenode* left = DISTR(A->left, B);
            Treenode* right = DISTR(A->right, B);
            return new Treenode('*', left, right);
        }

        if (B->nodeval == '*')
        {
            Treenode* left = DISTR(A, B->left);
            Treenode* right = DISTR(A, B->right);
            return new Treenode('*', left, right);
        }

        return new Treenode('+', A, B);
    }

    /**
     * @brief Converts formula to CNF (private helper)
     * @param phi Current node being converted
     * @return Treenode* CNF version of the subtree
     * 
     * Recursively converts to CNF by:
     * - Preserving AND nodes
     * - Distributing OR over AND when needed
     * - Keeping literals as-is
     */
    Treenode* CNF(Treenode* phi)
    {
        if (phi == nullptr)
            return nullptr;

        if ((phi->nodeval >= 'a' && phi->nodeval <= 'z') || (phi->nodeval >= 'A' && phi->nodeval <= 'Z') || phi->nodeval == '~')
            return phi;

        if (phi->nodeval == '*')
        {
            Treenode* left = CNF(phi->left);
            Treenode* right = CNF(phi->right);
            return new Treenode('*', left, right);
        }

        if (phi->nodeval == '+')
        {
            Treenode* left = CNF(phi->left);
            Treenode* right = CNF(phi->right);
            return DISTR(left, right);
        }

        return phi;
    }

    /**
     * @brief Converts the entire formula to CNF
     * 
     * Three-step process:
     * 1. Remove implications
     * 2. Convert to NNF
     * 3. Apply distribution to get CNF
     */
    void cnf()
    {
        
        impfree();
        nnf();
        roottree = CNF(roottree);
    }

    /**
     * @brief Converts parse tree to string representation
     * @param root Current node being converted
     * @param result Reference to result string being built
     * 
     * Creates a string with operators and variables in order.
     * Used for analyzing clause structure.
     */
    void treeToString(Treenode* root, string &result)
    {
        if (!root) return;

        if (root->nodeval == '*') 
        {
            treeToString(root->left, result);
            result += "*"; 
            treeToString(root->right, result);
        }
        else if (root->nodeval == '+') 
        {
            treeToString(root->left, result);
            result += "+"; 
            treeToString(root->right, result);
        }
        else if (root->nodeval == '~') 
        {
            result += "~";
            treeToString(root->right, result);
        }
        else
        {
            result += root->nodeval; 
        }
    }

    /**
     * @brief Checks if all clauses in the CNF formula are valid (tautologies)
     * @return bool True if all clauses are tautologies, false otherwise
     * 
     * A clause is a tautology if it contains both a literal and its negation
     * (e.g., p + ~p), making it always true.
     */
    bool checkvalid()
    {
        int no_of_clauses = 0;
        string s = "";
        treeToString(roottree,s);
        string token;
        stringstream ss(s);
        while(getline(ss,token,'*'))
        {
            no_of_clauses++;
        }
        if(validclause_no() == no_of_clauses) return true;
        else return false;
    }

    /**
     * @brief Counts the number of non-tautological clauses
     * @return int Number of clauses that are not tautologies
     */
    int nonvalidclause_no()
    {
        int no_of_clauses = 0;
        string s = "";
        treeToString(roottree,s);
        string token;
        stringstream ss(s);
        while(getline(ss,token,'*'))
        {
            no_of_clauses++;
        }
        return no_of_clauses-validclause_no();
    }

    /**
     * @brief Counts the number of tautological clauses in the CNF formula
     * @return int Number of clauses containing both a literal and its negation
     * 
     * Algorithm:
     * - Splits formula by '*' to get individual clauses
     * - For each clause, tracks normal and negated literals
     * - If any literal appears in both forms, clause is tautological
     */
    int validclause_no()
    {
        int ans = 0;
        string s = "";
        treeToString(roottree,s);
        cout<<endl;
        string token;
        stringstream ss(s);
        while(getline(ss,token,'*'))
        {
            bool negated = false;
            set<char> snormal;
            set <char> snegated;
            for(auto i: token)
            {
                if(i == '+') continue;

                else if(i == '~')
                {
                    negated = true;
                    continue;
                }
                else
                {
                    if(negated == true) 
                    {
                        negated = false;
                        snegated.insert(i);
                    }
                    else
                    {
                        snormal.insert(i);
                    }
                    
                }
            }
            for (auto &lit : snormal)
            {
                if (snegated.find(lit) != snegated.end())
                {
                    ans++;
                    break;
                }
            
            }
        }
        return ans;
    }
};

/**
 * @brief Entry point for Logic Formula Tester and CNF Converter/Validator.
 *
 * This main function allows the user to:
 *  - Test propositional logic formulas by constructing their parse trees,
 *    calculating their height, and generating truth tables.
 *  - Convert logical formulas into Conjunctive Normal Form (CNF)
 *    and validate whether the resulting CNF is tautological/valid or not.
 *
 * The user is prompted to select between two modes:
 *  1. Logic Formula Tester
 *  2. CNF Converter and Validator
 *
 * @note This function depends on the following components:
 *  - Parsetree: for representing and printing logic formula trees.
 *  - @ref valuecomputer : for calculating truth values and truth tables.
 *  - CNFConverter: for converting formulas to CNF and checking validity.
 */

int main()
{

    int crlt;
    cout<< "Enter 1 for logic formula tester  "<<endl<< "Enter 2 for CNF Converter and validator "<< endl;
    cin >> crlt;



    if(crlt == 1)
    { 
        cout<< "Enter number of formulas for test"<<endl;
        int n;
        cin >> n;
        for(int j = 0; j<n ; j++)
        {
            string i;
            cout<< "Enter Formula: ";
            cin>> i;
            Parsetree t(infixtoprefix(i));
            t.printtree();
            cout<< endl;
            cout<< "maxheight is: "<<t.height()<<endl;

            map<char,int> mpp;
            valuecomputer vc(t.root);
            set <char> aset = vc.calc_no_of_atoms();
            for(char i : aset)
            {
                int val;
                cout << "Enter Value(0/1) of "<< i<< ": ";
                cin >>val;
                mpp[i] = val;
            }
            cout << "The Truth value for the given assignment is = "<<vc.computetruth(mpp)<<endl;
            cout<< endl;
            cout<< "Truth Table"<<endl;
            vc.computealltruth();
            cout<<endl;
        }
    }
    
    if(crlt == 2)
    {
        cout<< "Enter number of formulas for test"<<endl;
        int n;
        cin >> n;

        for(int j = 0; j<n ; j++)
        {
            string i;
            cout<< "Enter Formula: ";
            cin>> i;
            Parsetree t(infixtoprefix(i));
            t.printtree();
            cout<< endl;
            CNFConverter converter(i);
            converter.cnf();
            t.printtree(converter.roottree);
            cout<< endl;
            if(converter.checkvalid()) cout << "Valid Formula";
            else cout << "Not Valid Formula";
            cout<<endl;
            cout<<"No of valid clause = " << converter.validclause_no()<<endl;
            cout<<"No of invalid clause= "<< converter.nonvalidclause_no()<<endl;
            cout<<endl;
        }
    }


        return 0;
}