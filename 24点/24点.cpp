#include <iostream>
#include <ctime>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
using namespace std;
inline double cal(double x1, double x2, char op);
bool find24(double a, double b, double c, double d);
string ExpressionProcessing(string& s,char op1,char op2,char op3);
int main()
{
	srand(time(nullptr));
	double nums[4];
	for(auto i=0;i < 10;++i)
	{
		for (auto& num : nums)
			num = static_cast<double>(rand() % 12 + 1);

		for (auto n : nums)
			cout << n << " ";
		cout << "\n";

		sort(nums, nums + 4);
		while (!find24(nums[0], nums[1], nums[2], nums[3]) && next_permutation(nums, nums + 4));
	}
}
inline double cal(const double x1, const double x2, const char op)
{
	switch (op)
	{
	case 1:return x1 + x2;
	case 2:return x1 - x2;
	case 3:return x1 * x2;
	case 4:return x1 / x2;
	default:return 0;
	}
}
bool find24(const double a, const double b, const double c, const double d)
{
	static string ops = " +-*/";
	for (char op1 = 1; op1 <= 4; ++op1)
		for (char op2 = 1; op2 <= 4; ++op2)
			for (char op3 = 1; op3 <= 4; ++op3)
			{
				auto r1 = cal(cal(cal(a, b, op1), c, op2), d, op3);// ( ( a b ) c ) d	
				auto r2 = cal(cal(a, b, op1), cal(c, d, op3), op2);// ( a b ) ( c d )   
				auto r3 = cal(cal(a, cal(b, c, op2), op1), d, op3);// ( a ( b c ) ) d	
				auto r4 = cal(a, cal(cal(b, c, op2), d, op3), op1);// a ( ( b c ) d )   
				auto r5 = cal(a, cal(b, cal(c, d, op3), op2), op1);// a ( b ( c d ) )
				multimap<double, string>
					result{ {r1,"((%.0lf%c%.0lf)%c%.0lf)%c%.0lf"} ,{r2,"(%.0lf%c%.0lf)%c(%.0lf%c%.0lf)"},
							{r3,"(%.0lf%c(%.0lf%c%.0lf))%c%.0lf"},{r4,"%.0lf%c((%.0lf%c%.0lf)%c%.0lf)"},
							{r5,"%.0lf%c(%.0lf%c(%.0lf%c%.0lf))"} };
				const auto it = result.find(24);
				/*for (const auto& r : result)
				{
					printf((r.second + " = %.0lf\n").c_str(), a, ops[op1], b, ops[op2], c, ops[op3], d, r.first);
				}*/
				if (it != result.end())
				{
					auto expression = it->second;
					expression = ExpressionProcessing(expression, op1, op2, op3);
					printf((expression + " = %.0lf\n\n").c_str(), a, ops[op1], b, ops[op2], c, ops[op3], d, it->first);
					return true;
				}
				
				result.clear();
			}
	return false;
}
string ExpressionProcessing(string& s, char op1, char op2, char op3)
{
	static char priority[5] = { 0,0,0,1,1 };
	if(s=="((%.0lf%c%.0lf)%c%.0lf)%c%.0lf")
	{
		if(priority[op2] <= priority[op1])
		{
			s[1] = '#';
			s[14] = '#';
		}
		if(priority[op3] <= priority[op2])
		{
			s[0] = '#';
			s[22] = '#';
		}
	}else if(s=="(%.0lf%c%.0lf)%c(%.0lf%c%.0lf)")
	{
		if (priority[op2] <= priority[op1])
		{
			s[0] = '#';
			s[13] = '#';
		}
		if(priority[op3] >= priority[op2])
		{
			s[16] = '#';
			s[29] = '#';
		}
	}else if(s=="(%.0lf%c(%.0lf%c%.0lf))%c%.0lf")
	{
		if(priority[op2] >= priority[op1])
		{
			s[8] = '#';
			s[21] = '#';
		}
		if(priority[op3] <= priority[op1])
		{
			s[0] = '#';
			s[22] = '#';
		}
	}else if(s== "%.0lf%c((%.0lf%c%.0lf)%c%.0lf)")
	{
		if(priority[op2] >= priority[op3])
		{
			s[8] = '#';
			s[21] = '#';
		}
		if(priority[op3] >= priority[op1])
		{
			s[7] = '#';
			s[29] = '#';
		}
	}else if(s== "%.0lf%c(%.0lf%c(%.0lf%c%.0lf))")
	{
		if(priority[op3] >= priority[op2])
		{
			s[15] = '#';
			s[28] = '#';
		}
		if(priority[op2] >= priority[op1])
		{
			s[7] = '#';
			s[29] = '#';
		}
	}
	string s1;
	for(auto c:s)
	{
		if (c != '#')
			s1 += c;
	}
	return s1;
}