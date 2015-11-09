#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<set>
#include<fstream>
using namespace std;
string path = "/Users/amrgadalla/Desktop/Digital Design/Quine-McCluskey/Quine-McCluskey/Text.txt";
ifstream in(path);
int vars;
vector<int> minterms, dontcare;		//the minterms covered by the function and the don't-care terms
struct cell {
	string name; //ex:10_100_10
	vector<int> min_covered;  //the minterms that are covered by this cell...for example 01_0 coveres 4 and 6
	int catg;				//the category ...the number of ones in the cell(if it is in the first column)
	int col;				//the column that the cell belongs to
	bool prime;    //if the cell found a match(a number with only one diffrenent bit)in the next category then it is not prime(false)
    
	cell(int minterm_num, int var_num) :prime(true), catg(0), col(0)
	{
		name.resize(16);
		for (int i = 0;i < var_num;i++)
		{
			if (minterm_num &(1 << i))
			{
				name[var_num - i - 1] = '1';
				catg++;
			}
			else
				name[var_num - i - 1] = '0';
		}
		min_covered.push_back(minterm_num);
	}
	cell() :col(0), prime(1), catg(0), name("") {}
	cell merge_cells(cell& x)
    /*use to combine two cells that have only one different bit...it returns a new cell object that carries
     the name,minterms covered by it...and it also sets the prime field in the two comined cells
     to not primes (false)
     note:please check if the two cells can be checked first ...otherwise it throws an exception
     ex: if(y.can_merge(x))
     y.merge_cells(x);
     */
	{
		if (can_merge(x))
		{
			prime = false;
			x.prime = false;
			cell new_cell(*this, x);
			return new_cell;
		}
		else
			throw "please check they can merge first using (can_merge) before using this method\n";
	}		//
	bool can_merge(cell& x) {				//checks if they are from the same column and from consequent categories and if the differece is only one bit
		if (only_one_diff(x) == -1)
			return false;
		return true;
	}
    
private:
	int only_one_diff(cell& x) {
		int n = 0, index = -1;
		if (abs(catg - x.catg) == 1 && col == x.col)
		{
			for (int i = 0;i < name.size();i++)
			{
				if (name[i] != x.name[i])
				{
					n++;
					index = i;
				}
				if (n > 1)
					return -1;
			}
			return index;
		}
		return -1;
	}
	cell(cell& x, cell &y) {
		name = x.name;
		name[y.only_one_diff(x)] = '_';
		min_covered.resize(y.min_covered.size() + x.min_covered.size());
		merge(y.min_covered.begin(), y.min_covered.end(), x.min_covered.begin(), x.min_covered.end(), min_covered.begin());
		sort(min_covered.begin(), min_covered.end());
		prime = true;
		catg = min(y.catg, x.catg);
		col = x.col;
	}
};
cell table[17][17][320];//table[column][category][row];
int col_size[17][17] = {};
vector<cell> primeI;
void the_table(vector<cell>& start, int col)
{
	
	if (start.size() == 0)
		return;
	vector<cell> nextcolumn;
	
	for (int i = 0;i < start.size();i++)
		table[col][start[i].catg][col_size[col][start[i].catg]++] = start[i];
    
	for (int k = 0;k < vars;k++)
		for (int i = 0; i < col_size[col][k];i++)
			for (int j = 0;j < col_size[col][k + 1];j++)
			{
				if (table[col][k][i].can_merge(table[col][k + 1][j]))
				{
					cell temp = table[col][k][i].merge_cells(table[col][k + 1][j]);
					nextcolumn.push_back(temp);
				}
			}
	for (int k = 0; k <= vars-col;k++)
	{
		for (int i = 0; i < col_size[col][k];i++)
			if (table[col][k][i].prime)
			{
				bool add = true;
				for (int j = 0; j < primeI.size();j++)
				{
					if (table[col][k][i].name == primeI[j].name)
					{
						add = false;
						break;
					}
				}
				if(add)
					primeI.push_back(table[col][k][i]);
			}
        
	}
	the_table(nextcolumn, col + 1);
}


void Gadalla( int vars )
{
    struct mins{
        vector<string> mint_num;
        int cov_by;
        int numb = 0;
        bool inside = false;
    };
    string Alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ", Function = "F = ", EPIs = "The EPI's are ";
    mins *mm;
    vector<string> EPI;
    int msize = minterms.size();
    int count = 0;
    for( int m = 0; m < msize; m++)            //excluding the don't cares
    {
        if(minterms[m] < minterms[m-1])
        {
            count = msize - m;
            break;
        }
    }
    mm = new mins[minterms.size()];
    for( int a = 0; a < count; a++)
    {
        minterms.pop_back();
    }
   
    for( int a = 0; a < minterms.size(); a++)
    {
        for (int i = 0; i < primeI.size();i++)
            {
            for ( int j  = 0; j < primeI[i].min_covered.size(); j++)
                {
                    if ( minterms[a] == primeI[i].min_covered[j])
                    {
                        mm[a].cov_by = minterms[a];
                        mm[a].mint_num.push_back(primeI[i].name);
                        mm[a].numb = mm[a].numb + 1;
                    }
                }
            }
    }
    
    for ( int i = 0; i < minterms.size(); i++)
    {
        if(mm[i].numb == 1 )
        {
            EPI.push_back(mm[i].mint_num[0]);
        }
    }
    
    
    for ( int a = 0; a < EPI.size(); a++)
        {
        for ( int i = 0; i < minterms.size(); i++)
            {
                for( int j =0; j < mm[i].mint_num.size(); j++)
                {
            if(mm[i].mint_num[j] == EPI[a])
                mm[i].inside = true;
                }
            }
        }
    for ( int i = 0; i < primeI.size(); i++)
    {
        for( int j =0; j < vars; j++)
        {
            if(primeI[i].name.at(j) == '0')
                Function = Function + Alpha.at(j) + "'";
            if(primeI[i].name.at(j) == '1')
                Function = Function + Alpha.at(j);
        }
        Function = Function + "+";
    }
        Function.at(Function.length()-1) = ' ';
    
    
    for ( int i = 0; i < EPI.size(); i++)
    {
        for( int j =0; j < vars; j++)
        {
            if(EPI[i].at(j) == '0')
                EPIs = EPIs + Alpha.at(j) + "'";
            if(EPI[i].at(j) == '1')
                EPIs = EPIs + Alpha.at(j);
        }
        EPIs = EPIs + " & ";
    }
    EPIs.at(EPIs.length()-2) = ' ';
    cout << "The functions is" << endl << Function << endl << EPIs << endl;
    
    
	//given the prime implicants as a vector of a vector<int>... we need to get the essintial ones and the rest of primes used.
}
int main()
{
	cout << "How many variables for this function?\nplease enter a number (Max. 16 variables) and press enter\n";
	in >> vars;
	int min_terms_num;
	in >> min_terms_num;
	int l;
	vector<cell> start;
	for (int i = 0; i < min_terms_num; i++)
	{
		in >> l;
		minterms.push_back(l);
		cell n(l, vars);
		start.push_back(n);
	}
	the_table(start, 0);
	Gadalla( vars );
	return 0;
}
