#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<set>
#include<fstream>
using namespace std;
ifstream in("Text.txt");
int vars;
vector<int> minterms, dontcare;		//the minterms covered by the function and the don't-care terms
struct cell {
	string name; //ex:10_100_10
	vector<int> min_covered;  //the minterms that is covered by this cell...for example 01_0 coveres 4 and 6
	int catg;				//the category ...the number of ones in the cell(if it is in the first cilumn)
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
	bool operator<(const cell x) const
	{
		return col < x.col;
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
	for (int k = 0; k < vars -col;k++)
	{
		for (int i = 0; i < col_size[col][k];i++)
			if (table[col][k][i].prime)
			{
				for (int i = 0;i<)
				primeI.push_back(table[col][k][i]);
				
	}
	the_table(nextcolumn, col + 1);
}
void gadallah()
{
	vector<cell> primes(primeI.begin(), primeI.end());
	for (set<cell>::iterator i = primeI.begin();i != primeI.end();i++)
		cout <<	i->name << endl;
	for (int i = 0; i < primes.size();i++)
		cout << primes[i].name << endl;
	//given the prime implicants as a vector of a vector<int>... we need to get the essintial ones and the rest of pimes used.
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
	gadallah();
	system("pause");
	return 0;
}