#include <iostream>
#include "infixtoprefix.hpp"
#include <algorithm>
#include <ctype.h>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <bitset>
#include <cstdlib>

using namespace std;

class Treenode
{
    public :
        char nodeval;
        Treenode* left;
        Treenode* right;
        int truthvalue = 0;

        Treenode(char nv, Treenode* l, Treenode* r)
        {
            this->left = l;
            this->right = r;
            this->nodeval = nv;
        }
};


class Parsetree
{
    public:
        int i;
        Treenode* root = nullptr;
        Parsetree(string s)
        {
            i = 0;
            if(s == "") return;
            else root = buildTree(s,i);
        }

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

        // Inorder Traversal
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

        void printtree()
        {
            printtree(root);
        }

        //Compute height of tree
        int height()
        {
            return height(root);
        }
        int height(Treenode* node)
        {
            if(node == nullptr) return 0;
            else
            {
                return 1+max(height(node->left),height(node->right));
            }
        }

};

class valuecomputer
{
    public :
    Treenode* root;
    map<char,int> atomvals;
    map<char,int> atomvals_temp;

    valuecomputer(Treenode* r,map<char,int> d)
    {
        root = r;
        atomvals = d;
        atomvals_temp = d;
    }

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

    int computetruth()
    {
        assignatoms(root,atomvals);
        return computetruth(root);
    }
    int computetruth(map<char,int> atomval)
    {
        assignatoms(root,atomval);
        return computetruth(root);
    }

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

    void computealltruth()
    {
        vector<char> v;
        for(auto k: atomvals)
        {
            v.push_back(k.first);
            cout << k.first;
        }
        cout<<" Truth value"<<endl;
        int n = 1<< atomvals.size();
        for(int i = 0; i<n; i++)
        {
            bitset<64> bits(i);
            string bitstring = bits.to_string().substr(64-atomvals.size());
            cout << bitstring << " ";
            for(int j = 0; j<atomvals.size();j++)
            {
                atomvals_temp[v[j]] = bitstring[j]-'0';
            } 
            cout << computetruth(atomvals_temp)<<endl;

        }
    }
};



// int main()
// {
//     Parsetree t(infixtoprefix("(a>b)*(q>r)"));
//     t.printtree();
//     cout<< endl;
//     cout<< t.height();
//     map<char,int> mpp = {{'a',1},{'b',0},{'q',1},{'r',0}};
//     valuecomputer vc(t.root,mpp);
//     cout<< endl;
//     cout << vc.computetruth()<<endl;
//     vc.computealltruth();
//     return 0;
// }




class CNFConverter{

    public:
    Treenode* roottree;
    string prefix_string;
    int valid_clause = 0;

    CNFConverter(string s)
    {
        prefix_string = infixtoprefix(s);
        roottree = (new Parsetree(prefix_string))->root;
    }

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

    Treenode* impfree()
    {
        impfree(roottree);
    }

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

    void nnf()
    {
        nnf(roottree);
    }


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

    void cnf()
    {
        
        impfree();
        nnf();
        roottree = CNF(roottree);
    }

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

    int validclause_no()
    {
        int ans = 0;
        string s = "";
        treeToString(roottree,s);
        cout<<s;
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

int main()
{

    vector<string> tests = {
        "(p>q)*(q>q)"
    };

    for(string i : tests)
    {
        Parsetree t(infixtoprefix(i));
        t.printtree();
        cout<< endl;
        CNFConverter converter(i);
        converter.cnf();
        t.printtree(converter.roottree);
        cout<< endl;
        if(converter.checkvalid()) cout << "Valid";
        else cout << "Not Valid";
        cout<<endl;
        cout<<"No of valid clause = " << converter.validclause_no()<<endl;
        cout<<endl;
    }
    return 0;
}

